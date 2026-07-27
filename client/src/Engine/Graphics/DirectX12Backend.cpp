#include "stdh.h"

#include <d3d12.h>
#include <d3d9on12.h>
#include <cstring>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/DirectX12Backend.h>
#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12InteropTextureManager.h>
#include <Engine/Graphics/DirectX12NativeRenderer.h>
#include <Engine/Graphics/DirectX12PresentationManager.h>
#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12UploadManager.h>
#include <Engine/Testing/ClientTestAutomation.h>

namespace
{
	typedef HRESULT(WINAPI* D3D12CreateDeviceProc)(
		IUnknown*,
		D3D_FEATURE_LEVEL,
		REFIID,
		void**);

	typedef IDirect3D9*(WINAPI* Direct3DCreate9On12Proc)(
		UINT,
		D3D9ON12_ARGS*,
		UINT);

	void ReportDeviceFailure(
		ID3D12Device* pDevice,
		const char* pStage,
		HRESULT operationResult)
	{
		const HRESULT removalReason = pDevice != NULL
			? pDevice->GetDeviceRemovedReason()
			: E_POINTER;
		CPrintF(
			"DX12 error de dispositivo en %s: operacion=0x%08X, "
			"remocion=0x%08X.\n",
			pStage != NULL ? pStage : "etapa desconocida",
			static_cast<unsigned int>(operationResult),
			static_cast<unsigned int>(removalReason));
	}

	DirectX12DrawPortValidationMode ReadDrawPortValidationMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_UI_COMPARE",
			value,
			sizeof(value));
		if (length == 0 || length >= sizeof(value))
			return DX12_DRAWPORT_VALIDATION_SHADOW;
		if (_stricmp(value, "overlay") == 0
			|| strcmp(value, "1") == 0)
			return DX12_DRAWPORT_VALIDATION_UI_OVERLAY;
		if (_stricmp(value, "split") == 0
			|| strcmp(value, "2") == 0)
			return DX12_DRAWPORT_VALIDATION_UI_SPLIT;
		if (_stricmp(value, "replace") == 0
			|| strcmp(value, "3") == 0)
			return DX12_DRAWPORT_VALIDATION_UI_REPLACE;
		return DX12_DRAWPORT_VALIDATION_SHADOW;
	}

	bool ReadKeepLegacyUiMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_UI_KEEP_LEGACY",
			value,
			sizeof(value));
		return length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
	}

	bool ReadRigidLitReplacementMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_REPLACE_RIGID_LIT",
			value,
			sizeof(value));
		return length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
	}

	bool ReadFull3DReplacementMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_REPLACE_ALL",
			value,
			sizeof(value));
		const bool requested = length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
		if (!requested)
			return false;
		char gatePath[MAX_PATH] = "";
		const DWORD gateLength = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_REPLACE_ALL_GATE",
			gatePath,
			sizeof(gatePath));
		return gateLength == 0
			|| (gateLength < sizeof(gatePath)
				&& GetFileAttributesA(gatePath)
					!= INVALID_FILE_ATTRIBUTES);
	}

	void AppendValidationLogLine(const char* pMessage)
	{
		if (pMessage == NULL || pMessage[0] == '\0')
			return;
		char path[MAX_PATH] = "";
		const DWORD pathLength = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_VALIDATION_LOG",
			path,
			sizeof(path));
		if (pathLength == 0 || pathLength >= sizeof(path))
			return;

		HANDLE hFile = CreateFileA(
			path,
			FILE_APPEND_DATA,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return;

		DWORD written = 0;
		WriteFile(
			hFile,
			pMessage,
			static_cast<DWORD>(strlen(pMessage)),
			&written,
			NULL);
		CloseHandle(hFile);
	}

	void AppendLegacy3DValidationLog(
		UINT capturedDrawCount,
		UINT rejectedDrawCount,
		UINT triangleCount,
		const UINT* pReasons,
		UINT reasonCount,
		UINT64 topVertexShaderFingerprint,
		UINT topVertexShaderDrawCount,
		UINT topVertexShaderTriangleCount,
		bool overlay)
	{
		char message[512];
		const int messageLength = _snprintf_s(
			message,
			sizeof(message),
			_TRUNCATE,
			"DX12 3D %s: %u envios capturados, "
			"%u rechazados, %u triangulos; "
			"motivos streams=%u, VS=%u, PS=%u, proyecto=%u, "
			"pasadas=%u, arrays=%u, limite=%u, indice=%u, estado=%u, "
			"destinoAuxiliar=%u, fixedTransparente=%u, fixedClip=%u, "
			"clipInvalido=%u; "
			"familiaVS=%016llX, enviosVS=%u, triangulosVS=%u.\r\n",
			overlay ? "overlay" : "sombra",
			capturedDrawCount,
			rejectedDrawCount,
			triangleCount,
			reasonCount > 0 ? pReasons[0] : 0,
			reasonCount > 1 ? pReasons[1] : 0,
			reasonCount > 2 ? pReasons[2] : 0,
			reasonCount > 3 ? pReasons[3] : 0,
			reasonCount > 4 ? pReasons[4] : 0,
			reasonCount > 5 ? pReasons[5] : 0,
			reasonCount > 6 ? pReasons[6] : 0,
			reasonCount > 7 ? pReasons[7] : 0,
			reasonCount > 8 ? pReasons[8] : 0,
			reasonCount > 9 ? pReasons[9] : 0,
			reasonCount > 10 ? pReasons[10] : 0,
			reasonCount > 11 ? pReasons[11] : 0,
			reasonCount > 12 ? pReasons[12] : 0,
			static_cast<unsigned long long>(topVertexShaderFingerprint),
			topVertexShaderDrawCount,
			topVertexShaderTriangleCount);
		if (messageLength > 0)
			AppendValidationLogLine(message);
	}
}

CDirectX12Backend::CDirectX12Backend()
	: m_hD3D12Module(NULL)
	, m_pDevice(NULL)
	, m_pGraphicsQueue(NULL)
	, m_pCommandList(NULL)
	, m_pFence(NULL)
	, m_pRenderTargets(NULL)
	, m_pUploadManager(NULL)
	, m_pResourceDescriptors(NULL)
	, m_pRenderTargetDescriptors(NULL)
	, m_pSamplerDescriptors(NULL)
	, m_pNativeRenderer(NULL)
	, m_pInteropTextures(NULL)
	, m_pPresentation(NULL)
	, m_pDevice9(NULL)
	, m_pLegacyPresentationTargetIdentity(NULL)
	, m_hPresentationWindow(NULL)
	, m_hFenceEvent(NULL)
	, m_nextFenceValue(1)
	, m_currentFrame(0)
	, m_frameOpen(false)
	, m_hasPresentedNativeUiFrame(false)
	, m_initialPresentationDeferralReported(false)
	, m_drawPortValidationMode(DX12_DRAWPORT_VALIDATION_SHADOW)
	, m_lastReportedUiPrimitiveCount(static_cast<UINT>(-1))
	, m_lastReportedUiSegmentCount(static_cast<UINT>(-1))
	, m_lastReportedUiBarrierCount(static_cast<UINT>(-1))
	, m_nextUiSegmentToSubmit(0)
	, m_currentSubmission(0)
	, m_partialSubmissionCapacityReported(false)
	, m_uiScopeDepth(0)
	, m_offscreenDrawPortDepth(0)
	, m_nativeOffscreenTexture(DX12_INVALID_RENDER_TEXTURE)
	, m_nativeOffscreenClearPending(false)
	, m_suppressedLegacyDrawCount(0)
	, m_fallbackLegacyDrawCount(0)
	, m_lastReportedSuppressedLegacyDrawCount(static_cast<UINT>(-1))
	, m_lastReportedFallbackLegacyDrawCount(static_cast<UINT>(-1))
	, m_legacy3DDepthAvailable(false)
	, m_suppressedLegacy3DDrawCount(0)
	, m_fallbackLegacy3DDrawCount(0)
	, m_lastReportedSuppressedLegacy3DDrawCount(static_cast<UINT>(-1))
	, m_lastReportedFallbackLegacy3DDrawCount(static_cast<UINT>(-1))
	, m_lastReportedLegacy3DCapturedDrawCount(static_cast<UINT>(-1))
	, m_lastReportedLegacy3DRejectedDrawCount(static_cast<UINT>(-1))
	, m_lastReportedLegacy3DTriangleCount(static_cast<UINT>(-1))
	, m_lastReportedLegacy3DTopVertexShaderFingerprint(
		static_cast<UINT64>(-1))
	, m_lastReportedLegacy3DTopVertexShaderDrawCount(
		static_cast<UINT>(-1))
	, m_lastReportedLegacy3DTopVertexShaderTriangleCount(
		static_cast<UINT>(-1))
{
	ZeroMemory(
		m_nativeOffscreenClearColor,
		sizeof(m_nativeOffscreenClearColor));
	for (UINT iReason = 0; iReason < 13; ++iReason)
	{
		m_lastReportedLegacy3DRejectionReasons[iReason] =
			static_cast<UINT>(-1);
	}
	for (UINT iFrame = 0; iFrame < FRAME_COUNT; ++iFrame)
	{
		for (UINT iSubmission = 0;
			iSubmission < MAX_SUBMISSIONS_PER_FRAME;
			++iSubmission)
		{
			m_aFrames[iFrame].apCommandAllocators[iSubmission] = NULL;
		}
		m_aFrames[iFrame].fenceValue = 0;
	}
}

CDirectX12Backend::~CDirectX12Backend()
{
	Shutdown();
}

bool CDirectX12Backend::Initialize(HMODULE hD3D9Module, IDirect3D9** ppD3D9)
{
	if (hD3D9Module == NULL || ppD3D9 == NULL)
		return false;

	Shutdown();
	*ppD3D9 = NULL;

	m_hD3D12Module = LoadLibrary(TEXT("d3d12.dll"));
	if (m_hD3D12Module == NULL)
		return false;

	D3D12CreateDeviceProc pCreateDevice =
		reinterpret_cast<D3D12CreateDeviceProc>(
			GetProcAddress(m_hD3D12Module, "D3D12CreateDevice"));
	Direct3DCreate9On12Proc pCreate9On12 =
		reinterpret_cast<Direct3DCreate9On12Proc>(
			GetProcAddress(hD3D9Module, "Direct3DCreate9On12"));

	if (pCreateDevice == NULL || pCreate9On12 == NULL)
	{
		Shutdown();
		return false;
	}

	HRESULT hr = pCreateDevice(
		NULL,
		D3D_FEATURE_LEVEL_11_0,
		__uuidof(ID3D12Device),
		reinterpret_cast<void**>(&m_pDevice));
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC queueDesc;
	ZeroMemory(&queueDesc, sizeof(queueDesc));
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	hr = m_pDevice->CreateCommandQueue(
		&queueDesc,
		__uuidof(ID3D12CommandQueue),
		reinterpret_cast<void**>(&m_pGraphicsQueue));
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	if (!CreateFrameResources())
	{
		Shutdown();
		return false;
	}

	m_pRenderTargets = new CDirectX12RenderTargetManager;
	if (m_pRenderTargets == NULL
		|| !m_pRenderTargets->Initialize(m_pDevice, m_pGraphicsQueue))
	{
		Shutdown();
		return false;
	}

	m_pUploadManager = new CDirectX12UploadManager;
	if (m_pUploadManager == NULL
		|| !m_pUploadManager->Initialize(m_pDevice, FRAME_COUNT))
	{
		Shutdown();
		return false;
	}

	m_pResourceDescriptors = new CDirectX12DescriptorHeap;
	if (m_pResourceDescriptors == NULL
		|| !m_pResourceDescriptors->Initialize(
			m_pDevice,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			8192,
			true))
	{
		Shutdown();
		return false;
	}

	m_pSamplerDescriptors = new CDirectX12DescriptorHeap;
	if (m_pSamplerDescriptors == NULL
		|| !m_pSamplerDescriptors->Initialize(
			m_pDevice,
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			256,
			true))
	{
		Shutdown();
		return false;
	}

	m_pRenderTargetDescriptors = new CDirectX12DescriptorHeap;
	if (m_pRenderTargetDescriptors == NULL
		|| !m_pRenderTargetDescriptors->Initialize(
			m_pDevice,
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			1024,
			false))
	{
		Shutdown();
		return false;
	}

	m_pNativeRenderer = new CDirectX12NativeRenderer;
	if (m_pNativeRenderer == NULL
		|| !m_pNativeRenderer->Initialize(m_pDevice))
	{
		Shutdown();
		return false;
	}

	m_pInteropTextures = new CDirectX12InteropTextureManager;
	if (m_pInteropTextures == NULL
		|| !m_pInteropTextures->Initialize(
			m_pDevice,
			m_pGraphicsQueue,
			m_pResourceDescriptors,
			m_pRenderTargetDescriptors))
	{
		Shutdown();
		return false;
	}

	m_pPresentation = new CDirectX12PresentationManager;
	if (m_pPresentation == NULL
		|| !m_pPresentation->Initialize(m_pDevice, m_pGraphicsQueue))
	{
		Shutdown();
		return false;
	}

	D3D9ON12_ARGS args;
	ZeroMemory(&args, sizeof(args));
	args.Enable9On12 = TRUE;
	args.pD3D12Device = m_pDevice;
	args.ppD3D12Queues[0] = m_pGraphicsQueue;
	args.NumQueues = 1;
	args.NodeMask = 0;

	*ppD3D9 = pCreate9On12(D3D_SDK_VERSION, &args, 1);
	if (*ppD3D9 == NULL)
	{
		Shutdown();
		return false;
	}

	return true;
}

void CDirectX12Backend::Shutdown()
{
	if (m_pGraphicsQueue != NULL && m_pFence != NULL)
	{
		if (m_frameOpen)
			EndFrame();
		WaitForGpu();
	}
	if (m_pLegacyPresentationTargetIdentity != NULL)
	{
		m_pLegacyPresentationTargetIdentity->Release();
		m_pLegacyPresentationTargetIdentity = NULL;
	}

	if (m_pNativeRenderer != NULL)
	{
		m_pNativeRenderer->Shutdown();
		delete m_pNativeRenderer;
		m_pNativeRenderer = NULL;
	}
	if (m_pInteropTextures != NULL)
	{
		m_pInteropTextures->Shutdown();
		delete m_pInteropTextures;
		m_pInteropTextures = NULL;
	}
	if (m_pPresentation != NULL)
	{
		m_pPresentation->Shutdown();
		delete m_pPresentation;
		m_pPresentation = NULL;
	}

	ReleaseFrameResources();

	if (m_pRenderTargets != NULL)
	{
		m_pRenderTargets->Shutdown();
		delete m_pRenderTargets;
		m_pRenderTargets = NULL;
	}

	if (m_pUploadManager != NULL)
	{
		m_pUploadManager->Shutdown();
		delete m_pUploadManager;
		m_pUploadManager = NULL;
	}

	if (m_pSamplerDescriptors != NULL)
	{
		m_pSamplerDescriptors->Shutdown();
		delete m_pSamplerDescriptors;
		m_pSamplerDescriptors = NULL;
	}

	if (m_pRenderTargetDescriptors != NULL)
	{
		m_pRenderTargetDescriptors->Shutdown();
		delete m_pRenderTargetDescriptors;
		m_pRenderTargetDescriptors = NULL;
	}

	if (m_pResourceDescriptors != NULL)
	{
		m_pResourceDescriptors->Shutdown();
		delete m_pResourceDescriptors;
		m_pResourceDescriptors = NULL;
	}

	if (m_pGraphicsQueue != NULL)
	{
		m_pGraphicsQueue->Release();
		m_pGraphicsQueue = NULL;
	}

	if (m_pDevice9 != NULL)
	{
		m_pDevice9->Release();
		m_pDevice9 = NULL;
	}

	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if (m_hD3D12Module != NULL)
	{
		FreeLibrary(m_hD3D12Module);
		m_hD3D12Module = NULL;
	}
	m_hasPresentedNativeUiFrame = false;
	m_initialPresentationDeferralReported = false;
}

bool CDirectX12Backend::CreateFrameResources()
{
	for (UINT iFrame = 0; iFrame < FRAME_COUNT; ++iFrame)
	{
		for (UINT iSubmission = 0;
			iSubmission < MAX_SUBMISSIONS_PER_FRAME;
			++iSubmission)
		{
			HRESULT hr = m_pDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				__uuidof(ID3D12CommandAllocator),
				reinterpret_cast<void**>(
					&m_aFrames[iFrame].apCommandAllocators[iSubmission]));
			if (FAILED(hr))
				return false;
		}

		m_aFrames[iFrame].fenceValue = 0;
	}

	HRESULT hr = m_pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_aFrames[0].apCommandAllocators[0],
		NULL,
		__uuidof(ID3D12GraphicsCommandList),
		reinterpret_cast<void**>(&m_pCommandList));
	if (FAILED(hr))
		return false;

	// Las listas se crean abiertas; BeginFrame la reinicia al comenzar a grabar.
	hr = m_pCommandList->Close();
	if (FAILED(hr))
		return false;

	hr = m_pDevice->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		__uuidof(ID3D12Fence),
		reinterpret_cast<void**>(&m_pFence));
	if (FAILED(hr))
		return false;

	m_hFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hFenceEvent == NULL)
		return false;

	m_pGraphicsQueue->SetName(L"LastChaos D3D12 Graphics Queue");
	m_pCommandList->SetName(L"LastChaos D3D12 Graphics Command List");
	m_pFence->SetName(L"LastChaos D3D12 Frame Fence");

	m_nextFenceValue = 1;
	m_currentFrame = 0;
	m_currentSubmission = 0;
	m_frameOpen = false;
	return true;
}

void CDirectX12Backend::ReleaseFrameResources()
{
	if (m_pCommandList != NULL)
	{
		m_pCommandList->Release();
		m_pCommandList = NULL;
	}

	for (UINT iFrame = 0; iFrame < FRAME_COUNT; ++iFrame)
	{
		for (UINT iSubmission = 0;
			iSubmission < MAX_SUBMISSIONS_PER_FRAME;
			++iSubmission)
		{
			ID3D12CommandAllocator*& pAllocator =
				m_aFrames[iFrame].apCommandAllocators[iSubmission];
			if (pAllocator != NULL)
			{
				pAllocator->Release();
				pAllocator = NULL;
			}
		}
		m_aFrames[iFrame].fenceValue = 0;
	}

	if (m_pFence != NULL)
	{
		m_pFence->Release();
		m_pFence = NULL;
	}

	if (m_hFenceEvent != NULL)
	{
		CloseHandle(m_hFenceEvent);
		m_hFenceEvent = NULL;
	}

	m_nextFenceValue = 1;
	m_currentFrame = 0;
	m_currentSubmission = 0;
	m_frameOpen = false;
}

bool CDirectX12Backend::BeginFrame()
{
	CClientTestAutomation::Instance().Tick();
	if (m_frameOpen || m_pCommandList == NULL || m_pFence == NULL)
		return false;

	FrameContext& frame = m_aFrames[m_currentFrame];
	if (!WaitForFence(frame.fenceValue))
		return false;

	m_currentSubmission = 0;
	HRESULT hr = frame.apCommandAllocators[0]->Reset();
	if (FAILED(hr))
		return false;

	hr = m_pCommandList->Reset(
		frame.apCommandAllocators[0],
		NULL);
	if (FAILED(hr))
		return false;

	if (m_pUploadManager == NULL
		|| !m_pUploadManager->BeginFrame(m_currentFrame))
		return false;
	if (m_pInteropTextures == NULL
		|| !m_pInteropTextures->BeginFrame(m_currentFrame))
		return false;

	ID3D12DescriptorHeap* descriptorHeaps[] = {
		m_pResourceDescriptors->GetHeap(),
		m_pSamplerDescriptors->GetHeap()
	};
	m_pCommandList->SetDescriptorHeaps(
		sizeof(descriptorHeaps) / sizeof(descriptorHeaps[0]),
		descriptorHeaps);

	if (m_pNativeRenderer == NULL)
		return false;
	m_pNativeRenderer->BeginFrame(m_currentFrame);
	m_drawPortValidationMode = ReadDrawPortValidationMode();
	m_hPresentationWindow = NULL;
	m_nextUiSegmentToSubmit = 0;
	m_partialSubmissionCapacityReported = false;
	m_uiScopeDepth = 0;
	m_offscreenDrawPortDepth = 0;
	m_nativeOffscreenTexture = DX12_INVALID_RENDER_TEXTURE;
	m_nativeOffscreenClearPending = false;
	m_suppressedLegacyDrawCount = 0;
	m_fallbackLegacyDrawCount = 0;
	m_legacy3DDepthAvailable = HasLegacy3DDepthSurface();
	m_suppressedLegacy3DDrawCount = 0;
	m_fallbackLegacy3DDrawCount = 0;
	m_frameOpen = true;
	return true;
}

bool CDirectX12Backend::EndFrame()
{
	if (!m_frameOpen || m_pCommandList == NULL)
		return false;

	const bool hasRenderTarget =
		m_pRenderTargets != NULL && m_pRenderTargets->IsAcquired();
	const bool keepLegacyUi = ReadKeepLegacyUiMode();
	const bool submitLegacy3D =
		!ReadFull3DReplacementMode()
		|| (m_pNativeRenderer != NULL
			&& m_pNativeRenderer->HasPendingLegacy3DDraws());
	bool nativeDrawSucceeded = true;
	if (hasRenderTarget && !keepLegacyUi)
	{
		nativeDrawSucceeded = m_pNativeRenderer != NULL
			&& m_pNativeRenderer->RenderValidationPass(
				m_pCommandList,
				m_pRenderTargets,
				m_pUploadManager,
				m_pResourceDescriptors,
				m_pSamplerDescriptors,
				m_pInteropTextures,
				m_drawPortValidationMode,
				m_nextUiSegmentToSubmit,
				static_cast<UINT>(-1),
				submitLegacy3D);
	}
	if (m_drawPortValidationMode != DX12_DRAWPORT_VALIDATION_SHADOW
		&& m_pNativeRenderer != NULL)
	{
		const UINT primitiveCount =
			m_pNativeRenderer->GetUiPrimitiveCount();
		const UINT segmentCount =
			m_pNativeRenderer->GetUiSegmentCount();
		const UINT barrierCount =
			m_pNativeRenderer->GetUiBarrierCount();
		if (primitiveCount != m_lastReportedUiPrimitiveCount
			|| segmentCount != m_lastReportedUiSegmentCount
			|| barrierCount != m_lastReportedUiBarrierCount)
		{
			CPrintF(
				"DX12 UI: %u primitivas, %u segmentos, %u barreras D3D9.\n",
				primitiveCount,
				segmentCount,
				barrierCount);
			m_lastReportedUiPrimitiveCount = primitiveCount;
			m_lastReportedUiSegmentCount = segmentCount;
			m_lastReportedUiBarrierCount = barrierCount;
		}
	}
	if (m_drawPortValidationMode
			== DX12_DRAWPORT_VALIDATION_UI_REPLACE
		&& (m_suppressedLegacyDrawCount
				!= m_lastReportedSuppressedLegacyDrawCount
			|| m_fallbackLegacyDrawCount
				!= m_lastReportedFallbackLegacyDrawCount))
	{
		CPrintF(
			"DX12 UI replace: %u envios D3D9 omitidos, "
			"%u fallbacks por captura.\n",
			m_suppressedLegacyDrawCount,
			m_fallbackLegacyDrawCount);
		m_lastReportedSuppressedLegacyDrawCount =
			m_suppressedLegacyDrawCount;
		m_lastReportedFallbackLegacyDrawCount =
			m_fallbackLegacyDrawCount;
	}
	if ((ReadRigidLitReplacementMode() || ReadFull3DReplacementMode())
		&& (m_suppressedLegacy3DDrawCount
				!= m_lastReportedSuppressedLegacy3DDrawCount
			|| m_fallbackLegacy3DDrawCount
				!= m_lastReportedFallbackLegacy3DDrawCount))
	{
		CPrintF(
			"DX12 3D replace: %u envios D3D9 omitidos, "
			"%u fallbacks por captura/profundidad.\n",
			m_suppressedLegacy3DDrawCount,
			m_fallbackLegacy3DDrawCount);
		m_lastReportedSuppressedLegacy3DDrawCount =
			m_suppressedLegacy3DDrawCount;
		m_lastReportedFallbackLegacy3DDrawCount =
			m_fallbackLegacy3DDrawCount;
	}
	if (m_pNativeRenderer != NULL)
	{
		enum { LEGACY_3D_REJECTION_REASON_COUNT = 13 };
		const UINT capturedDrawCount =
			m_pNativeRenderer->GetLegacy3DCapturedDrawCount();
		const UINT rejectedDrawCount =
			m_pNativeRenderer->GetLegacy3DRejectedDrawCount();
		const UINT triangleCount =
			m_pNativeRenderer->GetLegacy3DCapturedTriangleCount();
		const UINT64 topVertexShaderFingerprint =
			m_pNativeRenderer->GetLegacy3DTopVertexShaderFingerprint();
		const UINT topVertexShaderDrawCount =
			m_pNativeRenderer->GetLegacy3DTopVertexShaderDrawCount();
		const UINT topVertexShaderTriangleCount =
			m_pNativeRenderer->GetLegacy3DTopVertexShaderTriangleCount();
		UINT rejectionReasons[LEGACY_3D_REJECTION_REASON_COUNT];
		for (UINT iReason = 0;
			iReason < LEGACY_3D_REJECTION_REASON_COUNT;
			++iReason)
		{
			rejectionReasons[iReason] =
				m_pNativeRenderer->GetLegacy3DRejectedReasonCount(
					iReason);
		}
		const bool overlay =
			m_pNativeRenderer->IsLegacy3DOverlayComparisonEnabled();
		bool rejectionReasonsChanged = false;
		for (UINT iReason = 0;
			iReason < LEGACY_3D_REJECTION_REASON_COUNT;
			++iReason)
		{
			if (rejectionReasons[iReason]
				!= m_lastReportedLegacy3DRejectionReasons[iReason])
			{
				rejectionReasonsChanged = true;
				break;
			}
		}
		const bool hasLegacy3DActivity =
			capturedDrawCount > 0
			|| rejectedDrawCount > 0
			|| triangleCount > 0;
		if (hasLegacy3DActivity
			&& (capturedDrawCount
				!= m_lastReportedLegacy3DCapturedDrawCount
			|| rejectedDrawCount
				!= m_lastReportedLegacy3DRejectedDrawCount
			|| triangleCount != m_lastReportedLegacy3DTriangleCount
			|| topVertexShaderFingerprint
				!= m_lastReportedLegacy3DTopVertexShaderFingerprint
			|| topVertexShaderDrawCount
				!= m_lastReportedLegacy3DTopVertexShaderDrawCount
			|| topVertexShaderTriangleCount
				!= m_lastReportedLegacy3DTopVertexShaderTriangleCount
			|| rejectionReasonsChanged))
		{
			CPrintF(
				"DX12 3D %s: %u envios capturados, "
				"%u rechazados, %u triangulos.\n",
				overlay ? "overlay" : "sombra",
				capturedDrawCount,
				rejectedDrawCount,
				triangleCount);
			AppendLegacy3DValidationLog(
				capturedDrawCount,
				rejectedDrawCount,
				triangleCount,
				rejectionReasons,
				LEGACY_3D_REJECTION_REASON_COUNT,
				topVertexShaderFingerprint,
				topVertexShaderDrawCount,
				topVertexShaderTriangleCount,
				overlay);
			m_lastReportedLegacy3DCapturedDrawCount =
				capturedDrawCount;
			m_lastReportedLegacy3DRejectedDrawCount =
				rejectedDrawCount;
			m_lastReportedLegacy3DTriangleCount = triangleCount;
			m_lastReportedLegacy3DTopVertexShaderFingerprint =
				topVertexShaderFingerprint;
			m_lastReportedLegacy3DTopVertexShaderDrawCount =
				topVertexShaderDrawCount;
			m_lastReportedLegacy3DTopVertexShaderTriangleCount =
				topVertexShaderTriangleCount;
			for (UINT iReason = 0;
				iReason < LEGACY_3D_REJECTION_REASON_COUNT;
				++iReason)
			{
				m_lastReportedLegacy3DRejectionReasons[iReason] =
					rejectionReasons[iReason];
			}
		}
	}
	if (m_pInteropTextures == NULL
		|| !m_pInteropTextures->PrepareForSubmission(m_pCommandList))
		return false;

	const bool hasUiReadyForPresentation =
		HasUiReadyForInitialPresentation();
	const bool nativePresentationReady =
		m_hasPresentedNativeUiFrame || hasUiReadyForPresentation;
	if (m_drawPortValidationMode == DX12_DRAWPORT_VALIDATION_UI_REPLACE
		&& hasRenderTarget && !nativePresentationReady
		&& !m_initialPresentationDeferralReported)
	{
		CPrintF(
			"DX12 presentacion: se difiere el primer frame "
			"hasta capturar la UI completa.\n");
		m_initialPresentationDeferralReported = true;
	}
	const bool nativePresentQueued =
		m_drawPortValidationMode == DX12_DRAWPORT_VALIDATION_UI_REPLACE
		&& hasRenderTarget
		&& nativePresentationReady
		&& m_pPresentation != NULL
		&& m_pPresentation->QueueFrame(
			m_pCommandList,
			m_pRenderTargets->GetCurrentResource(),
			m_hPresentationWindow);
	if (nativePresentQueued && hasUiReadyForPresentation)
		m_hasPresentedNativeUiFrame = true;
	if (hasRenderTarget && !nativePresentQueued
		&& !m_pRenderTargets->PrepareForSubmission(m_pCommandList))
		return false;

	m_pUploadManager->EndFrame();

	HRESULT hr = m_pCommandList->Close();
	if (FAILED(hr))
	{
		ReportDeviceFailure(m_pDevice, "cierre de lista", hr);
		m_frameOpen = false;
		return false;
	}

	ID3D12CommandList* commandLists[] = { m_pCommandList };
	m_pGraphicsQueue->ExecuteCommandLists(1, commandLists);

	const UINT64 fenceValue = m_nextFenceValue++;
	hr = m_pGraphicsQueue->Signal(m_pFence, fenceValue);
	if (FAILED(hr))
	{
		ReportDeviceFailure(m_pDevice, "senal de fence", hr);
		m_frameOpen = false;
		return false;
	}

	m_aFrames[m_currentFrame].fenceValue = fenceValue;
	m_currentFrame = (m_currentFrame + 1) % FRAME_COUNT;
	m_frameOpen = false;

	if (!WaitForFence(fenceValue))
		return false;

	bool returnSucceeded = true;
	returnSucceeded = m_pInteropTextures != NULL
		&& m_pInteropTextures->ReturnToD3D9(m_pFence, fenceValue);
	if (hasRenderTarget)
	{
		returnSucceeded =
			m_pRenderTargets->ReturnToD3D9(m_pFence, fenceValue)
			&& returnSucceeded;
	}
	const bool presentationSucceeded =
		!nativePresentQueued || m_pPresentation->Present();
	return nativeDrawSucceeded && returnSucceeded
		&& presentationSucceeded;
}

bool CDirectX12Backend::SubmitUiSegmentsThrough(
	UINT maximumSegment,
	bool submitLegacy3D)
{
	if (!m_frameOpen || m_pDevice9 == NULL || m_pNativeRenderer == NULL
		|| m_pRenderTargets == NULL || m_pInteropTextures == NULL
		|| maximumSegment < m_nextUiSegmentToSubmit)
		return false;
	const bool hasUiCommands = m_pNativeRenderer->HasUiCommands(
		m_nextUiSegmentToSubmit,
		maximumSegment);
	if (!hasUiCommands && !submitLegacy3D)
	{
		m_nextUiSegmentToSubmit = maximumSegment + 1;
		return true;
	}
	if (m_currentSubmission + 1 >= MAX_SUBMISSIONS_PER_FRAME)
	{
		if (!m_partialSubmissionCapacityReported)
		{
			CPrintF(
				"DX12 UI: limite de envios parciales; "
				"los segmentos restantes se enviaran al final.\n");
			m_partialSubmissionCapacityReported = true;
		}
		return true;
	}

	IDirect3DSurface9* pRenderTarget9 = NULL;
	HRESULT hr = m_pDevice9->GetRenderTarget(0, &pRenderTarget9);
	if (FAILED(hr) || pRenderTarget9 == NULL)
		return false;

	hr = m_pDevice9->EndScene();
	if (FAILED(hr))
	{
		pRenderTarget9->Release();
		return false;
	}

	bool succeeded = AcquireRenderTarget(pRenderTarget9);
	pRenderTarget9->Release();
	if (succeeded)
	{
		succeeded = m_pNativeRenderer->RenderValidationPass(
			m_pCommandList,
			m_pRenderTargets,
			m_pUploadManager,
			m_pResourceDescriptors,
			m_pSamplerDescriptors,
			m_pInteropTextures,
			m_drawPortValidationMode,
			m_nextUiSegmentToSubmit,
			maximumSegment,
			submitLegacy3D);
	}
	if (succeeded)
		succeeded = m_pInteropTextures->PrepareForSubmission(m_pCommandList);
	if (succeeded)
		succeeded = m_pRenderTargets->PrepareForSubmission(m_pCommandList);
	if (succeeded)
		succeeded = SUCCEEDED(m_pCommandList->Close());

	UINT64 fenceValue = 0;
	if (succeeded)
	{
		ID3D12CommandList* commandLists[] = { m_pCommandList };
		m_pGraphicsQueue->ExecuteCommandLists(1, commandLists);
		fenceValue = m_nextFenceValue++;
		succeeded = SUCCEEDED(
			m_pGraphicsQueue->Signal(m_pFence, fenceValue));
		if (succeeded)
			m_aFrames[m_currentFrame].fenceValue = fenceValue;
	}
	if (succeeded)
	{
		succeeded = m_pInteropTextures->ReturnToD3D9(
			m_pFence,
			fenceValue,
			false);
		succeeded = m_pRenderTargets->ReturnToD3D9(
			m_pFence,
			fenceValue)
			&& succeeded;
	}
	if (succeeded)
		succeeded = AdvanceOpenCommandList();

	const HRESULT beginSceneResult = m_pDevice9->BeginScene();
	succeeded = SUCCEEDED(beginSceneResult) && succeeded;
	// Un envío anticipado que contiene solamente el mundo no consume el
	// segmento actual: la UI que comienza después todavía se graba en él.
	if (succeeded && hasUiCommands)
		m_nextUiSegmentToSubmit = maximumSegment + 1;
	return succeeded;
}

bool CDirectX12Backend::SubmitPendingLegacy3DForCurrentTarget(
	const char* pTransition)
{
	if (!m_frameOpen || m_pNativeRenderer == NULL
		|| !ReadFull3DReplacementMode()
		|| !m_pNativeRenderer->HasPendingLegacy3DDraws())
		return true;

	const UINT currentSegment =
		m_pNativeRenderer->GetCurrentSegment();
	if (SubmitUiSegmentsThrough(currentSegment, true))
		return true;

	CPrintF(
		"DX12 3D: fallo el envio antes de %s; se conserva D3D9.\n",
		pTransition != NULL ? pTransition : "cambiar el destino");
	return false;
}

bool CDirectX12Backend::HasUiReadyForInitialPresentation() const
{
	return m_pNativeRenderer != NULL
		&& m_pNativeRenderer->HasUiCommands(
			0,
			static_cast<UINT>(-1));
}

bool CDirectX12Backend::AdvanceOpenCommandList()
{
	if (m_currentSubmission + 1 >= MAX_SUBMISSIONS_PER_FRAME)
		return false;
	++m_currentSubmission;
	FrameContext& frame = m_aFrames[m_currentFrame];
	ID3D12CommandAllocator* pAllocator =
		frame.apCommandAllocators[m_currentSubmission];
	if (FAILED(pAllocator->Reset())
		|| FAILED(m_pCommandList->Reset(
			pAllocator,
			NULL)))
		return false;

	ID3D12DescriptorHeap* descriptorHeaps[] = {
		m_pResourceDescriptors->GetHeap(),
		m_pSamplerDescriptors->GetHeap()
	};
	m_pCommandList->SetDescriptorHeaps(
		sizeof(descriptorHeaps) / sizeof(descriptorHeaps[0]),
		descriptorHeaps);
	return true;
}

bool CDirectX12Backend::WaitForGpu()
{
	if (m_pGraphicsQueue == NULL || m_pFence == NULL)
		return false;

	const UINT64 fenceValue = m_nextFenceValue++;
	HRESULT hr = m_pGraphicsQueue->Signal(m_pFence, fenceValue);
	if (FAILED(hr))
	{
		ReportDeviceFailure(m_pDevice, "drenaje de GPU", hr);
		return false;
	}

	return WaitForFence(fenceValue);
}

bool CDirectX12Backend::WaitForFence(UINT64 fenceValue)
{
	if (fenceValue == 0)
		return true;

	const UINT64 completedValue = m_pFence->GetCompletedValue();
	// D3D12 devuelve UINT64_MAX cuando el dispositivo fue removido.
	if (completedValue == UINT64_MAX)
	{
		ReportDeviceFailure(
			m_pDevice,
			"consulta de fence",
			DXGI_ERROR_DEVICE_REMOVED);
		return false;
	}
	if (completedValue >= fenceValue)
		return true;

	HRESULT hr = m_pFence->SetEventOnCompletion(fenceValue, m_hFenceEvent);
	if (FAILED(hr))
	{
		ReportDeviceFailure(m_pDevice, "evento de fence", hr);
		return false;
	}

	const DWORD waitResult = WaitForSingleObject(m_hFenceEvent, 10000);
	if (waitResult != WAIT_OBJECT_0)
	{
		const HRESULT waitError = waitResult == WAIT_TIMEOUT
			? HRESULT_FROM_WIN32(ERROR_TIMEOUT)
			: HRESULT_FROM_WIN32(GetLastError());
		ReportDeviceFailure(m_pDevice, "espera de fence", waitError);
		return false;
	}
	return true;
}

bool CDirectX12Backend::AttachD3D9Device(IDirect3DDevice9* pDevice9)
{
	if (pDevice9 == NULL || m_pRenderTargets == NULL
		|| m_pInteropTextures == NULL)
		return false;
	if (!m_pRenderTargets->AttachD3D9Device(pDevice9)
		|| !m_pInteropTextures->AttachD3D9Device(pDevice9))
		return false;
	if (m_pDevice9 != NULL)
		m_pDevice9->Release();
	m_pDevice9 = pDevice9;
	m_pDevice9->AddRef();
	m_legacy3DDepthAvailable = HasLegacy3DDepthSurface();
	return true;
}

void CDirectX12Backend::SetLegacyPresentationRenderTarget(
	IDirect3DSurface9* pSurface9)
{
	IUnknown* pIdentity = NULL;
	if (pSurface9 != NULL)
		pSurface9->QueryInterface(IID_IUnknown, (void**)&pIdentity);
	if (m_pLegacyPresentationTargetIdentity != NULL)
		m_pLegacyPresentationTargetIdentity->Release();
	m_pLegacyPresentationTargetIdentity = pIdentity;
}

bool CDirectX12Backend::HasLegacyPresentationRenderTarget() const
{
	return m_pLegacyPresentationTargetIdentity != NULL;
}

DirectX12LegacyRenderTargetKind
CDirectX12Backend::ClassifyLegacyRenderTarget(
	IDirect3DDevice9* pDevice9) const
{
	if (pDevice9 == NULL || m_pLegacyPresentationTargetIdentity == NULL)
		return DX12_LEGACY_RENDER_TARGET_UNKNOWN;

	IDirect3DSurface9* pSurface9 = NULL;
	if (FAILED(pDevice9->GetRenderTarget(0, &pSurface9))
		|| pSurface9 == NULL)
		return DX12_LEGACY_RENDER_TARGET_UNKNOWN;

	IUnknown* pIdentity = NULL;
	pSurface9->QueryInterface(IID_IUnknown, (void**)&pIdentity);
	pSurface9->Release();
	if (pIdentity == NULL)
		return DX12_LEGACY_RENDER_TARGET_UNKNOWN;

	const bool presentation =
		pIdentity == m_pLegacyPresentationTargetIdentity;
	pIdentity->Release();
	return presentation
		? DX12_LEGACY_RENDER_TARGET_PRESENTATION
		: DX12_LEGACY_RENDER_TARGET_OFFSCREEN;
}

bool CDirectX12Backend::HasLegacy3DDepthSurface() const
{
	if (m_pDevice9 == NULL)
		return false;

	IDirect3DSurface9* pDepthSurface9 = NULL;
	const HRESULT hr =
		m_pDevice9->GetDepthStencilSurface(&pDepthSurface9);
	const bool available =
		SUCCEEDED(hr) && pDepthSurface9 != NULL;
	if (pDepthSurface9 != NULL)
		pDepthSurface9->Release();
	return available;
}

void CDirectX12Backend::ForgetLegacyTexture(IDirect3DTexture9* pTexture9)
{
	if (pTexture9 == NULL)
		return;
	if (m_pNativeRenderer != NULL)
		m_pNativeRenderer->ForgetTexture(pTexture9);
	if (m_pInteropTextures != NULL)
		m_pInteropTextures->ForgetTexture(pTexture9);
}

bool CDirectX12Backend::CreateNativeOffscreenTexture(
	IDirect3DTexture9* pTexture9,
	UINT width,
	UINT height,
	INT legacyFormat,
	DirectX12RenderTextureHandle* pHandle)
{
	return ReadFull3DReplacementMode()
		&& m_pInteropTextures != NULL
		&& m_pInteropTextures->CreateRenderTarget(
			pTexture9,
			width,
			height,
			static_cast<D3DFORMAT>(legacyFormat),
			pHandle);
}

void CDirectX12Backend::DestroyNativeOffscreenTexture(
	DirectX12RenderTextureHandle handle)
{
	if (handle == DX12_INVALID_RENDER_TEXTURE
		|| m_pInteropTextures == NULL)
		return;
	if (m_nativeOffscreenTexture == handle)
		EndNativeOffscreenTexture();
	m_pInteropTextures->DestroyRenderTarget(handle);
}

bool CDirectX12Backend::BeginNativeOffscreenTexture(
	DirectX12RenderTextureHandle handle)
{
	if (!ReadFull3DReplacementMode()
		|| m_pInteropTextures == NULL
		|| m_pInteropTextures->FindRenderTarget(handle) == NULL
		|| m_nativeOffscreenTexture != DX12_INVALID_RENDER_TEXTURE)
		return false;

	m_nativeOffscreenTexture = handle;
	m_nativeOffscreenClearPending = false;
	static bool nativeColorReported = false;
	if (!nativeColorReported)
	{
		CPrintF(
			"DX12 offscreen: color auxiliar administrado por "
			"un recurso RTV/SRV nativo.\n");
		AppendValidationLogLine(
			"DX12 offscreen: color auxiliar RTV/SRV nativo activo.\n");
		nativeColorReported = true;
	}
	return true;
}

void CDirectX12Backend::ClearNativeOffscreenTexture(ULONG color)
{
	if (m_nativeOffscreenTexture == DX12_INVALID_RENDER_TEXTURE)
		return;
	const FLOAT colorScale = 1.0f / 255.0f;
	m_nativeOffscreenClearColor[0] =
		static_cast<FLOAT>((color >> 16) & 0xFF) * colorScale;
	m_nativeOffscreenClearColor[1] =
		static_cast<FLOAT>((color >> 8) & 0xFF) * colorScale;
	m_nativeOffscreenClearColor[2] =
		static_cast<FLOAT>(color & 0xFF) * colorScale;
	m_nativeOffscreenClearColor[3] =
		static_cast<FLOAT>((color >> 24) & 0xFF) * colorScale;
	m_nativeOffscreenClearPending = true;
}

void CDirectX12Backend::EndNativeOffscreenTexture()
{
	m_nativeOffscreenTexture = DX12_INVALID_RENDER_TEXTURE;
	m_nativeOffscreenClearPending = false;
}

bool CDirectX12Backend::RenderNativeBloom(
	DirectX12RenderTextureHandle sourceTexture,
	DirectX12RenderTextureHandle filterTexture0,
	DirectX12RenderTextureHandle filterTexture1)
{
	if (!m_frameOpen || !ReadFull3DReplacementMode()
		|| m_pDevice9 == NULL || m_pNativeRenderer == NULL
		|| m_pRenderTargets == NULL || m_pInteropTextures == NULL
		|| sourceTexture == DX12_INVALID_RENDER_TEXTURE
		|| filterTexture0 == DX12_INVALID_RENDER_TEXTURE
		|| filterTexture1 == DX12_INVALID_RENDER_TEXTURE
		|| m_currentSubmission + 1 >= MAX_SUBMISSIONS_PER_FRAME)
		return false;
	CDirectX12Texture* pSourceTexture =
		m_pInteropTextures->FindRenderTarget(sourceTexture);
	CDirectX12Texture* pFilterTexture0 =
		m_pInteropTextures->FindRenderTarget(filterTexture0);
	CDirectX12Texture* pFilterTexture1 =
		m_pInteropTextures->FindRenderTarget(filterTexture1);
	if (pSourceTexture == NULL || pFilterTexture0 == NULL
		|| pFilterTexture1 == NULL)
		return false;
	if (!SubmitPendingLegacy3DForCurrentTarget("aplicar bloom nativo")
		|| m_currentSubmission + 1 >= MAX_SUBMISSIONS_PER_FRAME)
		return false;

	IDirect3DSurface9* pRenderTarget9 = NULL;
	HRESULT hr = m_pDevice9->GetRenderTarget(0, &pRenderTarget9);
	if (FAILED(hr) || pRenderTarget9 == NULL)
		return false;
	hr = m_pDevice9->EndScene();
	if (FAILED(hr))
	{
		pRenderTarget9->Release();
		return false;
	}

	bool succeeded = AcquireRenderTarget(pRenderTarget9);
	pRenderTarget9->Release();
	if (succeeded)
	{
		succeeded = m_pNativeRenderer->RenderBloom(
			m_pCommandList,
			m_pRenderTargets,
			m_pUploadManager,
			m_pResourceDescriptors,
			m_pSamplerDescriptors,
			pSourceTexture,
			pFilterTexture0,
			pFilterTexture1);
	}
	if (succeeded)
		succeeded = m_pInteropTextures->PrepareForSubmission(m_pCommandList);
	if (succeeded)
		succeeded = m_pRenderTargets->PrepareForSubmission(m_pCommandList);
	if (succeeded)
		succeeded = SUCCEEDED(m_pCommandList->Close());

	UINT64 fenceValue = 0;
	if (succeeded)
	{
		ID3D12CommandList* commandLists[] = { m_pCommandList };
		m_pGraphicsQueue->ExecuteCommandLists(1, commandLists);
		fenceValue = m_nextFenceValue++;
		succeeded = SUCCEEDED(
			m_pGraphicsQueue->Signal(m_pFence, fenceValue));
		if (succeeded)
			m_aFrames[m_currentFrame].fenceValue = fenceValue;
	}
	if (succeeded)
	{
		succeeded = m_pInteropTextures->ReturnToD3D9(
			m_pFence,
			fenceValue,
			false);
		succeeded = m_pRenderTargets->ReturnToD3D9(
			m_pFence,
			fenceValue)
			&& succeeded;
	}
	if (succeeded)
		succeeded = AdvanceOpenCommandList();

	const HRESULT beginSceneResult = m_pDevice9->BeginScene();
	succeeded = SUCCEEDED(beginSceneResult) && succeeded;
	if (succeeded)
	{
		static bool nativeBloomReported = false;
		if (!nativeBloomReported)
		{
			CPrintF(
				"DX12 postproceso: reduccion, blur y composicion "
				"de bloom nativos activos.\n");
			AppendValidationLogLine(
				"DX12 postproceso: bloom completamente nativo activo.\n");
			nativeBloomReported = true;
		}
	}
	return succeeded;
}

bool CDirectX12Backend::CopyLegacySurfaceRegion(
	IDirect3DSurface9* pSource9,
	const RECT& sourceRect,
	IDirect3DSurface9* pDestination9,
	UINT destinationX,
	UINT destinationY)
{
	if (!m_frameOpen || !ReadFull3DReplacementMode()
		|| m_pDevice9 == NULL || m_pGraphicsQueue == NULL
		|| m_pCommandList == NULL || m_pFence == NULL
		|| pSource9 == NULL || pDestination9 == NULL
		|| pSource9 == pDestination9
		|| sourceRect.left < 0 || sourceRect.top < 0
		|| sourceRect.right <= sourceRect.left
		|| sourceRect.bottom <= sourceRect.top
		|| m_currentSubmission + 1 >= MAX_SUBMISSIONS_PER_FRAME)
		return false;

	if (!SubmitPendingLegacy3DForCurrentTarget(
			"copiar una region para postproceso"))
		return false;
	if (m_currentSubmission + 1 >= MAX_SUBMISSIONS_PER_FRAME)
		return false;

	IDirect3DDevice9On12* pDevice9On12 = NULL;
	ID3D12Resource* pSource12 = NULL;
	ID3D12Resource* pDestination12 = NULL;
	bool sceneEnded = false;
	bool resourcesReturned = false;
	bool commandListClosed = false;
	bool succeeded = SUCCEEDED(m_pDevice9->EndScene());
	sceneEnded = succeeded;
	if (succeeded)
	{
		succeeded = SUCCEEDED(m_pDevice9->QueryInterface(
			__uuidof(IDirect3DDevice9On12),
			reinterpret_cast<void**>(&pDevice9On12)));
	}
	if (succeeded)
	{
		succeeded = SUCCEEDED(pDevice9On12->UnwrapUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(pSource9),
			m_pGraphicsQueue,
			__uuidof(ID3D12Resource),
			reinterpret_cast<void**>(&pSource12)));
	}
	if (succeeded)
	{
		succeeded = SUCCEEDED(pDevice9On12->UnwrapUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(pDestination9),
			m_pGraphicsQueue,
			__uuidof(ID3D12Resource),
			reinterpret_cast<void**>(&pDestination12)));
	}

	if (succeeded)
	{
		const D3D12_RESOURCE_DESC sourceDesc = pSource12->GetDesc();
		const D3D12_RESOURCE_DESC destinationDesc =
			pDestination12->GetDesc();
		const UINT copyWidth = static_cast<UINT>(
			sourceRect.right - sourceRect.left);
		const UINT copyHeight = static_cast<UINT>(
			sourceRect.bottom - sourceRect.top);
		succeeded =
			sourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D
			&& destinationDesc.Dimension
				== D3D12_RESOURCE_DIMENSION_TEXTURE2D
			&& sourceDesc.SampleDesc.Count == 1
			&& destinationDesc.SampleDesc.Count == 1
			&& sourceDesc.Format == destinationDesc.Format
			&& static_cast<UINT>(sourceRect.right) <= sourceDesc.Width
			&& static_cast<UINT>(sourceRect.bottom) <= sourceDesc.Height
			&& destinationX + copyWidth <= destinationDesc.Width
			&& destinationY + copyHeight <= destinationDesc.Height;
	}

	if (succeeded)
	{
		D3D12_RESOURCE_BARRIER barriers[2];
		ZeroMemory(barriers, sizeof(barriers));
		barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barriers[0].Transition.pResource = pSource12;
		barriers[0].Transition.Subresource =
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
		barriers[0].Transition.StateAfter =
			D3D12_RESOURCE_STATE_COPY_SOURCE;
		barriers[1] = barriers[0];
		barriers[1].Transition.pResource = pDestination12;
		barriers[1].Transition.StateAfter =
			D3D12_RESOURCE_STATE_COPY_DEST;
		m_pCommandList->ResourceBarrier(2, barriers);

		D3D12_TEXTURE_COPY_LOCATION sourceLocation;
		ZeroMemory(&sourceLocation, sizeof(sourceLocation));
		sourceLocation.pResource = pSource12;
		sourceLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		D3D12_TEXTURE_COPY_LOCATION destinationLocation;
		ZeroMemory(&destinationLocation, sizeof(destinationLocation));
		destinationLocation.pResource = pDestination12;
		destinationLocation.Type =
			D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		D3D12_BOX sourceBox;
		sourceBox.left = static_cast<UINT>(sourceRect.left);
		sourceBox.top = static_cast<UINT>(sourceRect.top);
		sourceBox.front = 0;
		sourceBox.right = static_cast<UINT>(sourceRect.right);
		sourceBox.bottom = static_cast<UINT>(sourceRect.bottom);
		sourceBox.back = 1;
		m_pCommandList->CopyTextureRegion(
			&destinationLocation,
			destinationX,
			destinationY,
			0,
			&sourceLocation,
			&sourceBox);

		barriers[0].Transition.StateBefore =
			D3D12_RESOURCE_STATE_COPY_SOURCE;
		barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
		barriers[1].Transition.StateBefore =
			D3D12_RESOURCE_STATE_COPY_DEST;
		barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
		m_pCommandList->ResourceBarrier(2, barriers);
		commandListClosed = SUCCEEDED(m_pCommandList->Close());
		succeeded = commandListClosed;
	}

	UINT64 fenceValue = 0;
	if (succeeded)
	{
		ID3D12CommandList* commandLists[] = { m_pCommandList };
		m_pGraphicsQueue->ExecuteCommandLists(1, commandLists);
		fenceValue = m_nextFenceValue++;
		succeeded = SUCCEEDED(
			m_pGraphicsQueue->Signal(m_pFence, fenceValue));
		if (succeeded)
			m_aFrames[m_currentFrame].fenceValue = fenceValue;
	}
	if (succeeded)
	{
		ID3D12Fence* fences[] = { m_pFence };
		UINT64 fenceValues[] = { fenceValue };
		const HRESULT destinationResult =
			pDevice9On12->ReturnUnderlyingResource(
				reinterpret_cast<IDirect3DResource9*>(pDestination9),
				1,
				fenceValues,
				fences);
		const HRESULT sourceResult =
			pDevice9On12->ReturnUnderlyingResource(
				reinterpret_cast<IDirect3DResource9*>(pSource9),
				1,
				fenceValues,
				fences);
		resourcesReturned = true;
		succeeded = SUCCEEDED(destinationResult)
			&& SUCCEEDED(sourceResult);
	}
	if (!resourcesReturned && pDevice9On12 != NULL)
	{
		if (pDestination12 != NULL)
			pDevice9On12->ReturnUnderlyingResource(
				reinterpret_cast<IDirect3DResource9*>(pDestination9),
				0,
				NULL,
				NULL);
		if (pSource12 != NULL)
			pDevice9On12->ReturnUnderlyingResource(
				reinterpret_cast<IDirect3DResource9*>(pSource9),
				0,
				NULL,
				NULL);
	}
	if (pDestination12 != NULL)
		pDestination12->Release();
	if (pSource12 != NULL)
		pSource12->Release();
	if (pDevice9On12 != NULL)
		pDevice9On12->Release();

	if (commandListClosed)
		succeeded = AdvanceOpenCommandList() && succeeded;
	if (sceneEnded)
		succeeded = SUCCEEDED(m_pDevice9->BeginScene()) && succeeded;
	if (succeeded)
	{
		static bool nativeCopyReported = false;
		if (!nativeCopyReported)
		{
			CPrintF(
				"DX12 postproceso: copia de superficie nativa activa.\n");
			AppendValidationLogLine(
				"DX12 postproceso: copia de superficie nativa activa.\n");
			nativeCopyReported = true;
		}
	}
	return succeeded;
}

bool CDirectX12Backend::AcquireRenderTarget(
	IDirect3DSurface9* pSurface9,
	HWND hPresentationWindow)
{
	if (m_nativeOffscreenTexture != DX12_INVALID_RENDER_TEXTURE
		&& m_pInteropTextures != NULL
		&& m_pRenderTargets != NULL)
	{
		CDirectX12Texture* pNativeTexture =
			m_pInteropTextures->FindRenderTarget(
				m_nativeOffscreenTexture);
		const bool acquired = pNativeTexture != NULL
			&& m_pRenderTargets->AcquireNative(
				pNativeTexture,
				m_pCommandList,
				m_currentFrame,
				m_currentSubmission,
				m_nativeOffscreenClearPending,
				m_nativeOffscreenClearColor);
		if (acquired)
			m_nativeOffscreenClearPending = false;
		return acquired;
	}

	IDirect3DSurface9* pDepthSurface9 = NULL;
	if (m_pDevice9 != NULL)
		m_pDevice9->GetDepthStencilSurface(&pDepthSurface9);
	if (!m_frameOpen || m_pRenderTargets == NULL
		|| !m_pRenderTargets->Acquire(
			pSurface9,
			pDepthSurface9,
			m_pCommandList,
			m_currentFrame,
			m_currentSubmission))
	{
		if (pDepthSurface9 != NULL)
			pDepthSurface9->Release();
		m_legacy3DDepthAvailable = false;
		return false;
	}
	if (pDepthSurface9 != NULL)
		pDepthSurface9->Release();
	m_legacy3DDepthAvailable =
		m_pRenderTargets->HasAcquiredDepth()
		|| HasLegacy3DDepthSurface();
	m_hPresentationWindow = hPresentationWindow;
	return true;
}

bool CDirectX12Backend::QueueDrawPortPoint(
	FLOAT x,
	FLOAT y,
	FLOAT radius,
	ULONG color,
	LONG scissorLeft,
	LONG scissorTop,
	LONG scissorRight,
	LONG scissorBottom)
{
	return m_frameOpen && m_offscreenDrawPortDepth == 0
		&& m_pNativeRenderer != NULL
		&& m_pNativeRenderer->QueueDrawPortPoint(
			x,
			y,
			radius,
			color,
			scissorLeft,
			scissorTop,
			scissorRight,
			scissorBottom);
}

bool CDirectX12Backend::BeginDrawPortScope(
	DirectX12DrawPortScope scope)
{
	// El inicio de la UI principal es el límite estable entre el mundo y el
	// HUD. Envía la geometría DX12 antes de que D3D9 o DrawPort compongan la
	// interfaz, evitando que el terreno la cubra al cerrar el frame.
	if (scope == DX12_DRAWPORT_SCOPE_UI
		&& m_uiScopeDepth == 0
		&& ReadFull3DReplacementMode()
		&& m_pNativeRenderer != NULL
		&& m_pNativeRenderer->HasPendingLegacy3DDraws())
	{
		const UINT currentSegment =
			m_pNativeRenderer != NULL
				? m_pNativeRenderer->GetCurrentSegment()
				: 0;
		if (!SubmitUiSegmentsThrough(currentSegment, true))
			return false;
	}
	const bool succeeded = m_frameOpen && m_pNativeRenderer != NULL
		&& m_pNativeRenderer->BeginDrawPortScope(scope);
	if (succeeded && scope == DX12_DRAWPORT_SCOPE_UI)
		++m_uiScopeDepth;
	return succeeded;
}

bool CDirectX12Backend::EndDrawPortScope(
	DirectX12DrawPortScope scope)
{
	const bool succeeded = m_frameOpen && m_pNativeRenderer != NULL
		&& m_pNativeRenderer->EndDrawPortScope(scope);
	if (succeeded && scope == DX12_DRAWPORT_SCOPE_UI
		&& m_uiScopeDepth > 0)
		--m_uiScopeDepth;
	return succeeded;
}

bool CDirectX12Backend::ClosePendingUiScope()
{
	bool succeeded = true;
	while (m_uiScopeDepth > 0)
	{
		if (!EndDrawPortScope(DX12_DRAWPORT_SCOPE_UI))
		{
			succeeded = false;
			break;
		}
	}
	return succeeded;
}

bool CDirectX12Backend::ShouldSubmitLegacyDrawPort(
	bool nativeCaptured)
{
	if (m_drawPortValidationMode
			!= DX12_DRAWPORT_VALIDATION_UI_REPLACE
		|| !m_frameOpen || m_uiScopeDepth == 0
		|| m_offscreenDrawPortDepth > 0
		|| ReadKeepLegacyUiMode())
		return true;
	if (nativeCaptured)
	{
		++m_suppressedLegacyDrawCount;
		return false;
	}
	++m_fallbackLegacyDrawCount;
	return true;
}

bool CDirectX12Backend::ShouldSubmitLegacy3DDraw(
	bool nativeCaptured)
{
	// Los DrawPort de la interfaz también terminan usando primitivas D3D9.
	// Nunca deben considerarse geometría del mundo ni ser suprimidos por el
	// reemplazo 3D.
	if (m_uiScopeDepth > 0)
		return true;
	const bool offscreenReplacement =
		m_offscreenDrawPortDepth > 0
		&& ReadFull3DReplacementMode();
	const bool fullWorldReplacement = ReadFull3DReplacementMode();
	if (!ReadRigidLitReplacementMode() && !fullWorldReplacement)
		return true;
	if (!nativeCaptured && fullWorldReplacement
		&& m_pNativeRenderer != NULL
		&& m_pNativeRenderer->HasPendingLegacy3DDraws())
	{
		// Conserva el orden original entre ambos command streams: todo lote
		// DX12 anterior debe llegar al depth buffer antes de ejecutar este draw
		// de fallback en D3D9On12.
		SubmitPendingLegacy3DForCurrentTarget(
			"un fallback D3D9");
	}
	if (nativeCaptured
		&& (fullWorldReplacement
			|| offscreenReplacement
			|| m_legacy3DDepthAvailable))
	{
		++m_suppressedLegacy3DDrawCount;
		return false;
	}
	++m_fallbackLegacy3DDrawCount;
	return true;
}

bool CDirectX12Backend::InsertDrawPortBarrier(
	DirectX12DrawPortBarrierKind kind)
{
	if (!m_frameOpen || m_pNativeRenderer == NULL
		|| !m_pNativeRenderer->InsertDrawPortBarrier(kind))
		return false;
	if (kind == DX12_DRAWPORT_BARRIER_RENDER_TARGET_BEGIN
		&& m_drawPortValidationMode != DX12_DRAWPORT_VALIDATION_SHADOW)
	{
		const UINT currentSegment =
			m_pNativeRenderer->GetCurrentSegment();
		if (currentSegment > 0
			&& !SubmitUiSegmentsThrough(currentSegment - 1))
		{
			CPrintF(
				"DX12 UI: fallo el envio parcial; se conserva D3D9.\n");
			return false;
		}
	}
	return true;
}

void CDirectX12Backend::BeginOffscreenDrawPortScope()
{
	if (m_frameOpen)
	{
		// CRenderTexture llama a este metodo antes de cambiar el destino D3D9.
		// El vaciado conserva el orden entre el mundo principal y la pasada
		// auxiliar sin mezclar rangos pertenecientes a recursos distintos.
		SubmitPendingLegacy3DForCurrentTarget(
			"entrar a un destino auxiliar");
		++m_offscreenDrawPortDepth;
	}
}

void CDirectX12Backend::EndOffscreenDrawPortScope()
{
	if (m_offscreenDrawPortDepth > 0)
	{
		// Debe ejecutarse mientras la textura auxiliar sigue enlazada. Al
		// devolverla a D3D9On12, la fence garantiza que el consumidor espere.
		SubmitPendingLegacy3DForCurrentTarget(
			"salir de un destino auxiliar");
		--m_offscreenDrawPortDepth;
	}
}

void CDirectX12Backend::SetLegacy3DVertexArray(
	const FLOAT* pPositions,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DVertexArray(
			pPositions,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DTexCoordArray(
			textureUnit,
			pTexCoords,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DProjectiveTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DProjectiveTexCoordArray(
			textureUnit,
			pTexCoords,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DNormalArray(
	const FLOAT* pNormals,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DNormalArray(
			pNormals,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DWeightArray(
	const BYTE* pWeights,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DWeightArray(
			pWeights,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DTangentArray(
	const FLOAT* pTangents,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DTangentArray(
			pTangents,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DColorArray(
	const ULONG* pColors,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DColorArray(
			pColors,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DConstantColor(ULONG color)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DConstantColor(color);
}

void CDirectX12Backend::SetLegacy3DStaticVertexArray(
	const FLOAT* pPositions,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DStaticVertexArray(
			pPositions,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DStaticTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DStaticTexCoordArray(
			textureUnit,
			pTexCoords,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DStaticNormalArray(
	const FLOAT* pNormals,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DStaticNormalArray(
			pNormals,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DStaticWeightArray(
	const BYTE* pWeights,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DStaticWeightArray(
			pWeights,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DStaticTangentArray(
	const FLOAT* pTangents,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DStaticTangentArray(
			pTangents,
			vertexCount);
}

void CDirectX12Backend::SetLegacy3DStaticD3DColorArray(
	const ULONG* pColors,
	UINT vertexCount)
{
	if (m_frameOpen && m_pNativeRenderer != NULL)
		m_pNativeRenderer->SetLegacy3DStaticD3DColorArray(
			pColors,
			vertexCount);
}

void CDirectX12Backend::PrepareLegacy3DDepthClear(
	IDirect3DDevice9* pDevice9)
{
	if (!m_frameOpen || m_uiScopeDepth > 0
		|| m_offscreenDrawPortDepth > 0
		|| m_pNativeRenderer == NULL
		|| !ReadFull3DReplacementMode()
		|| !m_legacy3DDepthAvailable
		|| ClassifyLegacyRenderTarget(pDevice9)
			!= DX12_LEGACY_RENDER_TARGET_PRESENTATION
		|| !m_pNativeRenderer->HasPendingLegacy3DDraws())
		return;

	// El Clear se ejecuta sobre el depth compartido de D3D9On12. Materializa
	// primero la geometría nativa anterior y deja que D3D9 lo borre una sola
	// vez; repetir el Clear más tarde elimina profundidad ya válida del suelo.
	const UINT currentSegment = m_pNativeRenderer->GetCurrentSegment();
	if (!SubmitUiSegmentsThrough(currentSegment, true))
	{
		CPrintF(
			"DX12 3D: fallo la barrera previa al borrado de profundidad.\n");
	}
}

bool CDirectX12Backend::QueueLegacy3DIndexedDraw(
	IDirect3DDevice9* pDevice9,
	const USHORT* pIndices,
	UINT indexCount,
	bool dynamicBuffer,
	bool usesVertexProgram,
	bool usesPixelProgram,
	bool usesColorArray,
	bool projectiveMapping,
	UINT texturePassCount)
{
	if (m_uiScopeDepth > 0)
		return false;
	const DirectX12LegacyRenderTargetKind renderTargetKind =
		ClassifyLegacyRenderTarget(pDevice9);
	if (renderTargetKind == DX12_LEGACY_RENDER_TARGET_OFFSCREEN
		&& m_currentSubmission + 1 >= MAX_SUBMISSIONS_PER_FRAME)
		return false;
	if (ReadRigidLitReplacementMode()
		&& !ReadFull3DReplacementMode()
		&& !m_legacy3DDepthAvailable
		&& renderTargetKind
			== DX12_LEGACY_RENDER_TARGET_PRESENTATION)
		return false;
	return m_frameOpen && m_pNativeRenderer != NULL
		&& m_pNativeRenderer->QueueLegacy3DIndexedDraw(
			pDevice9,
			pIndices,
			indexCount,
			dynamicBuffer,
			usesVertexProgram,
			usesPixelProgram,
			usesColorArray,
			projectiveMapping,
			texturePassCount,
			renderTargetKind);
}

bool CDirectX12Backend::QueueDrawPortLine(
	FLOAT x0,
	FLOAT y0,
	FLOAT x1,
	FLOAT y1,
	ULONG color,
	LONG scissorLeft,
	LONG scissorTop,
	LONG scissorRight,
	LONG scissorBottom)
{
	return m_frameOpen && m_offscreenDrawPortDepth == 0
		&& m_pNativeRenderer != NULL
		&& m_pNativeRenderer->QueueDrawPortLine(
			x0,
			y0,
			x1,
			y1,
			color,
			scissorLeft,
			scissorTop,
			scissorRight,
			scissorBottom);
}

bool CDirectX12Backend::QueueDrawPortTriangle(
	FLOAT x0,
	FLOAT y0,
	FLOAT x1,
	FLOAT y1,
	FLOAT x2,
	FLOAT y2,
	ULONG color0,
	ULONG color1,
	ULONG color2,
	LONG scissorLeft,
	LONG scissorTop,
	LONG scissorRight,
	LONG scissorBottom)
{
	return m_frameOpen && m_offscreenDrawPortDepth == 0
		&& m_pNativeRenderer != NULL
		&& m_pNativeRenderer->QueueDrawPortTriangle(
			x0,
			y0,
			x1,
			y1,
			x2,
			y2,
			color0,
			color1,
			color2,
			scissorLeft,
			scissorTop,
			scissorRight,
			scissorBottom);
}

bool CDirectX12Backend::QueueDrawPortRectangle(
	FLOAT x0,
	FLOAT y0,
	FLOAT x1,
	FLOAT y1,
	ULONG colorUpperLeft,
	ULONG colorUpperRight,
	ULONG colorLowerLeft,
	ULONG colorLowerRight,
	LONG scissorLeft,
	LONG scissorTop,
	LONG scissorRight,
	LONG scissorBottom)
{
	return m_frameOpen && m_offscreenDrawPortDepth == 0
		&& m_pNativeRenderer != NULL
		&& m_pNativeRenderer->QueueDrawPortRectangle(
			x0,
			y0,
			x1,
			y1,
			colorUpperLeft,
			colorUpperRight,
			colorLowerLeft,
			colorLowerRight,
			scissorLeft,
			scissorTop,
			scissorRight,
			scissorBottom);
}

bool CDirectX12Backend::QueueDrawPortTexturedTriangle(
	IDirect3DTexture9* pTexture,
	const DirectX12DrawPortTexturedVertex& vertex0,
	const DirectX12DrawPortTexturedVertex& vertex1,
	const DirectX12DrawPortTexturedVertex& vertex2,
	LONG scissorLeft,
	LONG scissorTop,
	LONG scissorRight,
	LONG scissorBottom,
	DirectX12BlendMode blendMode,
	DirectX12SamplerMode samplerMode)
{
	return m_frameOpen && m_offscreenDrawPortDepth == 0
		&& m_pNativeRenderer != NULL
		&& m_pNativeRenderer->QueueDrawPortTexturedTriangle(
			pTexture,
			vertex0.x, vertex0.y, vertex0.u, vertex0.v, vertex0.color,
			vertex1.x, vertex1.y, vertex1.u, vertex1.v, vertex1.color,
			vertex2.x, vertex2.y, vertex2.u, vertex2.v, vertex2.color,
			scissorLeft,
			scissorTop,
			scissorRight,
			scissorBottom,
			blendMode,
			samplerMode);
}

ID3D12Device* CDirectX12Backend::GetDevice() const
{
	return m_pDevice;
}

ID3D12CommandQueue* CDirectX12Backend::GetGraphicsQueue() const
{
	return m_pGraphicsQueue;
}

ID3D12GraphicsCommandList* CDirectX12Backend::GetCommandList() const
{
	return m_pCommandList;
}

CDirectX12RenderTargetManager*
CDirectX12Backend::GetRenderTargetManager() const
{
	return m_pRenderTargets;
}

CDirectX12UploadManager* CDirectX12Backend::GetUploadManager() const
{
	return m_pUploadManager;
}

CDirectX12DescriptorHeap*
CDirectX12Backend::GetResourceDescriptorHeap() const
{
	return m_pResourceDescriptors;
}

CDirectX12DescriptorHeap*
CDirectX12Backend::GetSamplerDescriptorHeap() const
{
	return m_pSamplerDescriptors;
}

bool CDirectX12Backend::IsFrameOpen() const
{
	return m_frameOpen;
}

bool CDirectX12Backend::ShouldBypassLegacyPresent() const
{
	return m_drawPortValidationMode
		== DX12_DRAWPORT_VALIDATION_UI_REPLACE;
}

bool CDirectX12Backend::IsFull3DReplacementEnabled() const
{
	return ReadFull3DReplacementMode();
}

bool CDirectX12Backend::RequiresLegacyOffscreenDepth() const
{
	return !ReadFull3DReplacementMode();
}

UINT CDirectX12Backend::GetUiPrimitiveCount() const
{
	return m_pNativeRenderer != NULL
		? m_pNativeRenderer->GetUiPrimitiveCount()
		: 0;
}

UINT CDirectX12Backend::GetUiSegmentCount() const
{
	return m_pNativeRenderer != NULL
		? m_pNativeRenderer->GetUiSegmentCount()
		: 0;
}

UINT CDirectX12Backend::GetUiBarrierCount() const
{
	return m_pNativeRenderer != NULL
		? m_pNativeRenderer->GetUiBarrierCount()
		: 0;
}

CDirectX12Backend& GetDirectX12Backend()
{
	static CDirectX12Backend backend;
	return backend;
}

CDirectX12DrawPortScope::CDirectX12DrawPortScope(
	CDirectX12Backend& backend,
	DirectX12DrawPortScope scope)
	: m_backend(backend)
	, m_scope(scope)
	, m_active(backend.BeginDrawPortScope(scope))
{
}

CDirectX12DrawPortScope::~CDirectX12DrawPortScope()
{
	if (m_active)
		m_backend.EndDrawPortScope(m_scope);
}

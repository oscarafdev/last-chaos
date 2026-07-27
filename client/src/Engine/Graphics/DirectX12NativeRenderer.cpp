#include "stdh.h"

#include <d3d12.h>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/DirectX12BloomRenderer.h>
#include <Engine/Graphics/DirectX12Buffer.h>
#include <Engine/Graphics/DirectX12DrawPortCommandBatch.h>
#include <Engine/Graphics/DirectX12InteropTextureManager.h>
#include <Engine/Graphics/DirectX12Legacy3DCommandBatch.h>
#include <Engine/Graphics/DirectX12NativeRenderer.h>
#include <Engine/Graphics/DirectX12PipelineCache.h>
#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12Texture.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	void ReportNativeFailureOnce(const char* pStage)
	{
		static const char* pLastStage = NULL;
		if (pStage == NULL || pStage == pLastStage)
			return;
		CPrintF("DX12 error en render nativo: %s.\n", pStage);
		pLastStage = pStage;
	}
}

CDirectX12NativeRenderer::CDirectX12NativeRenderer()
	: m_pDevice(NULL)
	, m_pPipelineCache(NULL)
	, m_pBloomRenderer(NULL)
	, m_pDrawPortCommands(NULL)
	, m_pLegacy3DCommands(NULL)
	, m_pVertexBuffer(NULL)
	, m_pIndexBuffer(NULL)
	, m_pTexture(NULL)
	, m_pSamplerHeap(NULL)
	, m_initializationAttempted(false)
	, m_resourcesReady(false)
{
}

CDirectX12NativeRenderer::~CDirectX12NativeRenderer()
{
	Shutdown();
}

bool CDirectX12NativeRenderer::Initialize(ID3D12Device* pDevice)
{
	if (pDevice == NULL)
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pPipelineCache = new CDirectX12PipelineCache;
	if (m_pPipelineCache == NULL
		|| !m_pPipelineCache->Initialize(m_pDevice))
	{
		Shutdown();
		return false;
	}
	m_pBloomRenderer = new CDirectX12BloomRenderer;
	if (m_pBloomRenderer == NULL
		|| !m_pBloomRenderer->Initialize(
			m_pDevice,
			m_pPipelineCache))
	{
		Shutdown();
		return false;
	}
	m_pDrawPortCommands = new CDirectX12DrawPortCommandBatch;
	if (m_pDrawPortCommands == NULL
		|| !m_pDrawPortCommands->Initialize(
			m_pDevice,
			m_pPipelineCache))
	{
		Shutdown();
		return false;
	}
	m_pLegacy3DCommands = new CDirectX12Legacy3DCommandBatch;
	if (m_pLegacy3DCommands == NULL
		|| !m_pLegacy3DCommands->Initialize(
			m_pDevice,
			m_pPipelineCache))
	{
		Shutdown();
		return false;
	}
	return true;
}

void CDirectX12NativeRenderer::Shutdown()
{
	if (m_pSamplerHeap != NULL)
	{
		for (UINT iSampler = 0;
			iSampler < DX12_SAMPLER_COUNT;
			++iSampler)
		{
			if (m_samplers[iSampler].IsValid())
				m_pSamplerHeap->Release(m_samplers[iSampler].index);
			m_samplers[iSampler] = DirectX12DescriptorHandle();
		}
	}
	m_pSamplerHeap = NULL;

	delete m_pTexture;
	m_pTexture = NULL;
	delete m_pIndexBuffer;
	m_pIndexBuffer = NULL;
	delete m_pVertexBuffer;
	m_pVertexBuffer = NULL;
	delete m_pDrawPortCommands;
	m_pDrawPortCommands = NULL;
	delete m_pLegacy3DCommands;
	m_pLegacy3DCommands = NULL;
	delete m_pBloomRenderer;
	m_pBloomRenderer = NULL;
	delete m_pPipelineCache;
	m_pPipelineCache = NULL;

	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	m_initializationAttempted = false;
	m_resourcesReady = false;
}

void CDirectX12NativeRenderer::ForgetTexture(IDirect3DTexture9* pTexture)
{
	if (m_pDrawPortCommands != NULL)
		m_pDrawPortCommands->ForgetTexture(pTexture);
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->ForgetTexture(pTexture);
}

void CDirectX12NativeRenderer::BeginFrame(UINT frameIndex)
{
	if (m_pDrawPortCommands != NULL)
		m_pDrawPortCommands->BeginFrame(frameIndex);
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->BeginFrame(frameIndex);
}

void CDirectX12NativeRenderer::SetLegacy3DVertexArray(
	const FLOAT* pPositions,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetVertexArray(pPositions, vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetTexCoordArray(
			textureUnit,
			pTexCoords,
			vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DProjectiveTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetProjectiveTexCoordArray(
			textureUnit,
			pTexCoords,
			vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DNormalArray(
	const FLOAT* pNormals,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetNormalArray(pNormals, vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DWeightArray(
	const BYTE* pWeights,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetWeightArray(pWeights, vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DTangentArray(
	const FLOAT* pTangents,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetTangentArray(pTangents, vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DColorArray(
	const ULONG* pColors,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetColorArray(pColors, vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DConstantColor(ULONG color)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetConstantColor(color);
}

void CDirectX12NativeRenderer::SetLegacy3DStaticVertexArray(
	const FLOAT* pPositions,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetStaticVertexArray(
			pPositions,
			vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DStaticTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetStaticTexCoordArray(
			textureUnit,
			pTexCoords,
			vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DStaticNormalArray(
	const FLOAT* pNormals,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetStaticNormalArray(
			pNormals,
			vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DStaticWeightArray(
	const BYTE* pWeights,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetStaticWeightArray(
			pWeights,
			vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DStaticTangentArray(
	const FLOAT* pTangents,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetStaticTangentArray(
			pTangents,
			vertexCount);
}

void CDirectX12NativeRenderer::SetLegacy3DStaticD3DColorArray(
	const ULONG* pColors,
	UINT vertexCount)
{
	if (m_pLegacy3DCommands != NULL)
		m_pLegacy3DCommands->SetStaticD3DColorArray(
			pColors,
			vertexCount);
}

bool CDirectX12NativeRenderer::QueueLegacy3DIndexedDraw(
	IDirect3DDevice9* pDevice9,
	const USHORT* pIndices,
	UINT indexCount,
	bool dynamicBuffer,
	bool usesVertexProgram,
	bool usesPixelProgram,
	bool usesColorArray,
	bool projectiveMapping,
	UINT texturePassCount,
	DirectX12LegacyRenderTargetKind renderTargetKind)
{
	return m_pLegacy3DCommands != NULL
		&& m_pLegacy3DCommands->QueueIndexedDraw(
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

bool CDirectX12NativeRenderer::BeginDrawPortScope(
	DirectX12DrawPortScope scope)
{
	return m_pDrawPortCommands != NULL
		&& m_pDrawPortCommands->BeginScope(scope);
}

bool CDirectX12NativeRenderer::EndDrawPortScope(
	DirectX12DrawPortScope scope)
{
	return m_pDrawPortCommands != NULL
		&& m_pDrawPortCommands->EndScope(scope);
}

bool CDirectX12NativeRenderer::InsertDrawPortBarrier(
	DirectX12DrawPortBarrierKind kind)
{
	return m_pDrawPortCommands != NULL
		&& m_pDrawPortCommands->InsertBarrier(kind);
}

bool CDirectX12NativeRenderer::QueueDrawPortPoint(
	FLOAT x,
	FLOAT y,
	FLOAT radius,
	ULONG color,
	LONG scissorLeft,
	LONG scissorTop,
	LONG scissorRight,
	LONG scissorBottom)
{
	if (m_pDrawPortCommands == NULL)
		return false;
	const D3D12_RECT scissor = {
		scissorLeft, scissorTop, scissorRight, scissorBottom
	};
	return m_pDrawPortCommands->QueuePoint(
		x, y, radius, color, scissor);
}

bool CDirectX12NativeRenderer::QueueDrawPortLine(
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
	if (m_pDrawPortCommands == NULL)
		return false;
	const D3D12_RECT scissor = {
		scissorLeft, scissorTop, scissorRight, scissorBottom
	};
	return m_pDrawPortCommands->QueueLine(
		x0, y0, x1, y1, color, scissor);
}

bool CDirectX12NativeRenderer::QueueDrawPortTriangle(
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
	if (m_pDrawPortCommands == NULL)
		return false;
	const D3D12_RECT scissor = {
		scissorLeft, scissorTop, scissorRight, scissorBottom
	};
	return m_pDrawPortCommands->QueueSolidTriangle(
		x0,
		y0,
		x1,
		y1,
		x2,
		y2,
		color0,
		color1,
		color2,
		scissor);
}

bool CDirectX12NativeRenderer::QueueDrawPortRectangle(
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
	if (m_pDrawPortCommands == NULL)
		return false;
	const D3D12_RECT scissor = {
		scissorLeft, scissorTop, scissorRight, scissorBottom
	};
	return m_pDrawPortCommands->QueueRectangle(
		x0,
		y0,
		x1,
		y1,
		colorUpperLeft,
		colorUpperRight,
		colorLowerLeft,
		colorLowerRight,
		scissor);
}

bool CDirectX12NativeRenderer::QueueDrawPortTexturedTriangle(
	IDirect3DTexture9* pTexture,
	FLOAT x0, FLOAT y0, FLOAT u0, FLOAT v0, ULONG color0,
	FLOAT x1, FLOAT y1, FLOAT u1, FLOAT v1, ULONG color1,
	FLOAT x2, FLOAT y2, FLOAT u2, FLOAT v2, ULONG color2,
	LONG scissorLeft,
	LONG scissorTop,
	LONG scissorRight,
	LONG scissorBottom,
	DirectX12BlendMode blendMode,
	DirectX12SamplerMode samplerMode)
{
	if (m_pDrawPortCommands == NULL)
		return false;
	const D3D12_RECT scissor = {
		scissorLeft, scissorTop, scissorRight, scissorBottom
	};
	return m_pDrawPortCommands->QueueTriangle(
		pTexture,
		x0, y0, u0, v0, color0,
		x1, y1, u1, v1, color1,
		x2, y2, u2, v2, color2,
		scissor,
		blendMode,
		samplerMode);
}

bool CDirectX12NativeRenderer::EnsureResources(
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	CDirectX12DescriptorHeap* pResourceDescriptors,
	CDirectX12DescriptorHeap* pSamplerDescriptors)
{
	if (m_resourcesReady)
		return true;
	if (m_initializationAttempted || pCommandList == NULL
		|| pUploadManager == NULL || pResourceDescriptors == NULL
		|| pSamplerDescriptors == NULL)
	{
		ReportNativeFailureOnce(
			"recursos ya fallidos o dependencias invalidas");
		return false;
	}
	m_initializationAttempted = true;

	const NativeVertex vertices[] = {
		{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } }
	};
	const USHORT indices[] = { 0, 1, 2, 2, 1, 3 };
	const BYTE whitePixel[] = { 255, 255, 255, 255 };

	m_pVertexBuffer = new CDirectX12Buffer;
	m_pIndexBuffer = new CDirectX12Buffer;
	m_pTexture = new CDirectX12Texture;
	if (m_pVertexBuffer == NULL || m_pIndexBuffer == NULL
		|| m_pTexture == NULL)
	{
		ReportNativeFailureOnce("asignacion de recursos de validacion");
		return false;
	}

	if (!m_pVertexBuffer->CreateVertexBuffer(
			m_pDevice,
			sizeof(vertices),
			sizeof(NativeVertex))
		|| !m_pIndexBuffer->CreateIndexBuffer(
			m_pDevice,
			sizeof(indices),
			DXGI_FORMAT_R16_UINT)
		|| !m_pTexture->Create2D(
			m_pDevice,
			pResourceDescriptors,
			1,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM))
	{
		ReportNativeFailureOnce("creacion de recursos de validacion");
		return false;
	}

	if (!m_pVertexBuffer->Upload(
			pUploadManager,
			pCommandList,
			vertices,
			sizeof(vertices))
		|| !m_pIndexBuffer->Upload(
			pUploadManager,
			pCommandList,
			indices,
			sizeof(indices)))
	{
		ReportNativeFailureOnce("carga de vertices o indices de validacion");
		return false;
	}

	DirectX12SubresourceData textureData;
	textureData.pData = whitePixel;
	textureData.rowPitch = sizeof(whitePixel);
	textureData.slicePitch = sizeof(whitePixel);
	if (!m_pTexture->Upload(
			pUploadManager,
			pCommandList,
			0,
			1,
			&textureData))
	{
		ReportNativeFailureOnce("carga de textura blanca de validacion");
		return false;
	}

	m_pSamplerHeap = pSamplerDescriptors;
	for (UINT iSampler = 0;
		iSampler < DX12_SAMPLER_COUNT;
		++iSampler)
	{
		if (!pSamplerDescriptors->Allocate(&m_samplers[iSampler]))
		{
			ReportNativeFailureOnce("asignacion de sampler nativo");
			return false;
		}
		D3D12_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		const bool anisotropic =
			iSampler == DX12_SAMPLER_ANISOTROPIC_CLAMP
			|| iSampler == DX12_SAMPLER_ANISOTROPIC_REPEAT;
		const bool linear =
			iSampler == DX12_SAMPLER_LINEAR_CLAMP
			|| iSampler == DX12_SAMPLER_LINEAR_REPEAT
			|| anisotropic;
		const bool repeat =
			iSampler == DX12_SAMPLER_POINT_REPEAT
			|| iSampler == DX12_SAMPLER_LINEAR_REPEAT
			|| iSampler == DX12_SAMPLER_ANISOTROPIC_REPEAT;
		samplerDesc.Filter = anisotropic
			? D3D12_FILTER_ANISOTROPIC
			: linear
				? D3D12_FILTER_MIN_MAG_MIP_LINEAR
				: D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.MaxAnisotropy = anisotropic ? 8 : 1;
		samplerDesc.AddressU = repeat
			? D3D12_TEXTURE_ADDRESS_MODE_WRAP
			: D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressV = samplerDesc.AddressU;
		samplerDesc.AddressW = samplerDesc.AddressU;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		m_pDevice->CreateSampler(
			&samplerDesc,
			m_samplers[iSampler].cpu);
	}

	m_resourcesReady = true;
	return true;
}

bool CDirectX12NativeRenderer::RenderValidationPass(
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12RenderTargetManager* pRenderTargets,
	CDirectX12UploadManager* pUploadManager,
	CDirectX12DescriptorHeap* pResourceDescriptors,
	CDirectX12DescriptorHeap* pSamplerDescriptors,
	CDirectX12InteropTextureManager* pInteropTextures,
	DirectX12DrawPortValidationMode validationMode,
	UINT minimumSegment,
	UINT maximumSegment,
	bool submitLegacy3D)
{
	if (pCommandList == NULL || pRenderTargets == NULL
		|| !pRenderTargets->IsAcquired())
	{
		ReportNativeFailureOnce("argumentos o render target invalidos");
		return false;
	}
	if (!EnsureResources(
			pCommandList,
			pUploadManager,
			pResourceDescriptors,
			pSamplerDescriptors))
	{
		ReportNativeFailureOnce("preparacion de recursos nativos");
		return false;
	}

	bool legacy3DSucceeded = true;
	if (submitLegacy3D)
	{
		legacy3DSucceeded = m_pLegacy3DCommands != NULL
			&& m_pLegacy3DCommands->RenderLegacy3DPass(
			pCommandList,
			pRenderTargets,
			pUploadManager,
			pInteropTextures,
			m_samplers,
			m_pTexture->GetShaderResourceView());
	}
	const bool commandStreamSucceeded = legacy3DSucceeded
		&& m_pDrawPortCommands != NULL
		&& m_pDrawPortCommands->Render(
			pCommandList,
			pRenderTargets,
			pUploadManager,
			pInteropTextures,
			m_samplers,
			m_pTexture->GetShaderResourceView(),
			validationMode,
			minimumSegment,
			maximumSegment);
	if (!commandStreamSucceeded)
		ReportNativeFailureOnce("reproduccion de comandos DrawPort");
	return commandStreamSucceeded;
}

bool CDirectX12NativeRenderer::RenderBloom(
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12RenderTargetManager* pRenderTargets,
	CDirectX12UploadManager* pUploadManager,
	CDirectX12DescriptorHeap* pResourceDescriptors,
	CDirectX12DescriptorHeap* pSamplerDescriptors,
	CDirectX12InteropTextureManager* pInteropTextures,
	IDirect3DTexture9* pSourceTexture,
	IDirect3DTexture9* pFilterTexture0,
	IDirect3DTexture9* pFilterTexture1)
{
	if (m_pBloomRenderer == NULL
		|| !EnsureResources(
			pCommandList,
			pUploadManager,
			pResourceDescriptors,
			pSamplerDescriptors))
		return false;
	const bool succeeded = m_pBloomRenderer->Render(
		pCommandList,
		pRenderTargets,
		pInteropTextures,
		pSourceTexture,
		pFilterTexture0,
		pFilterTexture1,
		m_pVertexBuffer,
		m_pIndexBuffer,
		m_samplers[DX12_SAMPLER_LINEAR_CLAMP]);
	if (!succeeded)
		ReportNativeFailureOnce("pasadas nativas de bloom");
	return succeeded;
}

UINT CDirectX12NativeRenderer::GetUiPrimitiveCount() const
{
	return m_pDrawPortCommands != NULL
		? m_pDrawPortCommands->GetUiPrimitiveCount()
		: 0;
}

UINT CDirectX12NativeRenderer::GetUiSegmentCount() const
{
	return m_pDrawPortCommands != NULL
		? m_pDrawPortCommands->GetUiSegmentCount()
		: 0;
}

UINT CDirectX12NativeRenderer::GetUiBarrierCount() const
{
	return m_pDrawPortCommands != NULL
		? m_pDrawPortCommands->GetUiBarrierCount()
		: 0;
}

UINT CDirectX12NativeRenderer::GetLegacy3DCapturedDrawCount() const
{
	return m_pLegacy3DCommands != NULL
		? m_pLegacy3DCommands->GetCapturedDrawCount()
		: 0;
}

bool CDirectX12NativeRenderer::HasPendingLegacy3DDraws() const
{
	return m_pLegacy3DCommands != NULL
		&& m_pLegacy3DCommands->HasPendingDraws();
}

UINT CDirectX12NativeRenderer::GetLegacy3DRejectedDrawCount() const
{
	return m_pLegacy3DCommands != NULL
		? m_pLegacy3DCommands->GetRejectedDrawCount()
		: 0;
}

UINT CDirectX12NativeRenderer::GetLegacy3DCapturedTriangleCount() const
{
	return m_pLegacy3DCommands != NULL
		? m_pLegacy3DCommands->GetCapturedTriangleCount()
		: 0;
}

UINT CDirectX12NativeRenderer::GetLegacy3DRejectedReasonCount(
	UINT reason) const
{
	return m_pLegacy3DCommands != NULL
		&& reason < CDirectX12Legacy3DCommandBatch::REJECT_REASON_COUNT
		? m_pLegacy3DCommands->GetRejectedReasonCount(
			static_cast<CDirectX12Legacy3DCommandBatch::RejectionReason>(
				reason))
		: 0;
}

UINT64 CDirectX12NativeRenderer::
GetLegacy3DTopVertexShaderFingerprint() const
{
	return m_pLegacy3DCommands != NULL
		? m_pLegacy3DCommands->GetTopVertexShaderFingerprint()
		: 0;
}

UINT CDirectX12NativeRenderer::GetLegacy3DTopVertexShaderDrawCount() const
{
	return m_pLegacy3DCommands != NULL
		? m_pLegacy3DCommands->GetTopVertexShaderDrawCount()
		: 0;
}

UINT CDirectX12NativeRenderer::
GetLegacy3DTopVertexShaderTriangleCount() const
{
	return m_pLegacy3DCommands != NULL
		? m_pLegacy3DCommands->GetTopVertexShaderTriangleCount()
		: 0;
}

bool CDirectX12NativeRenderer::IsLegacy3DOverlayComparisonEnabled() const
{
	return m_pLegacy3DCommands != NULL
		&& m_pLegacy3DCommands->IsOverlayComparisonEnabled();
}

UINT CDirectX12NativeRenderer::GetCurrentSegment() const
{
	return m_pDrawPortCommands != NULL
		? m_pDrawPortCommands->GetCurrentSegment()
		: 0;
}

bool CDirectX12NativeRenderer::HasUiCommands(
	UINT minimumSegment,
	UINT maximumSegment) const
{
	return m_pDrawPortCommands != NULL
		&& m_pDrawPortCommands->HasUiCommands(
			minimumSegment,
			maximumSegment);
}

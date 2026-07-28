#include "stdh.h"

#include <d3d12.h>
#include <dxgi1_4.h>

#include <Engine/Graphics/DirectX12PresentationManager.h>

CDirectX12PresentationManager::CDirectX12PresentationManager()
	: m_pDevice(NULL)
	, m_pGraphicsQueue(NULL)
	, m_hDxgiModule(NULL)
	, m_pFactory(NULL)
	, m_pSwapChain(NULL)
	, m_pRtvHeap(NULL)
	, m_pDsvHeap(NULL)
	, m_pDepthBuffer(NULL)
	, m_rtvDescriptorSize(0)
	, m_hWindow(NULL)
	, m_width(0)
	, m_height(0)
	, m_hRequestedWindow(NULL)
	, m_requestedWidth(0)
	, m_requestedHeight(0)
	, m_frameQueued(false)
	, m_clearTarget(false)
{
	for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i)
	{
		m_apBackBuffers[i] = NULL;
		m_aBackBufferStates[i] = D3D12_RESOURCE_STATE_PRESENT;
	}
}

CDirectX12PresentationManager::~CDirectX12PresentationManager()
{
	Shutdown();
}

bool CDirectX12PresentationManager::Initialize(
	ID3D12Device* pDevice,
	ID3D12CommandQueue* pGraphicsQueue)
{
	if (pDevice == NULL || pGraphicsQueue == NULL)
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pGraphicsQueue = pGraphicsQueue;
	m_pGraphicsQueue->AddRef();

	m_hDxgiModule = LoadLibraryA("dxgi.dll");
	if (m_hDxgiModule == NULL)
		return false;
	typedef HRESULT(WINAPI* CreateDXGIFactory2Proc)(
		UINT,
		REFIID,
		void**);
	CreateDXGIFactory2Proc pCreateFactory =
		reinterpret_cast<CreateDXGIFactory2Proc>(
			GetProcAddress(m_hDxgiModule, "CreateDXGIFactory2"));
	if (pCreateFactory == NULL)
		return false;
	return SUCCEEDED(pCreateFactory(
		0,
		__uuidof(IDXGIFactory4),
		reinterpret_cast<void**>(&m_pFactory)));
}

void CDirectX12PresentationManager::Configure(
	HWND hWindow,
	UINT width,
	UINT height)
{
	m_hRequestedWindow = hWindow;
	m_requestedWidth = width;
	m_requestedHeight = height;
}

bool CDirectX12PresentationManager::RequiresRebuild() const
{
	return m_pSwapChain == NULL
		|| m_hWindow != m_hRequestedWindow
		|| m_width != m_requestedWidth
		|| m_height != m_requestedHeight;
}

bool CDirectX12PresentationManager::BeginFrame()
{
	m_frameQueued = false;
	m_clearTarget = true;
	return EnsureSwapChain(
		m_hRequestedWindow,
		m_requestedWidth,
		m_requestedHeight);
}

void CDirectX12PresentationManager::Shutdown()
{
	ReleaseSwapChain();
	if (m_pFactory != NULL)
	{
		m_pFactory->Release();
		m_pFactory = NULL;
	}
	if (m_hDxgiModule != NULL)
	{
		FreeLibrary(m_hDxgiModule);
		m_hDxgiModule = NULL;
	}
	if (m_pGraphicsQueue != NULL)
	{
		m_pGraphicsQueue->Release();
		m_pGraphicsQueue = NULL;
	}
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	m_hRequestedWindow = NULL;
	m_requestedWidth = 0;
	m_requestedHeight = 0;
}

bool CDirectX12PresentationManager::EnsureSwapChain(
	HWND hWindow,
	UINT width,
	UINT height)
{
	if (m_pFactory == NULL || m_pGraphicsQueue == NULL || m_pDevice == NULL
		|| hWindow == NULL || width == 0 || height == 0)
		return false;
	if (m_pSwapChain != NULL && m_hWindow == hWindow
		&& m_width == width && m_height == height)
		return true;

	ReleaseSwapChain();
	DXGI_SWAP_CHAIN_DESC1 desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = BACK_BUFFER_COUNT;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

	IDXGISwapChain1* pSwapChain1 = NULL;
	HRESULT hr = m_pFactory->CreateSwapChainForHwnd(
		m_pGraphicsQueue,
		hWindow,
		&desc,
		NULL,
		NULL,
		&pSwapChain1);
	if (FAILED(hr))
		return false;

	hr = pSwapChain1->QueryInterface(
		__uuidof(IDXGISwapChain3),
		reinterpret_cast<void**>(&m_pSwapChain));
	pSwapChain1->Release();
	if (FAILED(hr))
		return false;

	m_pFactory->MakeWindowAssociation(hWindow, DXGI_MWA_NO_ALT_ENTER);
	m_hWindow = hWindow;
	m_width = width;
	m_height = height;
	if (CreateTargetViews())
		return true;
	ReleaseSwapChain();
	return false;
}

bool CDirectX12PresentationManager::CreateTargetViews()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = BACK_BUFFER_COUNT;
	if (FAILED(m_pDevice->CreateDescriptorHeap(
			&heapDesc,
			__uuidof(ID3D12DescriptorHeap),
			reinterpret_cast<void**>(&m_pRtvHeap))))
		return false;
	m_pRtvHeap->SetName(L"LastChaos D3D12 Presentation RTVs");
	m_rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE rtv =
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i)
	{
		if (FAILED(m_pSwapChain->GetBuffer(
				i,
				__uuidof(ID3D12Resource),
				reinterpret_cast<void**>(&m_apBackBuffers[i]))))
			return false;
		m_pDevice->CreateRenderTargetView(m_apBackBuffers[i], NULL, rtv);
		m_aBackBufferStates[i] = D3D12_RESOURCE_STATE_PRESENT;
		rtv.ptr += m_rtvDescriptorSize;
	}

	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = 1;
	if (FAILED(m_pDevice->CreateDescriptorHeap(
			&heapDesc,
			__uuidof(ID3D12DescriptorHeap),
			reinterpret_cast<void**>(&m_pDsvHeap))))
		return false;
	m_pDsvHeap->SetName(L"LastChaos D3D12 Presentation DSV");

	D3D12_CLEAR_VALUE clearValue;
	ZeroMemory(&clearValue, sizeof(clearValue));
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;

	D3D12_HEAP_PROPERTIES heapProperties;
	ZeroMemory(&heapProperties, sizeof(heapProperties));
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthDesc.Width = m_width;
	depthDesc.Height = m_height;
	depthDesc.DepthOrArraySize = 1;
	depthDesc.MipLevels = 1;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	if (FAILED(m_pDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			__uuidof(ID3D12Resource),
			reinterpret_cast<void**>(&m_pDepthBuffer))))
		return false;
	m_pDepthBuffer->SetName(L"LastChaos D3D12 Presentation Depth");
	m_pDevice->CreateDepthStencilView(
		m_pDepthBuffer,
		NULL,
		m_pDsvHeap->GetCPUDescriptorHandleForHeapStart());
	return true;
}

bool CDirectX12PresentationManager::AcquireCurrentTarget(
	ID3D12Resource** ppColorResource,
	D3D12_CPU_DESCRIPTOR_HANDLE* pColorView,
	ID3D12Resource** ppDepthResource,
	D3D12_CPU_DESCRIPTOR_HANDLE* pDepthView,
	D3D12_RESOURCE_STATES** ppColorState,
	bool* pClearTarget)
{
	if (m_pSwapChain == NULL || m_pRtvHeap == NULL
		|| m_pDepthBuffer == NULL || ppColorResource == NULL
		|| pColorView == NULL || ppDepthResource == NULL
		|| pDepthView == NULL || ppColorState == NULL
		|| pClearTarget == NULL)
		return false;

	const UINT index = m_pSwapChain->GetCurrentBackBufferIndex();
	if (index >= BACK_BUFFER_COUNT || m_apBackBuffers[index] == NULL)
		return false;
	*ppColorResource = m_apBackBuffers[index];
	*pColorView = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
	pColorView->ptr += static_cast<SIZE_T>(index) * m_rtvDescriptorSize;
	*ppDepthResource = m_pDepthBuffer;
	*pDepthView = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
	*ppColorState = &m_aBackBufferStates[index];
	*pClearTarget = m_clearTarget;
	m_clearTarget = false;
	return true;
}

bool CDirectX12PresentationManager::QueuePresent(
	ID3D12GraphicsCommandList* pCommandList)
{
	m_frameQueued = false;
	if (pCommandList == NULL || m_pSwapChain == NULL)
		return false;
	TransitionCurrentBackBuffer(
		pCommandList,
		D3D12_RESOURCE_STATE_PRESENT);
	m_frameQueued = true;
	return true;
}

bool CDirectX12PresentationManager::Present()
{
	if (!m_frameQueued || m_pSwapChain == NULL)
		return false;
	m_frameQueued = false;
	return SUCCEEDED(m_pSwapChain->Present(0, 0));
}

bool CDirectX12PresentationManager::HasDepthTarget() const
{
	return m_pDepthBuffer != NULL;
}

void CDirectX12PresentationManager::ReleaseSwapChain()
{
	if (m_pDepthBuffer != NULL)
	{
		m_pDepthBuffer->Release();
		m_pDepthBuffer = NULL;
	}
	for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i)
	{
		if (m_apBackBuffers[i] != NULL)
		{
			m_apBackBuffers[i]->Release();
			m_apBackBuffers[i] = NULL;
		}
		m_aBackBufferStates[i] = D3D12_RESOURCE_STATE_PRESENT;
	}
	if (m_pDsvHeap != NULL)
	{
		m_pDsvHeap->Release();
		m_pDsvHeap = NULL;
	}
	if (m_pRtvHeap != NULL)
	{
		m_pRtvHeap->Release();
		m_pRtvHeap = NULL;
	}
	if (m_pSwapChain != NULL)
	{
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	m_hWindow = NULL;
	m_width = 0;
	m_height = 0;
	m_rtvDescriptorSize = 0;
	m_frameQueued = false;
	m_clearTarget = false;
}

void CDirectX12PresentationManager::TransitionCurrentBackBuffer(
	ID3D12GraphicsCommandList* pCommandList,
	D3D12_RESOURCE_STATES after)
{
	const UINT index = m_pSwapChain->GetCurrentBackBufferIndex();
	if (index >= BACK_BUFFER_COUNT || m_apBackBuffers[index] == NULL
		|| m_aBackBufferStates[index] == after)
		return;
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_apBackBuffers[index];
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = m_aBackBufferStates[index];
	barrier.Transition.StateAfter = after;
	pCommandList->ResourceBarrier(1, &barrier);
	m_aBackBufferStates[index] = after;
}

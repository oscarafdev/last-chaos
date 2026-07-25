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
	, m_hWindow(NULL)
	, m_width(0)
	, m_height(0)
	, m_frameQueued(false)
{
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
}

bool CDirectX12PresentationManager::EnsureSwapChain(
	HWND hWindow,
	UINT width,
	UINT height,
	DXGI_FORMAT sourceFormat)
{
	if (m_pFactory == NULL || m_pGraphicsQueue == NULL
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
	desc.Format = sourceFormat;
	desc.SampleDesc.Count = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
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
	if (FAILED(hr) && sourceFormat == DXGI_FORMAT_B8G8R8X8_UNORM)
	{
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		hr = m_pFactory->CreateSwapChainForHwnd(
			m_pGraphicsQueue,
			hWindow,
			&desc,
			NULL,
			NULL,
			&pSwapChain1);
	}
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
	return true;
}

bool CDirectX12PresentationManager::QueueFrame(
	ID3D12GraphicsCommandList* pCommandList,
	ID3D12Resource* pSource,
	HWND hWindow)
{
	m_frameQueued = false;
	if (pCommandList == NULL || pSource == NULL)
		return false;

	const D3D12_RESOURCE_DESC sourceDesc = pSource->GetDesc();
	if (sourceDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| sourceDesc.SampleDesc.Count != 1
		|| !EnsureSwapChain(
			hWindow,
			static_cast<UINT>(sourceDesc.Width),
			sourceDesc.Height,
			sourceDesc.Format))
		return false;

	ID3D12Resource* pDestination = NULL;
	if (FAILED(m_pSwapChain->GetBuffer(
			m_pSwapChain->GetCurrentBackBufferIndex(),
			__uuidof(ID3D12Resource),
			reinterpret_cast<void**>(&pDestination))))
		return false;

	D3D12_RESOURCE_BARRIER barriers[2];
	ZeroMemory(barriers, sizeof(barriers));
	barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barriers[0].Transition.pResource = pSource;
	barriers[0].Transition.Subresource =
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barriers[1].Transition.pResource = pDestination;
	barriers[1].Transition.Subresource =
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	pCommandList->ResourceBarrier(2, barriers);
	pCommandList->CopyResource(pDestination, pSource);

	barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
	barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	pCommandList->ResourceBarrier(2, barriers);
	pDestination->Release();
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

void CDirectX12PresentationManager::ReleaseSwapChain()
{
	if (m_pSwapChain != NULL)
	{
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	m_hWindow = NULL;
	m_width = 0;
	m_height = 0;
	m_frameQueued = false;
}

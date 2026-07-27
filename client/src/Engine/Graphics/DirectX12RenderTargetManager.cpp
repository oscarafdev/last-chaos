#include "stdh.h"

#include <d3d9on12.h>

#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12Texture.h>

CDirectX12RenderTargetManager::CDirectX12RenderTargetManager()
	: m_pDevice(NULL)
	, m_pGraphicsQueue(NULL)
	, m_pDevice9On12(NULL)
	, m_pRtvHeap(NULL)
	, m_pDsvHeap(NULL)
	, m_pSurface9(NULL)
	, m_pDepthSurface9(NULL)
	, m_pResource12(NULL)
	, m_pDepthResource12(NULL)
	, m_pNativeTexture(NULL)
	, m_rtvDescriptorSize(0)
	, m_dsvDescriptorSize(0)
	, m_currentFrame(0)
	, m_currentSubmission(0)
	, m_isAcquired(false)
	, m_isDepthAcquired(false)
	, m_isNative(false)
	, m_clearNativeDepth(false)
{
}

CDirectX12RenderTargetManager::~CDirectX12RenderTargetManager()
{
	Shutdown();
}

bool CDirectX12RenderTargetManager::Initialize(
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

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors =
		FRAME_COUNT * MAX_SUBMISSIONS_PER_FRAME;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = m_pDevice->CreateDescriptorHeap(
		&heapDesc,
		__uuidof(ID3D12DescriptorHeap),
		reinterpret_cast<void**>(&m_pRtvHeap));
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	m_pRtvHeap->SetName(L"LastChaos D3D12 Backbuffer RTVs");
	m_rtvDescriptorSize =
		m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors =
		FRAME_COUNT * MAX_SUBMISSIONS_PER_FRAME;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_pDevice->CreateDescriptorHeap(
		&heapDesc,
		__uuidof(ID3D12DescriptorHeap),
		reinterpret_cast<void**>(&m_pDsvHeap));
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}
	m_pDsvHeap->SetName(L"LastChaos D3D12 D3D9On12 DSVs");
	m_dsvDescriptorSize =
		m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	return true;
}

bool CDirectX12RenderTargetManager::AttachD3D9Device(IDirect3DDevice9* pDevice9)
{
	if (pDevice9 == NULL || m_pRtvHeap == NULL || m_isAcquired)
		return false;

	if (m_pDevice9On12 != NULL)
	{
		m_pDevice9On12->Release();
		m_pDevice9On12 = NULL;
	}

	HRESULT hr = pDevice9->QueryInterface(
		__uuidof(IDirect3DDevice9On12),
		reinterpret_cast<void**>(&m_pDevice9On12));
	return SUCCEEDED(hr);
}

void CDirectX12RenderTargetManager::Shutdown()
{
	if (m_isAcquired && !m_isNative && m_pDevice9On12 != NULL)
	{
		if (m_isDepthAcquired)
			m_pDevice9On12->ReturnUnderlyingResource(
				reinterpret_cast<IDirect3DResource9*>(m_pDepthSurface9),
				0,
				NULL,
				NULL);
		m_pDevice9On12->ReturnUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(m_pSurface9),
			0,
			NULL,
			NULL);
	}
	ReleaseAcquiredReferences();

	if (m_pRtvHeap != NULL)
	{
		m_pRtvHeap->Release();
		m_pRtvHeap = NULL;
	}
	if (m_pDsvHeap != NULL)
	{
		m_pDsvHeap->Release();
		m_pDsvHeap = NULL;
	}
	if (m_pDevice9On12 != NULL)
	{
		m_pDevice9On12->Release();
		m_pDevice9On12 = NULL;
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

	m_rtvDescriptorSize = 0;
	m_dsvDescriptorSize = 0;
	m_currentFrame = 0;
	m_currentSubmission = 0;
}

bool CDirectX12RenderTargetManager::Acquire(
	IDirect3DSurface9* pSurface9,
	IDirect3DSurface9* pDepthSurface9,
	ID3D12GraphicsCommandList* pCommandList,
	UINT frameIndex,
	UINT submissionIndex)
{
	if (pSurface9 == NULL || pCommandList == NULL || m_pDevice9On12 == NULL
		|| m_isAcquired || frameIndex >= FRAME_COUNT
		|| submissionIndex >= MAX_SUBMISSIONS_PER_FRAME)
		return false;

	HRESULT hr = m_pDevice9On12->UnwrapUnderlyingResource(
		reinterpret_cast<IDirect3DResource9*>(pSurface9),
		m_pGraphicsQueue,
		__uuidof(ID3D12Resource),
		reinterpret_cast<void**>(&m_pResource12));
	if (FAILED(hr))
		return false;

	pSurface9->AddRef();
	m_pSurface9 = pSurface9;
	m_currentFrame = frameIndex;
	m_currentSubmission = submissionIndex;
	m_isAcquired = true;
	m_isNative = false;
	m_clearNativeDepth = false;

	const D3D12_RESOURCE_DESC resourceDesc = m_pResource12->GetDesc();
	if (resourceDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| !(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET))
	{
		m_pDevice9On12->ReturnUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(m_pSurface9),
			0,
			NULL,
			NULL);
		ReleaseAcquiredReferences();
		return false;
	}

	m_pDevice->CreateRenderTargetView(
		m_pResource12,
		NULL,
		GetCurrentView());
	Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	if (pDepthSurface9 != NULL)
	{
		hr = m_pDevice9On12->UnwrapUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(pDepthSurface9),
			m_pGraphicsQueue,
			__uuidof(ID3D12Resource),
			reinterpret_cast<void**>(&m_pDepthResource12));
		if (SUCCEEDED(hr))
		{
			const D3D12_RESOURCE_DESC depthDesc =
				m_pDepthResource12->GetDesc();
			DXGI_FORMAT dsvFormat = depthDesc.Format;
			if (dsvFormat == DXGI_FORMAT_R24G8_TYPELESS)
				dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			else if (dsvFormat == DXGI_FORMAT_R32_TYPELESS)
				dsvFormat = DXGI_FORMAT_D32_FLOAT;
			else if (dsvFormat == DXGI_FORMAT_R16_TYPELESS)
				dsvFormat = DXGI_FORMAT_D16_UNORM;

			if (depthDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D
				&& (depthDesc.Flags
					& D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL))
			{
				pDepthSurface9->AddRef();
				m_pDepthSurface9 = pDepthSurface9;
				m_isDepthAcquired = true;

				D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
				ZeroMemory(&dsvDesc, sizeof(dsvDesc));
				dsvDesc.Format = dsvFormat;
				dsvDesc.ViewDimension =
					depthDesc.SampleDesc.Count > 1
						? D3D12_DSV_DIMENSION_TEXTURE2DMS
						: D3D12_DSV_DIMENSION_TEXTURE2D;
				m_pDevice->CreateDepthStencilView(
					m_pDepthResource12,
					&dsvDesc,
					GetCurrentDepthView());
				D3D12_RESOURCE_BARRIER depthBarrier;
				ZeroMemory(&depthBarrier, sizeof(depthBarrier));
				depthBarrier.Type =
					D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				depthBarrier.Transition.pResource =
					m_pDepthResource12;
				depthBarrier.Transition.Subresource =
					D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				depthBarrier.Transition.StateBefore =
					D3D12_RESOURCE_STATE_COMMON;
				depthBarrier.Transition.StateAfter =
					D3D12_RESOURCE_STATE_DEPTH_WRITE;
				pCommandList->ResourceBarrier(1, &depthBarrier);
			}
			else
			{
				m_pDevice9On12->ReturnUnderlyingResource(
					reinterpret_cast<IDirect3DResource9*>(
						pDepthSurface9),
					0,
					NULL,
					NULL);
				m_pDepthResource12->Release();
				m_pDepthResource12 = NULL;
			}
		}
	}
	return true;
}

bool CDirectX12RenderTargetManager::AcquireNative(
	CDirectX12Texture* pTexture,
	ID3D12GraphicsCommandList* pCommandList,
	UINT frameIndex,
	UINT submissionIndex,
	bool clearColor,
	const FLOAT clearValue[4])
{
	if (pTexture == NULL || pTexture->GetResource() == NULL
		|| pTexture->GetRenderTargetView().ptr == 0
		|| pCommandList == NULL || m_pDevice == NULL
		|| m_isAcquired || frameIndex >= FRAME_COUNT
		|| submissionIndex >= MAX_SUBMISSIONS_PER_FRAME)
		return false;

	ID3D12Resource* pResource = pTexture->GetResource();
	const D3D12_RESOURCE_DESC resourceDesc = pResource->GetDesc();
	if (resourceDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| !(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET))
		return false;

	pResource->AddRef();
	m_pResource12 = pResource;
	m_pNativeTexture = pTexture;
	m_currentFrame = frameIndex;
	m_currentSubmission = submissionIndex;
	m_isAcquired = true;
	m_isNative = true;
	m_clearNativeDepth = clearColor;

	m_pNativeTexture->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	if (clearColor && clearValue != NULL)
		pCommandList->ClearRenderTargetView(
			GetCurrentView(),
			clearValue,
			0,
			NULL);
	return true;
}

bool CDirectX12RenderTargetManager::CopyCurrentColorTo(
	CDirectX12Texture* pDestination,
	ID3D12GraphicsCommandList* pCommandList)
{
	if (!m_isAcquired || m_pResource12 == NULL
		|| pDestination == NULL || pDestination->GetResource() == NULL
		|| pDestination->GetResource() == m_pResource12
		|| pCommandList == NULL)
		return false;

	const D3D12_RESOURCE_DESC sourceDesc = m_pResource12->GetDesc();
	const D3D12_RESOURCE_DESC destinationDesc =
		pDestination->GetResource()->GetDesc();
	if (sourceDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| destinationDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| sourceDesc.Width != destinationDesc.Width
		|| sourceDesc.Height != destinationDesc.Height
		|| sourceDesc.Format != destinationDesc.Format
		|| sourceDesc.SampleDesc.Count != 1
		|| destinationDesc.SampleDesc.Count != 1)
		return false;

	if (m_isNative && m_pNativeTexture != NULL)
	{
		m_pNativeTexture->Transition(
			pCommandList,
			D3D12_RESOURCE_STATE_COPY_SOURCE);
	}
	else
	{
		Transition(
			pCommandList,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_COPY_SOURCE);
	}
	pDestination->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_COPY_DEST);
	pCommandList->CopyResource(
		pDestination->GetResource(),
		m_pResource12);
	pDestination->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	if (m_isNative && m_pNativeTexture != NULL)
	{
		m_pNativeTexture->Transition(
			pCommandList,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
	else
	{
		Transition(
			pCommandList,
			D3D12_RESOURCE_STATE_COPY_SOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
	return true;
}

bool CDirectX12RenderTargetManager::PrepareForSubmission(
	ID3D12GraphicsCommandList* pCommandList)
{
	if (!m_isAcquired || pCommandList == NULL)
		return false;

	if (m_isNative && m_pNativeTexture != NULL)
	{
		m_pNativeTexture->Transition(
			pCommandList,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
	else
	{
		Transition(
			pCommandList,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_COMMON);
	}
	if (m_isDepthAcquired)
	{
		D3D12_RESOURCE_BARRIER depthBarrier;
		ZeroMemory(&depthBarrier, sizeof(depthBarrier));
		depthBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		depthBarrier.Transition.pResource = m_pDepthResource12;
		depthBarrier.Transition.Subresource =
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		depthBarrier.Transition.StateBefore =
			D3D12_RESOURCE_STATE_DEPTH_WRITE;
		depthBarrier.Transition.StateAfter =
			D3D12_RESOURCE_STATE_COMMON;
		pCommandList->ResourceBarrier(1, &depthBarrier);
	}
	return true;
}

bool CDirectX12RenderTargetManager::ReturnToD3D9(
	ID3D12Fence* pFence,
	UINT64 fenceValue)
{
	if (!m_isAcquired || pFence == NULL || fenceValue == 0)
		return false;
	if (m_isNative)
	{
		ReleaseAcquiredReferences();
		return true;
	}

	ID3D12Fence* fences[] = { pFence };
	UINT64 fenceValues[] = { fenceValue };
	bool depthSucceeded = true;
	if (m_isDepthAcquired)
	{
		depthSucceeded = SUCCEEDED(
			m_pDevice9On12->ReturnUnderlyingResource(
				reinterpret_cast<IDirect3DResource9*>(
					m_pDepthSurface9),
				1,
				fenceValues,
				fences));
	}
	HRESULT hr = m_pDevice9On12->ReturnUnderlyingResource(
		reinterpret_cast<IDirect3DResource9*>(m_pSurface9),
		1,
		fenceValues,
		fences);
	ReleaseAcquiredReferences();
	return SUCCEEDED(hr) && depthSucceeded;
}

bool CDirectX12RenderTargetManager::IsAcquired() const
{
	return m_isAcquired;
}

bool CDirectX12RenderTargetManager::HasAcquiredDepth() const
{
	return m_isDepthAcquired;
}

bool CDirectX12RenderTargetManager::IsNativeRenderTarget() const
{
	return m_isAcquired && m_isNative && m_pNativeTexture != NULL;
}

bool CDirectX12RenderTargetManager::ShouldClearNativeDepth() const
{
	return IsNativeRenderTarget() && m_clearNativeDepth;
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectX12RenderTargetManager::GetCurrentView() const
{
	if (m_isNative && m_pNativeTexture != NULL)
		return m_pNativeTexture->GetRenderTargetView();

	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
	const UINT descriptorIndex =
		m_currentFrame * MAX_SUBMISSIONS_PER_FRAME
		+ m_currentSubmission;
	handle.ptr +=
		static_cast<SIZE_T>(descriptorIndex) * m_rtvDescriptorSize;
	return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE
CDirectX12RenderTargetManager::GetCurrentDepthView() const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
	const UINT descriptorIndex =
		m_currentFrame * MAX_SUBMISSIONS_PER_FRAME
		+ m_currentSubmission;
	handle.ptr +=
		static_cast<SIZE_T>(descriptorIndex) * m_dsvDescriptorSize;
	return handle;
}

ID3D12Resource* CDirectX12RenderTargetManager::GetCurrentResource() const
{
	return m_pResource12;
}

ID3D12Resource*
CDirectX12RenderTargetManager::GetCurrentDepthResource() const
{
	return m_pDepthResource12;
}

void CDirectX12RenderTargetManager::ReleaseAcquiredReferences()
{
	if (m_pDepthResource12 != NULL)
	{
		m_pDepthResource12->Release();
		m_pDepthResource12 = NULL;
	}
	if (m_pDepthSurface9 != NULL)
	{
		m_pDepthSurface9->Release();
		m_pDepthSurface9 = NULL;
	}
	if (m_pResource12 != NULL)
	{
		m_pResource12->Release();
		m_pResource12 = NULL;
	}
	if (m_pSurface9 != NULL)
	{
		m_pSurface9->Release();
		m_pSurface9 = NULL;
	}
	m_isAcquired = false;
	m_isDepthAcquired = false;
	m_pNativeTexture = NULL;
	m_isNative = false;
	m_clearNativeDepth = false;
}

void CDirectX12RenderTargetManager::Transition(
	ID3D12GraphicsCommandList* pCommandList,
	D3D12_RESOURCE_STATES before,
	D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_pResource12;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	pCommandList->ResourceBarrier(1, &barrier);
}

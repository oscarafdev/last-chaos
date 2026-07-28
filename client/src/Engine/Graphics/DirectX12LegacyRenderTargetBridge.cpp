#include "stdh.h"

#include <d3d12.h>
#include <d3d9on12.h>

#include <Engine/Graphics/DirectX12LegacyRenderTargetBridge.h>

CDirectX12LegacyRenderTargetBridge::
CDirectX12LegacyRenderTargetBridge()
	: m_pGraphicsQueue(NULL)
	, m_pDevice9On12(NULL)
	, m_pColorSurface(NULL)
	, m_pDepthSurface(NULL)
	, m_pColorResource(NULL)
	, m_pDepthResource(NULL)
{
}

CDirectX12LegacyRenderTargetBridge::
~CDirectX12LegacyRenderTargetBridge()
{
	Shutdown();
}

bool CDirectX12LegacyRenderTargetBridge::Initialize(
	ID3D12CommandQueue* pGraphicsQueue)
{
	if (pGraphicsQueue == NULL)
		return false;

	Shutdown();
	m_pGraphicsQueue = pGraphicsQueue;
	m_pGraphicsQueue->AddRef();
	return true;
}

bool CDirectX12LegacyRenderTargetBridge::AttachD3D9Device(
	IDirect3DDevice9* pDevice9)
{
	if (pDevice9 == NULL || m_pGraphicsQueue == NULL || IsAcquired())
		return false;

	if (m_pDevice9On12 != NULL)
	{
		m_pDevice9On12->Release();
		m_pDevice9On12 = NULL;
	}
	return SUCCEEDED(pDevice9->QueryInterface(
		__uuidof(IDirect3DDevice9On12),
		reinterpret_cast<void**>(&m_pDevice9On12)));
}

void CDirectX12LegacyRenderTargetBridge::Shutdown()
{
	ReleaseImmediately();
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
}

bool CDirectX12LegacyRenderTargetBridge::Acquire(
	IDirect3DSurface9* pColorSurface,
	IDirect3DSurface9* pDepthSurface)
{
	if (pColorSurface == NULL || m_pDevice9On12 == NULL
		|| m_pGraphicsQueue == NULL || IsAcquired())
		return false;

	HRESULT hr = m_pDevice9On12->UnwrapUnderlyingResource(
		reinterpret_cast<IDirect3DResource9*>(pColorSurface),
		m_pGraphicsQueue,
		__uuidof(ID3D12Resource),
		reinterpret_cast<void**>(&m_pColorResource));
	if (FAILED(hr))
		return false;

	pColorSurface->AddRef();
	m_pColorSurface = pColorSurface;

	const D3D12_RESOURCE_DESC colorDesc = m_pColorResource->GetDesc();
	if (colorDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| !(colorDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET))
	{
		ReleaseImmediately();
		return false;
	}

	if (pDepthSurface != NULL)
	{
		hr = m_pDevice9On12->UnwrapUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(pDepthSurface),
			m_pGraphicsQueue,
			__uuidof(ID3D12Resource),
			reinterpret_cast<void**>(&m_pDepthResource));
		if (SUCCEEDED(hr))
		{
			const D3D12_RESOURCE_DESC depthDesc =
				m_pDepthResource->GetDesc();
			if (depthDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D
				&& (depthDesc.Flags
					& D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL))
			{
				pDepthSurface->AddRef();
				m_pDepthSurface = pDepthSurface;
			}
			else
			{
				m_pDevice9On12->ReturnUnderlyingResource(
					reinterpret_cast<IDirect3DResource9*>(pDepthSurface),
					0,
					NULL,
					NULL);
				m_pDepthResource->Release();
				m_pDepthResource = NULL;
			}
		}
	}
	return true;
}

bool CDirectX12LegacyRenderTargetBridge::ReturnToD3D9(
	ID3D12Fence* pFence,
	UINT64 fenceValue)
{
	if (!IsAcquired() || pFence == NULL || fenceValue == 0)
		return false;

	ID3D12Fence* fences[] = { pFence };
	UINT64 fenceValues[] = { fenceValue };
	bool depthSucceeded = true;
	if (HasDepth())
	{
		depthSucceeded = SUCCEEDED(
			m_pDevice9On12->ReturnUnderlyingResource(
				reinterpret_cast<IDirect3DResource9*>(m_pDepthSurface),
				1,
				fenceValues,
				fences));
	}
	const HRESULT colorResult =
		m_pDevice9On12->ReturnUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(m_pColorSurface),
			1,
			fenceValues,
			fences);
	ReleaseReferences();
	return SUCCEEDED(colorResult) && depthSucceeded;
}

void CDirectX12LegacyRenderTargetBridge::ReleaseImmediately()
{
	if (!IsAcquired())
		return;

	if (HasDepth())
	{
		m_pDevice9On12->ReturnUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(m_pDepthSurface),
			0,
			NULL,
			NULL);
	}
	m_pDevice9On12->ReturnUnderlyingResource(
		reinterpret_cast<IDirect3DResource9*>(m_pColorSurface),
		0,
		NULL,
		NULL);
	ReleaseReferences();
}

bool CDirectX12LegacyRenderTargetBridge::IsAcquired() const
{
	return m_pColorSurface != NULL && m_pColorResource != NULL;
}

bool CDirectX12LegacyRenderTargetBridge::HasDepth() const
{
	return m_pDepthSurface != NULL && m_pDepthResource != NULL;
}

ID3D12Resource*
CDirectX12LegacyRenderTargetBridge::GetColorResource() const
{
	return m_pColorResource;
}

ID3D12Resource*
CDirectX12LegacyRenderTargetBridge::GetDepthResource() const
{
	return m_pDepthResource;
}

void CDirectX12LegacyRenderTargetBridge::ReleaseReferences()
{
	if (m_pDepthResource != NULL)
	{
		m_pDepthResource->Release();
		m_pDepthResource = NULL;
	}
	if (m_pDepthSurface != NULL)
	{
		m_pDepthSurface->Release();
		m_pDepthSurface = NULL;
	}
	if (m_pColorResource != NULL)
	{
		m_pColorResource->Release();
		m_pColorResource = NULL;
	}
	if (m_pColorSurface != NULL)
	{
		m_pColorSurface->Release();
		m_pColorSurface = NULL;
	}
}

#include "stdh.h"

#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12Texture.h>

CDirectX12RenderTargetManager::CDirectX12RenderTargetManager()
	: m_pDevice(NULL)
	, m_pResource12(NULL)
	, m_pDepthResource12(NULL)
	, m_pNativeTexture(NULL)
	, m_pCurrentState(NULL)
	, m_isAcquired(false)
	, m_isDepthAcquired(false)
	, m_isNative(false)
	, m_clearNativeDepth(false)
{
	m_currentView.ptr = 0;
	m_currentDepthView.ptr = 0;
}

CDirectX12RenderTargetManager::~CDirectX12RenderTargetManager()
{
	Shutdown();
}

bool CDirectX12RenderTargetManager::Initialize(ID3D12Device* pDevice)
{
	if (pDevice == NULL)
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	return true;
}

void CDirectX12RenderTargetManager::Shutdown()
{
	ReleaseAcquiredReferences();

	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

}

bool CDirectX12RenderTargetManager::AcquirePresentation(
	ID3D12Resource* pColorResource,
	D3D12_CPU_DESCRIPTOR_HANDLE colorView,
	ID3D12Resource* pDepthResource,
	D3D12_CPU_DESCRIPTOR_HANDLE depthView,
	D3D12_RESOURCE_STATES* pColorState,
	ID3D12GraphicsCommandList* pCommandList,
	bool clearTarget)
{
	if (pColorResource == NULL || colorView.ptr == 0
		|| pDepthResource == NULL || depthView.ptr == 0
		|| pColorState == NULL || pCommandList == NULL
		|| m_isAcquired)
		return false;

	pColorResource->AddRef();
	pDepthResource->AddRef();
	m_pResource12 = pColorResource;
	m_pDepthResource12 = pDepthResource;
	m_currentView = colorView;
	m_currentDepthView = depthView;
	m_pCurrentState = pColorState;
	m_isAcquired = true;
	m_isDepthAcquired = true;
	m_isNative = false;
	m_clearNativeDepth = false;

	Transition(pCommandList, D3D12_RESOURCE_STATE_RENDER_TARGET);
	if (clearTarget)
	{
		const FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		pCommandList->ClearRenderTargetView(
			m_currentView, clearColor, 0, NULL);
		pCommandList->ClearDepthStencilView(
			m_currentDepthView,
			D3D12_CLEAR_FLAG_DEPTH,
			1.0f,
			0,
			0,
			NULL);
	}
	return true;
}

bool CDirectX12RenderTargetManager::AcquireNative(
	CDirectX12Texture* pTexture,
	ID3D12GraphicsCommandList* pCommandList,
	bool clearColor,
	const FLOAT clearValue[4])
{
	if (pTexture == NULL || pTexture->GetResource() == NULL
		|| pTexture->GetRenderTargetView().ptr == 0
		|| pCommandList == NULL || m_pDevice == NULL
		|| m_isAcquired)
		return false;

	ID3D12Resource* pResource = pTexture->GetResource();
	const D3D12_RESOURCE_DESC resourceDesc = pResource->GetDesc();
	if (resourceDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| !(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET))
		return false;

	pResource->AddRef();
	m_pResource12 = pResource;
	m_pNativeTexture = pTexture;
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
		Transition(pCommandList, D3D12_RESOURCE_STATE_COPY_SOURCE);
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
		Transition(pCommandList, D3D12_RESOURCE_STATE_RENDER_TARGET);
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
		Transition(pCommandList, D3D12_RESOURCE_STATE_COMMON);
	}
	return true;
}

void CDirectX12RenderTargetManager::ReleaseAfterSubmission()
{
	ReleaseAcquiredReferences();
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
	return m_currentView;
}

D3D12_CPU_DESCRIPTOR_HANDLE
CDirectX12RenderTargetManager::GetCurrentDepthView() const
{
	return m_currentDepthView;
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
	if (m_pResource12 != NULL)
	{
		m_pResource12->Release();
	}
	m_pResource12 = NULL;
	m_currentView.ptr = 0;
	m_currentDepthView.ptr = 0;
	m_pCurrentState = NULL;
	m_isAcquired = false;
	m_isDepthAcquired = false;
	m_pNativeTexture = NULL;
	m_isNative = false;
	m_clearNativeDepth = false;
}

void CDirectX12RenderTargetManager::Transition(
	ID3D12GraphicsCommandList* pCommandList,
	D3D12_RESOURCE_STATES after)
{
	if (m_pCurrentState == NULL || *m_pCurrentState == after)
		return;
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_pResource12;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = *m_pCurrentState;
	barrier.Transition.StateAfter = after;
	pCommandList->ResourceBarrier(1, &barrier);
	*m_pCurrentState = after;
}

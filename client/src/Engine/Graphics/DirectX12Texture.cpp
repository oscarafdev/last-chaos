#include "stdh.h"

#include <d3d12.h>

#include <Engine/Graphics/DirectX12Texture.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

CDirectX12Texture::CDirectX12Texture()
	: m_pResource(NULL)
	, m_pDescriptorHeap(NULL)
	, m_pRenderTargetDescriptorHeap(NULL)
	, m_width(0)
	, m_height(0)
	, m_mipLevels(0)
	, m_format(DXGI_FORMAT_UNKNOWN)
	, m_state(D3D12_RESOURCE_STATE_COMMON)
{
}

CDirectX12Texture::~CDirectX12Texture()
{
	Shutdown();
}

bool CDirectX12Texture::Create2D(
	ID3D12Device* pDevice,
	CDirectX12DescriptorHeap* pDescriptorHeap,
	UINT width,
	UINT height,
	UINT16 mipLevels,
	DXGI_FORMAT format,
	UINT componentMapping)
{
	return Create2DResource(
		pDevice,
		pDescriptorHeap,
		width,
		height,
		mipLevels,
		format,
		componentMapping,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_COPY_DEST,
		NULL,
		L"LastChaos D3D12 Texture 2D");
}

bool CDirectX12Texture::CreateRenderTarget2D(
	ID3D12Device* pDevice,
	CDirectX12DescriptorHeap* pResourceDescriptorHeap,
	CDirectX12DescriptorHeap* pRenderTargetDescriptorHeap,
	UINT width,
	UINT height,
	DXGI_FORMAT format,
	UINT componentMapping)
{
	D3D12_CLEAR_VALUE clearValue;
	ZeroMemory(&clearValue, sizeof(clearValue));
	clearValue.Format = format;
	if (pRenderTargetDescriptorHeap == NULL
		|| pRenderTargetDescriptorHeap->GetType()
			!= D3D12_DESCRIPTOR_HEAP_TYPE_RTV
		|| pRenderTargetDescriptorHeap->IsShaderVisible())
		return false;

	if (!Create2DResource(
		pDevice,
		pResourceDescriptorHeap,
		width,
		height,
		1,
		format,
		componentMapping,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		L"LastChaos D3D12 Render Texture"))
		return false;

	if (!pRenderTargetDescriptorHeap->Allocate(
			&m_renderTargetDescriptor))
	{
		Shutdown();
		return false;
	}
	pDevice->CreateRenderTargetView(
		m_pResource,
		NULL,
		m_renderTargetDescriptor.cpu);
	m_pRenderTargetDescriptorHeap = pRenderTargetDescriptorHeap;
	return true;
}

bool CDirectX12Texture::Create2DResource(
	ID3D12Device* pDevice,
	CDirectX12DescriptorHeap* pDescriptorHeap,
	UINT width,
	UINT height,
	UINT16 mipLevels,
	DXGI_FORMAT format,
	UINT componentMapping,
	D3D12_RESOURCE_FLAGS flags,
	D3D12_RESOURCE_STATES initialState,
	const D3D12_CLEAR_VALUE* pClearValue,
	const wchar_t* pDebugName)
{
	if (pDevice == NULL || pDescriptorHeap == NULL
		|| pDescriptorHeap->GetType()
			!= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		|| !pDescriptorHeap->IsShaderVisible()
		|| width == 0 || height == 0 || mipLevels == 0
		|| format == DXGI_FORMAT_UNKNOWN)
		return false;

	Shutdown();

	D3D12_HEAP_PROPERTIES heapProperties;
	ZeroMemory(&heapProperties, sizeof(heapProperties));
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc;
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = mipLevels;
	resourceDesc.Format = format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = flags;

	HRESULT hr = pDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		initialState,
		pClearValue,
		__uuidof(ID3D12Resource),
		reinterpret_cast<void**>(&m_pResource));
	if (FAILED(hr))
		return false;

	if (!pDescriptorHeap->Allocate(&m_descriptor))
	{
		Shutdown();
		return false;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = componentMapping;
	srvDesc.Texture2D.MipLevels = mipLevels;
	pDevice->CreateShaderResourceView(
		m_pResource,
		&srvDesc,
		m_descriptor.cpu);

	m_pDescriptorHeap = pDescriptorHeap;
	m_width = width;
	m_height = height;
	m_mipLevels = mipLevels;
	m_format = format;
	m_state = initialState;
	if (pDebugName != NULL)
		m_pResource->SetName(pDebugName);
	return true;
}

void CDirectX12Texture::Shutdown()
{
	if (m_pRenderTargetDescriptorHeap != NULL
		&& m_renderTargetDescriptor.IsValid())
	{
		m_pRenderTargetDescriptorHeap->Release(
			m_renderTargetDescriptor.index);
	}
	m_renderTargetDescriptor = DirectX12DescriptorHandle();
	m_pRenderTargetDescriptorHeap = NULL;

	if (m_pDescriptorHeap != NULL && m_descriptor.IsValid())
		m_pDescriptorHeap->Release(m_descriptor.index);
	m_descriptor = DirectX12DescriptorHandle();
	m_pDescriptorHeap = NULL;

	if (m_pResource != NULL)
	{
		m_pResource->Release();
		m_pResource = NULL;
	}

	m_width = 0;
	m_height = 0;
	m_mipLevels = 0;
	m_format = DXGI_FORMAT_UNKNOWN;
	m_state = D3D12_RESOURCE_STATE_COMMON;
}

bool CDirectX12Texture::Upload(
	CDirectX12UploadManager* pUploadManager,
	ID3D12GraphicsCommandList* pCommandList,
	UINT firstMip,
	UINT mipCount,
	const DirectX12SubresourceData* pSubresources)
{
	if (pUploadManager == NULL || m_pResource == NULL
		|| firstMip >= m_mipLevels
		|| mipCount == 0
		|| mipCount > m_mipLevels - firstMip)
		return false;

	const D3D12_RESOURCE_STATES shaderResourceState =
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		| D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	if (!pUploadManager->UploadTexture(
		pCommandList,
		m_pResource,
		firstMip,
		mipCount,
		pSubresources,
		m_state,
		shaderResourceState))
		return false;

	m_state = shaderResourceState;
	return true;
}

void CDirectX12Texture::Transition(
	ID3D12GraphicsCommandList* pCommandList,
	D3D12_RESOURCE_STATES newState)
{
	if (pCommandList == NULL || m_pResource == NULL
		|| m_state == newState)
		return;

	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_pResource;
	barrier.Transition.Subresource =
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = m_state;
	barrier.Transition.StateAfter = newState;
	pCommandList->ResourceBarrier(1, &barrier);
	m_state = newState;
}

ID3D12Resource* CDirectX12Texture::GetResource() const
{
	return m_pResource;
}

D3D12_GPU_DESCRIPTOR_HANDLE
CDirectX12Texture::GetShaderResourceView() const
{
	return m_descriptor.gpu;
}

D3D12_CPU_DESCRIPTOR_HANDLE
CDirectX12Texture::GetRenderTargetView() const
{
	return m_renderTargetDescriptor.cpu;
}

UINT CDirectX12Texture::GetDescriptorIndex() const
{
	return m_descriptor.index;
}

UINT CDirectX12Texture::GetWidth() const
{
	return m_width;
}

UINT CDirectX12Texture::GetHeight() const
{
	return m_height;
}

UINT16 CDirectX12Texture::GetMipLevels() const
{
	return m_mipLevels;
}

DXGI_FORMAT CDirectX12Texture::GetFormat() const
{
	return m_format;
}

D3D12_RESOURCE_STATES CDirectX12Texture::GetState() const
{
	return m_state;
}

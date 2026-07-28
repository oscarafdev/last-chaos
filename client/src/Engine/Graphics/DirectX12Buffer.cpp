#include "stdh.h"

#include <d3d12.h>

#include <Engine/Graphics/DirectX12Buffer.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

CDirectX12Buffer::CDirectX12Buffer()
	: m_pResource(NULL)
	, m_size(0)
	, m_stride(0)
	, m_indexFormat(DXGI_FORMAT_UNKNOWN)
	, m_state(D3D12_RESOURCE_STATE_COMMON)
	, m_kind(BK_NONE)
{
}

CDirectX12Buffer::~CDirectX12Buffer()
{
	Shutdown();
}

bool CDirectX12Buffer::CreateVertexBuffer(
	ID3D12Device* pDevice,
	UINT64 size,
	UINT stride)
{
	return stride > 0 && Create(
		pDevice,
		size,
		BK_VERTEX,
		stride,
		DXGI_FORMAT_UNKNOWN);
}

bool CDirectX12Buffer::CreateIndexBuffer(
	ID3D12Device* pDevice,
	UINT64 size,
	DXGI_FORMAT format)
{
	if (format != DXGI_FORMAT_R16_UINT && format != DXGI_FORMAT_R32_UINT)
		return false;

	return Create(pDevice, size, BK_INDEX, 0, format);
}

bool CDirectX12Buffer::Create(
	ID3D12Device* pDevice,
	UINT64 size,
	BufferKind kind,
	UINT stride,
	DXGI_FORMAT indexFormat)
{
	if (pDevice == NULL || size == 0 || size > UINT_MAX || kind == BK_NONE)
		return false;

	Shutdown();

	D3D12_HEAP_PROPERTIES heapProperties;
	ZeroMemory(&heapProperties, sizeof(heapProperties));
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc;
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = size;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = pDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		NULL,
		__uuidof(ID3D12Resource),
		reinterpret_cast<void**>(&m_pResource));
	if (FAILED(hr))
		return false;

	m_size = size;
	m_stride = stride;
	m_indexFormat = indexFormat;
	m_state = D3D12_RESOURCE_STATE_COPY_DEST;
	m_kind = kind;
	if (kind == BK_VERTEX)
	{
		m_vertexHandle = GetDirectX12ResourceRegistry().
			Allocate<DX12_RESOURCE_VERTEX_BUFFER>(this);
		if (!m_vertexHandle.IsValid())
		{
			Shutdown();
			return false;
		}
	}
	else
	{
		m_indexHandle = GetDirectX12ResourceRegistry().
			Allocate<DX12_RESOURCE_INDEX_BUFFER>(this);
		if (!m_indexHandle.IsValid())
		{
			Shutdown();
			return false;
		}
	}
	m_pResource->SetName(
		kind == BK_VERTEX
			? L"LastChaos D3D12 Vertex Buffer"
			: L"LastChaos D3D12 Index Buffer");
	return true;
}

void CDirectX12Buffer::Shutdown()
{
	if (m_vertexHandle.IsValid())
	{
		GetDirectX12ResourceRegistry().Release(m_vertexHandle);
		m_vertexHandle = DirectX12VertexBufferHandle();
	}
	if (m_indexHandle.IsValid())
	{
		GetDirectX12ResourceRegistry().Release(m_indexHandle);
		m_indexHandle = DirectX12IndexBufferHandle();
	}

	if (m_pResource != NULL)
	{
		m_pResource->Release();
		m_pResource = NULL;
	}

	m_size = 0;
	m_stride = 0;
	m_indexFormat = DXGI_FORMAT_UNKNOWN;
	m_state = D3D12_RESOURCE_STATE_COMMON;
	m_kind = BK_NONE;
}

bool CDirectX12Buffer::Upload(
	CDirectX12UploadManager* pUploadManager,
	ID3D12GraphicsCommandList* pCommandList,
	const void* pData,
	UINT64 dataSize,
	UINT64 destinationOffset)
{
	if (pUploadManager == NULL || m_pResource == NULL)
		return false;

	const D3D12_RESOURCE_STATES usageState = GetUsageState();
	if (!pUploadManager->UploadBuffer(
		pCommandList,
		m_pResource,
		destinationOffset,
		pData,
		dataSize,
		m_state,
		usageState))
		return false;

	m_state = usageState;
	return true;
}

ID3D12Resource* CDirectX12Buffer::GetResource() const
{
	return m_pResource;
}

UINT64 CDirectX12Buffer::GetSize() const
{
	return m_size;
}

D3D12_RESOURCE_STATES CDirectX12Buffer::GetState() const
{
	return m_state;
}

D3D12_VERTEX_BUFFER_VIEW CDirectX12Buffer::GetVertexView() const
{
	D3D12_VERTEX_BUFFER_VIEW view;
	ZeroMemory(&view, sizeof(view));
	if (m_pResource != NULL && m_kind == BK_VERTEX)
	{
		view.BufferLocation = m_pResource->GetGPUVirtualAddress();
		view.SizeInBytes = static_cast<UINT>(m_size);
		view.StrideInBytes = m_stride;
	}
	return view;
}

D3D12_INDEX_BUFFER_VIEW CDirectX12Buffer::GetIndexView() const
{
	D3D12_INDEX_BUFFER_VIEW view;
	ZeroMemory(&view, sizeof(view));
	if (m_pResource != NULL && m_kind == BK_INDEX)
	{
		view.BufferLocation = m_pResource->GetGPUVirtualAddress();
		view.SizeInBytes = static_cast<UINT>(m_size);
		view.Format = m_indexFormat;
	}
	return view;
}

DirectX12VertexBufferHandle CDirectX12Buffer::GetVertexHandle() const
{
	return m_vertexHandle;
}

DirectX12IndexBufferHandle CDirectX12Buffer::GetIndexHandle() const
{
	return m_indexHandle;
}

D3D12_RESOURCE_STATES CDirectX12Buffer::GetUsageState() const
{
	if (m_kind == BK_VERTEX)
		return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	if (m_kind == BK_INDEX)
		return D3D12_RESOURCE_STATE_INDEX_BUFFER;
	return D3D12_RESOURCE_STATE_COMMON;
}

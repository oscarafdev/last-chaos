#include "stdh.h"

#include <Engine/Graphics/DirectX12DepthBuffer.h>

namespace
{
	const DXGI_FORMAT DEPTH_FORMAT = DXGI_FORMAT_D32_FLOAT;
}

CDirectX12DepthBuffer::CDirectX12DepthBuffer()
	: m_pDevice(NULL)
	, m_pDsvHeap(NULL)
	, m_pResource(NULL)
	, m_width(0)
	, m_height(0)
{
	ZeroMemory(&m_sampleDesc, sizeof(m_sampleDesc));
}

CDirectX12DepthBuffer::~CDirectX12DepthBuffer()
{
	Shutdown();
}

bool CDirectX12DepthBuffer::Initialize(ID3D12Device* pDevice)
{
	if (pDevice == NULL)
		return false;
	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	const HRESULT hr = m_pDevice->CreateDescriptorHeap(
		&heapDesc,
		__uuidof(ID3D12DescriptorHeap),
		reinterpret_cast<void**>(&m_pDsvHeap));
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}
	m_pDsvHeap->SetName(L"LastChaos D3D12 Legacy 3D DSV");
	return true;
}

void CDirectX12DepthBuffer::Shutdown()
{
	if (m_pResource != NULL)
	{
		m_pResource->Release();
		m_pResource = NULL;
	}
	if (m_pDsvHeap != NULL)
	{
		m_pDsvHeap->Release();
		m_pDsvHeap = NULL;
	}
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	m_width = 0;
	m_height = 0;
	ZeroMemory(&m_sampleDesc, sizeof(m_sampleDesc));
}

bool CDirectX12DepthBuffer::EnsureCompatible(
	const D3D12_RESOURCE_DESC& renderTargetDesc)
{
	if (m_pDevice == NULL || m_pDsvHeap == NULL
		|| renderTargetDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| renderTargetDesc.Width == 0 || renderTargetDesc.Height == 0
		|| renderTargetDesc.SampleDesc.Count == 0)
		return false;

	if (m_pResource != NULL
		&& m_width == renderTargetDesc.Width
		&& m_height == renderTargetDesc.Height
		&& m_sampleDesc.Count == renderTargetDesc.SampleDesc.Count
		&& m_sampleDesc.Quality == renderTargetDesc.SampleDesc.Quality)
		return true;

	if (m_pResource != NULL)
	{
		m_pResource->Release();
		m_pResource = NULL;
	}

	D3D12_CLEAR_VALUE clearValue;
	ZeroMemory(&clearValue, sizeof(clearValue));
	clearValue.Format = DEPTH_FORMAT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	D3D12_HEAP_PROPERTIES heapProperties;
	ZeroMemory(&heapProperties, sizeof(heapProperties));
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc;
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = renderTargetDesc.Width;
	resourceDesc.Height = renderTargetDesc.Height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DEPTH_FORMAT;
	resourceDesc.SampleDesc = renderTargetDesc.SampleDesc;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	const HRESULT hr = m_pDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		__uuidof(ID3D12Resource),
		reinterpret_cast<void**>(&m_pResource));
	if (FAILED(hr))
		return false;

	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = DEPTH_FORMAT;
	viewDesc.ViewDimension =
		renderTargetDesc.SampleDesc.Count > 1
			? D3D12_DSV_DIMENSION_TEXTURE2DMS
			: D3D12_DSV_DIMENSION_TEXTURE2D;
	m_pDevice->CreateDepthStencilView(
		m_pResource,
		&viewDesc,
		GetView());
	m_pResource->SetName(L"LastChaos D3D12 Legacy 3D Depth Buffer");
	m_width = renderTargetDesc.Width;
	m_height = renderTargetDesc.Height;
	m_sampleDesc = renderTargetDesc.SampleDesc;
	return true;
}

ID3D12Resource* CDirectX12DepthBuffer::GetResource() const
{
	return m_pResource;
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectX12DepthBuffer::GetView() const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = { 0 };
	if (m_pDsvHeap != NULL)
		handle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
	return handle;
}

DXGI_FORMAT CDirectX12DepthBuffer::GetFormat() const
{
	return DEPTH_FORMAT;
}

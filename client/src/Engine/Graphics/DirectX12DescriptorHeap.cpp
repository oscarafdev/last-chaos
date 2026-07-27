#include "stdh.h"

#include <vector>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12DescriptorHeap.h>

const UINT INVALID_DESCRIPTOR_INDEX = UINT_MAX;

struct DirectX12DescriptorHeapState
{
	DirectX12DescriptorHeapState()
		: nextIndex(0)
		, allocatedCount(0)
	{
	}

	std::vector<UINT> freeIndices;
	std::vector<bool> allocated;
	UINT nextIndex;
	UINT allocatedCount;
};

DirectX12DescriptorHandle::DirectX12DescriptorHandle()
	: index(INVALID_DESCRIPTOR_INDEX)
{
	cpu.ptr = 0;
	gpu.ptr = 0;
}

bool DirectX12DescriptorHandle::IsValid() const
{
	return index != INVALID_DESCRIPTOR_INDEX;
}

CDirectX12DescriptorHeap::CDirectX12DescriptorHeap()
	: m_pHeap(NULL)
	, m_pState(NULL)
	, m_type(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES)
	, m_capacity(0)
	, m_descriptorSize(0)
	, m_shaderVisible(false)
{
}

CDirectX12DescriptorHeap::~CDirectX12DescriptorHeap()
{
	Shutdown();
}

bool CDirectX12DescriptorHeap::Initialize(
	ID3D12Device* pDevice,
	D3D12_DESCRIPTOR_HEAP_TYPE type,
	UINT capacity,
	bool shaderVisible)
{
	if (pDevice == NULL || type >= D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES
		|| capacity == 0)
		return false;

	if (shaderVisible
		&& type != D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		&& type != D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
		return false;

	Shutdown();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.Type = type;
	heapDesc.NumDescriptors = capacity;
	heapDesc.Flags = shaderVisible
		? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		: D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = pDevice->CreateDescriptorHeap(
		&heapDesc,
		__uuidof(ID3D12DescriptorHeap),
		reinterpret_cast<void**>(&m_pHeap));
	if (FAILED(hr))
		return false;

	m_pState = new DirectX12DescriptorHeapState;
	if (m_pState == NULL)
	{
		Shutdown();
		return false;
	}

	m_type = type;
	m_capacity = capacity;
	m_descriptorSize = pDevice->GetDescriptorHandleIncrementSize(type);
	m_shaderVisible = shaderVisible;
	m_pState->allocated.resize(capacity, false);
	const wchar_t* pHeapName =
		L"LastChaos D3D12 Resource Descriptor Heap";
	if (type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
		pHeapName = L"LastChaos D3D12 Sampler Descriptor Heap";
	else if (type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		pHeapName = L"LastChaos D3D12 Render Target Descriptor Heap";
	else if (type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
		pHeapName = L"LastChaos D3D12 Depth Descriptor Heap";
	m_pHeap->SetName(pHeapName);
	return true;
}

void CDirectX12DescriptorHeap::Shutdown()
{
	if (m_pHeap != NULL)
	{
		m_pHeap->Release();
		m_pHeap = NULL;
	}

	delete m_pState;
	m_pState = NULL;
	m_type = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
	m_capacity = 0;
	m_descriptorSize = 0;
	m_shaderVisible = false;
}

bool CDirectX12DescriptorHeap::Allocate(
	DirectX12DescriptorHandle* pHandle)
{
	if (pHandle == NULL || m_pHeap == NULL || m_pState == NULL)
		return false;

	UINT index = INVALID_DESCRIPTOR_INDEX;
	if (!m_pState->freeIndices.empty())
	{
		index = m_pState->freeIndices.back();
		m_pState->freeIndices.pop_back();
	}
	else if (m_pState->nextIndex < m_capacity)
	{
		index = m_pState->nextIndex++;
	}
	else
	{
		return false;
	}

	pHandle->index = index;
	pHandle->cpu = m_pHeap->GetCPUDescriptorHandleForHeapStart();
	pHandle->cpu.ptr += static_cast<SIZE_T>(index) * m_descriptorSize;
	pHandle->gpu.ptr = 0;
	if (m_shaderVisible)
	{
		pHandle->gpu = m_pHeap->GetGPUDescriptorHandleForHeapStart();
		pHandle->gpu.ptr += static_cast<UINT64>(index) * m_descriptorSize;
	}

	m_pState->allocated[index] = true;
	++m_pState->allocatedCount;
	return true;
}

void CDirectX12DescriptorHeap::Release(UINT index)
{
	if (m_pState == NULL || index >= m_pState->nextIndex
		|| !m_pState->allocated[index])
		return;

	m_pState->allocated[index] = false;
	m_pState->freeIndices.push_back(index);
	if (m_pState->allocatedCount > 0)
		--m_pState->allocatedCount;
}

ID3D12DescriptorHeap* CDirectX12DescriptorHeap::GetHeap() const
{
	return m_pHeap;
}

D3D12_DESCRIPTOR_HEAP_TYPE CDirectX12DescriptorHeap::GetType() const
{
	return m_type;
}

UINT CDirectX12DescriptorHeap::GetCapacity() const
{
	return m_capacity;
}

UINT CDirectX12DescriptorHeap::GetAllocatedCount() const
{
	return m_pState != NULL ? m_pState->allocatedCount : 0;
}

bool CDirectX12DescriptorHeap::IsShaderVisible() const
{
	return m_shaderVisible;
}

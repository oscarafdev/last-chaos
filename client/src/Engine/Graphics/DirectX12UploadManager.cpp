#include "stdh.h"

#include <algorithm>
#include <vector>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	const UINT64 UPLOAD_ALIGNMENT = D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT;

	UINT64 AlignUploadSize(UINT64 size)
	{
		return (size + UPLOAD_ALIGNMENT - 1) & ~(UPLOAD_ALIGNMENT - 1);
	}

	struct DirectX12UploadPage
	{
		DirectX12UploadPage()
			: pResource(NULL)
			, pCpuAddress(NULL)
			, capacity(0)
			, offset(0)
		{
		}

		ID3D12Resource* pResource;
		BYTE* pCpuAddress;
		UINT64 capacity;
		UINT64 offset;
	};
}

struct DirectX12UploadFramePool
{
	std::vector<DirectX12UploadPage*> pages;
};

CDirectX12UploadManager::CDirectX12UploadManager()
	: m_pDevice(NULL)
	, m_pFrames(NULL)
	, m_frameCount(0)
	, m_currentFrame(0)
	, m_defaultPageSize(0)
	, m_frameActive(false)
{
}

CDirectX12UploadManager::~CDirectX12UploadManager()
{
	Shutdown();
}

bool CDirectX12UploadManager::Initialize(
	ID3D12Device* pDevice,
	UINT frameCount,
	UINT64 defaultPageSize)
{
	if (pDevice == NULL || frameCount == 0 || defaultPageSize == 0)
		return false;

	Shutdown();

	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pFrames = new DirectX12UploadFramePool[frameCount];
	if (m_pFrames == NULL)
	{
		Shutdown();
		return false;
	}

	m_frameCount = frameCount;
	m_defaultPageSize = AlignUploadSize(defaultPageSize);
	return true;
}

void CDirectX12UploadManager::Shutdown()
{
	if (m_pFrames != NULL)
	{
		for (UINT iFrame = 0; iFrame < m_frameCount; ++iFrame)
		{
			DirectX12UploadFramePool& frame = m_pFrames[iFrame];
			for (size_t iPage = 0; iPage < frame.pages.size(); ++iPage)
			{
				DirectX12UploadPage* pPage = frame.pages[iPage];
				if (pPage->pResource != NULL)
				{
					if (pPage->pCpuAddress != NULL)
						pPage->pResource->Unmap(0, NULL);
					pPage->pResource->Release();
				}
				delete pPage;
			}
			frame.pages.clear();
		}
		delete[] m_pFrames;
		m_pFrames = NULL;
	}

	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	m_frameCount = 0;
	m_currentFrame = 0;
	m_defaultPageSize = 0;
	m_frameActive = false;
}

bool CDirectX12UploadManager::BeginFrame(UINT frameIndex)
{
	if (m_pFrames == NULL || frameIndex >= m_frameCount)
		return false;

	DirectX12UploadFramePool& frame = m_pFrames[frameIndex];
	for (size_t iPage = 0; iPage < frame.pages.size(); ++iPage)
		frame.pages[iPage]->offset = 0;

	m_currentFrame = frameIndex;
	m_frameActive = true;
	return true;
}

void CDirectX12UploadManager::EndFrame()
{
	m_frameActive = false;
}

bool CDirectX12UploadManager::UploadBuffer(
	ID3D12GraphicsCommandList* pCommandList,
	ID3D12Resource* pDestination,
	UINT64 destinationOffset,
	const void* pData,
	UINT64 dataSize,
	D3D12_RESOURCE_STATES stateBefore,
	D3D12_RESOURCE_STATES stateAfter)
{
	if (!m_frameActive || pCommandList == NULL || pDestination == NULL
		|| pData == NULL || dataSize == 0)
		return false;

	const D3D12_RESOURCE_DESC destinationDesc = pDestination->GetDesc();
	if (destinationDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER
		|| destinationOffset > destinationDesc.Width
		|| dataSize > destinationDesc.Width - destinationOffset)
		return false;

	ID3D12Resource* pUploadResource = NULL;
	UINT64 uploadOffset = 0;
	void* pCpuAddress = NULL;
	if (!Allocate(dataSize, &pUploadResource, &uploadOffset, &pCpuAddress))
		return false;

	memcpy(pCpuAddress, pData, static_cast<size_t>(dataSize));

	if (stateBefore != D3D12_RESOURCE_STATE_COPY_DEST)
	{
		D3D12_RESOURCE_BARRIER barrier;
		ZeroMemory(&barrier, sizeof(barrier));
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = pDestination;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = stateBefore;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		pCommandList->ResourceBarrier(1, &barrier);
	}

	pCommandList->CopyBufferRegion(
		pDestination,
		destinationOffset,
		pUploadResource,
		uploadOffset,
		dataSize);

	if (stateAfter != D3D12_RESOURCE_STATE_COPY_DEST)
	{
		D3D12_RESOURCE_BARRIER barrier;
		ZeroMemory(&barrier, sizeof(barrier));
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = pDestination;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = stateAfter;
		pCommandList->ResourceBarrier(1, &barrier);
	}

	return true;
}

bool CDirectX12UploadManager::UploadTexture(
	ID3D12GraphicsCommandList* pCommandList,
	ID3D12Resource* pDestination,
	UINT firstSubresource,
	UINT subresourceCount,
	const DirectX12SubresourceData* pSubresources,
	D3D12_RESOURCE_STATES stateBefore,
	D3D12_RESOURCE_STATES stateAfter)
{
	if (!m_frameActive || pCommandList == NULL || pDestination == NULL
		|| pSubresources == NULL || subresourceCount == 0)
		return false;

	const D3D12_RESOURCE_DESC destinationDesc = pDestination->GetDesc();
	const UINT totalSubresources =
		destinationDesc.MipLevels * destinationDesc.DepthOrArraySize;
	if (destinationDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| firstSubresource >= totalSubresources
		|| subresourceCount > totalSubresources - firstSubresource)
		return false;

	std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> layouts(subresourceCount);
	std::vector<UINT> rowCounts(subresourceCount);
	std::vector<UINT64> rowSizes(subresourceCount);
	UINT64 requiredSize = 0;
	m_pDevice->GetCopyableFootprints(
		&destinationDesc,
		firstSubresource,
		subresourceCount,
		0,
		&layouts[0],
		&rowCounts[0],
		&rowSizes[0],
		&requiredSize);
	if (requiredSize == 0)
		return false;

	ID3D12Resource* pUploadResource = NULL;
	UINT64 uploadOffset = 0;
	void* pCpuAddress = NULL;
	if (!Allocate(
		requiredSize,
		&pUploadResource,
		&uploadOffset,
		&pCpuAddress))
		return false;

	m_pDevice->GetCopyableFootprints(
		&destinationDesc,
		firstSubresource,
		subresourceCount,
		uploadOffset,
		&layouts[0],
		&rowCounts[0],
		&rowSizes[0],
		NULL);

	BYTE* pUploadBase =
		static_cast<BYTE*>(pCpuAddress) - uploadOffset;
	for (UINT iSubresource = 0;
		iSubresource < subresourceCount;
		++iSubresource)
	{
		const DirectX12SubresourceData& source =
			pSubresources[iSubresource];
		const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout =
			layouts[iSubresource];
		if (source.pData == NULL || source.rowPitch <= 0
			|| source.slicePitch <= 0
			|| static_cast<UINT64>(source.rowPitch)
				< rowSizes[iSubresource])
			return false;

		BYTE* pDestinationSlice = pUploadBase + layout.Offset;
		const BYTE* pSourceSlice =
			static_cast<const BYTE*>(source.pData);
		for (UINT iDepth = 0;
			iDepth < layout.Footprint.Depth;
			++iDepth)
		{
			BYTE* pDestinationRow = pDestinationSlice
				+ static_cast<SIZE_T>(iDepth)
				* layout.Footprint.RowPitch
				* rowCounts[iSubresource];
			const BYTE* pSourceRow = pSourceSlice
				+ static_cast<SIZE_T>(iDepth) * source.slicePitch;
			for (UINT iRow = 0;
				iRow < rowCounts[iSubresource];
				++iRow)
			{
				memcpy(
					pDestinationRow,
					pSourceRow,
					static_cast<size_t>(rowSizes[iSubresource]));
				pDestinationRow += layout.Footprint.RowPitch;
				pSourceRow += source.rowPitch;
			}
		}
	}

	if (stateBefore != D3D12_RESOURCE_STATE_COPY_DEST)
	{
		D3D12_RESOURCE_BARRIER barrier;
		ZeroMemory(&barrier, sizeof(barrier));
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = pDestination;
		barrier.Transition.Subresource =
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = stateBefore;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		pCommandList->ResourceBarrier(1, &barrier);
	}

	for (UINT iSubresource = 0;
		iSubresource < subresourceCount;
		++iSubresource)
	{
		D3D12_TEXTURE_COPY_LOCATION destinationLocation;
		ZeroMemory(&destinationLocation, sizeof(destinationLocation));
		destinationLocation.pResource = pDestination;
		destinationLocation.Type =
			D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		destinationLocation.SubresourceIndex =
			firstSubresource + iSubresource;

		D3D12_TEXTURE_COPY_LOCATION sourceLocation;
		ZeroMemory(&sourceLocation, sizeof(sourceLocation));
		sourceLocation.pResource = pUploadResource;
		sourceLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		sourceLocation.PlacedFootprint = layouts[iSubresource];

		pCommandList->CopyTextureRegion(
			&destinationLocation,
			0,
			0,
			0,
			&sourceLocation,
			NULL);
	}

	if (stateAfter != D3D12_RESOURCE_STATE_COPY_DEST)
	{
		D3D12_RESOURCE_BARRIER barrier;
		ZeroMemory(&barrier, sizeof(barrier));
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = pDestination;
		barrier.Transition.Subresource =
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = stateAfter;
		pCommandList->ResourceBarrier(1, &barrier);
	}

	return true;
}

bool CDirectX12UploadManager::Allocate(
	UINT64 size,
	ID3D12Resource** ppResource,
	UINT64* pOffset,
	void** ppCpuAddress)
{
	if (!m_frameActive || ppResource == NULL || pOffset == NULL
		|| ppCpuAddress == NULL)
		return false;

	const UINT64 alignedSize = AlignUploadSize(size);
	DirectX12UploadFramePool& frame = m_pFrames[m_currentFrame];

	DirectX12UploadPage* pSelectedPage = NULL;
	for (size_t iPage = 0; iPage < frame.pages.size(); ++iPage)
	{
		DirectX12UploadPage* pPage = frame.pages[iPage];
		if (alignedSize <= pPage->capacity - pPage->offset)
		{
			pSelectedPage = pPage;
			break;
		}
	}

	if (pSelectedPage == NULL)
	{
		pSelectedPage = new DirectX12UploadPage;
		if (pSelectedPage == NULL)
			return false;

		pSelectedPage->capacity = (std::max)(m_defaultPageSize, alignedSize);

		D3D12_HEAP_PROPERTIES heapProperties;
		ZeroMemory(&heapProperties, sizeof(heapProperties));
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CreationNodeMask = 1;
		heapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC resourceDesc;
		ZeroMemory(&resourceDesc, sizeof(resourceDesc));
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = pSelectedPage->capacity;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		HRESULT hr = m_pDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			NULL,
			__uuidof(ID3D12Resource),
			reinterpret_cast<void**>(&pSelectedPage->pResource));
		if (FAILED(hr))
		{
			delete pSelectedPage;
			return false;
		}

		D3D12_RANGE readRange = { 0, 0 };
		hr = pSelectedPage->pResource->Map(
			0,
			&readRange,
			reinterpret_cast<void**>(&pSelectedPage->pCpuAddress));
		if (FAILED(hr))
		{
			pSelectedPage->pResource->Release();
			delete pSelectedPage;
			return false;
		}

		pSelectedPage->pResource->SetName(L"LastChaos D3D12 Upload Page");
		frame.pages.push_back(pSelectedPage);
	}

	*ppResource = pSelectedPage->pResource;
	*pOffset = pSelectedPage->offset;
	*ppCpuAddress = pSelectedPage->pCpuAddress + pSelectedPage->offset;
	pSelectedPage->offset += alignedSize;
	return true;
}

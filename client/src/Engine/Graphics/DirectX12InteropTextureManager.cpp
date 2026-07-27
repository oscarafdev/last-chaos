#include "stdh.h"

#include <vector>
#include <d3d9on12.h>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12InteropTextureManager.h>
#include <Engine/Graphics/DirectX12SampledTextureCache.h>
#include <Engine/Graphics/DirectX12Texture.h>
#include <Engine/Graphics/DirectX12TextureFormat.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	struct InteropTextureEntry
	{
		IDirect3DTexture9* pTexture9;
		ID3D12Resource* pResource12;
		DirectX12DescriptorHandle descriptor;
		bool transitioned;
		bool returned;
	};

	struct NativeRenderTextureEntry
	{
		DirectX12RenderTextureHandle handle;
		IDirect3DTexture9* pTexture9;
		CDirectX12Texture* pNativeTexture;
	};

	void Transition(
		ID3D12GraphicsCommandList* pCommandList,
		ID3D12Resource* pResource,
		D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after)
	{
		D3D12_RESOURCE_BARRIER barrier;
		ZeroMemory(&barrier, sizeof(barrier));
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = pResource;
		barrier.Transition.Subresource =
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = before;
		barrier.Transition.StateAfter = after;
		pCommandList->ResourceBarrier(1, &barrier);
	}

}

struct DirectX12InteropTextureState
{
	DirectX12InteropTextureState()
		: nextRenderTextureHandle(1)
	{
	}

	std::vector<InteropTextureEntry> frames[
		CDirectX12InteropTextureManager::FRAME_COUNT];
	std::vector<NativeRenderTextureEntry> renderTextures;
	DirectX12RenderTextureHandle nextRenderTextureHandle;
};

CDirectX12InteropTextureManager::CDirectX12InteropTextureManager()
	: m_pDevice(NULL)
	, m_pGraphicsQueue(NULL)
	, m_pDevice9On12(NULL)
	, m_pResourceDescriptors(NULL)
	, m_pRenderTargetDescriptors(NULL)
	, m_pSampledTextureCache(NULL)
	, m_pState(NULL)
	, m_currentFrame(0)
	, m_frameActive(false)
	, m_resourcesReturned(true)
{
}

CDirectX12InteropTextureManager::~CDirectX12InteropTextureManager()
{
	Shutdown();
}

bool CDirectX12InteropTextureManager::Initialize(
	ID3D12Device* pDevice,
	ID3D12CommandQueue* pGraphicsQueue,
	CDirectX12DescriptorHeap* pResourceDescriptors,
	CDirectX12DescriptorHeap* pRenderTargetDescriptors)
{
	if (pDevice == NULL || pGraphicsQueue == NULL
		|| pResourceDescriptors == NULL
		|| pResourceDescriptors->GetType()
			!= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		|| !pResourceDescriptors->IsShaderVisible()
		|| pRenderTargetDescriptors == NULL
		|| pRenderTargetDescriptors->GetType()
			!= D3D12_DESCRIPTOR_HEAP_TYPE_RTV
		|| pRenderTargetDescriptors->IsShaderVisible())
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pGraphicsQueue = pGraphicsQueue;
	m_pGraphicsQueue->AddRef();
	m_pResourceDescriptors = pResourceDescriptors;
	m_pRenderTargetDescriptors = pRenderTargetDescriptors;
	m_pSampledTextureCache = new CDirectX12SampledTextureCache;
	m_pState = new DirectX12InteropTextureState;
	if (m_pSampledTextureCache == NULL || m_pState == NULL
		|| !m_pSampledTextureCache->Initialize(
			m_pDevice,
			m_pResourceDescriptors))
	{
		Shutdown();
		return false;
	}
	return true;
}

void CDirectX12InteropTextureManager::Shutdown()
{
	for (UINT iFrame = 0; iFrame < FRAME_COUNT; ++iFrame)
		ReleaseFrame(iFrame);
	ReleaseRenderTargets();
	delete m_pState;
	m_pState = NULL;
	delete m_pSampledTextureCache;
	m_pSampledTextureCache = NULL;

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
	m_pResourceDescriptors = NULL;
	m_pRenderTargetDescriptors = NULL;
	m_currentFrame = 0;
	m_frameActive = false;
	m_resourcesReturned = true;
}

bool CDirectX12InteropTextureManager::AttachD3D9Device(
	IDirect3DDevice9* pDevice9)
{
	if (pDevice9 == NULL || m_pState == NULL || m_frameActive)
		return false;

	if (m_pSampledTextureCache != NULL)
		m_pSampledTextureCache->Clear();
	ReleaseRenderTargets();
	if (m_pDevice9On12 != NULL)
	{
		m_pDevice9On12->Release();
		m_pDevice9On12 = NULL;
	}
	const HRESULT hr = pDevice9->QueryInterface(
		__uuidof(IDirect3DDevice9On12),
		reinterpret_cast<void**>(&m_pDevice9On12));
	return SUCCEEDED(hr);
}

bool CDirectX12InteropTextureManager::BeginFrame(UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= FRAME_COUNT
		|| !m_resourcesReturned)
		return false;

	ReleaseFrame(frameIndex);
	if (m_pSampledTextureCache != NULL)
		m_pSampledTextureCache->BeginFrame(frameIndex);
	m_currentFrame = frameIndex;
	m_frameActive = true;
	m_resourcesReturned = false;
	return true;
}

void CDirectX12InteropTextureManager::ForgetTexture(
	IDirect3DTexture9* pTexture9)
{
	if (m_pState == NULL || pTexture9 == NULL)
		return;
	if (m_pSampledTextureCache != NULL)
		m_pSampledTextureCache->Forget(pTexture9);
	for (size_t iEntry = 0;
		iEntry < m_pState->renderTextures.size();)
	{
		NativeRenderTextureEntry& entry =
			m_pState->renderTextures[iEntry];
		if (entry.pTexture9 != pTexture9)
		{
			++iEntry;
			continue;
		}
		delete entry.pNativeTexture;
		entry.pNativeTexture = NULL;
		entry.pTexture9->Release();
		entry.pTexture9 = NULL;
		m_pState->renderTextures.erase(
			m_pState->renderTextures.begin() + iEntry);
	}
}

void CDirectX12InteropTextureManager::RetireLegacyTextureBinding(
	IDirect3DTexture9* pTexture9)
{
	if (m_pSampledTextureCache != NULL)
		m_pSampledTextureCache->RetireLegacyBinding(pTexture9);
}

bool CDirectX12InteropTextureManager::CreateRenderTarget(
	IDirect3DTexture9* pTexture9,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	DirectX12RenderTextureHandle* pHandle)
{
	if (m_pState == NULL || pTexture9 == NULL || m_pDevice == NULL
		|| m_pResourceDescriptors == NULL
		|| m_pRenderTargetDescriptors == NULL || pHandle == NULL)
		return false;
	*pHandle = DX12_INVALID_RENDER_TEXTURE;
	for (size_t iEntry = 0;
		iEntry < m_pState->renderTextures.size();
		++iEntry)
	{
		const NativeRenderTextureEntry& existing =
			m_pState->renderTextures[iEntry];
		if (existing.pTexture9 == pTexture9)
		{
			*pHandle = existing.handle;
			return true;
		}
	}

	DirectX12TextureFormatInfo formatInfo;
	if (!GetDirectX12TextureFormat(legacyFormat, &formatInfo)
		|| formatInfo.conversion != DX12_TEXTURE_CONVERSION_NONE)
		return false;

	CDirectX12Texture* pNativeTexture = new CDirectX12Texture;
	if (pNativeTexture == NULL
		|| !pNativeTexture->CreateRenderTarget2D(
			m_pDevice,
			m_pResourceDescriptors,
			m_pRenderTargetDescriptors,
			width,
			height,
			formatInfo.format,
			formatInfo.componentMapping))
	{
		delete pNativeTexture;
		return false;
	}

	NativeRenderTextureEntry entry;
	entry.handle = m_pState->nextRenderTextureHandle++;
	if (entry.handle == DX12_INVALID_RENDER_TEXTURE)
		entry.handle = m_pState->nextRenderTextureHandle++;
	pTexture9->AddRef();
	entry.pTexture9 = pTexture9;
	entry.pNativeTexture = pNativeTexture;
	m_pState->renderTextures.push_back(entry);
	*pHandle = entry.handle;
	return true;
}

void CDirectX12InteropTextureManager::DestroyRenderTarget(
	DirectX12RenderTextureHandle handle)
{
	if (m_pState == NULL || handle == DX12_INVALID_RENDER_TEXTURE)
		return;
	for (size_t iEntry = 0;
		iEntry < m_pState->renderTextures.size();
		++iEntry)
	{
		NativeRenderTextureEntry& entry =
			m_pState->renderTextures[iEntry];
		if (entry.handle != handle)
			continue;
		delete entry.pNativeTexture;
		entry.pNativeTexture = NULL;
		if (entry.pTexture9 != NULL)
		{
			entry.pTexture9->Release();
			entry.pTexture9 = NULL;
		}
		m_pState->renderTextures.erase(
			m_pState->renderTextures.begin() + iEntry);
		return;
	}
}

CDirectX12Texture*
CDirectX12InteropTextureManager::FindRenderTarget(
	DirectX12RenderTextureHandle handle) const
{
	if (m_pState == NULL || handle == DX12_INVALID_RENDER_TEXTURE)
		return NULL;
	for (size_t iEntry = 0;
		iEntry < m_pState->renderTextures.size();
		++iEntry)
	{
		const NativeRenderTextureEntry& entry =
			m_pState->renderTextures[iEntry];
		if (entry.handle == handle)
			return entry.pNativeTexture;
	}
	return NULL;
}

CDirectX12Texture*
CDirectX12InteropTextureManager::FindRenderTarget(
	IDirect3DTexture9* pTexture9) const
{
	if (m_pState == NULL || pTexture9 == NULL)
		return NULL;
	for (size_t iEntry = 0;
		iEntry < m_pState->renderTextures.size();
		++iEntry)
	{
		const NativeRenderTextureEntry& entry =
			m_pState->renderTextures[iEntry];
		if (entry.pTexture9 == pTexture9)
			return entry.pNativeTexture;
	}
	return NULL;
}

bool CDirectX12InteropTextureManager::ReferencesResource(
	IDirect3DTexture9* pTexture9,
	ID3D12Resource* pResource12) const
{
	CDirectX12Texture* pRenderTexture = FindRenderTarget(pTexture9);
	return pRenderTexture != NULL
		&& pRenderTexture->GetResource() == pResource12;
}

bool CDirectX12InteropTextureManager::Acquire(
	IDirect3DTexture9* pTexture9,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (!m_frameActive || pTexture9 == NULL || pCommandList == NULL
		|| pUploadManager == NULL || pShaderResourceView == NULL
		|| m_pDevice9On12 == NULL)
		return false;

	CDirectX12Texture* pRenderTexture = FindRenderTarget(pTexture9);
	if (pRenderTexture != NULL)
	{
		pRenderTexture->Transition(
			pCommandList,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		*pShaderResourceView =
			pRenderTexture->GetShaderResourceView();
		return true;
	}

	D3DSURFACE_DESC legacyDesc;
	HRESULT hr = pTexture9->GetLevelDesc(0, &legacyDesc);
	if (FAILED(hr))
		return false;

	if (legacyDesc.Pool != D3DPOOL_DEFAULT)
		return m_pSampledTextureCache != NULL
			&& m_pSampledTextureCache->Acquire(
				pTexture9,
				pCommandList,
				pUploadManager,
				pShaderResourceView);

	std::vector<InteropTextureEntry>& entries =
		m_pState->frames[m_currentFrame];
	for (size_t iEntry = 0; iEntry < entries.size(); ++iEntry)
	{
		if (entries[iEntry].pTexture9 == pTexture9
			&& !entries[iEntry].returned)
		{
			*pShaderResourceView = entries[iEntry].descriptor.gpu;
			return true;
		}
	}

	InteropTextureEntry entry;
	ZeroMemory(&entry, sizeof(entry));
	hr = m_pDevice9On12->UnwrapUnderlyingResource(
		reinterpret_cast<IDirect3DResource9*>(pTexture9),
		m_pGraphicsQueue,
		__uuidof(ID3D12Resource),
		reinterpret_cast<void**>(&entry.pResource12));
	if (FAILED(hr))
	{
		CPrintF(
			"DX12 error: no se pudo desenvolver textura D3D9 (0x%08X).\n",
			static_cast<unsigned int>(hr));
		return false;
	}

	const D3D12_RESOURCE_DESC resourceDesc = entry.pResource12->GetDesc();
	if (resourceDesc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D
		|| resourceDesc.SampleDesc.Count != 1
		|| !m_pResourceDescriptors->Allocate(&entry.descriptor))
	{
		CPrintF(
			"DX12 error: textura incompatible o sin descriptor "
			"(dimension %d, muestras %u).\n",
			static_cast<int>(resourceDesc.Dimension),
			resourceDesc.SampleDesc.Count);
		m_pDevice9On12->ReturnUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(pTexture9),
			0,
			NULL,
			NULL);
		entry.pResource12->Release();
		return false;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = resourceDesc.Format;
	viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	DirectX12TextureFormatInfo formatInfo;
	viewDesc.Shader4ComponentMapping =
		GetDirectX12TextureFormat(legacyDesc.Format, &formatInfo)
			? formatInfo.componentMapping
			: D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = resourceDesc.MipLevels;
	viewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	m_pDevice->CreateShaderResourceView(
		entry.pResource12,
		&viewDesc,
		entry.descriptor.cpu);

	pTexture9->AddRef();
	entry.pTexture9 = pTexture9;
	entry.transitioned = true;
	Transition(
		pCommandList,
		entry.pResource12,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	entries.push_back(entry);
	*pShaderResourceView = entry.descriptor.gpu;
	return true;
}

bool CDirectX12InteropTextureManager::RefreshSampledTexture(
	IDirect3DTexture9* pTexture9,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager)
{
	if (!m_frameActive || m_pSampledTextureCache == NULL)
		return false;
	return m_pSampledTextureCache->Refresh(
		pTexture9,
		pCommandList,
		pUploadManager);
}

bool CDirectX12InteropTextureManager::PrepareForSubmission(
	ID3D12GraphicsCommandList* pCommandList)
{
	if (!m_frameActive || pCommandList == NULL)
		return false;

	std::vector<InteropTextureEntry>& entries =
		m_pState->frames[m_currentFrame];
	for (size_t iEntry = 0; iEntry < entries.size(); ++iEntry)
	{
		if (!entries[iEntry].transitioned)
			continue;
		Transition(
			pCommandList,
			entries[iEntry].pResource12,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_COMMON);
		entries[iEntry].transitioned = false;
	}
	return true;
}

bool CDirectX12InteropTextureManager::ReturnToD3D9(
	ID3D12Fence* pFence,
	UINT64 fenceValue,
	bool endFrame)
{
	if (!m_frameActive || pFence == NULL || fenceValue == 0
		|| m_pDevice9On12 == NULL)
		return false;

	bool succeeded = true;
	std::vector<InteropTextureEntry>& entries =
		m_pState->frames[m_currentFrame];
	ID3D12Fence* fences[] = { pFence };
	UINT64 fenceValues[] = { fenceValue };
	for (size_t iEntry = 0; iEntry < entries.size(); ++iEntry)
	{
		if (entries[iEntry].returned)
			continue;
		const HRESULT hr = m_pDevice9On12->ReturnUnderlyingResource(
			reinterpret_cast<IDirect3DResource9*>(
				entries[iEntry].pTexture9),
			1,
			fenceValues,
			fences);
		succeeded = SUCCEEDED(hr) && succeeded;
		entries[iEntry].returned = SUCCEEDED(hr);
	}
	if (endFrame)
	{
		m_frameActive = false;
		m_resourcesReturned = true;
	}
	return succeeded;
}

void CDirectX12InteropTextureManager::ReleaseFrame(UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= FRAME_COUNT)
		return;

	std::vector<InteropTextureEntry>& entries =
		m_pState->frames[frameIndex];
	for (size_t iEntry = 0; iEntry < entries.size(); ++iEntry)
	{
		if (entries[iEntry].descriptor.IsValid()
			&& m_pResourceDescriptors != NULL)
			m_pResourceDescriptors->Release(
				entries[iEntry].descriptor.index);
		if (entries[iEntry].pResource12 != NULL)
			entries[iEntry].pResource12->Release();
		if (entries[iEntry].pTexture9 != NULL)
			entries[iEntry].pTexture9->Release();
	}
	entries.clear();
}

void CDirectX12InteropTextureManager::ReleaseRenderTargets()
{
	if (m_pState == NULL)
		return;
	for (size_t iEntry = 0;
		iEntry < m_pState->renderTextures.size();
		++iEntry)
	{
		delete m_pState->renderTextures[iEntry].pNativeTexture;
		if (m_pState->renderTextures[iEntry].pTexture9 != NULL)
			m_pState->renderTextures[iEntry].pTexture9->Release();
	}
	m_pState->renderTextures.clear();
}

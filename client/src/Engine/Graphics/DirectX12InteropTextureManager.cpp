#include "stdh.h"

#include <vector>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12InteropTextureManager.h>
#include <Engine/Graphics/DirectX12SampledTextureCache.h>
#include <Engine/Graphics/DirectX12Texture.h>
#include <Engine/Graphics/DirectX12TextureFormat.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	struct NativeRenderTextureEntry
	{
		DirectX12RenderTextureHandle handle;
		CDirectX12Texture* pNativeTexture;
	};

}

struct DirectX12InteropTextureState
{
	std::vector<NativeRenderTextureEntry> renderTextures;
};

CDirectX12InteropTextureManager::CDirectX12InteropTextureManager()
	: m_pDevice(NULL)
	, m_pResourceDescriptors(NULL)
	, m_pRenderTargetDescriptors(NULL)
	, m_pSampledTextureCache(NULL)
	, m_pState(NULL)
	, m_frameActive(false)
{
}

CDirectX12InteropTextureManager::~CDirectX12InteropTextureManager()
{
	Shutdown();
}

bool CDirectX12InteropTextureManager::Initialize(
	ID3D12Device* pDevice,
	CDirectX12DescriptorHeap* pResourceDescriptors,
	CDirectX12DescriptorHeap* pRenderTargetDescriptors)
{
	if (pDevice == NULL || pResourceDescriptors == NULL
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
	ReleaseRenderTargets();
	delete m_pState;
	m_pState = NULL;
	delete m_pSampledTextureCache;
	m_pSampledTextureCache = NULL;

	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	m_pResourceDescriptors = NULL;
	m_pRenderTargetDescriptors = NULL;
	m_frameActive = false;
}

bool CDirectX12InteropTextureManager::ResetResources()
{
	if (m_pState == NULL || m_frameActive)
		return false;

	if (m_pSampledTextureCache != NULL)
		m_pSampledTextureCache->Clear();
	ReleaseRenderTargets();
	return true;
}

bool CDirectX12InteropTextureManager::BeginFrame(UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= FRAME_COUNT
		|| m_frameActive)
		return false;

	if (m_pSampledTextureCache != NULL)
		m_pSampledTextureCache->BeginFrame(frameIndex);
	m_frameActive = true;
	return true;
}

bool CDirectX12InteropTextureManager::EndFrame()
{
	if (!m_frameActive)
		return false;
	m_frameActive = false;
	return true;
}

bool CDirectX12InteropTextureManager::CreateRenderTarget(
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	DirectX12RenderTextureHandle* pHandle)
{
	if (m_pState == NULL || m_pDevice == NULL
		|| m_pResourceDescriptors == NULL
		|| m_pRenderTargetDescriptors == NULL || pHandle == NULL)
		return false;
	*pHandle = DirectX12RenderTextureHandle();
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
	entry.handle = pNativeTexture->GetRenderTextureHandle();
	entry.pNativeTexture = pNativeTexture;
	m_pState->renderTextures.push_back(entry);
	*pHandle = entry.handle;
	return entry.handle.IsValid();
}

bool CDirectX12InteropTextureManager::CreateSampledTexture(
	DirectX12TextureHandle* pHandle)
{
	return m_pSampledTextureCache != NULL
		&& m_pSampledTextureCache->CreateNative(pHandle);
}

void CDirectX12InteropTextureManager::DestroySampledTexture(
	DirectX12TextureHandle handle)
{
	if (m_pSampledTextureCache != NULL)
		m_pSampledTextureCache->DestroyNative(handle);
}

bool CDirectX12InteropTextureManager::
RefreshSampledTextureFromRgbaMipChain(
	DirectX12TextureHandle handle,
	const void* pPixels,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	UINT maximumMipCount,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	DirectX12TextureHandle* pNewHandle)
{
	return m_pSampledTextureCache != NULL
		&& m_pSampledTextureCache->RefreshNativeFromRgbaMipChain(
			handle,
			pPixels,
			width,
			height,
			legacyFormat,
			maximumMipCount,
			pCommandList,
			pUploadManager,
			pNewHandle);
}

bool CDirectX12InteropTextureManager::
RefreshSampledTextureFromCompressedBlob(
	DirectX12TextureHandle handle,
	const void* pBlob,
	size_t blobSize,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	UINT maximumMipCount,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	DirectX12TextureHandle* pNewHandle)
{
	return m_pSampledTextureCache != NULL
		&& m_pSampledTextureCache->RefreshNativeFromCompressedBlob(
			handle,
			pBlob,
			blobSize,
			width,
			height,
			legacyFormat,
			maximumMipCount,
			pCommandList,
			pUploadManager,
			pNewHandle);
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
		m_pState->renderTextures.erase(
			m_pState->renderTextures.begin() + iEntry);
		return;
	}
}

CDirectX12Texture*
CDirectX12InteropTextureManager::FindRenderTarget(
	DirectX12RenderTextureHandle handle) const
{
	if (m_pState == NULL || !handle.IsValid())
		return NULL;
	return static_cast<CDirectX12Texture*>(
		GetDirectX12ResourceRegistry().Resolve(handle));
}

bool CDirectX12InteropTextureManager::ReferencesResource(
	DirectX12RenderTextureHandle handle,
	ID3D12Resource* pResource12) const
{
	CDirectX12Texture* pRenderTexture = FindRenderTarget(handle);
	return pRenderTexture != NULL
		&& pRenderTexture->GetResource() == pResource12;
}

bool CDirectX12InteropTextureManager::Acquire(
	DirectX12TextureHandle handle,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (!m_frameActive || pCommandList == NULL || pUploadManager == NULL
		|| pShaderResourceView == NULL || m_pSampledTextureCache == NULL)
		return false;
	return m_pSampledTextureCache->Acquire(
		handle,
		pCommandList,
		pUploadManager,
		pShaderResourceView);
}

bool CDirectX12InteropTextureManager::Acquire(
	DirectX12RenderTextureHandle handle,
	ID3D12GraphicsCommandList* pCommandList,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (!m_frameActive || pCommandList == NULL
		|| pShaderResourceView == NULL)
		return false;
	CDirectX12Texture* pTexture = FindRenderTarget(handle);
	if (pTexture == NULL)
		return false;
	pTexture->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	*pShaderResourceView = pTexture->GetShaderResourceView();
	return pShaderResourceView->ptr != 0;
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
	}
	m_pState->renderTextures.clear();
}

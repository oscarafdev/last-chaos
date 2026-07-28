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
		IDirect3DTexture9* pTexture9;
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

bool CDirectX12InteropTextureManager::ResetLegacyBindings()
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
		GetDirectX12ResourceRegistry().UnbindLegacyAlias(
			entry.pTexture9,
			entry.handle);
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
	entry.handle = pNativeTexture->GetRenderTextureHandle();
	if (!entry.handle.IsValid()
		|| !GetDirectX12ResourceRegistry().BindLegacyAlias(
			pTexture9,
			entry.handle))
	{
		delete pNativeTexture;
		return false;
	}
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
		GetDirectX12ResourceRegistry().UnbindLegacyAlias(
			entry.pTexture9,
			entry.handle);
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
	if (m_pState == NULL || !handle.IsValid())
		return NULL;
	return static_cast<CDirectX12Texture*>(
		GetDirectX12ResourceRegistry().Resolve(handle));
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

DirectX12TextureHandle
CDirectX12InteropTextureManager::ResolveSampledTextureHandle(
	IDirect3DTexture9* pTexture9) const
{
	return m_pSampledTextureCache != NULL
		? m_pSampledTextureCache->FindHandle(pTexture9)
		: DirectX12TextureHandle();
}

DirectX12RenderTextureHandle
CDirectX12InteropTextureManager::ResolveRenderTextureHandle(
	IDirect3DTexture9* pTexture9) const
{
	return GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_RENDER_TEXTURE>(pTexture9);
}

bool CDirectX12InteropTextureManager::ReferencesResource(
	IDirect3DTexture9* pTexture9,
	ID3D12Resource* pResource12) const
{
	const DirectX12RenderTextureHandle handle =
		ResolveRenderTextureHandle(pTexture9);
	return ReferencesResource(handle, pResource12);
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
	IDirect3DTexture9* pTexture9,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (!m_frameActive || pTexture9 == NULL || pCommandList == NULL
		|| pUploadManager == NULL || pShaderResourceView == NULL)
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

	static bool defaultTextureWithoutHandleReported = false;
	if (!defaultTextureWithoutHandleReported)
	{
		CPrintF(
			"DX12 textura: se rechazo un recurso D3DPOOL_DEFAULT sin "
			"handle nativo.\n");
		defaultTextureWithoutHandleReported = true;
	}
	return false;
}

bool CDirectX12InteropTextureManager::Acquire(
	DirectX12TextureHandle handle,
	ID3D12GraphicsCommandList* pCommandList,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (!m_frameActive || pCommandList == NULL
		|| pShaderResourceView == NULL || m_pSampledTextureCache == NULL)
		return false;
	return m_pSampledTextureCache->Acquire(
		handle,
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

bool CDirectX12InteropTextureManager::
RefreshSampledTextureFromRgbaMipChain(
	IDirect3DTexture9* pTexture9,
	const void* pPixels,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	UINT maximumMipCount,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager)
{
	if (!m_frameActive || m_pSampledTextureCache == NULL)
		return false;
	return m_pSampledTextureCache->RefreshFromRgbaMipChain(
		pTexture9,
		pPixels,
		width,
		height,
		legacyFormat,
		maximumMipCount,
		pCommandList,
		pUploadManager);
}

bool CDirectX12InteropTextureManager::
RefreshSampledTextureFromCompressedBlob(
	IDirect3DTexture9* pTexture9,
	const void* pBlob,
	size_t blobSize,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	UINT maximumMipCount,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager)
{
	if (!m_frameActive || m_pSampledTextureCache == NULL)
		return false;
	return m_pSampledTextureCache->RefreshFromCompressedBlob(
		pTexture9,
		pBlob,
		blobSize,
		width,
		height,
		legacyFormat,
		maximumMipCount,
		pCommandList,
		pUploadManager);
}

void CDirectX12InteropTextureManager::ReleaseRenderTargets()
{
	if (m_pState == NULL)
		return;
	for (size_t iEntry = 0;
		iEntry < m_pState->renderTextures.size();
		++iEntry)
	{
		GetDirectX12ResourceRegistry().UnbindLegacyAlias(
			m_pState->renderTextures[iEntry].pTexture9,
			m_pState->renderTextures[iEntry].handle);
		delete m_pState->renderTextures[iEntry].pNativeTexture;
		if (m_pState->renderTextures[iEntry].pTexture9 != NULL)
			m_pState->renderTextures[iEntry].pTexture9->Release();
	}
	m_pState->renderTextures.clear();
}

#include "stdh.h"

#include <algorithm>
#include <vector>
#include <d3d9.h>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12SampledTextureCache.h>
#include <Engine/Graphics/DirectX12Texture.h>
#include <Engine/Graphics/DirectX12TextureUploadSource.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	class SampledTextureCacheLock
	{
	public:
		explicit SampledTextureCacheLock(CRITICAL_SECTION* pSection)
			: m_pSection(pSection)
		{
			EnterCriticalSection(m_pSection);
		}

		~SampledTextureCacheLock()
		{
			LeaveCriticalSection(m_pSection);
		}

	private:
		SampledTextureCacheLock(const SampledTextureCacheLock&);
		SampledTextureCacheLock& operator=(
			const SampledTextureCacheLock&);
		CRITICAL_SECTION* m_pSection;
	};

	struct NativeSampledTextureEntry
	{
		DirectX12TextureHandle handle;
		CDirectX12Texture* pNativeTexture;
		CDirectX12TextureUploadSource* pPendingSource;
	};
}

struct DirectX12SampledTextureCacheState
{
	std::vector<NativeSampledTextureEntry> textures;
	std::vector<CDirectX12Texture*> retired[DX12_FRAME_COUNT];
	std::vector<DirectX12TextureHandle> pendingNativeDestructions;
	CRITICAL_SECTION criticalSection;
	bool directRgbaUploadReported;
	bool directCompressedUploadReported;

	DirectX12SampledTextureCacheState()
		: directRgbaUploadReported(false)
		, directCompressedUploadReported(false)
	{
		InitializeCriticalSection(&criticalSection);
	}

	~DirectX12SampledTextureCacheState()
	{
		DeleteCriticalSection(&criticalSection);
	}
};

CDirectX12SampledTextureCache::CDirectX12SampledTextureCache()
	: m_pDevice(NULL)
	, m_pResourceDescriptors(NULL)
	, m_pState(NULL)
	, m_currentFrame(0)
	, m_hasFrame(false)
{
}

CDirectX12SampledTextureCache::~CDirectX12SampledTextureCache()
{
	Shutdown();
}

bool CDirectX12SampledTextureCache::Initialize(
	ID3D12Device* pDevice,
	CDirectX12DescriptorHeap* pResourceDescriptors)
{
	if (pDevice == NULL || pResourceDescriptors == NULL
		|| pResourceDescriptors->GetType()
			!= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		|| !pResourceDescriptors->IsShaderVisible())
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pResourceDescriptors = pResourceDescriptors;
	m_pState = new DirectX12SampledTextureCacheState;
	if (m_pState == NULL)
	{
		Shutdown();
		return false;
	}
	return true;
}

void CDirectX12SampledTextureCache::Shutdown()
{
	Clear();
	if (m_pState != NULL)
	{
		for (UINT iFrame = 0; iFrame < DX12_FRAME_COUNT; ++iFrame)
			ReleaseRetired(iFrame);
	}
	delete m_pState;
	m_pState = NULL;
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	m_pResourceDescriptors = NULL;
	m_currentFrame = 0;
	m_hasFrame = false;
}

void CDirectX12SampledTextureCache::Clear()
{
	if (m_pState == NULL)
		return;
	std::vector<NativeSampledTextureEntry> textures;
	{
		SampledTextureCacheLock lock(&m_pState->criticalSection);
		textures.swap(m_pState->textures);
		m_pState->pendingNativeDestructions.clear();
	}
	for (size_t iTexture = 0; iTexture < textures.size();
		++iTexture)
	{
		Retire(textures[iTexture].pNativeTexture);
		delete textures[iTexture].pPendingSource;
	}
}

bool CDirectX12SampledTextureCache::CreateNative(
	DirectX12TextureHandle* pHandle)
{
	if (m_pState == NULL || m_pDevice == NULL
		|| m_pResourceDescriptors == NULL || pHandle == NULL)
		return false;
	*pHandle = DirectX12TextureHandle();
	CDirectX12Texture* pNativeTexture = new CDirectX12Texture;
	if (pNativeTexture == NULL
		|| !pNativeTexture->Create2D(
			m_pDevice,
			m_pResourceDescriptors,
			1,
			1,
			1,
			DXGI_FORMAT_B8G8R8A8_UNORM))
	{
		delete pNativeTexture;
		return false;
	}
	NativeSampledTextureEntry entry;
	entry.handle = pNativeTexture->GetTextureHandle();
	entry.pNativeTexture = pNativeTexture;
	entry.pPendingSource = NULL;
	{
		SampledTextureCacheLock lock(&m_pState->criticalSection);
		m_pState->textures.push_back(entry);
	}
	*pHandle = entry.handle;
	return true;
}

void CDirectX12SampledTextureCache::DestroyNative(
	DirectX12TextureHandle handle)
{
	if (m_pState == NULL || !handle.IsValid())
		return;
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	if (std::find(
		m_pState->pendingNativeDestructions.begin(),
		m_pState->pendingNativeDestructions.end(),
		handle) == m_pState->pendingNativeDestructions.end())
	{
		m_pState->pendingNativeDestructions.push_back(handle);
	}
}

bool CDirectX12SampledTextureCache::RefreshNativeFromRgbaMipChain(
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
	CDirectX12TextureUploadSource source;
	return source.PrepareFromRgbaMipChain(
			pPixels,
			width,
			height,
			legacyFormat,
			maximumMipCount)
		&& ReplaceNative(
			handle,
			source,
			pCommandList,
			pUploadManager,
			CPU_UPLOAD_RGBA,
			pNewHandle);
}

bool CDirectX12SampledTextureCache::RefreshNativeFromCompressedBlob(
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
	CDirectX12TextureUploadSource source;
	if (!source.PrepareFromCompressedBlob(
			pBlob,
			blobSize,
			width,
			height,
			legacyFormat,
			maximumMipCount))
	{
		CPrintF(
			"DX12 textura DXT rechazada: %ux%u, formato=%d, "
			"bytes=%u, mips=%u.\n",
			width,
			height,
			static_cast<int>(legacyFormat),
			static_cast<unsigned int>(blobSize),
			maximumMipCount);
		return false;
	}
	if (!ReplaceNative(
			handle,
			source,
			pCommandList,
			pUploadManager,
			CPU_UPLOAD_COMPRESSED,
			pNewHandle))
	{
		CPrintF(
			"DX12 textura DXT sin destino: handle=%I64u.\n",
			handle.GetValue());
		return false;
	}
	return true;
}

void CDirectX12SampledTextureCache::BeginFrame(UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= DX12_FRAME_COUNT)
		return;
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	for (size_t iHandle = 0;
		iHandle < m_pState->pendingNativeDestructions.size();
		++iHandle)
	{
		Remove(m_pState->pendingNativeDestructions[iHandle]);
	}
	m_pState->pendingNativeDestructions.clear();
	// Una recreación D3D9 puede suceder después de capturar un draw. La
	// identidad anterior permanece disponible hasta terminar ese frame y se
	// desacopla aquí; el recurso DX12 se libera al reciclar su fence.
	// El backend espera la fence de este slot antes de BeginFrame.
	ReleaseRetired(frameIndex);
	m_currentFrame = frameIndex;
	m_hasFrame = true;
}

bool CDirectX12SampledTextureCache::Acquire(
	DirectX12TextureHandle handle,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (!handle.IsValid() || pShaderResourceView == NULL)
		return false;
	if (m_pState != NULL)
	{
		SampledTextureCacheLock lock(&m_pState->criticalSection);
		for (size_t iTexture = 0;
			iTexture < m_pState->textures.size();
			++iTexture)
		{
			NativeSampledTextureEntry& entry =
				m_pState->textures[iTexture];
			if (entry.handle != handle
				|| entry.pPendingSource == NULL)
				continue;
			const CDirectX12TextureUploadSource& source =
				*entry.pPendingSource;
			if (pCommandList == NULL || pUploadManager == NULL)
			{
				CPrintF(
					"DX12 textura pendiente sin contexto: "
					"handle=%I64u.\n",
					handle.GetValue());
				return false;
			}
			if (!entry.pNativeTexture->Recreate2D(
					m_pDevice,
					m_pResourceDescriptors,
					source.GetWidth(),
					source.GetHeight(),
					source.GetMipCount(),
					source.GetFormat(),
					source.GetComponentMapping()))
			{
				CPrintF(
					"DX12 textura pendiente no pudo recrearse: "
					"handle=%I64u, %ux%u, formato=%d, mips=%u, "
					"descriptores=%u/%u, device=%08X.\n",
					handle.GetValue(),
					source.GetWidth(),
					source.GetHeight(),
					static_cast<int>(source.GetFormat()),
					source.GetMipCount(),
					m_pResourceDescriptors->GetAllocatedCount(),
					m_pResourceDescriptors->GetCapacity(),
					static_cast<unsigned int>(
						m_pDevice->GetDeviceRemovedReason()));
				return false;
			}
			if (!entry.pNativeTexture->Upload(
					pUploadManager,
					pCommandList,
					0,
					source.GetMipCount(),
					source.GetSubresources()))
			{
				CPrintF(
					"DX12 textura pendiente no pudo cargarse: "
					"handle=%I64u, %ux%u, formato=%d, mips=%u, "
					"device=%08X.\n",
					handle.GetValue(),
					source.GetWidth(),
					source.GetHeight(),
					static_cast<int>(source.GetFormat()),
					source.GetMipCount(),
					static_cast<unsigned int>(
						m_pDevice->GetDeviceRemovedReason()));
				return false;
			}
			delete entry.pPendingSource;
			entry.pPendingSource = NULL;
			break;
		}
	}
	CDirectX12Texture* pTexture =
		static_cast<CDirectX12Texture*>(
			GetDirectX12ResourceRegistry().Resolve(handle));
	if (pTexture == NULL || pTexture->GetResource() == NULL)
	{
		CPrintF(
			"DX12 textura handle no resuelta: handle=%I64u, "
			"owner=%p, cache=%u.\n",
			handle.GetValue(),
			pTexture,
			m_pState != NULL
				? static_cast<unsigned int>(
					m_pState->textures.size())
				: 0U);
		return false;
	}
	*pShaderResourceView = pTexture->GetShaderResourceView();
	if (pShaderResourceView->ptr == 0)
	{
		CPrintF(
			"DX12 textura sin descriptor: handle=%I64u.\n",
			handle.GetValue());
		return false;
	}
	return true;
}

bool CDirectX12SampledTextureCache::Remove(
	DirectX12TextureHandle handle)
{
	if (m_pState == NULL || !handle.IsValid())
		return false;
	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		const NativeSampledTextureEntry removed =
			m_pState->textures[iTexture];
		if (removed.handle != handle)
			continue;
		m_pState->textures.erase(
			m_pState->textures.begin() + iTexture);
		Retire(removed.pNativeTexture);
		delete removed.pPendingSource;
		return true;
	}
	return false;
}

bool CDirectX12SampledTextureCache::ReplaceNative(
	DirectX12TextureHandle handle,
	const CDirectX12TextureUploadSource& source,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	CpuUploadKind cpuUploadKind,
	DirectX12TextureHandle* pNewHandle)
{
	if (m_pState == NULL || m_pDevice == NULL
		|| m_pResourceDescriptors == NULL || !handle.IsValid()
		|| pNewHandle == NULL || source.GetWidth() == 0
		|| source.GetHeight() == 0 || source.GetMipCount() == 0
		|| source.GetFormat() == DXGI_FORMAT_UNKNOWN
		|| source.GetSubresources() == NULL)
		return false;
	*pNewHandle = DirectX12TextureHandle();
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	NativeSampledTextureEntry* pEntry = NULL;
	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		if (m_pState->textures[iTexture].handle == handle)
		{
			pEntry = &m_pState->textures[iTexture];
			break;
		}
	}
	if (pEntry == NULL)
		return false;
	if (pCommandList == NULL || pUploadManager == NULL)
	{
		CDirectX12TextureUploadSource* pPending =
			new CDirectX12TextureUploadSource(source);
		if (pPending == NULL)
			return false;
		delete pEntry->pPendingSource;
		pEntry->pPendingSource = pPending;
		*pNewHandle = handle;
		return true;
	}
	if (!pEntry->pNativeTexture->Recreate2D(
			m_pDevice,
			m_pResourceDescriptors,
			source.GetWidth(),
			source.GetHeight(),
			source.GetMipCount(),
			source.GetFormat(),
			source.GetComponentMapping())
		|| !pEntry->pNativeTexture->Upload(
			pUploadManager,
			pCommandList,
			0,
			source.GetMipCount(),
			source.GetSubresources()))
		return false;
	delete pEntry->pPendingSource;
	pEntry->pPendingSource = NULL;
	*pNewHandle = handle;
	if (cpuUploadKind == CPU_UPLOAD_RGBA
		&& !m_pState->directRgbaUploadReported)
	{
		CPrintF(
			"DX12 texturas: recursos RGBA nacen y se cargan sin "
			"identidad D3D9.\n");
		m_pState->directRgbaUploadReported = true;
	}
	if (cpuUploadKind == CPU_UPLOAD_COMPRESSED
		&& !m_pState->directCompressedUploadReported)
	{
		CPrintF(
			"DX12 texturas: recursos DXT nacen y se cargan sin "
			"identidad D3D9.\n");
		m_pState->directCompressedUploadReported = true;
	}
	return true;
}

void CDirectX12SampledTextureCache::Retire(
	CDirectX12Texture* pTexture)
{
	if (pTexture == NULL)
		return;
	if (m_pState != NULL && m_hasFrame)
		m_pState->retired[m_currentFrame].push_back(pTexture);
	else
		delete pTexture;
}

void CDirectX12SampledTextureCache::ReleaseRetired(
	UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= DX12_FRAME_COUNT)
		return;
	std::vector<CDirectX12Texture*>& retired =
		m_pState->retired[frameIndex];
	for (size_t iTexture = 0; iTexture < retired.size(); ++iTexture)
		delete retired[iTexture];
	retired.clear();
}

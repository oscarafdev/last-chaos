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
		IDirect3DTexture9* pTexture9;
		CDirectX12Texture* pNativeTexture;
	};
}

struct DirectX12SampledTextureCacheState
{
	std::vector<NativeSampledTextureEntry> textures;
	std::vector<CDirectX12Texture*> retired[DX12_FRAME_COUNT];
	std::vector<IDirect3DTexture9*> retiredLegacyBindings;
	CRITICAL_SECTION criticalSection;
	bool activationReported;
	bool directRgbaUploadReported;
	bool directCompressedUploadReported;

	DirectX12SampledTextureCacheState()
		: activationReported(false)
		, directRgbaUploadReported(false)
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
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		GetDirectX12ResourceRegistry().UnbindLegacyAlias(
			m_pState->textures[iTexture].pTexture9,
			m_pState->textures[iTexture].handle);
		Retire(m_pState->textures[iTexture].pNativeTexture);
		if (m_pState->textures[iTexture].pTexture9 != NULL)
			m_pState->textures[iTexture].pTexture9->Release();
	}
	m_pState->textures.clear();
	m_pState->retiredLegacyBindings.clear();
}

DirectX12TextureHandle CDirectX12SampledTextureCache::FindHandle(
	IDirect3DTexture9* pTexture9) const
{
	return GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(pTexture9);
}

void CDirectX12SampledTextureCache::BeginFrame(UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= DX12_FRAME_COUNT)
		return;
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	// Una recreación D3D9 puede suceder después de capturar un draw. La
	// identidad anterior permanece disponible hasta terminar ese frame y se
	// desacopla aquí; el recurso DX12 se libera al reciclar su fence.
	for (size_t iBinding = 0;
		iBinding < m_pState->retiredLegacyBindings.size();
		++iBinding)
	{
		Remove(m_pState->retiredLegacyBindings[iBinding]);
	}
	m_pState->retiredLegacyBindings.clear();
	// El backend espera la fence de este slot antes de BeginFrame.
	ReleaseRetired(frameIndex);
	m_currentFrame = frameIndex;
	m_hasFrame = true;
}

void CDirectX12SampledTextureCache::Forget(
	IDirect3DTexture9* pTexture9)
{
	if (m_pState == NULL)
		return;
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	Remove(pTexture9);
	if (m_pState != NULL)
	{
		m_pState->retiredLegacyBindings.erase(
			std::remove(
				m_pState->retiredLegacyBindings.begin(),
				m_pState->retiredLegacyBindings.end(),
				pTexture9),
			m_pState->retiredLegacyBindings.end());
	}
}

void CDirectX12SampledTextureCache::RetireLegacyBinding(
	IDirect3DTexture9* pTexture9)
{
	if (m_pState == NULL || pTexture9 == NULL)
		return;
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	if (std::find(
		m_pState->retiredLegacyBindings.begin(),
		m_pState->retiredLegacyBindings.end(),
		pTexture9) == m_pState->retiredLegacyBindings.end())
	{
		m_pState->retiredLegacyBindings.push_back(pTexture9);
	}
}

bool CDirectX12SampledTextureCache::Refresh(
	IDirect3DTexture9* pTexture9,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager)
{
	CDirectX12TextureUploadSource source;
	if (!source.PrepareFromLegacyTexture(pTexture9))
		return false;
	D3D12_GPU_DESCRIPTOR_HANDLE ignored;
	ignored.ptr = 0;
	return Replace(
		pTexture9,
		source,
		pCommandList,
		pUploadManager,
		CPU_UPLOAD_NONE,
		&ignored);
}

bool CDirectX12SampledTextureCache::RefreshFromRgbaMipChain(
	IDirect3DTexture9* pTexture9,
	const void* pPixels,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	UINT maximumMipCount,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager)
{
	CDirectX12TextureUploadSource source;
	if (!source.PrepareFromRgbaMipChain(
		pPixels,
		width,
		height,
		legacyFormat,
		maximumMipCount))
		return false;
	D3D12_GPU_DESCRIPTOR_HANDLE ignored;
	ignored.ptr = 0;
	return Replace(
		pTexture9,
		source,
		pCommandList,
		pUploadManager,
		CPU_UPLOAD_RGBA,
		&ignored);
}

bool CDirectX12SampledTextureCache::RefreshFromCompressedBlob(
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
	CDirectX12TextureUploadSource source;
	if (!source.PrepareFromCompressedBlob(
		pBlob,
		blobSize,
		width,
		height,
		legacyFormat,
		maximumMipCount))
		return false;
	D3D12_GPU_DESCRIPTOR_HANDLE ignored;
	ignored.ptr = 0;
	return Replace(
		pTexture9,
		source,
		pCommandList,
		pUploadManager,
		CPU_UPLOAD_COMPRESSED,
		&ignored);
}

bool CDirectX12SampledTextureCache::Acquire(
	IDirect3DTexture9* pTexture9,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (m_pState == NULL || m_pDevice == NULL
		|| m_pResourceDescriptors == NULL || pTexture9 == NULL
		|| pCommandList == NULL || pUploadManager == NULL
		|| pShaderResourceView == NULL)
		return false;
	SampledTextureCacheLock lock(&m_pState->criticalSection);

	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		const NativeSampledTextureEntry& cached =
			m_pState->textures[iTexture];
		if (cached.pTexture9 == pTexture9)
		{
			return Acquire(cached.handle, pShaderResourceView);
		}
	}

	CDirectX12TextureUploadSource source;
	if (!source.PrepareFromLegacyTexture(pTexture9))
		return false;
	return Replace(
		pTexture9,
		source,
		pCommandList,
		pUploadManager,
		CPU_UPLOAD_NONE,
		pShaderResourceView);
}

bool CDirectX12SampledTextureCache::Acquire(
	DirectX12TextureHandle handle,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView) const
{
	if (!handle.IsValid() || pShaderResourceView == NULL)
		return false;
	CDirectX12Texture* pTexture =
		static_cast<CDirectX12Texture*>(
			GetDirectX12ResourceRegistry().Resolve(handle));
	if (pTexture == NULL || pTexture->GetResource() == NULL)
		return false;
	*pShaderResourceView = pTexture->GetShaderResourceView();
	return pShaderResourceView->ptr != 0;
}

bool CDirectX12SampledTextureCache::Replace(
	IDirect3DTexture9* pTexture9,
	const CDirectX12TextureUploadSource& source,
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12UploadManager* pUploadManager,
	CpuUploadKind cpuUploadKind,
	D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView)
{
	if (m_pState == NULL || m_pDevice == NULL
		|| m_pResourceDescriptors == NULL || pTexture9 == NULL
		|| pCommandList == NULL || pUploadManager == NULL
		|| pShaderResourceView == NULL
		|| source.GetWidth() == 0 || source.GetHeight() == 0
		|| source.GetMipCount() == 0
		|| source.GetFormat() == DXGI_FORMAT_UNKNOWN
		|| source.GetSubresources() == NULL)
		return false;
	SampledTextureCacheLock lock(&m_pState->criticalSection);

	CDirectX12Texture* pNativeTexture = new CDirectX12Texture;
	if (pNativeTexture == NULL
		|| !pNativeTexture->Create2D(
			m_pDevice,
			m_pResourceDescriptors,
			source.GetWidth(),
			source.GetHeight(),
			source.GetMipCount(),
			source.GetFormat(),
			source.GetComponentMapping())
		|| !pNativeTexture->Upload(
			pUploadManager,
			pCommandList,
			0,
			source.GetMipCount(),
			source.GetSubresources()))
	{
		delete pNativeTexture;
		return false;
	}

	Remove(pTexture9);
	NativeSampledTextureEntry entry;
	pTexture9->AddRef();
	entry.handle = pNativeTexture->GetTextureHandle();
	entry.pTexture9 = pTexture9;
	entry.pNativeTexture = pNativeTexture;
	if (!entry.handle.IsValid()
		|| !GetDirectX12ResourceRegistry().BindLegacyAlias(
			pTexture9,
			entry.handle))
	{
		pTexture9->Release();
		delete pNativeTexture;
		return false;
	}
	m_pState->textures.push_back(entry);
	*pShaderResourceView = pNativeTexture->GetShaderResourceView();

	if (!m_pState->activationReported)
	{
		CPrintF(
			"DX12 texturas muestreadas: cache de recursos nativos activo; "
			"D3D9 se conserva solo como identidad transitoria.\n");
		m_pState->activationReported = true;
	}
	if (cpuUploadKind == CPU_UPLOAD_RGBA
		&& !m_pState->directRgbaUploadReported)
	{
		CPrintF(
			"DX12 texturas muestreadas: uploads CPU RGBA directos activos; "
			"LockRect queda solo como compatibilidad diferida.\n");
		m_pState->directRgbaUploadReported = true;
	}
	if (cpuUploadKind == CPU_UPLOAD_COMPRESSED
		&& !m_pState->directCompressedUploadReported)
	{
		CPrintF(
			"DX12 texturas muestreadas: uploads CPU DXT directos activos; "
			"LockRect queda solo como compatibilidad diferida.\n");
		m_pState->directCompressedUploadReported = true;
	}
	return true;
}

bool CDirectX12SampledTextureCache::Remove(
	IDirect3DTexture9* pTexture9)
{
	if (m_pState == NULL || pTexture9 == NULL)
		return false;
	SampledTextureCacheLock lock(&m_pState->criticalSection);
	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		NativeSampledTextureEntry& entry =
			m_pState->textures[iTexture];
		if (entry.pTexture9 != pTexture9)
			continue;
		GetDirectX12ResourceRegistry().UnbindLegacyAlias(
			entry.pTexture9,
			entry.handle);
		Retire(entry.pNativeTexture);
		entry.pNativeTexture = NULL;
		entry.pTexture9->Release();
		entry.pTexture9 = NULL;
		m_pState->textures.erase(
			m_pState->textures.begin() + iTexture);
		return true;
	}
	return false;
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

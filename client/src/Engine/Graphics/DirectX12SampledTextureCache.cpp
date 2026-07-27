#include "stdh.h"

#include <algorithm>
#include <vector>
#include <d3d9.h>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12SampledTextureCache.h>
#include <Engine/Graphics/DirectX12Texture.h>
#include <Engine/Graphics/DirectX12TextureFormat.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	struct NativeSampledTextureEntry
	{
		IDirect3DTexture9* pTexture9;
		CDirectX12Texture* pNativeTexture;
	};

	bool IsBlockCompressed(D3DFORMAT format)
	{
		return format == D3DFMT_DXT1
			|| format == D3DFMT_DXT3
			|| format == D3DFMT_DXT5;
	}

	void ReportA8L8TextureOnce(
		IDirect3DTexture9* pTexture,
		const D3DSURFACE_DESC& description,
		const D3DLOCKED_RECT& lockedRect,
		const DirectX12TextureFormatInfo& formatInfo)
	{
		static UINT reportedCount = 0;
		if (reportedCount >= 16 || description.Format != D3DFMT_A8L8
			|| lockedRect.pBits == NULL || description.Width == 0
			|| description.Height == 0)
			return;
		++reportedCount;

		const unsigned char* pBits =
			static_cast<const unsigned char*>(lockedRect.pBits);
		const UINT last = description.Width - 1;
		const UINT sampleIndices[5] = {
			0,
			last / 4,
			last / 2,
			(last * 3) / 4,
			last
		};
		UINT minimumAlpha = 255;
		UINT maximumAlpha = 0;
		UINT nonZeroAlpha = 0;
		UINT fullAlpha = 0;
		UINT64 hash = 14695981039346656037ULL;
		for (UINT y = 0; y < description.Height; ++y)
		{
			const unsigned char* pRow =
				pBits + static_cast<size_t>(lockedRect.Pitch) * y;
			for (UINT x = 0; x < description.Width; ++x)
			{
				const UINT alpha = pRow[x * 2 + 1];
				minimumAlpha = (std::min)(minimumAlpha, alpha);
				maximumAlpha = (std::max)(maximumAlpha, alpha);
				if (alpha != 0)
					++nonZeroAlpha;
				if (alpha == 255)
					++fullAlpha;
				hash ^= pRow[x * 2 + 0];
				hash *= 1099511628211ULL;
				hash ^= pRow[x * 2 + 1];
				hash *= 1099511628211ULL;
			}
		}
		const unsigned char* pFirstRow = pBits;
		CPrintF(
			"DX12 diagnostico A8L8: tex=%p, %ux%u, pitch=%d, "
			"dxgi=%d, mapping=0x%08X, "
			"alpha=%u..%u/noCero=%u/lleno=%u, hash=%016llX, "
			"muestras L/A=[%u/%u,%u/%u,%u/%u,%u/%u,%u/%u].\n",
			pTexture,
			description.Width,
			description.Height,
			lockedRect.Pitch,
			static_cast<int>(formatInfo.format),
			formatInfo.componentMapping,
			minimumAlpha,
			maximumAlpha,
			nonZeroAlpha,
			fullAlpha,
			static_cast<unsigned long long>(hash),
			pFirstRow[sampleIndices[0] * 2 + 0],
			pFirstRow[sampleIndices[0] * 2 + 1],
			pFirstRow[sampleIndices[1] * 2 + 0],
			pFirstRow[sampleIndices[1] * 2 + 1],
			pFirstRow[sampleIndices[2] * 2 + 0],
			pFirstRow[sampleIndices[2] * 2 + 1],
			pFirstRow[sampleIndices[3] * 2 + 0],
			pFirstRow[sampleIndices[3] * 2 + 1],
			pFirstRow[sampleIndices[4] * 2 + 0],
			pFirstRow[sampleIndices[4] * 2 + 1]);
	}

	void ReportDxt3AlphaOnce(
		IDirect3DTexture9* pTexture,
		const D3DSURFACE_DESC& description,
		const D3DLOCKED_RECT& lockedRect)
	{
		static UINT reportedCount = 0;
		if (reportedCount >= 24 || description.Format != D3DFMT_DXT3
			|| lockedRect.pBits == NULL || description.Width != 256
			|| description.Height != 256)
			return;
		++reportedCount;

		UINT minimumAlpha = 255;
		UINT maximumAlpha = 0;
		UINT nonZeroAlpha = 0;
		UINT fullAlpha = 0;
		UINT regionNonZero = 0;
		UINT regionFull = 0;
		const unsigned char* pBits =
			static_cast<const unsigned char*>(lockedRect.pBits);
		for (UINT y = 0; y < description.Height; ++y)
		{
			const UINT blockY = y / 4;
			const UINT rowInBlock = y & 3;
			const unsigned char* pBlockRow =
				pBits + static_cast<size_t>(lockedRect.Pitch) * blockY;
			for (UINT x = 0; x < description.Width; ++x)
			{
				const UINT blockX = x / 4;
				const UINT pixelInBlock = rowInBlock * 4 + (x & 3);
				const unsigned char* pAlpha =
					pBlockRow + blockX * 16;
				const UINT packedByte = pAlpha[pixelInBlock / 2];
				const UINT alpha4 = (pixelInBlock & 1) != 0
					? packedByte >> 4
					: packedByte & 0x0F;
				const UINT alpha = alpha4 * 17;
				minimumAlpha = (std::min)(minimumAlpha, alpha);
				maximumAlpha = (std::max)(maximumAlpha, alpha);
				if (alpha != 0)
					++nonZeroAlpha;
				if (alpha == 255)
					++fullAlpha;
				if (x >= 32 && x < 64 && y >= 64 && y < 96)
				{
					if (alpha != 0)
						++regionNonZero;
					if (alpha == 255)
						++regionFull;
				}
			}
		}
		CPrintF(
			"DX12 diagnostico DXT3 alpha: tex=%p, alpha=%u..%u, "
			"noCero=%u/65536, lleno=%u/65536, "
			"region[32..64,64..96]=%u/1024, lleno=%u/1024.\n",
			pTexture,
			minimumAlpha,
			maximumAlpha,
			nonZeroAlpha,
			fullAlpha,
			regionNonZero,
			regionFull);
	}
}

struct DirectX12SampledTextureCacheState
{
	std::vector<NativeSampledTextureEntry> textures;
	std::vector<CDirectX12Texture*> retired[DX12_FRAME_COUNT];
	std::vector<IDirect3DTexture9*> retiredLegacyBindings;
	bool activationReported;

	DirectX12SampledTextureCacheState()
		: activationReported(false)
	{
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
	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		Retire(m_pState->textures[iTexture].pNativeTexture);
		if (m_pState->textures[iTexture].pTexture9 != NULL)
			m_pState->textures[iTexture].pTexture9->Release();
	}
	m_pState->textures.clear();
	m_pState->retiredLegacyBindings.clear();
}

void CDirectX12SampledTextureCache::BeginFrame(UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= DX12_FRAME_COUNT)
		return;
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
	if (pTexture9 == NULL || pCommandList == NULL
		|| pUploadManager == NULL)
		return false;
	Remove(pTexture9);
	D3D12_GPU_DESCRIPTOR_HANDLE ignored;
	ignored.ptr = 0;
	return Acquire(
		pTexture9,
		pCommandList,
		pUploadManager,
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

	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		const NativeSampledTextureEntry& cached =
			m_pState->textures[iTexture];
		if (cached.pTexture9 == pTexture9)
		{
			*pShaderResourceView =
				cached.pNativeTexture->GetShaderResourceView();
			return true;
		}
	}

	D3DSURFACE_DESC legacyDesc;
	if (FAILED(pTexture9->GetLevelDesc(0, &legacyDesc))
		|| legacyDesc.Pool == D3DPOOL_DEFAULT)
		return false;

	DirectX12TextureFormatInfo formatInfo;
	if (!GetDirectX12TextureFormat(legacyDesc.Format, &formatInfo))
	{
		CPrintF(
			"DX12 error: formato administrado no compatible (%d).\n",
			static_cast<int>(legacyDesc.Format));
		return false;
	}

	const UINT mipCount = pTexture9->GetLevelCount();
	CDirectX12Texture* pNativeTexture = new CDirectX12Texture;
	if (pNativeTexture == NULL
		|| !pNativeTexture->Create2D(
			m_pDevice,
			m_pResourceDescriptors,
			legacyDesc.Width,
			legacyDesc.Height,
			static_cast<UINT16>(mipCount),
			formatInfo.format,
			formatInfo.componentMapping))
	{
		delete pNativeTexture;
		return false;
	}

	std::vector<D3DLOCKED_RECT> lockedRects(mipCount);
	std::vector<DirectX12SubresourceData> subresources(mipCount);
	std::vector<std::vector<unsigned char> > convertedMips(mipCount);
	UINT lockedMipCount = 0;
	bool lockSucceeded = true;
	for (UINT iMip = 0; iMip < mipCount; ++iMip)
	{
		D3DSURFACE_DESC mipDesc;
		if (FAILED(pTexture9->GetLevelDesc(iMip, &mipDesc))
			|| FAILED(pTexture9->LockRect(
				iMip,
				&lockedRects[iMip],
				NULL,
				D3DLOCK_READONLY)))
		{
			lockSucceeded = false;
			break;
		}
		++lockedMipCount;
		if (iMip == 0)
		{
			ReportA8L8TextureOnce(
				pTexture9,
				mipDesc,
				lockedRects[iMip],
				formatInfo);
			ReportDxt3AlphaOnce(
				pTexture9,
				mipDesc,
				lockedRects[iMip]);
		}
		if (formatInfo.conversion != DX12_TEXTURE_CONVERSION_NONE)
		{
			const LONG convertedRowPitch =
				static_cast<LONG>(mipDesc.Width * 4U);
			convertedMips[iMip].resize(
				static_cast<size_t>(convertedRowPitch)
				* mipDesc.Height);
			if (!ConvertDirectX12TextureSubresource(
				formatInfo,
				lockedRects[iMip].pBits,
				lockedRects[iMip].Pitch,
				mipDesc.Width,
				mipDesc.Height,
				&convertedMips[iMip][0],
				convertedRowPitch))
			{
				lockSucceeded = false;
				break;
			}
			subresources[iMip].pData = &convertedMips[iMip][0];
			subresources[iMip].rowPitch = convertedRowPitch;
			subresources[iMip].slicePitch =
				static_cast<LONG_PTR>(convertedRowPitch)
				* mipDesc.Height;
		}
		else
		{
			const UINT rowCount = IsBlockCompressed(mipDesc.Format)
				? (std::max)(1U, (mipDesc.Height + 3U) / 4U)
				: mipDesc.Height;
			subresources[iMip].pData = lockedRects[iMip].pBits;
			subresources[iMip].rowPitch = lockedRects[iMip].Pitch;
			subresources[iMip].slicePitch =
				static_cast<LONG_PTR>(lockedRects[iMip].Pitch)
				* rowCount;
		}
	}

	const bool uploadSucceeded = lockSucceeded
		&& pNativeTexture->Upload(
			pUploadManager,
			pCommandList,
			0,
			mipCount,
			&subresources[0]);
	for (UINT iMip = 0; iMip < lockedMipCount; ++iMip)
		pTexture9->UnlockRect(iMip);
	if (!uploadSucceeded)
	{
		delete pNativeTexture;
		return false;
	}

	NativeSampledTextureEntry entry;
	pTexture9->AddRef();
	entry.pTexture9 = pTexture9;
	entry.pNativeTexture = pNativeTexture;
	m_pState->textures.push_back(entry);
	*pShaderResourceView = pNativeTexture->GetShaderResourceView();
	if (!m_pState->activationReported)
	{
		CPrintF(
			"DX12 texturas muestreadas: cache de recursos nativos activo; "
			"D3D9 se conserva solo como identidad transitoria.\n");
		m_pState->activationReported = true;
	}
	return true;
}

bool CDirectX12SampledTextureCache::Remove(
	IDirect3DTexture9* pTexture9)
{
	if (m_pState == NULL || pTexture9 == NULL)
		return false;
	for (size_t iTexture = 0;
		iTexture < m_pState->textures.size();
		++iTexture)
	{
		NativeSampledTextureEntry& entry =
			m_pState->textures[iTexture];
		if (entry.pTexture9 != pTexture9)
			continue;
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

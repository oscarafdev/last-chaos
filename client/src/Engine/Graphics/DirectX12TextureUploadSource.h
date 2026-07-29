#ifndef SE_INCL_DIRECTX12TEXTUREUPLOADSOURCE_H
#define SE_INCL_DIRECTX12TEXTUREUPLOADSOURCE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <vector>
#include <windows.h>
#include <d3d9.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12UploadManager.h>

// Convierte mirrors CPU de texturas del motor en subrecursos listos para DX12.
class CDirectX12TextureUploadSource
{
public:
	CDirectX12TextureUploadSource();
	CDirectX12TextureUploadSource(
		const CDirectX12TextureUploadSource& other);
	CDirectX12TextureUploadSource& operator=(
		const CDirectX12TextureUploadSource& other);

	void Clear();
	bool PrepareFromRgbaMipChain(
		const void* pPixels,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		UINT maximumMipCount);
	bool PrepareFromCompressedBlob(
		const void* pBlob,
		size_t blobSize,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		UINT maximumMipCount);

	UINT GetWidth() const;
	UINT GetHeight() const;
	UINT16 GetMipCount() const;
	DXGI_FORMAT GetFormat() const;
	UINT GetComponentMapping() const;
	const DirectX12SubresourceData* GetSubresources() const;

private:
	struct MipPayload
	{
		std::vector<unsigned char> bytes;
		LONG_PTR rowPitch;
		LONG_PTR slicePitch;

		MipPayload()
			: rowPitch(0)
			, slicePitch(0)
		{
		}
	};

	bool PrepareRgbaMip(
		const unsigned char* pPixels,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		MipPayload* pMip);
	void RebuildSubresources();

	UINT m_width;
	UINT m_height;
	DXGI_FORMAT m_format;
	UINT m_componentMapping;
	std::vector<MipPayload> m_mips;
	std::vector<DirectX12SubresourceData> m_subresources;
};

#endif

#ifndef SE_INCL_DIRECTX12TEXTUREFORMAT_H
#define SE_INCL_DIRECTX12TEXTUREFORMAT_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d9.h>
#include <d3d12.h>

enum DirectX12TextureConversion
{
	DX12_TEXTURE_CONVERSION_NONE,
	DX12_TEXTURE_CONVERSION_A4R4G4B4_TO_BGRA8,
	DX12_TEXTURE_CONVERSION_X4R4G4B4_TO_BGRA8
};

struct DirectX12TextureFormatInfo
{
	DirectX12TextureFormatInfo();

	DXGI_FORMAT format;
	UINT componentMapping;
	DirectX12TextureConversion conversion;
};

// Describe el formato DXGI y la conversión requerida para una textura D3D9.
bool GetDirectX12TextureFormat(
	D3DFORMAT legacyFormat,
	DirectX12TextureFormatInfo* pFormatInfo);

// Convierte un mip legado de 16 bits a BGRA8.
bool ConvertDirectX12TextureSubresource(
	const DirectX12TextureFormatInfo& formatInfo,
	const void* pSource,
	LONG sourceRowPitch,
	UINT width,
	UINT height,
	unsigned char* pDestination,
	LONG destinationRowPitch);

#endif

#include "stdh.h"

#include <d3d9.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12TextureFormat.h>

namespace
{
	const D3D12_SHADER_COMPONENT_MAPPING MEMORY_0 =
		D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0;
	const D3D12_SHADER_COMPONENT_MAPPING MEMORY_1 =
		D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1;
	const D3D12_SHADER_COMPONENT_MAPPING FORCE_1 =
		D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_1;

	UINT EncodeMapping(
		D3D12_SHADER_COMPONENT_MAPPING component0,
		D3D12_SHADER_COMPONENT_MAPPING component1,
		D3D12_SHADER_COMPONENT_MAPPING component2,
		D3D12_SHADER_COMPONENT_MAPPING component3)
	{
		return D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(
			component0,
			component1,
			component2,
			component3);
	}
}

DirectX12TextureFormatInfo::DirectX12TextureFormatInfo()
	: format(DXGI_FORMAT_UNKNOWN)
	, componentMapping(D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING)
	, conversion(DX12_TEXTURE_CONVERSION_NONE)
{
}

bool GetDirectX12TextureFormat(
	D3DFORMAT legacyFormat,
	DirectX12TextureFormatInfo* pFormatInfo)
{
	if (pFormatInfo == NULL)
		return false;

	*pFormatInfo = DirectX12TextureFormatInfo();
	switch (legacyFormat)
	{
	case D3DFMT_A8R8G8B8:
		pFormatInfo->format = DXGI_FORMAT_B8G8R8A8_UNORM;
		return true;
	case D3DFMT_X8R8G8B8:
		pFormatInfo->format = DXGI_FORMAT_B8G8R8X8_UNORM;
		return true;
	case D3DFMT_R5G6B5:
		pFormatInfo->format = DXGI_FORMAT_B5G6R5_UNORM;
		return true;
	case D3DFMT_A1R5G5B5:
		pFormatInfo->format = DXGI_FORMAT_B5G5R5A1_UNORM;
		return true;
	case D3DFMT_X1R5G5B5:
		pFormatInfo->format = DXGI_FORMAT_B5G5R5A1_UNORM;
		pFormatInfo->componentMapping = EncodeMapping(
			D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0,
			D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1,
			D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2,
			FORCE_1);
		return true;
	case D3DFMT_A4R4G4B4:
		pFormatInfo->format = DXGI_FORMAT_B8G8R8A8_UNORM;
		pFormatInfo->conversion =
			DX12_TEXTURE_CONVERSION_A4R4G4B4_TO_BGRA8;
		return true;
	case D3DFMT_X4R4G4B4:
		pFormatInfo->format = DXGI_FORMAT_B8G8R8A8_UNORM;
		pFormatInfo->conversion =
			DX12_TEXTURE_CONVERSION_X4R4G4B4_TO_BGRA8;
		return true;
	case D3DFMT_L8:
		pFormatInfo->format = DXGI_FORMAT_R8_UNORM;
		pFormatInfo->componentMapping =
			EncodeMapping(MEMORY_0, MEMORY_0, MEMORY_0, FORCE_1);
		return true;
	case D3DFMT_A8L8:
		pFormatInfo->format = DXGI_FORMAT_R8G8_UNORM;
		pFormatInfo->componentMapping =
			EncodeMapping(MEMORY_0, MEMORY_0, MEMORY_0, MEMORY_1);
		return true;
	case D3DFMT_A8:
		pFormatInfo->format = DXGI_FORMAT_R8_UNORM;
		pFormatInfo->componentMapping =
			EncodeMapping(FORCE_1, FORCE_1, FORCE_1, MEMORY_0);
		return true;
	case D3DFMT_DXT1:
		pFormatInfo->format = DXGI_FORMAT_BC1_UNORM;
		return true;
	case D3DFMT_DXT3:
		pFormatInfo->format = DXGI_FORMAT_BC2_UNORM;
		return true;
	case D3DFMT_DXT5:
		pFormatInfo->format = DXGI_FORMAT_BC3_UNORM;
		return true;
	default:
		return false;
	}
}

bool ConvertDirectX12TextureSubresource(
	const DirectX12TextureFormatInfo& formatInfo,
	const void* pSource,
	LONG sourceRowPitch,
	UINT width,
	UINT height,
	unsigned char* pDestination,
	LONG destinationRowPitch)
{
	if (formatInfo.conversion == DX12_TEXTURE_CONVERSION_NONE
		|| pSource == NULL || pDestination == NULL
		|| width == 0 || height == 0
		|| sourceRowPitch < static_cast<LONG>(width * sizeof(USHORT))
		|| destinationRowPitch < static_cast<LONG>(width * 4U))
		return false;

	const bool preserveAlpha =
		formatInfo.conversion
			== DX12_TEXTURE_CONVERSION_A4R4G4B4_TO_BGRA8;
	if (!preserveAlpha
		&& formatInfo.conversion
			!= DX12_TEXTURE_CONVERSION_X4R4G4B4_TO_BGRA8)
		return false;

	const unsigned char* pSourceBytes =
		static_cast<const unsigned char*>(pSource);
	for (UINT y = 0; y < height; ++y)
	{
		const USHORT* pSourceRow = reinterpret_cast<const USHORT*>(
			pSourceBytes + static_cast<size_t>(y) * sourceRowPitch);
		unsigned char* pDestinationRow =
			pDestination + static_cast<size_t>(y) * destinationRowPitch;
		for (UINT x = 0; x < width; ++x)
		{
			const USHORT pixel = pSourceRow[x];
			pDestinationRow[x * 4U + 0] =
				static_cast<unsigned char>((pixel & 0x000FU) * 17U);
			pDestinationRow[x * 4U + 1] =
				static_cast<unsigned char>(((pixel >> 4) & 0x000FU) * 17U);
			pDestinationRow[x * 4U + 2] =
				static_cast<unsigned char>(((pixel >> 8) & 0x000FU) * 17U);
			pDestinationRow[x * 4U + 3] = preserveAlpha
				? static_cast<unsigned char>(
					((pixel >> 12) & 0x000FU) * 17U)
				: 255U;
		}
	}
	return true;
}

#include "stdh.h"

#include <algorithm>
#include <cstring>
#include <d3d9.h>

#include <Engine/Graphics/DirectX12TextureFormat.h>
#include <Engine/Graphics/DirectX12TextureUploadSource.h>

namespace
{
	bool IsBlockCompressed(D3DFORMAT format)
	{
		return format == D3DFMT_DXT1
			|| format == D3DFMT_DXT3
			|| format == D3DFMT_DXT5;
	}

	UINT GetBlockSize(D3DFORMAT format)
	{
		if (format == D3DFMT_DXT1)
			return 8;
		if (format == D3DFMT_DXT3 || format == D3DFMT_DXT5)
			return 16;
		return 0;
	}

	UINT GetLegacyBytesPerPixel(D3DFORMAT format)
	{
		switch (format)
		{
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
			return 4;
		case D3DFMT_R5G6B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A8L8:
			return 2;
		case D3DFMT_L8:
		case D3DFMT_A8:
			return 1;
		default:
			return 0;
		}
	}

	void WriteU16(unsigned char* pDestination, USHORT value)
	{
		pDestination[0] = static_cast<unsigned char>(value & 0xFFU);
		pDestination[1] =
			static_cast<unsigned char>((value >> 8) & 0xFFU);
	}
}

CDirectX12TextureUploadSource::CDirectX12TextureUploadSource()
	: m_width(0)
	, m_height(0)
	, m_format(DXGI_FORMAT_UNKNOWN)
	, m_componentMapping(D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING)
{
}

CDirectX12TextureUploadSource::CDirectX12TextureUploadSource(
	const CDirectX12TextureUploadSource& other)
	: m_width(other.m_width)
	, m_height(other.m_height)
	, m_format(other.m_format)
	, m_componentMapping(other.m_componentMapping)
	, m_mips(other.m_mips)
{
	RebuildSubresources();
}

CDirectX12TextureUploadSource&
CDirectX12TextureUploadSource::operator=(
	const CDirectX12TextureUploadSource& other)
{
	if (this == &other)
		return *this;
	m_width = other.m_width;
	m_height = other.m_height;
	m_format = other.m_format;
	m_componentMapping = other.m_componentMapping;
	m_mips = other.m_mips;
	RebuildSubresources();
	return *this;
}

void CDirectX12TextureUploadSource::Clear()
{
	m_width = 0;
	m_height = 0;
	m_format = DXGI_FORMAT_UNKNOWN;
	m_componentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_mips.clear();
	m_subresources.clear();
}

bool CDirectX12TextureUploadSource::PrepareFromRgbaMipChain(
	const void* pPixels,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	UINT maximumMipCount)
{
	Clear();
	if (pPixels == NULL || width == 0 || height == 0
		|| maximumMipCount == 0 || IsBlockCompressed(legacyFormat))
		return false;

	DirectX12TextureFormatInfo formatInfo;
	if (!GetDirectX12TextureFormat(legacyFormat, &formatInfo))
		return false;

	UINT mipWidth = width;
	UINT mipHeight = height;
	UINT mipCount = 0;
	while (mipWidth > 0 && mipHeight > 0
		&& mipCount < maximumMipCount)
	{
		++mipCount;
		mipWidth >>= 1;
		mipHeight >>= 1;
	}
	if (mipCount == 0)
		return false;

	m_mips.resize(mipCount);
	const unsigned char* pCurrent =
		static_cast<const unsigned char*>(pPixels);
	mipWidth = width;
	mipHeight = height;
	for (UINT iMip = 0; iMip < mipCount; ++iMip)
	{
		if (!PrepareRgbaMip(
			pCurrent,
			mipWidth,
			mipHeight,
			legacyFormat,
			&m_mips[iMip]))
		{
			Clear();
			return false;
		}
		pCurrent += static_cast<size_t>(mipWidth)
			* mipHeight * 4U;
		mipWidth >>= 1;
		mipHeight >>= 1;
	}

	m_width = width;
	m_height = height;
	m_format = formatInfo.format;
	m_componentMapping = formatInfo.componentMapping;
	RebuildSubresources();
	return true;
}

bool CDirectX12TextureUploadSource::PrepareFromCompressedBlob(
	const void* pBlob,
	size_t blobSize,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	UINT maximumMipCount)
{
	Clear();
	if (pBlob == NULL || blobSize == 0 || width == 0 || height == 0
		|| maximumMipCount == 0 || !IsBlockCompressed(legacyFormat))
		return false;

	DirectX12TextureFormatInfo formatInfo;
	if (!GetDirectX12TextureFormat(legacyFormat, &formatInfo))
		return false;

	const unsigned char* pCurrent =
		static_cast<const unsigned char*>(pBlob);
	size_t remaining = blobSize;
	UINT mipWidth = width;
	UINT mipHeight = height;
	const UINT mipLimit = (std::min)(maximumMipCount, 15U);
	while (remaining >= sizeof(LONG)
		&& m_mips.size() < mipLimit)
	{
		LONG storedSize = 0;
		memcpy(&storedSize, pCurrent, sizeof(storedSize));
		pCurrent += sizeof(storedSize);
		remaining -= sizeof(storedSize);
		if (storedSize <= 0
			|| static_cast<size_t>(storedSize) > remaining)
		{
			Clear();
			return false;
		}

		const UINT blockSize = GetBlockSize(legacyFormat);
		const UINT blockColumns =
			(std::max)(1U, (mipWidth + 3U) / 4U);
		const UINT blockRows =
			(std::max)(1U, (mipHeight + 3U) / 4U);
		const size_t rowPitch =
			static_cast<size_t>(blockColumns) * blockSize;
		const size_t expectedSize = rowPitch * blockRows;
		if (static_cast<size_t>(storedSize) < expectedSize)
		{
			Clear();
			return false;
		}

		MipPayload mip;
		mip.bytes.assign(pCurrent, pCurrent + expectedSize);
		mip.rowPitch = static_cast<LONG_PTR>(rowPitch);
		mip.slicePitch = static_cast<LONG_PTR>(expectedSize);
		m_mips.push_back(mip);

		pCurrent += storedSize;
		remaining -= storedSize;
		mipWidth = (std::max)(1U, mipWidth >> 1);
		mipHeight = (std::max)(1U, mipHeight >> 1);
	}
	if (m_mips.empty())
	{
		Clear();
		return false;
	}

	m_width = width;
	m_height = height;
	m_format = formatInfo.format;
	m_componentMapping = formatInfo.componentMapping;
	RebuildSubresources();
	return true;
}

bool CDirectX12TextureUploadSource::PrepareRgbaMip(
	const unsigned char* pPixels,
	UINT width,
	UINT height,
	D3DFORMAT legacyFormat,
	MipPayload* pMip)
{
	if (pPixels == NULL || width == 0 || height == 0 || pMip == NULL)
		return false;

	UINT outputBytesPerPixel = 0;
	switch (legacyFormat)
	{
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
	case D3DFMT_A4R4G4B4:
	case D3DFMT_X4R4G4B4:
		outputBytesPerPixel = 4;
		break;
	case D3DFMT_R5G6B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A8L8:
		outputBytesPerPixel = 2;
		break;
	case D3DFMT_L8:
	case D3DFMT_A8:
		outputBytesPerPixel = 1;
		break;
	default:
		return false;
	}

	const size_t rowPitch =
		static_cast<size_t>(width) * outputBytesPerPixel;
	const size_t slicePitch = rowPitch * height;
	pMip->bytes.resize(slicePitch);
	for (UINT y = 0; y < height; ++y)
	{
		const unsigned char* pSourceRow =
			pPixels + static_cast<size_t>(y) * width * 4U;
		unsigned char* pDestinationRow =
			&pMip->bytes[0] + static_cast<size_t>(y) * rowPitch;
		for (UINT x = 0; x < width; ++x)
		{
			const unsigned char* pSource = pSourceRow + x * 4U;
			const unsigned char red = pSource[0];
			const unsigned char green = pSource[1];
			const unsigned char blue = pSource[2];
			const unsigned char alpha = pSource[3];
			unsigned char* pDestination =
				pDestinationRow + x * outputBytesPerPixel;
			switch (legacyFormat)
			{
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8R8G8B8:
				pDestination[0] = blue;
				pDestination[1] = green;
				pDestination[2] = red;
				pDestination[3] =
					legacyFormat == D3DFMT_A8R8G8B8 ? alpha : 255;
				break;
			case D3DFMT_R5G6B5:
				WriteU16(
					pDestination,
					static_cast<USHORT>(
						(static_cast<USHORT>(red >> 3) << 11)
						| (static_cast<USHORT>(green >> 2) << 5)
						| static_cast<USHORT>(blue >> 3)));
				break;
			case D3DFMT_A1R5G5B5:
			case D3DFMT_X1R5G5B5:
			{
				const USHORT alphaBit =
					legacyFormat == D3DFMT_X1R5G5B5
						|| alpha >= 128 ? 0x8000U : 0U;
				WriteU16(
					pDestination,
					static_cast<USHORT>(
						alphaBit
						| (static_cast<USHORT>(red >> 3) << 10)
						| (static_cast<USHORT>(green >> 3) << 5)
						| static_cast<USHORT>(blue >> 3)));
				break;
			}
			case D3DFMT_A4R4G4B4:
			case D3DFMT_X4R4G4B4:
				pDestination[0] =
					static_cast<unsigned char>((blue >> 4) * 17U);
				pDestination[1] =
					static_cast<unsigned char>((green >> 4) * 17U);
				pDestination[2] =
					static_cast<unsigned char>((red >> 4) * 17U);
				pDestination[3] =
					legacyFormat == D3DFMT_A4R4G4B4
						? static_cast<unsigned char>(
							(alpha >> 4) * 17U)
						: 255;
				break;
			case D3DFMT_L8:
				pDestination[0] = red;
				break;
			case D3DFMT_A8L8:
				pDestination[0] = red;
				pDestination[1] = alpha;
				break;
			case D3DFMT_A8:
				pDestination[0] = alpha;
				break;
			}
		}
	}
	pMip->rowPitch = static_cast<LONG_PTR>(rowPitch);
	pMip->slicePitch = static_cast<LONG_PTR>(slicePitch);
	return true;
}

void CDirectX12TextureUploadSource::RebuildSubresources()
{
	m_subresources.resize(m_mips.size());
	for (size_t iMip = 0; iMip < m_mips.size(); ++iMip)
	{
		m_subresources[iMip].pData = m_mips[iMip].bytes.empty()
			? NULL
			: &m_mips[iMip].bytes[0];
		m_subresources[iMip].rowPitch = m_mips[iMip].rowPitch;
		m_subresources[iMip].slicePitch = m_mips[iMip].slicePitch;
	}
}

UINT CDirectX12TextureUploadSource::GetWidth() const
{
	return m_width;
}

UINT CDirectX12TextureUploadSource::GetHeight() const
{
	return m_height;
}

UINT16 CDirectX12TextureUploadSource::GetMipCount() const
{
	return static_cast<UINT16>(m_mips.size());
}

DXGI_FORMAT CDirectX12TextureUploadSource::GetFormat() const
{
	return m_format;
}

UINT CDirectX12TextureUploadSource::GetComponentMapping() const
{
	return m_componentMapping;
}

const DirectX12SubresourceData*
CDirectX12TextureUploadSource::GetSubresources() const
{
	return m_subresources.empty() ? NULL : &m_subresources[0];
}

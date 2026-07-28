#include "stdh.h"

#include <Engine/Graphics/Texture.h>

#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Graphics/DirectX12Backend.h>
#include <Engine/Graphics/GfxLibrary.h>

#define TRACKTEX_HEAP() TRACKMEM(mem, "Textures (heap)")

extern ENGINE_API INDEX sam_iGfxAPI;

CRenderTexture::CRenderTexture()
{
	rt_pSurface = NULL;
	m_pOldRenderTarget = NULL;
	m_pOldDepthStencil = NULL;
	m_pDepthStencil = NULL;
	m_bNativeColorTarget = FALSE;
	m_bNativeColorActive = FALSE;
	m_bOldZEnable = FALSE;
	m_nativeColorHandle = DX12_INVALID_RENDER_TEXTURE;
}

CRenderTexture::~CRenderTexture()
{
	if (m_nativeColorHandle != DX12_INVALID_RENDER_TEXTURE)
	{
		GetDirectX12Backend().DestroyNativeOffscreenTexture(
			m_nativeColorHandle);
		m_nativeColorHandle = DX12_INVALID_RENDER_TEXTURE;
		rt_tdTexture.td_ulObject = NONE;
	}
}

BOOL CRenderTexture::Init(
	INDEX width,
	INDEX height,
	ULONG flag,
	D3DFORMAT fmt,
	ERenderTexturePurpose purpose)
{
	TRACKTEX_HEAP();
	if (width > MAX_MEX || height > MAX_MEX)
	{
		ASSERTALWAYS("El tamano de la textura supera el maximo permitido.");
		return FALSE;
	}

	rt_tdTexture.td_ulInternalFormat = fmt;
	rt_tdTexture.td_iFirstMipLevel = 0;
	rt_tdTexture.td_ulFlags = flag;
	rt_tdTexture.td_ulFlags |= TEX_ALPHACHANNEL;
	rt_tdTexture.td_ulFlags |= TEX_TRANSPARENT;
	rt_tdTexture.td_ulFlags |= TEX_32BIT;
	rt_tdTexture.td_ulFlags |= TEX_CONSTANT;
	rt_tdTexture.td_ulFlags &= ~TEX_STATIC;
	rt_tdTexture.td_ulFlags &= ~(TEX_COMPRESS | TEX_COMPRESSALPHA);
	rt_tdTexture.td_ctFrames = 1;
	rt_tdTexture.td_mexWidth = width;
	rt_tdTexture.td_mexHeight = height;
	rt_tdTexture.td_ctFineMipLevels = 1;
	rt_tdTexture.td_slFrameSize = 0;
	rt_tdTexture.td_pulFrames = NULL;

	if (sam_iGfxAPI == GAT_OGL)
		return FALSE;
	if (sam_iGfxAPI != GAT_D3D)
		return FALSE;

	if (!GetDirectX12Backend().CreateNativeOffscreenTexture(
		width,
		height,
		static_cast<INT>(fmt),
		&m_nativeColorHandle))
	{
		rt_tdTexture.td_ulObject = NONE;
		return FALSE;
	}

	rt_tdTexture.td_ulObject = m_nativeColorHandle.GetValue();
	m_bNativeColorTarget = TRUE;
	static BOOL bNativeTargetReported = FALSE;
	if (!bNativeTargetReported)
	{
		CPrintF(
			"DX12 offscreen: render texture creada directamente "
			"sin identidad D3D9.\n");
		bNativeTargetReported = TRUE;
	}
	return TRUE;
}

void CRenderTexture::Begin()
{
	if (sam_iGfxAPI != GAT_D3D)
		return;
	GetDirectX12Backend().InsertDrawPortBarrier(
		DX12_DRAWPORT_BARRIER_RENDER_TARGET_BEGIN);
	GetDirectX12Backend().BeginOffscreenDrawPortScope();
	m_bNativeColorActive = m_bNativeColorTarget
		&& GetDirectX12Backend().BeginNativeOffscreenTexture(
			m_nativeColorHandle);
	GetDirectX12Backend().TrackLegacy3DRenderTarget(
		DX12_LEGACY_RENDER_TARGET_OFFSCREEN);
}

void CRenderTexture::Clear(COLOR colClear, FLOAT fZVal)
{
	if (sam_iGfxAPI == GAT_D3D && m_bNativeColorActive)
		GetDirectX12Backend().ClearNativeOffscreenTexture(colClear);
}

void CRenderTexture::End()
{
	if (sam_iGfxAPI != GAT_D3D)
		return;
	GetDirectX12Backend().EndOffscreenDrawPortScope();
	if (m_bNativeColorActive)
	{
		GetDirectX12Backend().EndNativeOffscreenTexture();
		m_bNativeColorActive = FALSE;
	}
	GetDirectX12Backend().TrackLegacy3DRenderTarget(
		DX12_LEGACY_RENDER_TARGET_PRESENTATION);
	GetDirectX12Backend().InsertDrawPortBarrier(
		DX12_DRAWPORT_BARRIER_RENDER_TARGET_END);
}

DirectX12RenderTextureHandle
CRenderTexture::GetNativeTextureHandle() const
{
	return m_nativeColorHandle;
}

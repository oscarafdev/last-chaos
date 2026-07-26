#include "stdh.h"

#include <Engine/Graphics/Texture.h>

#include <Engine/Base/Stream.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/Console.h>
#include <Engine/Math/Functions.h>
#include <Engine/Graphics/DirectX12Backend.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Graphics/TextureEffects.h>

#include <Engine/Templates/DynamicArray.h>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/Stock_CtextureData.h>
#include <Engine/Templates/StaticArray.cpp>

#include <Engine/Base/Statistics_internal.h>

// Inicio de la incorporacion de CRenderTexture para renderizar a textura.
#include <Engine/Base/MemoryTracking.h>
// ###
#include <d3d9.h>

// track texture memory allocated in main heap
#define TRACKTEX_HEAP() TRACKMEM(mem, "Textures (heap)")
// track texture memory allocated in hardware
#define TRACKTEX_HW() TRACKMEM(mem, "Textures (hardware)")

extern ENGINE_API INDEX sam_iGfxAPI;	// 0==OpenGL

CRenderTexture::CRenderTexture()
{
	rt_pSurface = NULL;
	m_pDepthStencil = NULL;
	m_bNativeColorTarget = FALSE;
	m_bNativeColorActive = FALSE;
}

CRenderTexture::~CRenderTexture()
{
	if(rt_pSurface) rt_pSurface->Release();
	if(m_pDepthStencil) m_pDepthStencil->Release();
}

// Se agrega el parametro de formato D3D.
BOOL CRenderTexture::Init(
	INDEX width,
	INDEX height,
	ULONG flag,
	D3DFORMAT fmt,
	ERenderTexturePurpose purpose)
{
	TRACKTEX_HEAP();
	
	// check maximum supported texture dimension
	if( width>MAX_MEX || height>MAX_MEX)
	{
		ASSERTALWAYS("El tamano de la textura supera el maximo permitido.");
		return FALSE;
	}

	// El formato fijo D3DFMT_A8R8G8B8 se reemplaza por el parametro fmt.
	rt_tdTexture.td_ulInternalFormat = fmt;
	
	// determine mip index from mex size and alpha channel presence
	rt_tdTexture.td_iFirstMipLevel = 0;
	rt_tdTexture.td_ulFlags = flag;
	rt_tdTexture.td_ulFlags |= TEX_ALPHACHANNEL;	//alhpa
	rt_tdTexture.td_ulFlags |= TEX_TRANSPARENT;		// Siempre se habilita cuando existe canal alfa.
	rt_tdTexture.td_ulFlags |= TEX_32BIT;
	rt_tdTexture.td_ulFlags |= TEX_CONSTANT;
	rt_tdTexture.td_ulFlags &= ~TEX_STATIC;
	rt_tdTexture.td_ulFlags &= ~(TEX_COMPRESS|TEX_COMPRESSALPHA);	//not allowed compress in shadow

	// initialize general TextureData members
	rt_tdTexture.td_ctFrames  = 0;
	rt_tdTexture.td_mexWidth  = width << rt_tdTexture.td_iFirstMipLevel;
	rt_tdTexture.td_mexHeight = height << rt_tdTexture.td_iFirstMipLevel;
	
	// create all mip levels (either bilinear or downsampled)
	rt_tdTexture.td_ctFineMipLevels = 1;
	// get frame size (includes only one mip-map)
	rt_tdTexture.td_slFrameSize = 0;
	
	// allocate small ammount of memory just for Realloc sake
	rt_tdTexture.td_pulFrames = NULL;
	rt_tdTexture.td_ctFrames = 1;

	if( sam_iGfxAPI==GAT_OGL)
	{
		// ### TODO:
		// OpenGL
	} 
	else if( sam_iGfxAPI==GAT_D3D)
	{
		// Crea la textura Direct3D usada como render target.
		IDirect3DTexture9 *pTexture = NULL;
		// El formato fijo D3DFMT_A8R8G8B8 se reemplaza por el parametro fmt.
		HRESULT hr = _pGfx->gl_pd3d9Device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &pTexture, NULL);
		if(hr == NOERROR)
		{
			rt_tdTexture.td_ulObject = (ULONG64)pTexture;	// Conserva la textura creada.
			pTexture->GetSurfaceLevel(0, &rt_pSurface);
			if (purpose == RTP_SHADOW_MAP
				&& !GetDirectX12Backend().RequiresLegacyOffscreenDepth()
				&& GetDirectX12Backend().RegisterNativeOffscreenTexture(
					pTexture,
					width,
					height,
					static_cast<INT>(fmt)))
			{
				m_bNativeColorTarget = TRUE;
				static BOOL bNativeDepthReported = FALSE;
				if (!bNativeDepthReported)
				{
					CPrintF(
						"DX12 offscreen: profundidad auxiliar "
						"administrada por el backend nativo.\n");
					bNativeDepthReported = TRUE;
				}
				return TRUE;
			}
			// Los modos de comparación conservan el depth legado porque sus
			// draws D3D9 siguen siendo visibles junto con la pasada DX12.
		    hr = _pGfx->gl_pd3d9Device->CreateDepthStencilSurface(width, height, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pDepthStencil, NULL);
			if(hr == NOERROR)
			{
				return TRUE;
			}
		}
		else
		{
			rt_tdTexture.td_ulObject = NULL;
			return FALSE;
		}
	}
	return FALSE;
}

void CRenderTexture::Begin()	// SetRenderTarget current
{
	if( sam_iGfxAPI==GAT_OGL)
	{
		// ### TODO:
		// OpenGL
	} 
	else if( sam_iGfxAPI==GAT_D3D)
	{
		GetDirectX12Backend().InsertDrawPortBarrier(
			DX12_DRAWPORT_BARRIER_RENDER_TARGET_BEGIN);
		IDirect3DTexture9* pTexture = reinterpret_cast<IDirect3DTexture9*>(
			rt_tdTexture.td_ulObject);
		m_bNativeColorActive = m_bNativeColorTarget
			&& GetDirectX12Backend().BeginNativeOffscreenTexture(
				pTexture);
		GetDirectX12Backend().BeginOffscreenDrawPortScope();
		IDirect3DDevice9* pDev = _pGfx->gl_pd3d9Device;
		pDev->GetRenderTarget(0, &m_pOldRenderTarget);
		pDev->GetDepthStencilSurface(&m_pOldDepthStencil);
		pDev->SetDepthStencilSurface(m_pDepthStencil);
		pDev->SetRenderTarget(0, rt_pSurface);
	}
}

void CRenderTexture::Clear(COLOR colClear, FLOAT fZVal)
{
	if( sam_iGfxAPI==GAT_OGL)
	{
		// ### TODO:
		// OpenGL
	} 
	else if( sam_iGfxAPI==GAT_D3D)
	{
		if (m_bNativeColorActive)
			GetDirectX12Backend().ClearNativeOffscreenTexture(colClear);
		const DWORD clearFlags = m_pDepthStencil != NULL
			? D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
			: D3DCLEAR_TARGET;
		_pGfx->gl_pd3d9Device->Clear(
			0, NULL, clearFlags, colClear, fZVal, 0);
	}
}

void CRenderTexture::End()		// SetRenderTarget old
{
	if( sam_iGfxAPI==GAT_OGL)
	{
		// ### TODO:
		// OpenGL
	} 
	else if( sam_iGfxAPI==GAT_D3D)
	{
		IDirect3DDevice9 *pDev = _pGfx->gl_pd3d9Device;
		// El backend debe reproducir la geometria DX12 mientras la textura
		// auxiliar y su profundidad siguen siendo el destino activo.
		GetDirectX12Backend().EndOffscreenDrawPortScope();
		if (m_bNativeColorActive)
		{
			GetDirectX12Backend().EndNativeOffscreenTexture();
			m_bNativeColorActive = FALSE;
		}
		//pDev->SetRenderTarget(m_pOldRenderTarget, m_pOldDepthStencil);
	    pDev->SetDepthStencilSurface(m_pOldDepthStencil);
	    pDev->SetRenderTarget(0, m_pOldRenderTarget);
		GetDirectX12Backend().InsertDrawPortBarrier(
			DX12_DRAWPORT_BARRIER_RENDER_TARGET_END);
		if(m_pOldRenderTarget)
		{
			m_pOldRenderTarget->Release();
			m_pOldRenderTarget = NULL;
		}

		if(m_pOldDepthStencil)
		{
			m_pOldDepthStencil->Release();
			m_pOldDepthStencil = NULL;
		}
	}
}
// Fin de la incorporacion de CRenderTexture para renderizar a textura.

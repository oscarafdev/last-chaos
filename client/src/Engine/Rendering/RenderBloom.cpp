#include "stdh.h"

#include <Engine/Graphics/DirectX12Backend.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Rendering/Render.h>
#include <Engine/Rendering/Render_internal.h>

namespace
{
	const int BLOOM_FILTER_SIZE = 512;

	CRenderTexture* g_filterTargets[2] = { NULL, NULL };
	CRenderTexture* g_bloomSource = NULL;

	void ReleaseBloomRenderTargets()
	{
		for (int i = 0; i < 2; ++i)
		{
			delete g_filterTargets[i];
			g_filterTargets[i] = NULL;
		}
		delete g_bloomSource;
		g_bloomSource = NULL;
	}

	HRESULT ReportBloomInitializationFailure(
		const char* stage,
		HRESULT error)
	{
		CPrintF(
			"Bloom: etapa '%s' fallo (HRESULT=0x%08X).\n",
			stage,
			static_cast<ULONG>(error));
		return error;
	}
}

INDEX d3d_bDeviceChanged = TRUE;
BOOL _bFirst = TRUE;

extern void ReleaseBloomTexture()
{
	ReleaseBloomRenderTargets();
	_bFirst = TRUE;
	d3d_bDeviceChanged = TRUE;
}

HRESULT CRenderer::InitBloom()
{
	CDirectX12Backend& dx12 = GetDirectX12Backend();
	if (!dx12.IsFull3DReplacementEnabled())
		return E_NOTIMPL;

	if (_bFirst || d3d_bDeviceChanged)
	{
		const HRESULT hr = CreateTextureRenderTargets(
			BLOOM_FILTER_SIZE,
			BLOOM_FILTER_SIZE);
		if (FAILED(hr))
			return ReportBloomInitializationFailure(
				"configurar recursos nativos",
				hr);
		_bFirst = FALSE;
		d3d_bDeviceChanged = FALSE;
	}
	return S_OK;
}

HRESULT CRenderer::RenderBloom()
{
	CDirectX12Backend& dx12 = GetDirectX12Backend();
	if (!dx12.IsFull3DReplacementEnabled())
		return E_NOTIMPL;
	if (g_bloomSource == NULL || g_filterTargets[0] == NULL
		|| g_filterTargets[1] == NULL)
		return E_FAIL;

	return dx12.RenderNativeBloom(
		g_bloomSource->GetNativeTextureHandle(),
		g_filterTargets[0]->GetNativeTextureHandle(),
		g_filterTargets[1]->GetNativeTextureHandle())
		? S_OK
		: E_FAIL;
}

HRESULT CRenderer::CreateTextureRenderTargets(int width, int height)
{
	ReleaseBloomRenderTargets();

	IDirect3DSurface9* pBackbufferColor = NULL;
	D3DSURFACE_DESC backbufferDescription;
	if (FAILED(_pGfx->gl_pd3d9Device->GetRenderTarget(
			0,
			&pBackbufferColor))
		|| pBackbufferColor == NULL
		|| FAILED(pBackbufferColor->GetDesc(&backbufferDescription)))
	{
		if (pBackbufferColor != NULL)
			pBackbufferColor->Release();
		return E_FAIL;
	}
	pBackbufferColor->Release();

	for (int i = 0; i < 2; ++i)
	{
		g_filterTargets[i] = new CRenderTexture();
		if (g_filterTargets[i] == NULL
			|| !g_filterTargets[i]->Init(
				width,
				height,
				TEX_32BIT,
				backbufferDescription.Format,
				RTP_POST_PROCESS))
		{
			ReleaseBloomRenderTargets();
			return E_FAIL;
		}
	}

	g_bloomSource = new CRenderTexture();
	if (g_bloomSource == NULL
		|| !g_bloomSource->Init(
			backbufferDescription.Width,
			backbufferDescription.Height,
			TEX_32BIT,
			backbufferDescription.Format,
			RTP_POST_PROCESS))
	{
		ReleaseBloomRenderTargets();
		return E_FAIL;
	}
	return S_OK;
}

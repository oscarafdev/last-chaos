#include "stdh.h"

#include <d3d9.h>

#include <Engine/Graphics/DirectX12LegacyDrawState.h>

namespace
{
	void SetIdentity(FLOAT* pMatrix)
	{
		ZeroMemory(pMatrix, sizeof(FLOAT) * 16);
		pMatrix[0] = 1.0f;
		pMatrix[5] = 1.0f;
		pMatrix[10] = 1.0f;
		pMatrix[15] = 1.0f;
	}
}

CDirectX12LegacyDrawState::CDirectX12LegacyDrawState()
	: zEnable(FALSE)
	, zWrite(FALSE)
	, zFunction(D3DCMP_LESSEQUAL)
	, clipping(TRUE)
	, cullMode(D3DCULL_NONE)
	, blending(FALSE)
	, alphaTest(FALSE)
	, sourceBlend(D3DBLEND_SRCALPHA)
	, destinationBlend(D3DBLEND_INVSRCALPHA)
	, alphaReference(128)
	, colorWriteMask(
		D3DCOLORWRITEENABLE_RED
		| D3DCOLORWRITEENABLE_GREEN
		| D3DCOLORWRITEENABLE_BLUE
		| D3DCOLORWRITEENABLE_ALPHA)
	, textureFactor(0xFFFFFFFFUL)
	, samplerAddressU(D3DTADDRESS_WRAP)
	, samplerAddressV(D3DTADDRESS_WRAP)
	, samplerMinification(D3DTEXF_LINEAR)
	, samplerMagnification(D3DTEXF_LINEAR)
	, vertexDeclarationByteCount(0)
	, pVertexShader(NULL)
	, pPixelShader(NULL)
{
	SetIdentity(world);
	SetIdentity(view);
	SetIdentity(projection);
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.minimumDepth = 0.0f;
	viewport.maximumDepth = 1.0f;
	ZeroMemory(bumpEnvironmentState, sizeof(bumpEnvironmentState));
	ZeroMemory(vertexShaderConstants, sizeof(vertexShaderConstants));
	ZeroMemory(pixelShaderConstants, sizeof(pixelShaderConstants));
	ZeroMemory(vertexDeclaration, sizeof(vertexDeclaration));
	ZeroMemory(textures, sizeof(textures));
	for (UINT textureUnit = 0;
		textureUnit < DX12_LEGACY_TEXTURE_STAGE_COUNT;
		++textureUnit)
	{
		SetIdentity(textureTransform[textureUnit]);
		fixedColorOperation[textureUnit] = D3DTOP_DISABLE;
		fixedColorArgument1[textureUnit] = D3DTA_TEXTURE;
		fixedColorArgument2[textureUnit] = D3DTA_CURRENT;
		fixedAlphaOperation[textureUnit] = D3DTOP_DISABLE;
		fixedAlphaArgument1[textureUnit] = D3DTA_TEXTURE;
		fixedAlphaArgument2[textureUnit] = D3DTA_CURRENT;
		fixedResultArgument[textureUnit] = D3DTA_CURRENT;
		fixedStageConstant[textureUnit] = 0xFFFFFFFFUL;
		fixedTexCoordIndex[textureUnit] = textureUnit;
		fixedTextureTransformFlags[textureUnit] = D3DTTFF_DISABLE;
	}
}

CDirectX12LegacyDrawState::~CDirectX12LegacyDrawState()
{
	ReleaseBindings();
}

void CDirectX12LegacyDrawState::ReleaseBindings()
{
	for (UINT textureUnit = 0;
		textureUnit < DX12_LEGACY_TEXTURE_STAGE_COUNT;
		++textureUnit)
	{
		if (textures[textureUnit] != NULL)
		{
			textures[textureUnit]->Release();
			textures[textureUnit] = NULL;
		}
	}
	if (pVertexShader != NULL)
	{
		pVertexShader->Release();
		pVertexShader = NULL;
	}
	if (pPixelShader != NULL)
	{
		pPixelShader->Release();
		pPixelShader = NULL;
	}
}

bool CDirectX12LegacyDrawState::Capture(
	IDirect3DDevice9* pDevice9,
	bool usesVertexProgram,
	bool usesPixelProgram)
{
	if (pDevice9 == NULL)
		return false;
	ReleaseBindings();

	D3DVIEWPORT9 viewport9;
	if (FAILED(pDevice9->GetTransform(
			D3DTS_WORLD,
			reinterpret_cast<D3DMATRIX*>(world)))
		|| FAILED(pDevice9->GetTransform(
			D3DTS_VIEW,
			reinterpret_cast<D3DMATRIX*>(view)))
		|| FAILED(pDevice9->GetTransform(
			D3DTS_PROJECTION,
			reinterpret_cast<D3DMATRIX*>(projection)))
		|| FAILED(pDevice9->GetViewport(&viewport9))
		|| FAILED(pDevice9->GetRenderState(D3DRS_ZENABLE, &zEnable))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_ZWRITEENABLE, &zWrite))
		|| FAILED(pDevice9->GetRenderState(D3DRS_ZFUNC, &zFunction))
		|| FAILED(pDevice9->GetRenderState(D3DRS_CLIPPING, &clipping))
		|| FAILED(pDevice9->GetRenderState(D3DRS_CULLMODE, &cullMode))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_ALPHABLENDENABLE, &blending))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_ALPHATESTENABLE, &alphaTest))
		|| FAILED(pDevice9->GetRenderState(D3DRS_SRCBLEND, &sourceBlend))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_DESTBLEND, &destinationBlend))
		|| FAILED(pDevice9->GetRenderState(D3DRS_ALPHAREF, &alphaReference))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_COLORWRITEENABLE, &colorWriteMask))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_TEXTUREFACTOR, &textureFactor))
		|| FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_ADDRESSU, &samplerAddressU))
		|| FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_ADDRESSV, &samplerAddressV))
		|| FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_MINFILTER, &samplerMinification))
		|| FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_MAGFILTER, &samplerMagnification))
		|| (usesVertexProgram
			&& (FAILED(pDevice9->GetVertexShader(&pVertexShader))
				|| pVertexShader == NULL
				|| FAILED(pDevice9->GetVertexShaderConstantF(
					0,
					vertexShaderConstants,
					DX12_LEGACY_VERTEX_CONSTANT_COUNT))))
		|| (usesPixelProgram
			&& (FAILED(pDevice9->GetPixelShader(&pPixelShader))
				|| pPixelShader == NULL
				|| FAILED(pDevice9->GetPixelShaderConstantF(
					0,
					pixelShaderConstants,
					DX12_LEGACY_PIXEL_CONSTANT_COUNT)))))
	{
		ReleaseBindings();
		return false;
	}

	viewport.x = viewport9.X;
	viewport.y = viewport9.Y;
	viewport.width = viewport9.Width;
	viewport.height = viewport9.Height;
	viewport.minimumDepth = viewport9.MinZ;
	viewport.maximumDepth = viewport9.MaxZ;
	if (usesVertexProgram)
	{
		IDirect3DVertexDeclaration9* pDeclaration = NULL;
		if (SUCCEEDED(pDevice9->GetVertexDeclaration(&pDeclaration))
			&& pDeclaration != NULL)
		{
			UINT elementCount = 0;
			if (SUCCEEDED(pDeclaration->GetDeclaration(
					NULL,
					&elementCount))
				&& elementCount > 0
				&& elementCount * sizeof(D3DVERTEXELEMENT9)
					<= sizeof(vertexDeclaration)
				&& SUCCEEDED(pDeclaration->GetDeclaration(
					reinterpret_cast<D3DVERTEXELEMENT9*>(
						vertexDeclaration),
					&elementCount)))
			{
				vertexDeclarationByteCount =
					elementCount * sizeof(D3DVERTEXELEMENT9);
			}
			pDeclaration->Release();
		}
	}

	for (UINT component = 0; component < 4; ++component)
	{
		if (FAILED(pDevice9->GetTextureStageState(
			1,
			static_cast<D3DTEXTURESTAGESTATETYPE>(
				D3DTSS_BUMPENVMAT00 + component),
			&bumpEnvironmentState[component])))
			return false;
	}

	for (UINT textureUnit = 0;
		textureUnit < DX12_LEGACY_TEXTURE_STAGE_COUNT;
		++textureUnit)
	{
		if (FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_COLOROP,
				&fixedColorOperation[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_COLORARG1,
				&fixedColorArgument1[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_COLORARG2,
				&fixedColorArgument2[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_ALPHAOP,
				&fixedAlphaOperation[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_ALPHAARG1,
				&fixedAlphaArgument1[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_ALPHAARG2,
				&fixedAlphaArgument2[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_RESULTARG,
				&fixedResultArgument[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_CONSTANT,
				&fixedStageConstant[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_TEXCOORDINDEX,
				&fixedTexCoordIndex[textureUnit]))
			|| FAILED(pDevice9->GetTextureStageState(
				textureUnit, D3DTSS_TEXTURETRANSFORMFLAGS,
				&fixedTextureTransformFlags[textureUnit]))
			|| FAILED(pDevice9->GetTransform(
				static_cast<D3DTRANSFORMSTATETYPE>(
					D3DTS_TEXTURE0 + textureUnit),
				reinterpret_cast<D3DMATRIX*>(
					textureTransform[textureUnit]))))
		{
			ReleaseBindings();
			return false;
		}

		IDirect3DBaseTexture9* pBaseTexture = NULL;
		if (SUCCEEDED(pDevice9->GetTexture(textureUnit, &pBaseTexture))
			&& pBaseTexture != NULL)
		{
			pBaseTexture->QueryInterface(
				__uuidof(IDirect3DTexture9),
				reinterpret_cast<void**>(&textures[textureUnit]));
			pBaseTexture->Release();
		}
	}
	return true;
}

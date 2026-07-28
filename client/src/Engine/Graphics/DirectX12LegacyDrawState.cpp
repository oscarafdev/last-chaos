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
{
	ZeroMemory(vertexDeclaration, sizeof(vertexDeclaration));
	ZeroMemory(textures, sizeof(textures));
	pVertexShader = NULL;
	pPixelShader = NULL;
	Reset();
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
		textureHandles[textureUnit] = DirectX12TextureHandle();
		renderTextureHandles[textureUnit] =
			DirectX12RenderTextureHandle();
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

void CDirectX12LegacyDrawState::Reset()
{
	ReleaseBindings();
	SetIdentity(world);
	SetIdentity(view);
	SetIdentity(projection);
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.width = 8;
	viewport.height = 8;
	viewport.minimumDepth = 0.0f;
	viewport.maximumDepth = 1.0f;
	zEnable = FALSE;
	zWrite = FALSE;
	zFunction = D3DCMP_LESSEQUAL;
	clipping = TRUE;
	cullMode = D3DCULL_NONE;
	blending = FALSE;
	alphaTest = FALSE;
	sourceBlend = D3DBLEND_ONE;
	destinationBlend = D3DBLEND_ONE;
	alphaReference = 128;
	colorWriteMask =
		D3DCOLORWRITEENABLE_RED
		| D3DCOLORWRITEENABLE_GREEN
		| D3DCOLORWRITEENABLE_BLUE
		| D3DCOLORWRITEENABLE_ALPHA;
	textureFactor = 0xFFFFFFFFUL;
	samplerAddressU = D3DTADDRESS_WRAP;
	samplerAddressV = D3DTADDRESS_WRAP;
	samplerMinification = D3DTEXF_POINT;
	samplerMagnification = D3DTEXF_POINT;
	ZeroMemory(bumpEnvironmentState, sizeof(bumpEnvironmentState));
	ZeroMemory(vertexShaderConstants, sizeof(vertexShaderConstants));
	ZeroMemory(pixelShaderConstants, sizeof(pixelShaderConstants));
	ZeroMemory(vertexDeclaration, sizeof(vertexDeclaration));
	vertexDeclarationByteCount = 0;
	dynamicGeometry = false;
	for (UINT textureUnit = 0;
		textureUnit < DX12_LEGACY_TEXTURE_STAGE_COUNT;
		++textureUnit)
	{
		SetIdentity(textureTransform[textureUnit]);
		fixedColorOperation[textureUnit] = D3DTOP_DISABLE;
		fixedColorArgument1[textureUnit] = D3DTA_TEXTURE;
		fixedColorArgument2[textureUnit] = D3DTA_CURRENT;
		fixedAlphaOperation[textureUnit] = D3DTOP_MODULATE;
		fixedAlphaArgument1[textureUnit] = D3DTA_TEXTURE;
		fixedAlphaArgument2[textureUnit] = D3DTA_CURRENT;
		fixedResultArgument[textureUnit] = D3DTA_CURRENT;
		fixedStageConstant[textureUnit] = 0xFFFFFFFFUL;
		fixedTexCoordIndex[textureUnit] = textureUnit;
		fixedTextureTransformFlags[textureUnit] = D3DTTFF_DISABLE;
	}
}

bool CDirectX12LegacyDrawState::IsValidForDraw(
	bool usesVertexProgram,
	bool usesPixelProgram) const
{
	return viewport.width > 0 && viewport.height > 0
		&& (!usesVertexProgram
			|| (pVertexShader != NULL
				&& vertexDeclarationByteCount > 0))
		&& (!usesPixelProgram || pPixelShader != NULL);
}

void CDirectX12LegacyDrawState::SetTransform(
	INT transformState,
	const FLOAT* pMatrix)
{
	if (pMatrix == NULL)
		return;
	FLOAT* pDestination = NULL;
	if (transformState == D3DTS_WORLD)
		pDestination = world;
	else if (transformState == D3DTS_VIEW)
		pDestination = view;
	else if (transformState == D3DTS_PROJECTION)
		pDestination = projection;
	else if (transformState >= D3DTS_TEXTURE0
		&& transformState < D3DTS_TEXTURE0
			+ DX12_LEGACY_TEXTURE_STAGE_COUNT)
	{
		pDestination = textureTransform[
			transformState - D3DTS_TEXTURE0];
	}
	if (pDestination != NULL)
		CopyMemory(pDestination, pMatrix, sizeof(FLOAT) * 16);
}

void CDirectX12LegacyDrawState::SetViewport(
	DWORD x,
	DWORD y,
	DWORD width,
	DWORD height,
	FLOAT minimumDepth,
	FLOAT maximumDepth)
{
	viewport.x = x;
	viewport.y = y;
	viewport.width = width;
	viewport.height = height;
	viewport.minimumDepth = minimumDepth;
	viewport.maximumDepth = maximumDepth;
}

void CDirectX12LegacyDrawState::SetRenderState(INT state, DWORD value)
{
	switch (state)
	{
	case D3DRS_ZENABLE: zEnable = value; break;
	case D3DRS_ZWRITEENABLE: zWrite = value; break;
	case D3DRS_ZFUNC: zFunction = value; break;
	case D3DRS_CLIPPING: clipping = value; break;
	case D3DRS_CULLMODE: cullMode = value; break;
	case D3DRS_ALPHABLENDENABLE: blending = value; break;
	case D3DRS_ALPHATESTENABLE: alphaTest = value; break;
	case D3DRS_SRCBLEND: sourceBlend = value; break;
	case D3DRS_DESTBLEND: destinationBlend = value; break;
	case D3DRS_ALPHAREF: alphaReference = value; break;
	case D3DRS_COLORWRITEENABLE: colorWriteMask = value; break;
	case D3DRS_TEXTUREFACTOR: textureFactor = value; break;
	default: break;
	}
}

void CDirectX12LegacyDrawState::SetSamplerState(
	UINT sampler,
	INT state,
	DWORD value)
{
	if (sampler != 0)
		return;
	switch (state)
	{
	case D3DSAMP_ADDRESSU: samplerAddressU = value; break;
	case D3DSAMP_ADDRESSV: samplerAddressV = value; break;
	case D3DSAMP_MINFILTER: samplerMinification = value; break;
	case D3DSAMP_MAGFILTER: samplerMagnification = value; break;
	default: break;
	}
}

void CDirectX12LegacyDrawState::SetTextureStageState(
	UINT stage,
	INT state,
	DWORD value)
{
	if (stage >= DX12_LEGACY_TEXTURE_STAGE_COUNT)
		return;
	switch (state)
	{
	case D3DTSS_COLOROP: fixedColorOperation[stage] = value; break;
	case D3DTSS_COLORARG1: fixedColorArgument1[stage] = value; break;
	case D3DTSS_COLORARG2: fixedColorArgument2[stage] = value; break;
	case D3DTSS_ALPHAOP: fixedAlphaOperation[stage] = value; break;
	case D3DTSS_ALPHAARG1: fixedAlphaArgument1[stage] = value; break;
	case D3DTSS_ALPHAARG2: fixedAlphaArgument2[stage] = value; break;
	case D3DTSS_RESULTARG: fixedResultArgument[stage] = value; break;
	case D3DTSS_CONSTANT: fixedStageConstant[stage] = value; break;
	case D3DTSS_TEXCOORDINDEX: fixedTexCoordIndex[stage] = value; break;
	case D3DTSS_TEXTURETRANSFORMFLAGS:
		fixedTextureTransformFlags[stage] = value;
		break;
	case D3DTSS_BUMPENVMAT00:
	case D3DTSS_BUMPENVMAT01:
	case D3DTSS_BUMPENVMAT10:
	case D3DTSS_BUMPENVMAT11:
		if (stage == 1)
			bumpEnvironmentState[state - D3DTSS_BUMPENVMAT00] = value;
		break;
	default: break;
	}
}

void CDirectX12LegacyDrawState::SetVertexShader(
	IDirect3DVertexShader9* pShader,
	IDirect3DVertexDeclaration9* pDeclaration)
{
	if (pShader != NULL)
		pShader->AddRef();
	if (pVertexShader != NULL)
		pVertexShader->Release();
	pVertexShader = pShader;
	ZeroMemory(vertexDeclaration, sizeof(vertexDeclaration));
	vertexDeclarationByteCount = 0;
	if (pDeclaration == NULL)
		return;
	UINT elementCount = 0;
	if (SUCCEEDED(pDeclaration->GetDeclaration(NULL, &elementCount))
		&& elementCount > 0
		&& elementCount * sizeof(D3DVERTEXELEMENT9)
			<= sizeof(vertexDeclaration)
		&& SUCCEEDED(pDeclaration->GetDeclaration(
			reinterpret_cast<D3DVERTEXELEMENT9*>(vertexDeclaration),
			&elementCount)))
	{
		vertexDeclarationByteCount =
			elementCount * sizeof(D3DVERTEXELEMENT9);
	}
}

void CDirectX12LegacyDrawState::SetPixelShader(
	IDirect3DPixelShader9* pShader)
{
	if (pShader != NULL)
		pShader->AddRef();
	if (pPixelShader != NULL)
		pPixelShader->Release();
	pPixelShader = pShader;
}

void CDirectX12LegacyDrawState::SetVertexShaderConstants(
	UINT startRegister,
	const FLOAT* pConstants,
	UINT registerCount)
{
	if (pConstants == NULL
		|| startRegister >= DX12_LEGACY_VERTEX_CONSTANT_COUNT
		|| registerCount > DX12_LEGACY_VERTEX_CONSTANT_COUNT
			- startRegister)
		return;
	CopyMemory(
		vertexShaderConstants + startRegister * 4,
		pConstants,
		registerCount * 4 * sizeof(FLOAT));
}

void CDirectX12LegacyDrawState::SetPixelShaderConstants(
	UINT startRegister,
	const FLOAT* pConstants,
	UINT registerCount)
{
	if (pConstants == NULL
		|| startRegister >= DX12_LEGACY_PIXEL_CONSTANT_COUNT
		|| registerCount > DX12_LEGACY_PIXEL_CONSTANT_COUNT
			- startRegister)
		return;
	CopyMemory(
		pixelShaderConstants + startRegister * 4,
		pConstants,
		registerCount * 4 * sizeof(FLOAT));
}

void CDirectX12LegacyDrawState::SetTexture(
	UINT stage,
	IDirect3DTexture9* pTexture)
{
	if (stage >= DX12_LEGACY_TEXTURE_STAGE_COUNT)
		return;
	if (pTexture != NULL)
		pTexture->AddRef();
	if (textures[stage] != NULL)
		textures[stage]->Release();
	textures[stage] = pTexture;
	textureHandles[stage] = DirectX12TextureHandle();
	renderTextureHandles[stage] = DirectX12RenderTextureHandle();
}

void CDirectX12LegacyDrawState::SetTexture(
	UINT stage,
	DirectX12TextureHandle texture)
{
	if (stage >= DX12_LEGACY_TEXTURE_STAGE_COUNT)
		return;
	if (textures[stage] != NULL)
		textures[stage]->Release();
	textures[stage] = NULL;
	textureHandles[stage] = texture;
	renderTextureHandles[stage] = DirectX12RenderTextureHandle();
}

void CDirectX12LegacyDrawState::SetTexture(
	UINT stage,
	DirectX12RenderTextureHandle texture)
{
	if (stage >= DX12_LEGACY_TEXTURE_STAGE_COUNT)
		return;
	if (textures[stage] != NULL)
		textures[stage]->Release();
	textures[stage] = NULL;
	textureHandles[stage] = DirectX12TextureHandle();
	renderTextureHandles[stage] = texture;
}

void CDirectX12LegacyDrawState::ForgetTexture(
	DirectX12TextureHandle texture)
{
	if (!texture.IsValid())
		return;
	for (UINT stage = 0;
		stage < DX12_LEGACY_TEXTURE_STAGE_COUNT;
		++stage)
	{
		if (textureHandles[stage] == texture)
			textureHandles[stage] = DirectX12TextureHandle();
	}
}

void CDirectX12LegacyDrawState::ForgetTexture(
	DirectX12RenderTextureHandle texture)
{
	if (!texture.IsValid())
		return;
	for (UINT stage = 0;
		stage < DX12_LEGACY_TEXTURE_STAGE_COUNT;
		++stage)
	{
		if (renderTextureHandles[stage] == texture)
			renderTextureHandles[stage] =
				DirectX12RenderTextureHandle();
	}
}

void CDirectX12LegacyDrawState::SetDynamicGeometry(bool dynamic)
{
	dynamicGeometry = dynamic;
}

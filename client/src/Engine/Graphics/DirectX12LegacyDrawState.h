#ifndef SE_INCL_DIRECTX12LEGACYDRAWSTATE_H
#define SE_INCL_DIRECTX12LEGACYDRAWSTATE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <Engine/Graphics/DirectX12ResourceHandle.h>

struct IDirect3DVertexShader9;
struct IDirect3DPixelShader9;
struct IDirect3DVertexDeclaration9;

enum
{
	DX12_LEGACY_TEXTURE_STAGE_COUNT = 4,
	DX12_LEGACY_VERTEX_CONSTANT_COUNT = 96,
	DX12_LEGACY_PIXEL_CONSTANT_COUNT = 13
};

struct DirectX12LegacyViewportState
{
	DWORD x;
	DWORD y;
	DWORD width;
	DWORD height;
	FLOAT minimumDepth;
	FLOAT maximumDepth;
};

// Estado persistente alimentado por los setters del motor. QueueLegacy3DIndexedDraw
// lo consume sin volver a consultar el dispositivo D3D9 en cada draw.
class CDirectX12LegacyDrawState
{
public:
	CDirectX12LegacyDrawState();
	~CDirectX12LegacyDrawState();

	void Reset();
	bool IsValidForDraw(
		bool usesVertexProgram,
		bool usesPixelProgram) const;
	void SetTransform(INT transformState, const FLOAT* pMatrix);
	void SetViewport(
		DWORD x,
		DWORD y,
		DWORD width,
		DWORD height,
		FLOAT minimumDepth,
		FLOAT maximumDepth);
	void SetRenderState(INT state, DWORD value);
	void SetSamplerState(UINT sampler, INT state, DWORD value);
	void SetTextureStageState(UINT stage, INT state, DWORD value);
	void SetVertexShader(
		IDirect3DVertexShader9* pShader,
		IDirect3DVertexDeclaration9* pDeclaration);
	void SetPixelShader(IDirect3DPixelShader9* pShader);
	void SetVertexShaderConstants(
		UINT startRegister,
		const FLOAT* pConstants,
		UINT registerCount);
	void SetPixelShaderConstants(
		UINT startRegister,
		const FLOAT* pConstants,
		UINT registerCount);
	void SetTexture(UINT stage, DirectX12TextureHandle texture);
	void SetTexture(UINT stage, DirectX12RenderTextureHandle texture);
	void ForgetTexture(DirectX12TextureHandle texture);
	void ForgetTexture(DirectX12RenderTextureHandle texture);
	void SetDynamicGeometry(bool dynamic);

	FLOAT world[16];
	FLOAT view[16];
	FLOAT projection[16];
	FLOAT textureTransform[DX12_LEGACY_TEXTURE_STAGE_COUNT][16];
	DirectX12LegacyViewportState viewport;

	DWORD zEnable;
	DWORD zWrite;
	DWORD zFunction;
	DWORD clipping;
	DWORD cullMode;
	DWORD blending;
	DWORD alphaTest;
	DWORD sourceBlend;
	DWORD destinationBlend;
	DWORD alphaReference;
	DWORD colorWriteMask;
	DWORD textureFactor;
	DWORD samplerAddressU;
	DWORD samplerAddressV;
	DWORD samplerMinification;
	DWORD samplerMagnification;
	DWORD bumpEnvironmentState[4];
	DWORD fixedColorOperation[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedColorArgument1[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedColorArgument2[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedAlphaOperation[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedAlphaArgument1[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedAlphaArgument2[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedResultArgument[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedStageConstant[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedTexCoordIndex[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DWORD fixedTextureTransformFlags[DX12_LEGACY_TEXTURE_STAGE_COUNT];

	FLOAT vertexShaderConstants[
		DX12_LEGACY_VERTEX_CONSTANT_COUNT * 4];
	FLOAT pixelShaderConstants[
		DX12_LEGACY_PIXEL_CONSTANT_COUNT * 4];
	BYTE vertexDeclaration[256];
	UINT vertexDeclarationByteCount;
	DirectX12TextureHandle
		textureHandles[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	DirectX12RenderTextureHandle
		renderTextureHandles[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	IDirect3DVertexShader9* pVertexShader;
	IDirect3DPixelShader9* pPixelShader;
	bool dynamicGeometry;

private:
	CDirectX12LegacyDrawState(const CDirectX12LegacyDrawState&);
	CDirectX12LegacyDrawState& operator=(
		const CDirectX12LegacyDrawState&);
	void ReleaseBindings();
};

#endif

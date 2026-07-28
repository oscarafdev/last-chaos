#ifndef SE_INCL_DIRECTX12LEGACYDRAWSTATE_H
#define SE_INCL_DIRECTX12LEGACYDRAWSTATE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>

struct IDirect3DDevice9;
struct IDirect3DTexture9;
struct IDirect3DVertexShader9;
struct IDirect3DPixelShader9;

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

// Instantanea autocontenida de un draw D3D9. Esta es la unica estructura que
// cruza desde el adaptador de compatibilidad hacia el renderer DX12 nativo.
// Los objetos COM son aliases temporales y se liberan al destruir el snapshot.
class CDirectX12LegacyDrawState
{
public:
	CDirectX12LegacyDrawState();
	~CDirectX12LegacyDrawState();

	bool Capture(
		IDirect3DDevice9* pDevice9,
		bool usesVertexProgram,
		bool usesPixelProgram);

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
	IDirect3DTexture9* textures[DX12_LEGACY_TEXTURE_STAGE_COUNT];
	IDirect3DVertexShader9* pVertexShader;
	IDirect3DPixelShader9* pPixelShader;

private:
	CDirectX12LegacyDrawState(const CDirectX12LegacyDrawState&);
	CDirectX12LegacyDrawState& operator=(
		const CDirectX12LegacyDrawState&);
	void ReleaseBindings();
};

#endif

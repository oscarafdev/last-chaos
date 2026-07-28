#ifndef SE_INCL_DIRECTX12NATIVERENDERER_H
#define SE_INCL_DIRECTX12NATIVERENDERER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12RenderState.h>

struct IDirect3DTexture9;
class CDirectX12Buffer;
class CDirectX12BloomRenderer;
class CDirectX12DrawPortCommandBatch;
class CDirectX12InteropTextureManager;
class CDirectX12Legacy3DCommandBatch;
class CDirectX12LegacyDrawState;
class CDirectX12PipelineCache;
class CDirectX12RenderTargetManager;
class CDirectX12Texture;
class CDirectX12UploadManager;

// Primera ruta de dibujo nativa. Mantiene una pasada transparente mínima
// que valida buffers, textura, sampler, root signature y PSO en conjunto.
class CDirectX12NativeRenderer
{
public:
	CDirectX12NativeRenderer();
	~CDirectX12NativeRenderer();

	bool Initialize(ID3D12Device* pDevice);
	void Shutdown();
	void ForgetTexture(IDirect3DTexture9* pTexture);
	void BeginFrame(UINT frameIndex);
	bool BeginDrawPortScope(DirectX12DrawPortScope scope);
	bool EndDrawPortScope(DirectX12DrawPortScope scope);
	bool InsertDrawPortBarrier(DirectX12DrawPortBarrierKind kind);
	void SetLegacy3DVertexArray(
		const FLOAT* pPositions,
		UINT vertexCount);
	void SetLegacy3DTexCoordArray(
		UINT textureUnit,
		const FLOAT* pTexCoords,
		UINT vertexCount);
	void SetLegacy3DProjectiveTexCoordArray(
		UINT textureUnit,
		const FLOAT* pTexCoords,
		UINT vertexCount);
	void SetLegacy3DNormalArray(
		const FLOAT* pNormals,
		UINT vertexCount);
	void SetLegacy3DWeightArray(
		const BYTE* pWeights,
		UINT vertexCount);
	void SetLegacy3DTangentArray(
		const FLOAT* pTangents,
		UINT vertexCount);
	void SetLegacy3DColorArray(
		const ULONG* pColors,
		UINT vertexCount);
	void SetLegacy3DConstantColor(ULONG color);
	void SetLegacy3DStaticVertexArray(
		const FLOAT* pPositions,
		UINT vertexCount);
	void SetLegacy3DStaticTexCoordArray(
		UINT textureUnit,
		const FLOAT* pTexCoords,
		UINT vertexCount);
	void SetLegacy3DStaticNormalArray(
		const FLOAT* pNormals,
		UINT vertexCount);
	void SetLegacy3DStaticWeightArray(
		const BYTE* pWeights,
		UINT vertexCount);
	void SetLegacy3DStaticTangentArray(
		const FLOAT* pTangents,
		UINT vertexCount);
	void SetLegacy3DStaticD3DColorArray(
		const ULONG* pColors,
		UINT vertexCount);
	bool QueueLegacy3DIndexedDraw(
		const CDirectX12LegacyDrawState& drawState,
		const USHORT* pIndices,
		UINT indexCount,
		bool dynamicBuffer,
		bool usesVertexProgram,
		bool usesPixelProgram,
		bool usesColorArray,
		bool projectiveMapping,
		UINT texturePassCount,
		DirectX12LegacyRenderTargetKind renderTargetKind);

	bool QueueDrawPortPoint(
		FLOAT x,
		FLOAT y,
		FLOAT radius,
		ULONG color,
		LONG scissorLeft,
		LONG scissorTop,
		LONG scissorRight,
		LONG scissorBottom);
	bool QueueDrawPortLine(
		FLOAT x0,
		FLOAT y0,
		FLOAT x1,
		FLOAT y1,
		ULONG color,
		LONG scissorLeft,
		LONG scissorTop,
		LONG scissorRight,
		LONG scissorBottom);
	bool QueueDrawPortTriangle(
		FLOAT x0,
		FLOAT y0,
		FLOAT x1,
		FLOAT y1,
		FLOAT x2,
		FLOAT y2,
		ULONG color0,
		ULONG color1,
		ULONG color2,
		LONG scissorLeft,
		LONG scissorTop,
		LONG scissorRight,
		LONG scissorBottom);
	bool QueueDrawPortRectangle(
		FLOAT x0,
		FLOAT y0,
		FLOAT x1,
		FLOAT y1,
		ULONG colorUpperLeft,
		ULONG colorUpperRight,
		ULONG colorLowerLeft,
		ULONG colorLowerRight,
		LONG scissorLeft,
		LONG scissorTop,
		LONG scissorRight,
		LONG scissorBottom);
	bool QueueDrawPortTexturedTriangle(
		IDirect3DTexture9* pTexture,
		FLOAT x0, FLOAT y0, FLOAT u0, FLOAT v0, ULONG color0,
		FLOAT x1, FLOAT y1, FLOAT u1, FLOAT v1, ULONG color1,
		FLOAT x2, FLOAT y2, FLOAT u2, FLOAT v2, ULONG color2,
		LONG scissorLeft,
		LONG scissorTop,
		LONG scissorRight,
		LONG scissorBottom,
		DirectX12BlendMode blendMode,
		DirectX12SamplerMode samplerMode);

	bool RenderValidationPass(
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12RenderTargetManager* pRenderTargets,
		CDirectX12UploadManager* pUploadManager,
		CDirectX12DescriptorHeap* pResourceDescriptors,
		CDirectX12DescriptorHeap* pSamplerDescriptors,
		CDirectX12InteropTextureManager* pInteropTextures,
		DirectX12DrawPortValidationMode validationMode,
		UINT minimumSegment,
		UINT maximumSegment,
		bool submitLegacy3D);
	bool RenderBloom(
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12RenderTargetManager* pRenderTargets,
		CDirectX12UploadManager* pUploadManager,
		CDirectX12DescriptorHeap* pResourceDescriptors,
		CDirectX12DescriptorHeap* pSamplerDescriptors,
		CDirectX12Texture* pSourceTexture,
		CDirectX12Texture* pFilterTexture0,
		CDirectX12Texture* pFilterTexture1);
	UINT GetUiPrimitiveCount() const;
	UINT GetUiSegmentCount() const;
	UINT GetUiBarrierCount() const;
	UINT GetLegacy3DCapturedDrawCount() const;
	bool HasPendingLegacy3DDraws() const;
	UINT GetLegacy3DRejectedDrawCount() const;
	UINT GetLegacy3DCapturedTriangleCount() const;
	UINT GetLegacy3DRejectedReasonCount(UINT reason) const;
	UINT64 GetLegacy3DTopVertexShaderFingerprint() const;
	UINT GetLegacy3DTopVertexShaderDrawCount() const;
	UINT GetLegacy3DTopVertexShaderTriangleCount() const;
	bool IsLegacy3DOverlayComparisonEnabled() const;
	UINT GetCurrentSegment() const;
	bool HasUiCommands(UINT minimumSegment, UINT maximumSegment) const;

private:
	struct NativeVertex
	{
		FLOAT position[3];
		FLOAT texCoord[2];
	};

	CDirectX12NativeRenderer(const CDirectX12NativeRenderer&);
	CDirectX12NativeRenderer& operator=(const CDirectX12NativeRenderer&);

	bool EnsureResources(
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		CDirectX12DescriptorHeap* pResourceDescriptors,
		CDirectX12DescriptorHeap* pSamplerDescriptors);
	ID3D12Device* m_pDevice;
	CDirectX12PipelineCache* m_pPipelineCache;
	CDirectX12BloomRenderer* m_pBloomRenderer;
	CDirectX12DrawPortCommandBatch* m_pDrawPortCommands;
	CDirectX12Legacy3DCommandBatch* m_pLegacy3DCommands;
	CDirectX12Buffer* m_pVertexBuffer;
	CDirectX12Buffer* m_pIndexBuffer;
	CDirectX12Texture* m_pTexture;
	CDirectX12DescriptorHeap* m_pSamplerHeap;
	DirectX12DescriptorHandle m_samplers[DX12_SAMPLER_COUNT];
	bool m_initializationAttempted;
	bool m_resourcesReady;
};

#endif

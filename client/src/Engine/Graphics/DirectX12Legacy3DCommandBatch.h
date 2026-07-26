#ifndef SE_INCL_DIRECTX12LEGACY3DCOMMANDBATCH_H
#define SE_INCL_DIRECTX12LEGACY3DCOMMANDBATCH_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>
#include <Engine/Graphics/DirectX12RenderState.h>

struct IDirect3DDevice9;
struct IDirect3DTexture9;
class CDirectX12Buffer;
class CDirectX12DepthBuffer;
class CDirectX12InteropTextureManager;
class CDirectX12PipelineCache;
class CDirectX12RenderTargetManager;
class CDirectX12UploadManager;
struct DirectX12DescriptorHandle;
struct DirectX12Legacy3DCommandBatchState;

// Captura la primera familia acotada de geometria 3D dinamica del renderer
// legado y la reproduce mediante una lista de comandos D3D12 nativa.
class CDirectX12Legacy3DCommandBatch
{
public:
	enum RejectionReason
	{
		REJECT_NOT_DYNAMIC,
		REJECT_VERTEX_PROGRAM,
		REJECT_PIXEL_PROGRAM,
		REJECT_PROJECTIVE_MAPPING,
		REJECT_TEXTURE_PASS_COUNT,
		REJECT_MISSING_CPU_ARRAY,
		REJECT_CAPTURE_LIMIT,
		REJECT_INVALID_INDEX,
		REJECT_STATE_QUERY,
		REJECT_OFFSCREEN_RENDER_TARGET,
		REJECT_FIXED_TRANSPARENT_PASS,
		REJECT_FIXED_CLIP_VOLUME,
		REJECT_REASON_COUNT
	};

	CDirectX12Legacy3DCommandBatch();
	~CDirectX12Legacy3DCommandBatch();

	bool Initialize(
		ID3D12Device* pDevice,
		CDirectX12PipelineCache* pPipelineCache);
	void Shutdown();
	void BeginFrame();
	void ForgetTexture(IDirect3DTexture9* pTexture);

	void SetVertexArray(const FLOAT* pPositions, UINT vertexCount);
	void SetTexCoordArray(
		UINT textureUnit,
		const FLOAT* pTexCoords,
		UINT vertexCount);
	void SetNormalArray(const FLOAT* pNormals, UINT vertexCount);
	void SetWeightArray(const BYTE* pWeights, UINT vertexCount);
	void SetTangentArray(const FLOAT* pTangents, UINT vertexCount);
	void SetColorArray(const ULONG* pColors, UINT vertexCount);
	void SetStaticVertexArray(const FLOAT* pPositions, UINT vertexCount);
	void SetStaticTexCoordArray(
		UINT textureUnit,
		const FLOAT* pTexCoords,
		UINT vertexCount);
	void SetStaticNormalArray(const FLOAT* pNormals, UINT vertexCount);
	void SetStaticWeightArray(const BYTE* pWeights, UINT vertexCount);
	void SetStaticTangentArray(const FLOAT* pTangents, UINT vertexCount);
	void SetStaticD3DColorArray(const ULONG* pColors, UINT vertexCount);
	void SetConstantColor(ULONG color);
	bool QueueIndexedDraw(
		IDirect3DDevice9* pDevice9,
		const USHORT* pIndices,
		UINT indexCount,
		bool dynamicBuffer,
		bool usesVertexProgram,
		bool usesPixelProgram,
		bool usesColorArray,
		bool projectiveMapping,
		UINT texturePassCount,
		DirectX12LegacyRenderTargetKind renderTargetKind);

	bool RenderLegacy3DPass(
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12RenderTargetManager* pRenderTargets,
		CDirectX12UploadManager* pUploadManager,
		CDirectX12InteropTextureManager* pTextures,
		const DirectX12DescriptorHandle* pSamplers,
		D3D12_GPU_DESCRIPTOR_HANDLE fallbackTexture);

	UINT GetCapturedDrawCount() const;
	bool HasPendingDraws() const;
	UINT GetRejectedDrawCount() const;
	UINT GetCapturedTriangleCount() const;
	UINT GetRejectedReasonCount(RejectionReason reason) const;
	UINT64 GetTopVertexShaderFingerprint() const;
	UINT GetTopVertexShaderDrawCount() const;
	UINT GetTopVertexShaderTriangleCount() const;
	bool IsOverlayComparisonEnabled() const;

private:
	CDirectX12Legacy3DCommandBatch(
		const CDirectX12Legacy3DCommandBatch&);
	CDirectX12Legacy3DCommandBatch& operator=(
		const CDirectX12Legacy3DCommandBatch&);

	bool EnsureBuffers(UINT vertexBytes, UINT indexBytes);
	void SetVertexArrayInternal(
		const FLOAT* pPositions,
		UINT vertexCount,
		bool staticSource);
	void SetTexCoordArrayInternal(
		UINT textureUnit,
		const FLOAT* pTexCoords,
		UINT vertexCount,
		bool staticSource);
	void SetNormalArrayInternal(
		const FLOAT* pNormals,
		UINT vertexCount,
		bool staticSource);
	void SetWeightArrayInternal(
		const BYTE* pWeights,
		UINT vertexCount,
		bool staticSource);
	void SetTangentArrayInternal(
		const FLOAT* pTangents,
		UINT vertexCount,
		bool staticSource);

	ID3D12Device* m_pDevice;
	CDirectX12PipelineCache* m_pPipelineCache;
	CDirectX12Buffer* m_pVertexBuffer;
	CDirectX12Buffer* m_pIndexBuffer;
	CDirectX12DepthBuffer* m_pDepthBuffer;
	DirectX12Legacy3DCommandBatchState* m_pState;
};

#endif

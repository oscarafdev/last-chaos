#ifndef SE_INCL_DIRECTX12DRAWPORTCOMMANDBATCH_H
#define SE_INCL_DIRECTX12DRAWPORTCOMMANDBATCH_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>
#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12RenderState.h>

struct IDirect3DTexture9;
class CDirectX12Buffer;
class CDirectX12InteropTextureManager;
class CDirectX12PipelineCache;
class CDirectX12RenderTargetManager;
class CDirectX12UploadManager;
struct DirectX12DrawPortCommandBatchState;

// Agrupa todas las primitivas 2D manteniendo su orden, textura, mezcla,
// sampler y área de recorte para reproducir la interfaz mediante D3D12.
class CDirectX12DrawPortCommandBatch
{
public:
	enum { FRAME_COUNT = 3 };

	CDirectX12DrawPortCommandBatch();
	~CDirectX12DrawPortCommandBatch();

	bool Initialize(
		ID3D12Device* pDevice,
		CDirectX12PipelineCache* pPipelineCache);
	void Shutdown();
	void ForgetTexture(IDirect3DTexture9* pTexture);
	void BeginFrame(UINT frameIndex);
	bool BeginScope(DirectX12DrawPortScope scope);
	bool EndScope(DirectX12DrawPortScope scope);
	bool InsertBarrier(DirectX12DrawPortBarrierKind kind);

	bool QueuePoint(
		FLOAT x,
		FLOAT y,
		FLOAT radius,
		ULONG color,
		const D3D12_RECT& scissor);
	bool QueueLine(
		FLOAT x0,
		FLOAT y0,
		FLOAT x1,
		FLOAT y1,
		ULONG color,
		const D3D12_RECT& scissor);
	bool QueueSolidTriangle(
		FLOAT x0, FLOAT y0,
		FLOAT x1, FLOAT y1,
		FLOAT x2, FLOAT y2,
		ULONG color0, ULONG color1, ULONG color2,
		const D3D12_RECT& scissor);
	bool QueueRectangle(
		FLOAT x0, FLOAT y0,
		FLOAT x1, FLOAT y1,
		ULONG colorUpperLeft,
		ULONG colorUpperRight,
		ULONG colorLowerLeft,
		ULONG colorLowerRight,
		const D3D12_RECT& scissor);
	bool QueueTriangle(
		IDirect3DTexture9* pTexture,
		FLOAT x0, FLOAT y0, FLOAT u0, FLOAT v0, ULONG color0,
		FLOAT x1, FLOAT y1, FLOAT u1, FLOAT v1, ULONG color1,
		FLOAT x2, FLOAT y2, FLOAT u2, FLOAT v2, ULONG color2,
		const D3D12_RECT& scissor,
		DirectX12BlendMode blendMode,
		DirectX12SamplerMode samplerMode);
	bool QueueTriangle(
		DirectX12TextureHandle textureHandle,
		DirectX12RenderTextureHandle renderTextureHandle,
		FLOAT x0, FLOAT y0, FLOAT u0, FLOAT v0, ULONG color0,
		FLOAT x1, FLOAT y1, FLOAT u1, FLOAT v1, ULONG color1,
		FLOAT x2, FLOAT y2, FLOAT u2, FLOAT v2, ULONG color2,
		const D3D12_RECT& scissor,
		DirectX12BlendMode blendMode,
		DirectX12SamplerMode samplerMode);

	bool Render(
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12RenderTargetManager* pRenderTargets,
		CDirectX12UploadManager* pUploadManager,
		CDirectX12InteropTextureManager* pTextures,
		const DirectX12DescriptorHandle* pSamplers,
		D3D12_GPU_DESCRIPTOR_HANDLE solidTextureView,
		DirectX12DrawPortValidationMode validationMode,
		UINT minimumSegment,
		UINT maximumSegment);
	UINT GetPrimitiveCount() const;
	UINT GetUiPrimitiveCount() const;
	UINT GetUiSegmentCount() const;
	UINT GetUiBarrierCount() const;
	UINT GetCurrentSegment() const;
	bool HasUiCommands(UINT minimumSegment, UINT maximumSegment) const;

private:
	CDirectX12DrawPortCommandBatch(
		const CDirectX12DrawPortCommandBatch&);
	CDirectX12DrawPortCommandBatch& operator=(
		const CDirectX12DrawPortCommandBatch&);

	void ClearQueuedResources();

	ID3D12Device* m_pDevice;
	CDirectX12PipelineCache* m_pPipelineCache;
	DirectX12DrawPortCommandBatchState* m_pState;
	UINT m_currentFrame;
	bool m_frameActive;
};

#endif

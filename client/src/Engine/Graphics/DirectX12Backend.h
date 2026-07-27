#ifndef SE_INCL_DIRECTX12BACKEND_H
#define SE_INCL_DIRECTX12BACKEND_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <Engine/Graphics/DirectX12RenderState.h>

struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;
struct IDirect3DTexture9;
struct ID3D12Device;
struct ID3D12CommandQueue;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12Fence;
class CDirectX12RenderTargetManager;
class CDirectX12UploadManager;
class CDirectX12DescriptorHeap;
class CDirectX12NativeRenderer;
class CDirectX12InteropTextureManager;
class CDirectX12PresentationManager;
class CDirectX12Backend;

class CDirectX12DrawPortScope
{
public:
	CDirectX12DrawPortScope(
		CDirectX12Backend& backend,
		DirectX12DrawPortScope scope);
	~CDirectX12DrawPortScope();

private:
	CDirectX12DrawPortScope(const CDirectX12DrawPortScope&);
	CDirectX12DrawPortScope& operator=(const CDirectX12DrawPortScope&);

	CDirectX12Backend& m_backend;
	DirectX12DrawPortScope m_scope;
	bool m_active;
};

struct DirectX12DrawPortTexturedVertex
{
	FLOAT x;
	FLOAT y;
	FLOAT u;
	FLOAT v;
	ULONG color;
};

// Administra los objetos nativos de DirectX 12 usados por la capa D3D9On12.
// Mantener esta frontera separada de CGfxLibrary permite migrar gradualmente
// cada subsistema del renderer a D3D12 nativo.
class CDirectX12Backend
{
public:
	CDirectX12Backend();
	~CDirectX12Backend();

	bool Initialize(HMODULE hD3D9Module, IDirect3D9** ppD3D9);
	void Shutdown();

	bool BeginFrame();
	bool EndFrame();
	bool WaitForGpu();
	bool AttachD3D9Device(IDirect3DDevice9* pDevice9);
	void ForgetLegacyTexture(IDirect3DTexture9* pTexture9);
	bool CreateNativeOffscreenTexture(
		IDirect3DTexture9* pTexture9,
		UINT width,
		UINT height,
		INT legacyFormat,
		DirectX12RenderTextureHandle* pHandle);
	void DestroyNativeOffscreenTexture(
		DirectX12RenderTextureHandle handle);
	bool BeginNativeOffscreenTexture(
		DirectX12RenderTextureHandle handle);
	void ClearNativeOffscreenTexture(ULONG color);
	void EndNativeOffscreenTexture();
	bool CopyLegacySurfaceRegion(
		IDirect3DSurface9* pSource9,
		const RECT& sourceRect,
		IDirect3DSurface9* pDestination9,
		UINT destinationX,
		UINT destinationY);
	bool RenderNativeBloom(
		DirectX12RenderTextureHandle sourceTexture,
		DirectX12RenderTextureHandle filterTexture0,
		DirectX12RenderTextureHandle filterTexture1);
	bool AcquireRenderTarget(
		IDirect3DSurface9* pSurface9,
		HWND hPresentationWindow = NULL);
	void SetLegacyPresentationRenderTarget(IDirect3DSurface9* pSurface9);
	bool HasLegacyPresentationRenderTarget() const;
	bool BeginDrawPortScope(DirectX12DrawPortScope scope);
	bool EndDrawPortScope(DirectX12DrawPortScope scope);
	bool ClosePendingUiScope();
	bool InsertDrawPortBarrier(DirectX12DrawPortBarrierKind kind);
	void BeginOffscreenDrawPortScope();
	void EndOffscreenDrawPortScope();
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
	void PrepareLegacy3DDepthClear(IDirect3DDevice9* pDevice9);
	bool QueueLegacy3DIndexedDraw(
		IDirect3DDevice9* pDevice9,
		const USHORT* pIndices,
		UINT indexCount,
		bool dynamicBuffer,
		bool usesVertexProgram,
		bool usesPixelProgram,
		bool usesColorArray,
		bool projectiveMapping,
		UINT texturePassCount);
	bool ShouldSubmitLegacyDrawPort(bool nativeCaptured);
	bool ShouldSubmitLegacy3DDraw(bool nativeCaptured);
	bool QueueDrawPortPoint(
		FLOAT x, FLOAT y, FLOAT radius, ULONG color,
		LONG scissorLeft, LONG scissorTop,
		LONG scissorRight, LONG scissorBottom);
	bool QueueDrawPortLine(
		FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1, ULONG color,
		LONG scissorLeft, LONG scissorTop,
		LONG scissorRight, LONG scissorBottom);
	bool QueueDrawPortTriangle(
		FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1,
		FLOAT x2, FLOAT y2,
		ULONG color0, ULONG color1, ULONG color2,
		LONG scissorLeft, LONG scissorTop,
		LONG scissorRight, LONG scissorBottom);
	bool QueueDrawPortRectangle(
		FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1,
		ULONG colorUpperLeft, ULONG colorUpperRight,
		ULONG colorLowerLeft, ULONG colorLowerRight,
		LONG scissorLeft, LONG scissorTop,
		LONG scissorRight, LONG scissorBottom);
	bool QueueDrawPortTexturedTriangle(
		IDirect3DTexture9* pTexture,
		const DirectX12DrawPortTexturedVertex& vertex0,
		const DirectX12DrawPortTexturedVertex& vertex1,
		const DirectX12DrawPortTexturedVertex& vertex2,
		LONG scissorLeft, LONG scissorTop,
		LONG scissorRight, LONG scissorBottom,
		DirectX12BlendMode blendMode = DX12_BLEND_ALPHA,
		DirectX12SamplerMode samplerMode =
			DX12_SAMPLER_POINT_CLAMP);

	ID3D12Device* GetDevice() const;
	ID3D12CommandQueue* GetGraphicsQueue() const;
	ID3D12GraphicsCommandList* GetCommandList() const;
	CDirectX12RenderTargetManager* GetRenderTargetManager() const;
	CDirectX12UploadManager* GetUploadManager() const;
	CDirectX12DescriptorHeap* GetResourceDescriptorHeap() const;
	CDirectX12DescriptorHeap* GetSamplerDescriptorHeap() const;
	bool IsFrameOpen() const;
	bool ShouldBypassLegacyPresent() const;
	bool IsFull3DReplacementEnabled() const;
	bool RequiresLegacyOffscreenDepth() const;
	UINT GetUiPrimitiveCount() const;
	UINT GetUiSegmentCount() const;
	UINT GetUiBarrierCount() const;

private:
	enum
	{
		FRAME_COUNT = DX12_FRAME_COUNT,
		MAX_SUBMISSIONS_PER_FRAME = DX12_MAX_SUBMISSIONS_PER_FRAME
	};

	struct FrameContext
	{
		ID3D12CommandAllocator* apCommandAllocators[
			MAX_SUBMISSIONS_PER_FRAME];
		UINT64 fenceValue;
	};

	CDirectX12Backend(const CDirectX12Backend&);
	CDirectX12Backend& operator=(const CDirectX12Backend&);

	bool CreateFrameResources();
	void ReleaseFrameResources();
	bool WaitForFence(UINT64 fenceValue);
	bool SubmitUiSegmentsThrough(
		UINT maximumSegment,
		bool submitLegacy3D = false);
	bool SubmitPendingLegacy3DForCurrentTarget(
		const char* pTransition);
	bool AdvanceOpenCommandList();
	bool HasUiReadyForInitialPresentation() const;
	DirectX12LegacyRenderTargetKind ClassifyLegacyRenderTarget(
		IDirect3DDevice9* pDevice9) const;
	bool HasLegacy3DDepthSurface() const;

	HMODULE m_hD3D12Module;
	ID3D12Device* m_pDevice;
	ID3D12CommandQueue* m_pGraphicsQueue;
	FrameContext m_aFrames[FRAME_COUNT];
	ID3D12GraphicsCommandList* m_pCommandList;
	ID3D12Fence* m_pFence;
	CDirectX12RenderTargetManager* m_pRenderTargets;
	CDirectX12UploadManager* m_pUploadManager;
	CDirectX12DescriptorHeap* m_pResourceDescriptors;
	CDirectX12DescriptorHeap* m_pRenderTargetDescriptors;
	CDirectX12DescriptorHeap* m_pSamplerDescriptors;
	CDirectX12NativeRenderer* m_pNativeRenderer;
	CDirectX12InteropTextureManager* m_pInteropTextures;
	CDirectX12PresentationManager* m_pPresentation;
	IDirect3DDevice9* m_pDevice9;
	IUnknown* m_pLegacyPresentationTargetIdentity;
	HWND m_hPresentationWindow;
	HANDLE m_hFenceEvent;
	UINT64 m_nextFenceValue;
	UINT m_currentFrame;
	bool m_frameOpen;
	bool m_hasPresentedNativeUiFrame;
	bool m_initialPresentationDeferralReported;
	DirectX12DrawPortValidationMode m_drawPortValidationMode;
	UINT m_lastReportedUiPrimitiveCount;
	UINT m_lastReportedUiSegmentCount;
	UINT m_lastReportedUiBarrierCount;
	UINT m_nextUiSegmentToSubmit;
	UINT m_currentSubmission;
	bool m_partialSubmissionCapacityReported;
	UINT m_uiScopeDepth;
	UINT m_offscreenDrawPortDepth;
	DirectX12RenderTextureHandle m_nativeOffscreenTexture;
	bool m_nativeOffscreenClearPending;
	FLOAT m_nativeOffscreenClearColor[4];
	UINT m_suppressedLegacyDrawCount;
	UINT m_fallbackLegacyDrawCount;
	UINT m_lastReportedSuppressedLegacyDrawCount;
	UINT m_lastReportedFallbackLegacyDrawCount;
	bool m_legacy3DDepthAvailable;
	UINT m_suppressedLegacy3DDrawCount;
	UINT m_fallbackLegacy3DDrawCount;
	UINT m_lastReportedSuppressedLegacy3DDrawCount;
	UINT m_lastReportedFallbackLegacy3DDrawCount;
	UINT m_lastReportedLegacy3DCapturedDrawCount;
	UINT m_lastReportedLegacy3DRejectedDrawCount;
	UINT m_lastReportedLegacy3DTriangleCount;
	UINT m_lastReportedLegacy3DRejectionReasons[13];
	UINT64 m_lastReportedLegacy3DTopVertexShaderFingerprint;
	UINT m_lastReportedLegacy3DTopVertexShaderDrawCount;
	UINT m_lastReportedLegacy3DTopVertexShaderTriangleCount;
};

// Servicio compartido por el puente legado y los nuevos subsistemas D3D12.
CDirectX12Backend& GetDirectX12Backend();

#endif

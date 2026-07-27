#ifndef SE_INCL_DIRECTX12INTEROPTEXTUREMANAGER_H
#define SE_INCL_DIRECTX12INTEROPTEXTUREMANAGER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d9.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12RenderState.h>

struct IDirect3DDevice9;
struct IDirect3DTexture9;
class CDirectX12DescriptorHeap;
class CDirectX12SampledTextureCache;
class CDirectX12Texture;
class CDirectX12UploadManager;
struct DirectX12InteropTextureState;

// Mantiene las texturas D3D9 abiertas para D3D12 hasta que termina el frame
// que las utiliza y recicla sus descriptores cuando la GPU ya finalizó.
class CDirectX12InteropTextureManager
{
public:
	enum { FRAME_COUNT = 3 };

	CDirectX12InteropTextureManager();
	~CDirectX12InteropTextureManager();

	bool Initialize(
		ID3D12Device* pDevice,
		ID3D12CommandQueue* pGraphicsQueue,
		CDirectX12DescriptorHeap* pResourceDescriptors,
		CDirectX12DescriptorHeap* pRenderTargetDescriptors);
	void Shutdown();
	bool AttachD3D9Device(IDirect3DDevice9* pDevice9);
	bool BeginFrame(UINT frameIndex);
	void ForgetTexture(IDirect3DTexture9* pTexture9);
	void RetireLegacyTextureBinding(IDirect3DTexture9* pTexture9);
	bool CreateRenderTarget(
		IDirect3DTexture9* pTexture9,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		DirectX12RenderTextureHandle* pHandle);
	void DestroyRenderTarget(DirectX12RenderTextureHandle handle);
	CDirectX12Texture* FindRenderTarget(
		DirectX12RenderTextureHandle handle) const;
	CDirectX12Texture* FindRenderTarget(
		IDirect3DTexture9* pTexture9) const;
	bool ReferencesResource(
		IDirect3DTexture9* pTexture9,
		ID3D12Resource* pResource12) const;

	bool Acquire(
		IDirect3DTexture9* pTexture9,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);
	bool RefreshSampledTexture(
		IDirect3DTexture9* pTexture9,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager);
	bool PrepareForSubmission(ID3D12GraphicsCommandList* pCommandList);
	bool ReturnToD3D9(
		ID3D12Fence* pFence,
		UINT64 fenceValue,
		bool endFrame = true);

private:
	CDirectX12InteropTextureManager(
		const CDirectX12InteropTextureManager&);
	CDirectX12InteropTextureManager& operator=(
		const CDirectX12InteropTextureManager&);

	void ReleaseFrame(UINT frameIndex);
	void ReleaseRenderTargets();

	ID3D12Device* m_pDevice;
	ID3D12CommandQueue* m_pGraphicsQueue;
	struct IDirect3DDevice9On12* m_pDevice9On12;
	CDirectX12DescriptorHeap* m_pResourceDescriptors;
	CDirectX12DescriptorHeap* m_pRenderTargetDescriptors;
	CDirectX12SampledTextureCache* m_pSampledTextureCache;
	DirectX12InteropTextureState* m_pState;
	UINT m_currentFrame;
	bool m_frameActive;
	bool m_resourcesReturned;
};

#endif

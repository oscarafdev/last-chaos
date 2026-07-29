#ifndef SE_INCL_DIRECTX12INTEROPTEXTUREMANAGER_H
#define SE_INCL_DIRECTX12INTEROPTEXTUREMANAGER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <stddef.h>
#include <d3d9.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12RenderState.h>

class CDirectX12DescriptorHeap;
class CDirectX12SampledTextureCache;
class CDirectX12Texture;
class CDirectX12UploadManager;
struct DirectX12InteropTextureState;

// Gestiona texturas y render targets DX12 nativos durante el frame
// que las utiliza y recicla sus descriptores cuando la GPU ya finalizó.
class CDirectX12InteropTextureManager
{
public:
	enum { FRAME_COUNT = 3 };

	CDirectX12InteropTextureManager();
	~CDirectX12InteropTextureManager();

	bool Initialize(
		ID3D12Device* pDevice,
		CDirectX12DescriptorHeap* pResourceDescriptors,
		CDirectX12DescriptorHeap* pRenderTargetDescriptors);
	void Shutdown();
	bool ResetResources();
	bool BeginFrame(UINT frameIndex);
	bool EndFrame();
	bool CreateRenderTarget(
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		DirectX12RenderTextureHandle* pHandle);
	bool CreateSampledTexture(DirectX12TextureHandle* pHandle);
	void DestroySampledTexture(DirectX12TextureHandle handle);
	bool RefreshSampledTextureFromRgbaMipChain(
		DirectX12TextureHandle handle,
		const void* pPixels,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		UINT maximumMipCount,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		DirectX12TextureHandle* pNewHandle);
	bool RefreshSampledTextureFromCompressedBlob(
		DirectX12TextureHandle handle,
		const void* pBlob,
		size_t blobSize,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		UINT maximumMipCount,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		DirectX12TextureHandle* pNewHandle);
	void DestroyRenderTarget(DirectX12RenderTextureHandle handle);
	CDirectX12Texture* FindRenderTarget(
		DirectX12RenderTextureHandle handle) const;

	bool ReferencesResource(
		DirectX12RenderTextureHandle handle,
		ID3D12Resource* pResource12) const;

	bool Acquire(
		DirectX12TextureHandle handle,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);
	bool Acquire(
		DirectX12RenderTextureHandle handle,
		ID3D12GraphicsCommandList* pCommandList,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);
private:
	CDirectX12InteropTextureManager(
		const CDirectX12InteropTextureManager&);
	CDirectX12InteropTextureManager& operator=(
		const CDirectX12InteropTextureManager&);

	void ReleaseRenderTargets();

	ID3D12Device* m_pDevice;
	CDirectX12DescriptorHeap* m_pResourceDescriptors;
	CDirectX12DescriptorHeap* m_pRenderTargetDescriptors;
	CDirectX12SampledTextureCache* m_pSampledTextureCache;
	DirectX12InteropTextureState* m_pState;
	bool m_frameActive;
};

#endif

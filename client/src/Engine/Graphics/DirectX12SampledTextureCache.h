#ifndef SE_INCL_DIRECTX12SAMPLEDTEXTURECACHE_H
#define SE_INCL_DIRECTX12SAMPLEDTEXTURECACHE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <stddef.h>
#include <d3d9.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12RenderState.h>
#include <Engine/Graphics/DirectX12ResourceHandle.h>

class CDirectX12DescriptorHeap;
class CDirectX12TextureUploadSource;
class CDirectX12UploadManager;
struct DirectX12SampledTextureCacheState;

// Conserva recursos, uploads pendientes y SRV nativos para las texturas
// muestreadas por los draw calls DX12.
class CDirectX12SampledTextureCache
{
public:
	CDirectX12SampledTextureCache();
	~CDirectX12SampledTextureCache();

	bool Initialize(
		ID3D12Device* pDevice,
		CDirectX12DescriptorHeap* pResourceDescriptors);
	void Shutdown();
	void Clear();
	void BeginFrame(UINT frameIndex);

	bool CreateNative(DirectX12TextureHandle* pHandle);
	void DestroyNative(DirectX12TextureHandle handle);
	bool RefreshNativeFromRgbaMipChain(
		DirectX12TextureHandle handle,
		const void* pPixels,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		UINT maximumMipCount,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		DirectX12TextureHandle* pNewHandle);
	bool RefreshNativeFromCompressedBlob(
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
	bool Acquire(
		DirectX12TextureHandle handle,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);

private:
	enum CpuUploadKind
	{
		CPU_UPLOAD_RGBA,
		CPU_UPLOAD_COMPRESSED
	};

	CDirectX12SampledTextureCache(
		const CDirectX12SampledTextureCache&);
	CDirectX12SampledTextureCache& operator=(
		const CDirectX12SampledTextureCache&);

	bool Remove(DirectX12TextureHandle handle);
	bool ReplaceNative(
		DirectX12TextureHandle handle,
		const CDirectX12TextureUploadSource& source,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		CpuUploadKind cpuUploadKind,
		DirectX12TextureHandle* pNewHandle);
	void Retire(class CDirectX12Texture* pTexture);
	void ReleaseRetired(UINT frameIndex);

	ID3D12Device* m_pDevice;
	CDirectX12DescriptorHeap* m_pResourceDescriptors;
	DirectX12SampledTextureCacheState* m_pState;
	UINT m_currentFrame;
	bool m_hasFrame;
};

#endif

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

struct IDirect3DTexture9;
class CDirectX12DescriptorHeap;
class CDirectX12TextureUploadSource;
class CDirectX12UploadManager;
struct DirectX12SampledTextureCacheState;

// Conserva recursos y SRV nativos para las texturas muestreadas por los
// draw calls DX12. El objeto D3D9 es una clave no propietaria y una fuente
// transitoria mientras las cargas de assets sigan siendo legadas.
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
	void Forget(IDirect3DTexture9* pTexture9);
	void RetireLegacyBinding(IDirect3DTexture9* pTexture9);
	DirectX12TextureHandle FindHandle(IDirect3DTexture9* pTexture9) const;
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

	// Reemplaza anticipadamente la copia nativa después de un upload legado.
	// Si no puede hacerlo, Acquire conserva una ruta de creación bajo demanda.
	bool Refresh(
		IDirect3DTexture9* pTexture9,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager);
	bool RefreshFromRgbaMipChain(
		IDirect3DTexture9* pTexture9,
		const void* pPixels,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		UINT maximumMipCount,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager);
	bool RefreshFromCompressedBlob(
		IDirect3DTexture9* pTexture9,
		const void* pBlob,
		size_t blobSize,
		UINT width,
		UINT height,
		D3DFORMAT legacyFormat,
		UINT maximumMipCount,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager);
	bool Acquire(
		IDirect3DTexture9* pTexture9,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);
	bool Acquire(
		DirectX12TextureHandle handle,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);

private:
	enum CpuUploadKind
	{
		CPU_UPLOAD_NONE,
		CPU_UPLOAD_RGBA,
		CPU_UPLOAD_COMPRESSED
	};

	CDirectX12SampledTextureCache(
		const CDirectX12SampledTextureCache&);
	CDirectX12SampledTextureCache& operator=(
		const CDirectX12SampledTextureCache&);

	bool Replace(
		IDirect3DTexture9* pTexture9,
		const CDirectX12TextureUploadSource& source,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		CpuUploadKind cpuUploadKind,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);
	bool Remove(IDirect3DTexture9* pTexture9);
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

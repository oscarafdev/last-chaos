#ifndef SE_INCL_DIRECTX12SAMPLEDTEXTURECACHE_H
#define SE_INCL_DIRECTX12SAMPLEDTEXTURECACHE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12RenderState.h>

struct IDirect3DTexture9;
class CDirectX12DescriptorHeap;
class CDirectX12UploadManager;
struct DirectX12SampledTextureCacheState;

// Conserva recursos y SRV nativos para las texturas muestreadas por los
// draw calls DX12. El objeto D3D9 se usa solamente como identidad y como
// fuente transitoria mientras las cargas de assets sigan siendo legadas.
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

	// Reemplaza anticipadamente la copia nativa después de un upload legado.
	// Si no puede hacerlo, Acquire conserva una ruta de creación bajo demanda.
	bool Refresh(
		IDirect3DTexture9* pTexture9,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager);
	bool Acquire(
		IDirect3DTexture9* pTexture9,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		D3D12_GPU_DESCRIPTOR_HANDLE* pShaderResourceView);

private:
	CDirectX12SampledTextureCache(
		const CDirectX12SampledTextureCache&);
	CDirectX12SampledTextureCache& operator=(
		const CDirectX12SampledTextureCache&);

	bool Remove(IDirect3DTexture9* pTexture9);
	void Retire(class CDirectX12Texture* pTexture);
	void ReleaseRetired(UINT frameIndex);

	ID3D12Device* m_pDevice;
	CDirectX12DescriptorHeap* m_pResourceDescriptors;
	DirectX12SampledTextureCacheState* m_pState;
	UINT m_currentFrame;
	bool m_hasFrame;
};

#endif

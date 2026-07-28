#ifndef SE_INCL_DIRECTX12TEXTURE_H
#define SE_INCL_DIRECTX12TEXTURE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12ResourceHandle.h>

class CDirectX12UploadManager;
struct DirectX12SubresourceData;

// Representa una textura 2D en memoria de GPU y su vista SRV persistente.
class CDirectX12Texture
{
public:
	CDirectX12Texture();
	~CDirectX12Texture();

	bool Create2D(
		ID3D12Device* pDevice,
		CDirectX12DescriptorHeap* pDescriptorHeap,
		UINT width,
		UINT height,
		UINT16 mipLevels,
		DXGI_FORMAT format,
		UINT componentMapping =
			D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);
	bool Recreate2D(
		ID3D12Device* pDevice,
		CDirectX12DescriptorHeap* pDescriptorHeap,
		UINT width,
		UINT height,
		UINT16 mipLevels,
		DXGI_FORMAT format,
		UINT componentMapping =
			D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);
	bool CreateRenderTarget2D(
		ID3D12Device* pDevice,
		CDirectX12DescriptorHeap* pResourceDescriptorHeap,
		CDirectX12DescriptorHeap* pRenderTargetDescriptorHeap,
		UINT width,
		UINT height,
		DXGI_FORMAT format,
		UINT componentMapping =
			D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);
	void Shutdown();

	bool Upload(
		CDirectX12UploadManager* pUploadManager,
		ID3D12GraphicsCommandList* pCommandList,
		UINT firstMip,
		UINT mipCount,
		const DirectX12SubresourceData* pSubresources);
	void Transition(
		ID3D12GraphicsCommandList* pCommandList,
		D3D12_RESOURCE_STATES newState);

	ID3D12Resource* GetResource() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const;
	UINT GetDescriptorIndex() const;
	UINT GetWidth() const;
	UINT GetHeight() const;
	UINT16 GetMipLevels() const;
	DXGI_FORMAT GetFormat() const;
	D3D12_RESOURCE_STATES GetState() const;
	DirectX12TextureHandle GetTextureHandle() const;
	DirectX12RenderTextureHandle GetRenderTextureHandle() const;

private:
	CDirectX12Texture(const CDirectX12Texture&);
	CDirectX12Texture& operator=(const CDirectX12Texture&);
	bool Create2DResource(
		ID3D12Device* pDevice,
		CDirectX12DescriptorHeap* pDescriptorHeap,
		UINT width,
		UINT height,
		UINT16 mipLevels,
		DXGI_FORMAT format,
		UINT componentMapping,
		D3D12_RESOURCE_FLAGS flags,
		D3D12_RESOURCE_STATES initialState,
		const D3D12_CLEAR_VALUE* pClearValue,
		const wchar_t* pDebugName,
		DirectX12ResourceKind resourceKind,
		bool preserveIdentity);
	void ReleaseStorage();

	ID3D12Resource* m_pResource;
	CDirectX12DescriptorHeap* m_pDescriptorHeap;
	CDirectX12DescriptorHeap* m_pRenderTargetDescriptorHeap;
	DirectX12DescriptorHandle m_descriptor;
	DirectX12DescriptorHandle m_renderTargetDescriptor;
	UINT m_width;
	UINT m_height;
	UINT16 m_mipLevels;
	DXGI_FORMAT m_format;
	D3D12_RESOURCE_STATES m_state;
	DirectX12TextureHandle m_textureHandle;
	DirectX12RenderTextureHandle m_renderTextureHandle;
};

#endif

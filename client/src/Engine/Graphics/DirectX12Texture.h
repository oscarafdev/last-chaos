#ifndef SE_INCL_DIRECTX12TEXTURE_H
#define SE_INCL_DIRECTX12TEXTURE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12DescriptorHeap.h>

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
	void Shutdown();

	bool Upload(
		CDirectX12UploadManager* pUploadManager,
		ID3D12GraphicsCommandList* pCommandList,
		UINT firstMip,
		UINT mipCount,
		const DirectX12SubresourceData* pSubresources);

	ID3D12Resource* GetResource() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView() const;
	UINT GetDescriptorIndex() const;
	UINT GetWidth() const;
	UINT GetHeight() const;
	UINT16 GetMipLevels() const;
	DXGI_FORMAT GetFormat() const;
	D3D12_RESOURCE_STATES GetState() const;

private:
	CDirectX12Texture(const CDirectX12Texture&);
	CDirectX12Texture& operator=(const CDirectX12Texture&);

	ID3D12Resource* m_pResource;
	CDirectX12DescriptorHeap* m_pDescriptorHeap;
	DirectX12DescriptorHandle m_descriptor;
	UINT m_width;
	UINT m_height;
	UINT16 m_mipLevels;
	DXGI_FORMAT m_format;
	D3D12_RESOURCE_STATES m_state;
};

#endif

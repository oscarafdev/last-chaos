#ifndef SE_INCL_DIRECTX12DEPTHBUFFER_H
#define SE_INCL_DIRECTX12DEPTHBUFFER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

// Mantiene un depth buffer nativo compatible con el render target activo.
// Se recrea unicamente cuando cambian dimensiones o muestreo.
class CDirectX12DepthBuffer
{
public:
	CDirectX12DepthBuffer();
	~CDirectX12DepthBuffer();

	bool Initialize(ID3D12Device* pDevice);
	void Shutdown();
	bool EnsureCompatible(const D3D12_RESOURCE_DESC& renderTargetDesc);

	ID3D12Resource* GetResource() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetView() const;
	DXGI_FORMAT GetFormat() const;

private:
	CDirectX12DepthBuffer(const CDirectX12DepthBuffer&);
	CDirectX12DepthBuffer& operator=(const CDirectX12DepthBuffer&);

	ID3D12Device* m_pDevice;
	ID3D12DescriptorHeap* m_pDsvHeap;
	ID3D12Resource* m_pResource;
	UINT64 m_width;
	UINT m_height;
	DXGI_SAMPLE_DESC m_sampleDesc;
};

#endif

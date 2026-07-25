#ifndef SE_INCL_DIRECTX12DESCRIPTORHEAP_H
#define SE_INCL_DIRECTX12DESCRIPTORHEAP_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

struct DirectX12DescriptorHeapState;

struct DirectX12DescriptorHandle
{
	DirectX12DescriptorHandle();

	bool IsValid() const;

	UINT index;
	D3D12_CPU_DESCRIPTOR_HANDLE cpu;
	D3D12_GPU_DESCRIPTOR_HANDLE gpu;
};

// Administra un heap lineal con reciclado de índices para recursos o samplers.
// Los propietarios liberan sus descriptores después de finalizar su uso en GPU.
class CDirectX12DescriptorHeap
{
public:
	CDirectX12DescriptorHeap();
	~CDirectX12DescriptorHeap();

	bool Initialize(
		ID3D12Device* pDevice,
		D3D12_DESCRIPTOR_HEAP_TYPE type,
		UINT capacity,
		bool shaderVisible);
	void Shutdown();

	bool Allocate(DirectX12DescriptorHandle* pHandle);
	void Release(UINT index);

	ID3D12DescriptorHeap* GetHeap() const;
	D3D12_DESCRIPTOR_HEAP_TYPE GetType() const;
	UINT GetCapacity() const;
	UINT GetAllocatedCount() const;
	bool IsShaderVisible() const;

private:
	CDirectX12DescriptorHeap(const CDirectX12DescriptorHeap&);
	CDirectX12DescriptorHeap& operator=(const CDirectX12DescriptorHeap&);

	ID3D12DescriptorHeap* m_pHeap;
	DirectX12DescriptorHeapState* m_pState;
	D3D12_DESCRIPTOR_HEAP_TYPE m_type;
	UINT m_capacity;
	UINT m_descriptorSize;
	bool m_shaderVisible;
};

#endif

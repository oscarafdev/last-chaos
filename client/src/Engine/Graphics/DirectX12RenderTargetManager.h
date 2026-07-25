#ifndef SE_INCL_DIRECTX12RENDERTARGETMANAGER_H
#define SE_INCL_DIRECTX12RENDERTARGETMANAGER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

struct IDirect3DDevice9;
struct IDirect3DDevice9On12;
struct IDirect3DSurface9;

class CDirectX12RenderTargetManager
{
public:
	enum
	{
		FRAME_COUNT = 3,
		MAX_SUBMISSIONS_PER_FRAME = 16
	};

	CDirectX12RenderTargetManager();
	~CDirectX12RenderTargetManager();

	bool Initialize(ID3D12Device* pDevice, ID3D12CommandQueue* pGraphicsQueue);
	bool AttachD3D9Device(IDirect3DDevice9* pDevice9);
	void Shutdown();

	bool Acquire(
		IDirect3DSurface9* pSurface9,
		IDirect3DSurface9* pDepthSurface9,
		ID3D12GraphicsCommandList* pCommandList,
		UINT frameIndex,
		UINT submissionIndex);
	bool PrepareForSubmission(ID3D12GraphicsCommandList* pCommandList);
	bool ReturnToD3D9(ID3D12Fence* pFence, UINT64 fenceValue);

	bool IsAcquired() const;
	bool HasAcquiredDepth() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthView() const;
	ID3D12Resource* GetCurrentResource() const;
	ID3D12Resource* GetCurrentDepthResource() const;

private:
	CDirectX12RenderTargetManager(const CDirectX12RenderTargetManager&);
	CDirectX12RenderTargetManager& operator=(const CDirectX12RenderTargetManager&);

	void ReleaseAcquiredReferences();
	void Transition(
		ID3D12GraphicsCommandList* pCommandList,
		D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after);

	ID3D12Device* m_pDevice;
	ID3D12CommandQueue* m_pGraphicsQueue;
	IDirect3DDevice9On12* m_pDevice9On12;
	ID3D12DescriptorHeap* m_pRtvHeap;
	ID3D12DescriptorHeap* m_pDsvHeap;
	IDirect3DSurface9* m_pSurface9;
	IDirect3DSurface9* m_pDepthSurface9;
	ID3D12Resource* m_pResource12;
	ID3D12Resource* m_pDepthResource12;
	UINT m_rtvDescriptorSize;
	UINT m_dsvDescriptorSize;
	UINT m_currentFrame;
	UINT m_currentSubmission;
	bool m_isAcquired;
	bool m_isDepthAcquired;
};

#endif

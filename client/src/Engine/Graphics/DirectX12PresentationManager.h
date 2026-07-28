#ifndef SE_INCL_DIRECTX12PRESENTATIONMANAGER_H
#define SE_INCL_DIRECTX12PRESENTATIONMANAGER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <d3d12.h>

struct ID3D12CommandQueue;
struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;
struct ID3D12DescriptorHeap;
struct IDXGIFactory4;
struct IDXGISwapChain3;

class CDirectX12PresentationManager
{
public:
	enum
	{
		BACK_BUFFER_COUNT = 2
	};

	CDirectX12PresentationManager();
	~CDirectX12PresentationManager();

	bool Initialize(
		ID3D12Device* pDevice,
		ID3D12CommandQueue* pGraphicsQueue);
	void Shutdown();

	void Configure(HWND hWindow, UINT width, UINT height);
	bool RequiresRebuild() const;
	bool BeginFrame();
	bool AcquireCurrentTarget(
		ID3D12Resource** ppColorResource,
		D3D12_CPU_DESCRIPTOR_HANDLE* pColorView,
		ID3D12Resource** ppDepthResource,
		D3D12_CPU_DESCRIPTOR_HANDLE* pDepthView,
		D3D12_RESOURCE_STATES** ppColorState,
		bool* pClearTarget);
	bool QueuePresent(ID3D12GraphicsCommandList* pCommandList);
	bool Present();
	bool HasDepthTarget() const;

private:
	bool EnsureSwapChain(
		HWND hWindow,
		UINT width,
		UINT height);
	bool CreateTargetViews();
	void ReleaseSwapChain();
	void TransitionCurrentBackBuffer(
		ID3D12GraphicsCommandList* pCommandList,
		D3D12_RESOURCE_STATES after);

	ID3D12Device* m_pDevice;
	ID3D12CommandQueue* m_pGraphicsQueue;
	HMODULE m_hDxgiModule;
	IDXGIFactory4* m_pFactory;
	IDXGISwapChain3* m_pSwapChain;
	ID3D12DescriptorHeap* m_pRtvHeap;
	ID3D12DescriptorHeap* m_pDsvHeap;
	ID3D12Resource* m_apBackBuffers[BACK_BUFFER_COUNT];
	ID3D12Resource* m_pDepthBuffer;
	D3D12_RESOURCE_STATES m_aBackBufferStates[BACK_BUFFER_COUNT];
	UINT m_rtvDescriptorSize;
	HWND m_hWindow;
	UINT m_width;
	UINT m_height;
	HWND m_hRequestedWindow;
	UINT m_requestedWidth;
	UINT m_requestedHeight;
	bool m_frameQueued;
	bool m_clearTarget;
};

#endif

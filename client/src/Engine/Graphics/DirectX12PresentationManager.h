#ifndef SE_INCL_DIRECTX12PRESENTATIONMANAGER_H
#define SE_INCL_DIRECTX12PRESENTATIONMANAGER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <dxgiformat.h>

struct ID3D12CommandQueue;
struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;
struct IDXGIFactory4;
struct IDXGISwapChain3;

class CDirectX12PresentationManager
{
public:
	CDirectX12PresentationManager();
	~CDirectX12PresentationManager();

	bool Initialize(
		ID3D12Device* pDevice,
		ID3D12CommandQueue* pGraphicsQueue);
	void Shutdown();

	bool QueueFrame(
		ID3D12GraphicsCommandList* pCommandList,
		ID3D12Resource* pSource,
		HWND hWindow);
	bool Present();

private:
	bool EnsureSwapChain(
		HWND hWindow,
		UINT width,
		UINT height,
		DXGI_FORMAT sourceFormat);
	void ReleaseSwapChain();

	ID3D12Device* m_pDevice;
	ID3D12CommandQueue* m_pGraphicsQueue;
	HMODULE m_hDxgiModule;
	IDXGIFactory4* m_pFactory;
	IDXGISwapChain3* m_pSwapChain;
	HWND m_hWindow;
	UINT m_width;
	UINT m_height;
	bool m_frameQueued;
};

#endif

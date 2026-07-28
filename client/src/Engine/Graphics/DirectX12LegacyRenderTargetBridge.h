#ifndef SE_INCL_DIRECTX12LEGACYRENDERTARGETBRIDGE_H
#define SE_INCL_DIRECTX12LEGACYRENDERTARGETBRIDGE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>

struct ID3D12CommandQueue;
struct ID3D12Fence;
struct ID3D12Resource;
struct IDirect3DDevice9;
struct IDirect3DDevice9On12;
struct IDirect3DSurface9;

// Aisla la identidad y el ciclo de vida D3D9On12 de los render targets
// heredados. El resto del backend consume solamente recursos D3D12 prestados.
class CDirectX12LegacyRenderTargetBridge
{
public:
	CDirectX12LegacyRenderTargetBridge();
	~CDirectX12LegacyRenderTargetBridge();

	bool Initialize(ID3D12CommandQueue* pGraphicsQueue);
	bool AttachD3D9Device(IDirect3DDevice9* pDevice9);
	void Shutdown();

	bool Acquire(
		IDirect3DSurface9* pColorSurface,
		IDirect3DSurface9* pDepthSurface);
	bool ReturnToD3D9(ID3D12Fence* pFence, UINT64 fenceValue);
	void ReleaseImmediately();

	bool IsAcquired() const;
	bool HasDepth() const;
	ID3D12Resource* GetColorResource() const;
	ID3D12Resource* GetDepthResource() const;

private:
	CDirectX12LegacyRenderTargetBridge(
		const CDirectX12LegacyRenderTargetBridge&);
	CDirectX12LegacyRenderTargetBridge& operator=(
		const CDirectX12LegacyRenderTargetBridge&);

	void ReleaseReferences();

	ID3D12CommandQueue* m_pGraphicsQueue;
	IDirect3DDevice9On12* m_pDevice9On12;
	IDirect3DSurface9* m_pColorSurface;
	IDirect3DSurface9* m_pDepthSurface;
	ID3D12Resource* m_pColorResource;
	ID3D12Resource* m_pDepthResource;
};

#endif

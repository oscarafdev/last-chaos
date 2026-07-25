#ifndef SE_INCL_DIRECTX12UPLOADMANAGER_H
#define SE_INCL_DIRECTX12UPLOADMANAGER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

struct DirectX12UploadFramePool;

struct DirectX12SubresourceData
{
	const void* pData;
	LONG_PTR rowPitch;
	LONG_PTR slicePitch;
};

// Proporciona memoria temporal persistente por frame para copiar datos de CPU
// a recursos ubicados en el heap DEFAULT. Cada pool se reutiliza únicamente
// después de que el fence del frame correspondiente haya terminado.
class CDirectX12UploadManager
{
public:
	CDirectX12UploadManager();
	~CDirectX12UploadManager();

	bool Initialize(
		ID3D12Device* pDevice,
		UINT frameCount,
		UINT64 defaultPageSize = 4ULL * 1024ULL * 1024ULL);
	void Shutdown();

	bool BeginFrame(UINT frameIndex);
	void EndFrame();
	bool UploadBuffer(
		ID3D12GraphicsCommandList* pCommandList,
		ID3D12Resource* pDestination,
		UINT64 destinationOffset,
		const void* pData,
		UINT64 dataSize,
		D3D12_RESOURCE_STATES stateBefore,
		D3D12_RESOURCE_STATES stateAfter);
	bool UploadTexture(
		ID3D12GraphicsCommandList* pCommandList,
		ID3D12Resource* pDestination,
		UINT firstSubresource,
		UINT subresourceCount,
		const DirectX12SubresourceData* pSubresources,
		D3D12_RESOURCE_STATES stateBefore,
		D3D12_RESOURCE_STATES stateAfter);

private:
	CDirectX12UploadManager(const CDirectX12UploadManager&);
	CDirectX12UploadManager& operator=(const CDirectX12UploadManager&);

	bool Allocate(
		UINT64 size,
		ID3D12Resource** ppResource,
		UINT64* pOffset,
		void** ppCpuAddress);

	ID3D12Device* m_pDevice;
	DirectX12UploadFramePool* m_pFrames;
	UINT m_frameCount;
	UINT m_currentFrame;
	UINT64 m_defaultPageSize;
	bool m_frameActive;
};

#endif

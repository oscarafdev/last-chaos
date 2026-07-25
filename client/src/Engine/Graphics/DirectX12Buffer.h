#ifndef SE_INCL_DIRECTX12BUFFER_H
#define SE_INCL_DIRECTX12BUFFER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

class CDirectX12UploadManager;

// Encapsula un buffer ubicado en memoria de GPU y conserva su estado para
// permitir cargas iniciales y actualizaciones posteriores sin duplicar barreras.
class CDirectX12Buffer
{
public:
	CDirectX12Buffer();
	~CDirectX12Buffer();

	bool CreateVertexBuffer(
		ID3D12Device* pDevice,
		UINT64 size,
		UINT stride);
	bool CreateIndexBuffer(
		ID3D12Device* pDevice,
		UINT64 size,
		DXGI_FORMAT format);
	void Shutdown();

	bool Upload(
		CDirectX12UploadManager* pUploadManager,
		ID3D12GraphicsCommandList* pCommandList,
		const void* pData,
		UINT64 dataSize,
		UINT64 destinationOffset = 0);

	ID3D12Resource* GetResource() const;
	UINT64 GetSize() const;
	D3D12_RESOURCE_STATES GetState() const;
	D3D12_VERTEX_BUFFER_VIEW GetVertexView() const;
	D3D12_INDEX_BUFFER_VIEW GetIndexView() const;

private:
	enum BufferKind
	{
		BK_NONE,
		BK_VERTEX,
		BK_INDEX
	};

	CDirectX12Buffer(const CDirectX12Buffer&);
	CDirectX12Buffer& operator=(const CDirectX12Buffer&);

	bool Create(
		ID3D12Device* pDevice,
		UINT64 size,
		BufferKind kind,
		UINT stride,
		DXGI_FORMAT indexFormat);
	D3D12_RESOURCE_STATES GetUsageState() const;

	ID3D12Resource* m_pResource;
	UINT64 m_size;
	UINT m_stride;
	DXGI_FORMAT m_indexFormat;
	D3D12_RESOURCE_STATES m_state;
	BufferKind m_kind;
};

#endif

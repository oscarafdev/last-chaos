#ifndef SE_INCL_DIRECTX12RENDERTARGETMANAGER_H
#define SE_INCL_DIRECTX12RENDERTARGETMANAGER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>
#include <Engine/Graphics/DirectX12RenderState.h>

class CDirectX12Texture;

class CDirectX12RenderTargetManager
{
public:
	enum
	{
		FRAME_COUNT = DX12_FRAME_COUNT,
		MAX_SUBMISSIONS_PER_FRAME = DX12_MAX_SUBMISSIONS_PER_FRAME
	};

	CDirectX12RenderTargetManager();
	~CDirectX12RenderTargetManager();

	bool Initialize(ID3D12Device* pDevice);
	void Shutdown();

	bool AcquirePresentation(
		ID3D12Resource* pColorResource,
		D3D12_CPU_DESCRIPTOR_HANDLE colorView,
		ID3D12Resource* pDepthResource,
		D3D12_CPU_DESCRIPTOR_HANDLE depthView,
		D3D12_RESOURCE_STATES* pColorState,
		ID3D12GraphicsCommandList* pCommandList,
		bool clearTarget);
	bool AcquireNative(
		CDirectX12Texture* pTexture,
		ID3D12GraphicsCommandList* pCommandList,
		bool clearColor,
		const FLOAT clearValue[4]);
	bool CopyCurrentColorTo(
		CDirectX12Texture* pDestination,
		ID3D12GraphicsCommandList* pCommandList);
	bool PrepareForSubmission(ID3D12GraphicsCommandList* pCommandList);
	void ReleaseAfterSubmission();

	bool IsAcquired() const;
	bool HasAcquiredDepth() const;
	bool IsNativeRenderTarget() const;
	bool ShouldClearNativeDepth() const;
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
		D3D12_RESOURCE_STATES after);

	ID3D12Device* m_pDevice;
	ID3D12Resource* m_pResource12;
	ID3D12Resource* m_pDepthResource12;
	CDirectX12Texture* m_pNativeTexture;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentView;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentDepthView;
	D3D12_RESOURCE_STATES* m_pCurrentState;
	bool m_isAcquired;
	bool m_isDepthAcquired;
	bool m_isNative;
	bool m_clearNativeDepth;
};

#endif

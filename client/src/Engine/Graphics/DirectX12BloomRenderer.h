#ifndef SE_INCL_DIRECTX12BLOOMRENDERER_H
#define SE_INCL_DIRECTX12BLOOMRENDERER_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>

#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12PipelineCache.h>
#include <Engine/Graphics/DirectX12RenderState.h>

struct IDirect3DTexture9;
class CDirectX12Buffer;
class CDirectX12RenderTargetManager;
class CDirectX12Texture;

// Ejecuta el postproceso de bloom sin superficies ni draws de D3D9.
class CDirectX12BloomRenderer
{
public:
	CDirectX12BloomRenderer();
	~CDirectX12BloomRenderer();

	bool Initialize(CDirectX12PipelineCache* pPipelineCache);
	void Shutdown();
	bool Render(
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12RenderTargetManager* pRenderTargets,
		CDirectX12Texture* pSourceTexture,
		CDirectX12Texture* pFilterTexture0,
		CDirectX12Texture* pFilterTexture1,
		CDirectX12Buffer* pVertexBuffer,
		CDirectX12Buffer* pIndexBuffer,
		const DirectX12DescriptorHandle& linearClampSampler);

private:
	CDirectX12BloomRenderer(const CDirectX12BloomRenderer&);
	CDirectX12BloomRenderer& operator=(const CDirectX12BloomRenderer&);

	bool DrawPass(
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12Texture* pSource,
		D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView,
		const D3D12_RESOURCE_DESC& renderTargetDesc,
		DirectX12PipelineKind pipelineKind,
		DirectX12BlendMode blendMode,
		FLOAT directionX,
		FLOAT directionY,
		FLOAT intensity,
		FLOAT threshold,
		CDirectX12Buffer* pVertexBuffer,
		CDirectX12Buffer* pIndexBuffer,
		const DirectX12DescriptorHandle& linearClampSampler);

	CDirectX12PipelineCache* m_pPipelineCache;
};

#endif

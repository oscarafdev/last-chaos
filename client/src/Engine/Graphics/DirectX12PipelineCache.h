#ifndef SE_INCL_DIRECTX12PIPELINECACHE_H
#define SE_INCL_DIRECTX12PIPELINECACHE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <d3d12.h>
#include <Engine/Graphics/DirectX12RenderState.h>

struct DirectX12PipelineCacheState;

enum DirectX12PipelineKind
{
	DX12_PIPELINE_TEXTURED_VALIDATION,
	DX12_PIPELINE_TEXTURED_2D,
	DX12_PIPELINE_TEXTURED_ALPHA_TEST_2D,
	DX12_PIPELINE_BLOOM_DOWNSAMPLE,
	DX12_PIPELINE_BLOOM_BLUR,
	DX12_PIPELINE_BLOOM_COMPOSITE,
	DX12_PIPELINE_TEXTURED_3D_SHADOW,
	DX12_PIPELINE_TEXTURED_3D_OVERLAY,
	DX12_PIPELINE_RIGID_LIT_3D_SHADOW,
	DX12_PIPELINE_RIGID_LIT_3D_OVERLAY,
	DX12_PIPELINE_LEGACY_MATERIAL_3D_SHADOW,
	DX12_PIPELINE_LEGACY_MATERIAL_3D_OVERLAY
};

// Conserva la root signature y crea un PSO por formato de render target.
class CDirectX12PipelineCache
{
public:
	CDirectX12PipelineCache();
	~CDirectX12PipelineCache();

	bool Initialize(ID3D12Device* pDevice);
	void Shutdown();

	ID3D12RootSignature* GetRootSignature() const;
	ID3D12PipelineState* GetPipelineState(
		DirectX12PipelineKind kind,
		DXGI_FORMAT renderTargetFormat,
		DXGI_SAMPLE_DESC sampleDesc,
		DirectX12BlendMode blendMode = DX12_BLEND_ALPHA,
		bool depthEnabled = false,
		bool depthWriteEnabled = false,
		D3D12_COMPARISON_FUNC depthFunction =
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_NONE,
		DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT,
		bool depthClipEnabled = true);

private:
	CDirectX12PipelineCache(const CDirectX12PipelineCache&);
	CDirectX12PipelineCache& operator=(const CDirectX12PipelineCache&);

	bool CompileShaders();
	bool CreateRootSignature();
	ID3D12PipelineState* CreatePipelineState(
		DirectX12PipelineKind kind,
		DXGI_FORMAT renderTargetFormat,
		DXGI_SAMPLE_DESC sampleDesc,
		DirectX12BlendMode blendMode,
		bool depthEnabled,
		bool depthWriteEnabled,
		D3D12_COMPARISON_FUNC depthFunction,
		D3D12_CULL_MODE cullMode,
		DXGI_FORMAT depthStencilFormat,
		bool depthClipEnabled);

	ID3D12Device* m_pDevice;
	ID3D12RootSignature* m_pRootSignature;
	ID3DBlob* m_pVertexShader;
	ID3DBlob* m_pPixelShader;
	ID3DBlob* m_pTexturedVertexShader;
	ID3DBlob* m_pTexturedPixelShader;
	ID3DBlob* m_pAlphaTestPixelShader;
	ID3DBlob* m_pBloomVertexShader;
	ID3DBlob* m_pBloomDownsamplePixelShader;
	ID3DBlob* m_pBloomBlurPixelShader;
	ID3DBlob* m_pBloomCompositePixelShader;
	ID3DBlob* m_pLegacy3DVertexShader;
	ID3DBlob* m_pLegacy3DPixelShader;
	ID3DBlob* m_pRigidLit3DVertexShader;
	ID3DBlob* m_pRigidLit3DPixelShader;
	ID3DBlob* m_pLegacyMaterial3DVertexShader;
	ID3DBlob* m_pLegacyMaterial3DPixelShader;
	DirectX12PipelineCacheState* m_pState;
};

#endif

#include "stdh.h"

#include <d3d12.h>

#include <Engine/Graphics/DirectX12BloomRenderer.h>
#include <Engine/Graphics/DirectX12Buffer.h>
#include <Engine/Graphics/DirectX12InteropTextureManager.h>
#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12Texture.h>

CDirectX12BloomRenderer::CDirectX12BloomRenderer()
	: m_pDevice(NULL)
	, m_pPipelineCache(NULL)
	, m_pRtvHeap(NULL)
	, m_rtvDescriptorSize(0)
{
}

CDirectX12BloomRenderer::~CDirectX12BloomRenderer()
{
	Shutdown();
}

bool CDirectX12BloomRenderer::Initialize(
	ID3D12Device* pDevice,
	CDirectX12PipelineCache* pPipelineCache)
{
	if (pDevice == NULL || pPipelineCache == NULL)
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pPipelineCache = pPipelineCache;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 2;
	if (FAILED(m_pDevice->CreateDescriptorHeap(
			&heapDesc,
			__uuidof(ID3D12DescriptorHeap),
			reinterpret_cast<void**>(&m_pRtvHeap))))
	{
		Shutdown();
		return false;
	}
	m_rtvDescriptorSize =
		m_pDevice->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_pRtvHeap->SetName(L"LastChaos D3D12 Bloom RTV Heap");
	return true;
}

void CDirectX12BloomRenderer::Shutdown()
{
	if (m_pRtvHeap != NULL)
	{
		m_pRtvHeap->Release();
		m_pRtvHeap = NULL;
	}
	m_rtvDescriptorSize = 0;
	m_pPipelineCache = NULL;
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

bool CDirectX12BloomRenderer::DrawPass(
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
	const DirectX12DescriptorHandle& linearClampSampler)
{
	if (pCommandList == NULL || pSource == NULL
		|| pSource->GetResource() == NULL || pVertexBuffer == NULL
		|| pIndexBuffer == NULL || !linearClampSampler.IsValid())
		return false;

	ID3D12PipelineState* pPipelineState =
		m_pPipelineCache->GetPipelineState(
			pipelineKind,
			renderTargetDesc.Format,
			renderTargetDesc.SampleDesc,
			blendMode);
	if (pPipelineState == NULL)
		return false;

	const FLOAT bloomState[7] = {
		1.0f / static_cast<FLOAT>((std::max)(pSource->GetWidth(), 1U)),
		1.0f / static_cast<FLOAT>((std::max)(pSource->GetHeight(), 1U)),
		directionX,
		directionY,
		intensity,
		threshold,
		0.0f
	};
	const D3D12_VERTEX_BUFFER_VIEW vertexView =
		pVertexBuffer->GetVertexView();
	const D3D12_INDEX_BUFFER_VIEW indexView =
		pIndexBuffer->GetIndexView();
	D3D12_VIEWPORT viewport = {
		0.0f,
		0.0f,
		static_cast<FLOAT>(renderTargetDesc.Width),
		static_cast<FLOAT>(renderTargetDesc.Height),
		0.0f,
		1.0f
	};
	D3D12_RECT scissor = {
		0,
		0,
		static_cast<LONG>(renderTargetDesc.Width),
		static_cast<LONG>(renderTargetDesc.Height)
	};

	pCommandList->SetPipelineState(pPipelineState);
	pCommandList->SetGraphicsRootSignature(
		m_pPipelineCache->GetRootSignature());
	pCommandList->SetGraphicsRootDescriptorTable(
		0,
		pSource->GetShaderResourceView());
	pCommandList->SetGraphicsRootDescriptorTable(
		1,
		linearClampSampler.gpu);
	pCommandList->SetGraphicsRoot32BitConstants(
		4,
		sizeof(bloomState) / sizeof(bloomState[0]),
		bloomState,
		0);
	pCommandList->RSSetViewports(1, &viewport);
	pCommandList->RSSetScissorRects(1, &scissor);
	pCommandList->OMSetRenderTargets(
		1,
		&renderTargetView,
		FALSE,
		NULL);
	pCommandList->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexView);
	pCommandList->IASetIndexBuffer(&indexView);
	pCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	return true;
}

bool CDirectX12BloomRenderer::Render(
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12RenderTargetManager* pRenderTargets,
	CDirectX12InteropTextureManager* pInteropTextures,
	IDirect3DTexture9* pSourceTexture,
	IDirect3DTexture9* pFilterTexture0,
	IDirect3DTexture9* pFilterTexture1,
	CDirectX12Buffer* pVertexBuffer,
	CDirectX12Buffer* pIndexBuffer,
	const DirectX12DescriptorHandle& linearClampSampler)
{
	if (pCommandList == NULL || pRenderTargets == NULL
		|| !pRenderTargets->IsAcquired() || pInteropTextures == NULL
		|| m_pRtvHeap == NULL)
		return false;

	CDirectX12Texture* pSource =
		pInteropTextures->FindRenderTarget(pSourceTexture);
	CDirectX12Texture* pFilter0 =
		pInteropTextures->FindRenderTarget(pFilterTexture0);
	CDirectX12Texture* pFilter1 =
		pInteropTextures->FindRenderTarget(pFilterTexture1);
	ID3D12Resource* pMainTarget = pRenderTargets->GetCurrentResource();
	if (pSource == NULL || pFilter0 == NULL || pFilter1 == NULL
		|| pMainTarget == NULL
		|| pSource->GetWidth() != pMainTarget->GetDesc().Width
		|| pSource->GetHeight() != pMainTarget->GetDesc().Height
		|| pSource->GetFormat() != pMainTarget->GetDesc().Format)
		return false;

	D3D12_RESOURCE_BARRIER mainBarrier;
	ZeroMemory(&mainBarrier, sizeof(mainBarrier));
	mainBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	mainBarrier.Transition.pResource = pMainTarget;
	mainBarrier.Transition.Subresource =
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	mainBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	mainBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	pSource->Transition(pCommandList, D3D12_RESOURCE_STATE_COPY_DEST);
	pCommandList->ResourceBarrier(1, &mainBarrier);
	pCommandList->CopyResource(pSource->GetResource(), pMainTarget);
	mainBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	mainBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	pCommandList->ResourceBarrier(1, &mainBarrier);
	pSource->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	D3D12_CPU_DESCRIPTOR_HANDLE filterViews[2];
	filterViews[0] = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
	filterViews[1] = filterViews[0];
	filterViews[1].ptr += m_rtvDescriptorSize;
	m_pDevice->CreateRenderTargetView(
		pFilter0->GetResource(),
		NULL,
		filterViews[0]);
	m_pDevice->CreateRenderTargetView(
		pFilter1->GetResource(),
		NULL,
		filterViews[1]);
	const FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	pFilter0->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	pCommandList->ClearRenderTargetView(
		filterViews[0], clearColor, 0, NULL);
	bool succeeded = DrawPass(
		pCommandList,
		pSource,
		filterViews[0],
		pFilter0->GetResource()->GetDesc(),
		DX12_PIPELINE_BLOOM_DOWNSAMPLE,
		DX12_BLEND_OPAQUE,
		0.0f, 0.0f, 1.0f, 0.58f,
		pVertexBuffer,
		pIndexBuffer,
		linearClampSampler);

	pFilter0->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	pFilter1->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	pCommandList->ClearRenderTargetView(
		filterViews[1], clearColor, 0, NULL);
	succeeded = succeeded && DrawPass(
		pCommandList,
		pFilter0,
		filterViews[1],
		pFilter1->GetResource()->GetDesc(),
		DX12_PIPELINE_BLOOM_BLUR,
		DX12_BLEND_OPAQUE,
		1.0f, 0.0f, 1.0f, 0.0f,
		pVertexBuffer,
		pIndexBuffer,
		linearClampSampler);

	pFilter1->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	pFilter0->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	pCommandList->ClearRenderTargetView(
		filterViews[0], clearColor, 0, NULL);
	succeeded = succeeded && DrawPass(
		pCommandList,
		pFilter1,
		filterViews[0],
		pFilter0->GetResource()->GetDesc(),
		DX12_PIPELINE_BLOOM_BLUR,
		DX12_BLEND_OPAQUE,
		0.0f, 1.0f, 1.0f, 0.0f,
		pVertexBuffer,
		pIndexBuffer,
		linearClampSampler);

	pFilter0->Transition(
		pCommandList,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	return succeeded && DrawPass(
		pCommandList,
		pFilter0,
		pRenderTargets->GetCurrentView(),
		pMainTarget->GetDesc(),
		DX12_PIPELINE_BLOOM_COMPOSITE,
		DX12_BLEND_ADD,
		0.0f, 0.0f, 0.75f, 0.0f,
		pVertexBuffer,
		pIndexBuffer,
		linearClampSampler);
}

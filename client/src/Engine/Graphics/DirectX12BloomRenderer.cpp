#include "stdh.h"

#include <d3d12.h>

#include <Engine/Graphics/DirectX12BloomRenderer.h>
#include <Engine/Graphics/DirectX12Buffer.h>
#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12Texture.h>

CDirectX12BloomRenderer::CDirectX12BloomRenderer()
	: m_pPipelineCache(NULL)
{
}

CDirectX12BloomRenderer::~CDirectX12BloomRenderer()
{
	Shutdown();
}

bool CDirectX12BloomRenderer::Initialize(
	CDirectX12PipelineCache* pPipelineCache)
{
	if (pPipelineCache == NULL)
		return false;

	Shutdown();
	m_pPipelineCache = pPipelineCache;
	return true;
}

void CDirectX12BloomRenderer::Shutdown()
{
	m_pPipelineCache = NULL;
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
	CDirectX12Texture* pSource,
	CDirectX12Texture* pFilter0,
	CDirectX12Texture* pFilter1,
	CDirectX12Buffer* pVertexBuffer,
	CDirectX12Buffer* pIndexBuffer,
	const DirectX12DescriptorHandle& linearClampSampler)
{
	if (pCommandList == NULL || pRenderTargets == NULL
		|| !pRenderTargets->IsAcquired() || pSource == NULL
		|| pFilter0 == NULL || pFilter1 == NULL)
		return false;

	ID3D12Resource* pMainTarget = pRenderTargets->GetCurrentResource();
	if (pSource == NULL || pFilter0 == NULL || pFilter1 == NULL
		|| pMainTarget == NULL
		|| pSource->GetWidth() != pMainTarget->GetDesc().Width
		|| pSource->GetHeight() != pMainTarget->GetDesc().Height
		|| pSource->GetFormat() != pMainTarget->GetDesc().Format)
		return false;

	// La fuente ya no es una superficie D3D9. Captura el destino sincronizado
	// directamente en el recurso DX12 que conserva la identidad del bloom.
	if (!pRenderTargets->CopyCurrentColorTo(pSource, pCommandList))
		return false;

	D3D12_CPU_DESCRIPTOR_HANDLE filterViews[2];
	filterViews[0] = pFilter0->GetRenderTargetView();
	filterViews[1] = pFilter1->GetRenderTargetView();
	if (filterViews[0].ptr == 0 || filterViews[1].ptr == 0)
		return false;
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

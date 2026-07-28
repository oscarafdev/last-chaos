#include "stdh.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <d3d9.h>
#include <d3d12.h>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/DirectX12Buffer.h>
#include <Engine/Graphics/DirectX12DrawPortCommandBatch.h>
#include <Engine/Graphics/DirectX12InteropTextureManager.h>
#include <Engine/Graphics/DirectX12PipelineCache.h>
#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	struct TextureCpuVertex
	{
		FLOAT x;
		FLOAT y;
		FLOAT texCoord[2];
		FLOAT color[4];
	};

	struct TextureGpuVertex
	{
		FLOAT position[2];
		FLOAT texCoord[2];
		FLOAT color[4];
	};

	struct TextureBatchRange
	{
		UINT firstVertex;
		UINT vertexCount;
		IDirect3DTexture9* pTexture;
		DirectX12TextureHandle textureHandle;
		DirectX12RenderTextureHandle renderTextureHandle;
		D3D12_RECT scissor;
		DirectX12BlendMode blendMode;
		DirectX12SamplerMode samplerMode;
		DirectX12DrawPortScope scope;
		UINT segment;
	};

	struct DrawPortBarrier
	{
		UINT segmentAfter;
		DirectX12DrawPortBarrierKind kind;
	};

	bool SameScissor(
		const D3D12_RECT& left,
		const D3D12_RECT& right)
	{
		return left.left == right.left && left.top == right.top
			&& left.right == right.right
			&& left.bottom == right.bottom;
	}

	TextureCpuVertex MakeVertex(
		FLOAT x,
		FLOAT y,
		FLOAT u,
		FLOAT v,
		ULONG color)
	{
		TextureCpuVertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.texCoord[0] = u;
		vertex.texCoord[1] = v;
		vertex.color[0] =
			static_cast<FLOAT>((color & CT_RMASK) >> CT_RSHIFT) / 255.0f;
		vertex.color[1] =
			static_cast<FLOAT>((color & CT_GMASK) >> CT_GSHIFT) / 255.0f;
		vertex.color[2] =
			static_cast<FLOAT>((color & CT_BMASK) >> CT_BSHIFT) / 255.0f;
		vertex.color[3] =
			static_cast<FLOAT>((color & CT_AMASK) >> CT_ASHIFT) / 255.0f;
		return vertex;
	}

	void ReportBatchFailureOnce(const char* pStage)
	{
		static const char* pLastStage = NULL;
		if (pStage == NULL || pStage == pLastStage)
			return;
		CPrintF("DX12 error en lote DrawPort: %s.\n", pStage);
		pLastStage = pStage;
	}

	bool ReadUiDiagnosticsMode()
	{
		char value[16] = { 0 };
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_UI_DIAGNOSTICS",
			value,
			sizeof(value));
		if (length == 0 || length >= sizeof(value))
			return false;
		return _stricmp(value, "1") == 0
			|| _stricmp(value, "enabled") == 0;
	}

}

struct DirectX12DrawPortCommandBatchState
{
	std::vector<TextureCpuVertex> vertices;
	std::vector<TextureGpuVertex> gpuVertices;
	std::vector<TextureBatchRange> ranges;
	std::vector<DirectX12DrawPortScope> scopes;
	std::vector<DrawPortBarrier> barriers;
	std::vector<CDirectX12Buffer*> vertexBuffers[
		CDirectX12DrawPortCommandBatch::FRAME_COUNT];
	UINT primitiveCount;
	UINT uiPrimitiveCount;
	UINT currentSegment;

	DirectX12DrawPortCommandBatchState()
		: primitiveCount(0)
		, uiPrimitiveCount(0)
		, currentSegment(0)
	{
	}
};

CDirectX12DrawPortCommandBatch::CDirectX12DrawPortCommandBatch()
	: m_pDevice(NULL)
	, m_pPipelineCache(NULL)
	, m_pState(NULL)
	, m_currentFrame(0)
	, m_frameActive(false)
{
}

CDirectX12DrawPortCommandBatch::~CDirectX12DrawPortCommandBatch()
{
	Shutdown();
}

bool CDirectX12DrawPortCommandBatch::Initialize(
	ID3D12Device* pDevice,
	CDirectX12PipelineCache* pPipelineCache)
{
	if (pDevice == NULL || pPipelineCache == NULL)
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pPipelineCache = pPipelineCache;
	m_pState = new DirectX12DrawPortCommandBatchState;
	if (m_pState == NULL)
	{
		Shutdown();
		return false;
	}
	return true;
}

void CDirectX12DrawPortCommandBatch::Shutdown()
{
	ClearQueuedResources();
	if (m_pState != NULL)
	{
		for (UINT iFrame = 0; iFrame < FRAME_COUNT; ++iFrame)
		{
			std::vector<CDirectX12Buffer*>& buffers =
				m_pState->vertexBuffers[iFrame];
			for (size_t iBuffer = 0;
				iBuffer < buffers.size();
				++iBuffer)
				delete buffers[iBuffer];
			buffers.clear();
		}
	}
	delete m_pState;
	m_pState = NULL;
	m_pPipelineCache = NULL;
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	m_currentFrame = 0;
	m_frameActive = false;
}

void CDirectX12DrawPortCommandBatch::ForgetTexture(
	IDirect3DTexture9* pTexture)
{
	if (m_pState == NULL || pTexture == NULL)
		return;
	for (size_t iRange = 0; iRange < m_pState->ranges.size(); ++iRange)
	{
		TextureBatchRange& range = m_pState->ranges[iRange];
		if (range.pTexture == pTexture)
		{
			range.pTexture->Release();
			range.pTexture = NULL;
		}
	}
}

void CDirectX12DrawPortCommandBatch::BeginFrame(UINT frameIndex)
{
	ClearQueuedResources();
	if (m_pState == NULL || frameIndex >= FRAME_COUNT)
	{
		m_frameActive = false;
		return;
	}
	m_currentFrame = frameIndex;
	std::vector<CDirectX12Buffer*>& buffers =
		m_pState->vertexBuffers[frameIndex];
	for (size_t iBuffer = 0; iBuffer < buffers.size(); ++iBuffer)
		delete buffers[iBuffer];
	buffers.clear();
	m_frameActive = true;
	m_pState->scopes.push_back(DX12_DRAWPORT_SCOPE_DEFAULT);
}

bool CDirectX12DrawPortCommandBatch::BeginScope(
	DirectX12DrawPortScope scope)
{
	if (!m_frameActive || m_pState == NULL)
		return false;
	m_pState->scopes.push_back(scope);
	return true;
}

bool CDirectX12DrawPortCommandBatch::EndScope(
	DirectX12DrawPortScope scope)
{
	if (!m_frameActive || m_pState == NULL
		|| m_pState->scopes.size() <= 1
		|| m_pState->scopes.back() != scope)
		return false;
	m_pState->scopes.pop_back();
	return true;
}

bool CDirectX12DrawPortCommandBatch::InsertBarrier(
	DirectX12DrawPortBarrierKind kind)
{
	if (!m_frameActive || m_pState == NULL
		|| m_pState->scopes.empty()
		|| m_pState->scopes.back() != DX12_DRAWPORT_SCOPE_UI)
		return false;

	++m_pState->currentSegment;
	DrawPortBarrier barrier;
	barrier.segmentAfter = m_pState->currentSegment;
	barrier.kind = kind;
	m_pState->barriers.push_back(barrier);
	return true;
}

bool CDirectX12DrawPortCommandBatch::QueuePoint(
	FLOAT x,
	FLOAT y,
	FLOAT radius,
	ULONG color,
	const D3D12_RECT& scissor)
{
	if (!m_frameActive || radius <= 0.0f)
		return false;
	const FLOAT halfSize = radius * 0.5f;
	return QueueRectangle(
		x - halfSize,
		y - halfSize,
		x + halfSize,
		y + halfSize,
		color,
		color,
		color,
		color,
		scissor);
}

bool CDirectX12DrawPortCommandBatch::QueueLine(
	FLOAT x0,
	FLOAT y0,
	FLOAT x1,
	FLOAT y1,
	ULONG color,
	const D3D12_RECT& scissor)
{
	if (!m_frameActive)
		return false;

	const FLOAT deltaX = x1 - x0;
	const FLOAT deltaY = y1 - y0;
	const FLOAT length = sqrtf(deltaX * deltaX + deltaY * deltaY);
	if (length <= 0.0001f)
		return QueuePoint(x0, y0, 1.0f, color, scissor);

	const FLOAT offsetX = -deltaY * 0.5f / length;
	const FLOAT offsetY = deltaX * 0.5f / length;
	return QueueSolidTriangle(
		x0 + offsetX, y0 + offsetY,
		x1 + offsetX, y1 + offsetY,
		x0 - offsetX, y0 - offsetY,
		color, color, color, scissor)
		&& QueueSolidTriangle(
			x0 - offsetX, y0 - offsetY,
			x1 + offsetX, y1 + offsetY,
			x1 - offsetX, y1 - offsetY,
			color, color, color, scissor);
}

bool CDirectX12DrawPortCommandBatch::QueueSolidTriangle(
	FLOAT x0,
	FLOAT y0,
	FLOAT x1,
	FLOAT y1,
	FLOAT x2,
	FLOAT y2,
	ULONG color0,
	ULONG color1,
	ULONG color2,
	const D3D12_RECT& scissor)
{
	return QueueTriangle(
		NULL,
		x0, y0, 0.0f, 0.0f, color0,
		x1, y1, 0.0f, 0.0f, color1,
		x2, y2, 0.0f, 0.0f, color2,
		scissor,
		DX12_BLEND_ALPHA,
		DX12_SAMPLER_POINT_CLAMP);
}

bool CDirectX12DrawPortCommandBatch::QueueRectangle(
	FLOAT x0,
	FLOAT y0,
	FLOAT x1,
	FLOAT y1,
	ULONG colorUpperLeft,
	ULONG colorUpperRight,
	ULONG colorLowerLeft,
	ULONG colorLowerRight,
	const D3D12_RECT& scissor)
{
	if (!m_frameActive || x0 == x1 || y0 == y1)
		return false;
	return QueueSolidTriangle(
		x0, y0,
		x0, y1,
		x1, y1,
		colorUpperLeft,
		colorLowerLeft,
		colorLowerRight,
		scissor)
		&& QueueSolidTriangle(
			x0, y0,
			x1, y1,
			x1, y0,
			colorUpperLeft,
			colorLowerRight,
			colorUpperRight,
			scissor);
}

bool CDirectX12DrawPortCommandBatch::QueueTriangle(
	IDirect3DTexture9* pTexture,
	FLOAT x0, FLOAT y0, FLOAT u0, FLOAT v0, ULONG color0,
	FLOAT x1, FLOAT y1, FLOAT u1, FLOAT v1, ULONG color1,
	FLOAT x2, FLOAT y2, FLOAT u2, FLOAT v2, ULONG color2,
	const D3D12_RECT& scissor,
	DirectX12BlendMode blendMode,
	DirectX12SamplerMode samplerMode)
{
	if (!m_frameActive
		|| m_pState->scopes.empty()
		|| samplerMode < 0 || samplerMode >= DX12_SAMPLER_COUNT)
		return false;
	const DirectX12DrawPortScope scope = m_pState->scopes.back();

	const UINT firstVertex =
		static_cast<UINT>(m_pState->vertices.size());
	const TextureCpuVertex vertices[] = {
		MakeVertex(x0, y0, u0, v0, color0),
		MakeVertex(x1, y1, u1, v1, color1),
		MakeVertex(x2, y2, u2, v2, color2)
	};
	m_pState->vertices.insert(
		m_pState->vertices.end(),
		vertices,
		vertices + 3);
	const DirectX12TextureHandle textureHandle =
		GetDirectX12ResourceRegistry().
			ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(pTexture);
	const DirectX12RenderTextureHandle renderTextureHandle =
		GetDirectX12ResourceRegistry().
			ResolveLegacyAlias<DX12_RESOURCE_RENDER_TEXTURE>(pTexture);

	if (!m_pState->ranges.empty()
		&& m_pState->ranges.back().pTexture == pTexture
		&& m_pState->ranges.back().textureHandle == textureHandle
		&& m_pState->ranges.back().renderTextureHandle
			== renderTextureHandle
		&& SameScissor(m_pState->ranges.back().scissor, scissor)
		&& m_pState->ranges.back().blendMode == blendMode
		&& m_pState->ranges.back().samplerMode == samplerMode
		&& m_pState->ranges.back().scope == scope
		&& m_pState->ranges.back().segment == m_pState->currentSegment)
	{
		m_pState->ranges.back().vertexCount += 3;
	}
	else
	{
		TextureBatchRange range;
		range.firstVertex = firstVertex;
		range.vertexCount = 3;
		range.pTexture = pTexture;
		range.textureHandle = textureHandle;
		range.renderTextureHandle = renderTextureHandle;
		if (range.pTexture != NULL)
			range.pTexture->AddRef();
		range.scissor = scissor;
		range.blendMode = blendMode;
		range.samplerMode = samplerMode;
		range.scope = scope;
		range.segment = m_pState->currentSegment;
		m_pState->ranges.push_back(range);
	}
	++m_pState->primitiveCount;
	if (scope == DX12_DRAWPORT_SCOPE_UI)
		++m_pState->uiPrimitiveCount;
	return true;
}

bool CDirectX12DrawPortCommandBatch::Render(
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12RenderTargetManager* pRenderTargets,
	CDirectX12UploadManager* pUploadManager,
	CDirectX12InteropTextureManager* pTextures,
	const DirectX12DescriptorHandle* pSamplers,
	D3D12_GPU_DESCRIPTOR_HANDLE solidTextureView,
	DirectX12DrawPortValidationMode validationMode,
	UINT minimumSegment,
	UINT maximumSegment)
{
	if (!m_frameActive || pCommandList == NULL || pRenderTargets == NULL
		|| pUploadManager == NULL || pTextures == NULL
		|| pSamplers == NULL || solidTextureView.ptr == 0
		|| !pRenderTargets->IsAcquired())
	{
		ReportBatchFailureOnce("estado de frame o dependencias invalidas");
		return false;
	}
	if (m_pState->vertices.empty())
		return true;
	if (minimumSegment > maximumSegment)
	{
		ReportBatchFailureOnce("rango de segmentos invalido");
		return false;
	}

	bool hasSelectedCommands = false;
	for (size_t iRange = 0; iRange < m_pState->ranges.size(); ++iRange)
	{
		const TextureBatchRange& range = m_pState->ranges[iRange];
		if (range.segment >= minimumSegment
			&& range.segment <= maximumSegment)
		{
			hasSelectedCommands = true;
			break;
		}
	}
	if (!hasSelectedCommands)
		return true;

	const D3D12_RESOURCE_DESC targetDesc =
		pRenderTargets->GetCurrentResource()->GetDesc();
	if (targetDesc.Width == 0 || targetDesc.Height == 0)
	{
		ReportBatchFailureOnce("dimensiones de destino invalidas");
		return false;
	}
	m_pState->gpuVertices.resize(m_pState->vertices.size());
	for (size_t iVertex = 0;
		iVertex < m_pState->vertices.size();
		++iVertex)
	{
		const TextureCpuVertex& source = m_pState->vertices[iVertex];
		TextureGpuVertex& destination = m_pState->gpuVertices[iVertex];
		destination.position[0] =
			source.x * 2.0f / static_cast<FLOAT>(targetDesc.Width) - 1.0f;
		destination.position[1] =
			1.0f - source.y * 2.0f / static_cast<FLOAT>(targetDesc.Height);
		destination.texCoord[0] = source.texCoord[0];
		destination.texCoord[1] = source.texCoord[1];
		for (UINT iComponent = 0; iComponent < 4; ++iComponent)
			destination.color[iComponent] = source.color[iComponent];
		destination.color[3] = 0.0f;
	}

	const bool showUi =
		validationMode == DX12_DRAWPORT_VALIDATION_UI_OVERLAY
		|| validationMode == DX12_DRAWPORT_VALIDATION_UI_SPLIT
		|| validationMode == DX12_DRAWPORT_VALIDATION_UI_REPLACE;
	if (showUi)
	{
		for (size_t iRange = 0; iRange < m_pState->ranges.size(); ++iRange)
		{
			const TextureBatchRange& range = m_pState->ranges[iRange];
			if (range.scope != DX12_DRAWPORT_SCOPE_UI)
				continue;
			const UINT lastVertex = range.firstVertex + range.vertexCount;
			for (UINT iVertex = range.firstVertex;
				iVertex < lastVertex;
				++iVertex)
			{
				m_pState->gpuVertices[iVertex].color[3] =
					m_pState->vertices[iVertex].color[3];
			}
		}
	}

	const UINT64 uploadSize =
		m_pState->gpuVertices.size() * sizeof(TextureGpuVertex);
	UINT64 bufferSize = 64ULL * 1024ULL;
	while (bufferSize < uploadSize)
		bufferSize *= 2;
	CDirectX12Buffer* pVertexBuffer = new CDirectX12Buffer;
	if (pVertexBuffer == NULL
		|| !pVertexBuffer->CreateVertexBuffer(
			m_pDevice,
			bufferSize,
			sizeof(TextureGpuVertex))
		|| !pVertexBuffer->Upload(
			pUploadManager,
			pCommandList,
			&m_pState->gpuVertices[0],
			uploadSize))
	{
		delete pVertexBuffer;
		ReportBatchFailureOnce("creacion o carga del buffer de vertices");
		return false;
	}
	m_pState->vertexBuffers[m_currentFrame].push_back(pVertexBuffer);

	const D3D12_VERTEX_BUFFER_VIEW vertexView =
		pVertexBuffer->GetVertexView();
	const D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView =
		pRenderTargets->GetCurrentView();
	D3D12_VIEWPORT viewport = {
		0.0f,
		0.0f,
		static_cast<FLOAT>(targetDesc.Width),
		static_cast<FLOAT>(targetDesc.Height),
		0.0f,
		1.0f
	};

	pCommandList->SetGraphicsRootSignature(
		m_pPipelineCache->GetRootSignature());
	pCommandList->RSSetViewports(1, &viewport);
	pCommandList->OMSetRenderTargets(1, &renderTargetView, FALSE, NULL);
	pCommandList->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexView);

	UINT visibleRangesByBlend[DX12_BLEND_COUNT] = { 0 };
	UINT visibleVerticesByBlend[DX12_BLEND_COUNT] = { 0 };
	UINT visibleTexturedRangeCount = 0;
	UINT skippedTextureRangeCount = 0;
	for (size_t iRange = 0; iRange < m_pState->ranges.size(); ++iRange)
	{
		const TextureBatchRange& range = m_pState->ranges[iRange];
		if (range.segment < minimumSegment
			|| range.segment > maximumSegment)
			continue;
		D3D12_RECT scissor = m_pState->ranges[iRange].scissor;
		scissor.left = (std::max)(scissor.left, 0L);
		scissor.top = (std::max)(scissor.top, 0L);
		scissor.right = (std::min)(
			scissor.right,
			static_cast<LONG>(targetDesc.Width));
		scissor.bottom = (std::min)(
			scissor.bottom,
			static_cast<LONG>(targetDesc.Height));
		if (scissor.left >= scissor.right
			|| scissor.top >= scissor.bottom)
			continue;

		const bool visible =
			showUi && range.scope == DX12_DRAWPORT_SCOPE_UI;
		if (visible
			&& validationMode == DX12_DRAWPORT_VALIDATION_UI_SPLIT)
		{
			scissor.left = (std::max)(
				scissor.left,
				static_cast<LONG>(targetDesc.Width / 2));
			if (scissor.left >= scissor.right)
				continue;
		}
		const DirectX12BlendMode effectiveBlendMode =
			visible ? range.blendMode : DX12_BLEND_ALPHA;
		if (visible
			&& effectiveBlendMode >= 0
			&& effectiveBlendMode < DX12_BLEND_COUNT)
		{
			++visibleRangesByBlend[effectiveBlendMode];
			visibleVerticesByBlend[effectiveBlendMode] += range.vertexCount;
			if (range.pTexture != NULL)
				++visibleTexturedRangeCount;
		}
		const DirectX12PipelineKind pipelineKind =
			effectiveBlendMode == DX12_BLEND_ALPHA_TEST
				? DX12_PIPELINE_TEXTURED_ALPHA_TEST_2D
				: DX12_PIPELINE_TEXTURED_2D;
		ID3D12PipelineState* pPipelineState =
			m_pPipelineCache->GetPipelineState(
				pipelineKind,
				targetDesc.Format,
				targetDesc.SampleDesc,
				effectiveBlendMode);
		if (pPipelineState == NULL
			|| !pSamplers[range.samplerMode].IsValid())
		{
			ReportBatchFailureOnce("pipeline o sampler de rango invalido");
			return false;
		}
		pCommandList->SetPipelineState(pPipelineState);
		pCommandList->SetGraphicsRootDescriptorTable(
			1,
			pSamplers[range.samplerMode].gpu);

		D3D12_GPU_DESCRIPTOR_HANDLE textureView;
		if (range.pTexture != NULL)
		{
			const bool acquired =
				range.renderTextureHandle.IsValid()
					? pTextures->Acquire(
						range.renderTextureHandle,
						pCommandList,
						&textureView)
					: range.textureHandle.IsValid()
					? pTextures->Acquire(
						range.textureHandle,
						pCommandList,
						&textureView)
					: pTextures->Acquire(
						range.pTexture,
						pCommandList,
						pUploadManager,
						&textureView);
			if (!acquired)
			{
				// Una textura incompatible no debe cancelar los comandos de UI
				// restantes: se omite solamente su rango y se informa al final.
				if (visible)
					++skippedTextureRangeCount;
				continue;
			}
		}
		else
			textureView = solidTextureView;
		pCommandList->SetGraphicsRootDescriptorTable(0, textureView);
		pCommandList->RSSetScissorRects(1, &scissor);
		pCommandList->DrawInstanced(
			range.vertexCount,
			1,
			range.firstVertex,
			0);
	}
	static UINT s_lastSkippedTextureRangeCount = static_cast<UINT>(-1);
	if (skippedTextureRangeCount != s_lastSkippedTextureRangeCount)
	{
		if (skippedTextureRangeCount > 0)
		{
			CPrintF(
				"DX12 UI: %u lotes de textura omitidos durante el replay.\n",
				skippedTextureRangeCount);
		}
		s_lastSkippedTextureRangeCount = skippedTextureRangeCount;
	}
	if (ReadUiDiagnosticsMode())
	{
		static bool s_diagnosticsReported = false;
		UINT visibleVertexCount = 0;
		UINT visibleRangeCount = 0;
		for (UINT iBlend = 0; iBlend < DX12_BLEND_COUNT; ++iBlend)
		{
			visibleVertexCount += visibleVerticesByBlend[iBlend];
			visibleRangeCount += visibleRangesByBlend[iBlend];
		}
		if (!s_diagnosticsReported && visibleRangeCount > 0)
		{
			CPrintF(
				"DX12 UI diagnostico: %u rangos (%u con textura), "
				"%u vertices; mezcla O=%u AT=%u A=%u S=%u "
				"ADD=%u ADDA=%u M=%u IM=%u; omitidos=%u.\n",
				visibleRangeCount,
				visibleTexturedRangeCount,
				visibleVertexCount,
				visibleVerticesByBlend[DX12_BLEND_OPAQUE],
				visibleVerticesByBlend[DX12_BLEND_ALPHA_TEST],
				visibleVerticesByBlend[DX12_BLEND_ALPHA],
				visibleVerticesByBlend[DX12_BLEND_SHADE],
				visibleVerticesByBlend[DX12_BLEND_ADD],
				visibleVerticesByBlend[DX12_BLEND_ADD_ALPHA],
				visibleVerticesByBlend[DX12_BLEND_MULTIPLY],
				visibleVerticesByBlend[DX12_BLEND_INVERSE_MULTIPLY],
				skippedTextureRangeCount);
			s_diagnosticsReported = true;
		}
	}
	return true;
}

UINT CDirectX12DrawPortCommandBatch::GetPrimitiveCount() const
{
	return m_pState != NULL ? m_pState->primitiveCount : 0;
}

UINT CDirectX12DrawPortCommandBatch::GetUiPrimitiveCount() const
{
	return m_pState != NULL ? m_pState->uiPrimitiveCount : 0;
}

UINT CDirectX12DrawPortCommandBatch::GetUiSegmentCount() const
{
	if (m_pState == NULL)
		return 0;
	UINT segmentCount = 0;
	UINT previousSegment = static_cast<UINT>(-1);
	for (size_t iRange = 0; iRange < m_pState->ranges.size(); ++iRange)
	{
		const TextureBatchRange& range = m_pState->ranges[iRange];
		if (range.scope == DX12_DRAWPORT_SCOPE_UI
			&& range.segment != previousSegment)
		{
			++segmentCount;
			previousSegment = range.segment;
		}
	}
	return segmentCount;
}

UINT CDirectX12DrawPortCommandBatch::GetUiBarrierCount() const
{
	return m_pState != NULL
		? static_cast<UINT>(m_pState->barriers.size())
		: 0;
}

UINT CDirectX12DrawPortCommandBatch::GetCurrentSegment() const
{
	return m_pState != NULL ? m_pState->currentSegment : 0;
}

bool CDirectX12DrawPortCommandBatch::HasUiCommands(
	UINT minimumSegment,
	UINT maximumSegment) const
{
	if (m_pState == NULL || minimumSegment > maximumSegment)
		return false;
	for (size_t iRange = 0; iRange < m_pState->ranges.size(); ++iRange)
	{
		const TextureBatchRange& range = m_pState->ranges[iRange];
		if (range.scope == DX12_DRAWPORT_SCOPE_UI
			&& range.segment >= minimumSegment
			&& range.segment <= maximumSegment)
			return true;
	}
	return false;
}

void CDirectX12DrawPortCommandBatch::ClearQueuedResources()
{
	if (m_pState == NULL)
		return;
	for (size_t iRange = 0;
		iRange < m_pState->ranges.size();
		++iRange)
	{
		if (m_pState->ranges[iRange].pTexture != NULL)
			m_pState->ranges[iRange].pTexture->Release();
	}
	m_pState->vertices.clear();
	m_pState->gpuVertices.clear();
	m_pState->ranges.clear();
	m_pState->scopes.clear();
	m_pState->barriers.clear();
	m_pState->primitiveCount = 0;
	m_pState->uiPrimitiveCount = 0;
	m_pState->currentSegment = 0;
}

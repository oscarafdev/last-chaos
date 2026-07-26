#include "stdh.h"

#include <vector>
#include <d3d12.h>
#include <d3dcompiler.h>

#include <Engine/Base/Console.h>
#include <Engine/Graphics/DirectX12NativeShaderSource.h>
#include <Engine/Graphics/DirectX12PipelineCache.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")

namespace
{
	struct DirectX12PipelineEntry
	{
		DirectX12PipelineKind kind;
		DXGI_FORMAT format;
		DXGI_SAMPLE_DESC sampleDesc;
		DirectX12BlendMode blendMode;
		bool depthEnabled;
		bool depthWriteEnabled;
		D3D12_COMPARISON_FUNC depthFunction;
		D3D12_CULL_MODE cullMode;
		DXGI_FORMAT depthStencilFormat;
		ID3D12PipelineState* pPipelineState;
	};

	bool CompileShader(
		const char* pSource,
		const char* pSourceName,
		const char* pEntryPoint,
		const char* pTarget,
		UINT compileFlags,
		ID3DBlob** ppShader)
	{
		if (pSource == NULL || pSourceName == NULL
			|| pEntryPoint == NULL || pTarget == NULL
			|| ppShader == NULL)
			return false;

		ID3DBlob* pErrors = NULL;
		const HRESULT hr = D3DCompile(
			pSource,
			strlen(pSource),
			pSourceName,
			NULL,
			NULL,
			pEntryPoint,
			pTarget,
			compileFlags,
			0,
			ppShader,
			&pErrors);
		if (FAILED(hr))
		{
			const char* pDetails = pErrors != NULL
				? static_cast<const char*>(pErrors->GetBufferPointer())
				: "sin detalles del compilador";
			CPrintF(
				"DX12 error: No se pudo compilar %s/%s (%s, 0x%08X): %s\n",
				pSourceName,
				pEntryPoint,
				pTarget,
				static_cast<unsigned int>(hr),
				pDetails);
		}
		if (pErrors != NULL)
			pErrors->Release();
		return SUCCEEDED(hr);
	}
}

struct DirectX12PipelineCacheState
{
	std::vector<DirectX12PipelineEntry> entries;
};

CDirectX12PipelineCache::CDirectX12PipelineCache()
	: m_pDevice(NULL)
	, m_pRootSignature(NULL)
	, m_pVertexShader(NULL)
	, m_pPixelShader(NULL)
	, m_pTexturedVertexShader(NULL)
	, m_pTexturedPixelShader(NULL)
	, m_pAlphaTestPixelShader(NULL)
	, m_pBloomVertexShader(NULL)
	, m_pBloomDownsamplePixelShader(NULL)
	, m_pBloomBlurPixelShader(NULL)
	, m_pBloomCompositePixelShader(NULL)
	, m_pLegacy3DVertexShader(NULL)
	, m_pLegacy3DPixelShader(NULL)
	, m_pRigidLit3DVertexShader(NULL)
	, m_pRigidLit3DPixelShader(NULL)
	, m_pLegacyMaterial3DVertexShader(NULL)
	, m_pLegacyMaterial3DPixelShader(NULL)
	, m_pState(NULL)
{
}

CDirectX12PipelineCache::~CDirectX12PipelineCache()
{
	Shutdown();
}

bool CDirectX12PipelineCache::Initialize(ID3D12Device* pDevice)
{
	if (pDevice == NULL)
		return false;

	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pState = new DirectX12PipelineCacheState;
	if (m_pState == NULL || !CompileShaders() || !CreateRootSignature())
	{
		Shutdown();
		return false;
	}
	return true;
}

void CDirectX12PipelineCache::Shutdown()
{
	if (m_pState != NULL)
	{
		for (size_t iEntry = 0;
			iEntry < m_pState->entries.size();
			++iEntry)
		{
			m_pState->entries[iEntry].pPipelineState->Release();
		}
		delete m_pState;
		m_pState = NULL;
	}

	if (m_pPixelShader != NULL)
	{
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}
	if (m_pTexturedPixelShader != NULL)
	{
		m_pTexturedPixelShader->Release();
		m_pTexturedPixelShader = NULL;
	}
	if (m_pAlphaTestPixelShader != NULL)
	{
		m_pAlphaTestPixelShader->Release();
		m_pAlphaTestPixelShader = NULL;
	}
	if (m_pBloomCompositePixelShader != NULL)
	{
		m_pBloomCompositePixelShader->Release();
		m_pBloomCompositePixelShader = NULL;
	}
	if (m_pBloomBlurPixelShader != NULL)
	{
		m_pBloomBlurPixelShader->Release();
		m_pBloomBlurPixelShader = NULL;
	}
	if (m_pBloomDownsamplePixelShader != NULL)
	{
		m_pBloomDownsamplePixelShader->Release();
		m_pBloomDownsamplePixelShader = NULL;
	}
	if (m_pBloomVertexShader != NULL)
	{
		m_pBloomVertexShader->Release();
		m_pBloomVertexShader = NULL;
	}
	if (m_pLegacy3DPixelShader != NULL)
	{
		m_pLegacy3DPixelShader->Release();
		m_pLegacy3DPixelShader = NULL;
	}
	if (m_pRigidLit3DPixelShader != NULL)
	{
		m_pRigidLit3DPixelShader->Release();
		m_pRigidLit3DPixelShader = NULL;
	}
	if (m_pLegacyMaterial3DPixelShader != NULL)
	{
		m_pLegacyMaterial3DPixelShader->Release();
		m_pLegacyMaterial3DPixelShader = NULL;
	}
	if (m_pLegacyMaterial3DVertexShader != NULL)
	{
		m_pLegacyMaterial3DVertexShader->Release();
		m_pLegacyMaterial3DVertexShader = NULL;
	}
	if (m_pRigidLit3DVertexShader != NULL)
	{
		m_pRigidLit3DVertexShader->Release();
		m_pRigidLit3DVertexShader = NULL;
	}
	if (m_pLegacy3DVertexShader != NULL)
	{
		m_pLegacy3DVertexShader->Release();
		m_pLegacy3DVertexShader = NULL;
	}
	if (m_pTexturedVertexShader != NULL)
	{
		m_pTexturedVertexShader->Release();
		m_pTexturedVertexShader = NULL;
	}
	if (m_pVertexShader != NULL)
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}
	if (m_pRootSignature != NULL)
	{
		m_pRootSignature->Release();
		m_pRootSignature = NULL;
	}
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

bool CDirectX12PipelineCache::CompileShaders()
{
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifndef NDEBUG
	compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	const char* pShaderSource =
		GetDirectX12NativeValidationShader();
	if (!CompileShader(
			pShaderSource,
			"LastChaosNativeValidation",
			"VSMain",
			"vs_5_0",
			compileFlags,
			&m_pVertexShader))
		return false;
	if (!CompileShader(
		pShaderSource,
		"LastChaosNativeValidation",
		"PSMain",
		"ps_5_0",
		compileFlags,
		&m_pPixelShader))
		return false;

	const char* pTexturedShaderSource = GetDirectX12Textured2DShader();
	if (!CompileShader(
		pTexturedShaderSource,
		"LastChaosTextured2D",
		"VSMain",
		"vs_5_0",
		compileFlags,
		&m_pTexturedVertexShader))
		return false;
	if (!CompileShader(
		pTexturedShaderSource,
		"LastChaosTextured2D",
		"PSMain",
		"ps_5_0",
		compileFlags,
		&m_pTexturedPixelShader))
		return false;

	const char* pAlphaTestShaderSource =
		GetDirectX12TexturedAlphaTest2DShader();
	if (!CompileShader(
			pAlphaTestShaderSource,
		"LastChaosTexturedAlphaTest2D",
		"PSMain",
		"ps_5_0",
		compileFlags,
			&m_pAlphaTestPixelShader))
		return false;

	const char* pBloomShaderSource = GetDirectX12BloomShader();
	if (!CompileShader(
			pBloomShaderSource,
			"LastChaosBloom",
			"VSMain",
			"vs_5_0",
			compileFlags,
			&m_pBloomVertexShader)
		|| !CompileShader(
			pBloomShaderSource,
			"LastChaosBloom",
			"PSDownsample",
			"ps_5_0",
			compileFlags,
			&m_pBloomDownsamplePixelShader)
		|| !CompileShader(
			pBloomShaderSource,
			"LastChaosBloom",
			"PSBlur",
			"ps_5_0",
			compileFlags,
			&m_pBloomBlurPixelShader)
		|| !CompileShader(
			pBloomShaderSource,
			"LastChaosBloom",
			"PSComposite",
			"ps_5_0",
			compileFlags,
			&m_pBloomCompositePixelShader))
		return false;

	const char* pLegacy3DShaderSource =
		GetDirectX12Legacy3DShader();
	if (!CompileShader(
		pLegacy3DShaderSource,
		"LastChaosLegacy3D",
		"VSMain",
		"vs_5_0",
		compileFlags,
		&m_pLegacy3DVertexShader))
		return false;
	if (!CompileShader(
		pLegacy3DShaderSource,
		"LastChaosLegacy3D",
		"PSMain",
		"ps_5_0",
		compileFlags,
		&m_pLegacy3DPixelShader))
		return false;
	const char* pRigidLitShaderSource = GetDirectX12RigidLit3DShader();
	if (!CompileShader(
		pRigidLitShaderSource,
		"LastChaosRigidLit3D",
		"VSMain",
		"vs_5_0",
		compileFlags,
		&m_pRigidLit3DVertexShader))
		return false;
	if (!CompileShader(
		pRigidLitShaderSource,
		"LastChaosRigidLit3D",
		"PSMain",
		"ps_5_0",
		compileFlags,
		&m_pRigidLit3DPixelShader))
		return false;
	const char* pMaterialShaderSource =
		GetDirectX12LegacyMaterial3DShader();
	if (!CompileShader(
		pMaterialShaderSource,
		"LastChaosLegacyMaterial3D",
		"VSMain",
		"vs_5_0",
		compileFlags,
		&m_pLegacyMaterial3DVertexShader))
		return false;
	return CompileShader(
		pMaterialShaderSource,
		"LastChaosLegacyMaterial3D",
		"PSMain",
		"ps_5_0",
		compileFlags,
		&m_pLegacyMaterial3DPixelShader);
}

bool CDirectX12PipelineCache::CreateRootSignature()
{
	D3D12_DESCRIPTOR_RANGE ranges[5];
	ZeroMemory(ranges, sizeof(ranges));
	ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	ranges[0].NumDescriptors = 1;
	ranges[0].BaseShaderRegister = 0;
	ranges[0].OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
	ranges[1].NumDescriptors = 1;
	ranges[1].BaseShaderRegister = 0;
	ranges[1].OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	ranges[2].NumDescriptors = 1;
	ranges[2].BaseShaderRegister = 1;
	ranges[2].OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	for (UINT textureUnit = 2; textureUnit < 4; ++textureUnit)
	{
		D3D12_DESCRIPTOR_RANGE& range = ranges[textureUnit + 1];
		range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range.NumDescriptors = 1;
		range.BaseShaderRegister = textureUnit;
		range.OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	D3D12_ROOT_PARAMETER parameters[7];
	ZeroMemory(parameters, sizeof(parameters));
	for (UINT iParameter = 0; iParameter < 2; ++iParameter)
	{
		parameters[iParameter].ParameterType =
			D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		parameters[iParameter].DescriptorTable.NumDescriptorRanges = 1;
		parameters[iParameter].DescriptorTable.pDescriptorRanges =
			&ranges[iParameter];
		parameters[iParameter].ShaderVisibility =
			D3D12_SHADER_VISIBILITY_PIXEL;
	}
	parameters[2].ParameterType =
		D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	parameters[2].Constants.ShaderRegister = 0;
	parameters[2].Constants.RegisterSpace = 0;
	parameters[2].Constants.Num32BitValues = 13 * 4;
	// Las familias genéricas reutilizan las constantes heredadas tanto en el
	// VS como en el PS (materiales, terreno y geometría multipass).
	parameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	parameters[3].ParameterType =
		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	parameters[3].DescriptorTable.NumDescriptorRanges = 1;
	parameters[3].DescriptorTable.pDescriptorRanges = &ranges[2];
	parameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	parameters[4].ParameterType =
		D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	parameters[4].Constants.ShaderRegister = 1;
	parameters[4].Constants.RegisterSpace = 0;
	parameters[4].Constants.Num32BitValues = 7;
	parameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	for (UINT textureUnit = 2; textureUnit < 4; ++textureUnit)
	{
		D3D12_ROOT_PARAMETER& parameter = parameters[textureUnit + 3];
		parameter.ParameterType =
			D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		parameter.DescriptorTable.NumDescriptorRanges = 1;
		parameter.DescriptorTable.pDescriptorRanges =
			&ranges[textureUnit + 1];
		parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	D3D12_ROOT_SIGNATURE_DESC rootDesc;
	ZeroMemory(&rootDesc, sizeof(rootDesc));
	rootDesc.NumParameters =
		sizeof(parameters) / sizeof(parameters[0]);
	rootDesc.pParameters = parameters;
	rootDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	ID3DBlob* pSerializedSignature = NULL;
	ID3DBlob* pErrors = NULL;
	HRESULT hr = D3D12SerializeRootSignature(
		&rootDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&pSerializedSignature,
		&pErrors);
	if (pErrors != NULL)
		pErrors->Release();
	if (FAILED(hr))
		return false;

	hr = m_pDevice->CreateRootSignature(
		0,
		pSerializedSignature->GetBufferPointer(),
		pSerializedSignature->GetBufferSize(),
		__uuidof(ID3D12RootSignature),
		reinterpret_cast<void**>(&m_pRootSignature));
	pSerializedSignature->Release();
	if (FAILED(hr))
		return false;

	m_pRootSignature->SetName(
		L"LastChaos D3D12 Native Validation Root Signature");
	return true;
}

ID3D12RootSignature*
CDirectX12PipelineCache::GetRootSignature() const
{
	return m_pRootSignature;
}

ID3D12PipelineState* CDirectX12PipelineCache::GetPipelineState(
	DirectX12PipelineKind kind,
	DXGI_FORMAT renderTargetFormat,
	DXGI_SAMPLE_DESC sampleDesc,
	DirectX12BlendMode blendMode,
	bool depthEnabled,
	bool depthWriteEnabled,
	D3D12_COMPARISON_FUNC depthFunction,
	D3D12_CULL_MODE cullMode,
	DXGI_FORMAT depthStencilFormat)
{
	if (m_pState == NULL || renderTargetFormat == DXGI_FORMAT_UNKNOWN
		|| sampleDesc.Count == 0)
		return NULL;

	for (size_t iEntry = 0;
		iEntry < m_pState->entries.size();
		++iEntry)
	{
		if (m_pState->entries[iEntry].kind == kind
			&& m_pState->entries[iEntry].format == renderTargetFormat
			&& m_pState->entries[iEntry].sampleDesc.Count
				== sampleDesc.Count
			&& m_pState->entries[iEntry].sampleDesc.Quality
				== sampleDesc.Quality
			&& m_pState->entries[iEntry].blendMode == blendMode
			&& m_pState->entries[iEntry].depthEnabled == depthEnabled
			&& m_pState->entries[iEntry].depthWriteEnabled
				== depthWriteEnabled
			&& m_pState->entries[iEntry].depthFunction == depthFunction
			&& m_pState->entries[iEntry].cullMode == cullMode
			&& m_pState->entries[iEntry].depthStencilFormat
				== depthStencilFormat)
			return m_pState->entries[iEntry].pPipelineState;
	}

	ID3D12PipelineState* pPipelineState =
		CreatePipelineState(
			kind,
			renderTargetFormat,
			sampleDesc,
			blendMode,
			depthEnabled,
			depthWriteEnabled,
			depthFunction,
			cullMode,
			depthStencilFormat);
	if (pPipelineState == NULL)
		return NULL;

	DirectX12PipelineEntry entry;
	entry.kind = kind;
	entry.format = renderTargetFormat;
	entry.sampleDesc = sampleDesc;
	entry.blendMode = blendMode;
	entry.depthEnabled = depthEnabled;
	entry.depthWriteEnabled = depthWriteEnabled;
	entry.depthFunction = depthFunction;
	entry.cullMode = cullMode;
	entry.depthStencilFormat = depthStencilFormat;
	entry.pPipelineState = pPipelineState;
	m_pState->entries.push_back(entry);
	return pPipelineState;
}

ID3D12PipelineState* CDirectX12PipelineCache::CreatePipelineState(
	DirectX12PipelineKind kind,
	DXGI_FORMAT renderTargetFormat,
	DXGI_SAMPLE_DESC sampleDesc,
	DirectX12BlendMode blendMode,
	bool depthEnabled,
	bool depthWriteEnabled,
	D3D12_COMPARISON_FUNC depthFunction,
	D3D12_CULL_MODE cullMode,
	DXGI_FORMAT depthStencilFormat)
{
	D3D12_INPUT_ELEMENT_DESC inputElements[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};
	D3D12_INPUT_ELEMENT_DESC texturedInputElements[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			8,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			16,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};
	D3D12_INPUT_ELEMENT_DESC legacy3DInputElements[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
			0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,
			0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT,
			0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT,
			0, 64, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 88, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 104, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 120, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 13, DXGI_FORMAT_R32_FLOAT,
			0, 136, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	ID3DBlob* pSelectedVertexShader = m_pVertexShader;
	ID3DBlob* pSelectedPixelShader = m_pPixelShader;
	const D3D12_INPUT_ELEMENT_DESC* pSelectedInput = inputElements;
	UINT selectedInputCount =
		sizeof(inputElements) / sizeof(inputElements[0]);
	if (kind == DX12_PIPELINE_TEXTURED_2D)
	{
		pSelectedVertexShader = m_pTexturedVertexShader;
		pSelectedPixelShader = m_pTexturedPixelShader;
		pSelectedInput = texturedInputElements;
		selectedInputCount =
			sizeof(texturedInputElements) / sizeof(texturedInputElements[0]);
	}
	else if (kind == DX12_PIPELINE_TEXTURED_ALPHA_TEST_2D)
	{
		pSelectedVertexShader = m_pTexturedVertexShader;
		pSelectedPixelShader = m_pAlphaTestPixelShader;
		pSelectedInput = texturedInputElements;
		selectedInputCount =
			sizeof(texturedInputElements) / sizeof(texturedInputElements[0]);
	}
	else if (kind == DX12_PIPELINE_BLOOM_DOWNSAMPLE
		|| kind == DX12_PIPELINE_BLOOM_BLUR
		|| kind == DX12_PIPELINE_BLOOM_COMPOSITE)
	{
		pSelectedVertexShader = m_pBloomVertexShader;
		pSelectedPixelShader =
			kind == DX12_PIPELINE_BLOOM_DOWNSAMPLE
				? m_pBloomDownsamplePixelShader
				: kind == DX12_PIPELINE_BLOOM_BLUR
					? m_pBloomBlurPixelShader
					: m_pBloomCompositePixelShader;
	}
	else if (kind == DX12_PIPELINE_TEXTURED_3D_SHADOW
		|| kind == DX12_PIPELINE_TEXTURED_3D_OVERLAY)
	{
		pSelectedVertexShader = m_pLegacy3DVertexShader;
		pSelectedPixelShader = m_pLegacy3DPixelShader;
		pSelectedInput = legacy3DInputElements;
		selectedInputCount =
			sizeof(legacy3DInputElements)
			/ sizeof(legacy3DInputElements[0]);
	}
	else if (kind == DX12_PIPELINE_RIGID_LIT_3D_SHADOW
		|| kind == DX12_PIPELINE_RIGID_LIT_3D_OVERLAY)
	{
		pSelectedVertexShader = m_pRigidLit3DVertexShader;
		pSelectedPixelShader = m_pRigidLit3DPixelShader;
		pSelectedInput = legacy3DInputElements;
		selectedInputCount =
			sizeof(legacy3DInputElements)
			/ sizeof(legacy3DInputElements[0]);
	}
	else if (kind == DX12_PIPELINE_LEGACY_MATERIAL_3D_SHADOW
		|| kind == DX12_PIPELINE_LEGACY_MATERIAL_3D_OVERLAY)
	{
		pSelectedVertexShader = m_pLegacyMaterial3DVertexShader;
		pSelectedPixelShader = m_pLegacyMaterial3DPixelShader;
		pSelectedInput = legacy3DInputElements;
		selectedInputCount =
			sizeof(legacy3DInputElements)
			/ sizeof(legacy3DInputElements[0]);
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(psoDesc));
	psoDesc.pRootSignature = m_pRootSignature;
	psoDesc.VS.pShaderBytecode =
		pSelectedVertexShader->GetBufferPointer();
	psoDesc.VS.BytecodeLength =
		pSelectedVertexShader->GetBufferSize();
	psoDesc.PS.pShaderBytecode =
		pSelectedPixelShader->GetBufferPointer();
	psoDesc.PS.BytecodeLength =
		pSelectedPixelShader->GetBufferSize();
	psoDesc.BlendState.AlphaToCoverageEnable = FALSE;
	psoDesc.BlendState.IndependentBlendEnable = FALSE;
	D3D12_RENDER_TARGET_BLEND_DESC& blend =
		psoDesc.BlendState.RenderTarget[0];
	blend.BlendEnable =
		blendMode != DX12_BLEND_OPAQUE
		&& blendMode != DX12_BLEND_ALPHA_TEST;
	blend.LogicOpEnable = FALSE;
	blend.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blend.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	if (blendMode == DX12_BLEND_SHADE)
	{
		blend.SrcBlend = D3D12_BLEND_DEST_COLOR;
		blend.DestBlend = D3D12_BLEND_SRC_COLOR;
	}
	else if (blendMode == DX12_BLEND_ADD)
	{
		blend.SrcBlend = D3D12_BLEND_ONE;
		blend.DestBlend = D3D12_BLEND_ONE;
	}
	else if (blendMode == DX12_BLEND_ADD_ALPHA)
	{
		blend.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blend.DestBlend = D3D12_BLEND_ONE;
	}
	else if (blendMode == DX12_BLEND_MULTIPLY)
	{
		blend.SrcBlend = D3D12_BLEND_DEST_COLOR;
		blend.DestBlend = D3D12_BLEND_ZERO;
	}
	else if (blendMode == DX12_BLEND_INVERSE_MULTIPLY)
	{
		blend.SrcBlend = D3D12_BLEND_ZERO;
		blend.DestBlend = D3D12_BLEND_INV_SRC_COLOR;
	}
	else if (blendMode == DX12_BLEND_TERRAIN_LAYER)
	{
		blend.SrcBlend = D3D12_BLEND_ONE;
		blend.DestBlend = D3D12_BLEND_SRC_ALPHA;
	}
	blend.BlendOp = D3D12_BLEND_OP_ADD;
	blend.SrcBlendAlpha = D3D12_BLEND_ZERO;
	blend.DestBlendAlpha = D3D12_BLEND_ONE;
	blend.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blend.LogicOp = D3D12_LOGIC_OP_NOOP;
	blend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	if (kind == DX12_PIPELINE_TEXTURED_3D_SHADOW
		|| kind == DX12_PIPELINE_RIGID_LIT_3D_SHADOW
		|| kind == DX12_PIPELINE_LEGACY_MATERIAL_3D_SHADOW)
		blend.RenderTargetWriteMask = 0;

	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.CullMode = cullMode;
	psoDesc.RasterizerState.FrontCounterClockwise = FALSE;
	psoDesc.RasterizerState.DepthBias =
		D3D12_DEFAULT_DEPTH_BIAS;
	psoDesc.RasterizerState.DepthBiasClamp =
		D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	psoDesc.RasterizerState.SlopeScaledDepthBias =
		D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	psoDesc.RasterizerState.DepthClipEnable = TRUE;
	psoDesc.RasterizerState.MultisampleEnable =
		sampleDesc.Count > 1;
	psoDesc.DepthStencilState.DepthEnable = depthEnabled;
	psoDesc.DepthStencilState.DepthWriteMask = depthWriteEnabled
		? D3D12_DEPTH_WRITE_MASK_ALL
		: D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState.DepthFunc = depthFunction;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.InputLayout.pInputElementDescs = pSelectedInput;
	psoDesc.InputLayout.NumElements = selectedInputCount;
	psoDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = renderTargetFormat;
	psoDesc.DSVFormat = (kind == DX12_PIPELINE_TEXTURED_3D_SHADOW
		|| kind == DX12_PIPELINE_TEXTURED_3D_OVERLAY)
		|| kind == DX12_PIPELINE_RIGID_LIT_3D_SHADOW
		|| kind == DX12_PIPELINE_RIGID_LIT_3D_OVERLAY
		|| kind == DX12_PIPELINE_LEGACY_MATERIAL_3D_SHADOW
		|| kind == DX12_PIPELINE_LEGACY_MATERIAL_3D_OVERLAY
		? depthStencilFormat
		: DXGI_FORMAT_UNKNOWN;
	psoDesc.SampleDesc = sampleDesc;

	ID3D12PipelineState* pPipelineState = NULL;
	HRESULT hr = m_pDevice->CreateGraphicsPipelineState(
		&psoDesc,
		__uuidof(ID3D12PipelineState),
		reinterpret_cast<void**>(&pPipelineState));
	if (FAILED(hr))
	{
		D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport;
		ZeroMemory(&formatSupport, sizeof(formatSupport));
		formatSupport.Format = renderTargetFormat;
		const HRESULT supportResult = m_pDevice->CheckFeatureSupport(
			D3D12_FEATURE_FORMAT_SUPPORT,
			&formatSupport,
			sizeof(formatSupport));
		CPrintF(
			"DX12 error: CreateGraphicsPipelineState fallo "
			"(tipo %d, RTV %d, DSV %d, muestras %u/%u, mezcla %d, "
			"depth=%d/%d, funcion=%d, cull=%d, soporte=0x%08X/"
			"0x%08X, consulta=0x%08X, error=0x%08X).\n",
			static_cast<int>(kind),
			static_cast<int>(renderTargetFormat),
			static_cast<int>(psoDesc.DSVFormat),
			sampleDesc.Count,
			sampleDesc.Quality,
			static_cast<int>(blendMode),
			depthEnabled ? 1 : 0,
			depthWriteEnabled ? 1 : 0,
			static_cast<int>(depthFunction),
			static_cast<int>(cullMode),
			static_cast<unsigned int>(formatSupport.Support1),
			static_cast<unsigned int>(formatSupport.Support2),
			static_cast<unsigned int>(supportResult),
			static_cast<unsigned int>(hr));
		return NULL;
	}

	const wchar_t* pName = L"LastChaos D3D12 Native Validation PSO";
	if (kind == DX12_PIPELINE_TEXTURED_2D)
		pName = L"LastChaos D3D12 Textured 2D PSO";
	else if (kind == DX12_PIPELINE_TEXTURED_ALPHA_TEST_2D)
		pName = L"LastChaos D3D12 Textured Alpha Test 2D PSO";
	else if (kind == DX12_PIPELINE_BLOOM_DOWNSAMPLE)
		pName = L"LastChaos D3D12 Bloom Downsample PSO";
	else if (kind == DX12_PIPELINE_BLOOM_BLUR)
		pName = L"LastChaos D3D12 Bloom Blur PSO";
	else if (kind == DX12_PIPELINE_BLOOM_COMPOSITE)
		pName = L"LastChaos D3D12 Bloom Composite PSO";
	else if (kind == DX12_PIPELINE_TEXTURED_3D_SHADOW)
		pName = L"LastChaos D3D12 Legacy 3D Shadow PSO";
	else if (kind == DX12_PIPELINE_TEXTURED_3D_OVERLAY)
		pName = L"LastChaos D3D12 Legacy 3D Overlay PSO";
	else if (kind == DX12_PIPELINE_RIGID_LIT_3D_SHADOW)
		pName = L"LastChaos D3D12 Rigid Lit 3D Shadow PSO";
	else if (kind == DX12_PIPELINE_RIGID_LIT_3D_OVERLAY)
		pName = L"LastChaos D3D12 Rigid Lit 3D Overlay PSO";
	else if (kind == DX12_PIPELINE_LEGACY_MATERIAL_3D_SHADOW)
		pName = L"LastChaos D3D12 Legacy Material 3D Shadow PSO";
	else if (kind == DX12_PIPELINE_LEGACY_MATERIAL_3D_OVERLAY)
		pName = L"LastChaos D3D12 Legacy Material 3D Overlay PSO";
	pPipelineState->SetName(pName);
	return pPipelineState;
}

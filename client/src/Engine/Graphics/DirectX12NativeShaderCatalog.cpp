#include "stdh.h"

#include <DirectX12NativeShaderBlobs.h>

#include <Engine/Graphics/DirectX12NativeShaderCatalog.h>

namespace
{
	struct DirectX12ShaderBlob
	{
		const void* pData;
		SIZE_T size;
	};

#define DX12_GENERATED_SHADER(name) \
	{ \
		DirectX12GeneratedShaders::k##name, \
		sizeof(DirectX12GeneratedShaders::k##name) \
	}

	const DirectX12ShaderBlob g_shaderBlobs[DX12_SHADER_NATIVE_COUNT] = {
		DX12_GENERATED_SHADER(NativeValidationVS),
		DX12_GENERATED_SHADER(NativeValidationPS),
		DX12_GENERATED_SHADER(Textured2DVS),
		DX12_GENERATED_SHADER(Textured2DPS),
		DX12_GENERATED_SHADER(TexturedAlphaTest2DPS),
		DX12_GENERATED_SHADER(BloomVS),
		DX12_GENERATED_SHADER(BloomDownsamplePS),
		DX12_GENERATED_SHADER(BloomBlurPS),
		DX12_GENERATED_SHADER(BloomCompositePS),
		DX12_GENERATED_SHADER(Legacy3DVS),
		DX12_GENERATED_SHADER(Legacy3DPS),
		DX12_GENERATED_SHADER(RigidLit3DVS),
		DX12_GENERATED_SHADER(RigidLit3DPS),
		DX12_GENERATED_SHADER(LegacyMaterial3DVS),
		DX12_GENERATED_SHADER(LegacyMaterial3DPS)
	};

#undef DX12_GENERATED_SHADER
}

bool GetDirectX12NativeShaderBytecode(
	DirectX12NativeShaderId shader,
	D3D12_SHADER_BYTECODE* pBytecode)
{
	if (pBytecode == NULL || shader < 0
		|| shader >= DX12_SHADER_NATIVE_COUNT)
		return false;

	const DirectX12ShaderBlob& blob = g_shaderBlobs[shader];
	pBytecode->pShaderBytecode = blob.pData;
	pBytecode->BytecodeLength = blob.size;
	return blob.pData != NULL && blob.size > 0;
}

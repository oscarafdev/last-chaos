#include "stdh.h"

#include <algorithm>
#include <float.h>
#include <vector>
#include <d3d9.h>
#include <Engine/Math/GfxMath.h>

#include <Engine/Base/Console.h>
#include <Engine/Testing/CameraTestCapture.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/DirectX12Buffer.h>
#include <Engine/Graphics/DirectX12DepthBuffer.h>
#include <Engine/Graphics/DirectX12DescriptorHeap.h>
#include <Engine/Graphics/DirectX12InteropTextureManager.h>
#include <Engine/Graphics/DirectX12Legacy3DCommandBatch.h>
#include <Engine/Graphics/DirectX12LegacyShaderFamily.h>
#include <Engine/Graphics/DirectX12PipelineCache.h>
#include <Engine/Graphics/DirectX12RenderState.h>
#include <Engine/Graphics/DirectX12RenderTargetManager.h>
#include <Engine/Graphics/DirectX12UploadManager.h>

namespace
{
	const UINT PROBE_INTERVAL_FRAMES = 120;
	const UINT PROBE_MAX_DRAWS = 1;
	const UINT PROBE_MAX_VERTICES = 4096;
	const UINT PROBE_MAX_INDICES = 12288;
	const UINT CONTINUOUS_MAX_DRAWS = 8;
	const UINT CONTINUOUS_MAX_VERTICES = 32768;
	const UINT CONTINUOUS_MAX_INDICES = 98304;
	const UINT REPLACEMENT_MAX_DRAWS = 256;
	const UINT REPLACEMENT_MAX_VERTICES = 524288;
	const UINT REPLACEMENT_MAX_INDICES = 1572864;
	const UINT FULL_REPLACEMENT_MAX_DRAWS = 2048;
	const UINT FULL_REPLACEMENT_MAX_VERTICES = 2097152;
	const UINT FULL_REPLACEMENT_MAX_INDICES = 6291456;
	const UINT64 RIGID_LIT_VERTEX_SHADER_FAMILY =
		0x88CDE6E1231B48B2ULL;
	const UINT64 RIGID_LIT_PIXEL_SHADER_FAMILY =
		0x4E91DDD261F074A2ULL;
	const UINT RIGID_LIT_CONSTANT_COUNT = 13 * 4;
	const UINT RIGID_LIT_PIXEL_CONSTANT_COUNT = 7;

	enum Native3DCaptureProfile
	{
		NATIVE_3D_CAPTURE_OFF,
		NATIVE_3D_CAPTURE_PROBE,
		NATIVE_3D_CAPTURE_CONTINUOUS
	};

	enum FixedFunctionCaptureMode
	{
		FIXED_FUNCTION_CAPTURE_OPAQUE,
		FIXED_FUNCTION_CAPTURE_TRANSPARENT,
		FIXED_FUNCTION_CAPTURE_ALL
	};

	struct Native3DCaptureBudget
	{
		UINT maxDraws;
		UINT maxVertices;
		UINT maxIndices;
	};

	struct Legacy3DVertex
	{
		FLOAT position[3];
		FLOAT texCoord[2];
		FLOAT color[4];
		FLOAT normal[3];
		FLOAT texCoordExtra[3][2];
		FLOAT tangent[4];
		FLOAT blendIndices[4];
		FLOAT blendWeights[4];
		FLOAT secondaryColor[4];
		FLOAT clipW;
		FLOAT texCoordQ[4];
	};

	struct Legacy3DDrawRange
	{
		UINT firstIndex;
		UINT indexCount;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissor;
		IDirect3DTexture9* pTexture;
		IDirect3DTexture9* pTexture1;
		IDirect3DTexture9* pTexture2;
		IDirect3DTexture9* pTexture3;
		DirectX12TextureHandle textureHandle;
		DirectX12TextureHandle textureHandle1;
		DirectX12TextureHandle textureHandle2;
		DirectX12TextureHandle textureHandle3;
		DirectX12RenderTextureHandle renderTextureHandle;
		DirectX12RenderTextureHandle renderTextureHandle1;
		DirectX12RenderTextureHandle renderTextureHandle2;
		DirectX12RenderTextureHandle renderTextureHandle3;
		bool depthEnabled;
		bool depthWriteEnabled;
		bool colorWriteEnabled;
		bool depthClipEnabled;
		D3D12_COMPARISON_FUNC depthFunction;
		D3D12_CULL_MODE cullMode;
		DirectX12BlendMode blendMode;
		bool opaque;
		bool rigidLit;
		bool genericFamily;
		DirectX12SamplerMode samplerMode;
		FLOAT shaderConstants[RIGID_LIT_CONSTANT_COUNT];
		FLOAT pixelShaderConstants[RIGID_LIT_PIXEL_CONSTANT_COUNT];
	};

	struct Legacy3DVertexShaderFamily
	{
		UINT64 fingerprint;
		UINT drawCount;
		UINT triangleCount;
	};

	struct Legacy3DShaderPairInventory
	{
		UINT64 vertexFingerprint;
		UINT64 pixelFingerprint;
		UINT texturePassCount;
		bool dynamicBuffer;
		bool projectiveMapping;
		UINT drawCount;
		UINT triangleCount;
	};

	struct Legacy3DClipDiagnosticKey
	{
		UINT64 vertexFingerprint;
		UINT64 pixelFingerprint;
		bool clippingEnabled;
	};

	struct Legacy3DFixedFunctionInventory
	{
		UINT64 textureIdentity;
		UINT textureWidth;
		UINT textureHeight;
		INT textureFormat;
		UINT texturePassCount;
		UINT blendMode;
		UINT depthEnabled;
		UINT depthWriteEnabled;
		UINT depthFunction;
		UINT cullMode;
		UINT texCoordIndex;
		UINT textureTransformFlags;
		UINT colorOperation;
		UINT alphaOperation;
		bool dynamicBuffer;
		UINT drawCount;
		UINT triangleCount;
		FLOAT minimumX;
		FLOAT maximumX;
		FLOAT minimumY;
		FLOAT maximumY;
		FLOAT minimumZ;
		FLOAT maximumZ;
		FLOAT minimumW;
		FLOAT maximumW;
	};

	struct Legacy3DVertexShaderCacheEntry
	{
		IDirect3DVertexShader9* pShader;
		UINT64 fingerprint;
	};

	struct Legacy3DPixelShaderCacheEntry
	{
		IDirect3DPixelShader9* pShader;
		UINT64 fingerprint;
	};

	UINT64 HashBytes(UINT64 hash, const void* pData, size_t byteCount)
	{
		const BYTE* pBytes = static_cast<const BYTE*>(pData);
		for (size_t iByte = 0; iByte < byteCount; ++iByte)
		{
			hash ^= pBytes[iByte];
			hash *= 1099511628211ULL;
		}
		return hash;
	}

	void DumpVertexShader(
		UINT64 fingerprint,
		const BYTE* pBytecode,
		UINT byteCount,
		const char* pBinaryExtension)
	{
		char directory[MAX_PATH] = "";
		const DWORD directoryLength = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_SHADER_DUMP_DIR",
			directory,
			sizeof(directory));
		if (directoryLength == 0
			|| directoryLength >= sizeof(directory)
			|| pBytecode == NULL
			|| byteCount == 0)
			return;
		CreateDirectoryA(directory, NULL);

		char binaryPath[MAX_PATH] = "";
		_snprintf_s(
			binaryPath,
			sizeof(binaryPath),
			_TRUNCATE,
			"%s\\%016llX.%s",
			directory,
			static_cast<unsigned long long>(fingerprint),
			pBinaryExtension);
		HANDLE hBinary = CreateFileA(
			binaryPath,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hBinary != INVALID_HANDLE_VALUE)
		{
			DWORD written = 0;
			WriteFile(hBinary, pBytecode, byteCount, &written, NULL);
			CloseHandle(hBinary);
		}

	}

	D3D12_COMPARISON_FUNC ToDepthFunction(DWORD function)
	{
		switch (function)
		{
		case D3DCMP_NEVER: return D3D12_COMPARISON_FUNC_NEVER;
		case D3DCMP_LESS: return D3D12_COMPARISON_FUNC_LESS;
		case D3DCMP_EQUAL: return D3D12_COMPARISON_FUNC_EQUAL;
		case D3DCMP_LESSEQUAL: return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case D3DCMP_GREATER: return D3D12_COMPARISON_FUNC_GREATER;
		case D3DCMP_NOTEQUAL: return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case D3DCMP_GREATEREQUAL:
			return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		default: return D3D12_COMPARISON_FUNC_ALWAYS;
		}
	}

	D3D12_CULL_MODE ToCullMode(DWORD mode)
	{
		// El PSO considera horario el frente porque FrontCounterClockwise es
		// falso. Por eso D3DCULL_CW equivale a descartar el frente DX12 y
		// D3DCULL_CCW a descartar el dorso; invertirlos oculta las fachadas.
		if (mode == D3DCULL_CW)
			return D3D12_CULL_MODE_FRONT;
		if (mode == D3DCULL_CCW)
			return D3D12_CULL_MODE_BACK;
		return D3D12_CULL_MODE_NONE;
	}

	DirectX12BlendMode ToBlendMode(
		DWORD blending,
		DWORD alphaTest,
		DWORD sourceBlend,
		DWORD destinationBlend)
	{
		if (blending == FALSE)
			return alphaTest != FALSE
				? DX12_BLEND_ALPHA_TEST
				: DX12_BLEND_OPAQUE;
		if (sourceBlend == D3DBLEND_ONE
			&& destinationBlend == D3DBLEND_ONE)
			return DX12_BLEND_ADD;
		if (sourceBlend == D3DBLEND_SRCALPHA
			&& destinationBlend == D3DBLEND_ONE)
			return DX12_BLEND_ADD_ALPHA;
		if (sourceBlend == D3DBLEND_DESTCOLOR
			&& destinationBlend == D3DBLEND_ZERO)
			return DX12_BLEND_MULTIPLY;
		if (sourceBlend == D3DBLEND_DESTCOLOR
			&& destinationBlend == D3DBLEND_SRCCOLOR)
			return DX12_BLEND_SHADE;
		if (sourceBlend == D3DBLEND_ONE
			&& destinationBlend == D3DBLEND_SRCALPHA)
			return DX12_BLEND_TERRAIN_LAYER;
		if (sourceBlend == D3DBLEND_ZERO
			&& destinationBlend == D3DBLEND_INVSRCALPHA)
			return DX12_BLEND_DESTINATION_INVERSE_SOURCE_ALPHA;
		return DX12_BLEND_ALPHA;
	}

	DirectX12SamplerMode ToSamplerMode(
		DWORD addressU,
		DWORD addressV,
		DWORD minification,
		DWORD magnification)
	{
		const bool repeat =
			addressU == D3DTADDRESS_WRAP
			&& addressV == D3DTADDRESS_WRAP;
		const bool anisotropic =
			minification == D3DTEXF_ANISOTROPIC
			|| magnification == D3DTEXF_ANISOTROPIC;
		if (anisotropic)
			return repeat
				? DX12_SAMPLER_ANISOTROPIC_REPEAT
				: DX12_SAMPLER_ANISOTROPIC_CLAMP;
		const bool linear =
			minification != D3DTEXF_POINT
			|| magnification != D3DTEXF_POINT;
		if (linear)
			return repeat
				? DX12_SAMPLER_LINEAR_REPEAT
				: DX12_SAMPLER_LINEAR_CLAMP;
		return repeat
			? DX12_SAMPLER_POINT_REPEAT
			: DX12_SAMPLER_POINT_CLAMP;
	}

	void WriteD3DColor(DWORD color, FLOAT* pDestination)
	{
		pDestination[0] =
			static_cast<FLOAT>((color >> 16) & 0xFF) / 255.0f;
		pDestination[1] =
			static_cast<FLOAT>((color >> 8) & 0xFF) / 255.0f;
		pDestination[2] =
			static_cast<FLOAT>(color & 0xFF) / 255.0f;
		pDestination[3] =
			static_cast<FLOAT>((color >> 24) & 0xFF) / 255.0f;
	}

	UINT CountActiveFixedTextureStages(const DWORD* pColorOperations)
	{
		if (pColorOperations == NULL)
			return 0;
		UINT activeStageCount = 0;
		for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
		{
			if (pColorOperations[textureUnit] == D3DTOP_DISABLE)
				break;
			activeStageCount = textureUnit + 1;
		}
		return activeStageCount;
	}

	bool ReadFullReplacementMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_REPLACE_ALL",
			value,
			sizeof(value));
		const bool requested = length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
		if (!requested)
			return false;
		char gatePath[MAX_PATH] = "";
		const DWORD gateLength = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_REPLACE_ALL_GATE",
			gatePath,
			sizeof(gatePath));
		return gateLength == 0
			|| (gateLength < sizeof(gatePath)
				&& GetFileAttributesA(gatePath)
					!= INVALID_FILE_ATTRIBUTES);
	}

	bool ReadTerrainRasterDebugMode(const char* pVariableName)
	{
		if (pVariableName == NULL)
			return false;
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			pVariableName,
			value,
			sizeof(value));
		return length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
	}

	FixedFunctionCaptureMode ReadFixedFunctionCaptureMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_FIXED_CAPTURE",
			value,
			sizeof(value));
		if (length > 0 && length < sizeof(value))
		{
			if (_stricmp(value, "opaque") == 0)
				return FIXED_FUNCTION_CAPTURE_OPAQUE;
			if (_stricmp(value, "transparent") == 0)
				return FIXED_FUNCTION_CAPTURE_TRANSPARENT;
			if (_stricmp(value, "all") == 0)
				return FIXED_FUNCTION_CAPTURE_ALL;
		}
		return FIXED_FUNCTION_CAPTURE_OPAQUE;
	}

	int ReadOptionalEnvironmentInteger(
		const char* pVariableName,
		int minimum,
		int maximum)
	{
		if (pVariableName == NULL || minimum > maximum)
			return -1;
		char value[16] = "";
		const DWORD length = GetEnvironmentVariableA(
			pVariableName,
			value,
			sizeof(value));
		if (length == 0 || length >= sizeof(value))
			return -1;
		char* pEnd = NULL;
		const long parsedValue = strtol(value, &pEnd, 10);
		return pEnd != value
			&& *pEnd == '\0'
			&& parsedValue >= minimum
			&& parsedValue <= maximum
				? static_cast<int>(parsedValue)
				: -1;
	}

	int ReadFixedFunctionBlendFilter()
	{
		return ReadOptionalEnvironmentInteger(
			"LASTCHAOS_DX12_3D_FIXED_BLEND_MODE",
			0,
			DX12_BLEND_COUNT - 1);
	}

	int ReadFixedFunctionTextureWidthFilter()
	{
		return ReadOptionalEnvironmentInteger(
			"LASTCHAOS_DX12_3D_FIXED_TEXTURE_WIDTH",
			1,
			16384);
	}

	bool MatchesFixedFunctionTextureWidthFilter(
		IDirect3DDevice9* pDevice9,
		int expectedWidth)
	{
		if (expectedWidth < 0)
			return true;
		if (pDevice9 == NULL)
			return false;
		IDirect3DBaseTexture9* pBaseTexture = NULL;
		IDirect3DTexture9* pTexture = NULL;
		D3DSURFACE_DESC description;
		ZeroMemory(&description, sizeof(description));
		const bool matches =
			SUCCEEDED(pDevice9->GetTexture(0, &pBaseTexture))
			&& pBaseTexture != NULL
			&& SUCCEEDED(pBaseTexture->QueryInterface(
				__uuidof(IDirect3DTexture9),
				reinterpret_cast<void**>(&pTexture)))
			&& pTexture != NULL
			&& SUCCEEDED(pTexture->GetLevelDesc(0, &description))
			&& description.Width == static_cast<UINT>(expectedWidth);
		if (pTexture != NULL)
			pTexture->Release();
		if (pBaseTexture != NULL)
			pBaseTexture->Release();
		return matches;
	}

	bool ReadReplacementShaderFamily(
		const char* pEnvironmentVariable,
		UINT64* pFingerprint)
	{
		if (pEnvironmentVariable == NULL || pFingerprint == NULL)
			return false;
		*pFingerprint = 0;
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			pEnvironmentVariable,
			value,
			sizeof(value));
		if (length == 0 || length >= sizeof(value))
			return false;
		if (_stricmp(value, "fixed") == 0)
			return true;
		char* pEnd = NULL;
		const UINT64 fingerprint = _strtoui64(value, &pEnd, 16);
		if (pEnd == value || *pEnd != '\0')
			return false;
		*pFingerprint = fingerprint;
		return true;
	}

	bool ReadReplacementVertexFamily(UINT64* pFingerprint)
	{
		return ReadReplacementShaderFamily(
			"LASTCHAOS_DX12_3D_REPLACE_VERTEX_FAMILY",
			pFingerprint);
	}

	bool ReadReplacementPixelFamily(UINT64* pFingerprint)
	{
		return ReadReplacementShaderFamily(
			"LASTCHAOS_DX12_3D_REPLACE_PIXEL_FAMILY",
			pFingerprint);
	}

	bool ReadExcludedPixelFamily(UINT64* pFingerprint)
	{
		return ReadReplacementShaderFamily(
			"LASTCHAOS_DX12_3D_EXCLUDE_PIXEL_FAMILY",
			pFingerprint);
	}

	bool ReadExcludedVertexFamily(UINT64* pFingerprint)
	{
		return ReadReplacementShaderFamily(
			"LASTCHAOS_DX12_3D_EXCLUDE_VERTEX_FAMILY",
			pFingerprint);
	}

	bool ApplyTestShaderFamilyAlias(
		UINT64 actualVertexFingerprint,
		UINT64 actualPixelFingerprint,
		UINT64* pVertexFingerprint,
		UINT64* pPixelFingerprint)
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_TEST_FAMILY_ALIAS",
			value,
			sizeof(value));
		if (length == 0 || length >= sizeof(value)
			|| (_stricmp(value, "enabled") != 0
				&& strcmp(value, "1") != 0)
			|| pVertexFingerprint == NULL
			|| pPixelFingerprint == NULL)
			return false;

		UINT64 targetVertexFingerprint = 0;
		UINT64 targetPixelFingerprint = 0;
		if (!ReadReplacementVertexFamily(&targetVertexFingerprint)
			|| !ReadReplacementPixelFamily(&targetPixelFingerprint))
			return false;

		const bool rigidNormalMapAlias =
			actualVertexFingerprint == 0x4B5B9BE51A8EFA7EULL
			&& actualPixelFingerprint == 0xB5BD45A8BA08F65BULL
			&& targetVertexFingerprint == 0x3217ECE2D2C1D96AULL
			&& (targetPixelFingerprint == 0xF91A55624E94D8A1ULL
				|| targetPixelFingerprint == 0x5B3BD26F0B904B3DULL);
		const bool skinnedNormalMapAlias =
			actualVertexFingerprint == 0x0BDAEBAB2645C412ULL
			&& actualPixelFingerprint == 0xB5BD45A8BA08F65BULL
			&& targetVertexFingerprint == 0x7873727C8ED9D187ULL
			&& targetPixelFingerprint == 0x77162620F6305229ULL;
		if (!rigidNormalMapAlias && !skinnedNormalMapAlias)
			return false;

		*pVertexFingerprint = targetVertexFingerprint;
		*pPixelFingerprint = targetPixelFingerprint;
		return true;
	}

	int ReadTerrainDebugTexture()
	{
		char value[8] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_TERRAIN_DEBUG_TEXTURE",
			value,
			sizeof(value));
		if (length != 1 || value[0] < '0' || value[0] > '5')
			return -1;
		return value[0] - '0';
	}

	bool ReadOverlayComparisonMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_COMPARE",
			value,
			sizeof(value));
		const bool overlay = length > 0 && length < sizeof(value)
			&& (_stricmp(value, "overlay") == 0
				|| strcmp(value, "1") == 0);
		char replacementValue[32] = "";
		const DWORD replacementLength = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_REPLACE_RIGID_LIT",
			replacementValue,
			sizeof(replacementValue));
		const bool replacement =
			replacementLength > 0
			&& replacementLength < sizeof(replacementValue)
			&& (_stricmp(replacementValue, "enabled") == 0
				|| strcmp(replacementValue, "1") == 0);
		return overlay || replacement || ReadFullReplacementMode();
	}

	bool ReadRigidLitReplacementMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_REPLACE_RIGID_LIT",
			value,
			sizeof(value));
		return length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
	}

	bool ReadStaticCaptureMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_STATIC_CAPTURE",
			value,
			sizeof(value));
		return length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
	}

	bool ReadRigidLitProbeMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_TARGET_RIGID_LIT",
			value,
			sizeof(value));
		return length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
	}

	Native3DCaptureProfile ReadNative3DCaptureProfile()
	{
		// El reemplazo integral necesita capturar continuamente; exigir una
		// segunda variable dejaba REPLACE_ALL activo pero sin comandos nativos.
		if (ReadFullReplacementMode())
			return NATIVE_3D_CAPTURE_CONTINUOUS;

		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_CAPTURE",
			value,
			sizeof(value));
		if (length == 0 || length >= sizeof(value))
			return NATIVE_3D_CAPTURE_OFF;
		if (_stricmp(value, "probe") == 0)
			return NATIVE_3D_CAPTURE_PROBE;
		if (_stricmp(value, "enabled") == 0
			|| strcmp(value, "1") == 0)
			return NATIVE_3D_CAPTURE_CONTINUOUS;
		return NATIVE_3D_CAPTURE_OFF;
	}

	Native3DCaptureBudget GetCaptureBudget(
		Native3DCaptureProfile profile)
	{
		Native3DCaptureBudget budget;
		if (ReadFullReplacementMode())
		{
			budget.maxDraws = FULL_REPLACEMENT_MAX_DRAWS;
			budget.maxVertices = FULL_REPLACEMENT_MAX_VERTICES;
			budget.maxIndices = FULL_REPLACEMENT_MAX_INDICES;
		}
		else if (ReadRigidLitReplacementMode())
		{
			budget.maxDraws = REPLACEMENT_MAX_DRAWS;
			budget.maxVertices = REPLACEMENT_MAX_VERTICES;
			budget.maxIndices = REPLACEMENT_MAX_INDICES;
		}
		else if (profile == NATIVE_3D_CAPTURE_PROBE)
		{
			budget.maxDraws = PROBE_MAX_DRAWS;
			budget.maxVertices = PROBE_MAX_VERTICES;
			budget.maxIndices = PROBE_MAX_INDICES;
		}
		else
		{
			budget.maxDraws = CONTINUOUS_MAX_DRAWS;
			budget.maxVertices = CONTINUOUS_MAX_VERTICES;
			budget.maxIndices = CONTINUOUS_MAX_INDICES;
		}
		return budget;
	}

	DXGI_FORMAT ToDepthStencilViewFormat(DXGI_FORMAT format)
	{
		if (format == DXGI_FORMAT_R24G8_TYPELESS)
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		if (format == DXGI_FORMAT_R32_TYPELESS)
			return DXGI_FORMAT_D32_FLOAT;
		if (format == DXGI_FORMAT_R16_TYPELESS)
			return DXGI_FORMAT_D16_UNORM;
		return format;
	}

	void ReleaseTexture(IDirect3DTexture9*& pTexture)
	{
		if (pTexture != NULL)
		{
			pTexture->Release();
			pTexture = NULL;
		}
	}

	bool AcquireTexture(
		CDirectX12InteropTextureManager* pTextures,
		DirectX12TextureHandle textureHandle,
		DirectX12RenderTextureHandle renderTextureHandle,
		IDirect3DTexture9* pLegacyTexture,
		ID3D12GraphicsCommandList* pCommandList,
		CDirectX12UploadManager* pUploadManager,
		D3D12_GPU_DESCRIPTOR_HANDLE* pView)
	{
		if (renderTextureHandle.IsValid())
			return pTextures->Acquire(
				renderTextureHandle,
				pCommandList,
				pView);
		if (textureHandle.IsValid())
			return pTextures->Acquire(
				textureHandle,
				pCommandList,
				pView);
		return pTextures->Acquire(
			pLegacyTexture,
			pCommandList,
			pUploadManager,
			pView);
	}

	bool ReferencesRenderTarget(
		CDirectX12InteropTextureManager* pTextures,
		DirectX12RenderTextureHandle renderTextureHandle,
		IDirect3DTexture9* pLegacyTexture,
		ID3D12Resource* pResource)
	{
		return renderTextureHandle.IsValid()
			? pTextures->ReferencesResource(
				renderTextureHandle,
				pResource)
			: pTextures->ReferencesResource(
				pLegacyTexture,
				pResource);
	}
}

struct DirectX12Legacy3DCommandBatchState
{
	std::vector<FLOAT> positions;
	std::vector<FLOAT> texCoords[4];
	std::vector<FLOAT> projectiveTexCoordQ[4];
	std::vector<FLOAT> normals;
	std::vector<BYTE> weights;
	std::vector<FLOAT> tangents;
	std::vector<ULONG> colors;
	std::vector<Legacy3DVertex> vertices;
	std::vector<UINT> indices;
	std::vector<Legacy3DDrawRange> ranges;
	std::vector<Legacy3DVertexShaderFamily> vertexShaderFamilies;
	std::vector<Legacy3DShaderPairInventory> shaderPairInventory;
	std::vector<Legacy3DClipDiagnosticKey> clipDiagnostics;
	std::vector<Legacy3DFixedFunctionInventory> fixedFunctionInventory;
	std::vector<Legacy3DVertexShaderCacheEntry> vertexShaderCache;
	std::vector<Legacy3DPixelShaderCacheEntry> pixelShaderCache;
	size_t submittedRangeCount;
	UINT capturedDrawCount;
	UINT rejectedDrawCount;
	UINT capturedTriangleCount;
	UINT rejectedReasons[
		CDirectX12Legacy3DCommandBatch::REJECT_REASON_COUNT];
	ULONG constantColor;
	bool staticPositionSelected;
	bool staticTexCoordSelected;
	bool staticNormalSelected;
	UINT fixedDiagnosticCount;
	bool fixedBlendDiagnosticReported[DX12_BLEND_COUNT];
	bool missingArrayDiagnosticReported;
	bool testFamilyAliasDiagnosticReported;
	bool terrainColorPendingAfterDepthMask;
	UINT terrainDepthMaskVertexCount;
	bool terrainDepthMaskReported;
	bool terrainOpaqueInitializationReported;
	bool terrainCompatibilityFallbackReported;
	bool fixedGeneralFallbackReported;
	ID3D12Resource* pNativeDepthTarget;
	UINT64 frameSerial;
	UINT64 lastInventoryDumpFrame;

	DirectX12Legacy3DCommandBatchState()
		: submittedRangeCount(0)
		, capturedDrawCount(0)
		, rejectedDrawCount(0)
		, capturedTriangleCount(0)
		, constantColor(C_WHITE | CT_OPAQUE)
		, staticPositionSelected(false)
		, staticTexCoordSelected(false)
		, staticNormalSelected(false)
		, fixedDiagnosticCount(0)
		, missingArrayDiagnosticReported(false)
		, testFamilyAliasDiagnosticReported(false)
		, terrainColorPendingAfterDepthMask(false)
		, terrainDepthMaskVertexCount(0)
		, terrainDepthMaskReported(false)
		, terrainOpaqueInitializationReported(false)
		, terrainCompatibilityFallbackReported(false)
		, fixedGeneralFallbackReported(false)
		, pNativeDepthTarget(NULL)
		, frameSerial(0)
		, lastInventoryDumpFrame(0)
	{
		ZeroMemory(rejectedReasons, sizeof(rejectedReasons));
		ZeroMemory(
			fixedBlendDiagnosticReported,
			sizeof(fixedBlendDiagnosticReported));
	}
};

namespace
{
	bool DetectTerrainDepthMask(
		DirectX12Legacy3DCommandBatchState* pState,
		IDirect3DDevice9* pDevice9)
	{
		if (pState == NULL || pDevice9 == NULL)
			return false;
		DWORD colorWriteMask = 0;
		DWORD depthWrite = FALSE;
		DWORD alphaTest = FALSE;
		if (FAILED(pDevice9->GetRenderState(
				D3DRS_COLORWRITEENABLE,
				&colorWriteMask))
			|| FAILED(pDevice9->GetRenderState(
				D3DRS_ZWRITEENABLE,
				&depthWrite))
			|| FAILED(pDevice9->GetRenderState(
				D3DRS_ALPHATESTENABLE,
				&alphaTest))
			|| colorWriteMask != 0
			|| depthWrite == FALSE
			|| alphaTest == FALSE)
			return false;

		pState->terrainColorPendingAfterDepthMask = true;
		pState->terrainDepthMaskVertexCount =
			static_cast<UINT>(pState->positions.size() / 3);
		if (!pState->terrainDepthMaskReported)
		{
			CPrintF(
				"DX12 diagnostico terreno: mascara de profundidad "
				"detectada, vertices=%u; la siguiente capa nativa "
				"inicializara el color.\n",
				pState->terrainDepthMaskVertexCount);
			pState->terrainDepthMaskReported = true;
		}
		return true;
	}

	UINT64 GetVertexShaderFingerprint(
		DirectX12Legacy3DCommandBatchState* pState,
		IDirect3DDevice9* pDevice9)
	{
		IDirect3DVertexShader9* pShader = NULL;
		if (pState == NULL || pDevice9 == NULL
			|| FAILED(pDevice9->GetVertexShader(&pShader))
			|| pShader == NULL)
			return 0;

		for (size_t iEntry = 0;
			iEntry < pState->vertexShaderCache.size();
			++iEntry)
		{
			if (pState->vertexShaderCache[iEntry].pShader == pShader)
			{
				const UINT64 fingerprint =
					pState->vertexShaderCache[iEntry].fingerprint;
				pShader->Release();
				return fingerprint;
			}
		}

		UINT64 fingerprint = 14695981039346656037ULL;
		UINT byteCount = 0;
		std::vector<BYTE> bytecode;
		if (SUCCEEDED(pShader->GetFunction(NULL, &byteCount))
			&& byteCount > 0)
		{
			bytecode.resize(byteCount);
			if (SUCCEEDED(pShader->GetFunction(
				&bytecode[0],
				&byteCount)))
			{
				fingerprint = HashBytes(
					fingerprint,
					&bytecode[0],
					byteCount);
			}
		}

		IDirect3DVertexDeclaration9* pDeclaration = NULL;
		if (SUCCEEDED(pDevice9->GetVertexDeclaration(&pDeclaration))
			&& pDeclaration != NULL)
		{
			UINT elementCount = 0;
			if (SUCCEEDED(pDeclaration->GetDeclaration(
				NULL,
				&elementCount))
				&& elementCount > 0)
			{
				std::vector<D3DVERTEXELEMENT9> elements(elementCount);
				if (SUCCEEDED(pDeclaration->GetDeclaration(
					&elements[0],
					&elementCount)))
				{
					fingerprint = HashBytes(
						fingerprint,
						&elements[0],
						elementCount * sizeof(D3DVERTEXELEMENT9));
				}
			}
			pDeclaration->Release();
		}
		if (!bytecode.empty())
			DumpVertexShader(
				fingerprint,
				&bytecode[0],
				byteCount,
				"vso");

		Legacy3DVertexShaderCacheEntry cacheEntry;
		cacheEntry.pShader = pShader;
		cacheEntry.fingerprint = fingerprint;
		pState->vertexShaderCache.push_back(cacheEntry);
		pShader->Release();
		return fingerprint;
	}

	UINT64 GetPixelShaderFingerprint(
		DirectX12Legacy3DCommandBatchState* pState,
		IDirect3DDevice9* pDevice9)
	{
		IDirect3DPixelShader9* pShader = NULL;
		if (pState == NULL || pDevice9 == NULL
			|| FAILED(pDevice9->GetPixelShader(&pShader))
			|| pShader == NULL)
			return 0;
		for (size_t iEntry = 0;
			iEntry < pState->pixelShaderCache.size();
			++iEntry)
		{
			if (pState->pixelShaderCache[iEntry].pShader == pShader)
			{
				const UINT64 fingerprint =
					pState->pixelShaderCache[iEntry].fingerprint;
				pShader->Release();
				return fingerprint;
			}
		}
		UINT byteCount = 0;
		UINT64 fingerprint = 14695981039346656037ULL;
		if (SUCCEEDED(pShader->GetFunction(NULL, &byteCount))
			&& byteCount > 0)
		{
			std::vector<BYTE> bytecode(byteCount);
			if (SUCCEEDED(pShader->GetFunction(&bytecode[0], &byteCount)))
			{
				fingerprint = HashBytes(
					fingerprint,
					&bytecode[0],
					byteCount);
				DumpVertexShader(
					fingerprint,
					&bytecode[0],
					byteCount,
					"pso");
			}
		}
		Legacy3DPixelShaderCacheEntry cacheEntry;
		cacheEntry.pShader = pShader;
		cacheEntry.fingerprint = fingerprint;
		pState->pixelShaderCache.push_back(cacheEntry);
		pShader->Release();
		return fingerprint;
	}

	bool ReadShaderInventoryMode()
	{
		char value[32] = "";
		const DWORD length = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_3D_INVENTORY",
			value,
			sizeof(value));
		return length > 0 && length < sizeof(value)
			&& (_stricmp(value, "enabled") == 0
				|| strcmp(value, "1") == 0);
	}

	void RecordShaderPair(
		DirectX12Legacy3DCommandBatchState* pState,
		UINT64 vertexFingerprint,
		UINT64 pixelFingerprint,
		UINT texturePassCount,
		bool dynamicBuffer,
		bool projectiveMapping,
		UINT indexCount)
	{
		for (size_t iPair = 0;
			iPair < pState->shaderPairInventory.size();
			++iPair)
		{
			Legacy3DShaderPairInventory& pair =
				pState->shaderPairInventory[iPair];
			if (pair.vertexFingerprint == vertexFingerprint
				&& pair.pixelFingerprint == pixelFingerprint
				&& pair.texturePassCount == texturePassCount
				&& pair.dynamicBuffer == dynamicBuffer
				&& pair.projectiveMapping == projectiveMapping)
			{
				++pair.drawCount;
				pair.triangleCount += indexCount / 3;
				return;
			}
		}
		Legacy3DShaderPairInventory pair;
		pair.vertexFingerprint = vertexFingerprint;
		pair.pixelFingerprint = pixelFingerprint;
		pair.texturePassCount = texturePassCount;
		pair.dynamicBuffer = dynamicBuffer;
		pair.projectiveMapping = projectiveMapping;
		pair.drawCount = 1;
		pair.triangleCount = indexCount / 3;
		pState->shaderPairInventory.push_back(pair);
	}

	void ReportTerrainClipStateOnce(
		DirectX12Legacy3DCommandBatchState* pState,
		UINT64 vertexFingerprint,
		UINT64 pixelFingerprint,
		bool legacyClippingEnabled,
		bool depthClipEnabled,
		bool depthEnabled,
		bool depthWriteEnabled,
		bool colorWriteEnabled,
		D3D12_COMPARISON_FUNC depthFunction,
		bool alphaTestEnabled,
		FLOAT alphaReference,
		DWORD blending,
		DWORD sourceBlend,
		DWORD destinationBlend,
		DWORD samplerAddressU,
		DWORD samplerAddressV,
		DWORD samplerMinification,
		DWORD samplerMagnification,
		const FLOAT* pPixelConstants,
		IDirect3DTexture9* pTexture,
		IDirect3DTexture9* pTexture1,
		IDirect3DTexture9* pTexture2,
		IDirect3DTexture9* pTexture3,
		const std::vector<Legacy3DVertex>& vertices,
		UINT baseVertex,
		UINT vertexCount,
		UINT clampedFarDepthCount)
	{
		if (pState == NULL || vertexCount == 0)
			return;
		for (size_t iKey = 0; iKey < pState->clipDiagnostics.size(); ++iKey)
		{
			const Legacy3DClipDiagnosticKey& key =
				pState->clipDiagnostics[iKey];
			if (key.vertexFingerprint == vertexFingerprint
				&& key.pixelFingerprint == pixelFingerprint
				&& key.clippingEnabled == depthClipEnabled)
				return;
		}

		FLOAT minimumDepth = FLT_MAX;
		FLOAT maximumDepth = -FLT_MAX;
		FLOAT minimumNdcX = FLT_MAX;
		FLOAT maximumNdcX = -FLT_MAX;
		FLOAT minimumNdcY = FLT_MAX;
		FLOAT maximumNdcY = -FLT_MAX;
		UINT beyondFarPlane = 0;
		UINT behindCamera = 0;
		UINT64 positionFingerprint = 14695981039346656037ULL;
		FLOAT minimumU[4] = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };
		FLOAT maximumU[4] = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
		FLOAT minimumV[4] = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };
		FLOAT maximumV[4] = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
		for (UINT iVertex = 0; iVertex < vertexCount; ++iVertex)
		{
			const Legacy3DVertex& vertex = vertices[baseVertex + iVertex];
			for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
			{
				const FLOAT* pTexCoord = textureUnit == 0
					? vertex.texCoord
					: vertex.texCoordExtra[textureUnit - 1];
				minimumU[textureUnit] =
					(std::min)(minimumU[textureUnit], pTexCoord[0]);
				maximumU[textureUnit] =
					(std::max)(maximumU[textureUnit], pTexCoord[0]);
				minimumV[textureUnit] =
					(std::min)(minimumV[textureUnit], pTexCoord[1]);
				maximumV[textureUnit] =
					(std::max)(maximumV[textureUnit], pTexCoord[1]);
			}
			positionFingerprint = HashBytes(
				positionFingerprint,
				vertex.position,
				sizeof(vertex.position));
			positionFingerprint = HashBytes(
				positionFingerprint,
				&vertex.clipW,
				sizeof(vertex.clipW));
			if (vertex.clipW <= 0.0f)
			{
				++behindCamera;
				continue;
			}
			const FLOAT normalizedDepth =
				vertex.position[2] / vertex.clipW;
			const FLOAT normalizedX =
				vertex.position[0] / vertex.clipW;
			const FLOAT normalizedY =
				vertex.position[1] / vertex.clipW;
			minimumNdcX = (std::min)(minimumNdcX, normalizedX);
			maximumNdcX = (std::max)(maximumNdcX, normalizedX);
			minimumNdcY = (std::min)(minimumNdcY, normalizedY);
			maximumNdcY = (std::max)(maximumNdcY, normalizedY);
			minimumDepth = (std::min)(minimumDepth, normalizedDepth);
			maximumDepth = (std::max)(maximumDepth, normalizedDepth);
			if (normalizedDepth > 1.0f)
				++beyondFarPlane;
		}
		D3DSURFACE_DESC textureDescription;
		ZeroMemory(&textureDescription, sizeof(textureDescription));
		const bool hasTextureDescription =
			pTexture != NULL
			&& SUCCEEDED(pTexture->GetLevelDesc(0, &textureDescription));
		D3DSURFACE_DESC extraTextureDescriptions[3];
		ZeroMemory(
			extraTextureDescriptions,
			sizeof(extraTextureDescriptions));
		IDirect3DTexture9* extraTextures[3] = {
			pTexture1, pTexture2, pTexture3
		};
		for (UINT iTexture = 0; iTexture < 3; ++iTexture)
		{
			if (extraTextures[iTexture] != NULL)
				extraTextures[iTexture]->GetLevelDesc(
					0,
					&extraTextureDescriptions[iTexture]);
		}

		CPrintF(
			"DX12 diagnostico terreno: pareja=%016llX/%016llX, "
			"D3DRS_CLIPPING=%u, DepthClipEnable=%u, "
			"z=%u/%u/%u, color=%u, posiciones=%016llX, "
			"alphaTest=%u/ref=%.6f, blend=%u/%u/%u, "
			"sampler=%u/%u/%u/%u, "
			"c7=(%.6f,%.6f,%.6f,%.6f), "
			"uv0=(%.6f..%.6f,%.6f..%.6f), "
			"uv1=(%.6f..%.6f,%.6f..%.6f), "
			"uv2=(%.6f..%.6f,%.6f..%.6f), "
			"uv3=(%.6f..%.6f,%.6f..%.6f), "
			"tex=%p/%ux%u/f%d/p%d, "
			"tex1=%p/%ux%u/f%d, tex2=%p/%ux%u/f%d, "
			"tex3=%p/%ux%u/f%d, "
			"ndc=(%.6f..%.6f,%.6f..%.6f), "
			"z/w=%.8f..%.8f, fueraLejano=%u/%u, "
			"corregidosLejano=%u, detrasCamara=%u.\n",
			static_cast<unsigned long long>(vertexFingerprint),
			static_cast<unsigned long long>(pixelFingerprint),
			legacyClippingEnabled ? 1U : 0U,
			depthClipEnabled ? 1U : 0U,
			depthEnabled ? 1U : 0U,
			depthWriteEnabled ? 1U : 0U,
			static_cast<UINT>(depthFunction),
			colorWriteEnabled ? 1U : 0U,
			static_cast<unsigned long long>(positionFingerprint),
			alphaTestEnabled ? 1U : 0U,
			alphaReference,
			blending,
			sourceBlend,
			destinationBlend,
			samplerAddressU,
			samplerAddressV,
			samplerMinification,
			samplerMagnification,
			pPixelConstants[7 * 4 + 0],
			pPixelConstants[7 * 4 + 1],
			pPixelConstants[7 * 4 + 2],
			pPixelConstants[7 * 4 + 3],
			minimumU[0], maximumU[0], minimumV[0], maximumV[0],
			minimumU[1], maximumU[1], minimumV[1], maximumV[1],
			minimumU[2], maximumU[2], minimumV[2], maximumV[2],
			minimumU[3], maximumU[3], minimumV[3], maximumV[3],
			pTexture,
			hasTextureDescription ? textureDescription.Width : 0,
			hasTextureDescription ? textureDescription.Height : 0,
			hasTextureDescription
				? static_cast<int>(textureDescription.Format) : 0,
			hasTextureDescription
				? static_cast<int>(textureDescription.Pool) : 0,
			pTexture1,
			extraTextureDescriptions[0].Width,
			extraTextureDescriptions[0].Height,
			static_cast<int>(extraTextureDescriptions[0].Format),
			pTexture2,
			extraTextureDescriptions[1].Width,
			extraTextureDescriptions[1].Height,
			static_cast<int>(extraTextureDescriptions[1].Format),
			pTexture3,
			extraTextureDescriptions[2].Width,
			extraTextureDescriptions[2].Height,
			static_cast<int>(extraTextureDescriptions[2].Format),
			minimumNdcX,
			maximumNdcX,
			minimumNdcY,
			maximumNdcY,
			minimumDepth,
			maximumDepth,
			beyondFarPlane,
			vertexCount,
			clampedFarDepthCount,
			behindCamera);

		Legacy3DClipDiagnosticKey key;
		key.vertexFingerprint = vertexFingerprint;
		key.pixelFingerprint = pixelFingerprint;
		key.clippingEnabled = depthClipEnabled;
		pState->clipDiagnostics.push_back(key);
	}

	void RecordFixedFunctionDraw(
		DirectX12Legacy3DCommandBatchState* pState,
		IDirect3DTexture9* pTexture,
		const D3DSURFACE_DESC* pTextureDescription,
		UINT texturePassCount,
		DirectX12BlendMode blendMode,
		DWORD zEnable,
		DWORD zWrite,
		DWORD zFunction,
		DWORD cullMode,
		DWORD texCoordIndex,
		DWORD textureTransformFlags,
		DWORD colorOperation,
		DWORD alphaOperation,
		bool dynamicBuffer,
		UINT indexCount,
		const std::vector<Legacy3DVertex>& vertices,
		UINT baseVertex,
		UINT vertexCount)
	{
		if (pState == NULL || vertexCount == 0)
			return;
		const UINT64 textureIdentity =
			static_cast<UINT64>(reinterpret_cast<ULONG_PTR>(pTexture));
		const UINT textureWidth =
			pTextureDescription != NULL ? pTextureDescription->Width : 0;
		const UINT textureHeight =
			pTextureDescription != NULL ? pTextureDescription->Height : 0;
		const INT textureFormat = pTextureDescription != NULL
			? static_cast<INT>(pTextureDescription->Format)
			: 0;
		for (size_t iEntry = 0;
			iEntry < pState->fixedFunctionInventory.size();
			++iEntry)
		{
			Legacy3DFixedFunctionInventory& entry =
				pState->fixedFunctionInventory[iEntry];
			if (entry.textureIdentity == textureIdentity
				&& entry.texturePassCount == texturePassCount
				&& entry.blendMode == static_cast<UINT>(blendMode)
				&& entry.depthEnabled == (zEnable != FALSE ? 1U : 0U)
				&& entry.depthWriteEnabled
					== (zWrite != FALSE ? 1U : 0U)
				&& entry.depthFunction == zFunction
				&& entry.cullMode == cullMode
				&& entry.texCoordIndex == texCoordIndex
				&& entry.textureTransformFlags == textureTransformFlags
				&& entry.colorOperation == colorOperation
				&& entry.alphaOperation == alphaOperation
				&& entry.dynamicBuffer == dynamicBuffer)
			{
				++entry.drawCount;
				entry.triangleCount += indexCount / 3;
				return;
			}
		}

		const Legacy3DVertex& firstVertex = vertices[baseVertex];
		Legacy3DFixedFunctionInventory entry;
		entry.textureIdentity = textureIdentity;
		entry.textureWidth = textureWidth;
		entry.textureHeight = textureHeight;
		entry.textureFormat = textureFormat;
		entry.texturePassCount = texturePassCount;
		entry.blendMode = static_cast<UINT>(blendMode);
		entry.depthEnabled = zEnable != FALSE ? 1U : 0U;
		entry.depthWriteEnabled = zWrite != FALSE ? 1U : 0U;
		entry.depthFunction = zFunction;
		entry.cullMode = cullMode;
		entry.texCoordIndex = texCoordIndex;
		entry.textureTransformFlags = textureTransformFlags;
		entry.colorOperation = colorOperation;
		entry.alphaOperation = alphaOperation;
		entry.dynamicBuffer = dynamicBuffer;
		entry.drawCount = 1;
		entry.triangleCount = indexCount / 3;
		entry.minimumX = entry.maximumX = firstVertex.position[0];
		entry.minimumY = entry.maximumY = firstVertex.position[1];
		entry.minimumZ = entry.maximumZ = firstVertex.position[2];
		entry.minimumW = entry.maximumW = firstVertex.clipW;
		for (UINT iVertex = 1; iVertex < vertexCount; ++iVertex)
		{
			const Legacy3DVertex& vertex = vertices[baseVertex + iVertex];
			entry.minimumX = (std::min)(entry.minimumX, vertex.position[0]);
			entry.maximumX = (std::max)(entry.maximumX, vertex.position[0]);
			entry.minimumY = (std::min)(entry.minimumY, vertex.position[1]);
			entry.maximumY = (std::max)(entry.maximumY, vertex.position[1]);
			entry.minimumZ = (std::min)(entry.minimumZ, vertex.position[2]);
			entry.maximumZ = (std::max)(entry.maximumZ, vertex.position[2]);
			entry.minimumW = (std::min)(entry.minimumW, vertex.clipW);
			entry.maximumW = (std::max)(entry.maximumW, vertex.clipW);
		}
		pState->fixedFunctionInventory.push_back(entry);
	}

	void DumpShaderPairInventory(
		DirectX12Legacy3DCommandBatchState* pState)
	{
		if (pState == NULL || !ReadShaderInventoryMode()
			|| pState->shaderPairInventory.empty())
			return;
		char directory[MAX_PATH] = "";
		const DWORD directoryLength = GetEnvironmentVariableA(
			"LASTCHAOS_DX12_SHADER_DUMP_DIR",
			directory,
			sizeof(directory));
		if (directoryLength == 0 || directoryLength >= sizeof(directory))
			return;
		CreateDirectoryA(directory, NULL);
		char inventoryPath[MAX_PATH] = "";
		_snprintf_s(
			inventoryPath,
			sizeof(inventoryPath),
			_TRUNCATE,
			"%s\\shader-pairs.csv",
			directory);
		HANDLE hFile = CreateFileA(
			inventoryPath,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return;
		const char* pHeader =
			"vertex_shader,pixel_shader,texture_passes,dynamic,"
			"projective,draws,triangles\r\n";
		DWORD written = 0;
		WriteFile(
			hFile,
			pHeader,
			static_cast<DWORD>(strlen(pHeader)),
			&written,
			NULL);
		for (size_t iPair = 0;
			iPair < pState->shaderPairInventory.size();
			++iPair)
		{
			const Legacy3DShaderPairInventory& pair =
				pState->shaderPairInventory[iPair];
			char line[256] = "";
			_snprintf_s(
				line,
				sizeof(line),
				_TRUNCATE,
				"%016llX,%016llX,%u,%u,%u,%u,%u\r\n",
				static_cast<unsigned long long>(
					pair.vertexFingerprint),
				static_cast<unsigned long long>(
					pair.pixelFingerprint),
				pair.texturePassCount,
				pair.dynamicBuffer ? 1U : 0U,
				pair.projectiveMapping ? 1U : 0U,
				pair.drawCount,
				pair.triangleCount);
			WriteFile(
				hFile,
				line,
				static_cast<DWORD>(strlen(line)),
				&written,
				NULL);
		}
		CloseHandle(hFile);

		char fixedInventoryPath[MAX_PATH] = "";
		_snprintf_s(
			fixedInventoryPath,
			sizeof(fixedInventoryPath),
			_TRUNCATE,
			"%s\\fixed-function.csv",
			directory);
		hFile = CreateFileA(
			fixedInventoryPath,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			const char* pFixedHeader =
				"texture,width,height,format,texture_passes,blend,"
				"depth_enabled,depth_write,depth_function,cull,"
				"texcoord_index,texture_transform,color_operation,"
				"alpha_operation,dynamic,draws,triangles,"
				"min_x,max_x,min_y,max_y,min_z,max_z,min_w,max_w\r\n";
			WriteFile(
				hFile,
				pFixedHeader,
				static_cast<DWORD>(strlen(pFixedHeader)),
				&written,
				NULL);
			for (size_t iEntry = 0;
				iEntry < pState->fixedFunctionInventory.size();
				++iEntry)
			{
				const Legacy3DFixedFunctionInventory& entry =
					pState->fixedFunctionInventory[iEntry];
				char line[512] = "";
				_snprintf_s(
					line,
					sizeof(line),
					_TRUNCATE,
					"%016llX,%u,%u,%d,%u,%u,%u,%u,%u,%u,"
					"%08X,%08X,%u,%u,%u,%u,%u,"
					"%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\r\n",
					static_cast<unsigned long long>(
						entry.textureIdentity),
					entry.textureWidth,
					entry.textureHeight,
					entry.textureFormat,
					entry.texturePassCount,
					entry.blendMode,
					entry.depthEnabled,
					entry.depthWriteEnabled,
					entry.depthFunction,
					entry.cullMode,
					entry.texCoordIndex,
					entry.textureTransformFlags,
					entry.colorOperation,
					entry.alphaOperation,
					entry.dynamicBuffer ? 1U : 0U,
					entry.drawCount,
					entry.triangleCount,
					entry.minimumX,
					entry.maximumX,
					entry.minimumY,
					entry.maximumY,
					entry.minimumZ,
					entry.maximumZ,
					entry.minimumW,
					entry.maximumW);
				WriteFile(
					hFile,
					line,
					static_cast<DWORD>(strlen(line)),
					&written,
					NULL);
			}
			CloseHandle(hFile);
		}
		pState->lastInventoryDumpFrame = pState->frameSerial;
	}

	void RecordVertexShaderFamily(
		DirectX12Legacy3DCommandBatchState* pState,
		IDirect3DDevice9* pDevice9,
		UINT indexCount)
	{
		const UINT64 fingerprint =
			GetVertexShaderFingerprint(pState, pDevice9);
		if (fingerprint == 0)
			return;
		for (size_t iFamily = 0;
			iFamily < pState->vertexShaderFamilies.size();
			++iFamily)
		{
			Legacy3DVertexShaderFamily& family =
				pState->vertexShaderFamilies[iFamily];
			if (family.fingerprint == fingerprint)
			{
				++family.drawCount;
				family.triangleCount += indexCount / 3;
				return;
			}
		}
		Legacy3DVertexShaderFamily family;
		family.fingerprint = fingerprint;
		family.drawCount = 1;
		family.triangleCount = indexCount / 3;
		pState->vertexShaderFamilies.push_back(family);
	}

	const Legacy3DVertexShaderFamily* GetTopVertexShaderFamily(
		const DirectX12Legacy3DCommandBatchState* pState)
	{
		const Legacy3DVertexShaderFamily* pTop = NULL;
		for (size_t iFamily = 0;
			pState != NULL
				&& iFamily < pState->vertexShaderFamilies.size();
			++iFamily)
		{
			const Legacy3DVertexShaderFamily& family =
				pState->vertexShaderFamilies[iFamily];
			if (pTop == NULL || family.drawCount > pTop->drawCount)
				pTop = &family;
		}
		return pTop;
	}

	FLOAT Dot3(const GfxVector3& vector, const FLOAT* pConstant)
	{
		return vector.x * pConstant[0]
			+ vector.y * pConstant[1]
			+ vector.z * pConstant[2];
	}

	GfxVector3 TransformPosition3x4(
		const GfxVector3& position,
		const FLOAT* pConstants)
	{
		return GfxVector3(
			Dot3(position, pConstants + 0) + pConstants[3],
			Dot3(position, pConstants + 4) + pConstants[7],
			Dot3(position, pConstants + 8) + pConstants[11]);
	}

	GfxVector3 TransformDirection3x3(
		const GfxVector3& direction,
		const FLOAT* pConstants)
	{
		return GfxVector3(
			Dot3(direction, pConstants + 0),
			Dot3(direction, pConstants + 4),
			Dot3(direction, pConstants + 8));
	}

	GfxVector3 NormalizeSafe(const GfxVector3& vector)
	{
		const FLOAT lengthSquared =
			vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
		if (lengthSquared <= 0.0000001f)
			return GfxVector3(0.0f, 0.0f, 1.0f);
		const FLOAT inverseLength = 1.0f / sqrtf(lengthSquared);
		return vector * inverseLength;
	}

	void SkinVertex(
		const GfxVector3& sourcePosition,
		const GfxVector3& sourceNormal,
		const GfxVector3& sourceTangent,
		const BYTE* pSkinData,
		const FLOAT* pConstants,
		GfxVector3* pPosition,
		GfxVector3* pNormal,
		GfxVector3* pTangent)
	{
		*pPosition = GfxVector3(0.0f, 0.0f, 0.0f);
		*pNormal = GfxVector3(0.0f, 0.0f, 0.0f);
		*pTangent = GfxVector3(0.0f, 0.0f, 0.0f);
		FLOAT weights[4] = {
			pSkinData[6] / 255.0f,
			pSkinData[5] / 255.0f,
			pSkinData[4] / 255.0f,
			0.0f
		};
		const UINT sourceIndices[4] = { 2, 1, 0, 3 };
		weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
		for (UINT influence = 0; influence < 4; ++influence)
		{
			const UINT constantIndex =
				21 + static_cast<UINT>(
					pSkinData[sourceIndices[influence]]) * 3;
			if (constantIndex + 2 >= 96)
				continue;
			const FLOAT* pBone = pConstants + constantIndex * 4;
			*pPosition += TransformPosition3x4(
				sourcePosition,
				pBone) * weights[influence];
			*pNormal += TransformDirection3x3(
				sourceNormal,
				pBone) * weights[influence];
			*pTangent += TransformDirection3x3(
				sourceTangent,
				pBone) * weights[influence];
		}
	}

	void WriteClipPosition(
		const GfxVector3& position,
		const FLOAT* pConstants,
		Legacy3DVertex* pVertex)
	{
		const FLOAT x = Dot3(position, pConstants + 0)
			+ pConstants[3];
		const FLOAT y = Dot3(position, pConstants + 4)
			+ pConstants[7];
		const FLOAT z = Dot3(position, pConstants + 8)
			+ pConstants[11];
		const FLOAT w = Dot3(position, pConstants + 12)
			+ pConstants[15];
		// Conservamos XYZ en espacio clip. Dividir por W en CPU pierde
		// precisión cerca del plano de cámara y puede generar coordenadas no
		// finitas antes de que el rasterizador recorte la geometría.
		pVertex->position[0] = x;
		pVertex->position[1] = y;
		pVertex->position[2] = z;
		pVertex->clipW = w;
	}

	bool IsProjectedTerrainVertexFamily(
		DirectX12LegacyVertexFamily family)
	{
		return family == DX12_LEGACY_VS_PROJECTED_ONE
			|| family == DX12_LEGACY_VS_PROJECTED_TWO
			|| family == DX12_LEGACY_VS_PROJECTED_FOUR;
	}

	void CaptureProjectedTerrainCamera(IDirect3DDevice9* pDevice9)
	{
		if (pDevice9 == NULL)
			return;
		const UINT TERRAIN_VERTEX_CONSTANT_COUNT = 96;
		FLOAT vertexConstants[TERRAIN_VERTEX_CONSTANT_COUNT * 4];
		if (SUCCEEDED(pDevice9->GetVertexShaderConstantF(
				0,
				vertexConstants,
				TERRAIN_VERTEX_CONSTANT_COUNT)))
		{
			CCameraTestCapture::CaptureTerrainView(
				pDevice9,
				vertexConstants,
				TERRAIN_VERTEX_CONSTANT_COUNT);
		}
	}

	bool ClampProjectedTerrainFarDepth(Legacy3DVertex* pVertex)
	{
		if (pVertex == NULL
			|| pVertex->clipW <= 0.0f
			|| pVertex->position[2] <= pVertex->clipW)
			return false;
		// El vertex shader D3D9 original deja algunos vértices del terreno
		// unas diezmilésimas más allá del plano lejano. En DX12 esos
		// fragmentos fallan LESS_EQUAL aunque DepthClipEnable esté
		// desactivado. Llevar sólo esos vértices a z=w conserva la superficie
		// sin alterar X/Y ni la cámara.
		pVertex->position[2] = pVertex->clipW;
		return true;
	}

	void WriteLighting(
		const GfxVector3& normal,
		const FLOAT* pConstants,
		bool useDiffuseAlpha,
		Legacy3DVertex* pVertex)
	{
		const GfxVector3 normalized = NormalizeSafe(normal);
		const FLOAT lighting = max(
			pConstants[7 * 4 + 0],
			min(pConstants[7 * 4 + 1],
				Dot3(normalized, pConstants + 4 * 4)));
		for (UINT component = 0; component < 3; ++component)
		{
			pVertex->color[component] = min(
				1.0f,
				pConstants[5 * 4 + component] * lighting
					+ pConstants[6 * 4 + component]);
		}
		pVertex->color[3] = useDiffuseAlpha
			? pConstants[5 * 4 + 3]
			: pConstants[7 * 4 + 1];
	}

	void WriteProjectedTexCoord(
		const GfxVector3& position,
		const FLOAT* pConstants,
		UINT firstConstant,
		UINT destinationUnit,
		Legacy3DVertex* pVertex)
	{
		FLOAT* pDestination = destinationUnit == 0
			? pVertex->texCoord
			: pVertex->texCoordExtra[destinationUnit - 1];
		pDestination[0] =
			Dot3(position, pConstants + firstConstant * 4)
			+ pConstants[(firstConstant + 2) * 4 + 0];
		pDestination[1] =
			Dot3(position, pConstants + (firstConstant + 1) * 4)
			+ pConstants[(firstConstant + 2) * 4 + 1];
		pDestination[0] *= pConstants[(firstConstant + 2) * 4 + 2];
		pDestination[1] *= pConstants[(firstConstant + 2) * 4 + 3];
	}

	void WriteHomogeneousProjectedTexCoord(
		const GfxVector3& position,
		const FLOAT* pConstants,
		UINT firstConstant,
		UINT destinationUnit,
		Legacy3DVertex* pVertex)
	{
		FLOAT projected[4];
		for (UINT component = 0; component < 4; ++component)
		{
			const FLOAT* pRow =
				pConstants + (firstConstant + component) * 4;
			projected[component] =
				Dot3(position, pRow) + pRow[3];
		}
		const FLOAT inverseQ = fabsf(projected[3]) > 0.000001f
			? 1.0f / projected[3]
			: 1.0f;
		FLOAT* pDestination = destinationUnit == 0
			? pVertex->texCoord
			: pVertex->texCoordExtra[destinationUnit - 1];
		const FLOAT u = projected[0] * inverseQ;
		const FLOAT v = projected[1] * inverseQ;
		pDestination[0] = _finite(u) ? u : 0.5f;
		pDestination[1] = _finite(v) ? v : 0.5f;
	}

	void WritePlanarTexCoord(
		const GfxVector3& position,
		const FLOAT* pConstants,
		UINT firstConstant,
		UINT destinationUnit,
		Legacy3DVertex* pVertex)
	{
		const FLOAT planar[4] = {
			position.x,
			position.z,
			pConstants[7 * 4 + 0],
			pConstants[7 * 4 + 1]
		};
		FLOAT* pDestination = destinationUnit == 0
			? pVertex->texCoord
			: pVertex->texCoordExtra[destinationUnit - 1];
		for (UINT component = 0; component < 2; ++component)
		{
			const FLOAT* pRow =
				pConstants + (firstConstant + component) * 4;
			pDestination[component] =
				planar[0] * pRow[0] + planar[1] * pRow[1]
				+ planar[2] * pRow[2] + planar[3] * pRow[3];
		}
	}

	void WriteTerrainMapTexCoord(
		const GfxVector3& position,
		const FLOAT* pConstants,
		Legacy3DVertex* pVertex)
	{
		const FLOAT projectedU =
			Dot3(position, pConstants + 18 * 4)
			+ pConstants[20 * 4 + 0];
		const FLOAT projectedV =
			Dot3(position, pConstants + 19 * 4)
			+ pConstants[20 * 4 + 1];
		pVertex->texCoord[0] =
			projectedU * pConstants[20 * 4 + 2];
		pVertex->texCoord[1] =
			projectedV * pConstants[20 * 4 + 3];
	}
}

CDirectX12Legacy3DCommandBatch::CDirectX12Legacy3DCommandBatch()
	: m_pDevice(NULL)
	, m_pPipelineCache(NULL)
	, m_currentFrame(0)
	, m_currentSubmissionBuffer(0)
	, m_pDepthBuffer(NULL)
	, m_pState(NULL)
{
	for (UINT iFrame = 0; iFrame < BUFFER_FRAME_COUNT; ++iFrame)
	{
		for (UINT iSubmission = 0;
			iSubmission < BUFFER_SUBMISSION_COUNT;
			++iSubmission)
		{
			m_pVertexBuffers[iFrame][iSubmission] = NULL;
			m_pIndexBuffers[iFrame][iSubmission] = NULL;
		}
	}
}

CDirectX12Legacy3DCommandBatch::~CDirectX12Legacy3DCommandBatch()
{
	Shutdown();
}

bool CDirectX12Legacy3DCommandBatch::Initialize(
	ID3D12Device* pDevice,
	CDirectX12PipelineCache* pPipelineCache)
{
	if (pDevice == NULL || pPipelineCache == NULL)
		return false;
	Shutdown();
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pPipelineCache = pPipelineCache;
	m_pState = new DirectX12Legacy3DCommandBatchState;
	m_pDepthBuffer = new CDirectX12DepthBuffer;
	if (m_pState == NULL || m_pDepthBuffer == NULL
		|| !m_pDepthBuffer->Initialize(m_pDevice))
	{
		Shutdown();
		return false;
	}
	return true;
}

void CDirectX12Legacy3DCommandBatch::Shutdown()
{
	if (m_pState != NULL)
	{
		DumpShaderPairInventory(m_pState);
		for (size_t iRange = 0;
			iRange < m_pState->ranges.size();
			++iRange)
		{
			ReleaseTexture(m_pState->ranges[iRange].pTexture);
			ReleaseTexture(m_pState->ranges[iRange].pTexture1);
			ReleaseTexture(m_pState->ranges[iRange].pTexture2);
			ReleaseTexture(m_pState->ranges[iRange].pTexture3);
		}
		delete m_pState;
		m_pState = NULL;
	}
	for (UINT iFrame = 0; iFrame < BUFFER_FRAME_COUNT; ++iFrame)
	{
		for (UINT iSubmission = 0;
			iSubmission < BUFFER_SUBMISSION_COUNT;
			++iSubmission)
		{
			delete m_pIndexBuffers[iFrame][iSubmission];
			m_pIndexBuffers[iFrame][iSubmission] = NULL;
			delete m_pVertexBuffers[iFrame][iSubmission];
			m_pVertexBuffers[iFrame][iSubmission] = NULL;
		}
	}
	delete m_pDepthBuffer;
	m_pDepthBuffer = NULL;
	m_currentFrame = 0;
	m_currentSubmissionBuffer = 0;
	m_pPipelineCache = NULL;
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

void CDirectX12Legacy3DCommandBatch::BeginFrame(UINT frameIndex)
{
	if (m_pState == NULL || frameIndex >= BUFFER_FRAME_COUNT)
		return;
	m_currentFrame = frameIndex;
	m_currentSubmissionBuffer = 0;
	++m_pState->frameSerial;
	if (m_pState->frameSerial - m_pState->lastInventoryDumpFrame
		>= (ReadShaderInventoryMode() ? 20U : PROBE_INTERVAL_FRAMES))
		DumpShaderPairInventory(m_pState);
	for (size_t iRange = 0;
		iRange < m_pState->ranges.size();
		++iRange)
	{
		ReleaseTexture(m_pState->ranges[iRange].pTexture);
		ReleaseTexture(m_pState->ranges[iRange].pTexture1);
		ReleaseTexture(m_pState->ranges[iRange].pTexture2);
		ReleaseTexture(m_pState->ranges[iRange].pTexture3);
	}
	m_pState->positions.clear();
	for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
	{
		m_pState->texCoords[textureUnit].clear();
		m_pState->projectiveTexCoordQ[textureUnit].clear();
	}
	m_pState->normals.clear();
	m_pState->weights.clear();
	m_pState->tangents.clear();
	m_pState->colors.clear();
	m_pState->vertices.clear();
	m_pState->indices.clear();
	m_pState->ranges.clear();
	m_pState->submittedRangeCount = 0;
	m_pState->vertexShaderFamilies.clear();
	m_pState->capturedDrawCount = 0;
	m_pState->rejectedDrawCount = 0;
	m_pState->capturedTriangleCount = 0;
	ZeroMemory(
		m_pState->rejectedReasons,
		sizeof(m_pState->rejectedReasons));
	m_pState->constantColor = C_WHITE | CT_OPAQUE;
	m_pState->staticPositionSelected = false;
	m_pState->staticTexCoordSelected = false;
	m_pState->staticNormalSelected = false;
	m_pState->terrainColorPendingAfterDepthMask = false;
	m_pState->terrainDepthMaskVertexCount = 0;
	m_pState->pNativeDepthTarget = NULL;
}

void CDirectX12Legacy3DCommandBatch::ForgetTexture(
	IDirect3DTexture9* pTexture)
{
	if (m_pState == NULL || pTexture == NULL)
		return;

	for (size_t iRange = 0;
		iRange < m_pState->ranges.size();
		++iRange)
	{
		IDirect3DTexture9** textures[] = {
			&m_pState->ranges[iRange].pTexture,
			&m_pState->ranges[iRange].pTexture1,
			&m_pState->ranges[iRange].pTexture2,
			&m_pState->ranges[iRange].pTexture3
		};
		for (UINT textureUnit = 0;
			textureUnit < sizeof(textures) / sizeof(textures[0]);
			++textureUnit)
		{
			if (*textures[textureUnit] == pTexture)
				ReleaseTexture(*textures[textureUnit]);
		}
	}
}

void CDirectX12Legacy3DCommandBatch::SetVertexArray(
	const FLOAT* pPositions,
	UINT vertexCount)
{
	SetVertexArrayInternal(pPositions, vertexCount, false);
}

void CDirectX12Legacy3DCommandBatch::SetStaticVertexArray(
	const FLOAT* pPositions,
	UINT vertexCount)
{
	SetVertexArrayInternal(pPositions, vertexCount, true);
}

void CDirectX12Legacy3DCommandBatch::SetVertexArrayInternal(
	const FLOAT* pPositions,
	UINT vertexCount,
	bool staticSource)
{
	if (m_pState == NULL || pPositions == NULL || vertexCount == 0)
		return;
	m_pState->positions.assign(
		pPositions,
		pPositions + static_cast<size_t>(vertexCount) * 3);
	for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
		m_pState->texCoords[textureUnit].clear();
	m_pState->normals.clear();
	m_pState->weights.clear();
	m_pState->tangents.clear();
	m_pState->colors.clear();
	m_pState->staticPositionSelected = staticSource;
	m_pState->staticTexCoordSelected = false;
}

void CDirectX12Legacy3DCommandBatch::SetNormalArray(
	const FLOAT* pNormals,
	UINT vertexCount)
{
	SetNormalArrayInternal(pNormals, vertexCount, false);
}

void CDirectX12Legacy3DCommandBatch::SetStaticNormalArray(
	const FLOAT* pNormals,
	UINT vertexCount)
{
	SetNormalArrayInternal(pNormals, vertexCount, true);
}

void CDirectX12Legacy3DCommandBatch::SetNormalArrayInternal(
	const FLOAT* pNormals,
	UINT vertexCount,
	bool staticSource)
{
	if (m_pState == NULL || pNormals == NULL || vertexCount == 0)
		return;
	m_pState->normals.assign(
		pNormals,
		pNormals + static_cast<size_t>(vertexCount) * 3);
	m_pState->staticNormalSelected = staticSource;
}

void CDirectX12Legacy3DCommandBatch::SetTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	SetTexCoordArrayInternal(
		textureUnit,
		pTexCoords,
		vertexCount,
		false);
}

void CDirectX12Legacy3DCommandBatch::SetProjectiveTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	if (m_pState == NULL || textureUnit >= 4
		|| pTexCoords == NULL || vertexCount == 0)
		return;
	std::vector<FLOAT>& texCoords = m_pState->texCoords[textureUnit];
	std::vector<FLOAT>& texCoordQ =
		m_pState->projectiveTexCoordQ[textureUnit];
	texCoords.resize(static_cast<size_t>(vertexCount) * 2);
	texCoordQ.resize(vertexCount);
	for (UINT vertex = 0; vertex < vertexCount; ++vertex)
	{
		texCoords[vertex * 2 + 0] = pTexCoords[vertex * 4 + 0];
		texCoords[vertex * 2 + 1] = pTexCoords[vertex * 4 + 1];
		texCoordQ[vertex] = pTexCoords[vertex * 4 + 3];
	}
	if (textureUnit == 0)
		m_pState->staticTexCoordSelected = false;
}

void CDirectX12Legacy3DCommandBatch::SetStaticTexCoordArray(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount)
{
	SetTexCoordArrayInternal(
		textureUnit,
		pTexCoords,
		vertexCount,
		true);
}

void CDirectX12Legacy3DCommandBatch::SetTexCoordArrayInternal(
	UINT textureUnit,
	const FLOAT* pTexCoords,
	UINT vertexCount,
	bool staticSource)
{
	if (m_pState == NULL || textureUnit >= 4
		|| pTexCoords == NULL || vertexCount == 0)
		return;
	m_pState->texCoords[textureUnit].assign(
		pTexCoords,
		pTexCoords + static_cast<size_t>(vertexCount) * 2);
	m_pState->projectiveTexCoordQ[textureUnit].clear();
	if (textureUnit == 0)
		m_pState->staticTexCoordSelected = staticSource;
}

void CDirectX12Legacy3DCommandBatch::SetWeightArray(
	const BYTE* pWeights,
	UINT vertexCount)
{
	SetWeightArrayInternal(pWeights, vertexCount, false);
}

void CDirectX12Legacy3DCommandBatch::SetStaticWeightArray(
	const BYTE* pWeights,
	UINT vertexCount)
{
	SetWeightArrayInternal(pWeights, vertexCount, true);
}

void CDirectX12Legacy3DCommandBatch::SetWeightArrayInternal(
	const BYTE* pWeights,
	UINT vertexCount,
	bool)
{
	if (m_pState == NULL || pWeights == NULL || vertexCount == 0)
		return;
	m_pState->weights.assign(
		pWeights,
		pWeights + static_cast<size_t>(vertexCount) * 8);
}

void CDirectX12Legacy3DCommandBatch::SetTangentArray(
	const FLOAT* pTangents,
	UINT vertexCount)
{
	SetTangentArrayInternal(pTangents, vertexCount, false);
}

void CDirectX12Legacy3DCommandBatch::SetStaticTangentArray(
	const FLOAT* pTangents,
	UINT vertexCount)
{
	SetTangentArrayInternal(pTangents, vertexCount, true);
}

void CDirectX12Legacy3DCommandBatch::SetTangentArrayInternal(
	const FLOAT* pTangents,
	UINT vertexCount,
	bool)
{
	if (m_pState == NULL || pTangents == NULL || vertexCount == 0)
		return;
	m_pState->tangents.assign(
		pTangents,
		pTangents + static_cast<size_t>(vertexCount) * 4);
}

void CDirectX12Legacy3DCommandBatch::SetColorArray(
	const ULONG* pColors,
	UINT vertexCount)
{
	if (m_pState == NULL || pColors == NULL || vertexCount == 0)
		return;
	// GFXColor conserva los canales dinámicos en ABGR. D3D9 los convierte
	// al llenar su stream; la captura DX12 debe normalizarlos nuevamente al
	// formato RGBA interno antes de descomponer cada canal.
	m_pState->colors.resize(vertexCount);
	for (UINT iColor = 0; iColor < vertexCount; ++iColor)
		m_pState->colors[iColor] = ByteSwap(pColors[iColor]);
}

void CDirectX12Legacy3DCommandBatch::SetStaticD3DColorArray(
	const ULONG* pColors,
	UINT vertexCount)
{
	if (m_pState == NULL || pColors == NULL || vertexCount == 0)
		return;
	m_pState->colors.resize(vertexCount);
	for (UINT iColor = 0; iColor < vertexCount; ++iColor)
	{
		const ULONG argb = pColors[iColor];
		m_pState->colors[iColor] =
			((argb << 8) & 0xFFFFFF00UL)
			| ((argb >> 24) & 0x000000FFUL);
	}
}

void CDirectX12Legacy3DCommandBatch::SetConstantColor(ULONG color)
{
	if (m_pState == NULL)
		return;
	m_pState->constantColor = color;
	m_pState->colors.clear();
}

bool CDirectX12Legacy3DCommandBatch::QueueIndexedDraw(
	IDirect3DDevice9* pDevice9,
	const USHORT* pIndices,
	UINT indexCount,
	bool dynamicBuffer,
	bool usesVertexProgram,
	bool usesPixelProgram,
	bool usesColorArray,
	bool projectiveMapping,
	UINT texturePassCount,
	DirectX12LegacyRenderTargetKind renderTargetKind)
{
	if (m_pState == NULL || pDevice9 == NULL || pIndices == NULL
		|| indexCount < 3 || indexCount % 3 != 0)
		return false;
	// La repetición 3D todavía es diagnóstica y no reemplaza el draw heredado.
	// El perfil probe toma una sola muestra cada 120 frames y ambos perfiles
	// tienen presupuestos duros para impedir una carga nativa ilimitada.
	const Native3DCaptureProfile captureProfile =
		ReadNative3DCaptureProfile();
	const bool inventoryMode = ReadShaderInventoryMode();
	if (captureProfile == NATIVE_3D_CAPTURE_OFF && !inventoryMode)
		return false;
	UINT64 vertexShaderFingerprint = usesVertexProgram
		? GetVertexShaderFingerprint(m_pState, pDevice9)
		: 0;
	UINT64 pixelShaderFingerprint = usesPixelProgram
		? GetPixelShaderFingerprint(m_pState, pDevice9)
		: 0;
	const UINT64 actualVertexShaderFingerprint =
		vertexShaderFingerprint;
	const UINT64 actualPixelShaderFingerprint =
		pixelShaderFingerprint;
	const bool testFamilyAliasApplied = ApplyTestShaderFamilyAlias(
		actualVertexShaderFingerprint,
		actualPixelShaderFingerprint,
		&vertexShaderFingerprint,
		&pixelShaderFingerprint);
	if (testFamilyAliasApplied
		&& !m_pState->testFamilyAliasDiagnosticReported)
	{
		CPrintF(
			"DX12 prueba: familia %016llX/%016llX sustituida por "
			"%016llX/%016llX para el fixture seleccionado.\n",
			static_cast<unsigned long long>(
				actualVertexShaderFingerprint),
			static_cast<unsigned long long>(
				actualPixelShaderFingerprint),
			static_cast<unsigned long long>(
				vertexShaderFingerprint),
			static_cast<unsigned long long>(
				pixelShaderFingerprint));
		m_pState->testFamilyAliasDiagnosticReported = true;
	}
	const bool rigidLit =
		vertexShaderFingerprint == RIGID_LIT_VERTEX_SHADER_FAMILY;
	const bool rigidLitPixel =
		pixelShaderFingerprint == RIGID_LIT_PIXEL_SHADER_FAMILY;
	const bool fixedFunctionDraw =
		!usesVertexProgram && !usesPixelProgram;
	const bool fixedProjectiveDraw =
		projectiveMapping && fixedFunctionDraw;
	const bool terrainDepthMaskDraw =
		fixedFunctionDraw
		&& DetectTerrainDepthMask(m_pState, pDevice9);
	// Las sombras proyectadas consumen el render target nativo mediante la
	// ruta fixed-function. Esta pareja no tiene fingerprints de shader, pero
	// su estado y su generacion de coordenadas se traducen explicitamente.
	DirectX12LegacyShaderFamily shaderFamily;
	const bool knownShaderFamily = GetDirectX12LegacyShaderFamily(
		vertexShaderFingerprint,
		pixelShaderFingerprint,
		texturePassCount,
		&shaderFamily);
	if (inventoryMode)
		RecordShaderPair(
			m_pState,
			vertexShaderFingerprint,
			pixelShaderFingerprint,
			texturePassCount,
			dynamicBuffer,
			projectiveMapping,
			indexCount);
	if (captureProfile == NATIVE_3D_CAPTURE_OFF)
		return false;
	// En reemplazo integral, la mascara de profundidad y las capas proyectadas
	// forman una sola unidad DX12. Los modos de diagnostico conservan el
	// terreno completo en D3D9On12 para no mezclar dos implementaciones.
	if (knownShaderFamily
		&& IsProjectedTerrainVertexFamily(shaderFamily.vertex)
		&& !ReadFullReplacementMode())
	{
		CaptureProjectedTerrainCamera(pDevice9);
		if (!m_pState->terrainCompatibilityFallbackReported)
		{
			CPrintF(
				"DX12 terreno: familias multipass conservadas en "
				"D3D9On12 para preservar profundidad y capas.\n");
			m_pState->terrainCompatibilityFallbackReported = true;
		}
		return false;
	}
	UINT64 replacementVertexFamily = 0;
	const bool replacementVertexFamilySelected =
		ReadReplacementVertexFamily(&replacementVertexFamily);
	UINT64 replacementPixelFamily = 0;
	const bool replacementPixelFamilySelected =
		ReadReplacementPixelFamily(&replacementPixelFamily);
	const bool replacementFamilySelected =
		replacementVertexFamilySelected || replacementPixelFamilySelected;
	const bool selectedFamilyMatches =
		(!replacementVertexFamilySelected
			|| vertexShaderFingerprint == replacementVertexFamily)
		&& (!replacementPixelFamilySelected
			|| pixelShaderFingerprint == replacementPixelFamily);
	UINT64 excludedPixelFamily = 0;
	if (ReadExcludedPixelFamily(&excludedPixelFamily)
		&& pixelShaderFingerprint == excludedPixelFamily)
		return false;
	UINT64 excludedVertexFamily = 0;
	if (ReadExcludedVertexFamily(&excludedVertexFamily)
		&& vertexShaderFingerprint == excludedVertexFamily)
		return false;
	const bool offscreenReplacementAllowed =
		renderTargetKind == DX12_LEGACY_RENDER_TARGET_OFFSCREEN
		&& ReadFullReplacementMode()
		&& selectedFamilyMatches
		// El agua consume la reflexion y no puede formar parte de la pasada
		// que produce esa misma textura: seria una realimentacion RTV/SRV.
		&& shaderFamily.pixel != DX12_LEGACY_PS_WATER
		&& (replacementFamilySelected
			|| shaderFamily.replacementValidated);
	if (renderTargetKind != DX12_LEGACY_RENDER_TARGET_PRESENTATION
		&& !offscreenReplacementAllowed)
	{
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_OFFSCREEN_RENDER_TARGET];
		return false;
	}
	// Las parejas programables deben estar validadas. Fixed-function general
	// sigue en fallback: mezcla espacios de vertices y pasadas auxiliares que
	// pueden producir paneles gigantes. Solo la proyeccion controlada y la
	// mascara de profundidad del terreno tienen contrato nativo validado.
	if (ReadFullReplacementMode()
		&& !replacementFamilySelected
		&& !shaderFamily.replacementValidated
		&& !fixedProjectiveDraw
		&& !terrainDepthMaskDraw)
	{
		if (fixedFunctionDraw
			&& !m_pState->fixedGeneralFallbackReported)
		{
			CPrintF(
				"DX12 3D guardia: fixed-function general permanece "
				"en fallback para evitar paneles o triangulos gigantes.\n");
			m_pState->fixedGeneralFallbackReported = true;
		}
		return false;
	}
	if (ReadFullReplacementMode()
		&& replacementVertexFamilySelected
		&& !selectedFamilyMatches)
		return false;
	if (ReadFullReplacementMode()
		&& replacementPixelFamilySelected
		&& !selectedFamilyMatches)
		return false;
	const bool rigidLitProbe = ReadRigidLitProbeMode();
	if (captureProfile == NATIVE_3D_CAPTURE_PROBE
		&& !rigidLitProbe
		&& m_pState->frameSerial % PROBE_INTERVAL_FRAMES != 0)
		return false;
	const Native3DCaptureBudget captureBudget =
		GetCaptureBudget(captureProfile);
	if (usesVertexProgram)
		RecordVertexShaderFamily(m_pState, pDevice9, indexCount);
	if (rigidLitProbe && !rigidLit)
		return false;

	const UINT vertexCount =
		static_cast<UINT>(m_pState->positions.size() / 3);
	RejectionReason reason = REJECT_REASON_COUNT;
	if (usesVertexProgram && !knownShaderFamily)
		reason = REJECT_VERTEX_PROGRAM;
	else if (usesPixelProgram && !knownShaderFamily)
		reason = REJECT_PIXEL_PROGRAM;
	else if (projectiveMapping && !fixedProjectiveDraw)
		reason = REJECT_PROJECTIVE_MAPPING;
	else if (texturePassCount > 4) reason = REJECT_TEXTURE_PASS_COUNT;
	else if (!dynamicBuffer
		&& (!m_pState->staticPositionSelected
			|| (shaderFamily.requiresSourceTexCoords
				&& !m_pState->staticTexCoordSelected)
			|| (shaderFamily.requiresNormals
				&& !m_pState->staticNormalSelected)))
		reason = REJECT_NOT_DYNAMIC;
	else if (!dynamicBuffer
		&& !ReadStaticCaptureMode()
		&& !ReadFullReplacementMode()
		&& !(rigidLit && (captureProfile == NATIVE_3D_CAPTURE_PROBE
			|| ReadRigidLitReplacementMode())))
		reason = REJECT_NOT_DYNAMIC;
	else if (vertexCount == 0)
		reason = REJECT_MISSING_CPU_ARRAY;
	else if (shaderFamily.requiresSourceTexCoords
		&& m_pState->texCoords[0].size()
			!= static_cast<size_t>(vertexCount) * 2)
		reason = REJECT_MISSING_CPU_ARRAY;
	else if (shaderFamily.requiresNormals
		&& m_pState->normals.size()
			!= static_cast<size_t>(vertexCount) * 3)
		reason = REJECT_MISSING_CPU_ARRAY;
	else if (shaderFamily.requiresWeights
		&& m_pState->weights.size()
			!= static_cast<size_t>(vertexCount) * 8)
		reason = REJECT_MISSING_CPU_ARRAY;
	else if (shaderFamily.requiresTangents
		&& m_pState->tangents.size()
			!= static_cast<size_t>(vertexCount) * 4)
		reason = REJECT_MISSING_CPU_ARRAY;
	if (reason != REJECT_REASON_COUNT)
	{
		if (reason == REJECT_MISSING_CPU_ARRAY
			&& !m_pState->missingArrayDiagnosticReported)
		{
			CPrintF(
				"DX12 diagnostico arrays: VS=%016llX, PS=%016llX, "
				"dinamico=%u, vertices=%u, pos=%u, uv0=%u, "
				"normales=%u, pesos=%u, tangentes=%u, colores=%u, "
				"requiereUV=%u, requiereN=%u, requiereP=%u, "
				"requiereT=%u.\n",
				static_cast<unsigned long long>(
					vertexShaderFingerprint),
				static_cast<unsigned long long>(
					pixelShaderFingerprint),
				dynamicBuffer ? 1U : 0U,
				vertexCount,
				static_cast<UINT>(m_pState->positions.size()),
				static_cast<UINT>(m_pState->texCoords[0].size()),
				static_cast<UINT>(m_pState->normals.size()),
				static_cast<UINT>(m_pState->weights.size()),
				static_cast<UINT>(m_pState->tangents.size()),
				static_cast<UINT>(m_pState->colors.size()),
				shaderFamily.requiresSourceTexCoords ? 1U : 0U,
				shaderFamily.requiresNormals ? 1U : 0U,
				shaderFamily.requiresWeights ? 1U : 0U,
				shaderFamily.requiresTangents ? 1U : 0U);
			m_pState->missingArrayDiagnosticReported = true;
		}
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[reason];
		return false;
	}

	std::vector<UINT> sourceVertexIndices;
	std::vector<UINT> remappedIndices(indexCount);
	std::vector<UINT> sourceToLocal(
		vertexCount,
		static_cast<UINT>(-1));
	for (UINT iIndex = 0; iIndex < indexCount; ++iIndex)
	{
		if (pIndices[iIndex] >= vertexCount)
		{
			++m_pState->rejectedDrawCount;
			++m_pState->rejectedReasons[REJECT_INVALID_INDEX];
			return false;
		}
		const UINT sourceIndex = pIndices[iIndex];
		UINT& localIndex = sourceToLocal[sourceIndex];
		if (localIndex == static_cast<UINT>(-1))
		{
			localIndex = static_cast<UINT>(sourceVertexIndices.size());
			sourceVertexIndices.push_back(sourceIndex);
		}
		remappedIndices[iIndex] = localIndex;
	}
	const UINT usedVertexCount =
		static_cast<UINT>(sourceVertexIndices.size());
	if (m_pState->capturedDrawCount >= captureBudget.maxDraws
		|| m_pState->vertices.size() + usedVertexCount
			> captureBudget.maxVertices
		|| m_pState->indices.size() + indexCount
			> captureBudget.maxIndices)
	{
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_CAPTURE_LIMIT];
		return false;
	}

	D3DMATRIX world;
	D3DMATRIX view;
	D3DMATRIX projection;
	D3DVIEWPORT9 viewport9;
	DWORD zEnable = FALSE;
	DWORD zWrite = FALSE;
	DWORD zFunction = D3DCMP_LESSEQUAL;
	DWORD clipping = TRUE;
	DWORD cullMode = D3DCULL_NONE;
	DWORD blending = FALSE;
	DWORD alphaTest = FALSE;
	DWORD sourceBlend = D3DBLEND_SRCALPHA;
	DWORD destinationBlend = D3DBLEND_INVSRCALPHA;
	DWORD alphaReference = 128;
	DWORD colorWriteMask =
		D3DCOLORWRITEENABLE_RED
		| D3DCOLORWRITEENABLE_GREEN
		| D3DCOLORWRITEENABLE_BLUE
		| D3DCOLORWRITEENABLE_ALPHA;
	DWORD samplerAddressU = D3DTADDRESS_WRAP;
	DWORD samplerAddressV = D3DTADDRESS_WRAP;
	DWORD samplerMinification = D3DTEXF_LINEAR;
	DWORD samplerMagnification = D3DTEXF_LINEAR;
	DWORD bumpEnvironmentState[4] = { 0, 0, 0, 0 };
	DWORD textureFactor = 0xFFFFFFFFUL;
	DWORD fixedColorOperation[4] = {
		D3DTOP_DISABLE, D3DTOP_DISABLE,
		D3DTOP_DISABLE, D3DTOP_DISABLE
	};
	DWORD fixedColorArgument1[4] = {
		D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE
	};
	DWORD fixedColorArgument2[4] = {
		D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT
	};
	DWORD fixedAlphaOperation[4] = {
		D3DTOP_DISABLE, D3DTOP_DISABLE,
		D3DTOP_DISABLE, D3DTOP_DISABLE
	};
	DWORD fixedAlphaArgument1[4] = {
		D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE, D3DTA_TEXTURE
	};
	DWORD fixedAlphaArgument2[4] = {
		D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT
	};
	DWORD fixedResultArgument[4] = {
		D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT, D3DTA_CURRENT
	};
	DWORD fixedStageConstant[4] = {
		0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL
	};
	DWORD fixedTexCoordIndex[4] = { 0, 1, 2, 3 };
	DWORD fixedTextureTransformFlags[4] = {
		D3DTTFF_DISABLE, D3DTTFF_DISABLE,
		D3DTTFF_DISABLE, D3DTTFF_DISABLE
	};
	D3DMATRIX fixedTextureTransform[4];
	for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
		GfxMatrixIdentity(
			reinterpret_cast<GfxMatrix*>(
				&fixedTextureTransform[textureUnit]));
	const UINT LEGACY_VERTEX_CONSTANT_COUNT = 96 * 4;
	FLOAT vertexShaderConstants[LEGACY_VERTEX_CONSTANT_COUNT];
	FLOAT shaderConstants[RIGID_LIT_CONSTANT_COUNT];
	FLOAT legacyPixelConstants[RIGID_LIT_CONSTANT_COUNT];
	FLOAT pixelShaderConstants[RIGID_LIT_PIXEL_CONSTANT_COUNT];
	ZeroMemory(vertexShaderConstants, sizeof(vertexShaderConstants));
	ZeroMemory(shaderConstants, sizeof(shaderConstants));
	ZeroMemory(legacyPixelConstants, sizeof(legacyPixelConstants));
	ZeroMemory(pixelShaderConstants, sizeof(pixelShaderConstants));
	bool initializeTerrainColor = false;
	if (FAILED(pDevice9->GetTransform(D3DTS_WORLD, &world))
		|| FAILED(pDevice9->GetTransform(D3DTS_VIEW, &view))
		|| FAILED(pDevice9->GetTransform(D3DTS_PROJECTION, &projection))
		|| FAILED(pDevice9->GetViewport(&viewport9))
		|| FAILED(pDevice9->GetRenderState(D3DRS_ZENABLE, &zEnable))
		|| FAILED(pDevice9->GetRenderState(D3DRS_ZWRITEENABLE, &zWrite))
		|| FAILED(pDevice9->GetRenderState(D3DRS_ZFUNC, &zFunction))
		|| FAILED(pDevice9->GetRenderState(D3DRS_CLIPPING, &clipping))
		|| FAILED(pDevice9->GetRenderState(D3DRS_CULLMODE, &cullMode))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_ALPHABLENDENABLE,
			&blending))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_ALPHATESTENABLE,
			&alphaTest))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_SRCBLEND,
			&sourceBlend))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_DESTBLEND,
			&destinationBlend))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_ALPHAREF,
			&alphaReference))
		|| FAILED(pDevice9->GetRenderState(
			D3DRS_COLORWRITEENABLE,
			&colorWriteMask))
		|| (usesVertexProgram && FAILED(
			pDevice9->GetVertexShaderConstantF(
			0,
			vertexShaderConstants,
			96)))
		|| (usesPixelProgram && FAILED(
			pDevice9->GetPixelShaderConstantF(
			0,
			legacyPixelConstants,
			13))))
	{
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_STATE_QUERY];
		return false;
	}
	if (FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_ADDRESSU, &samplerAddressU))
		|| FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_ADDRESSV, &samplerAddressV))
		|| FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_MINFILTER, &samplerMinification))
		|| FAILED(pDevice9->GetSamplerState(
			0, D3DSAMP_MAGFILTER, &samplerMagnification)))
	{
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_STATE_QUERY];
		return false;
	}
	if (shaderFamily.pixel == DX12_LEGACY_PS_WATER
		&& (FAILED(pDevice9->GetTextureStageState(
				1, D3DTSS_BUMPENVMAT00, &bumpEnvironmentState[0]))
			|| FAILED(pDevice9->GetTextureStageState(
				1, D3DTSS_BUMPENVMAT01, &bumpEnvironmentState[1]))
			|| FAILED(pDevice9->GetTextureStageState(
				1, D3DTSS_BUMPENVMAT10, &bumpEnvironmentState[2]))
			|| FAILED(pDevice9->GetTextureStageState(
				1, D3DTSS_BUMPENVMAT11, &bumpEnvironmentState[3]))))
	{
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_STATE_QUERY];
		return false;
	}
	if (!usesVertexProgram && !usesPixelProgram)
	{
		bool fixedStateAvailable = SUCCEEDED(
			pDevice9->GetRenderState(D3DRS_TEXTUREFACTOR, &textureFactor));
		for (UINT textureUnit = 0;
			textureUnit < 4 && fixedStateAvailable;
			++textureUnit)
		{
			fixedStateAvailable =
				SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_COLOROP,
					&fixedColorOperation[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_COLORARG1,
					&fixedColorArgument1[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_COLORARG2,
					&fixedColorArgument2[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_ALPHAOP,
					&fixedAlphaOperation[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_ALPHAARG1,
					&fixedAlphaArgument1[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_ALPHAARG2,
					&fixedAlphaArgument2[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_RESULTARG,
					&fixedResultArgument[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_CONSTANT,
					&fixedStageConstant[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_TEXCOORDINDEX,
					&fixedTexCoordIndex[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTextureStageState(
					textureUnit,
					D3DTSS_TEXTURETRANSFORMFLAGS,
					&fixedTextureTransformFlags[textureUnit]))
				&& SUCCEEDED(pDevice9->GetTransform(
					static_cast<D3DTRANSFORMSTATETYPE>(
						D3DTS_TEXTURE0 + textureUnit),
					&fixedTextureTransform[textureUnit]));
		}
		if (!fixedStateAvailable)
		{
			++m_pState->rejectedDrawCount;
			++m_pState->rejectedReasons[REJECT_STATE_QUERY];
			return false;
		}
		// _iTexPass cuenta todos los arrays UV preparados desde el ultimo
		// bloqueo de vertices. Fog, haze y otros pases tardios pueden heredar
		// un valor mayor aunque el draw actual tenga una sola etapa activa.
		// COLOROP es el contrato autoritativo de fixed-function en D3D9.
		shaderFamily.textureCount =
			CountActiveFixedTextureStages(fixedColorOperation);
		shaderFamily.requiresSourceTexCoords =
			shaderFamily.textureCount > 0;
	}
	if (shaderFamily.nativeRigidPipeline)
	{
		CopyMemory(
			shaderConstants,
			vertexShaderConstants,
			sizeof(shaderConstants));
		CopyMemory(
			pixelShaderConstants,
			legacyPixelConstants,
			sizeof(pixelShaderConstants));
	}
	else
	{
		CopyMemory(
			shaderConstants,
			legacyPixelConstants,
			sizeof(shaderConstants));
		if (shaderFamily.pixel == DX12_LEGACY_PS_WATER)
		{
			for (UINT component = 0; component < 4; ++component)
			{
				FLOAT bumpValue = 0.0f;
				CopyMemory(
					&bumpValue,
					&bumpEnvironmentState[component],
					sizeof(FLOAT));
				if (!_finite(bumpValue) || fabsf(bumpValue) > 1.0f)
					bumpValue = 0.0f;
				shaderConstants[4 * 4 + component] = bumpValue;
			}
			FLOAT& waterAlpha = shaderConstants[3 * 4 + 3];
			if (!_finite(waterAlpha))
				waterAlpha = 1.0f;
			waterAlpha = (std::max)(0.0f, (std::min)(1.0f, waterAlpha));
			static bool waterStateReported = false;
			if (!waterStateReported)
			{
				CPrintF(
					"DX12 agua: alpha=%.4f, EMBM=(%.6f,%.6f,"
					"%.6f,%.6f).\n",
					waterAlpha,
					shaderConstants[4 * 4 + 0],
					shaderConstants[4 * 4 + 1],
					shaderConstants[4 * 4 + 2],
					shaderConstants[4 * 4 + 3]);
				waterStateReported = true;
			}
		}
		const int terrainDebugTexture =
			shaderFamily.pixel == DX12_LEGACY_PS_TERRAIN_FOUR_LAYER
				? ReadTerrainDebugTexture()
				: -1;
		pixelShaderConstants[0] = terrainDebugTexture >= 0
			? static_cast<FLOAT>(20 + terrainDebugTexture)
			: static_cast<FLOAT>(shaderFamily.pixel);
		pixelShaderConstants[1] =
			static_cast<FLOAT>(shaderFamily.textureCount);
		pixelShaderConstants[2] = alphaTest != FALSE ? 1.0f : 0.0f;
		pixelShaderConstants[3] =
			static_cast<FLOAT>(alphaReference) / 255.0f;
		const bool terrainLayerPass =
			shaderFamily.pixel == DX12_LEGACY_PS_TERRAIN_FOUR_LAYER
			|| shaderFamily.pixel == DX12_LEGACY_PS_ALPHA_MASK;
		const UINT currentTerrainVertexCount =
			static_cast<UINT>(m_pState->positions.size() / 3);
		const bool matchingDepthMask =
			m_pState->terrainColorPendingAfterDepthMask
			&& m_pState->terrainDepthMaskVertexCount
				== currentTerrainVertexCount;
		initializeTerrainColor =
			terrainLayerPass
			&& (zWrite != FALSE
				|| matchingDepthMask);
		if (initializeTerrainColor)
			pixelShaderConstants[3] = -1.0f;
		if (terrainLayerPass)
		{
			m_pState->terrainColorPendingAfterDepthMask = false;
			m_pState->terrainDepthMaskVertexCount = 0;
		}
		else if (colorWriteMask == 0 && zWrite != FALSE
			&& alphaTest != FALSE)
		{
			m_pState->terrainColorPendingAfterDepthMask = true;
			m_pState->terrainDepthMaskVertexCount =
				currentTerrainVertexCount;
		}
		if (!usesVertexProgram && !usesPixelProgram)
		{
			ZeroMemory(shaderConstants, sizeof(shaderConstants));
			for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
			{
				const UINT constantBase = textureUnit * 3;
				shaderConstants[(constantBase + 0) * 4 + 0] =
					static_cast<FLOAT>(fixedColorOperation[textureUnit]);
				shaderConstants[(constantBase + 0) * 4 + 1] =
					static_cast<FLOAT>(fixedColorArgument1[textureUnit]);
				shaderConstants[(constantBase + 0) * 4 + 2] =
					static_cast<FLOAT>(fixedColorArgument2[textureUnit]);
				shaderConstants[(constantBase + 0) * 4 + 3] =
					static_cast<FLOAT>(fixedAlphaOperation[textureUnit]);
				shaderConstants[(constantBase + 1) * 4 + 0] =
					static_cast<FLOAT>(fixedAlphaArgument1[textureUnit]);
				shaderConstants[(constantBase + 1) * 4 + 1] =
					static_cast<FLOAT>(fixedAlphaArgument2[textureUnit]);
				shaderConstants[(constantBase + 1) * 4 + 2] =
					static_cast<FLOAT>(fixedResultArgument[textureUnit]);
				WriteD3DColor(
					fixedStageConstant[textureUnit],
					shaderConstants + (constantBase + 2) * 4);
			}
			WriteD3DColor(textureFactor, shaderConstants + 12 * 4);
		}
	}
	// Los draws 2D del login y del HUD también pasan por este wrapper.
	// El reemplazo 3D integral sólo es autoritativo cuando la profundidad
	// está activa; los demás continúan en la ruta de UI ya migrada.
	if (ReadFullReplacementMode() && zEnable == FALSE)
	{
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_STATE_QUERY];
		return false;
	}
	const FixedFunctionCaptureMode fixedFunctionCaptureMode =
		ReadFixedFunctionCaptureMode();
	const bool fixedFunctionBlended =
		blending != FALSE || alphaTest != FALSE;
	const DirectX12BlendMode fixedFunctionBlendMode = ToBlendMode(
		blending,
		alphaTest,
		sourceBlend,
		destinationBlend);
	const int fixedFunctionBlendFilter =
		ReadFixedFunctionBlendFilter();
	const int fixedFunctionTextureWidthFilter =
		ReadFixedFunctionTextureWidthFilter();
	// Los grupos fixed se validan de forma aislada. El modo estable conserva
	// solo los opacos; el laboratorio puede ejercer las capas sin escritura de
	// profundidad sin volver autoritativos los demás estados en el mismo frame.
	if (ReadFullReplacementMode()
		&& fixedFunctionDraw
		&& fixedFunctionCaptureMode == FIXED_FUNCTION_CAPTURE_OPAQUE
		&& zWrite == FALSE
		&& !fixedProjectiveDraw)
	{
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_FIXED_TRANSPARENT_PASS];
		return false;
	}
	if (ReadFullReplacementMode()
		&& fixedFunctionDraw
		&& fixedFunctionCaptureMode == FIXED_FUNCTION_CAPTURE_TRANSPARENT
		&& (zWrite != FALSE || !fixedFunctionBlended))
		return false;
	if (ReadFullReplacementMode()
		&& fixedFunctionDraw
		&& fixedFunctionBlendFilter >= 0
		&& fixedFunctionBlendMode
			!= static_cast<DirectX12BlendMode>(
				fixedFunctionBlendFilter))
		return false;
	if (ReadFullReplacementMode()
		&& fixedFunctionDraw
		&& !MatchesFixedFunctionTextureWidthFilter(
			pDevice9,
			fixedFunctionTextureWidthFilter))
		return false;

	GfxMatrix worldView;
	GfxMatrix worldViewProjection;
	GfxMatrixMultiply(
		&worldView,
		reinterpret_cast<const GfxMatrix*>(&world),
		reinterpret_cast<const GfxMatrix*>(&view));
	GfxMatrixMultiply(
		&worldViewProjection,
		&worldView,
		reinterpret_cast<const GfxMatrix*>(&projection));

	if (!usesVertexProgram && !usesPixelProgram)
	{
		for (UINT textureUnit = 0;
			textureUnit < shaderFamily.textureCount;
			++textureUnit)
		{
			if (fixedColorOperation[textureUnit] == D3DTOP_DISABLE)
				break;
			const DWORD coordinateGeneration =
				fixedTexCoordIndex[textureUnit] & 0xFFFF0000UL;
			const UINT sourceTexCoord =
				fixedTexCoordIndex[textureUnit] & 0x0000FFFFUL;
			const bool passThrough =
				coordinateGeneration == D3DTSS_TCI_PASSTHRU;
			const bool cameraSpacePosition =
				coordinateGeneration
					== D3DTSS_TCI_CAMERASPACEPOSITION;
			if ((!passThrough && !cameraSpacePosition)
				|| (passThrough
					&& (sourceTexCoord >= 4
						|| m_pState->texCoords[sourceTexCoord].size()
							!= static_cast<size_t>(vertexCount) * 2)))
			{
				++m_pState->rejectedDrawCount;
				++m_pState->rejectedReasons[
					REJECT_MISSING_CPU_ARRAY];
				return false;
			}
		}
	}

	const UINT baseVertex =
		static_cast<UINT>(m_pState->vertices.size());
	bool invalidClipCoordinate = false;
	const bool projectedTerrain =
		IsProjectedTerrainVertexFamily(shaderFamily.vertex);
	UINT clampedFarDepthCount = 0;
	// Las matrices CPU pueden describir el búfer completo. Compactamos los
	// índices a los vértices únicos de este draw para no multiplicar cientos
	// de veces el tráfico PCIe ni conservar huecos de otros submeshes.
	for (UINT iVertex = 0; iVertex < usedVertexCount; ++iVertex)
	{
		const UINT sourceVertexIndex = sourceVertexIndices[iVertex];
		GfxVector3 source(
			m_pState->positions[sourceVertexIndex * 3 + 0],
			m_pState->positions[sourceVertexIndex * 3 + 1],
			m_pState->positions[sourceVertexIndex * 3 + 2]);
		Legacy3DVertex vertex;
		ZeroMemory(&vertex, sizeof(vertex));
		for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
			vertex.texCoordQ[textureUnit] = 1.0f;
		GfxVector3 sourceNormal(0.0f, 0.0f, 1.0f);
		if (m_pState->normals.size()
			== static_cast<size_t>(vertexCount) * 3)
		{
			sourceNormal = GfxVector3(
				m_pState->normals[sourceVertexIndex * 3 + 0],
				m_pState->normals[sourceVertexIndex * 3 + 1],
				m_pState->normals[sourceVertexIndex * 3 + 2]);
		}
		GfxVector3 sourceTangent(1.0f, 0.0f, 0.0f);
		FLOAT tangentSign = 1.0f;
		if (m_pState->tangents.size()
			== static_cast<size_t>(vertexCount) * 4)
		{
			sourceTangent = GfxVector3(
				m_pState->tangents[sourceVertexIndex * 4 + 0],
				m_pState->tangents[sourceVertexIndex * 4 + 1],
				m_pState->tangents[sourceVertexIndex * 4 + 2]);
			tangentSign = m_pState->tangents[sourceVertexIndex * 4 + 3];
		}
		vertex.texCoord[0] = m_pState->texCoords[0].size()
			== static_cast<size_t>(vertexCount) * 2
			? m_pState->texCoords[0][sourceVertexIndex * 2 + 0] : 0.0f;
		vertex.texCoord[1] = m_pState->texCoords[0].size()
			== static_cast<size_t>(vertexCount) * 2
			? m_pState->texCoords[0][sourceVertexIndex * 2 + 1] : 0.0f;
		for (UINT textureUnit = 1; textureUnit < 4; ++textureUnit)
		{
			const std::vector<FLOAT>& texCoords =
				m_pState->texCoords[textureUnit];
			vertex.texCoordExtra[textureUnit - 1][0] =
				texCoords.size() == static_cast<size_t>(vertexCount) * 2
					? texCoords[sourceVertexIndex * 2 + 0]
					: vertex.texCoord[0];
			vertex.texCoordExtra[textureUnit - 1][1] =
				texCoords.size() == static_cast<size_t>(vertexCount) * 2
					? texCoords[sourceVertexIndex * 2 + 1]
					: vertex.texCoord[1];
		}
		for (UINT textureUnit = 0; textureUnit < 4; ++textureUnit)
		{
			const std::vector<FLOAT>& texCoordQ =
				m_pState->projectiveTexCoordQ[textureUnit];
			if (texCoordQ.size() == vertexCount)
				vertex.texCoordQ[textureUnit] =
					texCoordQ[sourceVertexIndex];
		}
		if (!usesVertexProgram && !usesPixelProgram)
		{
			for (UINT textureUnit = 0;
				textureUnit < shaderFamily.textureCount;
				++textureUnit)
			{
				if (fixedColorOperation[textureUnit] == D3DTOP_DISABLE)
					break;
				const UINT sourceTexCoord =
					fixedTexCoordIndex[textureUnit] & 0x0000FFFFUL;
				const DWORD coordinateGeneration =
					fixedTexCoordIndex[textureUnit] & 0xFFFF0000UL;
				GfxVector4 inputCoordinate;
				if (coordinateGeneration
					== D3DTSS_TCI_CAMERASPACEPOSITION)
				{
					GfxVec3Transform(
						&inputCoordinate,
						&source,
						&worldView);
				}
				else
				{
					const std::vector<FLOAT>& sourceCoordinates =
						m_pState->texCoords[sourceTexCoord];
					inputCoordinate = GfxVector4(
						sourceCoordinates[sourceVertexIndex * 2 + 0],
						sourceCoordinates[sourceVertexIndex * 2 + 1],
						0.0f,
						1.0f);
				}
				FLOAT transformedU = inputCoordinate.x;
				FLOAT transformedV = inputCoordinate.y;
				const DWORD transformFlags =
					fixedTextureTransformFlags[textureUnit];
				if ((transformFlags & 0xFFUL) != D3DTTFF_DISABLE)
				{
					GfxVector4 outputCoordinate;
					GfxVec4Transform(
						&outputCoordinate,
						&inputCoordinate,
						reinterpret_cast<const GfxMatrix*>(
							&fixedTextureTransform[textureUnit]));
					transformedU = outputCoordinate.x;
					transformedV = outputCoordinate.y;
					if ((transformFlags & D3DTTFF_PROJECTED) != 0)
					{
						const DWORD coordinateCount =
							transformFlags & 0xFFUL;
						const FLOAT divisor = coordinateCount == 3
							? outputCoordinate.z
							: outputCoordinate.w;
						if (divisor != 0.0f)
						{
							transformedU /= divisor;
							transformedV /= divisor;
						}
					}
				}
				if (textureUnit == 0)
				{
					vertex.texCoord[0] = transformedU;
					vertex.texCoord[1] = transformedV;
				}
				else
				{
					vertex.texCoordExtra[textureUnit - 1][0] =
						transformedU;
					vertex.texCoordExtra[textureUnit - 1][1] =
						transformedV;
				}
			}
		}
		const ULONG color = usesColorArray
			&& m_pState->colors.size() == vertexCount
			? m_pState->colors[sourceVertexIndex]
			: m_pState->constantColor;
		vertex.color[0] =
			static_cast<FLOAT>((color & CT_RMASK) >> CT_RSHIFT) / 255.0f;
		vertex.color[1] =
			static_cast<FLOAT>((color & CT_GMASK) >> CT_GSHIFT) / 255.0f;
		vertex.color[2] =
			static_cast<FLOAT>((color & CT_BMASK) >> CT_BSHIFT) / 255.0f;
		vertex.color[3] =
			static_cast<FLOAT>((color & CT_AMASK) >> CT_ASHIFT) / 255.0f;
		for (UINT component = 0; component < 4; ++component)
		{
			vertex.secondaryColor[component] = 0.5f;
			vertex.tangent[component] =
				m_pState->tangents.size()
					== static_cast<size_t>(vertexCount) * 4
					? m_pState->tangents[
						sourceVertexIndex * 4 + component]
					: (component == 3 ? 1.0f : 0.0f);
			vertex.blendIndices[component] =
				m_pState->weights.size()
					== static_cast<size_t>(vertexCount) * 8
					? static_cast<FLOAT>(
						m_pState->weights[
							sourceVertexIndex * 8 + component])
					: 0.0f;
			vertex.blendWeights[component] =
				m_pState->weights.size()
					== static_cast<size_t>(vertexCount) * 8
					? static_cast<FLOAT>(
						m_pState->weights[
							sourceVertexIndex * 8 + 4 + component])
						/ 255.0f
					: (component == 0 ? 1.0f : 0.0f);
		}

		if (shaderFamily.nativeRigidPipeline)
		{
			vertex.position[0] = source.x;
			vertex.position[1] = source.y;
			vertex.position[2] = source.z;
			vertex.normal[0] = sourceNormal.x;
			vertex.normal[1] = sourceNormal.y;
			vertex.normal[2] = sourceNormal.z;
			vertex.clipW = 1.0f;
		}
		else if (usesVertexProgram)
		{
			GfxVector3 transformedPosition = source;
			GfxVector3 transformedNormal = sourceNormal;
			GfxVector3 transformedTangent = sourceTangent;
			if (shaderFamily.requiresWeights)
			{
				SkinVertex(
					source,
					sourceNormal,
					sourceTangent,
					&m_pState->weights[sourceVertexIndex * 8],
					vertexShaderConstants,
					&transformedPosition,
					&transformedNormal,
					&transformedTangent);
			}
			WriteClipPosition(
				transformedPosition,
				vertexShaderConstants,
				&vertex);
			if (projectedTerrain
				&& ClampProjectedTerrainFarDepth(&vertex))
				++clampedFarDepthCount;
			if (shaderFamily.vertex == DX12_LEGACY_VS_RIGID_LIT
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_RIGID_LIT_PROJECTED)
				WriteLighting(
					transformedNormal,
					vertexShaderConstants,
					false,
					&vertex);
			else if (shaderFamily.vertex
					== DX12_LEGACY_VS_RIGID_REFLECTED
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_LIT
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_LIT_DETAIL
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_LIT_PROJECTED
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_RIGID_LIT_REFLECTED)
				WriteLighting(
					transformedNormal,
					vertexShaderConstants,
					true,
					&vertex);

			if (shaderFamily.vertex
				== DX12_LEGACY_VS_RIGID_LIT_PROJECTED)
			{
				vertex.texCoordExtra[0][0] =
					vertex.texCoord[0] * vertexShaderConstants[12 * 4];
				vertex.texCoordExtra[0][1] =
					vertex.texCoord[1] * vertexShaderConstants[12 * 4 + 1];
				WriteProjectedTexCoord(
					transformedPosition,
					vertexShaderConstants,
					18,
					2,
					&vertex);
			}
			else if (shaderFamily.vertex
				== DX12_LEGACY_VS_SKINNED_LIT_DETAIL)
			{
				vertex.texCoordExtra[0][0] =
					vertex.texCoord[0] * vertexShaderConstants[12 * 4];
				vertex.texCoordExtra[0][1] =
					vertex.texCoord[1] * vertexShaderConstants[12 * 4 + 1];
			}
			else if (shaderFamily.vertex
				== DX12_LEGACY_VS_RIGID_REFLECTED
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_LIT_PROJECTED)
			{
				WriteProjectedTexCoord(
					transformedPosition,
					vertexShaderConstants,
					18,
					2,
					&vertex);
			}
			else if (shaderFamily.vertex
				== DX12_LEGACY_VS_PROJECTED_ONE)
			{
				WriteTerrainMapTexCoord(
					transformedPosition,
					vertexShaderConstants,
					&vertex);
			}
			else if (shaderFamily.vertex
				== DX12_LEGACY_VS_PROJECTED_TWO
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_PROJECTED_FOUR)
			{
				WritePlanarTexCoord(
					transformedPosition,
					vertexShaderConstants,
					21,
					0,
					&vertex);
				WritePlanarTexCoord(
					transformedPosition,
					vertexShaderConstants,
					24,
					1,
					&vertex);
				if (shaderFamily.vertex
					== DX12_LEGACY_VS_PROJECTED_FOUR)
				{
					WritePlanarTexCoord(
						transformedPosition,
						vertexShaderConstants,
						27,
						2,
						&vertex);
					WritePlanarTexCoord(
						transformedPosition,
						vertexShaderConstants,
						30,
						3,
						&vertex);
				}
			}
			else if (shaderFamily.vertex
				== DX12_LEGACY_VS_RIGID_LIT_REFLECTED)
			{
				WriteProjectedTexCoord(
					transformedPosition,
					vertexShaderConstants,
					18,
					1,
					&vertex);
			}
			else if (shaderFamily.vertex == DX12_LEGACY_VS_WATER)
			{
				vertex.texCoord[0] +=
					vertexShaderConstants[17 * 4 + 1];
				vertex.texCoord[1] +=
					vertexShaderConstants[17 * 4 + 2];
				WriteHomogeneousProjectedTexCoord(
					transformedPosition,
					vertexShaderConstants,
					21,
					1,
					&vertex);
			}
			else if (shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_TANGENT
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_TANGENT_PROJECTED
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_TANGENT_SPECULAR
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_RIGID_TANGENT
				|| shaderFamily.vertex
					== DX12_LEGACY_VS_RIGID_TANGENT_PROJECTED)
			{
				const GfxVector3 normal =
					NormalizeSafe(transformedNormal);
				const GfxVector3 tangent =
					NormalizeSafe(transformedTangent);
				GfxVector3 bitangent;
				GfxVec3Cross(&bitangent, &tangent, &normal);
				bitangent = NormalizeSafe(bitangent) * tangentSign;
				const GfxVector3 light(
					vertexShaderConstants[4 * 4 + 0],
					vertexShaderConstants[4 * 4 + 1],
					vertexShaderConstants[4 * 4 + 2]);
				const FLOAT scale = vertexShaderConstants[7 * 4 + 3];
				vertex.color[0] = vertexShaderConstants[5 * 4 + 0];
				vertex.color[1] = vertexShaderConstants[5 * 4 + 1];
				vertex.color[2] = vertexShaderConstants[5 * 4 + 2];
				vertex.color[3] = vertexShaderConstants[5 * 4 + 3];
				vertex.secondaryColor[0] =
					GfxVec3Dot(&light, &bitangent) * scale + scale;
				vertex.secondaryColor[1] =
					GfxVec3Dot(&light, &tangent) * scale + scale;
				vertex.secondaryColor[2] =
					GfxVec3Dot(&light, &normal) * scale + scale;
				vertex.secondaryColor[3] = 1.0f;
				if (shaderFamily.vertex
					== DX12_LEGACY_VS_SKINNED_TANGENT_SPECULAR)
				{
					GfxVector3 viewDirection(
						vertexShaderConstants[8 * 4 + 0]
							- transformedPosition.x,
						vertexShaderConstants[8 * 4 + 1]
							- transformedPosition.y,
						vertexShaderConstants[8 * 4 + 2]
							- transformedPosition.z);
					viewDirection = NormalizeSafe(viewDirection);
					const GfxVector3 halfVector =
						NormalizeSafe(viewDirection + light);
					const FLOAT normalHalf = max(
						0.0f,
						GfxVec3Dot(&normal, &halfVector));
					vertex.secondaryColor[3] = powf(
						normalHalf,
						vertexShaderConstants[8 * 4 + 3])
						* vertexShaderConstants[9 * 4 + 3];
				}
				if (shaderFamily.vertex
						== DX12_LEGACY_VS_RIGID_TANGENT_PROJECTED
					|| shaderFamily.vertex
						== DX12_LEGACY_VS_SKINNED_TANGENT_PROJECTED)
				{
					WriteProjectedTexCoord(
						transformedPosition,
						vertexShaderConstants,
						18,
						2,
						&vertex);
				}
			}
		}
		else
		{
			GfxVector4 clip;
			GfxVec3Transform(&clip, &source, &worldViewProjection);
			vertex.position[0] = clip.x;
			vertex.position[1] = clip.y;
			vertex.position[2] = clip.z;
			vertex.clipW = clip.w;
			vertex.normal[2] = 1.0f;
		}
		if (!_finite(vertex.position[0])
			|| !_finite(vertex.position[1])
			|| !_finite(vertex.position[2])
			|| !_finite(vertex.clipW))
		{
			invalidClipCoordinate = true;
			break;
		}
		m_pState->vertices.push_back(vertex);
	}
	if (invalidClipCoordinate)
	{
		m_pState->vertices.resize(baseVertex);
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_INVALID_CLIP_COORDINATE];
		return false;
	}

	const UINT firstIndex =
		static_cast<UINT>(m_pState->indices.size());
	UINT capturedIndexCount = 0;
	for (UINT iIndex = 0; iIndex + 2 < indexCount; iIndex += 3)
	{
		const UINT localIndex0 = remappedIndices[iIndex + 0];
		const UINT localIndex1 = remappedIndices[iIndex + 1];
		const UINT localIndex2 = remappedIndices[iIndex + 2];
		if (!usesVertexProgram
			&& !usesPixelProgram
			&& (m_pState->vertices[baseVertex + localIndex0].clipW
					<= 0.000001f
				|| m_pState->vertices[baseVertex + localIndex1].clipW
					<= 0.000001f
				|| m_pState->vertices[baseVertex + localIndex2].clipW
					<= 0.000001f))
		{
			continue;
		}
		m_pState->indices.push_back(baseVertex + localIndex0);
		m_pState->indices.push_back(baseVertex + localIndex1);
		m_pState->indices.push_back(baseVertex + localIndex2);
		capturedIndexCount += 3;
	}
	if (capturedIndexCount == 0)
	{
		m_pState->vertices.resize(baseVertex);
		++m_pState->rejectedDrawCount;
		++m_pState->rejectedReasons[REJECT_FIXED_CLIP_VOLUME];
		return false;
	}

	IDirect3DBaseTexture9* pBaseTexture = NULL;
	IDirect3DTexture9* pTexture = NULL;
	IDirect3DTexture9* pTexture1 = NULL;
	IDirect3DTexture9* pTexture2 = NULL;
	IDirect3DTexture9* pTexture3 = NULL;
	IDirect3DTexture9** ppTextures[4] = {
		&pTexture,
		&pTexture1,
		&pTexture2,
		&pTexture3
	};
	for (UINT textureUnit = 0;
		textureUnit < shaderFamily.textureCount;
		++textureUnit)
	{
		pBaseTexture = NULL;
		if (SUCCEEDED(pDevice9->GetTexture(
			textureUnit,
			&pBaseTexture))
			&& pBaseTexture != NULL)
		{
			pBaseTexture->QueryInterface(
				__uuidof(IDirect3DTexture9),
				reinterpret_cast<void**>(ppTextures[textureUnit]));
			pBaseTexture->Release();
		}
	}
	D3DSURFACE_DESC fixedTextureDescription;
	ZeroMemory(&fixedTextureDescription, sizeof(fixedTextureDescription));
	const bool hasFixedTextureDescription =
		pTexture != NULL
		&& SUCCEEDED(pTexture->GetLevelDesc(
			0,
			&fixedTextureDescription));
	if (inventoryMode && fixedFunctionDraw)
	{
		RecordFixedFunctionDraw(
			m_pState,
			pTexture,
			hasFixedTextureDescription
				? &fixedTextureDescription
				: NULL,
			shaderFamily.textureCount,
			fixedFunctionBlendMode,
			zEnable,
			zWrite,
			zFunction,
			cullMode,
			fixedTexCoordIndex[0],
			fixedTextureTransformFlags[0],
			fixedColorOperation[0],
			fixedAlphaOperation[0],
			dynamicBuffer,
			capturedIndexCount,
			m_pState->vertices,
			baseVertex,
			usedVertexCount);
	}
	if (inventoryMode
		&& !usesVertexProgram && !usesPixelProgram
		&& m_pState->fixedDiagnosticCount < 20)
	{
		DWORD colorOperation = 0;
		DWORD colorArgument1 = 0;
		DWORD colorArgument2 = 0;
		DWORD alphaOperation = 0;
		DWORD alphaArgument1 = 0;
		DWORD alphaArgument2 = 0;
		pDevice9->GetTextureStageState(
			0, D3DTSS_COLOROP, &colorOperation);
		pDevice9->GetTextureStageState(
			0, D3DTSS_COLORARG1, &colorArgument1);
		pDevice9->GetTextureStageState(
			0, D3DTSS_COLORARG2, &colorArgument2);
		pDevice9->GetTextureStageState(
			0, D3DTSS_ALPHAOP, &alphaOperation);
		pDevice9->GetTextureStageState(
			0, D3DTSS_ALPHAARG1, &alphaArgument1);
		pDevice9->GetTextureStageState(
			0, D3DTSS_ALPHAARG2, &alphaArgument2);
		const Legacy3DVertex& firstVertex =
			m_pState->vertices[baseVertex];
		FLOAT minimumX = firstVertex.position[0];
		FLOAT maximumX = firstVertex.position[0];
		FLOAT minimumY = firstVertex.position[1];
		FLOAT maximumY = firstVertex.position[1];
		FLOAT minimumW = firstVertex.clipW;
		FLOAT maximumW = firstVertex.clipW;
		for (UINT iVertex = 1; iVertex < usedVertexCount; ++iVertex)
		{
			const Legacy3DVertex& diagnosticVertex =
				m_pState->vertices[baseVertex + iVertex];
			minimumX = minimumX < diagnosticVertex.position[0]
				? minimumX : diagnosticVertex.position[0];
			maximumX = maximumX > diagnosticVertex.position[0]
				? maximumX : diagnosticVertex.position[0];
			minimumY = minimumY < diagnosticVertex.position[1]
				? minimumY : diagnosticVertex.position[1];
			maximumY = maximumY > diagnosticVertex.position[1]
				? maximumY : diagnosticVertex.position[1];
			minimumW = minimumW < diagnosticVertex.clipW
				? minimumW : diagnosticVertex.clipW;
			maximumW = maximumW > diagnosticVertex.clipW
				? maximumW : diagnosticVertex.clipW;
		}
		CPrintF(
			"DX12 diagnostico fixed: pases=%u, textura=%p, "
			"desc=%u, formato=%d, pool=%d, tamano=%ux%u, "
			"color=%08X, z=%u/%u, blend=%u, vertices=%u, "
			"etapa0=%u/%u/%u alfa=%u/%u/%u, "
			"coord=%08X transform=%08X, "
			"fuente=%s viewport=%u,%u,%ux%u, "
			"primero=(%.4f,%.4f,%.4f,w=%.4f), "
			"limites=(%.3f..%.3f,%.3f..%.3f,w=%.3f..%.3f), "
			"uv=(%.4f,%.4f), rgba=(%.3f,%.3f,%.3f,%.3f).\n",
			shaderFamily.textureCount,
			pTexture,
			hasFixedTextureDescription ? 1U : 0U,
			static_cast<int>(fixedTextureDescription.Format),
			static_cast<int>(fixedTextureDescription.Pool),
			fixedTextureDescription.Width,
			fixedTextureDescription.Height,
			m_pState->constantColor,
			zEnable != FALSE ? 1U : 0U,
			zWrite != FALSE ? 1U : 0U,
			static_cast<UINT>(ToBlendMode(
				blending,
				alphaTest,
				sourceBlend,
				destinationBlend)),
			usedVertexCount,
			colorOperation,
			colorArgument1,
			colorArgument2,
			alphaOperation,
			alphaArgument1,
			alphaArgument2,
			fixedTexCoordIndex[0],
			fixedTextureTransformFlags[0],
			dynamicBuffer ? "dinamica" : "estatica",
			viewport9.X,
			viewport9.Y,
			viewport9.Width,
			viewport9.Height,
			firstVertex.position[0],
			firstVertex.position[1],
			firstVertex.position[2],
			firstVertex.clipW,
			minimumX,
			maximumX,
			minimumY,
			maximumY,
			minimumW,
			maximumW,
			firstVertex.texCoord[0],
			firstVertex.texCoord[1],
			firstVertex.color[0],
			firstVertex.color[1],
			firstVertex.color[2],
			firstVertex.color[3]);
		++m_pState->fixedDiagnosticCount;
	}

	Legacy3DDrawRange range;
	range.firstIndex = firstIndex;
	range.indexCount = capturedIndexCount;
	range.viewport.TopLeftX = static_cast<FLOAT>(viewport9.X);
	range.viewport.TopLeftY = static_cast<FLOAT>(viewport9.Y);
	range.viewport.Width = static_cast<FLOAT>(viewport9.Width);
	range.viewport.Height = static_cast<FLOAT>(viewport9.Height);
	range.viewport.MinDepth = viewport9.MinZ;
	range.viewport.MaxDepth = viewport9.MaxZ;
	range.scissor.left = static_cast<LONG>(viewport9.X);
	range.scissor.top = static_cast<LONG>(viewport9.Y);
	range.scissor.right =
		static_cast<LONG>(viewport9.X + viewport9.Width);
	range.scissor.bottom =
		static_cast<LONG>(viewport9.Y + viewport9.Height);
	range.pTexture = pTexture;
	range.pTexture1 = pTexture1;
	range.pTexture2 = pTexture2;
	range.pTexture3 = pTexture3;
	range.textureHandle = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(pTexture);
	range.textureHandle1 = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(pTexture1);
	range.textureHandle2 = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(pTexture2);
	range.textureHandle3 = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(pTexture3);
	range.renderTextureHandle = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_RENDER_TEXTURE>(pTexture);
	range.renderTextureHandle1 = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_RENDER_TEXTURE>(pTexture1);
	range.renderTextureHandle2 = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_RENDER_TEXTURE>(pTexture2);
	range.renderTextureHandle3 = GetDirectX12ResourceRegistry().
		ResolveLegacyAlias<DX12_RESOURCE_RENDER_TEXTURE>(pTexture3);
	range.depthEnabled = zEnable != FALSE;
	range.depthWriteEnabled = zWrite != FALSE;
	range.colorWriteEnabled = colorWriteMask != 0;
	// D3D9 permite desactivar el recorte del volumen de vista. DX12 separa
	// esa compatibilidad en DepthClipEnable del PSO; conservarla evita que
	// los bloques periféricos del terreno desaparezcan en el plano lejano.
	range.depthClipEnabled = clipping != FALSE;
	// Estas familias reproducen el vertex shader D3D9 en CPU. La diferencia
	// de precisión deja parte de cada bloque apenas fuera del plano lejano
	// (medido hasta z/w=1.000096) aunque D3D9 conserva la superficie. Sólo
	// para esta geometría desactivamos el recorte de profundidad del PSO;
	// X/Y siguen siendo recortados normalmente por el rasterizador DX12.
	if (projectedTerrain
		|| shaderFamily.pixel == DX12_LEGACY_PS_WATER)
		range.depthClipEnabled = false;
	range.depthFunction = ToDepthFunction(zFunction);
	// Las capas del terreno vuelven a rasterizar la misma malla con shaders
	// distintos. D3D9 toleraba pequeñas diferencias de profundidad entre
	// esas pasadas; EQUAL exacto en DX12 dejaba capas completas sin componer.
	// LESS_EQUAL conserva la oclusión frente a objetos y admite la misma
	// superficie cuando la conversión CPU varía en el último bit.
	if ((shaderFamily.vertex == DX12_LEGACY_VS_PROJECTED_ONE
			|| shaderFamily.vertex == DX12_LEGACY_VS_PROJECTED_TWO
			|| shaderFamily.vertex == DX12_LEGACY_VS_PROJECTED_FOUR)
		&& range.depthFunction == D3D12_COMPARISON_FUNC_EQUAL
		&& !range.depthWriteEnabled)
	{
		range.depthFunction = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}
	range.cullMode = ToCullMode(cullMode);
	if (projectedTerrain
		&& ReadTerrainRasterDebugMode(
			"LASTCHAOS_DX12_3D_TERRAIN_DISABLE_DEPTH"))
	{
		range.depthEnabled = false;
		range.depthWriteEnabled = false;
	}
	if (projectedTerrain
		&& ReadTerrainRasterDebugMode(
			"LASTCHAOS_DX12_3D_TERRAIN_DISABLE_CULL"))
	{
		range.cullMode = D3D12_CULL_MODE_NONE;
	}
	range.blendMode = fixedFunctionDraw
		? fixedFunctionBlendMode
		: ToBlendMode(
			blending,
			alphaTest,
			sourceBlend,
			destinationBlend);
	// La primera capa ya produce color premultiplicado. Escribirla sin mezcla
	// inicializa el destino de forma determinista; depender de alfa=0 con
	// ONE/SRC_ALPHA conservaba partes del cielo en algunos controladores.
	// Las capas posteriores mantienen el blending multipass original.
	if (initializeTerrainColor)
	{
		range.blendMode = DX12_BLEND_OPAQUE;
		if (!m_pState->terrainOpaqueInitializationReported)
		{
			CPrintF(
				"DX12 terreno: primera capa configurada como "
				"inicializacion opaca; las capas siguientes conservan "
				"ONE/SRC_ALPHA.\n");
			m_pState->terrainOpaqueInitializationReported = true;
		}
	}
	if (inventoryMode
		&& fixedFunctionDraw
		&& range.blendMode < DX12_BLEND_COUNT
		&& !m_pState->fixedBlendDiagnosticReported[range.blendMode])
	{
		D3DSURFACE_DESC fixedTextureDescription;
		ZeroMemory(
			&fixedTextureDescription,
			sizeof(fixedTextureDescription));
		const bool fixedTextureDescriptionAvailable =
			pTexture != NULL
			&& SUCCEEDED(pTexture->GetLevelDesc(
				0,
				&fixedTextureDescription));
		CPrintF(
			"DX12 diagnostico fixed blend: modo=%u, "
			"origen=%u, destino=%u, blend=%u, alphaTest=%u, "
			"z=%u/%u, pases=%u, pasesUV=%u, vertices=%u, indices=%u, "
			"formato=%d, tamano=%ux%u, etapa0=%u/%u/%u "
			"alfa=%u/%u/%u.\n",
			static_cast<UINT>(range.blendMode),
			sourceBlend,
			destinationBlend,
			blending != FALSE ? 1U : 0U,
			alphaTest != FALSE ? 1U : 0U,
			zEnable != FALSE ? 1U : 0U,
			zWrite != FALSE ? 1U : 0U,
			shaderFamily.textureCount,
			texturePassCount,
			usedVertexCount,
			capturedIndexCount,
			fixedTextureDescriptionAvailable
				? static_cast<int>(fixedTextureDescription.Format)
				: 0,
			fixedTextureDescription.Width,
			fixedTextureDescription.Height,
			fixedColorOperation[0],
			fixedColorArgument1[0],
			fixedColorArgument2[0],
			fixedAlphaOperation[0],
			fixedAlphaArgument1[0],
			fixedAlphaArgument2[0]);
		m_pState->fixedBlendDiagnosticReported[range.blendMode] = true;
	}
	range.opaque = blending == FALSE && alphaTest == FALSE;
	range.rigidLit = rigidLit;
	range.genericFamily = !shaderFamily.nativeRigidPipeline;
	range.samplerMode = ToSamplerMode(
		samplerAddressU,
		samplerAddressV,
		samplerMinification,
		samplerMagnification);
	CopyMemory(
		range.shaderConstants,
		shaderConstants,
		sizeof(range.shaderConstants));
	CopyMemory(
		range.pixelShaderConstants,
		pixelShaderConstants,
		sizeof(range.pixelShaderConstants));
	if (projectedTerrain)
	{
		CCameraTestCapture::CaptureTerrainView(
			pDevice9,
			vertexShaderConstants,
			96);
		ReportTerrainClipStateOnce(
			m_pState,
			vertexShaderFingerprint,
			pixelShaderFingerprint,
			clipping != FALSE,
			range.depthClipEnabled,
			range.depthEnabled,
			range.depthWriteEnabled,
			range.colorWriteEnabled,
			range.depthFunction,
			alphaTest != FALSE,
			static_cast<FLOAT>(alphaReference) / 255.0f,
			blending,
			sourceBlend,
			destinationBlend,
			samplerAddressU,
			samplerAddressV,
			samplerMinification,
			samplerMagnification,
			legacyPixelConstants,
			pTexture,
			pTexture1,
			pTexture2,
			pTexture3,
			m_pState->vertices,
			baseVertex,
			usedVertexCount,
			clampedFarDepthCount);
	}
	m_pState->ranges.push_back(range);
	++m_pState->capturedDrawCount;
	m_pState->capturedTriangleCount += capturedIndexCount / 3;
	return true;
}

bool CDirectX12Legacy3DCommandBatch::EnsureBuffers(
	UINT vertexBytes,
	UINT indexBytes)
{
	if (m_pDevice == NULL || vertexBytes == 0 || indexBytes == 0)
		return false;
	if (m_currentSubmissionBuffer >= BUFFER_SUBMISSION_COUNT)
		return false;
	CDirectX12Buffer*& pVertexBuffer =
		m_pVertexBuffers[m_currentFrame][m_currentSubmissionBuffer];
	CDirectX12Buffer*& pIndexBuffer =
		m_pIndexBuffers[m_currentFrame][m_currentSubmissionBuffer];
	if (pVertexBuffer == NULL
		|| pVertexBuffer->GetSize() < vertexBytes)
	{
		delete pVertexBuffer;
		pVertexBuffer = new CDirectX12Buffer;
		if (pVertexBuffer == NULL
			|| !pVertexBuffer->CreateVertexBuffer(
				m_pDevice,
				vertexBytes,
				sizeof(Legacy3DVertex)))
			return false;
	}
	if (pIndexBuffer == NULL
		|| pIndexBuffer->GetSize() < indexBytes)
	{
		delete pIndexBuffer;
		pIndexBuffer = new CDirectX12Buffer;
		if (pIndexBuffer == NULL
			|| !pIndexBuffer->CreateIndexBuffer(
				m_pDevice,
				indexBytes,
				DXGI_FORMAT_R32_UINT))
			return false;
	}
	return true;
}

bool CDirectX12Legacy3DCommandBatch::RenderLegacy3DPass(
	ID3D12GraphicsCommandList* pCommandList,
	CDirectX12RenderTargetManager* pRenderTargets,
	CDirectX12UploadManager* pUploadManager,
	CDirectX12InteropTextureManager* pTextures,
	const DirectX12DescriptorHandle* pSamplers,
	D3D12_GPU_DESCRIPTOR_HANDLE fallbackTexture)
{
	if (m_pState == NULL
		|| m_pState->submittedRangeCount >= m_pState->ranges.size())
		return true;
	if (pCommandList == NULL || pRenderTargets == NULL
		|| pUploadManager == NULL || pTextures == NULL
		|| pSamplers == NULL)
		return false;

	const UINT vertexBytes = static_cast<UINT>(
		m_pState->vertices.size() * sizeof(Legacy3DVertex));
	const UINT indexBytes = static_cast<UINT>(
		m_pState->indices.size() * sizeof(UINT));
	if (!EnsureBuffers(vertexBytes, indexBytes))
		return false;
	CDirectX12Buffer* pVertexBuffer =
		m_pVertexBuffers[m_currentFrame][m_currentSubmissionBuffer];
	CDirectX12Buffer* pIndexBuffer =
		m_pIndexBuffers[m_currentFrame][m_currentSubmissionBuffer];
	if (pVertexBuffer == NULL || pIndexBuffer == NULL
		|| !pVertexBuffer->Upload(
			pUploadManager,
			pCommandList,
			&m_pState->vertices[0],
			vertexBytes)
		|| !pIndexBuffer->Upload(
			pUploadManager,
			pCommandList,
			&m_pState->indices[0],
			indexBytes))
		return false;

	const D3D12_RESOURCE_DESC targetDesc =
		pRenderTargets->GetCurrentResource()->GetDesc();
	const bool overlay = ReadOverlayComparisonMode();
	const bool nativeOffscreen =
		pRenderTargets->IsNativeRenderTarget();
	const bool writeColor = overlay || nativeOffscreen;
	// El inventario debe limitarse a capturar y clasificar comandos. Aunque el
	// PSO de diagnostico no escriba color, abrir una pasada DX12 sobre el
	// recurso compartido puede reemplazar segmentos D3D9 aun no compuestos.
	// El mapa nativo, en cambio, reproduce Shadow y NoShadow para generar la
	// silueta que posteriormente se proyecta sobre el mundo.
	if (!writeColor)
		return true;
	const bool useInteropDepth =
		overlay && pRenderTargets->HasAcquiredDepth();
	if (!useInteropDepth
		&& (m_pDepthBuffer == NULL
			|| !m_pDepthBuffer->EnsureCompatible(targetDesc)))
		return false;

	const D3D12_CPU_DESCRIPTOR_HANDLE renderTarget =
		pRenderTargets->GetCurrentView();
	const D3D12_CPU_DESCRIPTOR_HANDLE depthTarget =
		useInteropDepth
			? pRenderTargets->GetCurrentDepthView()
			: m_pDepthBuffer->GetView();
	const DXGI_FORMAT depthStencilFormat = useInteropDepth
		? ToDepthStencilViewFormat(
			pRenderTargets->GetCurrentDepthResource()->GetDesc().Format)
		: DXGI_FORMAT_D32_FLOAT;
	const D3D12_VERTEX_BUFFER_VIEW vertexView =
		pVertexBuffer->GetVertexView();
	const D3D12_INDEX_BUFFER_VIEW indexView =
		pIndexBuffer->GetIndexView();
	pCommandList->SetGraphicsRootSignature(
		m_pPipelineCache->GetRootSignature());
	pCommandList->OMSetRenderTargets(
		1,
		&renderTarget,
		FALSE,
		&depthTarget);
	ID3D12Resource* pCurrentDepthTarget =
		pRenderTargets->GetCurrentResource();
	const bool startsNewPrivateDepth =
		!useInteropDepth
		&& (m_pState->pNativeDepthTarget != pCurrentDepthTarget
			|| (nativeOffscreen
				&& pRenderTargets->ShouldClearNativeDepth()));
	if (startsNewPrivateDepth)
	{
		pCommandList->ClearDepthStencilView(
			depthTarget,
			D3D12_CLEAR_FLAG_DEPTH,
			1.0f,
			0,
			0,
			NULL);
		m_pState->pNativeDepthTarget = pCurrentDepthTarget;
	}
	pCommandList->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexView);
	pCommandList->IASetIndexBuffer(&indexView);

	for (size_t iRange = m_pState->submittedRangeCount;
		iRange < m_pState->ranges.size();
		++iRange)
	{
		const Legacy3DDrawRange& range = m_pState->ranges[iRange];
		const bool rangeWritesColor =
			writeColor && range.colorWriteEnabled;
		ID3D12Resource* pCurrentTarget =
			pRenderTargets->GetCurrentResource();
		// Una textura no puede estar enlazada simultaneamente como destino y
		// como entrada. Omitir este draw dentro de la propia reflexion evita
		// la realimentacion RTV/SRV sin introducir una ruta D3D9 alternativa.
		if (ReferencesRenderTarget(
				pTextures,
				range.renderTextureHandle,
				range.pTexture,
				pCurrentTarget)
			|| ReferencesRenderTarget(
				pTextures,
				range.renderTextureHandle1,
				range.pTexture1,
				pCurrentTarget)
			|| ReferencesRenderTarget(
				pTextures,
				range.renderTextureHandle2,
				range.pTexture2,
				pCurrentTarget)
			|| ReferencesRenderTarget(
				pTextures,
				range.renderTextureHandle3,
				range.pTexture3,
				pCurrentTarget))
			continue;
		ID3D12PipelineState* pPipeline =
			m_pPipelineCache->GetPipelineState(
				range.genericFamily
					? (rangeWritesColor
						? DX12_PIPELINE_LEGACY_MATERIAL_3D_OVERLAY
						: DX12_PIPELINE_LEGACY_MATERIAL_3D_SHADOW)
					: range.rigidLit
					? (rangeWritesColor
						? DX12_PIPELINE_RIGID_LIT_3D_OVERLAY
						: DX12_PIPELINE_RIGID_LIT_3D_SHADOW)
					: (rangeWritesColor
						? DX12_PIPELINE_TEXTURED_3D_OVERLAY
						: DX12_PIPELINE_TEXTURED_3D_SHADOW),
				targetDesc.Format,
				targetDesc.SampleDesc,
				range.blendMode,
				range.depthEnabled,
				range.depthWriteEnabled,
				range.depthFunction,
				range.cullMode,
				depthStencilFormat,
				range.depthClipEnabled);
		if (pPipeline == NULL)
			return false;
		pCommandList->SetPipelineState(pPipeline);
		if (!pSamplers[range.samplerMode].IsValid())
			return false;
		pCommandList->SetGraphicsRootDescriptorTable(
			1,
			pSamplers[range.samplerMode].gpu);
		if (range.rigidLit || range.genericFamily)
		{
			pCommandList->SetGraphicsRoot32BitConstants(
				2,
				RIGID_LIT_CONSTANT_COUNT,
				range.shaderConstants,
				0);
		}
		D3D12_GPU_DESCRIPTOR_HANDLE textureView = fallbackTexture;
		D3D12_GPU_DESCRIPTOR_HANDLE textureView1 = fallbackTexture;
		D3D12_GPU_DESCRIPTOR_HANDLE textureView2 = fallbackTexture;
		D3D12_GPU_DESCRIPTOR_HANDLE textureView3 = fallbackTexture;
		// Las pasadas de borde del terreno no escriben color, pero su
		// alpha-test decide qué fragmentos pueden escribir profundidad. Deben
		// conservar la máscara real; usar la textura blanca de respaldo crea
		// un oclusor invisible sobre toda la baldosa.
		const bool samplesTextureForDepth =
			!rangeWritesColor
			&& range.pixelShaderConstants[2] > 0.5f;
		const bool needsTextureSampling =
			rangeWritesColor || samplesTextureForDepth;
		if (needsTextureSampling && range.pTexture != NULL
			&& !AcquireTexture(
				pTextures,
				range.textureHandle,
				range.renderTextureHandle,
				range.pTexture,
				pCommandList,
				pUploadManager,
				&textureView))
			return false;
		pCommandList->SetGraphicsRootDescriptorTable(0, textureView);
		if ((range.rigidLit || range.genericFamily)
			&& rangeWritesColor && range.pTexture1 != NULL
			&& !AcquireTexture(
				pTextures,
				range.textureHandle1,
				range.renderTextureHandle1,
				range.pTexture1,
				pCommandList,
				pUploadManager,
				&textureView1))
			return false;
		if (range.rigidLit || range.genericFamily)
		{
			pCommandList->SetGraphicsRootDescriptorTable(3, textureView1);
			pCommandList->SetGraphicsRoot32BitConstants(
				4,
				RIGID_LIT_PIXEL_CONSTANT_COUNT,
				range.pixelShaderConstants,
				0);
		}
		if (range.genericFamily && rangeWritesColor
			&& range.pTexture2 != NULL
			&& !AcquireTexture(
				pTextures,
				range.textureHandle2,
				range.renderTextureHandle2,
				range.pTexture2,
				pCommandList,
				pUploadManager,
				&textureView2))
			return false;
		if (range.genericFamily && rangeWritesColor
			&& range.pTexture3 != NULL
			&& !AcquireTexture(
				pTextures,
				range.textureHandle3,
				range.renderTextureHandle3,
				range.pTexture3,
				pCommandList,
				pUploadManager,
				&textureView3))
			return false;
		pCommandList->SetGraphicsRootDescriptorTable(5, textureView2);
		pCommandList->SetGraphicsRootDescriptorTable(6, textureView3);
		pCommandList->RSSetViewports(1, &range.viewport);
		pCommandList->RSSetScissorRects(1, &range.scissor);
		pCommandList->DrawIndexedInstanced(
			range.indexCount,
			1,
			range.firstIndex,
			0,
			0);
	}
	m_pState->submittedRangeCount = m_pState->ranges.size();
	++m_currentSubmissionBuffer;
	return true;
}

UINT CDirectX12Legacy3DCommandBatch::GetCapturedDrawCount() const
{
	return m_pState != NULL ? m_pState->capturedDrawCount : 0;
}

bool CDirectX12Legacy3DCommandBatch::HasPendingDraws() const
{
	return m_pState != NULL
		&& m_pState->submittedRangeCount < m_pState->ranges.size();
}

UINT CDirectX12Legacy3DCommandBatch::GetRejectedDrawCount() const
{
	return m_pState != NULL ? m_pState->rejectedDrawCount : 0;
}

UINT CDirectX12Legacy3DCommandBatch::GetCapturedTriangleCount() const
{
	return m_pState != NULL ? m_pState->capturedTriangleCount : 0;
}

UINT CDirectX12Legacy3DCommandBatch::GetRejectedReasonCount(
	RejectionReason reason) const
{
	return m_pState != NULL && reason >= 0 && reason < REJECT_REASON_COUNT
		? m_pState->rejectedReasons[reason]
		: 0;
}

UINT64 CDirectX12Legacy3DCommandBatch::GetTopVertexShaderFingerprint() const
{
	const Legacy3DVertexShaderFamily* pTop =
		GetTopVertexShaderFamily(m_pState);
	return pTop != NULL ? pTop->fingerprint : 0;
}

UINT CDirectX12Legacy3DCommandBatch::GetTopVertexShaderDrawCount() const
{
	const Legacy3DVertexShaderFamily* pTop =
		GetTopVertexShaderFamily(m_pState);
	return pTop != NULL ? pTop->drawCount : 0;
}

UINT CDirectX12Legacy3DCommandBatch::GetTopVertexShaderTriangleCount() const
{
	const Legacy3DVertexShaderFamily* pTop =
		GetTopVertexShaderFamily(m_pState);
	return pTop != NULL ? pTop->triangleCount : 0;
}

bool CDirectX12Legacy3DCommandBatch::IsOverlayComparisonEnabled() const
{
	return ReadOverlayComparisonMode();
}

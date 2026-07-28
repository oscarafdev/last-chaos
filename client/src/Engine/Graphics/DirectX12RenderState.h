#ifndef SE_INCL_DIRECTX12RENDERSTATE_H
#define SE_INCL_DIRECTX12RENDERSTATE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>
#include <Engine/Graphics/DirectX12ResourceHandle.h>

// Los envios parciales conservan el orden entre draws nativos y fallbacks
// D3D9On12. Terreno, agua y UI pueden cruzar varias barreras en un solo
// frame, por lo que backend y lotes 3D deben compartir exactamente la misma
// capacidad de recursos en vuelo.
enum
{
	DX12_FRAME_COUNT = 3,
	DX12_MAX_SUBMISSIONS_PER_FRAME = 128
};

enum DirectX12BlendMode
{
	DX12_BLEND_OPAQUE,
	DX12_BLEND_ALPHA_TEST,
	DX12_BLEND_ALPHA,
	DX12_BLEND_SHADE,
	DX12_BLEND_ADD,
	DX12_BLEND_ADD_ALPHA,
	DX12_BLEND_MULTIPLY,
	DX12_BLEND_INVERSE_MULTIPLY,
	DX12_BLEND_TERRAIN_LAYER,
	// Sombras proyectadas: conserva solamente el destino atenuado por el
	// alfa calculado en la pasada, sin escribir el color de la textura.
	DX12_BLEND_DESTINATION_INVERSE_SOURCE_ALPHA,
	DX12_BLEND_COUNT
};

enum DirectX12SamplerMode
{
	DX12_SAMPLER_POINT_CLAMP,
	DX12_SAMPLER_POINT_REPEAT,
	DX12_SAMPLER_LINEAR_CLAMP,
	DX12_SAMPLER_LINEAR_REPEAT,
	DX12_SAMPLER_ANISOTROPIC_CLAMP,
	DX12_SAMPLER_ANISOTROPIC_REPEAT,
	DX12_SAMPLER_COUNT
};

enum DirectX12DrawPortScope
{
	DX12_DRAWPORT_SCOPE_DEFAULT,
	DX12_DRAWPORT_SCOPE_UI
};

enum DirectX12DrawPortValidationMode
{
	DX12_DRAWPORT_VALIDATION_SHADOW,
	DX12_DRAWPORT_VALIDATION_UI_OVERLAY,
	DX12_DRAWPORT_VALIDATION_UI_SPLIT,
	DX12_DRAWPORT_VALIDATION_UI_REPLACE
};

enum DirectX12DrawPortBarrierKind
{
	DX12_DRAWPORT_BARRIER_RENDER_TARGET_BEGIN,
	DX12_DRAWPORT_BARRIER_RENDER_TARGET_END
};

// Distingue el backbuffer visible de los destinos auxiliares del renderer
// heredado. Los destinos auxiliares requieren una reproducción DX12 propia.
enum DirectX12LegacyRenderTargetKind
{
	DX12_LEGACY_RENDER_TARGET_UNKNOWN,
	DX12_LEGACY_RENDER_TARGET_PRESENTATION,
	DX12_LEGACY_RENDER_TARGET_OFFSCREEN
};

#endif

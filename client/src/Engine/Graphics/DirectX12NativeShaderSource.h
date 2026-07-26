#ifndef SE_INCL_DIRECTX12NATIVESHADERSOURCE_H
#define SE_INCL_DIRECTX12NATIVESHADERSOURCE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

// Devuelve el HLSL compartido por la primera pasada de validación nativa.
const char* GetDirectX12NativeValidationShader();
const char* GetDirectX12Textured2DShader();
const char* GetDirectX12TexturedAlphaTest2DShader();
const char* GetDirectX12BloomShader();
const char* GetDirectX12Legacy3DShader();
const char* GetDirectX12RigidLit3DShader();
const char* GetDirectX12LegacyMaterial3DShader();

#endif

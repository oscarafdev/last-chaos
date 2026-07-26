#ifndef SE_INCL_SHADERVALIDATIONHOOKS_H
#define SE_INCL_SHADERVALIDATIONHOOKS_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

// Funciones optativas para construir escenas de validación deterministas.
// No se invocan durante una ejecución normal del cliente.
ENGINE_API void PrepareNormalMapFogShaderValidation();
ENGINE_API void PrepareNormalMapRigidShaderValidation();

#endif

#ifndef SE_INCL_LEGACYD3D9SHADERBYTECODE_H
#define SE_INCL_LEGACYD3D9SHADERBYTECODE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <stddef.h>

enum class LegacyD3D9ShaderKind
{
  Vertex,
  Pixel,
};

struct LegacyD3D9ShaderBytecodeView
{
  const DWORD *Data;
  size_t Size;
};

ENGINE_API LegacyD3D9ShaderBytecodeView
FindLegacyD3D9ShaderBytecode(
  const char *source,
  LegacyD3D9ShaderKind kind);

#endif

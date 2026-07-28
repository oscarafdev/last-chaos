#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Graphics/LegacyD3D9ShaderBytecode.h>

namespace
{
struct LegacyD3D9ShaderBytecodeEntry
{
  unsigned long long SourceFingerprint;
  unsigned int SourceSize;
  LegacyD3D9ShaderKind Kind;
  const DWORD *Data;
  size_t Size;
};

#include <Engine/Graphics/Generated/LegacyD3D9ShaderBytecode.h>

unsigned long long HashSource(const char *source, size_t size)
{
  const unsigned long long offset = 14695981039346656037ULL;
  const unsigned long long prime = 1099511628211ULL;
  unsigned long long value = offset;
  for (size_t index = 0; index < size; ++index) {
    value ^= static_cast<unsigned char>(source[index]);
    value *= prime;
  }
  return value;
}
}

LegacyD3D9ShaderBytecodeView FindLegacyD3D9ShaderBytecode(
  const char *source,
  LegacyD3D9ShaderKind kind)
{
  LegacyD3D9ShaderBytecodeView missing = { NULL, 0 };
  if (source == NULL) {
    return missing;
  }

  const size_t sourceSize = strlen(source);
  const unsigned long long fingerprint = HashSource(source, sourceSize);
  const size_t count = sizeof(LEGACY_SHADER_BYTECODE_CATALOG)
                     / sizeof(LEGACY_SHADER_BYTECODE_CATALOG[0]);
  for (size_t index = 0; index < count; ++index) {
    const LegacyD3D9ShaderBytecodeEntry &entry =
      LEGACY_SHADER_BYTECODE_CATALOG[index];
    if (entry.SourceFingerprint == fingerprint
     && entry.SourceSize == sourceSize
     && entry.Kind == kind) {
      LegacyD3D9ShaderBytecodeView found = { entry.Data, entry.Size };
      return found;
    }
  }

  CPrintF(
    "DX12 error: legacy %s shader source is absent from the offline catalog "
    "(fingerprint=%016llX, size=%u).\n",
    kind == LegacyD3D9ShaderKind::Vertex ? "vertex" : "pixel",
    fingerprint,
    static_cast<unsigned int>(sourceSize));
  return missing;
}

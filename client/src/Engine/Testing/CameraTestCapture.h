#ifndef SE_INCL_CAMERATESTCAPTURE_H
#define SE_INCL_CAMERATESTCAPTURE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Base/CTString.h>

// Captura local para reproducir defectos visuales. No envía datos al servidor
// ni requiere privilegios GM.
class ENGINE_API CCameraTestCapture
{
public:
	static void Request(const CTString& requestedName);
	static void PollRequest();
	static void CaptureTerrainView(
		const FLOAT* viewMatrix,
		const FLOAT* projectionMatrix,
		DWORD viewportX,
		DWORD viewportY,
		DWORD viewportWidth,
		DWORD viewportHeight,
		FLOAT viewportMinimumDepth,
		FLOAT viewportMaximumDepth,
		const FLOAT* vertexShaderConstants,
		UINT constantCount);

private:
	static BOOL Save(
		const CTString& requestedName,
		const FLOAT* viewMatrix,
		const FLOAT* projectionMatrix,
		DWORD viewportX,
		DWORD viewportY,
		DWORD viewportWidth,
		DWORD viewportHeight,
		FLOAT viewportMinimumDepth,
		FLOAT viewportMaximumDepth,
		const FLOAT* vertexShaderConstants,
		UINT constantCount,
		CTString& outputPath,
		CTString& errorMessage);
};

#endif

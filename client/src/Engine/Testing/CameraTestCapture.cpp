#include "StdH.h"

#include <Engine/Testing/CameraTestCapture.h>

#include <Engine/Entities/Entity.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Network/CNetwork.h>

#include <ctype.h>
#include <stdio.h>

namespace
{
	const char* CAPTURE_REQUEST_FILE = "dx12-camera-capture.request";
	const char* CAPTURE_DIRECTORY = "dx12-camera-captures";
	CTString pendingCaptureName;
	CTString pendingRequestPath;

	CTString SanitizeCaptureName(const CTString& requestedName)
	{
		const char* source = requestedName;
		char sanitized[49];
		ZeroMemory(sanitized, sizeof(sanitized));
		INDEX destination = 0;
		for (INDEX index = 0;
			source != NULL && source[index] != '\0'
				&& destination < 48;
			++index)
		{
			const unsigned char character =
				static_cast<unsigned char>(source[index]);
			if (isalnum(character) || character == '-'
				|| character == '_')
				sanitized[destination++] = static_cast<char>(character);
		}
		return destination > 0 ? CTString(sanitized) : CTString("last");
	}

	BOOL IsDirectory(const CTString& path)
	{
		const DWORD attributes = GetFileAttributesA(path);
		return attributes != INVALID_FILE_ATTRIBUTES
			&& (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}

	BOOL FindConfigurationDirectory(CTString& configurationDirectory)
	{
		char currentDirectory[MAX_PATH];
		if (GetCurrentDirectoryA(MAX_PATH, currentDirectory) == 0)
			return FALSE;

		CTString candidate = currentDirectory;
		for (INDEX depth = 0; depth < 6; ++depth)
		{
			CTString configuration;
			configuration.PrintF(
				"%s\\.itconfig",
				static_cast<const char*>(candidate));
			if (IsDirectory(configuration))
			{
				configurationDirectory = configuration;
				return TRUE;
			}

			char parent[MAX_PATH];
			strcpy_s(parent, static_cast<const char*>(candidate));
			char* separator = strrchr(parent, '\\');
			if (separator == NULL)
				break;
			*separator = '\0';
			candidate = parent;
		}
		return FALSE;
	}

	void WriteVector(FILE* file, const FLOAT3D& vector)
	{
		fprintf(
			file,
			"[%.9g, %.9g, %.9g]",
			vector(1),
			vector(2),
			vector(3));
	}

	void WriteAngles(FILE* file, const ANGLE3D& angles)
	{
		fprintf(
			file,
			"[%.9g, %.9g, %.9g]",
			angles(1),
			angles(2),
			angles(3));
	}

	void WritePlacement(FILE* file, const CPlacement3D& placement)
	{
		fprintf(file, "{\"position\": ");
		WriteVector(file, placement.pl_PositionVector);
		fprintf(file, ", \"orientation\": ");
		WriteAngles(file, placement.pl_OrientationAngle);
		fprintf(file, "}");
	}

	void WriteMatrix(FILE* file, const D3DMATRIX& matrix)
	{
		fprintf(file, "[");
		const FLOAT* values = reinterpret_cast<const FLOAT*>(&matrix);
		for (INDEX index = 0; index < 16; ++index)
		{
			if (index > 0)
				fprintf(file, ", ");
			fprintf(file, "%.9g", values[index]);
		}
		fprintf(file, "]");
	}
}

void CCameraTestCapture::Request(const CTString& requestedName)
{
	pendingCaptureName = SanitizeCaptureName(requestedName);
	pendingRequestPath = "";
	CPrintF(
		"DX12 captura de cámara solicitada: se guardará en el "
		"próximo draw de terreno.\n");
}

void CCameraTestCapture::PollRequest()
{
	static ULONG lastPoll = 0;
	const ULONG now = GetTickCount();
	if (now - lastPoll < 250UL)
		return;
	lastPoll = now;

	if (pendingCaptureName.Length() > 0)
		return;

	CTString configurationDirectory;
	if (!FindConfigurationDirectory(configurationDirectory))
		return;

	CTString requestPath;
	requestPath.PrintF(
		"%s\\%s",
		static_cast<const char*>(configurationDirectory),
		CAPTURE_REQUEST_FILE);
	FILE* request = fopen(requestPath, "rb");
	if (request == NULL)
		return;

	char requestedName[64];
	ZeroMemory(requestedName, sizeof(requestedName));
	fgets(requestedName, sizeof(requestedName), request);
	fclose(request);
	for (INDEX index = 0; requestedName[index] != '\0'; ++index)
	{
		if (requestedName[index] == '\r'
			|| requestedName[index] == '\n')
		{
			requestedName[index] = '\0';
			break;
		}
	}

	pendingCaptureName = SanitizeCaptureName(requestedName);
	pendingRequestPath = requestPath;
	CPrintF(
		"DX12 captura de cámara pendiente: se guardará en el "
		"próximo draw de terreno.\n");
}

void CCameraTestCapture::CaptureTerrainView(
	const FLOAT* viewMatrix,
	const FLOAT* projectionMatrix,
	DWORD viewportX,
	DWORD viewportY,
	DWORD viewportWidth,
	DWORD viewportHeight,
	FLOAT viewportMinimumDepth,
	FLOAT viewportMaximumDepth,
	const FLOAT* vertexShaderConstants,
	UINT constantCount)
{
	if (pendingCaptureName.Length() == 0)
		return;

	CTString outputPath;
	CTString errorMessage;
	if (Save(
			pendingCaptureName,
			viewMatrix,
			projectionMatrix,
			viewportX,
			viewportY,
			viewportWidth,
			viewportHeight,
			viewportMinimumDepth,
			viewportMaximumDepth,
			vertexShaderConstants,
			constantCount,
			outputPath,
			errorMessage))
	{
		if (pendingRequestPath.Length() > 0)
			DeleteFileA(pendingRequestPath);
		pendingCaptureName = "";
		pendingRequestPath = "";
		CPrintF(
			"DX12 captura de cámara guardada durante terreno: %s\n",
			static_cast<const char*>(outputPath));
	}
	else
	{
		CPrintF(
			"DX12 captura de cámara pendiente: %s\n",
			static_cast<const char*>(errorMessage));
	}
}

BOOL CCameraTestCapture::Save(
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
	CTString& errorMessage)
{
	outputPath = "";
	errorMessage = "";
	if (_pNetwork == NULL)
	{
		errorMessage = "la red del cliente todavía no está disponible";
		return FALSE;
	}

	CPlayerEntity* player =
		static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0));
	if (player == NULL)
	{
		errorMessage = "el personaje local todavía no está disponible";
		return FALSE;
	}

	CTString configurationDirectory;
	if (!FindConfigurationDirectory(configurationDirectory))
	{
		errorMessage = "no se encontró la carpeta .itconfig";
		return FALSE;
	}
	CTString captureDirectory;
	captureDirectory.PrintF(
		"%s\\%s",
		static_cast<const char*>(configurationDirectory),
		CAPTURE_DIRECTORY);
	if (!CreateDirectoryA(captureDirectory, NULL)
		&& GetLastError() != ERROR_ALREADY_EXISTS)
	{
		errorMessage = "no se pudo crear la carpeta de capturas";
		return FALSE;
	}

	const CTString captureName = SanitizeCaptureName(requestedName);
	outputPath.PrintF(
		"%s\\camera-%s.json",
		static_cast<const char*>(captureDirectory),
		static_cast<const char*>(captureName));
	FILE* file = fopen(outputPath, "wb");
	if (file == NULL)
	{
		errorMessage = "no se pudo abrir el archivo de captura";
		return FALSE;
	}

	D3DMATRIX view;
	D3DMATRIX projection;
	D3DVIEWPORT9 viewport;
	ZeroMemory(&view, sizeof(view));
	ZeroMemory(&projection, sizeof(projection));
	ZeroMemory(&viewport, sizeof(viewport));
	const bool viewAvailable = viewMatrix != NULL;
	const bool projectionAvailable = projectionMatrix != NULL;
	const bool viewportAvailable =
		viewportWidth > 0 && viewportHeight > 0;
	if (viewAvailable)
		CopyMemory(&view, viewMatrix, sizeof(view));
	if (projectionAvailable)
		CopyMemory(&projection, projectionMatrix, sizeof(projection));
	viewport.X = viewportX;
	viewport.Y = viewportY;
	viewport.Width = viewportWidth;
	viewport.Height = viewportHeight;
	viewport.MinZ = viewportMinimumDepth;
	viewport.MaxZ = viewportMaximumDepth;

	fprintf(file, "{\n");
	fprintf(file, "  \"format\": 1,\n");
	fprintf(file, "  \"name\": \"%s\",\n",
		static_cast<const char*>(captureName));
	fprintf(file, "  \"zone\": %lu,\n", _pNetwork->MyCharacterInfo.zoneNo);
	fprintf(file, "  \"area\": %lu,\n", _pNetwork->MyCharacterInfo.areaNo);
	fprintf(file, "  \"layer\": %d,\n",
		static_cast<int>(_pNetwork->MyCharacterInfo.yLayer));
	fprintf(file, "  \"networkCameraAngle\": %.9g,\n",
		_pNetwork->MyCharacterInfo.camera_angle);
	fprintf(file, "  \"player\": ");
	WritePlacement(file, player->GetPlacement());
	fprintf(file, ",\n  \"viewpoint\": ");
	WritePlacement(file, player->en_plViewpoint);
	fprintf(file, ",\n  \"lastViewpoint\": ");
	WritePlacement(file, player->en_plLastViewpoint);
	fprintf(file, ",\n  \"viewMatrix\": ");
	if (viewAvailable)
		WriteMatrix(file, view);
	else
		fprintf(file, "null");
	fprintf(file, ",\n  \"projectionMatrix\": ");
	if (projectionAvailable)
		WriteMatrix(file, projection);
	else
		fprintf(file, "null");
	fprintf(file, ",\n  \"viewport\": ");
	if (viewportAvailable)
	{
		fprintf(
			file,
			"{\"x\": %lu, \"y\": %lu, \"width\": %lu, "
			"\"height\": %lu, \"minZ\": %.9g, \"maxZ\": %.9g}",
			viewport.X,
			viewport.Y,
			viewport.Width,
			viewport.Height,
			viewport.MinZ,
			viewport.MaxZ);
	}
	else
		fprintf(file, "null");
	fprintf(file, ",\n  \"vertexShaderConstants\": ");
	if (vertexShaderConstants != NULL && constantCount > 0)
	{
		fprintf(file, "[");
		for (UINT index = 0; index < constantCount * 4; ++index)
		{
			if (index > 0)
				fprintf(file, ", ");
			fprintf(file, "%.9g", vertexShaderConstants[index]);
		}
		fprintf(file, "]");
	}
	else
		fprintf(file, "null");
	fprintf(file, "\n}\n");
	fclose(file);
	return TRUE;
}

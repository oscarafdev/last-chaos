#include "StdH.h"

#include <Engine/Testing/ShaderValidationHooks.h>

#include <Engine/Graphics/Shader.h>

void PrepareNormalMapFogShaderValidation()
{
	if (shaGetRenFlags() & SHA_RMF_FOG)
		return;

	CAnyProjection3D* projection = shaGetProjection();
	Matrix12* objectToView = shaGetObjToViewMatrix();
	Matrix12* objectToAbsolute = shaGetObjToAbsMatrix();
	if (projection == NULL
		|| objectToView == NULL
		|| objectToAbsolute == NULL)
		return;

	const ULONG renderFlags = shaGetRenFlags() | SHA_RMF_FOG;
	shaSetRenFlags(renderFlags);
	shaInitSharedFogAndHazeParams(
		renderFlags,
		*projection,
		*objectToView,
		*objectToAbsolute);
}

void PrepareNormalMapRigidShaderValidation()
{
	// El fixture ModelHolder3 usa la ruta SKA aunque su malla sea rígida.
	// La prueba desactiva sus pesos para ejercitar el vertex shader rígido real.
	shaSetWeightsPerVertex(0);
}

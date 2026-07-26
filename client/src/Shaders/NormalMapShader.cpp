// Archivo agregado originalmente por Ahn Tae-hoon.
// Inicio de la modificación: normal mapping en espacio tangente (0.1).

#include "StdH.h"
#include <Shaders/Common.h>
#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Testing/ShaderValidationHooks.h>

#define NORMAL_USE_SPECULAR			(1UL<<(BASE_FLAG_OFFSET+0)) // Use Object Space Normal for Normal Map Shader
#define NORMAL_ALWAYS_OPAQUE		(1UL<<(BASE_FLAG_OFFSET+1)) // Always opaque mode
#define NORMAL_BILLBOARD			(1UL<<(BASE_FLAG_OFFSET+2))	// Use Billboard
#define NORMAL_CYLINDER_BILLBOARD	(1UL<<(BASE_FLAG_OFFSET+3))	// Use Cylinder Billboard
#define NORMAL_FLIP					(1UL<<(BASE_FLAG_OFFSET+4))	// Use Flip
//#define NORMAL_OBJECTSPACE			(1UL<<(BASE_FLAG_OFFSET+?)) // Use Object Space Normal for Normal Map Shader

static const INDEX ctTextures = 2;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 1;
static const INDEX ctFlags    = 7;
static const INDEX ctVertexPrograms = 2;
static const INDEX ctPixelPrograms = 2;

static const INDEX iBaseTexture			= 0;
static const INDEX iNormalMapTexture	= 1;
static const INDEX iBaseUVMap	= 0;
static const INDEX iDiffuseColor	= 0;
static const INDEX iSpecularPowerFloat	= 0;
static const INDEX iNonSpecularVP	= 0;
static const INDEX iUseSpecularVP	= 1;
static const INDEX iNonSpecularPP	= 0;
static const INDEX iUseSpecularPP	= 1;

static BOOL IsNormalMapFogValidationRequested()
{
	const char* value = getenv(
		"LASTCHAOS_DX12_3D_TEST_FORCE_NORMALMAP_FOG");
	return value != NULL && strcmp(value, "1") == 0;
}

static BOOL IsNormalMapRigidValidationRequested()
{
	const char* value = getenv(
		"LASTCHAOS_DX12_3D_TEST_FORCE_NORMALMAP_RIGID");
	return value != NULL && strcmp(value, "1") == 0;
}

static void PrepareNormalMapFogValidation()
{
	if (!IsNormalMapFogValidationRequested()
		|| (shaGetRenFlags() & SHA_RMF_FOG))
		return;

	PrepareNormalMapFogShaderValidation();
}

static void PrepareNormalMapRigidValidation()
{
	if (!IsNormalMapRigidValidationRequested())
		return;

	PrepareNormalMapRigidShaderValidation();
}

static void SetRenderingState(BOOL bOpaque)
{
	if(bOpaque)
	{
		// Set depth test
		shaEnableDepthTest();
		shaDepthFunc(GFX_LESS_EQUAL);
		// Set depth write
		shaEnableDepthWrite();
		// Set blend mode
		shaDisableBlend();
		// Set alpha test
		shaEnableAlphaTest();
	}
	else
	{
		// Set depth test
		shaEnableDepthTest();
		shaDepthFunc(GFX_LESS_EQUAL);
		// Set depth write
		shaDisableDepthWrite();
		// Set blend mode
		shaEnableBlend();
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		// Set alpha test
		shaDisableAlphaTest();
	}
}

static void VertexShaderProcess(BOOL bBillboard, BOOL bCylBillboard, BOOL bFullBright, COLOR colModelColor, BOOL bUseSpecular)
{
	//if use billboard, register c0~c3 <- billboard matrix.
	if(bBillboard)
	{
		shaPrepareForBillboard(bCylBillboard, !bFullBright);
	}
	shaSetVertexProgramConst_TransformMatrix();	//c0~c3
	if(bUseSpecular)
	{
		FLOAT3D vLightDir = shaGetLightDirection();
		FLOAT len = vLightDir.Length();
		if(!IsNormalFloat(len)) vLightDir = FLOAT3D(0,1,0);
		const ShaderRegister srLightDir(vLightDir(1), vLightDir(2), vLightDir(3), 765.01f);
		shaSetVertexProgramConst( 4, &srLightDir,     1);
	}
	else shaSetVertexProgramConst_LightDir();		//c4
	shaSetVertexProgramConst_Multiply();		//c7
	shaSetVertexProgramConst_BoneMatrix();		//c21~

	//c5
	GFXColor colLight   = shaGetLightColor();
	GFXColor colAmbient = shaGetAmbientColor();
	// if full bright
	if(bFullBright)
	{
		colLight.abgr = 0xFF000000;
		colAmbient.abgr = 0xFF7F7F7F;
	}
	if(bUseSpecular)
	{
		//c9 for specular
		INDEX r = colLight.r + colAmbient.r;
		INDEX g = colLight.g + colAmbient.g;
		INDEX b = colLight.b + colAmbient.b;
		#define MAX(a, b) ( ((a) > (b)) ? (a) : (b) )
		UBYTE temp = Clamp(MAX(r, MAX(g, b)), (INDEX)0x00, (INDEX)0x7F);
		#undef MAX
		const ShaderRegister srMaxSpeVal(0, 0, 0, NormByteToFloat(2*2*temp));
		shaSetVertexProgramConst(9, &srMaxSpeVal, 1);
	}

	//c5
	colLight.MultiplyRGBCopyA1( (GFXColor)colModelColor, colLight );
	const ShaderRegister srLightColor(colLight);
	shaSetVertexProgramConst(5, &srLightColor,   1);

	if(bUseSpecular)
	{
		//c8
		//viewer pos & specular power setting
		CAnyProjection3D &apr  = *shaGetProjection();
		FLOAT specularPower = shaGetFloat(iSpecularPowerFloat);
		FLOATmatrix3D &mViewer = apr->pr_ViewerRotationMatrix;
		FLOAT3D vViewerObj( mViewer(3,1), mViewer(3,2), mViewer(3,3) );
		FLOAT len = vViewerObj.Length();
		if(!IsNormalFloat(len)) vViewerObj = FLOAT3D(0,0,1);
		else
		{
			vViewerObj(1) /= len;
			vViewerObj(2) /= len;
			vViewerObj(3) /= len;
		}
		const ShaderRegister srSpecular(vViewerObj(1), vViewerObj(2), vViewerObj(3), specularPower);
		shaSetVertexProgramConst(8, &srSpecular,   1);
	}

	//set vertex shader
	if(bUseSpecular) shaSetVertexProgram(iUseSpecularVP, NMMT_TANSPACE);
	else shaSetVertexProgram(iNonSpecularVP, NMMT_TANSPACE);
}

static void PixelShaderProcess(BOOL bFullBright, COLOR colModelColor, BOOL bUseSpecular)
{
	// Set texture wrapping
	shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT, 0);
	shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT, 1);
	// Selecciona las texturas que se utilizarán.
	shaSetTexture(iNormalMapTexture, 0);
	shaSetTexture(iBaseTexture, 1);
	// Selecciona el conjunto de coordenadas UV.
	shaSetUVMap(iBaseUVMap);
	
	//Ambient color setting
	GFXColor colAmbient;
	if(bFullBright) colAmbient = shaGetAmbientColor() | 0xFF7F7F7F;
	else colAmbient = shaGetAmbientColor();
	colAmbient.MultiplyRGBCopyA1( (GFXColor)colModelColor, colAmbient );
	ShaderRegister srAmbientColor(colAmbient);
	// El pixel shader multiplica por dos, por eso aquí se divide por dos.
	//srAmbientColor.sr_fx *= 0.5f;
	//srAmbientColor.sr_fy *= 0.5f;
	//srAmbientColor.sr_fz *= 0.5f;
	shaSetPixelProgramConst( 0, &srAmbientColor, 1);

	//set pixel shader
	if(bUseSpecular) shaSetPixelProgram(iUseSpecularPP);
	else shaSetPixelProgram(iNonSpecularPP);
}

static void FogProcess(BOOL bOpaque)
{
// Inicio de la modificación de Kang Dong-min.
	// Cambio de orden.

	// Selecciona la etapa de textura utilizada cuando hay niebla.
	shaSetFogTextureUnit(2, bOpaque);
	// Set texture wrapping
	shaSetTextureWrapping(GFX_CLAMP, GFX_CLAMP, 2);
	// prepare fog and haze
	shaPrepareFogAndHaze(bOpaque);	
// Fin de la modificación de Kang Dong-min.
}

static void Render(BOOL bOpaque, BOOL bDoubleSided, BOOL bFlip)
{
	if (bOpaque)
	{
		// if double sided flag is on
		if (bDoubleSided) shaCullFace(GFX_NONE);	// no face culling
		else if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
		else shaCullFace(GFX_BACK);					// back face culling
		// Render surface
		shaRender();
	}
	else
	{
		// if double sided flag is on
		if (bDoubleSided)
		{
			// Render back side
			if(bFlip) shaCullFace(GFX_BACK);		// front face culling
			else shaCullFace(GFX_FRONT);			// back face culling
			shaRender();
		}
		// Render front side
		if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
		else shaCullFace(GFX_BACK);				// back face culling
		shaRender();
	}
}

SHADER_MAIN(NormalMap)
{
	const COLOR colModelColor = MulColors(shaGetModelColor(), shaGetColor(iDiffuseColor));
	// Es translúcido si no es totalmente opaco, salvo que se fuerce opacidad.
	const BOOL bOpaque = ((colModelColor&0xFF) == 0xFF)
						|| ((shaGetFlags() & NORMAL_ALWAYS_OPAQUE)==NORMAL_ALWAYS_OPAQUE);
	if((colModelColor&0xFF) == 0x00) return;	// Totalmente transparente.
	const BOOL bDoubleSided = shaGetFlags() & BASE_DOUBLE_SIDED;
	const BOOL bFullBright = shaGetFlags() & BASE_FULL_BRIGHT;
	const BOOL bUseSpecular = shaGetFlags() & NORMAL_USE_SPECULAR;
	const BOOL bBillboard = shaGetFlags() & NORMAL_BILLBOARD;
	const BOOL bCylBillboard = shaGetFlags() & NORMAL_CYLINDER_BILLBOARD;
	const BOOL bFlip = shaGetFlags() & NORMAL_FLIP;
//	const BOOL bObjectSpace = shaGetFlags() & NORMAL_OBJECTSPACE;
	const BOOL bHardwareShader = shaUseHWShaders();
	//const BOOL bHardwareShader = FALSE;

	if(bHardwareShader)
	{
		PrepareNormalMapFogValidation();
		PrepareNormalMapRigidValidation();
		SetRenderingState(bOpaque);
		FogProcess(bOpaque);
		VertexShaderProcess(bBillboard, bCylBillboard, bFullBright, colModelColor, bUseSpecular);
		PixelShaderProcess(bFullBright, colModelColor, bUseSpecular);
		Render(bOpaque, bDoubleSided, bFlip);
	}
	// El modo por software no procesa el normal map.
	else	//USE VS(SOFT or HARD), NON-PS
	{
		// Set texture wrapping
		shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
		// Set depth test
		shaEnableDepthTest();
		shaEnableDepthWrite();
		shaDepthFunc(GFX_LESS_EQUAL);
		
		shaSetTexture(iBaseTexture,0);
		shaEnableOverBrightning();
		if(bUseSpecular)
		{
			// El modo por software no procesa el componente especular.
			//shaSetTexture(iNormalMapTexture, 1);
		}
		else
		{
		}
		shaSetUVMap(iBaseUVMap);
		shaSetColor(iDiffuseColor);
		
		shaSetLightColor(shaGetAmbientColor(), shaGetLightColor());
		
		if(bOpaque) 
		{
			// Set blend mode
			shaDisableBlend();
			// Set alpha test
			shaEnableAlphaTest();

			// enable overbrightning
			shaEnableOverBrightning();
			// calculate shading
			shaCalculateLight();
			// prepare fog and haze
			shaPrepareFogAndHaze(bOpaque);

			// if double sided flag is on
			// no face culling
			if(bDoubleSided) shaCullFace(GFX_NONE);
			else if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
			else shaCullFace(GFX_BACK);					// back face culling
			// Render surface
			shaRender();
			
			shaDepthFunc(GFX_EQUAL);
			// render fog and haze pass for surface
			shaDoFogAndHazePass();
			shaDepthFunc(GFX_LESS_EQUAL);
			
			// disable overbrighning
			shaDisableOverBrightning();
		}
		else 
		{
			// Set blend mode
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			shaEnableBlend();
			shaDisableDepthWrite();
			// Set alpha test
			shaDisableAlphaTest();

			// enable overbrightning
			shaEnableOverBrightning();
			// calculate shading
			shaCalculateLight();
			// prepare fog and haze
			shaPrepareFogAndHaze(bOpaque);

			// adjust alpha for fog and haze
			shaAttenuateAlphaForFogAndHaze();
			
			// if double sided flag is on
			if(bDoubleSided) 
			{
				if(bFlip) shaCullFace(GFX_BACK);		// front face culling
				else shaCullFace(GFX_FRONT);			// back face culling
				shaRender();
			}
			if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
			else shaCullFace(GFX_BACK);				// back face culling
			shaRender();
			
			// disable overbrighning
			shaDisableOverBrightning();
		}
	}
}

// Inicio de la modificación de Ahn Tae-hoon: rendimiento (0.1).
SHADER_DESC(NormalMap, ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_astrTextureNames.New(ctTextures);
		shDescMe.sd_astrTexCoordNames.New(ctUVMaps);
		shDescMe.sd_astrColorNames.New(ctColors);
		shDescMe.sd_astrFloatNames.New(ctFloats);
		shDescMe.sd_astrFlagNames.New(ctFlags);

		shDescMe.sd_astrTextureNames[0] = "Diffuse map";
		shDescMe.sd_astrTextureNames[1] = "Normal map";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrFloatNames[0] = "Specular Power";
		shDescMe.sd_astrColorNames[0] = "Mat.Diffuse";
		shDescMe.sd_astrFlagNames[0] = "Double Sided";
		shDescMe.sd_astrFlagNames[1] = "Full Bright";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+0] = "Use Specular";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+1] = "Always Opaque";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+2] = "Billboard";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+3] = "Cyl.Billboard";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+4] = "Face Flip";
	//	shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+?] = "Object Space";
		shDescMe.sd_strShaderInfo = "NormalMap shader";

		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM|GFX_TANGENT_STREAM;
		shDescMe.sd_ulStreamFlags[1] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM|GFX_TANGENT_STREAM;
	}
	pshDesc = &shDescMe;
// Fin de la modificación de Ahn Tae-hoon: rendimiento (0.1).
}

SHADER_VCODE(NormalMap, CTString &strVPCode, INDEX iVertexProgram)
{
	// Se pueden registrar constantes entre c8 y c17.
	// El vertex blending puede utilizar hasta 66 slots de instrucciones.
	//--------------- Tagent Space Normal Map Vertex Shader --------------//
	//--------------- Input ---------------------//
	//--- v5     - tex coord                  ---//
	//--- r0     - vertex position            ---//
	//--- r1.xyz - light direction            ---//
	//--- r6.xyz - tangent                    ---//
	//--- r7.xyz - binormal                   ---//
	//--- r8.xyz - normal                     ---//
	//--- c0~c3  - mProjection*mView          ---//
	//--- c4.xyz - vLightDir                  ---//
	//--- c4.w   - 765.01f                    ---//
	//--- c5     - model*light rgb & model a  ---//
	//--- c7     - (0.0f, 1.0f, 2.0f, 0.5f)   ---//
	//--- c8     - viewer position & Spe.Pow. ---//
	//--- c9.w   - Max Specular value         ---//
	//-------------- Output ---------------------//
	//--- oPos   - Position Output            ---//
	//--- oT0    - Tex Coord Output           ---//
	//--- oT1    - Tex Coord Output           ---//
	//--- oD0    - Model*Light rgb & model a  ---//
	//--- oD1    - Light Dir. & Specular val. ---//

	// Usar la dirección en vez de la posición del visor ahorra al menos dos
	// slots de instrucciones, y quizá tres o cuatro en el cálculo de luz.
	// Reduce la calidad porque la vista pasa de vértice a objeto; se aprecia
	// especialmente en objetos grandes.
	if(iVertexProgram == iNonSpecularVP)
	{
		//--- Over Burning, None Specular ---//
		strVPCode = "m4x4 oPos,    r0,      c0                  \n"	// Envía las coordenadas de pantalla.
					"mov  oT0,     v5                           \n"	// Envía las coordenadas del normal map.
					"mov  oT1,     v5                           \n"	// Envía las coordenadas del normal map.
					"mov  oD0,     c5                           \n"	//diffuse color
					//"mul  oD0,     r9,      c7.wwwy             \n"	// Compensa el factor x2 del pixel shader.
					"mad  oD1.xyz, r1.xyz,  c7.www,  c7.www     \n"	// Envía la dirección de luz como color especular.
					;
	}
	else if(iVertexProgram == iUseSpecularVP)
	{
		//--- Over Burning, Use Specular ---//
		strVPCode = "m4x4 oPos,    r0,      c0                  \n"	// Envía las coordenadas de pantalla.
					"mov  oT0,     v5                           \n"	// Envía las coordenadas del normal map.
					"mov  oT1,     v5                           \n"	// Envía las coordenadas del diffuse map.
					"mov  oD0,     c5                           \n"	//diffuse color
//					"mul  oD0,     r9,      c7.wwwy             \n"	// Compensa el factor x2 del pixel shader.
					"mad  oD1.xyz, r1.xyz,  c7.www,  c7.www     \n"	// Envía la dirección de luz como color especular.

					// Inicio del cálculo especular.
					"sub  r2.xyz,  c8.xyz,  r0.xyz              \n"	//make view direction, No need Normalize
					"dp3  r2.w,    r2.xyz,  r2.xyz              \n"	//normalize view direction
					"rsq  r2.w,    r2.w                         \n"
//					"mul  r2.xyz,  r2.xyz,  r2.www              \n"	// Finaliza la normalización.
//					"add  r2.xyz,  r2.xyz,  c4.xyz              \n"	// Forma el vector medio; optimizado con mad.
					"mad  r2.xyz,  r2.xyz,  r2.www, c4.xyz      \n"	//make half vector
					"dp3  r2.w,    r2.xyz,  r2.xyz              \n"	//normalize half vector
					"rsq  r2.w,    r2.w                         \n"
					"mul  r2.xyz,  r2.xyz,  r2.www              \n"
					"dp3  r3.xyz,  r8.xyz,  r2.xyz              \n"	//Normal dot HalfVector
					"mov  r3.w,    c8.w                         \n"	//specular power for lit command
					"lit  r3.z,    r3                           \n"	// Calcula la iluminación.
					"mul  oD1.w,   r3.z,    c9.w                \n"	// Envía el valor especular en oD1.w.
					// Fin del cálculo especular.
					;
	}
	return;
}

SHADER_PCODE(NormalMap, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	// Solo se registra c0 para mantener compatibilidad con tarjetas sin PS.
	//--------------- Tagent Space Normal Map Pixel Shader --------------//
	//--------------- Input ---------------------//
	//--- t0     - Normal map & Specular map  ---//
	//--- t1     - Diffuse map                ---//
	//--- t2     - Fog map                    ---// Solo se usa con niebla.
	//--- v0     - diffuse color              ---//
	//--- v1.rgb - light direction            ---//
	//--- v1.a   - Specular Value             ---// Solo con bUseSpecular.
	//--- c0     - Ambient Color              ---//
	//--- c7     - Fog color                  ---// Solo se usa con niebla.
	//-------------- Output ---------------------//
	//--- r0     - Output Pixel COLOR & Alpha ---//

	if(eFogType == FT_NONE)
	{
		if(iPixelProgram == iNonSpecularPP)
		{
			//--- Over Burning, None Specular ---//
			// Usa dos texturas y hasta tres etapas de textura.
/*			strPPCode = "tex         t0                                   \n"	//normal map & specular map
						"tex         t1                                   \n"	//base(diffuse) map
						"dp3_sat     r1.rgb,  t0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"mad_sat     r1.rgb,  r1,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  t1,      r1                 \n"	//prev result * diffuse map
						"+mul         r0.a,    t1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						;*/
			strPPCode = "texld  r0,		t0								\n"	//normal map & specular map
						"texld	r1,		t1								\n"	//base(diffuse) map
						"dp3_sat     r2.rgb,  r0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"mad_sat     r2.rgb,  r2,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  r1,      r2                 \n"	//prev result * diffuse map
						"+mul         r0.a,    r1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						;
		}
		else if(iPixelProgram == iUseSpecularPP)
		{
			//--- Over Burning, Use Specular ---//
			// Usa dos texturas y hasta cuatro etapas de textura.
/*			strPPCode = "tex         t0                                   \n"	//normal map & specular map
						"tex         t1                                   \n"	//base(diffuse) map
						"dp3_sat     r1.rgb,  t0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"+mul        r1.a,    t0.a,    v1.a               \n"	//r1.a   - specular pixel value
						"mad_sat     r1.rgb,  r1,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  t1,      r1                 \n"	//prev result * diffuse map
						"+mul         r0.a,    t1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"add_sat     r0.rgb,  r0,      r1.a               \n"	//Output RGB = prev result + specular value
						;*/
			strPPCode = "texld	r0,		t0								\n"	//normal map & specular map
						"texld	r1,		t1								\n"	//base(diffuse) map
						"dp3_sat     r2.rgb,  r0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"+mul        r2.a,    r0.a,    v1.a               \n"	//r1.a   - specular pixel value
						"mad_sat     r2.rgb,  r2,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  r1,      r2                 \n"	//prev result * diffuse map
						"+mul         r0.a,    r1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"add_sat     r0.rgb,  r0,      r2.a               \n"	//Output RGB = prev result + specular value
						;
		}
	}
	else if(eFogType == FT_OPAQUE)
	{
		if(iPixelProgram == iNonSpecularPP)
		{
/*			strPPCode = "tex         t0                                   \n"	//normal map & specular map
						"tex         t1                                   \n"	//base(diffuse) map
						"tex         t2                                   \n"	//fog map
						"dp3_sat     r1.rgb,  t0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"mad_sat     r1.rgb,  r1,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  t1,      r1                 \n"	//prev result * diffuse map
						"+mul         r0.a,    t1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"mul         t2,      t2,      c7                 \n"	// mul fog texture with fog color
						"lrp         r0.rgb,  t2.a,    t2,      r0        \n"	// Add fog
						;*/
			strPPCode = "texld	r0,		t0								\n"	//normal map & specular map
						"texld	r1,		t1								\n"	//base(diffuse) map
						"texld	r2,		t2								\n"	//fog map
						"dp3_sat     r3.rgb,  r0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"mad_sat     r3.rgb,  r3,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  r1,      r3                 \n"	//prev result * diffuse map
						"+mul         r0.a,    r1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"mul         r2,      r2,      c7                 \n"	// mul fog texture with fog color
						"lrp         r0.rgb,  r2.a,    r2,      r0        \n"	// Add fog
						;
		}
		else if(iPixelProgram == iUseSpecularPP)
		{
/*			strPPCode = "tex         t0                                   \n"	//normal map & specular map
						"tex         t1                                   \n"	//base(diffuse) map
						"tex         t2                                   \n"	//fog map
						"dp3_sat     r1.rgb,  t0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"+mul        r1.a,    t0.a,    v1.a               \n"	//r1.a   - specular pixel value
						"mad_sat     r1.rgb,  r1,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  t1,      r1                 \n"	//prev result * diffuse map
						"+mul         r0.a,    t1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"add_sat     r0.rgb,  r0,      r1.a               \n"	//Output RGB = prev result + specular value
						"mul         t2,      t2,      c7                 \n"	// mul fog texture with fog color
						"lrp         r0.rgb,  t2.a,    t2,      r0        \n"	// Add fog
						;*/
			strPPCode = "texld  r0,			t0							\n"	//normal map & specular map
						"texld	r1,			t1							\n"	//base(diffuse) map
						"texld  r2,			t2							\n"	//fog map
						"dp3_sat     r3.rgb,  r0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"+mul        r3.a,    r0.a,    v1.a               \n"	//r1.a   - specular pixel value
						"mad_sat     r3.rgb,  r3,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  r1,      r3                 \n"	//prev result * diffuse map
						"+mul         r0.a,    r1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"add_sat     r0.rgb,  r0,      r3.a               \n"	//Output RGB = prev result + specular value
						"mul         r2,      r2,      c7                 \n"	// mul fog texture with fog color
						"lrp         r0.rgb,  r2.a,    r2,      r0        \n"	// Add fog
						;
		}
	}
	else if(eFogType == FT_NON_OPAQUE)
	{
		if(iPixelProgram == iNonSpecularPP)
		{
/*			strPPCode = "tex         t0                                   \n"	//normal map & specular map
						"tex         t1                                   \n"	//base(diffuse) map
						"tex         t2                                   \n"	//fog map
						"mul         t2,      t2,      c7                 \n"	// mul fog texture with fog color
						"mul         t0.a,    t0,      1-t2.a             \n"	// attenuate base tex with fog alpha
						"dp3_sat     r1.rgb,  t0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"mad_sat     r1.rgb,  r1,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  t1,      r1                 \n"	//prev result * diffuse map
						"+mul         r0.a,    t1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						;*/
			strPPCode = "texld		r0,       t0						\n"	//normal map & specular map
						"texld		r1,       t1						\n"	//base(diffuse) map
						"texld		r2,       t2						\n"	//fog map
						"mul         r2,      r2,      c7                 \n"	// mul fog texture with fog color
						"mul         r0.a,    r0,      1-r2.a             \n"	// attenuate base tex with fog alpha
						"dp3_sat     r3.rgb,  r0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"mad_sat     r3.rgb,  r3,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  r1,      r3                 \n"	//prev result * diffuse map
						"+mul         r0.a,    r1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						;
		}
		else if(iPixelProgram == iUseSpecularPP)
		{
/*			strPPCode = "tex         t0                                   \n"	//normal map & specular map
						"tex         t1                                   \n"	//base(diffuse) map
						"tex         t2                                   \n"	//fog map
						"mul         t2,      t2,      c7                 \n"	// mul fog texture with fog color
						"mul         t0.a,    t0,      1-t2.a             \n"	// attenuate base tex with fog alpha
						"dp3_sat     r1.rgb,  t0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"+mul        r1.a,    t0.a,    v1.a               \n"	//r1.a   - specular pixel value
						"mad_sat     r1.rgb,  r1,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  t1,      r1                 \n"	//prev result * diffuse map
						"+mul         r0.a,    t1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"add_sat     r0.rgb,  r0,      r1.a               \n"	//Output RGB = prev result + specular value
						;*/
			strPPCode = "texld		r0,         t0						\n"	//normal map & specular map
						"texld		r1,         t1						\n"	//base(diffuse) map
						"texld		r2,         t2						\n"	//fog map
						"mul         r2,      r2,      c7                 \n"	// mul fog texture with fog color
						"mul         r0.a,    r0,      1-r2.a             \n"	// attenuate base tex with fog alpha
						"dp3_sat     r3.rgb,  r0_bx2,  v1_bx2             \n"	//r1.rgb - diffuse shade
						"+mul        r3.a,    r0.a,    v1.a               \n"	//r1.a   - specular pixel value
						"mad_sat     r3.rgb,  r3,      v0,      c0        \n"	//diffuse shade * diffuse RGB + ambient RGB
						"mul_x2_sat  r0.rgb,  r1,      r3                 \n"	//prev result * diffuse map
						"+mul         r0.a,    r1.a,    v0.a              \n"	//Output A = diffuse map A * diffuse color A
						"add_sat     r0.rgb,  r0,      r1.a               \n"	//Output RGB = prev result + specular value
						;
		}
	}
	return;
}
// Fin de la modificación de Ahn Tae-hoon: normal mapping tangente (0.1).

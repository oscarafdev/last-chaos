
// DIRECT3D "LOW-LEVEL" INTERFACE (COMMON)

#include "stdh.h"

#include <Engine/Base/Translation.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Math/Float.h>

#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ViewPort.h>

#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CTextureData.h>

//#ifdef SE1_D3D
#include <Engine/Graphics/Gfx_Direct3D.h>
#include <Engine/Graphics/Gfx_Direct3D_Functions.h>
#include <Engine/Graphics/DirectX12Backend.h>
#include <Engine/Graphics/DirectX12Buffer.h>
#include <vector>

// Inicio de modificacion de Ahn Tae-hoon: agregar y modificar el efecto SSSE (0.1).
#include <Engine/Effect/EffectCommon.h>
// Fin de modificacion de Ahn Tae-hoon: agregar y modificar el efecto SSSE (0.1).

#undef new
#pragma comment(lib, "d3d9.lib")

// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

//#define ASMOPT 1


extern ULONG64 _fog_ulTexture;
extern ULONG64 _haze_ulTexture;

// occlusion queries
extern UINT *_puiOcclusionQueryIDs;

// ### Vertex Shader Program static stack array
extern CStaticStackArray<VertexShaderProgram>	_avsVertexShaderProgram;

// state variables
extern BOOL GFX_bDepthTest;
extern BOOL GFX_bDepthWrite;
extern BOOL GFX_bAlphaTest;
extern BOOL GFX_bBlending;
extern BOOL GFX_bDithering;
extern BOOL GFX_bLighting;
extern BOOL GFX_bClipping;
extern BOOL GFX_bClipPlane;
extern BOOL GFX_bColorArray;
extern BOOL GFX_bFrontFace;
extern BOOL GFX_bTruform;
extern INDEX GFX_iActiveTexUnit;
extern FLOAT GFX_fMinDepthRange;
extern FLOAT GFX_fMaxDepthRange;
extern GfxBlend GFX_eBlendSrc;
extern GfxBlend GFX_eBlendDst;
extern GfxComp  GFX_eDepthFunc;
extern GfxFace  GFX_eCullFace;
extern INDEX GFX_iTexModulation[GFX_MAXTEXUNITS];
extern BOOL  GFX_abTexture[GFX_MAXTEXUNITS];
extern BOOL  GFX_abLights[GFX_MAXLIGHTS];

extern INDEX GFX_ctVertices;
extern ULONG GFX_ulCurrentColorMask;
extern BOOL  D3D_bUseColorArray = FALSE;
extern BOOL _bUsingDynamicBuffer;

extern BOOL  GFX_bUseVertexProgram;
extern BOOL  GFX_bUsePixelProgram;

// sehan
extern INDEX d3d_bDeviceChanged;
// sehan end

// internal vars
static INDEX _iVtxOffset = 0;
static INDEX _iVtxPos = 0;
static INDEX _iTexPass = 0;
static INDEX _iColPass = 0;
extern DWORD _dwCurrentVS;
extern LPDIRECT3DPIXELSHADER9 _dwCurrentPS = NONE;
extern DWORD _dwLastVertexProgram = NONE;
extern ULONG _ulStreamsMask = NONE;
static ULONG _ulLastStreamsMask = NONE;
static BOOL  _bProjectiveMapping = FALSE;
static BOOL  _bLastProjectiveMapping = FALSE;
extern BOOL  _bGenerateTexCoord = FALSE;
static BOOL  _bLastGenerateTexCoord = FALSE;

static std::vector<UBYTE> _dx12DynamicPositions;
static std::vector<UBYTE> _dx12DynamicNormals;
static std::vector<UBYTE> _dx12DynamicTangents;
static std::vector<UBYTE> _dx12DynamicWeights;
static std::vector<UBYTE> _dx12DynamicColors[GFX_MAXLAYERS];
static std::vector<UBYTE> _dx12DynamicTexCoords[GFX_MAXLAYERS];
static std::vector<UWORD> _dx12DynamicIndices;
static CDirectX12Buffer* _dx12PositionBuffer = NULL;
static CDirectX12Buffer* _dx12NormalBuffer = NULL;
static CDirectX12Buffer* _dx12TangentBuffer = NULL;
static CDirectX12Buffer* _dx12WeightBuffer = NULL;
static CDirectX12Buffer* _dx12ColorBuffers[GFX_MAXLAYERS] = { NULL };
static CDirectX12Buffer* _dx12TexCoordBuffers[GFX_MAXLAYERS] = { NULL };
static CDirectX12Buffer* _dx12IndexBuffer = NULL;

static CDirectX12Buffer* CreateNativeVertexBuffer(
	UINT size,
	UINT stride)
{
	CDirectX12Buffer* pBuffer = new CDirectX12Buffer;
	if (pBuffer == NULL || !pBuffer->CreateVertexBuffer(
		GetDirectX12Backend().GetDevice(),
		size,
		stride))
	{
		delete pBuffer;
		return NULL;
	}
	return pBuffer;
}

// swap intervals tables
static UINT _auiSwapIntervals[] = {
	D3DPRESENT_INTERVAL_IMMEDIATE,
	D3DPRESENT_INTERVAL_ONE,
	D3DPRESENT_INTERVAL_TWO,
	D3DPRESENT_INTERVAL_THREE };

// shaders created so far
struct VertexShader {
	DWORD vs_dwHandle;
	ULONG vs_ulStreamMask;
	IDirect3DVertexShader9 *Shader;
	IDirect3DVertexDeclaration9 *Declaration;
};

static CStaticStackArray<VertexShader> _avsFixedShaders;

// size of one vertex
#define VTXSIZE (GFX_POSSIZE + GFX_NORSIZE + GFX_WGHSIZE + GFX_TX4SIZE \
							 + GFX_COLSIZE *  _pGfx->gl_ctColBuffers \
							 + GFX_TEXSIZE * (_pGfx->gl_ctTexBuffers-1))
// SHADER SETUP PARAMS

#define DECLTEXOFS (2*GFX_TEXIDX)

// NOTE: fixed function vs and programmable vs templates must have same streams and same register count

// template shader for fixed function vertex shader
DWORD _adwDeclTemplateFF[] = {
	D3DVSD_STREAM(0),
	D3DVSD_REG( D3DVSDE_POSITION,  D3DVSDT_FLOAT3),
	D3DVSD_STREAM(1),
	D3DVSD_REG( D3DVSDE_DIFFUSE,   D3DVSDT_D3DCOLOR),
	D3DVSD_STREAM(2),
	D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(3),
	D3DVSD_REG( D3DVSDE_TEXCOORD1, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(4),
	D3DVSD_REG( D3DVSDE_TEXCOORD2, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(5),
	D3DVSD_REG( D3DVSDE_TEXCOORD3, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(6),
	D3DVSD_REG( D3DVSDE_NORMAL,    D3DVSDT_FLOAT3),
	D3DVSD_STREAM(7), // not used in fixed-function vs but must be declared for stream reset
	D3DVSD_REG( NONE, NONE),
	D3DVSD_REG( NONE, NONE),
	D3DVSD_END()
};

// template shader for programmable vertex shader
DWORD _adwDeclTemplateVP[] = {
	D3DVSD_STREAM(0),
	D3DVSD_REG( 0,  D3DVSDT_FLOAT3),    // position
	D3DVSD_STREAM(1),
	D3DVSD_REG( 4,  D3DVSDT_D3DCOLOR),  // diffuse
	D3DVSD_STREAM(2),
	D3DVSD_REG( 5,  D3DVSDT_FLOAT2),    // texcoord0
	D3DVSD_STREAM(3),
	D3DVSD_REG( 6,  D3DVSDT_FLOAT2),    // texcoord1
	D3DVSD_STREAM(4),
	D3DVSD_REG( 7,  D3DVSDT_FLOAT2),    // texcoord2
	D3DVSD_STREAM(5),
	D3DVSD_REG( 8, D3DVSDT_FLOAT2),     // texcoord3
	D3DVSD_STREAM(6),
	D3DVSD_REG( 1,  D3DVSDT_FLOAT3),    // normal
	D3DVSD_STREAM(7),
	D3DVSD_REG( 3,  D3DVSDT_D3DCOLOR),  // blend indices
	D3DVSD_REG( 2,  D3DVSDT_D3DCOLOR),  // blend weights
	D3DVSD_END()
};

INDEX _aiStreamRegs[] = {
	1,// D3DVSDE_POSITION
	1,// D3DVSDE_DIFFUSE
	1,// D3DVSDE_TEXCOORD0
	1,// D3DVSDE_TEXCOORD1
	1,// D3DVSDE_TEXCOORD2
	1,// D3DVSDE_TEXCOORD3
	1,// D3DVSDE_NORMAL
	2,// D3DVSDE_BLENDINDICES & D3DVSDE_BLENDWEIGHT
};

// current shader
DWORD _adwCurrentDecl[MAX_SHADER_DECL_SIZE];


// check whether texture format is supported in D3D
static BOOL HasTextureFormat_D3D( D3DFORMAT d3dTextureFormat)
{
	// quickie?
	const D3DFORMAT d3dScreenFormat = _pGfx->gl_d3dColorFormat;
	if( d3dTextureFormat==D3DFMT_UNKNOWN || d3dScreenFormat==NONE) return TRUE;
	// checkie! :)
	HRESULT hr = _pGfx->gl_pD3D9->CheckDeviceFormat( _pGfx->gl_iCurrentAdapter, d3dDevType, d3dScreenFormat,
																									0, D3DRTYPE_TEXTURE, d3dTextureFormat);
	return( hr==D3D_OK);
}

// returns number of vertices based on vertex size and required size in memory (KB)
extern INDEX VerticesFromSize_D3D( SLONG slSizeKB)
{
	const INDEX ctVertices = slSizeKB*1024 / VTXSIZE;
	ASSERT( ctVertices>0 && ctVertices<65536);
	return( ctVertices);
}

// returns size in memory based on number of vertices
extern SLONG SizeFromVertices_D3D( INDEX ctVertices)
{
	ASSERT( ctVertices>0 && ctVertices<65536);
	return( ctVertices * VTXSIZE);
}

// get shader declaration
extern void GetShaderDeclaration_D3D9(D3DVERTEXELEMENT9* ulRetDecl, ULONG ulStreamFlags)
{
	// if using position stream
	if (ulStreamFlags & GFX_POSITION_STREAM) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_POSIDX;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_FLOAT3;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_POSITION;
		e.UsageIndex = 0;

		*(ulRetDecl) = e;
		ulStreamFlags &= ~GFX_POSITION_STREAM;
	}

	if (ulStreamFlags & GFX_COLOR_STREAM) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_COLIDX;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_D3DCOLOR;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_COLOR;
		e.UsageIndex = 0;

		*(++ulRetDecl) = e;
		ulStreamFlags &= ~GFX_COLOR_STREAM;
	}

	if (ulStreamFlags & GFX_TEXCOORD0) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TEXIDX;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_FLOAT2;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_TEXCOORD;
		e.UsageIndex = 0;

		*(++ulRetDecl) = e;
		ulStreamFlags &= ~GFX_TEXCOORD0;
	}

	if (ulStreamFlags & GFX_TEXCOORD1) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TEXIDX + 1;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_FLOAT2;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_TEXCOORD;
		e.UsageIndex = 1;
		*(++ulRetDecl) = e;
		ulStreamFlags &= ~GFX_TEXCOORD1;
	}

	if (ulStreamFlags & GFX_TEXCOORD2) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TEXIDX + 2;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_FLOAT2;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_TEXCOORD;
		e.UsageIndex = 2;
		*(++ulRetDecl) = e;
		ulStreamFlags &= ~GFX_TEXCOORD2;
	}

	if (ulStreamFlags & GFX_TEXCOORD3) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TEXIDX + 3;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_FLOAT2;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_TEXCOORD;
		e.UsageIndex = 3;
		*(++ulRetDecl) = e;
		ulStreamFlags &= ~GFX_TEXCOORD3;
	}

	if (ulStreamFlags & GFX_NORMAL_STREAM) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_NORIDX;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_FLOAT3;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_NORMAL;
		e.UsageIndex = 0;

		*(++ulRetDecl) = e;
		ulStreamFlags &= ~GFX_NORMAL_STREAM;
	}

	if (ulStreamFlags & GFX_WEIGHT_STREAM) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_WGHIDX;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_D3DCOLOR;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_BLENDINDICES;
		e.UsageIndex = 0;

		D3DVERTEXELEMENT9 e2;
		e2.Stream = GFX_WGHIDX;
		e2.Offset = 4;
		e2.Type = D3DDECLTYPE_D3DCOLOR;
		e2.Method = D3DDECLMETHOD_DEFAULT;
		e2.Usage = D3DDECLUSAGE_BLENDWEIGHT;
		e2.UsageIndex = 0;

		*(++ulRetDecl) = e;
		*(++ulRetDecl) = e2;
		ulStreamFlags &= ~GFX_WEIGHT_STREAM;
	}

	if (ulStreamFlags & GFX_TANGENT_STREAM) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TANIDX;
		e.Offset = 0;
		e.Type = D3DDECLTYPE_FLOAT4;
		e.Method = D3DDECLMETHOD_DEFAULT;
		e.Usage = D3DDECLUSAGE_TANGENT;
		e.UsageIndex = 0;

		//*(++ulRetDecl) = D3DVSD_STREAM(8);	// Version original.
		*(++ulRetDecl) = e;
		ulStreamFlags &= ~GFX_TANGENT_STREAM;
	}

	ASSERT(ulStreamFlags == 0); // make sure stream flags were valid

	D3DVERTEXELEMENT9 eend;
	eend.Stream = 0xFF;
	eend.Offset = 0;
	eend.Type = D3DDECLTYPE_UNUSED;
	eend.Method = 0;
	eend.Usage = 0;
	eend.UsageIndex = 0;
	*(++ulRetDecl) = eend;
}



// construct vertex shader out of streams' bit-mask
extern DWORD SetupShader_D3D( ULONG ulStreamsMask)
{
	HRESULT hr;
	const INDEX ctFixedShaders = _avsFixedShaders.Count();

	INDEX iVS;

	// delete all shaders?
	if( ulStreamsMask==NONE) {
		// first set default vertex shader
		hr = _pGfx->gl_pd3d9Device->SetVertexShader(nullptr);
		hr = _pGfx->gl_pd3d9Device->SetFVF(D3DFVF_CTVERTEX);
		GetDirectX12Backend().TrackLegacy3DVertexShader(NULL, NULL);
		D3D_CHECKERROR(hr);
		gfxSetPixelProgram(NONE);

		for( iVS=0; iVS<ctFixedShaders; iVS++) {
			dxDeleteVertexShader(_avsFixedShaders[iVS].vs_dwHandle, 0);
			D3D_CHECKERROR(hr);
		}

		// Delete all programable vertex and pixel programs
		extern void ClearVertexAndPixelPrograms(void);
		ClearVertexAndPixelPrograms();

		// free array
		_avsFixedShaders.PopAll();
		// ###
		_avsVertexShaderProgram.PopAll();
		// ###
		_dwCurrentVS = NONE;
		_dwCurrentPS = NONE;
		_dwLastVertexProgram = NONE;
		_currentVS_Declaration = nullptr;
		_currentVS_Shader = nullptr;
		return NONE;
	}

	// see if required fixed function shader has already been created
	for( iVS=0; iVS<ctFixedShaders; iVS++) {
		if( _avsFixedShaders[iVS].vs_ulStreamMask==ulStreamsMask) {
			_currentVS_Declaration = _avsFixedShaders[iVS].Declaration;
			_currentVS_Shader = _avsFixedShaders[iVS].Shader;
			return _avsFixedShaders[iVS].vs_dwHandle;
		}
	}

	// darn, need to create shader :(
	// pre-adjustment for eventual projective mapping
	_adwDeclTemplateFF[DECLTEXOFS+1] = D3DVSD_REG( D3DVSDE_TEXCOORD0, (ulStreamsMask&0x1000) ? D3DVSDT_FLOAT4 : D3DVSDT_FLOAT2);
	ulStreamsMask &= ~0x1000;
	ULONG ulMask = ulStreamsMask;

	// process mask, bit by bit
	INDEX iSrcDecl=0, iDstDecl=0;
	INDEX iStream=0;
	while(_adwDeclTemplateFF[iSrcDecl]!=D3DVSD_END())
	{ // add declarator if used
		INDEX ctRegs = _aiStreamRegs[iStream] + 1; // n*D3DVSD_REG() + D3DVSD_STREAM()
		if( ulMask&1) {
			for(INDEX ireg=0;ireg<ctRegs;ireg++) {
				_adwCurrentDecl[iDstDecl+ireg] = _adwDeclTemplateFF[iSrcDecl+ireg];
			}
			iDstDecl+=ctRegs;
		}
		iSrcDecl+=ctRegs;
		ulMask >>= 1;
		iStream++;
	}
	// mark end
	_adwCurrentDecl[iDstDecl] = D3DVSD_END();
	// ASSERT( iDstDecl < MAXSTREAMS);
	#pragma message(">> Fix ASSERT( iDstDecl < MAXSTREAMS)")
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);

	// create new vertex shader
	const DWORD dwFlags = (_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL) ? NONE : D3DUSAGE_SOFTWAREPROCESSING;

	VertexShader &vs = _avsFixedShaders.Push();
	vs.vs_ulStreamMask = ulStreamsMask;

	// pre-adjustment for eventual projective mapping
	const BOOL bPosStream = (ulStreamsMask >> GFX_POSIDX & 1);
	const BOOL bColStream = (ulStreamsMask >> GFX_COLIDX & 1);
	const BOOL bTexStream1 = (ulStreamsMask >> (GFX_TEXIDX + 0) & 1);
	const BOOL bTexStream2 = (ulStreamsMask >> (GFX_TEXIDX + 1) & 1);
	const BOOL bTexStream3 = (ulStreamsMask >> (GFX_TEXIDX + 2) & 1);
	const BOOL bTexStream4 = (ulStreamsMask >> (GFX_TEXIDX + 3) & 1);
	const BOOL bNorStream = (ulStreamsMask >> GFX_NORIDX & 1);

	std::vector<D3DVERTEXELEMENT9> vecDec;

	if (bPosStream == TRUE) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_POSIDX; e.Offset = 0; e.Type = D3DDECLTYPE_FLOAT3; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_POSITION; e.UsageIndex = 0;
		vecDec.push_back(e);
	}

	if (bColStream == TRUE) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_COLIDX; e.Offset = 0; e.Type = D3DDECLTYPE_D3DCOLOR; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_COLOR; e.UsageIndex = 0;
		vecDec.push_back(e);
	}

	if (bTexStream1 == TRUE) {
		D3DVERTEXELEMENT9 e;
		if (ulStreamsMask & 0x1000) {
			e.Stream = GFX_TEXIDX + 0; e.Offset = 0; e.Type = D3DDECLTYPE_FLOAT2; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_TEXCOORD; e.UsageIndex = 0;
		} else {
			e.Stream = GFX_TEXIDX + 0; e.Offset = 0; e.Type = D3DDECLTYPE_FLOAT2; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_TEXCOORD; e.UsageIndex = 0;
		}
		vecDec.push_back(e);
	}

	if (bTexStream2 == TRUE) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TEXIDX + 1; e.Offset = 0; e.Type = D3DDECLTYPE_FLOAT2; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_TEXCOORD; e.UsageIndex = 1;
		vecDec.push_back(e);
	}

	if (bTexStream3 == TRUE) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TEXIDX + 2; e.Offset = 0; e.Type = D3DDECLTYPE_FLOAT2; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_TEXCOORD; e.UsageIndex = 2;
		vecDec.push_back(e);
	}

	if (bTexStream4 == TRUE) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_TEXIDX + 3; e.Offset = 0; e.Type = D3DDECLTYPE_FLOAT2; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_TEXCOORD; e.UsageIndex = 3;
		vecDec.push_back(e);
	}

	if (bNorStream == TRUE) {
		D3DVERTEXELEMENT9 e;
		e.Stream = GFX_NORIDX; e.Offset = 0; e.Type = D3DDECLTYPE_FLOAT3; e.Method = D3DDECLMETHOD_DEFAULT; e.Usage = D3DDECLUSAGE_NORMAL; e.UsageIndex = 0;
		vecDec.push_back(e);
	}

	D3DVERTEXELEMENT9 e;
	e.Stream = 0xFF; e.Offset = 0; e.Type = D3DDECLTYPE_UNUSED; e.Method = 0; e.Usage = 0; e.UsageIndex = 0;
	vecDec.push_back(e);

	hr = _pGfx->gl_pd3d9Device->CreateVertexDeclaration(&vecDec[0], &vs.Declaration);
	if (FAILED(hr)) {
		CPrintF("Create Vertex Declaration Error!\n"); //return NULL;
	}
	dxCreateFixedVertexShader(&vs.vs_dwHandle, &vs.Shader, vs.Declaration);

	// reset current shader
	_pGfx->gl_dwVertexShader = NONE;
	//_currentVSDecl = vs.Declaration;
	_currentVS_Declaration = vs.Declaration;
	_currentVS_Shader = vs.Shader;
	return vs.vs_dwHandle;
}

// DIRECT3D "LOW-LEVEL" INTERFACE FOR PC
extern INDEX gap_iTruformLevel;

static INDEX _iIdxOffset = 0;
static DWORD _dwVtxLockFlag;                  // for vertex and normal
static DWORD _dwColLockFlags[GFX_MAXLAYERS];  // for colors
static DWORD _dwTexLockFlags[GFX_MAXLAYERS];  // for texture coords
static LPDIRECT3DVERTEXBUFFER9 _pd3dLockedVtx = NULL;
static LPDIRECT3DVERTEXBUFFER9 _pd3dLockedNor = NULL;
// Inicio de modificacion de Ahn Tae-hoon: mapa normal en espacio tangente (0.1).
static LPDIRECT3DVERTEXBUFFER9 _pd3dLockedTan = NULL;
// Fin de modificacion de Ahn Tae-hoon: mapa normal en espacio tangente (0.1).
static LPDIRECT3DVERTEXBUFFER9 _pd3dLockedWgh = NULL;
static LPDIRECT3DVERTEXBUFFER9 _pd3dLockedCol = NULL;
static LPDIRECT3DVERTEXBUFFER9 _pd3dLockedTex = NULL;

// system gamma table
extern UWORD _auwSystemGammaTable[256*3];
static D3DGAMMARAMP *pgrtSystemGamma = (D3DGAMMARAMP*)&_auwSystemGammaTable[0];



// prepare vertex arrays for drawing
extern void SetupVertexArrays_D3D( INDEX ctVertices)
{
	INDEX i;
	ASSERT( ctVertices>=0);

	// do nothing if buffer is sufficient
	ctVertices = ClampUp( ctVertices, 65535L); // need to clamp max vertices first
	if( ctVertices!=0 && ctVertices<=_pGfx->gl_ctVertices) return;
	delete _dx12PositionBuffer;
	delete _dx12NormalBuffer;
	delete _dx12TangentBuffer;
	delete _dx12WeightBuffer;
	_dx12PositionBuffer = NULL;
	_dx12NormalBuffer = NULL;
	_dx12TangentBuffer = NULL;
	_dx12WeightBuffer = NULL;
	for (i = 0; i < GFX_MAXLAYERS; ++i)
	{
		delete _dx12ColorBuffers[i];
		delete _dx12TexCoordBuffers[i];
		_dx12ColorBuffers[i] = NULL;
		_dx12TexCoordBuffers[i] = NULL;
		_dx12DynamicColors[i].clear();
		_dx12DynamicTexCoords[i].clear();
	}
	_dx12DynamicPositions.clear();
	_dx12DynamicNormals.clear();
	_dx12DynamicTangents.clear();
	_dx12DynamicWeights.clear();
	_pGfx->gl_pd3dVtx = NULL;
	_pGfx->gl_pd3dNor = NULL;
	_pGfx->gl_pd3dTan = NULL;
	_pGfx->gl_pd3dWgh = NULL;

	// allocate if needed
	if( ctVertices>0)
	{
		// update max vertex count
		if( _pGfx->gl_ctVertices < ctVertices) _pGfx->gl_ctVertices = ctVertices;
		else ctVertices = _pGfx->gl_ctVertices;

		_dx12DynamicPositions.resize(ctVertices * GFX_POSSIZE);
		_dx12DynamicNormals.resize(ctVertices * GFX_NORSIZE);
		_dx12DynamicTangents.resize(ctVertices * GFX_TANSIZE);
		_dx12DynamicWeights.resize(ctVertices * GFX_WGHSIZE);
		_dx12PositionBuffer = CreateNativeVertexBuffer(
			ctVertices * GFX_POSSIZE, GFX_POSSIZE);
		_dx12NormalBuffer = CreateNativeVertexBuffer(
			ctVertices * GFX_NORSIZE, GFX_NORSIZE);
		_dx12TangentBuffer = CreateNativeVertexBuffer(
			ctVertices * GFX_TANSIZE, GFX_TANSIZE);
		_dx12WeightBuffer = CreateNativeVertexBuffer(
			ctVertices * GFX_WGHSIZE, GFX_WGHSIZE);

		for( i=0; i<_pGfx->gl_ctColBuffers; i++) {
			_dx12DynamicColors[i].resize(ctVertices * GFX_COLSIZE);
			_dx12ColorBuffers[i] = CreateNativeVertexBuffer(
				ctVertices * GFX_COLSIZE, GFX_COLSIZE);
			_pGfx->gl_pd3dCol[i] = NULL;
		}
		for( i=0; i<_pGfx->gl_ctTexBuffers; i++) {
			const SLONG slSize = ctVertices * (i==0 ? GFX_TX4SIZE : GFX_TEXSIZE); // 1st texture buffer might have projective mapping
			const UINT stride = i == 0 ? GFX_TX4SIZE : GFX_TEXSIZE;
			_dx12DynamicTexCoords[i].resize(slSize);
			_dx12TexCoordBuffers[i] = CreateNativeVertexBuffer(
				slSize, stride);
			_pGfx->gl_pd3dTex[i] = NULL;
		}
		ASSERT(_dx12PositionBuffer != NULL
			&& _dx12NormalBuffer != NULL
			&& _dx12TangentBuffer != NULL
			&& _dx12WeightBuffer != NULL);
	}
	// just switch it off if not needed any more (i.e. D3D is shutting down)
	else _pGfx->gl_ctVertices = 0;

	// reset and check
	_iVtxOffset = 0;
	_pGfx->gl_dwVertexShader = NONE;
	_pGfx->gl_dwPixelShader = NONE;
	_ulStreamsMask = NONE;
	_ulLastStreamsMask = NONE;
	_bProjectiveMapping = FALSE;
	_bLastProjectiveMapping = FALSE;
	_bGenerateTexCoord = FALSE;
	_bLastGenerateTexCoord = FALSE;
	_pd3dLockedVtx = _pd3dLockedNor = _pd3dLockedWgh = NULL;
	_pd3dLockedCol = _pd3dLockedTex = NULL;

	// reset to initial locking flags
	_dwVtxLockFlag = D3DLOCK_DISCARD;
	for( i=0; i<GFX_MAXLAYERS; i++) _dwColLockFlags[i] = _dwTexLockFlags[i] = D3DLOCK_DISCARD;
}



// prepare index arrays for drawing
extern void SetupIndexArray_D3D( INDEX ctIndices)
{
	ASSERT( ctIndices>=0);
	// clamp max indices
	// ctIndices = ClampUp( ctIndices, 65535L);

	delete _dx12IndexBuffer;
	_dx12IndexBuffer = NULL;
	_dx12DynamicIndices.clear();
	_pGfx->gl_pd3dIdx = NULL;

	// allocate if needed
	if( ctIndices>0)
	{
		// eventually update max index count
		if( _pGfx->gl_ctIndices < ctIndices) _pGfx->gl_ctIndices = ctIndices;
		else ctIndices = _pGfx->gl_ctIndices;
		_dx12DynamicIndices.resize(ctIndices);
		_dx12IndexBuffer = new CDirectX12Buffer;
		if (_dx12IndexBuffer == NULL
			|| !_dx12IndexBuffer->CreateIndexBuffer(
				GetDirectX12Backend().GetDevice(),
				ctIndices * GFX_IDXSIZE,
				DXGI_FORMAT_R16_UINT))
		{
			delete _dx12IndexBuffer;
			_dx12IndexBuffer = NULL;
			ASSERTALWAYS("No se pudo crear el index buffer DX12.");
		}
	}
	// just switch it off if not needed any more (i.e. D3D is shutting down)
	else _pGfx->gl_ctIndices = 0;
	
	// reset and check
	_iIdxOffset = 0;
	// ASSERT(_pGfx->gl_ctIndices<65536);
}


// initialize Direct3D driver
BOOL CGfxLibrary::InitDriver_D3D(void)
{
	// D3D9 is loaded only as the Windows translation entry point. Rendering is
	// performed by the explicitly supplied D3D12 device and command queue.
	CPrintF("Init DirectX 12 backend (D3D9On12 transition layer).\n");
	gl_hiDriver = LoadLibrary("D3D9.DLL"); // ###
	if (gl_hiDriver == NONE) {
		CPrintF("DX12 error: D3D9On12 entry module is not installed.\n");
		gl_gaAPI[GAT_D3D].ga_ctAdapters = 0;
		return FALSE;
	}
	CPrintF("DX12 info: D3D9On12 entry module loaded.\n");

	// Create a native D3D12 device/queue and bind the compatibility interface
	// to those exact objects. There is intentionally no native-D3D9 fallback.
	if (!GetDirectX12Backend().Initialize((HMODULE)gl_hiDriver, &gl_pD3D9)) {
		CPrintF("DX12 error: Cannot initialize D3D12 or D3D9On12.\n");
		FreeLibrary((HMODULE)gl_hiDriver);
		gl_hiDriver = NONE;
		return FALSE;
	}
	CPrintF("DX12 info: Native device and graphics queue initialized.\n");

	// made it!
	return TRUE;
}

// initialize Direct3D driver
void CGfxLibrary::EndDriver_D3D(void)
{
	// free occlusion queries
	if( _puiOcclusionQueryIDs!=NULL) { 
		FreeMemory(_puiOcclusionQueryIDs);
	 _puiOcclusionQueryIDs = NULL;
	}

	// reset shader and vertices
	SetupShader_D3D(NONE); 
	SetupVertexArrays_D3D(0); 
	SetupIndexArray_D3D(0); 
	gl_d3dColorFormat = (D3DFORMAT)NONE;
	gl_d3dDepthFormat = (D3DFORMAT)NONE;

	// restore system gamma table
	if( gl_ulFlags & GLF_ADJUSTABLEGAMMA) {
		gl_pd3d9Device->SetGammaRamp(/* #### [in] UINT iSwapChain */ 0, NONE, pgrtSystemGamma);
	}

	// El backend conserva referencias a texturas D3D9 del ultimo lote de UI.
	// Debe vaciarlas mientras el dispositivo y sus objetos COM siguen vivos.
	GetDirectX12Backend().Shutdown();

	// shutdown device and d3d
	INDEX iRef;
	iRef = gl_pd3d9Device->Release();
	iRef = gl_pD3D9->Release();
	gl_pd3d9Device = NULL;
	gl_pD3D9 = NULL;
}


// prepare current viewport for rendering thru Direct3D
BOOL CGfxLibrary::SetCurrentViewport_D3D(CViewPort *pvp)
{
	// determine full screen mode
	CDisplayMode dm;
	RECT rectWindow;
	GetCurrentDisplayMode(dm);
	ASSERT( (dm.dm_pixSizeI==0 && dm.dm_pixSizeJ==0) || (dm.dm_pixSizeI!=0 && dm.dm_pixSizeJ!=0));
	GetClientRect( pvp->vp_hWnd, &rectWindow);
	const PIX pixWinSizeI = rectWindow.right  - rectWindow.left;
	const PIX pixWinSizeJ = rectWindow.bottom - rectWindow.top;
	GetDirectX12Backend().ConfigurePresentation(
		pvp->vp_hWnd,
		static_cast<UINT>(pixWinSizeI),
		static_cast<UINT>(pixWinSizeJ));

	// full screen allows only one window (main one, which has already been initialized)
	if( dm.dm_pixSizeI==pixWinSizeI && dm.dm_pixSizeJ==pixWinSizeJ) {
		gl_pvpActive = pvp;  // remember as current viewport (must do that BEFORE InitContext)
		if( gl_ulFlags & GLF_INITONNEXTWINDOW) InitContext_D3D();
		gl_ulFlags &= ~GLF_INITONNEXTWINDOW;
		return TRUE; 
	}

	// if must init entire D3D
	if( gl_ulFlags & GLF_INITONNEXTWINDOW) {
		gl_ulFlags &= ~GLF_INITONNEXTWINDOW;
		// additional swap chains have been destroyed
		pvp->vp9_pSwapChain = NULL;
		pvp->vp9_pSurfDepth = NULL;
		// reopen window
		pvp->CloseCanvas();
		pvp->OpenCanvas();
		gl_pvpActive = pvp;
		InitContext_D3D();
		pvp->vp_ctDisplayChanges = gl_ctDriverChanges;
		return TRUE;
	}

	// if window was not set for this driver
	if( pvp->vp_ctDisplayChanges<gl_ctDriverChanges) {
		// additional swap chains have been destroyed
		//pvp->vp9_pSwapChain = NULL;
		//pvp->vp9_pSurfDepth = NULL;
		// reopen window
		pvp->CloseCanvas();
		pvp->OpenCanvas();
		pvp->vp_ctDisplayChanges = gl_ctDriverChanges;
		gl_pvpActive = pvp;
		return TRUE;
	}

	// no need to set context if it is the same window as last time
	if( gl_pvpActive!=NULL && gl_pvpActive->vp_hWnd==pvp->vp_hWnd) return TRUE;

	// set rendering target
	HRESULT hr;
	LPDIRECT3DSURFACE9 pColorSurface =NULL;
	if( pvp->vp9_pSwapChain != NULL) 
	{
		hr = pvp->vp9_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pColorSurface);
		if( hr!=D3D_OK) return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////
	// [070711: Su-won] WORLDEDITOR_BUGFIX							  |---------->
	if(pvp->vp9_pSurfDepth==NULL || pColorSurface ==NULL)
	{
		return FALSE;	
	}
	// [070711: Su-won] WORLDEDITOR_BUGFIX							  <----------|
	//////////////////////////////////////////////////////////////////////////////

	hr = gl_pd3d9Device->SetRenderTarget(0,  pColorSurface/*, pvp->vp9_pSurfDepth*/);
	D3DRELEASE( pColorSurface, TRUE);
	if( hr!=D3D_OK) return FALSE;
	GetDirectX12Backend().TrackLegacy3DRenderTarget(
		DX12_LEGACY_RENDER_TARGET_PRESENTATION);

	// remember as current window
	gl_pvpActive = pvp;
	return TRUE;
}


// prepares Direct3D drawing context
void CGfxLibrary::InitContext_D3D()
{
	// must have context
	ASSERT(gl_pvpActive != NULL);

	// report header
	CPrintF(TRANS("\n* Direct3D context created: *----------------------------------\n"));
	CDisplayAdapter& da = gl_gaAPI[GAT_D3D].ga_adaAdapter[gl_iCurrentAdapter];
	CPrintF("  (%s, %s, %s)\n\n", da.da_strVendor, da.da_strRenderer, da.da_strVersion);

	// <-- Sección para registrar la información de pantalla en ErrorLog.txt.
	extern CTString _strDisplayDriver;
	extern CTString _strDisplayDriverVersion;
	_strDisplayDriver = da.da_strRenderer;
	_strDisplayDriverVersion = da.da_strVersion;
	// -->

	HRESULT hr;
	GetDirectX12Backend().ResetLegacy3DState();

	// reset engine's internal Direct3D state variables
	GFX_bTruform = FALSE;
	GFX_bClipping = TRUE;
	GFX_bFrontFace = TRUE;
	GFX_bUseVertexProgram = FALSE;
	GFX_bUsePixelProgram = FALSE;
	GFX_ulCurrentColorMask = 12345678; // force next call to this function to be efficient
	hr = gl_pd3d9Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);     D3D_CHECKERROR(hr);  GFX_bDepthTest = FALSE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);      D3D_CHECKERROR(hr);  GFX_bDepthWrite = FALSE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);   D3D_CHECKERROR(hr);  GFX_bAlphaTest = FALSE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);  D3D_CHECKERROR(hr);  GFX_bBlending = FALSE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_DITHERENABLE, TRUE);       D3D_CHECKERROR(hr);  GFX_bDithering = TRUE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_COLORVERTEX, FALSE);       D3D_CHECKERROR(hr);  GFX_bColorArray = FALSE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_LIGHTING, FALSE);          D3D_CHECKERROR(hr);  GFX_bLighting = FALSE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);   D3D_CHECKERROR(hr);  GFX_eCullFace = GFX_NONE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);  D3D_CHECKERROR(hr);  GFX_eDepthFunc = GFX_LESS_EQUAL;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);  D3D_CHECKERROR(hr);  GFX_eBlendSrc = GFX_ONE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);  D3D_CHECKERROR(hr);  GFX_eBlendDst = GFX_ONE;
	hr = gl_pd3d9Device->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);   D3D_CHECKERROR(hr);  GFX_bClipPlane = FALSE;

	// set global ambient to black and disable all lights
	hr = gl_pd3d9Device->SetRenderState(D3DRS_AMBIENT, 0); D3D_CHECKERROR(hr); // or 0xFFFFFFFF !!!!
	for (INDEX iLight = 0; iLight < GFX_MAXLIGHTS; iLight++) {
		GFX_abLights[iLight] = FALSE;
		gl_pd3d9Device->LightEnable(iLight, FALSE);
	}

	// (re)set some D3D defaults
	hr = gl_pd3d9Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL); D3D_CHECKERROR(hr);
	hr = gl_pd3d9Device->SetRenderState(D3DRS_ALPHAREF, 128);                 D3D_CHECKERROR(hr);

	// constant color default setup
	D3DMATERIAL9 d3dMaterial;
	memset(&d3dMaterial, 0, sizeof(d3dMaterial));
	d3dMaterial.Diffuse.r = d3dMaterial.Ambient.r = 1.0f;
	d3dMaterial.Diffuse.g = d3dMaterial.Ambient.g = 1.0f;
	d3dMaterial.Diffuse.b = d3dMaterial.Ambient.b = 1.0f;
	d3dMaterial.Diffuse.a = d3dMaterial.Ambient.a = 1.0f;
	hr = gl_pd3d9Device->SetMaterial(&d3dMaterial);
	D3D_CHECKERROR(hr);

	// set default texture unit and modulation mode
	GFX_iActiveTexUnit = 0;
	// reset frustum/ortho matrix
	extern BOOL  GFX_bViewMatrix;
	extern FLOAT GFX_fLastL, GFX_fLastR, GFX_fLastT, GFX_fLastB, GFX_fLastN, GFX_fLastF;
	GFX_fLastL = GFX_fLastR = GFX_fLastT = GFX_fLastB = GFX_fLastN = GFX_fLastF = 0;
	GFX_bViewMatrix = TRUE;

	// reset depth range
	GFX_fMinDepthRange = 0.0f;
	GFX_fMaxDepthRange = 1.0f;
	D3DVIEWPORT9 d3dViewPort = { 0,0, 8,8, 0,1 };
	hr = gl_pd3d9Device->SetViewport(&d3dViewPort);
	D3D_CHECKERROR(hr);
	GetDirectX12Backend().TrackLegacy3DViewport(
		d3dViewPort.X,
		d3dViewPort.Y,
		d3dViewPort.Width,
		d3dViewPort.Height,
		d3dViewPort.MinZ,
		d3dViewPort.MaxZ);
#ifndef NDEBUG
	hr = gl_pd3d9Device->GetViewport(&d3dViewPort);
	D3D_CHECKERROR(hr);
	ASSERT(d3dViewPort.MinZ == 0 && d3dViewPort.MaxZ == 1);
#endif

	// get capabilities
	D3DCAPS9 d3dCaps;
	hr = gl_pd3d9Device->GetDeviceCaps(&d3dCaps);
	D3D_CHECKERROR(hr);

	// if full screen and gamma adjustment is supported
	gl_ulFlags &= ~GLF_ADJUSTABLEGAMMA;
	if (gl_ulFlags & GLF_FULLSCREEN) {
		if (d3dCaps.Caps2 & D3DCAPS2_FULLSCREENGAMMA) {
			// store system gamma table
			gl_pd3d9Device->GetGammaRamp(0, pgrtSystemGamma);
			gl_ulFlags |= GLF_ADJUSTABLEGAMMA;
			for (INDEX i = 0; i < 256 * 3; i++) ((UWORD*)pgrtSystemGamma)[i] <<= 8;
		}
		else CPrintF(TRANS("\nWARNING: Gamma, brightness and contrast are not adjustable.\n\n"));
	}

	// determine rasterizer acceleration
	gl_ulFlags &= ~GLF_HASACCELERATION;
	if ((d3dCaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION)
		|| d3dDevType == D3DDEVTYPE_REF) gl_ulFlags |= GLF_HASACCELERATION;

	// determine support for 32-bit textures
	gl_ulFlags &= ~GLF_32BITTEXTURES;
	if (HasTextureFormat_D3D(D3DFMT_X8R8G8B8)
		|| HasTextureFormat_D3D(D3DFMT_A8R8G8B8)) gl_ulFlags |= GLF_32BITTEXTURES;

	// determine support for compressed textures
	gl_ulFlags &= ~GLF_TEXTURECOMPRESSION;
	if (HasTextureFormat_D3D(D3DFMT_DXT1)) gl_ulFlags |= GLF_TEXTURECOMPRESSION;

	// determine max supported dimension of texture
	gl_pixMaxTextureDimension = d3dCaps.MaxTextureWidth;
	ASSERT(gl_pixMaxTextureDimension == d3dCaps.MaxTextureHeight); // perhaps not ?

	// determine max primitive count
	gl_ctMaxPrimitives = d3dCaps.MaxPrimitiveCount;

	// determine support for disabling of color buffer writes
	gl_ulFlags &= ~GLF_D3D_COLORWRITES;
	if (d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) gl_ulFlags |= GLF_D3D_COLORWRITES;

	// determine support for custom clip planes
	gl_ulFlags &= ~GLF_D3D_CLIPPLANE;
	if (d3dCaps.MaxUserClipPlanes > 0) gl_ulFlags |= GLF_D3D_CLIPPLANE;
	else CPrintF(TRANS("User clip plane not supported - mirrors will not work well.\n"));

	// determine support for texture LOD biasing
	gl_fMaxTextureLODBias = 0.0f;
	if (d3dCaps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS) {
		gl_fMaxTextureLODBias = 4.0f;
	}

	// determine support for anisotropic filtering
	gl_iMaxTextureAnisotropy = 1;
	if (d3dCaps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY) {
		gl_iMaxTextureAnisotropy = d3dCaps.MaxAnisotropy;
		ASSERT(gl_iMaxTextureAnisotropy > 1);
	}

	// determine support for z-biasing
	//
	//gl_ulFlags &= ~GLF_D3D_ZBIAS;
	//if( d3dCaps.RasterCaps & D3DPRASTERCAPS_ZBIAS) gl_ulFlags |= GLF_D3D_ZBIAS;
	//

	// check support for vsync swapping
	gl_ulFlags &= ~GLF_VSYNC;
	if (d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE) {
		if (d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE) gl_ulFlags |= GLF_VSYNC;
	}
	else CPrintF(TRANS("  Vertical syncronization cannot be disabled.\n"));

	// determine support for vertex shader (i.e. program)
	// Fecha: 2006-05-16 (16:48:55), por eons.
	gl_ulFlags &= ~GLF_VERTEXPROGRAM;
	if (_pGfx->gl_pd3d9Caps.MaxStreams >= 8 && _pGfx->gl_pd3d9Caps.VertexShaderVersion >= 0x0101 && _pGfx->gl_pd3d9Caps.MaxVertexShaderConst >= 96) {
		gl_ulFlags |= GLF_VERTEXPROGRAM;
	}

	// determine support for pixel shader
	gl_ulFlags &= ~GLF_PIXELPROGRAM;
	if (d3dCaps.PixelShaderVersion >= 0x0101) {
		gl_ulFlags |= GLF_PIXELPROGRAM;
	}

	BOOL bPS14 = TRUE;

	if (d3dCaps.PixelShaderVersion < D3DPS_VERSION(1, 4))
	{ // Pixel Shader 1.4 no es compatible.
		bPS14 = FALSE;
	}

	// determine support for N-Patches
	extern INDEX truform_iLevel;
	extern BOOL  truform_bLinear;
	truform_iLevel = -1;
	truform_bLinear = FALSE;
	gl_iTessellationLevel = 0;
	gl_iMaxTessellationLevel = 0;
	if (d3dCaps.DevCaps & D3DDEVCAPS_NPATCHES) {
		if (gl_ctMaxStreams > GFX_MINSTREAMS) {
			gl_iMaxTessellationLevel = 7;
			hr = gl_pd3d9Device->SetRenderState(D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE);
			D3D_CHECKERROR(hr);
		}
		else CPrintF(TRANS("Not enough streams - N-Patches cannot be used.\n"));
	}

	// determine support for multi-texturing (only if Modulate2X mode is supported!)
	gl_ctTextureUnits = 1;
	gl_ctRealTextureUnits = d3dCaps.MaxSimultaneousTextures;
	if (gl_ctRealTextureUnits > 1) {
		// check everything that is required for multi-texturing
		if (!(d3dCaps.TextureOpCaps & D3DTOP_MODULATE2X)) CPrintF(TRANS("Texture operation MODULATE2X missing - multi-texturing cannot be used.\n"));
		else if (gl_ctMaxStreams <= GFX_MINSTREAMS)       CPrintF(TRANS("Not enough streams - multi-texturing cannot be used.\n"));
		else gl_ctTextureUnits = Min(GFX_USETEXUNITS, Min(gl_ctRealTextureUnits, 1 + gl_ctMaxStreams - GFX_MINSTREAMS));
	}

	// disable texturing
	for (INDEX iUnit = 0; iUnit < gl_ctRealTextureUnits; iUnit++) {
		GFX_abTexture[iUnit] = FALSE;
		GFX_iTexModulation[iUnit] = 1;
		hr = gl_pd3d9Device->SetTexture(iUnit, NULL);                                      D3D_CHECKERROR(hr);
		hr = gl_pd3d9Device->SetTextureStageState(iUnit, D3DTSS_COLOROP, D3DTOP_DISABLE);  D3D_CHECKERROR(hr);
		hr = gl_pd3d9Device->SetTextureStageState(iUnit, D3DTSS_ALPHAOP, D3DTOP_MODULATE); D3D_CHECKERROR(hr);
	}

	// setup fog and haze textures
	extern PIX _fog_pixSizeH;
	extern PIX _fog_pixSizeL;
	extern PIX _haze_pixSize;
	_fog_ulTexture = NONE;
	_haze_ulTexture = NONE;
	_fog_pixSizeH = 0;
	_fog_pixSizeL = 0;
	_haze_pixSize = 0;

	// prepare pattern texture
	extern CTexParams _tpPattern;
	extern ULONG64 _ulPatternTexture;
	extern ULONG64 _ulLastUploadedPattern;
	_ulPatternTexture = NONE;
	_ulLastUploadedPattern = 0;
	_tpPattern.Clear();

	// determine number of color/texcoord buffers
	gl_ctColBuffers = 1;
	gl_ctTexBuffers = gl_ctTextureUnits;
	INDEX ctStreamsRemain = gl_ctMaxStreams - (GFX_MINSTREAMS - 1 + gl_ctTextureUnits); // -1 because of 1 texture unit inside MinStreams
	while (ctStreamsRemain > 0) {
		if (gl_ctTexBuffers == GFX_MAXLAYERS && gl_ctColBuffers == GFX_MAXLAYERS) break;  // done if no need for more streams
		(gl_ctColBuffers < gl_ctTexBuffers) ? gl_ctColBuffers++ : gl_ctTexBuffers++;    // increase number of tex or color buffers
		ctStreamsRemain--;  // advance to next stream
	}

	// prepare vertex arrays
	INDEX i;
	gl_pd3dIdx = NULL;
	gl_pd3dVtx = NULL;
	gl_pd3dNor = NULL;
	gl_pd3dWgh = NULL;
	for (i = 0; i < GFX_MAXLAYERS; i++) gl_pd3dCol[i] = gl_pd3dTex[i] = NULL;
	ASSERT(gl_ctTexBuffers > 0 && gl_ctTexBuffers <= GFX_MAXLAYERS);
	ASSERT(gl_ctColBuffers > 0 && gl_ctColBuffers <= GFX_MAXLAYERS);
	gl_ctVertices = 0;
	gl_ctIndices = 0;
	extern INDEX d3d_iVertexBuffersSize;
	extern INDEX _iLastVertexBufferSize;
	d3d_iVertexBuffersSize = (d3d_iVertexBuffersSize + 3) & (~3); // round to 4
	d3d_iVertexBuffersSize = Clamp(d3d_iVertexBuffersSize, 64L, 4096L);
	const INDEX ctVertices = VerticesFromSize_D3D(d3d_iVertexBuffersSize);
	_iLastVertexBufferSize = d3d_iVertexBuffersSize;
	SetupVertexArrays_D3D(ctVertices);
	SetupIndexArray_D3D(2 * ctVertices);
	// init vertex buffers
	extern void InitVertexBuffers(void);
	InitVertexBuffers();

	// reset texture filtering and some static vars
	for (i = 0; i < GFX_MAXTEXUNITS; i++) _tpGlobal[i].Clear();
	_avsFixedShaders.Clear();
	_iVtxOffset = 0;
	_iIdxOffset = 0;
	_dwCurrentVS = NONE;
	_dwCurrentPS = NONE;
	_dwLastVertexProgram = NONE;
	GFX_ctVertices = 0;

	// set default texture filtering/biasing
	extern INDEX gap_iTextureFiltering;
	extern INDEX gap_iTextureAnisotropy;
	extern FLOAT gap_fTextureLODBias;
	gfxSetTextureFiltering(gap_iTextureFiltering, gap_iTextureAnisotropy);
	gfxSetTextureBiasing(gap_fTextureLODBias);

	// generate occlusion query IDs
	_puiOcclusionQueryIDs = (UINT*)AllocMemory(GFX_MAXOCCQUERIES * sizeof(UINT));
	for (INDEX iID = 0; iID < GFX_MAXOCCQUERIES; iID++) _puiOcclusionQueryIDs[iID] = iID;

	// mark pretouching and probing
	extern BOOL _bNeedPretouch;
	_bNeedPretouch = TRUE;
	gl_bAllowProbing = FALSE;

	// update console system vars
	extern void UpdateGfxSysCVars(void);
	UpdateGfxSysCVars();

	// reload all loaded textures and eventually shadowmaps
	extern INDEX shd_bCacheAll;
	extern void ReloadTextures(void);
	extern void ReloadMeshes(void);
	extern void CacheShadows(void);
	ReloadTextures();
	ReloadMeshes();
	if (shd_bCacheAll) CacheShadows();

	// Inicio de la modificación de Ahn Tae-hoon. // (Agregar y modificar el efecto SSSE) (0.1)
	//	Initialize_EffectSystem();
	// Fin de la modificación de Ahn Tae-hoon. // (Agregar y modificar el efecto SSSE) (0.1)
}

/*
// prepares Direct3D drawing context
void CGfxLibrary::InitContext_D3D()
{
	// must have context
	ASSERT( gl_pvpActive!=NULL);

	// report header
	CPrintF( TRANS("\n* Direct3D context created: *----------------------------------\n"));
	CDisplayAdapter &da = gl_gaAPI[GAT_D3D].ga_adaAdapter[gl_iCurrentAdapter];
	CPrintF( "  (%s, %s, %s)\n\n", da.da_strVendor, da.da_strRenderer, da.da_strVersion);

	// <-- Seccion para registrar la informacion de pantalla en ErrorLog.txt.
	extern CTString _strDisplayDriver;
	extern CTString _strDisplayDriverVersion;
	_strDisplayDriver = da.da_strRenderer;
	_strDisplayDriverVersion = da.da_strVersion;
	// -->
	
	HRESULT hr;

	// reset engine's internal Direct3D state variables
	GFX_bTruform   = FALSE;
	GFX_bClipping  = TRUE;
	GFX_bFrontFace = TRUE;
	GFX_bUseVertexProgram = FALSE;
	GFX_bUsePixelProgram  = FALSE;
	GFX_ulCurrentColorMask = 12345678; // force next call to this function to be efficient
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE);     D3D_CHECKERROR(hr);  GFX_bDepthTest  = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);      D3D_CHECKERROR(hr);  GFX_bDepthWrite = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);   D3D_CHECKERROR(hr);  GFX_bAlphaTest  = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);  D3D_CHECKERROR(hr);  GFX_bBlending   = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);       D3D_CHECKERROR(hr);  GFX_bDithering  = TRUE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, FALSE);       D3D_CHECKERROR(hr);  GFX_bColorArray = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);          D3D_CHECKERROR(hr);  GFX_bLighting   = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);   D3D_CHECKERROR(hr);  GFX_eCullFace   = GFX_NONE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);  D3D_CHECKERROR(hr);  GFX_eDepthFunc  = GFX_LESS_EQUAL;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE);  D3D_CHECKERROR(hr);  GFX_eBlendSrc   = GFX_ONE; 
	hr = gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);  D3D_CHECKERROR(hr);  GFX_eBlendDst   = GFX_ONE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, FALSE);   D3D_CHECKERROR(hr);  GFX_bClipPlane  = FALSE;

	// set global ambient to black and disable all lights
	hr = gl_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0); D3D_CHECKERROR(hr); // or 0xFFFFFFFF !!!!
	for( INDEX iLight=0; iLight<GFX_MAXLIGHTS; iLight++) {
		GFX_abLights[iLight] = FALSE;
		gl_pd3dDevice->LightEnable( iLight, FALSE);
	}
		
	// (re)set some D3D defaults
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL); D3D_CHECKERROR(hr); 
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,  128);                 D3D_CHECKERROR(hr); 

	// constant color default setup
	D3DMATERIAL8 d3dMaterial;
	memset( &d3dMaterial, 0, sizeof(d3dMaterial));
	d3dMaterial.Diffuse.r = d3dMaterial.Ambient.r = 1.0f;
	d3dMaterial.Diffuse.g = d3dMaterial.Ambient.g = 1.0f;
	d3dMaterial.Diffuse.b = d3dMaterial.Ambient.b = 1.0f;
	d3dMaterial.Diffuse.a = d3dMaterial.Ambient.a = 1.0f;
	hr = gl_pd3dDevice->SetMaterial(&d3dMaterial);
	D3D_CHECKERROR(hr); 

	// set default texture unit and modulation mode
	GFX_iActiveTexUnit = 0;
	// reset frustum/ortho matrix
	extern BOOL  GFX_bViewMatrix;
	extern FLOAT GFX_fLastL, GFX_fLastR, GFX_fLastT, GFX_fLastB, GFX_fLastN, GFX_fLastF;
	GFX_fLastL = GFX_fLastR = GFX_fLastT = GFX_fLastB = GFX_fLastN = GFX_fLastF = 0;
	GFX_bViewMatrix = TRUE;

	// reset depth range
	GFX_fMinDepthRange = 0.0f;
	GFX_fMaxDepthRange = 1.0f;
	D3DVIEWPORT8 d3dViewPort = { 0,0, 8,8, 0,1 };
	hr = gl_pd3dDevice->SetViewport( &d3dViewPort);
	D3D_CHECKERROR(hr);
#ifndef NDEBUG
	hr = gl_pd3dDevice->GetViewport( &d3dViewPort);
	D3D_CHECKERROR(hr);
	ASSERT( d3dViewPort.MinZ==0 && d3dViewPort.MaxZ==1);
#endif

	// get capabilities
	D3DCAPS8 d3dCaps;
	hr = gl_pd3dDevice->GetDeviceCaps(&d3dCaps);
	D3D_CHECKERROR(hr);

	// if full screen and gamma adjustment is supported
	gl_ulFlags &= ~GLF_ADJUSTABLEGAMMA;
	if( gl_ulFlags & GLF_FULLSCREEN) {
		if( d3dCaps.Caps2 & D3DCAPS2_FULLSCREENGAMMA) {
			// store system gamma table
			gl_pd3dDevice->GetGammaRamp(pgrtSystemGamma);
			gl_ulFlags |= GLF_ADJUSTABLEGAMMA;
			for( INDEX i=0; i<256*3; i++) ((UWORD*)pgrtSystemGamma)[i] <<= 8;
		} else CPrintF( TRANS("\nWARNING: Gamma, brightness and contrast are not adjustable.\n\n"));
	}

	// determine rasterizer acceleration
	gl_ulFlags &= ~GLF_HASACCELERATION;
	if( (d3dCaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION)
		|| d3dDevType==D3DDEVTYPE_REF) gl_ulFlags |= GLF_HASACCELERATION;

	// determine support for 32-bit textures
	gl_ulFlags &= ~GLF_32BITTEXTURES;
	if( HasTextureFormat_D3D(D3DFMT_X8R8G8B8)
	 || HasTextureFormat_D3D(D3DFMT_A8R8G8B8)) gl_ulFlags |= GLF_32BITTEXTURES;

	// determine support for compressed textures
	gl_ulFlags &= ~GLF_TEXTURECOMPRESSION;
	if( HasTextureFormat_D3D(D3DFMT_DXT1)) gl_ulFlags |= GLF_TEXTURECOMPRESSION;

	// determine max supported dimension of texture
	gl_pixMaxTextureDimension = d3dCaps.MaxTextureWidth;
	ASSERT( gl_pixMaxTextureDimension == d3dCaps.MaxTextureHeight); // perhaps not ?

	// determine max primitive count
	gl_ctMaxPrimitives = d3dCaps.MaxPrimitiveCount;

	// determine support for disabling of color buffer writes
	gl_ulFlags &= ~GLF_D3D_COLORWRITES;
	if( d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) gl_ulFlags |= GLF_D3D_COLORWRITES;

	// determine support for custom clip planes
	gl_ulFlags &= ~GLF_D3D_CLIPPLANE;
	if( d3dCaps.MaxUserClipPlanes>0) gl_ulFlags |= GLF_D3D_CLIPPLANE;
	else CPrintF( TRANS("User clip plane not supported - mirrors will not work well.\n"));

	// determine support for texture LOD biasing
	gl_fMaxTextureLODBias = 0.0f;
	if( d3dCaps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS) {
		gl_fMaxTextureLODBias = 4.0f;
	}

	// determine support for anisotropic filtering
	gl_iMaxTextureAnisotropy = 1;
	if( d3dCaps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY) {
		gl_iMaxTextureAnisotropy = d3dCaps.MaxAnisotropy;
		ASSERT( gl_iMaxTextureAnisotropy>1); 
	}

	// determine support for z-biasing
	gl_ulFlags &= ~GLF_D3D_ZBIAS;
	if( d3dCaps.RasterCaps & D3DPRASTERCAPS_ZBIAS) gl_ulFlags |= GLF_D3D_ZBIAS;

	// check support for vsync swapping
	gl_ulFlags &= ~GLF_VSYNC;
	if( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE) {
		if( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE) gl_ulFlags |= GLF_VSYNC;  
	} else CPrintF( TRANS("  Vertical syncronization cannot be disabled.\n"));

	// determine support for vertex shader (i.e. program)
	// Fecha: 2006-05-16 (4:48:55 p. m.), por eons.
	gl_ulFlags &= ~GLF_VERTEXPROGRAM;
	if( _pGfx->gl_pd3dCaps.MaxStreams>=8 && _pGfx->gl_pd3dCaps.VertexShaderVersion>=0x0101 && _pGfx->gl_pd3dCaps.MaxVertexShaderConst>=96 ) {
		gl_ulFlags |= GLF_VERTEXPROGRAM;
	}

	// determine support for pixel shader
	gl_ulFlags &= ~GLF_PIXELPROGRAM;
	if( d3dCaps.PixelShaderVersion>=0x0101 && d3dCaps.MaxPixelShaderValue>=1) {
		gl_ulFlags |= GLF_PIXELPROGRAM;
	}

	BOOL bPS14 = TRUE;

	if (d3dCaps.PixelShaderVersion < D3DPS_VERSION(1,4))
	{ // Pixel Shader 1.4 no es compatible.
		bPS14 = FALSE;
	}

	// determine support for N-Patches
	extern INDEX truform_iLevel;
	extern BOOL  truform_bLinear;
	truform_iLevel  = -1;
	truform_bLinear = FALSE;
	gl_iTessellationLevel    = 0;
	gl_iMaxTessellationLevel = 0;
	if( d3dCaps.DevCaps & D3DDEVCAPS_NPATCHES) {
		if( gl_ctMaxStreams>GFX_MINSTREAMS) {
			gl_iMaxTessellationLevel = 7;
			hr = gl_pd3dDevice->SetRenderState( D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE);
			D3D_CHECKERROR(hr);
		} else CPrintF( TRANS("Not enough streams - N-Patches cannot be used.\n"));
	}

	// determine support for multi-texturing (only if Modulate2X mode is supported!)
	gl_ctTextureUnits = 1;
	gl_ctRealTextureUnits = d3dCaps.MaxSimultaneousTextures;
	if( gl_ctRealTextureUnits>1) {
		// check everything that is required for multi-texturing
		if( !(d3dCaps.TextureOpCaps&D3DTOP_MODULATE2X)) CPrintF( TRANS("Texture operation MODULATE2X missing - multi-texturing cannot be used.\n"));
		else if( gl_ctMaxStreams<=GFX_MINSTREAMS)       CPrintF( TRANS("Not enough streams - multi-texturing cannot be used.\n"));
		else gl_ctTextureUnits = Min( GFX_USETEXUNITS, Min( gl_ctRealTextureUnits, 1+gl_ctMaxStreams-GFX_MINSTREAMS));
	}

	// disable texturing
	for( INDEX iUnit=0; iUnit<gl_ctRealTextureUnits; iUnit++) {
		GFX_abTexture[iUnit] = FALSE;
		GFX_iTexModulation[iUnit] = 1;
		hr = gl_pd3dDevice->SetTexture( iUnit, NULL);                                      D3D_CHECKERROR(hr);
		hr = gl_pd3dDevice->SetTextureStageState( iUnit, D3DTSS_COLOROP, D3DTOP_DISABLE);  D3D_CHECKERROR(hr); 
		hr = gl_pd3dDevice->SetTextureStageState( iUnit, D3DTSS_ALPHAOP, D3DTOP_MODULATE); D3D_CHECKERROR(hr); 
	}

	// setup fog and haze textures
	extern PIX _fog_pixSizeH;
	extern PIX _fog_pixSizeL;
	extern PIX _haze_pixSize;
	_fog_ulTexture  = NONE;
	_haze_ulTexture = NONE;
	_fog_pixSizeH = 0;
	_fog_pixSizeL = 0;
	_haze_pixSize = 0;

	// prepare pattern texture
	extern CTexParams _tpPattern;
    extern ULONG64 _ulPatternTexture;
    extern ULONG64 _ulLastUploadedPattern;
	_ulPatternTexture = NONE;
	_ulLastUploadedPattern = 0;
	_tpPattern.Clear();

	// determine number of color/texcoord buffers
	gl_ctColBuffers = 1;
	gl_ctTexBuffers = gl_ctTextureUnits;
	INDEX ctStreamsRemain = gl_ctMaxStreams - (GFX_MINSTREAMS-1+gl_ctTextureUnits); // -1 because of 1 texture unit inside MinStreams
	while( ctStreamsRemain>0) {
		if( gl_ctTexBuffers==GFX_MAXLAYERS && gl_ctColBuffers==GFX_MAXLAYERS) break;  // done if no need for more streams
		(gl_ctColBuffers<gl_ctTexBuffers) ? gl_ctColBuffers++ : gl_ctTexBuffers++;    // increase number of tex or color buffers
		ctStreamsRemain--;  // advance to next stream
	}

	// prepare vertex arrays
	INDEX i;
	gl_pd3dIdx = NULL;
	gl_pd3dVtx = NULL;
	gl_pd3dNor = NULL;
	gl_pd3dWgh = NULL;
	for( i=0; i<GFX_MAXLAYERS; i++) gl_pd3dCol[i] = gl_pd3dTex[i] = NULL;
	ASSERT( gl_ctTexBuffers>0 && gl_ctTexBuffers<=GFX_MAXLAYERS);
	ASSERT( gl_ctColBuffers>0 && gl_ctColBuffers<=GFX_MAXLAYERS);
	gl_ctVertices = 0;
	gl_ctIndices  = 0;
	extern INDEX d3d_iVertexBuffersSize;
	extern INDEX _iLastVertexBufferSize;
	d3d_iVertexBuffersSize = (d3d_iVertexBuffersSize+3) & (~3); // round to 4
	d3d_iVertexBuffersSize = Clamp( d3d_iVertexBuffersSize, 64L, 4096L);
	const INDEX ctVertices = VerticesFromSize_D3D(d3d_iVertexBuffersSize);
	_iLastVertexBufferSize = d3d_iVertexBuffersSize;
	SetupVertexArrays_D3D(ctVertices); 
	SetupIndexArray_D3D(2*ctVertices);
	// init vertex buffers
	extern void InitVertexBuffers(void);
	InitVertexBuffers();

	// reset texture filtering and some static vars
	for( i=0; i<GFX_MAXTEXUNITS; i++) _tpGlobal[i].Clear();
	_avsFixedShaders.Clear();
	_iVtxOffset = 0;
	_iIdxOffset = 0;
	_dwCurrentVS = NONE;
	_dwCurrentPS = NONE;
	_dwLastVertexProgram = NONE;
	GFX_ctVertices = 0;

	// set default texture filtering/biasing
	extern INDEX gap_iTextureFiltering;
	extern INDEX gap_iTextureAnisotropy;
	extern FLOAT gap_fTextureLODBias;
	gfxSetTextureFiltering( gap_iTextureFiltering, gap_iTextureAnisotropy);
	gfxSetTextureBiasing( gap_fTextureLODBias);

	// generate occlusion query IDs
	_puiOcclusionQueryIDs = (UINT*)AllocMemory( GFX_MAXOCCQUERIES*sizeof(UINT));
	for( INDEX iID=0; iID<GFX_MAXOCCQUERIES; iID++) _puiOcclusionQueryIDs[iID] = iID;

	// mark pretouching and probing
	extern BOOL _bNeedPretouch;
	_bNeedPretouch = TRUE;
	gl_bAllowProbing = FALSE;

	// update console system vars
	extern void UpdateGfxSysCVars(void);
	UpdateGfxSysCVars();

	// reload all loaded textures and eventually shadowmaps
	extern INDEX shd_bCacheAll;
	extern void ReloadTextures(void);
	extern void ReloadMeshes(void);
	extern void CacheShadows(void);
	ReloadTextures();
	ReloadMeshes();
	if( shd_bCacheAll) CacheShadows();

// Inicio de modificacion de Ahn Tae-hoon: agregar y modificar el efecto SSSE (0.1).
//	Initialize_EffectSystem();
// Fin de modificacion de Ahn Tae-hoon: agregar y modificar el efecto SSSE (0.1).
}
*/


// HELPER: returns number of bits for depth buffer format
static INDEX BitsFromDepthFormat_D3D( const D3DFORMAT d3df)
{
	switch(d3df) {
	case D3DFMT_D16:      return 16;
	case D3DFMT_D15S1:    return 15 | 0x80000000;
	case D3DFMT_D32:      return 32;
	case D3DFMT_D24X8:    return 24;
	case D3DFMT_D24S8:    return 24 | 0x80000000;
	case D3DFMT_D24X4S4:  return 24 | 0x80000000;
	case D3DFMT_D16_LOCKABLE:  return 16;
	default: return 0;
	}
}


// find depth buffer format (for specified color format) that closest matches required bit depth
// (returns new z-depth bits, and flag if stencil buffer is supported - highest bit in depth-bits!)
static D3DFORMAT FindDepthFormat_D3D( INDEX iAdapter, D3DFORMAT d3dfColor, INDEX iDepthBits)
{
	// adjust required Z-depth from color depth if needed
	if( iDepthBits==0) {
		if( d3dfColor==D3DFMT_X8R8G8B8 || d3dfColor==D3DFMT_A8R8G8B8) iDepthBits = 32;
		else iDepthBits = 16;
	}

	// tries' tables
	const  INDEX ctTries = 7;
	static D3DFORMAT ad3df16BitsTable[] = { D3DFMT_D16,   D3DFMT_D15S1, D3DFMT_D16_LOCKABLE, D3DFMT_D32,     D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D24X4S4      };
	static D3DFORMAT ad3df24BitsTable[] = { D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D24X4S4,      D3DFMT_D32,     D3DFMT_D16,   D3DFMT_D15S1, D3DFMT_D16_LOCKABLE };
	static D3DFORMAT ad3df32BitsTable[] = { D3DFMT_D32,   D3DFMT_D24X8, D3DFMT_D24S8,        D3DFMT_D24X4S4, D3DFMT_D16,   D3DFMT_D15S1, D3DFMT_D16_LOCKABLE };

	// find corresponding table based on depth bits
	D3DFORMAT *pd3dfDepthTable = &ad3df32BitsTable[0];
	if( iDepthBits<21) pd3dfDepthTable = &ad3df16BitsTable[0];
	else if( iDepthBits<28) pd3dfDepthTable = &ad3df24BitsTable[0];

// Inicio de modificacion de Kang Dong-min.
	/*
	// Configura el formato como D3DFMT_D24S8.
	D3DFORMAT d3dfDepth = pd3dfDepthTable[2];
	HRESULT hr;
	hr = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter, d3dDevType, d3dfColor, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, d3dfDepth);
	//D3D_CheckError(hr);

	hr = _pGfx->gl_pD3D->CheckDepthStencilMatch( iAdapter, d3dDevType, d3dfColor, d3dfColor, d3dfDepth);
	if( hr==D3D_OK) 
		return d3dfDepth; // done if found
		*/

	// Version original.
	// loop thru table
	for( INDEX i=0; i<ctTries; i++)
	{ // fetch format from table
		HRESULT hr;
		D3DFORMAT d3dfDepth = pd3dfDepthTable[i];
		hr = _pGfx->gl_pD3D9->CheckDeviceFormat( iAdapter, d3dDevType, d3dfColor, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, d3dfDepth);
		if( hr!=D3D_OK) continue; // skip if format not supported at all
		hr = _pGfx->gl_pD3D9->CheckDepthStencilMatch( iAdapter, d3dDevType, d3dfColor, d3dfColor, d3dfDepth);
		if( hr==D3D_OK) return d3dfDepth; // done if found
	}
// Fin de modificacion de Kang Dong-min.

	// not found :(
	iDepthBits = 0;
	ASSERT( "FindDepthFormat_D3D: Not found?!" );
	return D3DFMT_UNKNOWN; 
}


// prepare display mode
BOOL CGfxLibrary::InitDisplay_D3D( INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ, enum DisplayDepth eColorDepth)
{
	// reset
	HRESULT hr;
	D3DDISPLAYMODE d3dDisplayMode;
	D3DPRESENT_PARAMETERS d3dPresentParams; // ###
	gl_pD3D9->GetAdapterDisplayMode( iAdapter, &d3dDisplayMode);
	memset( &d3dPresentParams, 0, sizeof(d3dPresentParams));

	// readout device capabilities
	D3DCAPS9 d3dCaps;
	hr = gl_pD3D9->GetDeviceCaps( iAdapter, d3dDevType, &d3dCaps);
	D3D_CHECKERROR(hr);

	// clamp depth/stencil values
	extern INDEX gap_iDepthBits;
			 if( gap_iDepthBits<12) gap_iDepthBits = 0;
	else if( gap_iDepthBits<21) gap_iDepthBits = 16;
	else if( gap_iDepthBits<28) gap_iDepthBits = 24;
	else                        gap_iDepthBits = 32;

	// prepare  
	INDEX iZDepth = gap_iDepthBits;
	D3DFORMAT d3dDepthFormat = D3DFMT_UNKNOWN;
	D3DFORMAT d3dColorFormat = d3dDisplayMode.Format;
	d3dPresentParams.BackBufferCount = 1;
	d3dPresentParams.MultiSampleType = D3DMULTISAMPLE_NONE; // !!!! TODO
	d3dPresentParams.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dPresentParams.SwapEffect = D3DSWAPEFFECT_COPY;
	const BOOL bFullScreen = (pixSizeI>0 && pixSizeJ>0); 

	// setup for full screen
	if( bFullScreen) {
		// determine color and depth format
		if( eColorDepth==DISPD_16BIT) d3dColorFormat = D3DFMT_R5G6B5;
		if( eColorDepth==DISPD_32BIT) d3dColorFormat = D3DFMT_X8R8G8B8;
		d3dDepthFormat = FindDepthFormat_D3D( iAdapter, d3dColorFormat, iZDepth);
		iZDepth = BitsFromDepthFormat_D3D(d3dDepthFormat);

		// determine refresh rate and presentation interval
		extern INDEX gap_iRefreshRate;
		const UINT uiRefresh = gap_iRefreshRate>0 ? gap_iRefreshRate : D3DPRESENT_RATE_DEFAULT;

		// determine presentation interval
		extern INDEX gap_iSwapInterval;
		gap_iSwapInterval = Clamp( gap_iSwapInterval, 0L, 3L);
		// check if supported
		const UINT uiSupportedIntervals = d3dCaps.PresentationIntervals;
		if( gap_iSwapInterval==3 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_THREE))     gap_iSwapInterval = 2;
		if( gap_iSwapInterval==2 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_TWO))       gap_iSwapInterval = 1;
		if( gap_iSwapInterval==1 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_ONE))       gap_iSwapInterval = 0;
		if( gap_iSwapInterval==0 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_IMMEDIATE)) gap_iSwapInterval = 1;
		const UINT uiInterval = _auiSwapIntervals[gap_iSwapInterval];
		gl_iSwapInterval = gap_iSwapInterval;  // copy to gfx lib

		// set context directly to main window
		d3dPresentParams.Windowed = FALSE;
		d3dPresentParams.BackBufferWidth  = pixSizeI;
		d3dPresentParams.BackBufferHeight = pixSizeJ;
		d3dPresentParams.BackBufferFormat = d3dColorFormat;
		d3dPresentParams.EnableAutoDepthStencil = TRUE;
		d3dPresentParams.AutoDepthStencilFormat = d3dDepthFormat;
		d3dPresentParams.FullScreen_RefreshRateInHz = uiRefresh;
		d3dPresentParams.PresentationInterval = uiInterval;
	}
	// setup for windowed mode
	else {
		// create dummy Direct3D context
		d3dPresentParams.Windowed = TRUE;
		d3dPresentParams.BackBufferWidth  = 8;
		d3dPresentParams.BackBufferHeight = 8;
		d3dPresentParams.BackBufferFormat = d3dColorFormat;
// Inicio de modificacion de Kang Dong-min.
		/*
		d3dPresentParams.EnableAutoDepthStencil = TRUE;
		d3dDepthFormat = FindDepthFormat_D3D( iAdapter, d3dColorFormat, iZDepth);
		d3dPresentParams.AutoDepthStencilFormat = d3dDepthFormat;
		*/
		// Version original.
		d3dPresentParams.EnableAutoDepthStencil = FALSE;		
		d3dDepthFormat = FindDepthFormat_D3D( iAdapter, d3dColorFormat, iZDepth);		
// Fin de modificacion de Kang Dong-min.
		iZDepth = BitsFromDepthFormat_D3D(d3dDepthFormat);
		gl_iSwapInterval = -1;

		// Disable V-Sync
		d3dPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// determine HW or SW vertex processing
	DWORD dwVP = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	gl_ulFlags &= ~(GLF_D3D_HASHWTNL|GLF_D3D_USINGHWTNL); 
	gl_ctMaxStreams = 16; // software T&L has enough streams
	extern INDEX d3d_bUseHardwareTnL;

	// cannot have HW VP if not supported by HW, right?
	if( d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		gl_ulFlags |= GLF_D3D_HASHWTNL;
		gl_ctMaxStreams = d3dCaps.MaxStreams;
		if( gl_ctMaxStreams<GFX_MINSTREAMS) d3d_bUseHardwareTnL = 0; // cannot use HW T&L if not enough streams
		if( d3d_bUseHardwareTnL) {
			d3d_bUseHardwareTnL = 1; // clamp just in case
			dwVP = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			gl_ulFlags |= GLF_D3D_USINGHWTNL;
		} // no HW T&L 
	} else d3d_bUseHardwareTnL = 0;

	// go for it ...
	extern HWND _hwndMain;
	extern const D3DDEVTYPE d3dDevType;
	hr = gl_pD3D9->CreateDevice( iAdapter, d3dDevType, _hwndMain, dwVP, &d3dPresentParams, &gl_pd3d9Device);
// Inicio de modificacion de Ahn Tae-hoon: quinta beta cerrada (0.2).
	/*
	IUnknown *pD3DDev = NULL;
	if(S_OK == gl_pd3dDevice->QueryInterface(IID_IDirect3DDevice8, (void**)&pD3DDev))
	{
		if(pD3DDev != gl_pd3dDevice)
		{
			ASSERTALWAYS("D3D Device »????? ?®?¦ »?±?.");
			ExitProcess(1);
			return FALSE;
		}
	}
	*/
// Fin de modificacion de Ahn Tae-hoon: quinta beta cerrada (0.2).
	if( hr!=D3D_OK) return FALSE;
	if (!GetDirectX12Backend().AttachD3D9Device(gl_pd3d9Device)) {
		CPrintF("DX12 error: No se pudo conectar IDirect3DDevice9On12.\n");
		gl_pd3d9Device->Release();
		gl_pd3d9Device = NULL;
		return FALSE;
	}
	gl_d3dColorFormat = d3dColorFormat;
	gl_d3dDepthFormat = d3dDepthFormat;
	gl_ctDepthBits = iZDepth & 0x7FFFFFF; // clamp stencil presence flag
// Inicio de modificacion de Kang Dong-min.
	// Version original.
	if( iZDepth & 0x8000000) 
		gl_ulFlags |= GLF_STENCILBUFFER; 
	else 
		gl_ulFlags &= ~GLF_STENCILBUFFER;
	/*
	gl_ulFlags |= GLF_STENCILBUFFER; 
	*/
// Fin de modificacion de Kang Dong-min.

	// sehan
	d3d_bDeviceChanged = TRUE;
	// sehan end

	// done
	return TRUE;
}
		



// fallback D3D internal format
// (reverts to next format that closely matches requied one)
static D3DFORMAT FallbackFormat_D3D( D3DFORMAT eFormat, BOOL b2ndTry)
{
	switch( eFormat) {
	case D3DFMT_X8R8G8B8: return !b2ndTry ? D3DFMT_A8R8G8B8 : D3DFMT_R5G6B5;
	case D3DFMT_X1R5G5B5: return !b2ndTry ? D3DFMT_R5G6B5   : D3DFMT_A1R5G5B5;
	case D3DFMT_R5G6B5:   return !b2ndTry ? D3DFMT_X1R5G5B5 : D3DFMT_A1R5G5B5;
	case D3DFMT_L8:       return !b2ndTry ? D3DFMT_A8L8     : D3DFMT_X8R8G8B8;
	case D3DFMT_A8:       return !b2ndTry ? D3DFMT_A8L8     : D3DFMT_A8R8G8B8;
	case D3DFMT_A8L8:     return D3DFMT_A8R8G8B8;
	case D3DFMT_A1R5G5B5: return D3DFMT_A4R4G4B4;
	case D3DFMT_A8R8G8B8: return D3DFMT_A4R4G4B4;
	case D3DFMT_DXT1:     return D3DFMT_A1R5G5B5;
	case D3DFMT_DXT3:     return D3DFMT_A4R4G4B4;
	case D3DFMT_DXT5:     return D3DFMT_A4R4G4B4;
	case D3DFMT_A4R4G4B4: // must have this one!
	default: ASSERTALWAYS( "Can't fallback texture format.");
	} // missed!
	return D3DFMT_UNKNOWN;
}


// find closest 
extern D3DFORMAT FindClosestFormat_D3D( D3DFORMAT d3df)
{
	FOREVER {
		if( HasTextureFormat_D3D(d3df)) return d3df;
		D3DFORMAT d3df2 = FallbackFormat_D3D( d3df, FALSE);
		if( HasTextureFormat_D3D(d3df2)) return d3df2;
		d3df = FallbackFormat_D3D( d3df, TRUE);
	}
}



// VERTEX/INDEX BUFFERS SUPPORT THRU STREAMS


// DEBUG helper
static void CheckStreams(void)
{
	return;
	UINT uiRet, ui;
	INDEX iRef, iPass;
	HRESULT hr;
	LPDIRECT3DVERTEXBUFFER9 pVBRet, pVB;
	const LPDIRECT3DDEVICE9 pd3dDev = _pGfx->gl_pd3d9Device;

	// check passes and buffer position
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);

	// check vertex positions
	ASSERT( _ulStreamsMask & (1<<GFX_POSIDX)); // must be in shader!
	hr = pd3dDev->GetStreamSource( GFX_POSIDX, &pVBRet, &uiRet, 0);
	D3D_CHECKERROR(hr);
	ASSERT( pVBRet!=NULL);
	iRef = pVBRet->Release();
	ASSERT( iRef==1 && pVBRet==_pGfx->gl_pd3dVtx && uiRet==GFX_POSSIZE);

	// check normals
	pVB = NULL;
	ui  = GFX_NORSIZE;
	hr  = pd3dDev->GetStreamSource( GFX_NORIDX, &pVBRet, &uiRet, 0);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<GFX_NORIDX)) pVB = _pGfx->gl_pd3dNor;
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0));

	// check weights
	pVB = NULL;
	ui  = GFX_WGHSIZE;
	hr  = pd3dDev->GetStreamSource( GFX_WGHIDX, &pVBRet, &uiRet, 0);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<GFX_WGHIDX)) pVB = _pGfx->gl_pd3dWgh;
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0));

	// check colors
	pVB = NULL;
	ui  = GFX_COLSIZE;
	hr  = pd3dDev->GetStreamSource( GFX_COLIDX, &pVBRet, &uiRet, 0);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<GFX_COLIDX)) {
		iPass = (_iColPass-1) % _pGfx->gl_ctColBuffers;
		pVB = _pGfx->gl_pd3dCol[iPass];
	}
	if( !GFX_bColorArray) pVBRet = pVB; // force OK if disabled!
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0));

	/* check 1st texture coords
	pVB = NULL;
	ui  = _bProjectiveMapping ? GFX_TX4SIZE : GFX_TEXSIZE;
	hr  = pd3dDev->GetStreamSource( TEXIDX, &pVBRet, &uiRet);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<(TEXIDX))) {
		iPass = (_iTexPass-1) % _pGfx->gl_ctTexBuffers;
		pVB = _pGfx->gl_pd3dTex[iPass];
	}
	if( !GFX_bColorArray) pVBRet = pVB; // force OK if disabled!
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0)); */

	// check indices
	LPDIRECT3DINDEXBUFFER9 pIBRet;
	hr = pd3dDev->GetIndices( &pIBRet/*, &uiRet*/);
	D3D_CHECKERROR(hr);
	ASSERT( pIBRet!=NULL);
	iRef = pIBRet->Release();
	ASSERT( iRef==1 && pIBRet==_pGfx->gl_pd3dIdx);

	// check shader
/*	const LPDIRECT3DDEVICE8 pd3d8Dev = _pGfx->gl_pd3dDevice;
	hr = pd3d8Dev->GetVertexShader( &dwVS);
	D3D_CHECKERROR(hr);
	ASSERT( dwVS!=NONE && dwVS==_pGfx->gl_dwVertexShader);*/

#pragma message(">> Check pixel shader")
	/* check shader declaration (SEEMS LIKE THIS SHIT DOESN'T WORK!)
	const INDEX ctMaxDecls = 2*MAXSTREAMS+1;
	INDEX ctDecls = ctMaxDecls;
	DWORD adwDeclRet[ctMaxDecls];
	hr = pd3dDev->GetVertexShaderDeclaration( _pGfx->gl_dwVertexShader, (void*)&adwDeclRet[0], (DWORD*)&ctDecls);
	D3D_CHECKERROR(hr);
	ASSERT( ctDecls>0 && ctDecls<ctMaxDecls);
	INDEX iRet = memcmp( &adwDeclRet[0], &_adwCurrentDecl[0], ctDecls*sizeof(DWORD));
	ASSERT( iRet==0); */
}



// prepare vertex array for D3D
extern inline void *LockVertexArray_D3D( const INDEX ctVertices)
{
	// make sure that we have enough space in vertex buffers
	ASSERT( ctVertices>0 && ctVertices<65536);
	GFX_ctVertices = ClampUp( ctVertices, 65535L);
	if( GFX_ctVertices>_pGfx->gl_ctVertices) SetupVertexArrays_D3D(GFX_ctVertices);
    _bUsingDynamicBuffer = TRUE;  // signal the rendering from dynamic buffer

	// determine lock type
	const BOOL bHWTnL = _pGfx->gl_ulFlags & GLF_D3D_USINGHWTNL;
	if( (_iVtxOffset+GFX_ctVertices)>=_pGfx->gl_ctVertices || !bHWTnL) {
		// reset position and flag (eventually)
		_iVtxOffset = 0; 
		_dwVtxLockFlag = D3DLOCK_DISCARD;
		// signal that to texcoord and color buffers
		for( INDEX i=0; i<GFX_MAXLAYERS; i++) _dwColLockFlags[i] = _dwTexLockFlags[i] = _dwVtxLockFlag;
	}
	// just proceed to next buffer portion
	else _dwVtxLockFlag = D3DLOCK_NOOVERWRITE;

	// keep current lock position
	_iVtxPos = _iVtxOffset;

	// reset array states
	_ulStreamsMask = NONE;
	_bProjectiveMapping = FALSE;
	_iTexPass = _iColPass = 0;
	ASSERT( _iVtxPos>=0 && _iVtxPos<65536);
	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_POSIDX;
	void *pLockedBuffer =
		&_dx12DynamicPositions[_iVtxPos * GFX_POSSIZE];
	_pd3dLockedVtx =
		reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(pLockedBuffer);
	return pLockedBuffer;
}


// unlock array
extern inline void UnlockVertexArray_D3D(void)
{
	ASSERT( _pd3dLockedVtx!=NULL && _bUsingDynamicBuffer);
	_pd3dLockedVtx = NULL;
	// advance to next available lock position
	_iVtxOffset += GFX_ctVertices;
}


// prepare normal array for D3D
extern inline void *LockNormalArray_D3D(void)
{
	ASSERT( _bUsingDynamicBuffer);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos <65536);
	ASSERT( _iTexPass <2 && _iColPass<2);  // normals must be set in 1st pass (completed or not)
	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_NORIDX;
	void *pLockedBuffer =
		&_dx12DynamicNormals[_iVtxPos * GFX_NORSIZE];
	_pd3dLockedNor =
		reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(pLockedBuffer);
	return pLockedBuffer;
}

	
// unlock array
extern inline void UnlockNormalArray_D3D(void)
{
	ASSERT( _pd3dLockedNor!=NULL && _bUsingDynamicBuffer);
 _pd3dLockedNor = NULL;
}

// Inicio de modificacion de Ahn Tae-hoon: mapa normal en espacio tangente (0.1).
extern inline void *LockTangentArray_D3D(void)
{
  ASSERT( _bUsingDynamicBuffer);
  ASSERT( _iTexPass>=0 && _iColPass>=0);
  ASSERT( _iVtxPos >=0 && _iVtxPos <65536);
  ASSERT( _iTexPass <2 && _iColPass<2);  // normals must be set in 1st pass (completed or not)
  // update streams mask and assign buffer
  _ulStreamsMask |= 1<<GFX_TANIDX;
  void *pLockedBuffer =
	  &_dx12DynamicTangents[_iVtxPos * GFX_TANSIZE];
  _pd3dLockedTan =
	  reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(pLockedBuffer);
  return pLockedBuffer;
}

  
// unlock array
extern inline void UnlockTangentArray_D3D(void)
{
  ASSERT( _pd3dLockedTan!=NULL && _bUsingDynamicBuffer);
 _pd3dLockedTan = NULL;
}
// Fin de modificacion de Ahn Tae-hoon: mapa normal en espacio tangente (0.1).


// prepare weight array for D3D
extern inline void *LockWeightArray_D3D(void)
{
	ASSERT( _bUsingDynamicBuffer);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos <65536);
	ASSERT( _iTexPass <2 && _iColPass<2);  // weights must be set in 1st pass (completed or not)
	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_WGHIDX;
	void *pLockedBuffer =
		&_dx12DynamicWeights[_iVtxPos * GFX_WGHSIZE];
	_pd3dLockedWgh =
		reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(pLockedBuffer);
	return pLockedBuffer;
}

	
// unlock array
extern inline void UnlockWeightArray_D3D(void)
{
	ASSERT( _pd3dLockedWgh!=NULL && _bUsingDynamicBuffer);
 _pd3dLockedWgh = NULL;
}



// prepare color array for D3D
extern inline void *LockColorArray_D3D(void)
{
	ASSERT( _bUsingDynamicBuffer);
	INDEX iThisPass = _iColPass;
	DWORD dwLockFlag = _dwColLockFlags[iThisPass];  // assume enough buffers by default
	// restart in case of too many passes
	if( iThisPass>=_pGfx->gl_ctColBuffers) {
		dwLockFlag = D3DLOCK_DISCARD;
		iThisPass %= _pGfx->gl_ctColBuffers;
	}
	// mark
	_dwColLockFlags[iThisPass] = D3DLOCK_NOOVERWRITE;
	ASSERT( iThisPass>=0 && iThisPass<_pGfx->gl_ctColBuffers);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);
	_iColPass++; // advance to next color pass

	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_COLIDX;
	void *pLockedBuffer =
		&_dx12DynamicColors[iThisPass][_iVtxPos * GFX_COLSIZE];
	_pd3dLockedCol =
		reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(pLockedBuffer);
	return pLockedBuffer;
}


// unlock array
extern inline void UnlockColorArray_D3D(void)
{
	ASSERT( _pd3dLockedCol!=NULL && _bUsingDynamicBuffer);
 _pd3dLockedCol = NULL;
}


// prepare texture coordinates array for D3D
extern inline void *LockTexCoordArray_D3D( const BOOL bProjectiveMapping/*=FALSE*/)
{
	ASSERT( _bUsingDynamicBuffer);
	SLONG slStride;
	INDEX ctLockSize, iLockOffset;
	INDEX iThisPass = _iTexPass;
	DWORD dwLockFlag = _dwTexLockFlags[iThisPass];  // assume enough buffers by default
	// restart in case of too many passes
	if( iThisPass>=_pGfx->gl_ctTexBuffers) {
		dwLockFlag = D3DLOCK_DISCARD;
		iThisPass %= _pGfx->gl_ctTexBuffers;
	}
	// mark
	_dwTexLockFlags[iThisPass] = D3DLOCK_NOOVERWRITE;
	ASSERT( iThisPass>=0 && iThisPass<_pGfx->gl_ctTexBuffers);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);
	_iTexPass++; // advance to next texture pass

	// determine stride, lock pos and size
	_bProjectiveMapping = bProjectiveMapping;
	const INDEX iStream = GFX_TEXIDX +GFX_iActiveTexUnit;  // must take into account tex-unit, because of multi-texturing!
	if( !bProjectiveMapping) {
		ctLockSize  = GFX_ctVertices*GFX_TEXSIZE; 
		iLockOffset = _iVtxPos*GFX_TEXSIZE; 
		slStride = GFX_TEXSIZE;
	} else {
		ctLockSize  = GFX_ctVertices*GFX_TX4SIZE; 
		iLockOffset = _iVtxPos*GFX_TX4SIZE; 
		slStride = GFX_TX4SIZE;
	}

	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<iStream;
	void *pLockedBuffer =
		&_dx12DynamicTexCoords[iThisPass][iLockOffset];
	_pd3dLockedTex =
		reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(pLockedBuffer);
	return pLockedBuffer;
}


// unlock array
extern inline void UnlockTexCoordArray_D3D(void)
{
	ASSERT( _pd3dLockedTex!=NULL && _bUsingDynamicBuffer);
 _pd3dLockedTex = NULL;
}


extern void SetupShaderStreams(void)
{
#if _DEBUG && 0
	const BOOL bPosStream = (_ulStreamsMask>>GFX_POSIDX&1);
	const BOOL bColStream = (_ulStreamsMask>>GFX_COLIDX&1);
	const BOOL bNorStream = (_ulStreamsMask>>GFX_NORIDX&1);
	const BOOL bWghStream = (_ulStreamsMask>>GFX_WGHIDX&1);
	const BOOL bTexStream1 = (_ulStreamsMask>>(GFX_TEXIDX+0)&1);
	const BOOL bTexStream2 = (_ulStreamsMask>>(GFX_TEXIDX+1)&1);
	const BOOL bTexStream3 = (_ulStreamsMask>>(GFX_TEXIDX+2)&1);
	const BOOL bTexStream4 = (_ulStreamsMask>>(GFX_TEXIDX+3)&1);

	const BOOL bLastPosStream = (_ulLastStreamsMask>>GFX_POSIDX&1);
	const BOOL bLastColStream = (_ulLastStreamsMask>>GFX_COLIDX&1);
	const BOOL bLastNorStream = (_ulLastStreamsMask>>GFX_NORIDX&1);
	const BOOL bLastWghStream = (_ulLastStreamsMask>>GFX_WGHIDX&1);
	const BOOL bLastTexStream1 = (_ulLastStreamsMask>>(GFX_TEXIDX+0)&1);
	const BOOL bLastTexStream2 = (_ulLastStreamsMask>>(GFX_TEXIDX+1)&1);
	const BOOL bLastTexStream3 = (_ulLastStreamsMask>>(GFX_TEXIDX+2)&1);
	const BOOL bLastTexStream4 = (_ulLastStreamsMask>>(GFX_TEXIDX+3)&1);

	CTString strStreams;
	CTString strLastStreams;
	strStreams.PrintF("%d,%d,%d,%d,%d,%d,%d,%d",bPosStream,bColStream,bNorStream,bWghStream,
										bTexStream1,bTexStream2,bTexStream3,bTexStream4);
	strLastStreams.PrintF("%d,%d,%d,%d,%d,%d,%d,%d",bLastPosStream,bLastColStream,bLastNorStream,bLastWghStream,
										bLastTexStream1,bLastTexStream2,bLastTexStream3,bLastTexStream4);
#endif

	const LPDIRECT3DDEVICE9 pd3dDev = _pGfx->gl_pd3d9Device;
	// eventually (re)construct vertex shader out of streams' bit-mask
	if( _ulLastStreamsMask != _ulStreamsMask)
	{ // reset streams that were used before
		ULONG ulThisMask = _ulStreamsMask;
		ULONG ulLastMask = _ulLastStreamsMask;
		for( INDEX iStream=0; iStream<MAXSTREAMS; iStream++) {
			if( (ulThisMask&1)==0 && (ulLastMask&1)!=0) {
				HRESULT hr = pd3dDev->SetStreamSource( iStream,NULL,0, 0);
				D3D_CHECKERROR(hr);
			} // next stream
			ulThisMask >>= 1;
			ulLastMask >>= 1;
		}
	}
	// Setup fixed-function vertex shader if last vertex shader was programmable
	// or shader streams are different
	const BOOL bSetupFFShader = (_dwLastVertexProgram!=NONE || _ulLastStreamsMask!=_ulStreamsMask) && (!GFX_bUseVertexProgram);
	if(bSetupFFShader) {
		// Setup fixed function vertex shader
		_dwCurrentVS = SetupShader_D3D(_ulStreamsMask);
	}
	_ulLastStreamsMask = _ulStreamsMask;
}

// Reset all stream sources
extern void ClearStreams(void)
{
	// Make sure that morhed buffer was not last buffer that was used !
	const LPDIRECT3DDEVICE9 pd3dDev = _pGfx->gl_pd3d9Device;
	for( INDEX iStream=0; iStream<MAXSTREAMS; iStream++) {
		HRESULT hr = pd3dDev->SetStreamSource( iStream,NULL,0,0);
		D3D_CHECKERROR(hr);
	}
}

// Inicio de modificacion de Ahn Tae-hoon: rendimiento (0.2).
#include <Engine/Base/Statistics_Internal.h>
// Fin de modificacion de Ahn Tae-hoon: rendimiento (0.2).
// prepare and draw arrays
extern void DrawElements_D3D( INDEX ctIndices, const UWORD *puwIndices)
{
	// paranoid & sunburnt (by Skunk Anansie:)
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);
	const LPDIRECT3DDEVICE9 pd3dDev = _pGfx->gl_pd3d9Device;
	HRESULT hr = D3D_OK;
	// at least one triangle must be sent

	ASSERT( ctIndices>=3 && ((ctIndices/3)*3)==ctIndices);
	if( ctIndices<3) return;
	// clamp indices and eventually adjust size of index buffer
	ctIndices = ClampUp( ctIndices, _pGfx->gl_ctMaxPrimitives);
	if( ctIndices>_pGfx->gl_ctIndices) SetupIndexArray_D3D(ctIndices);

	// determine lock position and type
	if( (_iIdxOffset+ctIndices) >= _pGfx->gl_ctIndices) {
		_iIdxOffset = 0;
	}

	// determine range span usage
	BOOL  bSetRange=FALSE;
	INDEX iVtxStart=0, ctVtxUsed=GFX_ctVertices;
	if( !(_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL)) {
		extern INDEX d3d_iVertexRangeTreshold;
		d3d_iVertexRangeTreshold = Clamp( d3d_iVertexRangeTreshold, 0L, 9999L);
		bSetRange = (GFX_ctVertices>d3d_iVertexRangeTreshold);
	}

	// copy indices to index buffer
	UWORD *puwLockedBuffer =
		&_dx12DynamicIndices[_iIdxOffset];

#if (defined __MSVC_INLINE__) && (defined  PLATFORM_32BIT) && (defined ENABLE_X86_ASM)

	__asm {
		cld
		mov     esi,D [puwIndices]
		mov     edi,D [puwLockedBuffer]
		mov     ecx,D [ctIndices]
		shr     ecx,1 // will not be 0, since min indices is 3
		rep     movsd
		test    D [ctIndices],1
		jz      elemRange
		movzx   eax,W [esi]
		mov     W [edi],ax

elemRange:
		// find min/max index (if needed)
		cmp     D [bSetRange],0
		jz      elemEnd

		mov     edi,65536
		mov     edx,0
		mov     esi,D [puwIndices]
		mov     ecx,D [ctIndices]
elemTLoop:
		movzx   eax,W [esi]
		cmp     eax,edi
		cmovl   edi,eax
		cmp     eax,edx
		cmovg   edx,eax
		add     esi,2
		dec     ecx
		jnz     elemTLoop
		sub     edx,edi
		inc     edx
		mov     D [iVtxStart],edi
		mov     D [ctVtxUsed],edx
elemEnd:
	}

#else

	INDEX iMaxIndex = 0;
	INDEX iMinIndex = 65536;
	for( INDEX idx=0; idx<ctIndices; idx++) {
		const INDEX iIndex = puwIndices[idx];
		puwLockedBuffer[idx] = iIndex;
		if( !bSetRange) continue;
		if (iMinIndex>iIndex) iMinIndex = iIndex;
		else if (iMaxIndex<iIndex) iMaxIndex = iIndex;
	}
	// set range?
	if( bSetRange) { 
		iVtxStart = iMinIndex;            
		ctVtxUsed = iMaxIndex-iMinIndex+1;
	}

#endif // ASMOPT

	// no need to do all the vertex-shader and streams mumbo-jumbos if static buffer is used
	if(_bUsingDynamicBuffer) {
		// check whether to use color array or not
		if(GFX_bColorArray) {
			_ulStreamsMask |= (1<<GFX_COLIDX);
		} else {
			_ulStreamsMask &= ~(1<<GFX_COLIDX);
		}
	}

	// Set up shader streams
	SetupShaderStreams();

	if(_bUsingDynamicBuffer)
	{
		// (re)set vertex shader
		ASSERT(_dwCurrentVS!=NONE);
		if( _pGfx->gl_dwVertexShader!=_dwCurrentVS) {
			ASSERT(!GFX_bUseVertexProgram); // must be fixed-function vertex program
			_dwLastVertexProgram = NONE;    // set last programmable vertex shader as none
			ASSERT(_dwCurrentVS!=NONE);
			hr = _pGfx->gl_pd3d9Device->SetVertexShader( _currentVS_Shader );
			hr = _pGfx->gl_pd3d9Device->SetVertexDeclaration(_currentVS_Declaration);
			D3D_CHECKERROR(hr);
			GetDirectX12Backend().TrackLegacy3DVertexShader(
				_currentVS_Shader,
				_currentVS_Declaration);
		 _pGfx->gl_dwVertexShader = _dwCurrentVS;
		}

#ifndef NDEBUG
		// Paranoid Android (by Radiohead:)
		CheckStreams();
#endif
	}

	// if not using pixel program
	if(!GFX_bUsePixelProgram) {
		// eventually clear current pixel program
		gfxSetPixelProgram(NONE);
	}

// Inicio de modificacion de Ahn Tae-hoon: rendimiento (0.2).
	_sfStats.IncrementCounter(CStatForm::SCI_DPCOUNT);
// Fin de modificacion de Ahn Tae-hoon: rendimiento (0.2).
	// Captura la ruta compatible para reproducirla en DirectX 12. El modo
	// de reemplazo omite solamente el envio D3D9 que fue capturado.
	const bool native3DCaptured =
		GetDirectX12Backend().QueueLegacy3DIndexedDraw(
		(const USHORT*)puwIndices,
		(UINT)ctIndices,
		GFX_bUseVertexProgram!=FALSE,
		GFX_bUsePixelProgram!=FALSE,
		GFX_bColorArray!=FALSE,
		_bProjectiveMapping!=FALSE,
		(UINT)_iTexPass);

	// draw indices
	ASSERT( ctVtxUsed>0);
	if (GetDirectX12Backend().ShouldSubmitLegacy3DDraw(
		native3DCaptured))
	{
		hr = pd3dDev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			iVtxStart,
			ctVtxUsed,
			_iIdxOffset,
			ctIndices / 3);
		D3D_CHECKERROR(hr);
	}
	// move to next available lock position
	_iIdxOffset += ctIndices;
}                                                                             





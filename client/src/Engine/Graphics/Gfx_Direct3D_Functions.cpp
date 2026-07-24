/* Copyright (c) 2024 Alexander Pavlov.
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

//#include "Engine/StdH.h"
#include "stdh.h"

#include <Engine/Base/Translation.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/Console.h>

#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ViewPort.h>

#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CTextureData.h>

#ifdef SE1_D3D
#include <D3DX9Shader.h>
#pragma comment(lib, "d3dx9.lib")

#include <Engine/Graphics/Gfx_Direct3D_Functions.h>
#include <Engine/Graphics/Gfx_Direct3D_Types.h>

#include <string>
#include <regex>
#include <unordered_set>
#include <assert.h>

#define ProxyInterface _pGfx->gl_pd3d9Device

// Store Shader Handles so they can be destroyed later to mirror D3D8 behavior 
//std::unordered_set<ULONG64> VertexShaderHandles;
unsigned int VertexShaderAndDeclarationCount = 0;
DWORD _dwCurrentVS = NONE;

IDirect3DVertexDeclaration9 *_dwTexCoord4OffsetVS_Declaration = nullptr;
IDirect3DVertexShader9 *_dwTexCoord4OffsetVS_Shader = nullptr;

IDirect3DVertexShader9* _currentVS_Shader = nullptr;
IDirect3DVertexDeclaration9 * _currentVS_Declaration = nullptr;

//###############################################################
//######################   Functions   ##########################
//###############################################################

void dxCreateFixedVertexShader(DWORD* pHandle, IDirect3DVertexShader9** _vsShader, IDirect3DVertexDeclaration9* _vsDeclaration)
{
	*pHandle = 0;
	VertexShaderInfo* ShaderInfo;
	ShaderInfo = new VertexShaderInfo();
	ShaderInfo->Shader = nullptr;
	ShaderInfo->Declaration = _vsDeclaration;

	// Since 'Shader' is at least 8 byte aligned, we can safely shift it to right and end up not overwriting the top bit
	assert((reinterpret_cast<DWORD>(ShaderInfo) & 1) == 0);
	const DWORD ShaderMagic = reinterpret_cast<DWORD>(ShaderInfo) >> 1;
	*pHandle = ShaderMagic | 0x80000000;
	*_vsShader = ShaderInfo->Shader;
}
HRESULT STDMETHODCALLTYPE /*Direct3DDevice8::*/ dxDeleteVertexShader(DWORD Handle, DWORD dwFlag)
{
	if ((Handle & 0x80000000) == 0)
		return D3DERR_INVALIDCALL;

	//VertexShaderHandles.erase(Handle);

	if (_dwCurrentVS == Handle)
		_pGfx->gl_pd3d9Device->SetVertexShader(nullptr);
#if defined(PLATFORM_32BT)
	const DWORD HandleMagic = Handle << 1;
	VertexShaderInfo *const ShaderInfo = reinterpret_cast<VertexShaderInfo *>(HandleMagic);

	if (ShaderInfo->Shader != nullptr) 
	{
		ShaderInfo->Shader->Release();
		VertexShaderAndDeclarationCount--;
	}
	if (ShaderInfo->Declaration != nullptr)
	{
		ShaderInfo->Declaration->Release();
		VertexShaderAndDeclarationCount--;
	}

	delete ShaderInfo;
#else
	if( dwFlag == 10 ) {
		VertexShaderAndDeclarationCount--;
	} else {
		VertexShaderAndDeclarationCount--;
		VertexShaderAndDeclarationCount--;
	}
#endif
	return D3D_OK;
}

#endif

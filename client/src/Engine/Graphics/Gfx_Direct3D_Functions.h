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

#ifndef SE_INCL_GFX_DIRECT3D_FUNCTION_H
#define SE_INCL_GFX_DIRECT3D_FUNCTION_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#ifdef SE1_D3D
struct VertexShaderInfo
{
	IDirect3DVertexShader9 *Shader;
	IDirect3DVertexDeclaration9 *Declaration;
};

extern DWORD _dwCurrentVS;

extern IDirect3DVertexDeclaration9* _dwTexCoord4OffsetVS_Declaration;
extern IDirect3DVertexShader9* _dwTexCoord4OffsetVS_Shader;

extern IDirect3DVertexShader9* _currentVS_Shader;
extern IDirect3DVertexDeclaration9* _currentVS_Declaration;

extern void dxCreateFixedVertexShader(DWORD* pHandle, IDirect3DVertexShader9** _vsShader, IDirect3DVertexDeclaration9* _vsDeclaration);
extern HRESULT STDMETHODCALLTYPE dxDeleteVertexShader(DWORD Handle, DWORD dwFlag);

#endif
#endif  /* include-once check. */

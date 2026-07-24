#ifndef SE_INCL_VERTEX_H
#define SE_INCL_VERTEX_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Graphics/Color.h>

struct GFXVertex
{
	FLOAT x,y,z;
};


struct GFXNormal
{
	FLOAT nx,ny,nz;
};

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
struct GFXTangent
{
  FLOAT tx,ty,tz;
  FLOAT sb;
};
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

struct GFXTexCoord
{
	union {
		struct { FLOAT u,v; };
		struct { FLOAT s,t; };
	};
};


struct GFXTexCoord4
{
	FLOAT s,t,r,q;
};

struct GFXWeight
{
	UBYTE aubIndices[4]; // max of 4 weight indices per vertex
	UBYTE aubWeights[4]; // max of 4 weight factors per vertex
};


struct GFXColor
{
	union {
		struct { UBYTE r,g,b,a; };
		struct { ULONG abgr;    };  // reverse order - use ByteSwap()!
	};

	GFXColor() {};

/*
 * rcg10052001 This is a REALLY bad idea;
 *  never rely on the memory layout of even a
 *  simple class. It works for MSVC, though,
 *  so we'll keep it.
 */
#if (defined __MSVC_INLINE__) && (defined  PLATFORM_32BIT) && (defined ENABLE_X86_ASM)
  GFXColor( COLOR col) {
    _asm mov   ecx,dword ptr [this]
    _asm mov   eax,dword ptr [col]
    _asm bswap eax
    _asm mov   dword ptr [ecx],eax
  }

  __forceinline void Set( COLOR col) {
    _asm mov   ecx,dword ptr [this]
    _asm mov   eax,dword ptr [col]
    _asm bswap eax
    _asm mov   dword ptr [ecx],eax
  }
#else
  GFXColor( COLOR col) { abgr = ByteSwap(col); }
  __forceinline void Set( COLOR col) { abgr = ByteSwap(col); }
#endif

	void MultiplyRGBA( const GFXColor &col1, const GFXColor &col2) {
		r = (ULONG(col1.r)*col2.r)>>8;
		g = (ULONG(col1.g)*col2.g)>>8;
		b = (ULONG(col1.b)*col2.b)>>8;
		a = (ULONG(col1.a)*col2.a)>>8;
	}

	void MultiplyRGB( const GFXColor &col1, const GFXColor &col2) {
		r = (ULONG(col1.r)*col2.r)>>8;
		g = (ULONG(col1.g)*col2.g)>>8;
		b = (ULONG(col1.b)*col2.b)>>8;
	}

	void MultiplyRGBCopyA1( const GFXColor &col1, const GFXColor &col2) {
		r = (ULONG(col1.r)*col2.r)>>8;
		g = (ULONG(col1.g)*col2.g)>>8;
		b = (ULONG(col1.b)*col2.b)>>8;
		a = col1.a;
	}

	void AttenuateRGB( ULONG ulA) {
		r = (ULONG(r)*ulA)>>8;
		g = (ULONG(g)*ulA)>>8;
		b = (ULONG(b)*ulA)>>8;
	}

	void AttenuateA( ULONG ulA) {
		a = (ULONG(a)*ulA)>>8;
	}
};

#endif  /* include-once check. */


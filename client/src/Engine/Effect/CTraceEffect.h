//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CTRACEEFFECT_H__
#define __CTRACEEFFECT_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable :4786)
#include <list>
#include <Engine/Base/FileName.h>
#include <Engine/Graphics/Vertex.h>
#include <vector>
#include <Engine/Graphics/Texture.h>

#include "CEffect.h"
#include "CRefCountPtr.h"
#include "FreeMemStack.h"

class CTag;

class ENGINE_API CTraceEffect : public CEffect
{
public:
	typedef std::vector< UWORD >		vector_index;
	typedef std::vector< GFXVertex >	vector_pos;
	typedef std::vector< GFXColor >		vector_color;
	typedef std::vector< GFXTexCoord >	vector_texcoord;
	typedef std::vector< FLOAT >		vector_time;
public:
	CTraceEffect();
	virtual ~CTraceEffect();

	//NEW_DELETE_DEFINITION(CTraceEffect);

//안태훈 수정 시작	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	BOOL SetTexture(const CTFileName &filename);
	const CTFileName &GetTexFileName()	{ return m_strTextureFileName;	}

	//acess function
	inline void SetTraceTime(FLOAT time)	{ m_fTraceTime = time; }
	inline FLOAT GetTraceTime()				{ return m_fTraceTime; }

	inline void SetTimeInterval(FLOAT time)	{ m_fTimeInterval = time; }
	inline FLOAT GetTimeInterval()			{ return m_fTimeInterval; }

	inline void SetTwinklePeriod(FLOAT twinklePeriod)	{ m_fTwinklePeriod = twinklePeriod;	}
	inline FLOAT GetTwinklePeriod()						{ return m_fTwinklePeriod;			}

	inline void SetColor(COLOR colBegin, COLOR colEnd)	{ m_colTraceBegin = colBegin; m_colTraceEnd = colEnd; }
	inline COLOR GetColorBegin()						{ return m_colTraceBegin;			}
	inline COLOR GetColorEnd()							{ return m_colTraceEnd;				}

	inline void SetCapEnd(BOOL bCapEnd)					{ m_bCapEnd = bCapEnd;				}
	inline BOOL GetCapEnd()								{ return m_bCapEnd;					}

	inline void SetBlendType(PredefinedBlendType blendType)	{ m_eBlendType = blendType; }
	inline PredefinedBlendType GetBlendType()				{ return m_eBlendType;		}

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

	static void InitializeShaders();
	static void FinalizeShaders();
protected:
	CTraceEffect( CTraceEffect &other ) {}
	CTraceEffect &operator =(CTraceEffect &other) {return *this;}

	inline void RemakeColorsAndTexCoords(INDEX cntVtx);
	
protected:
	//content variable
	CTFileName			m_strTextureFileName;	//texture의 파일 이름
	COLOR				m_colTraceBegin;		//trace가 시작되는 부분의 색깔
	COLOR				m_colTraceEnd;			//trace가 끝나는 부분의 색깔
	FLOAT				m_fTraceTime;			//trace되는 시간
	FLOAT				m_fTimeInterval;		//trace되는 간격, 시작부분은 항상 됨.
	FLOAT				m_fTwinklePeriod;		//전체의 알파를 일정간격으로 조정.
	BOOL				m_bCapEnd;				//끝부분을 봉합한다.
	PredefinedBlendType	m_eBlendType;
	//instance variable
	static ULONG		m_ulVertexProgramNoTex;
	static LPDIRECT3DPIXELSHADER9		m_ulPixelProgramNoTex;
	static ULONG		m_ulVertexProgramTex;
	static LPDIRECT3DPIXELSHADER9		m_ulPixelProgramTex;
	INDEX				m_iTraceCount;			//trace되는 시간과 간격을 고려한 값
	CTextureData		*m_ptdTexture;			//실제 로딩된 texture
	vector_index		m_vectorIndex;			//그려질 tri의 index들
	vector_pos			m_vectorPos;			//그려질 vertex의 pos
	vector_color		m_vectorColor;			//그려질 vertex의 color
	vector_texcoord		m_vectorTexCoord;		//그려질 vertex의 texure 좌표, 단 texture가 세팅된 경우만 존재.
	vector_time			m_vectorAddTime;		//vertex가 추가된 시간을 표시한다.
	FLOAT				m_fTwinkleValue;		//반짝반짝값, 매프레임마다 계산됨.
	INDEX				m_iTagCount;
};

#endif //__CTRACEEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

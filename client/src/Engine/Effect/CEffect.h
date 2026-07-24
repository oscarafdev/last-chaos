//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CEFFECT_H__
#define __CEFFECT_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Console.h>
#pragma warning(disable :4786)
#include <string>
#include <list>
#include <vector>
#include <algorithm>

#include <Engine/Entities/Entity.h>
#include "CRefCountPtr.h"
#include "FreeMemStack.h"

class CTag;

enum EFF_RENDER_TYPE
{
	ER_NORMAL = 0,	// CRenderer::RenderEffects() 에서 그려지는 Effect
	ER_IN_UI,		// UI 내부에서 그려질 Effect -- HUD_DrawSkaModel()
	ER_COUNT,
};

enum EFF_RENDER_SUB_TYPE
{
	ERS_NORMAL		= 0,
	ERS_CASHSHOP,
	ERS_MATRYOSHKA,
	ERS_MAKETITLE,
};
enum EFFECT_TYPE
{
	ET_NOTHING			= 0,
	ET_TERRAIN,
	ET_LIGHT,
	ET_PARTICLE,
	ET_SKA,
	ET_MDL,
	ET_TRACE,
	ET_SOUND,
	ET_SPLINEBILLBOARD,
	ET_ORBIT,
	ET_SHOCKWAVE,
	ET_SPLINEPATH,
	ET_CAMERA,
	ET_ENTITY,
	ET_COUNT,
};

//effect의 play 상태 정보
enum EFFECT_STATE
{
	ES_NOT_STARTED	= 0,	//아직 시작하지 않은 상태, 초기 상태임.
	ES_PLAYING		= 1,	//플레이 중, 한번만 play함.
	ES_PLAY_END		= 3,	//플레이가 끝난 상태.
};

enum EFFECT_OF_TAG_TYPE
{
	EOTT_NONE	= 0,
	EOTT_START	= 1,
	EOTT_ALWAYS	= 2,
};

class ENGINE_API CEffect
{
public:
	typedef std::vector<CTag *> my_vector;
public:
	CEffect();
	virtual ~CEffect() = 0;

//안태훈 수정 시작	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile) = 0;
	virtual void Write(CTStream *ostrFile) = 0;
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE) = 0;
	virtual BOOL Process(FLOAT time) = 0;
	virtual CEffect *Copy() = 0;
	virtual void Render() {};
	virtual void Stop(FLOAT leftTime);

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

	virtual INDEX GetWantTagCount()						{ return 0;		}
	virtual const char *GetWantTagName(INDEX index)		{ return "";	}
	virtual void SetWantTag(INDEX index, ptr_tag tag)	{ NULL;			}

	void AttachToTag(const ptr_tag &ptrTag)	{ if(ptrTag.NotNull()) m_ptrAttachTag = ptrTag; }
	void DetachTag()					{ m_ptrAttachTag.SetNull();		}
	ptr_tag GetTag()					{ return m_ptrAttachTag;		}

	//access time
	inline BOOL Playing()				{ return (ES_PLAYING == m_eState);	}
	inline EFFECT_STATE GetState()		{ return m_eState;					}
	inline EFFECT_TYPE GetType()		{ return m_eType;					}
	inline FLOAT GetLastProcessedTime()	{ return m_fLastProcessTime;		}
	inline void SetLastProcessedTime(FLOAT time)	{ m_fLastProcessTime = time; }

	inline FLOAT GetLeftTime()			{ return m_fLeftTime; }

	//Fade를 쓰지 않는 경우. fade 타임도 Setting
	inline void SetLifeTime(FLOAT time) { m_fLifeTime = time; m_fFadeInTime = 0; m_fFadeOutTime = 0; }
	inline FLOAT GetLifeTime()			{ return m_fLifeTime; }

	//Fade를 쓰는 경우. Life Time도 Setting.
	inline void SetFadeTime(FLOAT fadeInTime, FLOAT noFadeTime, FLOAT fadeOutTime)
	{
		m_fLifeTime = fadeInTime + noFadeTime + fadeOutTime;
		m_fFadeInTime = fadeInTime;
		m_fFadeOutTime = fadeOutTime;
	}
	inline FLOAT GetFadeInTime()					{ return m_fFadeInTime; }
	inline FLOAT GetFadeOutTime()					{ return m_fFadeOutTime; }

	//바운딩구 관련
	inline void SetBoundingSphereRadius(FLOAT radius)	{ m_fBoundingSphereRadius = radius;	}
	inline FLOAT GetBoundingSphereRadius()				{ return m_fBoundingSphereRadius;	}

	//반복회수 설정, Fade와 같이 쓸 경우 glittering(반짝임)도 가능.
	inline SLONG GetRepeatCount()					{ return m_slRepeat;	}
	inline void SetRepeatCount(SLONG cnt)			{ m_slRepeat = cnt;		}
	
	inline void SetName(const std::string &name)	{ m_strName = name;		}
	inline void SetName(const char *name)			{ m_strName = name;		}
	inline const std::string &GetName() const		{ return m_strName;		}

	inline BOOL IsNotRenderAtThisFrame()			{ return m_bNotRenderAtThisFrame; }
	virtual void SetNotRenderAtThisFrame();

	//life등에 이 값이 곱해진다.
	inline void SetSpeedMul(FLOAT fSpeedMul)	{ m_fSpeedMul = fSpeedMul;	}
	inline FLOAT GetSpeedMul()					{ return m_fSpeedMul;		}

	//Entity
	inline void SetOwner(CEntity *pen)			{ m_penOwner = pen;			}
	inline CEntity *GetOwner()					{ return m_penOwner.ep_pen;	}

	inline void SetERType(EFF_RENDER_TYPE erType) { m_ERType = erType; }
	inline EFF_RENDER_TYPE GetERType(void)		{ return m_ERType; }

	inline void SetERSubType(EFF_RENDER_SUB_TYPE ersType )		{ m_ERSubType = ersType; }
	inline EFF_RENDER_SUB_TYPE GetERSubType(void)				{ return m_ERSubType; }

	// 이펙트가 컬링 되더라도 이펙트 타임은 업데이트 되어야 한다.
	BOOL UpdateETime(FLOAT tm);

protected:
	CEffect( CEffect &other ) {}
	CEffect &operator =(CEffect &other) {return *this;}

	//자식들에서 공통적으로 쓰이는 부분들.
	BOOL PreStart(FLOAT time, BOOL bRestart);		//Start의 구현에서 앞부분에 쓰임.
	void PostStart() { m_eState = ES_PLAYING; } //Start의 구현에서 뒷부분에 쓰임.
	
	BOOL PreProcess(FLOAT time, BOOL &bRetValue, BOOL &bRenderInThisFrame, FLOAT &retDeltaTime, FLOAT &retProcessedTime);	//Process의 구현에서 앞부분에 쓰임
	void PostProcess() {}	//Process의 구현에서 뒷부분에 쓰임
	
	BOOL PreRender();
	void PostRender() {}

	void SetContent(const CEffect *pEffect);	//CEffect의 Content를 Setting함.

	inline FLOAT GetFadeValue(FLOAT fProcessedTime)
	{
		//fade in, [0, fade-in time)
		if(fProcessedTime <= m_fFadeInTime) {
			if(m_fFadeInTime > 0.0f || m_fFadeInTime < 0.0f)
			{
				return (fProcessedTime / m_fFadeInTime);
			}

			return 0.0f;
		}
		//fade out, [fade-in time, life time - fade-out time)
		else if(m_fLeftTime <= m_fFadeOutTime) {
			if(m_fFadeOutTime > 0.0f || m_fFadeOutTime < 0.0f)			
			{
				return (m_fLeftTime / m_fFadeOutTime);
			}

			return 0.0f;
		}
		//no fade, [life time - fade-out time, life time)
		else if(fProcessedTime <= m_fLifeTime) return 1.0f;

		return 0.0f;
	}
protected:
	//content variable
	EFFECT_TYPE		m_eType;				//타입, 상속받은 객체를 나타냄.
	std::string		m_strName;				//이름. 등록되는 이름을 나타냄
	FLOAT			m_fLifeTime;			//생명의 길이, 항상 양이어야 함.
	SLONG			m_slRepeat;				//몇 번 반복할것인가를 나타냄. life time * 횟수 만큼의 시간동안 effect 유지, -1이면 무한반복
	FLOAT			m_fFadeInTime;			//Fade-in되는 시간
	FLOAT			m_fFadeOutTime;			//Fade-Out되는 시간
	FLOAT			m_fBoundingSphereRadius;//Bounding-Sphere의 반지름.
	//instance variable
	EFFECT_STATE	m_eState;				//현재의 플레이 상태
	FLOAT			m_fLeftTime;			//남은 플레이 시간
	FLOAT			m_fStartTime;			//시작되었다면 그 시작시간
	FLOAT			m_fLastProcessTime;		//마지막으로 Process가 호출된 시간
	ptr_tag			m_ptrAttachTag;			//붙어있는 Tag
	BOOL			m_bNotRenderAtThisFrame;//참이면 한 프레임동안 Rendering하지 않음.
	FLOAT			m_fSpeedMul;			//수명등에 곱해짐.
	CEntityPointer	m_penOwner;			//이 Effect가 붙여지는 Entity, Null도 상관없음.

	EFF_RENDER_TYPE m_ERType;				// 이펙트가 그려지는 공간 별로 타입 지정
	EFF_RENDER_SUB_TYPE m_ERSubType;
};

#endif //__CEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
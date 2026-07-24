#ifndef		MY_INFO_H_
#define		MY_INFO_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <Engine/Info/MyInfoSkill.h>
#include <Engine/Entities/TargetInfo.h>
#include <Engine/Network/PetTarget.h>
#include <Engine/Network/SlaveTarget.h>

struct stGuildMark
{
	SBYTE	gm_row;
	SBYTE	gm_col;
	SBYTE	bg_row;
	SBYTE	bg_col;

	void setData(SBYTE row, SBYTE col, SBYTE brow, SBYTE bcol) {
		gm_row = row;		gm_col = col;
		bg_row = brow;		bg_col = bcol;
	}

	void remove() {
		gm_row = -1;		gm_col = -1;
		bg_row = -1;		bg_col = -1;
	}
};


class ENGINE_API MyInfo : public MyInfoSkill,
	public CSingletonBase< MyInfo >
{
public:
	MyInfo();

	void	clear();

	void	SetBuyAddition(int nValue)	{ nBuy_addition = nValue;	}
	void	SetBuyRate(int nRate)		{ nBuy_rate = nRate;		}
	void	SetSellAddition(int nValue) { nSell_addition = nValue;	}
	void	SetSellRate(int nRate)		{ nSell_rate = nRate;		}

	int		GetBuyAddition() { return nBuy_addition;	}
	int		GetBuyRate()	 { return nBuy_rate;		}
	int		GetSellAddition(){ return nSell_addition;	}
	int		GetSellRate()	 { return nSell_rate;		}

	__int64 CalcPriceRate(__int64 iShopPrice, BOOL bBuy);
	__int64 CalcPriceAddition(BOOL bBuy);

	void	add_makeitem(INDEX nmakeitem);
	bool	find_makeitem(INDEX nmakeitem);

	stGuildMark		_guildmark;
private:
	// B1408 길드원 스킬 1 에누리권, 2 타고난 장사꾼
	int				nBuy_addition; // 더해지는 값
	int				nBuy_rate; // %
	int				nSell_addition;
	int				nSell_rate;

	std::vector<INDEX>		m_vctMakeLearn;
};

#define		MY_INFO()	MyInfo::getSingleton()

enum eTARGET_TYPE
{
	eTARGET = 0,
	eTARGET_REAL,
	eTARGET_ALL
};

class ENGINE_API ObjInfo : public CSingletonBase< ObjInfo >
{
public:
	ObjInfo();
	
	void clear();

	// Target Info 전용 ////////////////////////
	void SetTargetInfo(CTargetInfom Info);
	void SetTargetHealth(float fCur, float fMax);
	void SetTargetPKMode(int pkMode, int pkState, int Legit, int pkTitle = 0);
	void SetTargetPKHide(BOOL bPkState);
	void SetTargetSyndicate(int nType, int nGrade);

	int GetTargetSyndiType()	{ return m_TargetInfo.nSyndicateType; }
	int GetTargetSyndiGrade()	{ return m_TargetInfo.nSyndicateGrade; }
	BOOL IsTargetPKHide()		{ return m_TargetInfo.bPkHideState; }
	///////////////////////////////////////////////

	// Target Real Info 전용
	void SetTargetRealInfo(CTargetInfomReal Info);
	
	SQUAD GetTargetCount()			{ return m_TargetInfoReal.llCount; }
	LONG GetTargetGuildIdx()		{ return m_TargetInfoReal.lGuildIndex; }
	CTString GetTargetGuildName()	{ return m_TargetInfoReal.strGuildName; }
	SBYTE GetTargetGuildRank()		{ return m_TargetInfoReal.sbGuildRank; }
	UBYTE GetTargetGuildColor()		{ return m_TargetInfoReal.ubGuildNameColor; }
	INDEX GetTargetNickIdx()		{ return m_TargetInfoReal.iNickIndex; }

	// 공통
	void SetTargetActive(eTARGET_TYPE eType, BOOL bActive);

	CEntity* GetTargetEntity(eTARGET_TYPE eType);
	int GetTargetServerIdx(eTARGET_TYPE eType);
	int GetTargetDBIdx(eTARGET_TYPE eType);
	int GetTargetLevel(eTARGET_TYPE eType);
	int GetTargetPKState(eTARGET_TYPE eType);
	int GetTargetPKMode(eTARGET_TYPE eType);
	float GetTargetCurHP(eTARGET_TYPE eType);
	float GetTargetMaxHP(eTARGET_TYPE eType);
	SBYTE GetTargetType(eTARGET_TYPE eType);
	char* GetTargetName(eTARGET_TYPE eType);
	BOOL IsTargetActive(eTARGET_TYPE eType);
	void TargetClear(eTARGET_TYPE eType = eTARGET_ALL);
	
	// My Obj Info //////////////////

	// APET
	CWildPetTarget* GetMyApetInfo()			{ return m_MyApetInfo; }
	void SetMyApet(CWildPetTarget* pApet)	{ m_MyApetInfo = pApet; }

	// PET
	CPetTargetInfom* GetMyPetInfo()			{ return &m_MyPetInfo; }
	
	// SLAVE ( element )
	CSlaveTargetInfom* GetMySlaveInfo(int nIndex)	{ return &m_MySlaveInfo[nIndex]; }

	// Relic Effect
	int	GetShowAnotherRelicEffect();
private:
	CTargetInfom		m_TargetInfo;
	CTargetInfomReal	m_TargetInfoReal;
	CWildPetTarget*		m_MyApetInfo;				//내가 착용하고 있는 공격형 펫
	CPetTargetInfom		m_MyPetInfo;
	CSlaveTargetInfom	m_MySlaveInfo[2];
};

#define		INFO()			ObjInfo::getSingleton()
#define		MY_APET_INFO()	ObjInfo::getSingleton()->GetMyApetInfo()
#define		MY_PET_INFO()	ObjInfo::getSingleton()->GetMyPetInfo()
#define		MY_SLAVE_INFO(n)	ObjInfo::getSingleton()->GetMySlaveInfo(n)

#endif		// MY_INFO_H_
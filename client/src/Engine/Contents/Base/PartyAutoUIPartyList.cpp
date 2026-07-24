#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "CommunityDefines.h"
#include <Engine/Interface/UIPartyAuto.h>
#include "PartyAutoUIPartyList.h"
#include <Engine/Help/Util_Help.h>

class CmdPAPartyBtnCallback : public Command
{
public:
	CmdPAPartyBtnCallback() : _ui(NULL), nCmd(0) {}
	void setData(CUIPartyAutoPartyList* pUI, int cmd_idx) {
		_ui = pUI;	nCmd = cmd_idx;
	}

	void execute() {
		if (_ui != NULL)
			_ui->SendPartyReq(nCmd);
	}

private:
	CUIPartyAutoPartyList*		_ui;
	int nCmd;
};

// -------------------------------------------------------------------------


CUIPartyAutoPartyList::CUIPartyAutoPartyList()
	: m_pList(NULL)
	, m_pImgLevel(NULL)
	, m_nCurrentPageNum(0)
	, m_pBtnPrev(NULL)
	, m_pBtnNext(NULL)
	, m_pCbClass(NULL)
	, m_pChkLimit(NULL)
	, m_pCbType(NULL)
	, m_nCharIndex(-1)
{
	setInherit(false);

	int		i;

	for (i = 0; i < TOTAL_JOB; ++i)
		m_pImgClass[i] = NULL;


	for (i = 0; i < PT_MAX; ++i)
		m_pImgType[i] = NULL;
}

void CUIPartyAutoPartyList::initialize()
{
	int		i;

	CUIText* pTxt = (CUIText*)findUI("txt_title");

	if (pTxt != NULL)
	{
		int l, t, r, b;
		l = pTxt->GetPosX();
		t = pTxt->GetPosY();
		r = l + pTxt->GetWidth();
		b = t + pTxt->GetHeight();
		setTitleRect(l, t, r, b);
	}

	m_pList = (CUIList*)findUI("list_invite");

	if (m_pList != NULL)
	{
		m_pList->SetSelectedForeRender(true);
		m_pList->DeleteAllListItem();
	}

	{
		// class 등록
		const char* arr_class_name[] = {
			"img_titan",
			"img_knight",
			"img_healer",
			"img_mage",
			"img_rogue",
			"img_sorcerer",
			"img_nightshadow",
			"img_exrogue",
			"img_archmage",
		};

		for (i = 0; i < TOTAL_JOB; ++i)
			m_pImgClass[i] = (CUIImage*)findUI(arr_class_name[i]);
	}

	{
		const char* arr_paryy_type[] = {
			"img_type_peaceever",
			"img_type_survival",
			"img_type_attack"
		};

		for (i = 0; i < PT_MAX; ++i)
			m_pImgType[i] = (CUIImage*)findUI(arr_paryy_type[i]);
	}

	m_pBtnPrev = (CUIButton*)findUI("btn_prev");

	if (m_pBtnPrev != NULL)
	{
		CmdPAPartyBtnCallback* pCmd = new CmdPAPartyBtnCallback;
		pCmd->setData(this, -1);
		m_pBtnPrev->SetCommandUp(pCmd);
	}

	m_pBtnNext = (CUIButton*)findUI("btn_next");

	if (m_pBtnNext != NULL)
	{
		CmdPAPartyBtnCallback* pCmd = new CmdPAPartyBtnCallback;
		pCmd->setData(this, 1);
		m_pBtnNext->SetCommandUp(pCmd);
	}

	m_pImgLevel = (CUIImage*)findUI("img_leveldiff");

	m_pCbClass = (CUIComboBox*)findUI("cb_class");
	m_pChkLimit = (CUICheckButton*)findUI("chk_limitlevel");
	m_pCbType = (CUIComboBox*)findUI("cb_type");

	// ---------------------------------------------------------------------
#ifndef		WORLD_EDITOR
	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoPartyList::close, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoPartyList::close, this));

	pBtn = (CUIButton*)findUI("btn_search");

	if (pBtn != NULL)
	{
		CmdPAPartyBtnCallback* pCmd = new CmdPAPartyBtnCallback;
		pCmd->setData(this, 0);
		pBtn->SetCommandUp(pCmd);
	}

	pBtn = (CUIButton*)findUI("btn_join");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoPartyList::_join, this));
#endif	// WORLD_EDITOR

	init_data();
}

void CUIPartyAutoPartyList::open()
{
	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_PARTYAUTO_PARTY_LIST, TRUE);

	_pNetwork->SendPartyListReq(m_nCurrentPageNum, -1, 0, -1, TRUE);
}

void CUIPartyAutoPartyList::close()
{
	SetVisible(FALSE);
	Hide(TRUE);

	{
		m_nCurrentPageNum = 0;
		m_vectorPartyListData.clear();
	}

	CUIBase* pUI = CUIFocus::getSingleton()->getUI();

	if (pUI == m_pCbClass || pUI == m_pCbType)
		CUIFocus::getSingleton()->setUI(NULL);
}

void CUIPartyAutoPartyList::ReceivePartyData( CNetworkMessage* istr )
{
    int		i, j;

    SLONG	slCnt;
    SLONG	slPageNum;

    (*istr) >> slPageNum;
    (*istr) >> slCnt;

    if (slPageNum < 0 || slCnt == 0)	// 잘못된 페이지 or 목록 없음
        return;

	// 기존 아이템 삭제
	if (m_pList != NULL)
		m_pList->DeleteAllListItem();

    m_nCurrentPageNum = slPageNum;
    m_vectorPartyListData.clear();

    for (i = 0; i < slCnt; ++i)
    {
        SLONG		slIndex, slZone, slJobFlag;
        SBYTE		sbLvdiff, sbPtType, sbLimitLv;
        CTString	strName;
        SLONG		slPartyNum = 2;

        PartyListData	TempData;

        (*istr) >> slIndex;		// 파티장 식별자(n)
        (*istr) >> sbLvdiff;	// 레벨 제한(c)
        (*istr) >> slZone;		// 위치(n)
        (*istr) >> slPartyNum;	// 파티 매칭 인원
        (*istr) >> slJobFlag;	// 직업(n)
        (*istr) >> sbPtType;	// 파티 종류(c)
        (*istr) >> sbLimitLv;	// 제한 레벨( 쓰지 않음 )(c)
        (*istr) >> strName;		// 파티 이름(str)

        TempData.nIndex = (int)slIndex;
        TempData.nLeveldiff = (int)sbLvdiff;
        TempData.nZone = (int)slZone;
        TempData.nPartyNum = (int)slPartyNum;
        TempData.dwJobflag = (DWORD)slJobFlag;
        TempData.nPartyType = (int)sbPtType;
        TempData.nLimitLv = (int)sbLimitLv;
        TempData.strSubject = strName;

        m_vectorPartyListData.push_back(TempData);
    }

	// update list
	if (m_pList != NULL)
	{
		int max = m_vectorPartyListData.size();

		CUIListItem* pItem;
		CTString	strTmp;
		CUIText*	pTxt;
		CUIImage*	pImg;

		for (i = 0; i < max; ++i)
		{
			pItem = (CUIListItem*)m_pList->GetListItemTemplate()->Clone();

			m_pList->AddListItem(pItem);

			int no = ((m_nCurrentPageNum * 10) + i + 1);

			pTxt = (CUIText*)pItem->findUI("txt_no");

			if (pTxt != NULL)
			{
				strTmp.PrintF("%d", no);
				pTxt->SetText(strTmp);
			}

			pTxt = (CUIText*)pItem->findUI("txt_name");

			if (pTxt != NULL)
			{
				pTxt->SetText(m_vectorPartyListData[i].strSubject);

				if (m_vectorPartyListData[i].nPartyNum >= 8)
				{
					pTxt->setFontColor(0xA7A9A5FF);
				}
				else
				{
					pTxt->setFontColor(DEF_UI_COLOR_WHITE);
				}
			}

			pTxt = (CUIText*)pItem->findUI("txt_num");

			if (pTxt != NULL)
			{
				int nPartyNum = m_vectorPartyListData[i].nPartyNum;

				CTString strPartyNum;
				strPartyNum.PrintF("%d/8",nPartyNum);

				pTxt->SetText(strPartyNum);

				if (nPartyNum >= 8)
					pTxt->setFontColor(0xA7A9A5FF);
				else
					pTxt->setFontColor(DEF_UI_COLOR_WHITE);
			}

			pTxt = (CUIText*)pItem->findUI("txt_pos");

			if (pTxt != NULL)
			{
				strTmp = CZoneInfo::getSingleton()->GetZoneName(m_vectorPartyListData[i].nZone);
				pTxt->SetText(strTmp);
			}

			if (m_vectorPartyListData[i].nLeveldiff == 0)
			{
				pImg = (CUIImage*)pItem->findUI("img_level");

				if (pImg != NULL)
				{
					if (m_pImgLevel != NULL)
						m_pImgLevel->Copy(pImg);
				}
			}

			CUIBase* pBase = (CUIBase*)pItem->findUI("base_class");

			if (pBase != NULL)
			{
				// 중앙 정렬을 위해 Count 검사
				int	nJobCnt = 0;
				DWORD dwClass = m_vectorPartyListData[i].dwJobflag;

				if( dwClass & JF_TITAN ) nJobCnt++;
				if( dwClass & JF_KNIGHT ) nJobCnt++;
				if( dwClass & JF_HEALER ) nJobCnt++;
				if( dwClass & JF_MAGE ) nJobCnt++;
				if( dwClass & JF_ROGUE ) nJobCnt++;
				if( dwClass & JF_SORCERER ) nJobCnt++;
				if( dwClass & JF_NIGHTSHADOW ) nJobCnt++;
#ifdef CHAR_EX_ROGUE
				if( dwClass & JF_EX_ROGUE ) nJobCnt++;
#endif
#ifdef CHAR_EX_MAGE
				if( dwClass & JF_EX_MAGE ) nJobCnt++;
#endif

				if (nJobCnt > 0)
				{
					int spos = 0;

					spos = (TOTAL_JOB - nJobCnt) * 9;

					for (j = 0; j < TOTAL_JOB; ++j)
					{
						if (dwClass & (1 << j))
						{
							CUIImage* pClone = (CUIImage*)m_pImgClass[j]->Clone();
							pClone->SetPosX(spos);
							pClone->Hide(FALSE);

							pBase->addChild(pClone);

							spos += 18;
						}					
					}
				}
			}

			pImg = (CUIImage*)pItem->findUI("img_type");

			if (pImg != NULL)
			{
				int type = m_vectorPartyListData[i].nPartyType;
				if (m_pImgType[type] != NULL)
					m_pImgType[type]->Copy(pImg);
			}
		}

		m_pList->UpdateList();
	}
}

void CUIPartyAutoPartyList::ReceivePartyJoinMessage( int nErrorcode, CNetworkMessage *istr )
{
	CTString strMessage;

	close();

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_OK:
		{
			SBYTE sbPartyType, sbCharJob;
			SLONG slBoss, slChar;
			CTString strBossName, strCharName;

			(*istr) >> sbPartyType;
			(*istr) >> slBoss;
			(*istr) >> strBossName;
			(*istr) >> slChar;
			(*istr) >> strCharName;
			(*istr) >> sbCharJob;

			PartyMatchJoin( sbPartyType, slBoss, strBossName, slChar, strCharName, sbCharJob );
			return;
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET:
		{
			strMessage = _S( 2709, "이미 다른 파티에 가입중입니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_NOT_FOUND:
		{
			strMessage = _S( 2710, "매칭 목록에 없습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_LEVEL:
		{
			strMessage = _S( 2711, "파티에 참가하기에는 레벨이 맞지 않습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL:
		{
			strMessage = _S( 2712, "레벨 제한 조건을 만족하지 못합니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_JOB:
		{
			strMessage = _S( 2713, "직업이 조건에 만족하지 않습니다" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_FULL:
		{
			strMessage = _S( 2700, "파티 인원이 가득 차있습니다." );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST:
		{
			strMessage = _S( 2715, "초대 요청자가 있어서 참가신청을 할 수 없습니다" );
		}
		break;
	}

	_pNetwork->ClientSystemMessage( strMessage );
}

void CUIPartyAutoPartyList::SendPartyReq(int nNum)
{
	int nSendPage;
	int nJob = 0;
	int nPtType = 0;
	int nLvLimit = 0;

	if (m_pCbClass != NULL)
		nJob = m_pCbClass->GetCurSel();

	if (m_pCbType != NULL)
	{
		nPtType = m_pCbType->GetCurSel();

		// 입수우선이 없으르로 중간에 +1을 함
		if (nPtType == 2)
			nPtType++;
	}

	if (m_pChkLimit != NULL)
		nLvLimit = m_pChkLimit->IsChecked();

	if (nNum != 0)
	{
		nSendPage = m_nCurrentPageNum + nNum;
	}
	else if (nNum == 0)
	{
		nSendPage = 0;
		m_vectorPartyListData.clear();

		if (m_pList != NULL)
			m_pList->DeleteAllListItem();

	}

	// -1 페이지는 존재하지 않는다.
	if (nSendPage < 0)
		return;

	_pNetwork->SendPartyListReq(nSendPage, nJob - 1, nLvLimit, nPtType - 1, TRUE);
}

void CUIPartyAutoPartyList::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_PARTYAUTO_JOIN_ALLOW:
		{
			if (m_nCharIndex < 0)
				return;

			if( bOK )
			{
				_pNetwork->SendPartyJoinAllow(m_nCharIndex);
			}
			else
			{
				_pNetwork->SendPartyJoinReject(m_nCharIndex);
			}

			m_nCharIndex = -1;
		}
		break;
	}
}

// -------------------------------------------------------------------------

void CUIPartyAutoPartyList::init_data()
{
	int		i;

	CTString strAll = _S(506, "전체");

	if (m_pCbClass != NULL)
	{
		m_pCbClass->AddString(strAll);

		for (i = 0; i < TOTAL_JOB; ++i)
			m_pCbClass->AddString(UTIL_HELP()->GetClassName((eJob)i));
	}

	if (m_pCbType != NULL)
	{
		m_pCbType->AddString(strAll);
		m_pCbType->AddString(_S(3354, "일반"));
		m_pCbType->AddString(_S(2665, "전투형"));
	}
}

void CUIPartyAutoPartyList::_join()
{
	if (m_pList != NULL)
	{
		int nSel = m_pList->getCurSel();

		if (nSel < 0 || nSel >= m_vectorPartyListData.size())
		{
			CTString strMessage = _S(2694, "참가할 파티를 선택하지 않았습니다");
			_pNetwork->ClientSystemMessage(strMessage);
		}
		else
		{
			if (m_vectorPartyListData.empty() == false)
				_pNetwork->SendPartyJoinReq(m_vectorPartyListData[nSel].nIndex);
		}
	}
}

void CUIPartyAutoPartyList::PartyMatchJoin( SBYTE sbType, SLONG slBoss, CTString strBossName, 
											SLONG slChar, CTString strCharName, SBYTE sbCharJob )
{
	CTString strMessage;
	CTString strJob;

	strJob = UTIL_HELP()->GetClassName((eJob)sbCharJob);
	m_nCharIndex = (int)slChar;

	CUIManager* pUIManager = CUIManager::getSingleton();

	SLONG slMyIndex = _pNetwork->MyCharacterInfo.index;

	if (slBoss == slMyIndex)
	{
	    strMessage.PrintF(_S(2716, "%s님(%s)이 파티 가입 신청을 하셨습니다. 허락하시겠습니까?"), strCharName, strJob);

	    CUIMsgBox_Info	MsgBoxInfo;
	    MsgBoxInfo.SetMsgBoxInfo(_S(215, "파티"), UMBS_YESNO, UI_PARTYAUTO_PARTY_LIST, MSGCMD_PARTYAUTO_JOIN_ALLOW);
	    MsgBoxInfo.AddString(strMessage);
	    pUIManager->CreateMessageBox(MsgBoxInfo);
	}
	else
	{
	    strMessage.PrintF(_S(2717, "파티장 %s에게 파티참가를 요청 중입니다"), strBossName);

	    CUIMsgBox_Info	MsgBoxInfo;
	    MsgBoxInfo.SetMsgBoxInfo(_S(215, "파티"), UMBS_CANCEL, UI_PARTYAUTO_PARTY_LIST, MSGCMD_PARTYAUTO_JOIN_ALLOW);
	    MsgBoxInfo.AddString(strMessage);
	    pUIManager->CreateMessageBox(MsgBoxInfo);
	}
}
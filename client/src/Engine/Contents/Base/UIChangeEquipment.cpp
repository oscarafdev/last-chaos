#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/UIChangeEquipment.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/ChangeWeapon.h>

#define DEF_LIST_INFO_COUNT 5
#define DEF_GOLD_TOKEN_IDX 5124

enum eERROR_MSG
{
	eERROR_NOT_TRADE_ITEM = 0,
	eERROR_NO_TRADE_ITEM,
	eERROR_NO_SELECT_TRADE_ITEM,
	eERROR_SEALED_ITEM,
	eERROR_ONLY_WEAPON,
	eERROR_ONLY_ARMOR,
	eERROR_WEARING_EQUIPMENT,
	eERROR_USE_SKILL,
	eERROR_USE_TELEPORT,
	eERROR_GOLDTOKEN_NOT_ENOUGH,
	eERROR_FULL_INVENTORY_WEAPON,
	eERROR_FULL_INVENTORY_ARMOR,
};

//--------------------------------------------------------------
// 창 닫기
class CmdChangeEquipClose : public Command
{
public:
	CmdChangeEquipClose() : pUI_(NULL) {}
	void setData(CUIChangeEquipment* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIChangeEquipment* pUI_;
};

//--------------------------------------------------------------
// 확인 클릭
class CmdChangeEquipOk : public Command
{
public:
	CmdChangeEquipOk() : pUI_(NULL) {}
	void setData(CUIChangeEquipment* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressOK();
	}
private:
	CUIChangeEquipment* pUI_;
};

//--------------------------------------------------------------
// 리스트 클릭
class CmdChangeEquipClick : public Command
{
public:
	CmdChangeEquipClick() : pUI_(NULL) {}
	void setData(CUIChangeEquipment* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ClickListItem();
	}
private:
	CUIChangeEquipment* pUI_;
};

CUIChangeEquipment::CUIChangeEquipment()
	: m_ptbContent(NULL)
	, m_psbList(NULL)
	, m_bDrag(false)
	, m_nGoldTokenCount(0)
	, m_nOriX(0), m_nOriY(0)
{
	m_eGuideState = eGUIDE_REGISTER_TRADE_ITEM;
	m_eChangeEquipState	= CHANGEEQUIP_WEAPON;
	int i;	
	for (i = 0; i < eBUTTON_MAX; i++)
	{
		m_pbtn[i] = NULL;
	}

	for (i = 0; i < eTEXT_MAX; i++)
	{
		m_pText[i] = NULL;
	}

	for (i = 0; i < eLIST_MAX; i++)
	{
		m_plist[i] = NULL;
	}

	for (i = 0; i < eICON_MAX; i++)
	{
		m_pIcon[i] = NULL;
	}

	setInherit(false);
}

void CUIChangeEquipment::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIChangeEquipment::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIChangeEquipment::PressOK()
{
	SendChangeEquipReq();
}

void CUIChangeEquipment::ClickListItem()
{
	refreshListInfo();
}

void CUIChangeEquipment::ShowSystemMsg( int nErrorCode )
{
	CTString strMessage;
	
	switch (nErrorCode)
	{
	case eERROR_NO_TRADE_ITEM:
		strMessage.PrintF( _S( 1076, "교체할 아이템이 없습니다." ) );
		break;
	case eERROR_NO_SELECT_TRADE_ITEM:
		strMessage.PrintF( _S( 2321,"새로운 무기및 방어구를 선택해주십시요." ) );
		break;
	case eERROR_SEALED_ITEM:
		strMessage.PrintF( _S( 1079, "봉인된 아이템은 교체할 수 없습니다." ) );
		break;
	case eERROR_ONLY_WEAPON:
		strMessage.PrintF( _S( 1072, "무기만 교체 할 수 있습니다.") );
		break;
	case eERROR_ONLY_ARMOR:
		strMessage.PrintF( _S( 2320, "방어구만 교체 할 수 있습니다." ) );
		break;
	case eERROR_WEARING_EQUIPMENT:
		strMessage.PrintF( _S( 1071, "착용된 아이템은 교체할 수 없습니다." ) );
		break;
	case eERROR_USE_SKILL:
		strMessage.PrintF( _S( 1074, "스킬 사용중에는 아이템을 교체할 수 없습니다." ) );
		break;
	case eERROR_USE_TELEPORT:
		strMessage.PrintF( _S( 1075, "순간 이동중에는 아이템를 교체할 수 없습니다." ) );
		break;
	case eERROR_NOT_TRADE_ITEM:
		strMessage.PrintF( _S( 7027, "이곳에서 교체 할 수 없는 아이템 입니다.") );
		break;
	case eERROR_GOLDTOKEN_NOT_ENOUGH:
		strMessage.PrintF( _S( 7028, "골드 토큰이 부족하여 교체를 할 수 없습니다.") );
		break;
	case eERROR_FULL_INVENTORY_WEAPON:
		strMessage.PrintF( _S( 1329, "무기를 교체 할 수 없습니다.") );
		break;
	case eERROR_FULL_INVENTORY_ARMOR:
		strMessage.PrintF( _S( 7029, "방어구를 교체 할 수 없습니다.") );
		break;
	default:
		return;
	}

	_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
}

void CUIChangeEquipment::initialize()
{
	m_pIcon[eICON_ITEM] = (CUIIcon*)findUI("icon_item");
	m_pIcon[eICON_SUB] = (CUIIcon*)findUI("icon_sub");
	m_psbList = (CUIScrollBar*)findUI("list_scroll");
	m_ptbContent = (CUITextBox*)findUI("tb_content");

	CTString strBtn[eBUTTON_MAX] = { "btn_close", "btn_ok", "btn_no" };
	CTString strText[eTEXT_MAX] = { "move_title", "str_title", "str_listType" };
	CTString strList[eLIST_MAX] = { "list_item", "list_itemInfo" };
	int i;	
	for (i = 0; i < eBUTTON_MAX; i++)
	{
		m_pbtn[i] = (CUIButton*)findUI(strBtn[i]);
	}

	for (i = 0; i < eTEXT_MAX; i++)
	{
		m_pText[i] = (CUIText*)findUI(strText[i]);
	}

	for (i = 0; i < eLIST_MAX; i++)
	{
		m_plist[i] = (CUIList*)findUI(strList[i]);
	}

	// command 셋팅
	if (m_pbtn[eBUTTON_CLOSE] != NULL)
	{
		CmdChangeEquipClose* pCmd = new CmdChangeEquipClose;
		pCmd->setData(this);
		m_pbtn[eBUTTON_CLOSE]->SetCommandUp(pCmd);
	}

	if (m_pbtn[eBUTTON_NO] != NULL)
	{
		CmdChangeEquipClose* pCmd = new CmdChangeEquipClose;
		pCmd->setData(this);
		m_pbtn[eBUTTON_NO]->SetCommandUp(pCmd);
	}

	if (m_pbtn[eBUTTON_OK] != NULL)
	{
		CmdChangeEquipOk* pCmd = new CmdChangeEquipOk;
		pCmd->setData(this);
		m_pbtn[eBUTTON_OK]->SetCommandUp(pCmd);
	}
}

void CUIChangeEquipment::open( eChangeEquipState eType, float x, float z )
{
	m_eChangeEquipState = eType;
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );

	m_fNpcX = x;
	m_fNpcZ = z;
	SetVisible(TRUE);
	Hide(FALSE);

	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (pData != NULL)
		pData->Clear();

	updateGuide(eGUIDE_REGISTER_TRADE_ITEM);
	UpdateUI();

	pUIManager->RearrangeOrder( UI_CHANGE_EQUIPMENT, TRUE );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_CHANGEWEAPON );
}

void CUIChangeEquipment::close()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	CUITooltipMgr::getSingleton()->setData(NULL);
	SetVisible(FALSE);
	Hide(TRUE);
	
	pUIManager->RearrangeOrder( UI_CHANGE_EQUIPMENT, FALSE );
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_CHANGEWEAPON );
}

void CUIChangeEquipment::UpdateUI()
{	
	if (refreshString() == false)
		close();

	if (refreshListItem() == false)
		close();

	refreshListInfo();

	clearIcon(eICON_ITEM);
	clearIcon(eICON_SUB);

	m_nGoldTokenCount = 0;
}

void CUIChangeEquipment::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (IsVisible() == FALSE)
		return;

	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{
		close();
	}
}


WMSG_RESULT CUIChangeEquipment::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

WMSG_RESULT CUIChangeEquipment::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pText[eTEXT_MOVE] && m_pText[eTEXT_MOVE]->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_CHANGE_EQUIPMENT, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIChangeEquipment::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_SUB] == NULL)
	{
		CUIManager::getSingleton()->ResetHoldBtn();
		return WMSG_FAIL;
	}

	bool bError = false;

	CUIIcon* pDrag = pUIManager->GetDragIcon();
	bool bHoldBtnEmpty = (pDrag == NULL);
	// 골드 토큰 
	if (m_pIcon[eICON_SUB]->IsInside(x, y) == TRUE && bHoldBtnEmpty == false)
	{
		if (pDrag->getIndex() != DEF_GOLD_TOKEN_IDX)
			bError = true;

		if (m_pIcon[eICON_ITEM]->IsEmpty() != false)
			bError = true;

		if (bError == true)
		{
			CUIManager::getSingleton()->ResetHoldBtn();
			return WMSG_FAIL;
		}

		CItems* pItems = pDrag->getItems();

		if (pItems != NULL)
		{
			CItemData*	pItemData = pItems->ItemData;

			if (pItemData != NULL)
			{
				pData->SetGoldTokenVirIndex(pItems->Item_UniIndex);
				m_pIcon[eICON_SUB]->setData(pItems);
				updateGuide(eGUIDE_ALL_READY);
			}		
		}		
	}
	// 아이템 검사
	else if (m_pIcon[eICON_ITEM]->IsInside(x, y) == TRUE && bHoldBtnEmpty == false)
	{
		// Error check
		if( pDrag->IsWearTab() == true)
		{
			ShowSystemMsg(eERROR_WEARING_EQUIPMENT);
			bError = true;
		}

		CItems* pItems = pDrag->getItems();		

		if (pItems == NULL)
			return WMSG_FAIL;

		CItemData*	pItemData = pItems->ItemData;

		if (pItemData == NULL)
			return WMSG_FAIL;

		if (pItems->Item_Flag & FLAG_ITEM_SEALED || pItems->Item_Flag & FLAG_ITEM_COMPOSITION)
		{
			ShowSystemMsg(eERROR_SEALED_ITEM);
			bError = true;
		}

		if (bError == true)
		{
			CUIManager::getSingleton()->ResetHoldBtn();
			return WMSG_FAIL;
		}

		{
			if(pItemData->IsFlag(ITEM_FLAG_RARE) == false)
			{
				ShowSystemMsg(eERROR_NOT_TRADE_ITEM);
				CUIManager::getSingleton()->ResetHoldBtn();
				return WMSG_FAIL;
			}

			switch(m_eChangeEquipState)
			{
			case CHANGEEQUIP_WEAPON:
				{
					if (pItemData->GetType() != CItemData::ITEM_WEAPON)
					{
						ShowSystemMsg(eERROR_ONLY_WEAPON);
						bError = true;
					}
				}
				break;
			case CHANGEEQUIP_ARMOR:
				{
					if (pItemData->GetType() != CItemData::ITEM_SHIELD)
					{
						ShowSystemMsg(eERROR_ONLY_ARMOR);
						bError = true;
					}
				}
				break;
			}

			if (checkValidItem(pItemData->getindex()) == true && bError == false)
			{
				m_nGoldTokenCount = getConsumeTokenCnt(pItemData->getindex());

				if (m_nGoldTokenCount <= 0)
				{
					ShowSystemMsg(eERROR_NOT_TRADE_ITEM);
					CUIManager::getSingleton()->ResetHoldBtn();
					return WMSG_FAIL;
				}
				
				if (pData != NULL)
				{
					pData->SetInvenTab(pItems->Item_Tab);
					pData->SetInvenIdx(pItems->InvenIndex);
					pData->SetVirtualIndex(pItems->Item_UniIndex);
					pData->SetGoldTokenCount(m_nGoldTokenCount);
				}
				m_pIcon[eICON_ITEM]->setData(pItems);
				clearIcon(eICON_SUB);
				updateGuide(eGUIDE_TRADE_FOR_NEED_ITEM, m_nGoldTokenCount);
				refreshListInfo();
			}
		}
	}

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

bool CUIChangeEquipment::refreshString()
{
	CTString strString[eTEXT_MAX];

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			strString[eTEXT_TITLE].PrintF( _S(1049, "무기 교체"));
			strString[eTEXT_LISTNAME].PrintF( _S(1059, "무기 리스트"));
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			strString[eTEXT_TITLE].PrintF( _S(2313, "방어구 교체"));
			strString[eTEXT_LISTNAME].PrintF( _S(2316, "클래스 리스트"));
		}
		break;
	default:
		return false;
	}

	for (int i = 1; i < eTEXT_MAX; i++)
	{
		if (m_pText[i] == NULL)
		{
			LOG_DEBUG("CUIChangeEquipment::refreshString() ERROR %d String Null ptr", i);
			return false;
		}

		m_pText[i]->SetText(strString[i]);
	}

	return true;
}

bool CUIChangeEquipment::refreshListItem()
{
	if (m_plist[eLIST_ITEM] == NULL)
		return false;

	m_plist[eLIST_ITEM]->DeleteAllListItem();
	m_plist[eLIST_ITEM]->setCurSel(-1);

	if (m_psbList != NULL)
		m_psbList->SetScrollPos(0);

	int i, nMax = 0;
	CUIListItem* pTempItem = m_plist[eLIST_ITEM]->GetListItemTemplate();
	CUIListItem* pItem = NULL;

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			nMax = DEF_TRADE_WEAPON_MAX - 1; // (총갯수 - 교환가능 탭)
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			nMax = TOTAL_JOB - 1; // (나이트 쉐도우 빼줌)
		}
		break;
	default:
		return false;
	}

	int nListString[CHANGEEQUIP_MAX][DEF_TRADE_WEAPON_MAX - 1] = { \
	{1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 2306, 2307}, \
	{43, 44, 45, 46, 47, 48, 5732, 5820, 0, 0, 0, 0}		\
	};

	CTString strTemp;

	for (i = 0; i < nMax; i++)
	{
		m_plist[eLIST_ITEM]->AddListItem(pTempItem->Clone());
		pItem = (CUIListItem*)m_plist[eLIST_ITEM]->GetListItem( i );

		if (pItem == NULL)
			continue;

		CUIText* pText = (CUIText*)pItem->findUI("str_itemType");

		if (pText != NULL)
		{
			strTemp.PrintF(_S( nListString[m_eChangeEquipState][i], "리스트 스트링"));
			pText->SetText(strTemp);
		}

		CmdChangeEquipClick* pCmd = new CmdChangeEquipClick;
		pCmd->setData(this);
		pItem->SetCommandUp(pCmd);	
	}

	m_plist[eLIST_ITEM]->UpdateList();
	m_plist[eLIST_ITEM]->UpdateScroll(nMax);
	return true;
}

bool CUIChangeEquipment::refreshListInfo()
{
	if (m_plist[eLIST_INFO] == NULL)
		return false;
	// 인포 초기화
	m_plist[eLIST_INFO]->DeleteAllListItem();

	// 인포 셋팅 검사
	if (m_pIcon[eICON_ITEM] == NULL)
		return false;

	if (m_pIcon[eICON_ITEM]->IsEmpty() != false)
		return false;

	if (m_plist[eLIST_ITEM] == NULL)
		return false;

	// 교환할 아이템 인덱스 얻기
	int nSelectItemIdx = m_pIcon[eICON_ITEM]->getIndex();
	int nSelectListIdx = m_plist[eLIST_ITEM]->getCurSel();

	if (nSelectListIdx < 0)
		return false;

	// 인포 셋팅
	int i;
	int nTradeItemIdx = 0;
	CUIListItem* pTempItem = m_plist[eLIST_INFO]->GetListItemTemplate();
	CUIListItem* pItem = NULL;

	// 교활될 아이템 정보 얻기
	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			nTradeItemIdx = CChangeEquipment::getTradeWeaponIndex(nSelectItemIdx, nSelectListIdx);
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			nTradeItemIdx = CChangeEquipment::getTradeArmorIndex(nSelectItemIdx, nSelectListIdx);
		}
		break;
	default:
		return false;
	}

	if (nTradeItemIdx <= 0)
		return false;

	if (GAMEDATAMGR()->GetChangeWeapon() != NULL)
		GAMEDATAMGR()->GetChangeWeapon()->SetTradeItemIndex(nTradeItemIdx);

	// 인포 타이틀 스트링 인덱스
	int nListString[CHANGEEQUIP_MAX][DEF_LIST_INFO_COUNT] = { \
	{1065, 1066, 1067, 1068, 1069}, \
	{2317, 2318, 1067, 87, 0}		\
	};

	// String XML ID
	CTString strTitleID[DEF_LIST_INFO_COUNT] = {"str_nameTitle", "str_levelTitle", "str_classTitle", \
		"str_point1Title", "str_point2Title" };
	// String XML ID
	CTString strValueID[DEF_LIST_INFO_COUNT] = {"str_name", "str_level", "str_class", "str_point1", "str_point2" };

	CTString strTemp;
	m_plist[eLIST_INFO]->AddListItem(pTempItem->Clone());
	pItem = (CUIListItem*)m_plist[eLIST_INFO]->GetListItem(0);

	if (pItem == NULL)
	{
		m_plist[eLIST_INFO]->DeleteAllListItem();
		return false;
	}

	CUIText* pTextString = NULL;
	CUIText* pTextValue = NULL;

	// 타이틀 셋팅
	for (i = 0; i < DEF_LIST_INFO_COUNT; i++)
	{
		pTextString = (CUIText*)pItem->findUI(strTitleID[i]);

		if (pTextString != NULL)
		{
			strTemp.PrintF(_S( nListString[m_eChangeEquipState][i], "타이틀 스트링"));
			pTextString->SetText(strTemp);
		}
	}

	
	pTextString = (CUIText*)pItem->findUI(strTitleID[4]);

	if (pTextString != NULL)
	{
		if (m_eChangeEquipState == CHANGEEQUIP_ARMOR)
			pTextString->Hide(TRUE);
		else
			pTextString->Hide(FALSE);
	}

	// 아이템 정보 셋팅
	CTString strValue[DEF_LIST_INFO_COUNT];

	CItemData* pItemData = CItemData::getData(nTradeItemIdx);

	if (pItemData == NULL)
	{
		m_plist[eLIST_INFO]->DeleteAllListItem();
		return false;
	}	

	// 아이템 이름
	strValue[0] = pItemData->GetName();
	// 아이템 레벨
	strValue[1].PrintF("%d", pItemData->GetLevel());
	// 아이템 사용 클래스
	CUIManager::getSingleton()->GetClassOfItem(pItemData, strValue[2]);

	int nPoint1 = 0, nPoint2 = 0;
	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			nPoint1 = pItemData->GetPhysicalAttack();
			nPoint2 = pItemData->GetMagicAttack();
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			nPoint1 = pItemData->GetPhysicalDefence();
		}
		break;
	}
	// 공격력, 방어력
	strValue[3].PrintF("%d", nPoint1);
	// 마법 공격력, 없음
	strValue[4].PrintF("%d", nPoint2);

	for (i = 0; i < DEF_LIST_INFO_COUNT; i++)
	{
		pTextValue = (CUIText*)pItem->findUI(strValueID[i]);

		if (pTextValue != NULL)
		{
			pTextValue->SetText(strValue[i]);
		}
	}

	pTextValue = (CUIText*)pItem->findUI(strValueID[4]);

	if (pTextValue != NULL)
	{
		if (m_eChangeEquipState == CHANGEEQUIP_ARMOR)
			pTextValue->Hide(TRUE);
		else
			pTextValue->Hide(FALSE);
	}

	m_plist[eLIST_INFO]->UpdateList();
	m_plist[eLIST_INFO]->UpdateScroll(1);
	return true;
}

bool CUIChangeEquipment::checkValidItem( int nItemIdx )
{
	bool bValid = false;

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		bValid = CChangeEquipment::IsValidWeapon(nItemIdx);
		break;
	case CHANGEEQUIP_ARMOR:
		bValid = CChangeEquipment::IsValidArmor(nItemIdx);
		break;
	}

	if (bValid == false)
	{
		ShowSystemMsg(eERROR_NOT_TRADE_ITEM);
		return false;
	}

	return true;
}

void CUIChangeEquipment::updateGuide( eGUIDE_STATE eType, int nCount )
{
	m_eGuideState = eType;

	if (m_ptbContent == NULL)
		return;

	int nGuideString[eGUIDE_MAX] = { 7030, 7031, 7032, };
	CTString strTemp;

	switch(m_eGuideState)
	{
	case eGUIDE_REGISTER_TRADE_ITEM:
		strTemp.PrintF(_S(7030, "교체 하고 싶은 아이템을 등록 해 주세요."));
		break;
	case eGUIDE_TRADE_FOR_NEED_ITEM:
		strTemp.PrintF(_S(7031, "등록 한 아이템을 교체 하기 위해서는 골드 토큰 %d개가 필요 합니다."), nCount);
		break;
	case eGUIDE_ALL_READY:
		strTemp.PrintF(_S(7032, "교체 할 아이템을 선택 후 확인 버튼을 클릭 하면 교체가 완료 됩니다."));
		break;
	default:
		return;
	}

	m_ptbContent->SetText(strTemp);
}

int CUIChangeEquipment::getSelectIdx( int nSelIdx )
{
	int ret = -1;

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		ret = nWeaponSelType[nSelIdx];
		break;
	case CHANGEEQUIP_ARMOR:
		ret = nClassSelType[nSelIdx];
		break;
	}
	return ret;
}

int CUIChangeEquipment::getConsumeTokenCnt( int nItemIdx )
{
	CItemData* pItemData = CItemData::getData(nItemIdx);

	if (pItemData == NULL)
		return 0;

	int nlevel = pItemData->GetLevel();
	int nCnt = 0;

	if (nlevel >= 70 && nlevel <= 100)
		nCnt = 1;
	else if (nlevel > 100 && nlevel <= 130)
		nCnt = 2;
	else if (nlevel > 130 && nlevel <= 140)
		nCnt = 3;
	else if (nlevel > 140 && nlevel <= 150)
		nCnt = 4;
	else if (nlevel > 150 && nlevel <= 175)
		nCnt = 5;
	else
		nCnt = 0;

	return nCnt;
}

void CUIChangeEquipment::clearIcon( int nType )
{
	if (nType < eICON_ITEM || nType >= eICON_MAX)
		return;

	int nStringIdx[eICON_MAX] = { 7033, 7034, };
	CTString strToolTip;

	if (m_pIcon[nType] != NULL)
	{
		m_pIcon[nType]->clearIconData();
		strToolTip.PrintF(_S(nStringIdx[nType], "슬롯 툴팁"));
		m_pIcon[nType]->setTooltip(strToolTip);
	}
}

void CUIChangeEquipment::SendChangeEquipReq()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_pIcon[eICON_ITEM] == NULL)
	{
		ShowSystemMsg(eERROR_NO_TRADE_ITEM);
		return;
	}

	if (m_pIcon[eICON_ITEM]->IsEmpty() != false)
	{
		ShowSystemMsg(eERROR_NO_TRADE_ITEM);
		return;
	}

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		ShowSystemMsg(eERROR_USE_SKILL);
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		ShowSystemMsg(eERROR_USE_TELEPORT);
		return;
	}

	if (m_plist[eLIST_ITEM] == NULL)
	{
		ShowSystemMsg(eERROR_NO_SELECT_TRADE_ITEM);
		return;
	}

	int nSelIdx = m_plist[eLIST_ITEM]->getCurSel();

	if( nSelIdx < 0 )
	{
		ShowSystemMsg(eERROR_NO_SELECT_TRADE_ITEM);
		return;
	}

	if (m_pIcon[eICON_SUB] == NULL)
	{
		ShowSystemMsg(eERROR_GOLDTOKEN_NOT_ENOUGH);
		return;
	}

	if (m_pIcon[eICON_SUB]->IsEmpty() != false)
	{
		ShowSystemMsg(eERROR_GOLDTOKEN_NOT_ENOUGH);
		return;
	}

	int nCount = pUIManager->GetInventory()->GetItemCount(DEF_GOLD_TOKEN_IDX);
	if ( m_nGoldTokenCount > nCount )
	{
		ShowSystemMsg(eERROR_GOLDTOKEN_NOT_ENOUGH);
		return;
	}

	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (pData == NULL)
		return;

	pData->SetSelectIdx(getSelectIdx(nSelIdx));

	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );

	// Create message box of remission
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2308, "무기및 방어구 교체" ), UMBS_OKCANCEL,UI_CHANGE_EQUIPMENT, MSGCMD_CHANGEWEAPON_REP );
	MsgBoxInfo.AddString( _S(6290, "은밀한 무기 및 방어구 교체 시 제련 된 기록과 장착 된 보석은 옮겨지지만 아이템 옵션은 미공개 상태로 변경 됩니다." ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUIChangeEquipment::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_CHANGEWEAPON_REP:		// 무기 교체 확인.
		{
			if( bOK )
			{
				ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

				if (pData != NULL)
					pData->SendChangeEquipment(FALSE);
			}
		}
		break;
	}
}

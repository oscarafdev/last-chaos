#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include "SessionState.h"
#include <Common/Packet/ptype_old_extend.h>
#include <Common/Packet/ptype_cef_ui.h>
#include <Engine/Network/Web.h>
#include <Common/Packet/ptype_old_do_monstercombo.h>
#include <Common/Packet/ptype_old_do_title.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Login/CharacterSelect.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Contents/function/attendance.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Contents/function/TitleData.h>


namespace
{
std::string LegacyTextToUtf8(const char* value)
{
	if (value == NULL || value[0] == '\0')
		return "";

	const int wideLength = MultiByteToWideChar(CP_ACP, 0, value, -1, NULL, 0);
	if (wideLength <= 0)
		return value;

	std::vector<wchar_t> wideValue(wideLength);
	MultiByteToWideChar(CP_ACP, 0, value, -1, &wideValue[0], wideLength);

	const int utf8Length = WideCharToMultiByte(
		CP_UTF8, 0, &wideValue[0], -1, NULL, 0, NULL, NULL);
	if (utf8Length <= 1)
		return "";

	std::vector<char> utf8Value(utf8Length);
	WideCharToMultiByte(
		CP_UTF8, 0, &wideValue[0], -1, &utf8Value[0], utf8Length, NULL, NULL);
	return std::string(&utf8Value[0], utf8Length - 1);
}

std::string EncodeCefUiParameter(const char* value)
{
	static const char hex[] = "0123456789ABCDEF";
	const std::string utf8Value = LegacyTextToUtf8(value);
	std::string encoded;
	const unsigned char* cursor =
		reinterpret_cast<const unsigned char*>(utf8Value.c_str());

	while (*cursor != '\0')
	{
		const unsigned char current = *cursor++;
		if ((current >= 'a' && current <= 'z') ||
			(current >= 'A' && current <= 'Z') ||
			(current >= '0' && current <= '9') ||
			current == '-' || current == '_' || current == '.' || current == '~')
		{
			encoded += static_cast<char>(current);
		}
		else
		{
			encoded += '%';
			encoded += hex[current >> 4];
			encoded += hex[current & 0x0F];
		}
	}

	return encoded;
}
}

bool CSessionState::RecvExtend(CNetworkMessage* istr)
{
	bool bRet = true;
	pTypeThirdBase* pBase = reinterpret_cast<pTypeThirdBase*>(istr->GetBuffer());

	switch (pBase->subType)
	{
	case MSG_EX_RESTART:
		recvExRestart(istr);
		break;
	
	case MSG_EX_MONSTERCOMBO:
		recvExMonsterCombo(istr);
		break;

	case MSG_EX_ATTENDANCE_EXP_SYSTEM:
		ReceiveAttendanceMessage(istr);
		break;
	case MSG_EX_CEF_UI:
		ReceiveCefUiMessage(istr);
		break;
	case MSG_EX_NOTICE:
		ReceiveNewsMessage(istr);
		break;
	case MSG_EX_TITLE_SYSTEM:
		ReceiveNickNameMessage(istr);
		break;
	default:
		bRet = false;
		break;
	}

	return bRet;
}


void CSessionState::ReceiveCefUiMessage(CNetworkMessage* istr)
{
	const pTypeCefUi* packet =
		reinterpret_cast<const pTypeCefUi*>(istr->GetBuffer());

	extern cWeb g_web;
	if (packet->thirdType == MSG_EX_CEF_UI_CLOSE)
	{
		g_web.SendWebPageOpenMsg(FALSE);
		return;
	}

	if (packet->thirdType != MSG_EX_CEF_UI_OPEN ||
		strcmp(packet->route, "test") != 0)
		return;

	const INDEX safeWidth = Clamp(packet->width, 320, 1600);
	const INDEX safeHeight = Clamp(packet->height, 240, 1000);
	std::string url = "lcui://test?player=";
	url += EncodeCefUiParameter(packet->playerName);
	url += "&parameters=";
	url += EncodeCefUiParameter(packet->parameters);

	g_web.SetWebPosition(safeWidth, safeHeight);
	g_web.SendWebPageOpenMsg(TRUE);
	g_web.UpdatePos();
	g_web.SetWebUrl(url);
}

void CSessionState::recvExRestart( CNetworkMessage* istr )
{
	_pNetwork->SetDelivery(true);
	_pNetwork->SendReceiveRestartGame();

	ResponseClient::exRestart* pRecv = reinterpret_cast<ResponseClient::exRestart*>(istr->GetBuffer());

	extern BOOL _bLoginProcess;
	_bLoginProcess = TRUE;
	_pNetwork->bMoveCharacterSelectUI = TRUE;
	CLoginJobInfo::getSingleton()->LoginModelCreate();

	if (pRecv->thirdType == 1)
	{
		GameDataManager* pGame = GameDataManager::getSingleton();

		if (pGame != NULL)
		{
			pGame->GetCharSelData()->SetHardCoreMsgType(1); // 0 일반 접속, 1 캐릭터가 죽은 뒤 캐릭터 선택창으로 보내짐
		}
	}
}

void CSessionState::recvExMonsterCombo( CNetworkMessage* istr )
{
	pTypeThirdBase* pBase = reinterpret_cast<pTypeThirdBase*>(istr->GetBuffer());
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(pBase->thirdType)
	{
	case  MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP:
		{
			ResponseClient::MCEditContext* pPack = reinterpret_cast<ResponseClient::MCEditContext*>(istr->GetBuffer());

			int* pList = NULL;

			if (pPack->count > 0)
			{
				pList = new int[pPack->count];
				memcpy(pList, &pPack->list[0], sizeof(int) * pPack->count);
			}				

			for(int i = 0; i < pPack->count; i++)
			{
				pUIManager->GetCombo()->SetComboList(i, pList[i]);
			}
			pUIManager->GetCombo()->SetComboCount(pPack->count);
			pUIManager->GetCombo()->SetActionChack(TRUE);
			pUIManager->GetCombo()->OpenMonsterCombo(TRUE,_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);

			SAFE_ARRAY_DELETE(pList);
		}
		break;
	case MSG_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT:
		{
			ResponseClient::MCGotoWaitRoomPrompt* pPack = reinterpret_cast<ResponseClient::MCGotoWaitRoomPrompt*>(istr->GetBuffer());

			CUIMsgBox_Info	MsgBoxInfo;
			CTString tv_str, strNas;

			pUIManager->GetCombo()->SetBossIndex(pPack->boosIndex);
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT)) return ;
			MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_YESNO, UI_NONE, MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT );

			strNas.PrintF("%I64d", pPack->nas);
			pUIManager->InsertCommaToString(strNas);
			tv_str.PrintF(_S(4049,"몬스터 콤보 %s Nas 입장료를 내고 입장하시겠습니까?"), strNas);
			MsgBoxInfo.AddString(tv_str);
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EX_MONSTERCOMBO_MISSION_COMPLETE:
		{
			ResponseClient::MCMissionComplete* pPack = reinterpret_cast<ResponseClient::MCMissionComplete*>(istr->GetBuffer());

			pUIManager->GetCombo()->StageComplete(pPack->stage, pPack->success);
		}
		break;
	case MSG_EX_MONSTERCOMBO_NOTICE_STAGE:  // stage(n) 시작시 stage 넘버메시지 전달
		{
			ResponseClient::MCNoticeStage* pPack = reinterpret_cast<ResponseClient::MCNoticeStage*>(istr->GetBuffer());
			
			pUIManager->GetCombo()->SetStageNum(pPack->stage);
			pUIManager->GetCombo()->SetSysImage(SYS_STAGE,TRUE);				
		}
		break;
	case MSG_EX_MONSTERCOMBO_ERROR:
		{
			ResponseClient::MCErrorMsg* pPack = reinterpret_cast<ResponseClient::MCErrorMsg*>(istr->GetBuffer());
			
			pUIManager->GetCombo()->RecComboErrorMessage(pPack->errorCode);
		}
		break;
	}

}


void CSessionState::ReceiveAttendanceMessage( CNetworkMessage* istr )
{
	GAMEDATAMGR()->GetAttendance()->RecvAttendanceMsg(istr);
}

void CSessionState::ReceiveNewsMessage(CNetworkMessage* istr)
{
	GAMEDATAMGR()->GetNews()->RecvMsg(istr);
}

void CSessionState::ReceiveNickNameMessage(CNetworkMessage* istr)
{
	GAMEDATAMGR()->GetTitleNetwork()->RecvTitleMessage(istr);
}
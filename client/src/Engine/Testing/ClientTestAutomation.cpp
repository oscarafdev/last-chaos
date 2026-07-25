#include "StdH.h"

#include <Engine/Testing/ClientTestAutomation.h>

#include <Engine/Contents/Login/LoginNew.h>
#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/GameState.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/TcpIpConnection.h>

CClientTestAutomation& CClientTestAutomation::Instance()
{
	static CClientTestAutomation automation;
	return automation;
}

CClientTestAutomation::CClientTestAutomation()
	: m_serverIndex(0)
	, m_channelIndex(0)
	, m_characterIndex(0)
	, m_enabled(FALSE)
	, m_loginSubmitted(FALSE)
	, m_serverSubmitted(FALSE)
	, m_characterSubmitted(FALSE)
	, m_lastReportedStage(-2)
{
}

void CClientTestAutomation::ConfigureLogin(
	const CTString& userName,
	const CTString& password)
{
	m_userName = userName;
	m_password = password;
	m_enabled =
		m_userName.Length() > 0 && m_password.Length() > 0;
	m_loginSubmitted = FALSE;
	m_serverSubmitted = FALSE;
	m_characterSubmitted = FALSE;
}

void CClientTestAutomation::ConfigureWorldSelection(
	INDEX serverIndex,
	INDEX channelIndex,
	INDEX characterIndex)
{
	m_serverIndex = (std::max)(serverIndex, static_cast<INDEX>(0));
	m_channelIndex = (std::max)(channelIndex, static_cast<INDEX>(0));
	m_characterIndex =
		(std::max)(characterIndex, static_cast<INDEX>(0));
}

void CClientTestAutomation::Tick()
{
	if (!m_enabled || STAGEMGR() == NULL)
		return;

	const INDEX stage = STAGEMGR()->GetCurStage();
	if (stage != m_lastReportedStage)
	{
		CPrintF(
			"Prueba automatizada: etapa %d detectada.\n",
			stage);
		m_lastReportedStage = stage;
	}
	switch (stage)
	{
	case eSTAGE_LOGIN:
		if (!m_loginSubmitted)
			m_loginSubmitted = TrySubmitLogin();
		break;
	case eSTAGE_SELSERVER:
		if (!m_serverSubmitted)
			m_serverSubmitted = TrySubmitServer();
		break;
	case eSTAGE_SELCHAR:
		if (!m_characterSubmitted)
			m_characterSubmitted = TrySubmitCharacter();
		break;
	default:
		break;
	}
}

BOOL CClientTestAutomation::TrySubmitLogin()
{
	if (_pNetwork == NULL || _pNetwork->m_bSendMessage)
		return FALSE;

	GameDataManager* pGameData = GameDataManager::getSingleton();
	CUIManager* pUiManager = CUIManager::getSingleton();
	if (pGameData == NULL || pUiManager == NULL
		|| pUiManager->GetGame() == NULL)
		return FALSE;

	LoginNew* pLogin = pGameData->GetLoginData();
	if (pLogin == NULL)
		return FALSE;

	pLogin->SetUserId(m_userName);
	pLogin->SetPassword(m_password);
	if (!pUiManager->GetGame()->PreNewGame()
		|| !pLogin->ConnectToLoginServer())
		return FALSE;

	CPrintF("Prueba automatizada: inicio de sesion enviado.\n");
	ClearPassword();
	return TRUE;
}

BOOL CClientTestAutomation::TrySubmitServer()
{
	if (_pNetwork == NULL || _pNetwork->m_bSendMessage)
		return FALSE;

	GameDataManager* pGameData = GameDataManager::getSingleton();
	if (pGameData == NULL)
		return FALSE;
	CServerSelect* pSelection = pGameData->GetServerData();
	if (pSelection == NULL)
		return FALSE;

	sServerInfo* pServer = pSelection->ServerListAt(m_serverIndex);
	if (pServer == NULL
		|| m_channelIndex < 0
		|| m_channelIndex
			>= static_cast<INDEX>(pServer->vecChannelInfo.size()))
		return FALSE;
	sChannelInfo& channel = pServer->vecChannelInfo[m_channelIndex];

	pSelection->ConnectToServer(channel.strAddress, channel.iPortNum);
	_pNetwork->m_iServerGroup = pServer->iServerNo;
	_pNetwork->m_iServerCh = channel.iSubNum;
	_pNetwork->m_iServerType = pServer->ubServerType;
	_pGameState->ClearCharacterSlot();
	CPrintF(
		"Prueba automatizada: servidor %d, canal %d enviados.\n",
		m_serverIndex,
		m_channelIndex);
	return TRUE;
}

BOOL CClientTestAutomation::TrySubmitCharacter()
{
	if (_pNetwork == NULL || _pNetwork->m_bSendMessage
		|| _pGameState == NULL
		|| m_characterIndex < 0
		|| m_characterIndex >= _pGameState->GetExistCharNum())
		return FALSE;

	const ULONG characterId =
		_pGameState->m_SlotInfo[m_characterIndex].index;
	_pNetwork->SendGameStart(characterId);
	CPrintF(
		"Prueba automatizada: personaje %d enviado.\n",
		m_characterIndex);
	return TRUE;
}

void CClientTestAutomation::ClearPassword()
{
	m_password = "";
}

BOOL CClientTestAutomation::IsEnabled() const
{
	return m_enabled;
}

const CTString& CClientTestAutomation::GetUserName() const
{
	return m_userName;
}

const CTString& CClientTestAutomation::GetPassword() const
{
	return m_password;
}

INDEX CClientTestAutomation::GetServerIndex() const
{
	return m_serverIndex;
}

INDEX CClientTestAutomation::GetChannelIndex() const
{
	return m_channelIndex;
}

INDEX CClientTestAutomation::GetCharacterIndex() const
{
	return m_characterIndex;
}

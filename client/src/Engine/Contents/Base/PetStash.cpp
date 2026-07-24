#include "StdH.h"
#include "PetStash.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/UIPetStash.h>
#include <Engine/Contents/Base/UIPetStashSelectEffect.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Ska/Render.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Object/ActorMgr.h>

PetStash::PetStash(void)
	: m_bUseStashItem(false)
	, m_nPetCount(0)
	, m_nProxyPetIndex(0)
	, m_nEffectModelNo(-1)
{
}

PetStash::~PetStash(void)
{
	ClearPetList();
}

////////////////// interface ////////////////////////////////////////
void PetStash::ClearPetList()
{
	std::vector<PetStashData*>::iterator iter = m_vecPetStashList.begin();
	std::vector<PetStashData*>::iterator iter_end = m_vecPetStashList.end();

	PetStashData* ptrTemp = NULL;
	for (; iter != iter_end; ++iter)
	{
		ptrTemp = (*iter);
		SAFE_DELETE(ptrTemp);
	}
	m_vecPetStashList.clear();
	m_nPetCount = 0;
}

PetStashData* PetStash::GetStashListAt( int idx )
{
	if ( idx < 0 || idx >= m_nPetCount )
		return NULL;

	if ( idx >= m_vecPetStashList.size() )
		return NULL;

	return m_vecPetStashList[idx];
}


void PetStash::SendTakeReq( int CharIndex, int PetIndex )
{
	CNetworkMessage nmTake;
	RequestClient::PetStashTake* packet = reinterpret_cast<RequestClient::PetStashTake*>(nmTake.nm_pubMessage);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_TAKE;
	packet->charIndex = CharIndex;
	packet->petItemVIndex = PetIndex;
	nmTake.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmTake);
}

void PetStash::SendKeepReq( int CharIndex, int PetIndex )
{
	CNetworkMessage nmKeep;
	RequestClient::PetStashKeep* packet = reinterpret_cast<RequestClient::PetStashKeep*>(nmKeep.nm_pubMessage);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_KEEP;
	packet->charIndex = CharIndex;
	packet->petItemVIndex = PetIndex;
	nmKeep.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmKeep);
}

void PetStash::SendProxyReq( int PetIndex )
{
	CNetworkMessage nmProxy;
	RequestClient::PetStashProxyReg* packet = reinterpret_cast<RequestClient::PetStashProxyReg*>(nmProxy.nm_pubMessage);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PROXY;
	packet->petItemVIndex = PetIndex;
	nmProxy.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmProxy);
}

void PetStash::SendProxyCancelReq( int PetIndex )
{
	CNetworkMessage nmProxy;
	RequestClient::PetStashProxyCancel* packet = reinterpret_cast<RequestClient::PetStashProxyCancel*>(nmProxy.nm_pubMessage);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PROXY_CANCEL;
	packet->petItemVIndex = PetIndex;
	nmProxy.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmProxy);
}

void PetStash::SendEffectReq( int nEffectNo )
{
	CNetworkMessage nmEffect;
	RequestClient::PetStashRequestEffectBroadcast* packet = reinterpret_cast<RequestClient::PetStashRequestEffectBroadcast*>(nmEffect.nm_pubMessage);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_REQUEST_EFFECT_BROADCAST;
	packet->effectNo = nEffectNo;
	nmEffect.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmEffect);
}

void PetStash::RecvPetStashMessage(CNetworkMessage* istr)
{
	CUIManager* pUImanager = CUIManager::getSingleton();

	CUIPetStash* pStash = pUImanager->GetPetStash();
	CUIPetStashSelectEffect* pEffect = pUImanager->GetPetStashSelectEffect();

	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	switch (pBase->subType)
	{
	case MSG_PET_STASH_LIST_UPDATE:
		{
			UpdateClient::PetStashList* pRecv = reinterpret_cast<UpdateClient::PetStashList*>(istr->GetBuffer());

			if (m_nPetCount != 0)
			{
				// 팻 창고 클리어
				ClearPetList();
				// UI 클리어
				if (pStash)
					pStash->ClearUI();
			}

			m_nProxyPetIndex = pRecv->proxyPetItemVIndex;
			m_nPetCount = pRecv->count;
						
			if ( m_nPetCount <= 0 )
			{
				// 펫 창고 클리어
				ClearPetList();
				// UI 클리어
				if (pStash)
					pStash->ClearUI();
			}
			else
			{
				PetStashData* pPetStashList = NULL;
				for (int i = 0; i < pRecv->count; i++)
				{
					pPetStashList = new PetStashData;
					memcpy( pPetStashList, &pRecv->petStashData[i], sizeof(PetStashData));
					m_vecPetStashList.push_back(pPetStashList);
					pPetStashList = NULL;
				}
				UpdatePetInfo();
			}
		}
		break;

	case MSG_PET_STASH_PET_UPDATE:
		{
			UpdateClient::PetStashPetData* pRecv = reinterpret_cast<UpdateClient::PetStashPetData*>(istr->GetBuffer());

			PetStashData* pPetData = new PetStashData;
			memcpy( pPetData, &pRecv->petStashData, sizeof(PetStashData) );
			_UpdatePet(pPetData);

			SAFE_DELETE(pPetData);
		}
		break;

	case MSG_PET_STASH_TAKE:
		{
			ResponseClient::PetStashTake* pRecv = reinterpret_cast<ResponseClient::PetStashTake*>(istr->GetBuffer());

			if (pRecv->result != ERROR_PET_STASH_TAKE_SUCCESS)
			{
				_PetErrorSystemMessage(pRecv->result);
			}
		}
		break;

	case MSG_PET_STASH_TAKE_UPDATE:
		{
			UpdateClient::PetStashTake* pRecv = reinterpret_cast<UpdateClient::PetStashTake*>(istr->GetBuffer());
			DeletePet(pRecv->petItemVIndex);
		}
		break;

	case MSG_PET_STASH_KEEP:
		{
			ResponseClient::PetStashKeep* pRecv = reinterpret_cast<ResponseClient::PetStashKeep*>(istr->GetBuffer());

			if (pRecv->result != ERROR_PET_STASH_KEEP_SUCCESS)
			{
				_PetErrorSystemMessage(pRecv->result);
			}
		}
		break;

	case MSG_PET_STASH_KEEP_UPDATE:
		{
			UpdateClient::PetStashKeep* pRecv = reinterpret_cast<UpdateClient::PetStashKeep*>(istr->GetBuffer());

			PetStashData* pPetData = new PetStashData;
			memcpy( pPetData, &pRecv->petStashData, sizeof(PetStashData) );
			AddPet(pPetData);
		}
		break;

	case MSG_PET_STASH_PROXY:
		{
			ResponseClient::PetStashProxyReg* pRecv = reinterpret_cast<ResponseClient::PetStashProxyReg*>(istr->GetBuffer());
			
			if (pRecv->result != ERROR_PET_STASH_PROXY_SUCCESS)
			{
				_PetErrorSystemMessage(pRecv->result);
			}
		}
		break;

	case MSG_PET_STASH_PROXY_CANCEL:
		{
			ResponseClient::PetStashProxyCancel* pRecv = reinterpret_cast<ResponseClient::PetStashProxyCancel*>(istr->GetBuffer());

			if (pRecv->result != ERROR_PET_STASH_PROXY_CANCEL_SUCCESS)
			{
				_PetErrorSystemMessage(pRecv->result);
			}
		}
		break;

	case MSG_PET_STASH_EFFECT:
		{
			UpdateClient::PetStashEffect* pRecv = reinterpret_cast<UpdateClient::PetStashEffect*>(istr->GetBuffer());
			
			m_nEffectModelNo = pRecv->effectNo;
			
			if (pRecv->effectNo >= 0)
			{
				pUImanager->GetPetStashSelectEffect()->OpenPetStashSelectEffect();
			}
		}
		break;

	case MSG_PET_STASH_EFFECT_BY_INMAP: // 인맵, 기간만료시 제거
		{
			UpdateClient::PetStashEffectByInMap* pRecv = reinterpret_cast<UpdateClient::PetStashEffectByInMap*>(istr->GetBuffer());

			SetEffectModelNo(pRecv->effectNo);
		}
		break;

	case MSG_PET_STASH_EFFECT_BROAD_CAST:
		{
			UpdateClient::PetStashEffectBr* pRecv = reinterpret_cast<UpdateClient::PetStashEffectBr*>(istr->GetBuffer());

			int charIndex = pRecv->charIndex;
			int EffectNo = pRecv->effectNo;

			if (charIndex == _pNetwork->MyCharacterInfo.index)
				return;

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, charIndex);

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
				
				pTarget->cha_itemEffect.AddPetStashEffect( EffectNo, &(pTarget->m_pEntity->en_pmiModelInstance->m_tmSkaTagManager) );
			}
		}
		break;

	case MSG_PET_STASH_ERROR:
		{
			ResponseClient::PetStashError* pRecv = reinterpret_cast<ResponseClient::PetStashError*>(istr->GetBuffer());
			int errorCode = pRecv->errorCode;
						
			_PetErrorSystemMessage(errorCode);
		}
		break;

	case MSG_PET_STASH_PROXY_UPDATE:
		{
			UpdateClient::PetStashProxyReg* pRecv = reinterpret_cast<UpdateClient::PetStashProxyReg*>(istr->GetBuffer());
			m_nProxyPetIndex = pRecv->virtualIndex;
		}
		break;

	case MSG_PET_STASH_PROXY_CANCEL_UPDATE:
		{
			UpdateClient::PetStashProxyCancel* pRecv = reinterpret_cast<UpdateClient::PetStashProxyCancel*>(istr->GetBuffer());
			m_nProxyPetIndex = pRecv->virtualIndex;
		}
		break;
	}

	if (pStash->IsVisible() != FALSE)
		pStash->UpdateUI();
}

void PetStash::SetEffectModelNo( int nEffectNo )
{
	m_nEffectModelNo = nEffectNo; 

	if (m_nEffectModelNo < 0)
	{
		_pNetwork->MyCharacterInfo.itemEffect.DeletePetStashEffect();
		return;
	}

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
	CModelInstance *pMI			= NULL;
	
	if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
	{
		CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

		INDEX ctmi = pMITemp->mi_cmiChildren.Count();
		if( ctmi > 0 )
			pMI = &pMITemp->mi_cmiChildren[0];
		else
			pMI	= penPlayerEntity->GetModelInstance();
	}
	else
	{
		pMI	= penPlayerEntity->GetModelInstance();
	}			

	if (pMI != NULL)
		_pNetwork->MyCharacterInfo.itemEffect.AddPetStashEffect( m_nEffectModelNo, &pMI->m_tmSkaTagManager);
}

void PetStash::AddPet( PetStashData* pPetInfo )
{
	m_vecPetStashList.push_back( pPetInfo );
	m_nPetCount = m_vecPetStashList.size();

	if ( pPetInfo->isPetOrApet == PET )
		_P1PetAdd( pPetInfo );
	else if ( pPetInfo->isPetOrApet == APET )
		_P2PetAdd( pPetInfo );
}

void PetStash::DeletePet( int nPetVirIndex )
{
	std::vector<PetStashData*>::iterator iter = m_vecPetStashList.begin();
	std::vector<PetStashData*>::iterator iter_end = m_vecPetStashList.end();

	for ( ; iter != iter_end; ++iter )
	{
		if ( (*iter)->virtualIndex == nPetVirIndex )
		{
			PetStashData* ptrTemp = *iter;
			m_vecPetStashList.erase( iter );
			m_nPetCount = m_vecPetStashList.size();
			if (nPetVirIndex == m_nProxyPetIndex)
				m_nProxyPetIndex = 0;
			SAFE_DELETE(ptrTemp);
			return;
		}
	}
}

void PetStash::UpdatePetInfo()
{
	std::vector<PetStashData*>::iterator iter = m_vecPetStashList.begin();
	std::vector<PetStashData*>::iterator iter_end = m_vecPetStashList.end();

	for ( ; iter != iter_end; ++iter )
	{
		if ( (*iter)->isPetOrApet == PET )
			_P1PetAdd( (*iter) );
		else
			_P2PetAdd( (*iter) );
	}
}

void PetStash::_P1PetDelete( PetStashData* pPetInfo )
{
	const INDEX iPetIndex = pPetInfo->plus;
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex = iPetIndex;
	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
	std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	if( iter != _pNetwork->m_vectorPetList.end() )
	{
		_pNetwork->m_vectorPetList.erase( iter );
	}
}
void PetStash::_P1PetAdd( PetStashData* pPetInfo )
{
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= pPetInfo->plus;
	TempPet.sbPetTypeGrade		= pPetInfo->grade;
	TempPet.lLevel				= pPetInfo->petLevel;
	TempPet.llExp				= pPetInfo->petExp;
	TempPet.llNeedExp			= pPetInfo->petLevelupExp;
	TempPet.lHP					= pPetInfo->hp;
	TempPet.lMaxHP				= pPetInfo->maxHp;
	TempPet.lAbility			= pPetInfo->abilityPoint;
	TempPet.lHungry				= pPetInfo->petHungry;
	TempPet.lMaxHungry			= pPetInfo->petHungryMax;
	TempPet.lSympathy			= pPetInfo->petFaith;
	TempPet.lMaxSympathy		= pPetInfo->petFaithMax;
	TempPet.strNameCard.PrintF(pPetInfo->petName);

	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );

	// 펫 정보를 못 찾으면 추가함.
	if( iter == _pNetwork->m_vectorPetList.end() )
	{
		_pNetwork->m_vectorPetList.push_back(TempPet);
	}	
	// 펫 정보를 갱신함.
	else
	{		
		(*iter).sbPetTypeGrade	= TempPet.sbPetTypeGrade;
		(*iter).lLevel			= TempPet.lLevel;
		(*iter).llExp			= TempPet.llExp;
		(*iter).llNeedExp		= TempPet.llNeedExp;
		(*iter).lHP				= TempPet.lHP;
		(*iter).lMaxHP			= TempPet.lMaxHP;
		(*iter).lHungry			= TempPet.lHungry;
		(*iter).lMaxHungry		= TempPet.lMaxHungry;
		(*iter).lSympathy		= TempPet.lSympathy;
		(*iter).lMaxSympathy	= TempPet.lMaxSympathy;
		(*iter).lAbility		= TempPet.lAbility;
		(*iter).strNameCard		= TempPet.strNameCard;
	}
	_pNetwork->UpdatePetTargetInfo( TempPet.lIndex );
}

void PetStash::_P2PetAdd( PetStashData* pPetInfo )
{
	CUIManager* pUImanager = CUIManager::getSingleton();
	sPetItem_Info temPetInfo;
	temPetInfo.pet_index = pPetInfo->plus;
	temPetInfo.pet_name.PrintF(pPetInfo->petName);
	temPetInfo.pet_level = pPetInfo->petLevel;
	temPetInfo.pet_str = pPetInfo->petStr + pPetInfo->petPlusStr;
	temPetInfo.pet_con = pPetInfo->petCon + pPetInfo->petPlusCon;
	temPetInfo.pet_dex = pPetInfo->petDex + pPetInfo->petPlusDex;
	temPetInfo.pet_int = pPetInfo->petInt + pPetInfo->petPlusInt;

	temPetInfo.pet_accexp = pPetInfo->petAccExp;
	if (pPetInfo->remainTime > 0)
	{
		temPetInfo.pet_cooltime = (unsigned int)_pTimer->GetLerpedCurrentTick() + pPetInfo->remainTime;
	}else
	{
		temPetInfo.pet_cooltime = 0;
	}

	pUImanager->GetWildPetInfoUI()->AddWildPetInfo(temPetInfo);
}

void PetStash::_UpdatePet( PetStashData* pPetInfo )
{
	std::vector<PetStashData*>::iterator iter = m_vecPetStashList.begin();
	std::vector<PetStashData*>::iterator iter_end = m_vecPetStashList.end();

	for ( ; iter != iter_end; ++iter )
	{
		if ( (*iter)->virtualIndex == pPetInfo->virtualIndex )
		{			
			(*iter)->petLevel = pPetInfo->petLevel;
			(*iter)->petHungry = pPetInfo->petHungry;
			(*iter)->petLevelupExp = pPetInfo->petLevelupExp;
			(*iter)->petExp = pPetInfo->petExp;
			(*iter)->petFaith = pPetInfo->petFaith;
			(*iter)->hp = pPetInfo->hp;
			(*iter)->maxHp = pPetInfo->maxHp;
			(*iter)->abilityPoint = pPetInfo->abilityPoint;
			(*iter)->petAccExp = pPetInfo->petAccExp;
			(*iter)->remainTime = pPetInfo->remainTime;

			if ( pPetInfo->isPetOrApet == PET )
				_P1PetAdd( pPetInfo );
			else if ( pPetInfo->isPetOrApet == APET )
				_P2PetAdd( pPetInfo );
		}
	}
}

void PetStash::_PetErrorSystemMessage( int nErrorCode )
{
	CTString strMessage;

	switch(nErrorCode)
	{
	case ERROR_PET_STASH_KEEP_FAIL_ALREADY_EQ_PET:		//펫을 착용중이다.
		{
			strMessage.PrintF( _S( 825, "장착하고 있는 아이템은 보관할 수 없습니다.") );
		}
		break;
	case ERROR_PET_STASH_KEEP_FAIL_PET_SEAL:			//펫이 봉인상태이다.
		{
			strMessage.PrintF( _S( 5952, "봉인 된 펫은 보관 하실 수 없습니다.") );
		}
		break;
	case ERROR_PET_STASH_FAIL_ETC:						//알수 없는 오류
		{
			strMessage.PrintF( _S( 16, "알수 없는 오류가 발생했습니다.") );
		}
		break;
	case ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET:		//펫이 존재하지 않음
		{
			strMessage.PrintF( _S( 6071, "펫이 존재 하지 않습니다.") );
		}
		break;
	case ERROR_PET_STASH_KEEP_FAIL_TERMINATION_ITEM:	//펫 등록 아이템 사용기간 만료	
		{
			strMessage.PrintF( _S( 6072, "펫 창고 이용권 사용기간이 만료 되었습니다.") );
		}
		break;
	case ERROR_PET_STASH_KEEP_FAIL_FULL:				//펫 창고가 가득 참.
		{
			strMessage.PrintF( _S( 6073, "펫 창고에 공간이 부족 합니다.") );
		}
		break;
	case ERROR_PET_STASH_TAKE_FAIL:						//펫 찾기 실패
		{
			strMessage.PrintF( _S( 2850, "인벤토리가 가득 찼습니다.") );
		}
		break;
	case ERROR_PET_STASH_ERROR_NOT_USE_SYSTEM:			//펫 창고 시스템을 사용할 수 없습니다.
		{
			strMessage.PrintF( _S( 6075, "펫 창고 시스템을 사용할 수 없습니다.") );
		}
		break;
	default:
		return;
	}
	_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
}




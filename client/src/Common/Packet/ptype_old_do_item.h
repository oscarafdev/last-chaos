#ifndef __PTYPE_OLD_DO_ITEM_H__
#define __PTYPE_OLD_DO_ITEM_H__

#define MAX_COMPOSE_SLOT_COUNT 5

#include "ptype_base.h"

enum
{
	MASTERSTONE_INCREASE = 1,
	MASTERSTONE_NOT_CHANGE,
	MASTERSTONE_DECREASE,
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
// MSG_ITEM : MSG_ITEM_TAKE
struct doItemTake : public pTypeBase
{
	char			char_type;
	int				char_index;
	int				virtualIndex;
};

// MSG_ITEM : MSG_ITEM_THROW
struct doItemThrow : public pTypeBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
	unsigned int	virtualIndex;
	LONGLONG		count;
};

// MSG_ITEM : MSG_ITEM_DELETE
struct doItemDelete : public pTypeBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
	unsigned int	virualIndex;
};

// MSG_ITEM : MSG_ITEM_WEAR
struct doItemWear : public pTypeBase
{
	char			wearPos;
	unsigned short	tab;
	unsigned short	invenIndex;
};

struct doItemWearTakeOff : public pTypeBase
{
	char			wearPos;
	unsigned short	tab;
	unsigned short	invenIndex;
};

// MSG_ITEM : MSG_ITEM_SWAP
struct doItemSwap : public pTypeBase
{
	unsigned short	stab;
	unsigned short	sinvenIndex;
	unsigned short	ttab;
	unsigned short	tinvenIndex;
};

// MSG_ITEM : MSG_ITEM_BUY
struct doItemBuy : public pTypeBase
{
	int				npcIndex;
	LONGLONG		clientPrice;
	unsigned short	buyCount;
	struct
	{
		unsigned int	dbIndex;
		unsigned short	count;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_SELL
struct doItemSell : public pTypeBase
{
	int				npcIndex;
	LONGLONG		clientPrice;
	unsigned short	sellCount;
	struct
	{
		unsigned short	tab;
		unsigned short	invenIndex;
		short			count;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_UPGRADE_REQ
struct doItemUpgrade : public pTypeBase
{
	unsigned int		wearPos;			// 업그레이드 아이템 / 착용중인 아이템
	unsigned int		vIndex;
	unsigned short		tab_2;			// 제련석 아이템
	unsigned short		invenIndex_2;
	unsigned short		tab_3;			// 제련 보호 아이템
	unsigned short		invenIndex_3;
	int					plus;
	int					level;
	int					runeItemDBIndex;
};

// MSG_ITEM : MSG_ITEM_REFINE_REQ
struct doItemRefin : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
};

// MSG_ITEM : MSG_ITEM_OPTION_ADD_REQ
struct doItemAddOption : public pTypeBase
{
	unsigned short		weapon_wearPos;
	int					weapon_virtualIndex;
	unsigned short		jam_tab;
	unsigned short		jam_invenIndex;
	int					jam_virtualIndex;
};

// MSG_ITEM : MSG_ITEM_OPTION_DEL_REQ
struct doItemDelOption : public pTypeBase
{
	unsigned short		weapon_wearPos;
	int					weapon_virtualIndex;
	unsigned short		jam_tab;
	unsigned short		jam_invenIndex;
	int					jam_virtualIndex;
};

// MSG_ITEM : MSG_ITEM_PROCESS_REQ
struct doItemProcess : public pTypeBase
{
	unsigned short		useitem_tab;
	unsigned short		useitem_invenIndex;
	int					resultItemDBIndex;
	unsigned short		listCount;
	struct
	{
		unsigned short	tab;
		unsigned short	invenIndex;
		int				count;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_MAKE_REQ
struct doItemMake : public pTypeBase
{
	unsigned short		useitem_tab;
	unsigned short		useitem_invenIndex;
	int					resultItemDBIndex;
	unsigned short		listCount;
	struct
	{
		int				dbIndex;
		int				count;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_MIX_REQ
struct doItemMix : public pTypeBase
{
	unsigned short		useitem_tab;
	unsigned short		useitem_invenIndex;
	struct
	{
		unsigned short	tab;
		unsigned short	invenIndex;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_ARCANE_REQ
struct doItemArcane : public pTypeBase
{
	unsigned short		useitem_tab;
	unsigned short		useitem_invenIndex;
	struct
	{
		unsigned short	tab;
		unsigned short	invenIndex;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_CHANGEWEAPON_REQ
struct doItemChangeWeapon : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
	int					exchangeDBIndex;
	int					virtualIndex;
	int					goldTokenItemVIndex;
	int					goldTokenCount;
	int					changeType;
};

// MSG_ITEM : MSG_ITEM_PROCESS_NPC
struct doItemProcessNpc : public pTypeBase
{
	int					itemDBIndex;
	int					count;
};

// MSG_ITEM : MSG_ITEM_ADD_BOOSTER
struct doItemAddBooster : public pTypeBase
{
	int					boostitem_vitualIndex;
};

// MSG_ITEM : MSG_ITEM_MIX_WARITEM
struct doItemMixWar : public pTypeBase
{
	unsigned short		tab_1;
	unsigned short		invenIndex_1;
	unsigned short		tab_2;
	unsigned short		invenIndex_2;
	unsigned short		tab_3;
	unsigned short		invenIndex_3;
};

// MSG_ITEM : MSG_ITEM_USE
struct doItemUse : public pTypeBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
	unsigned int	virtualIndex;
	int				extra_1;
	int				extra_2;
};

// MSG_ITEM : MSG_ITEM_LEND_WEAPON
struct doItemLendWeapon : public pTypeBase
{
	int				itemDBIndex;
};

// MSG_ITEM : MSG_ITEM_TARGET
struct doItemTarget : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
	int					virtualIndex;
	int					charIndex;
	char				charType;
};

// MSG_ITEM : MSG_ITEM_EXCHANGE_EQUIP_REQ
struct doItemUseExchangeEquip : public pTypeBase
{
	unsigned short		tab_1;
	unsigned short		invenIndex_1;
	unsigned short		tab_2;
	unsigned short		invenIndex_2;
	int					virtualIndex;
	int					changeType;
	int					exchangeDBIndex;
};

// MSG_ITEM : MSG_ITEM_USE_GROCERY_BUY
struct doItemGroceryItemBuy : public pTypeBase
{
	LONGLONG			clientPrice;
	unsigned short		buyCount;
	struct
	{
		int				itemDBIndex;
		int				count;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_USE_GROCERY_SELL
struct doItemGroceryItemSell : public pTypeBase
{
	LONGLONG			clientPrice;
	unsigned short		sellCount;
	struct
	{
		unsigned short	tab;
		unsigned short	invenIndex;
		int				count;
	} list[0];
};

// MSG_ITEM : MSG_ITEM_USE_WARPDOC
struct doItemUseWarpDoc : public pTypeBase
{
	int					virtualIndex;
};

// MSG_ITEM : MSG_ITEM_USE_PRESSCORPS
struct doItemUsePressCorps : public pTypeBase
{
	int					virtuaIndex;
	char				str[1024];
};

// MSG_ITEM : MSG_ITEM_CHECK_COMPOSITION
struct doItemCheckComposition : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
	int					virtualIndex;
};

// MSG_ITEM : MSG_ITEM_WEAR_COSTUME
struct doItemWearCostume : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
	char				wearPos;
};

struct doItemWearCostumeTakeOff : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
	char				wearPos;
	int					vIndex;
};

// MSG_ITEM : MSG_ITEM_WEAR_COSTUME_SUIT
struct doItemWearCostumeSuit : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
};

struct doItemWearCostumeSuitTakeOff : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
};

// MSG_ITEM : MSG_ITEM_COSTUME_SUIT_FORTUNE
struct doItemMakeFortune : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;
};

// MSG_ITEM : MSG_ITEM_MASTERSTONE_USE
struct doItemUseMasterStoneUSA : public pTypeBase
{
	unsigned short		tab_1;			// 마스터 스톤
	unsigned short		invenIndex_1;
	int					itemDBIndex_1;
	unsigned short		tab_2;			// 업그레이드할 장비
	unsigned short		invenIndex_2;
	int					itemDBIndex_2;
};

// MSG_ITEM : MSG_ITEM_DIVIDE
struct doItemDivide : public pTypeBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
	unsigned short	ttab;
	unsigned short	tinvenIndex;
	int				divide_count;
};

struct doItemInventorySort : public pTypeBase
{
};

struct doItemExchange : public pTypeBase
{
	int		exchangeIndex;
	short	listCount;
	struct tag_list
	{
		int					itemIndex;
		int					itemCount;
	} list[0];
};

struct slotItemInfo
{
	unsigned short	tab;
	unsigned short	inven_index;
	int				virtual_index;

	slotItemInfo() : tab(-1), inven_index(-1), virtual_index(-1) {}
};

struct doItemCompose : public pTypeBase
{
	int matItem_Index;			//	재료 인덱스
	slotItemInfo matInfo[MAX_COMPOSE_SLOT_COUNT];	//	재료 아이템 인포
};

struct doItemUpgradePet : public pTypeBase
{
	unsigned int		wearPos;			// 착용중인 펫 장비 장착위치 정보
	unsigned short		tab;				// 제련석 아이템 tab
	unsigned short		inven_index;		// 제련석 아이템 inven_index
};

}

//////////////////////////////////////////////////////////////////////////

namespace ResponseClient
{
enum
{
	WEAR_ERR_OK,
	WEAR_ERR_INVALID_POS,		//착용위치가 잘못 되었습니다.
	WEAR_ERR_CANNOT_TAKEOFF,	//아이템을 벗을 수 없습니다.
	WEAR_ERR_CANNOT_WEAR,		//아이템을 착용할 수 없습니다.
	WEAR_ERR_NOT_ENOUGH_INVEN,	//인벤 공간 부족
	WEAR_ERR_INVALID_ITEM_INFO, //올바른 아이템이 아닙니다.
	WEAR_ERR_ITEM_SEAL,			//봉인된 아이템 입니다.
	WEAR_ERR_CANNOT_SWAP_ITEM_CASH_INVEN,	//캐쉬 인벤토리의 기간이 만료되어 벗은 아이템을 넣을 수 없습니다.
	//아이템 교환
	EXCHANGE_ERR_WRONG_ITEM_INFO,	//잘못된 아이템 정보
	EXCHANGE_ERR_NOT_ENOUGH_MONEY,	//돈 부족
	EXCHANGE_ERR_INVENTORY_FULL,	//인벤토리 공간 부족
	EXCHANGE_ERR_OK,				//교환 성공
	
	//펫 장비 강화
	PET_ITEM_UPGRADE_PLUS,			//강화 성공
	PET_ITEM_UPGRADE_MINUS,			//강화 실패 1마이너스
	PET_ITEM_UPGRADE_NO_CHANGE,		//강화 실패 변화 없음
	PET_ITEM_UPGRADE_BROKE,			//강화 실패 장비 파괴
};
struct doItemUse : public pTypeBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
	int				index;
	int				extral;
};

struct doItemNotuse : public pTypeBase
{
	unsigned short	error_type;
	unsigned short	tab;
	unsigned short	invenIndex;
	int				index;
};

struct doItemWearError : public pTypeBase
{
	int errorCode;
};

struct doItemExchange : public pTypeBase
{
	char	result;	// 0 - ok, >= 1 - fail
};

struct doItemChangeWeapon : public pTypeBase
{
	int oldDBIndex;
	int newDBIndex;
	int errorCode;
};

struct doItemCompose : public pTypeBase
{
	int item_index;
	int errorCode;
};

struct doItemUseMasterStoneUSA : public pTypeBase
{
	int success_type;
	int error;
};

struct doItemUpgradePet : public pTypeBase
{
	unsigned int		error;
};

#ifndef _CLIENT_
inline void ItemUseMsg(CNetMsg::SP& msg, unsigned short tab, unsigned short invenIndex, int index, int extra)
{
	doItemUse* packet = reinterpret_cast<doItemUse*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE;
	packet->tab = tab;
	packet->invenIndex = invenIndex;
	packet->index = index;
	packet->extral = extra;
	msg->setSize(sizeof(doItemUse));
}

inline void ItemNotUseMsg(CNetMsg::SP& msg, int error_type, int tab = 0, int invenIndex = 0, int index = 0)
{
	doItemNotuse* packet = reinterpret_cast<doItemNotuse*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE_ERROR;
	packet->error_type = error_type;
	packet->tab = tab;
	packet->invenIndex = invenIndex;
	packet->index = index;
	msg->setSize(sizeof(doItemNotuse));
}

inline void ItemTakeoffMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::doItemWearError* packet = reinterpret_cast<ResponseClient::doItemWearError*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR_TAKE_OFF;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(doItemWearError));
}

inline void ItemWearMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::doItemWearError* packet = reinterpret_cast<ResponseClient::doItemWearError*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(doItemWearError));
}

inline void CostItemWearMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::doItemWearError* packet = reinterpret_cast<ResponseClient::doItemWearError*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR_COSTUME;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(doItemWearError));
}

inline void CostItemTakeOffMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::doItemWearError* packet = reinterpret_cast<ResponseClient::doItemWearError*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR_COSTUME_TAKEOFF;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(doItemWearError));
}

inline void CostSuitItemWearMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::doItemWearError* packet = reinterpret_cast<ResponseClient::doItemWearError*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR_COSTUME_SUIT;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(doItemWearError));
}

inline void CostSuitItemTakeOffMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::doItemWearError* packet = reinterpret_cast<ResponseClient::doItemWearError*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR_COSTUME_SUIT_TAKEOFF;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(doItemWearError));
}

inline void makeItemExchange(CNetMsg::SP& msg, int result)
{
	ResponseClient::doItemExchange* packet = reinterpret_cast<ResponseClient::doItemExchange*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_EXCHANGE;
	packet->result = result;
	msg->setSize(sizeof(ResponseClient::doItemExchange));
}

inline void makeItemChangeWeapon(CNetMsg::SP& msg, int oldDBIndex, int newDBIndex, int errorCode)
{
	ResponseClient::doItemChangeWeapon* packet = reinterpret_cast<ResponseClient::doItemChangeWeapon*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_CHANGEWEAPON_REP;
	packet->oldDBIndex = oldDBIndex;
	packet->newDBIndex = newDBIndex;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(ResponseClient::doItemChangeWeapon));
}

inline void makeItemCompose(CNetMsg::SP& msg, int item_index, int errorCode)
{
	ResponseClient::doItemCompose* packet = reinterpret_cast<ResponseClient::doItemCompose*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_COMPOSE;
	packet->item_index = item_index;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(ResponseClient::doItemCompose));
}

inline void makeMasterStone(CNetMsg::SP& msg, int random_success, int error)
{
	ResponseClient::doItemUseMasterStoneUSA* packet = reinterpret_cast<ResponseClient::doItemUseMasterStoneUSA*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_MASTERSTONE_USE;
	packet->success_type = random_success;
	packet->error = error;
	msg->setSize(sizeof(ResponseClient::doItemUseMasterStoneUSA));
}

inline void makePetItemUpgrade(CNetMsg::SP& msg, int error_code)
{
	doItemUpgradePet* packet = reinterpret_cast<doItemUpgradePet*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_UPGRADE_PET;
	packet->error = error_code;
	msg->setSize(sizeof(doItemUpgradePet));
}
#endif
}

namespace UpdateClient
{
struct doItemWear : public pTypeBase	// 앞쪽은 RequestClient::doItemWear와 모양이 같아야함
{
	itemInfo	info;
};

struct doItemWearChangeInfo : public pTypeBase	// 앞쪽은 RequestClient::doItemWear와 모양이 같아야함
{
	itemInfo	info;
};

struct doItemWearTakeOff : public pTypeBase	// 앞쪽은 RequestClient::doItemWear와 모양이 같아야함
{
	char		wearPos;
};

struct doItemWearList : public pTypeBase
{
	short		count;
	itemInfo	info_list[0];
};

struct doItemWearCostume : public pTypeBase
{
	itemInfo	info;
};

struct doItemWearCostumeTakeOff : public pTypeBase
{
	char		wearPos;
};

struct doItemWearCostumeList : public pTypeBase
{
	short		count;
	itemInfo	info_list[0];
};

struct doItemWearCostumeSuit : public pTypeBase
{
	int			DBIndex;
	int			VIndex;
	itemInfo	info_list[5];
};

#ifndef _CLIENT_
inline void ItemWearMsg(CNetMsg::SP& msg)
{
	UpdateClient::doItemWear* packet = reinterpret_cast<UpdateClient::doItemWear*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_WEAR;
	msg->setSize(sizeof(doItemWear));
}
inline void ItemTakeoffMsg(CNetMsg::SP& msg, char wearPos)
{
	UpdateClient::doItemWearTakeOff* packet = reinterpret_cast<UpdateClient::doItemWearTakeOff*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_WEAR_TAKEOFF;
	packet->wearPos = wearPos;
	msg->setSize(sizeof(doItemWearTakeOff));
}
inline void ItemWearListMsg(CNetMsg::SP& msg)
{
	UpdateClient::doItemWearList* packet = reinterpret_cast<UpdateClient::doItemWearList*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_WEAR_LIST;
}
inline void ItemWearCostumeMsg(CNetMsg::SP& msg)
{
	UpdateClient::doItemWearCostume* packet = reinterpret_cast<UpdateClient::doItemWearCostume*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_COST_WEAR;
	msg->setSize(sizeof(doItemWearCostume));
}
inline void ItemWearCostumeTakeoffMsg(CNetMsg::SP& msg, char wearPos)
{
	UpdateClient::doItemWearCostumeTakeOff* packet = reinterpret_cast<UpdateClient::doItemWearCostumeTakeOff*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_COST_WEAR_TAKEOFF;
	packet->wearPos = wearPos;
	msg->setSize(sizeof(doItemWearCostumeTakeOff));
}
inline void ItemWearCostumeListMsg(CNetMsg::SP& msg)
{
	UpdateClient::doItemWearCostumeList* packet = reinterpret_cast<UpdateClient::doItemWearCostumeList*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_COST_WEAR_LIST;
}
inline void ItemWearCostumeSuitMsg(CNetMsg::SP& msg, CItem* item)
{
	UpdateClient::doItemWearCostumeSuit* packet = reinterpret_cast<UpdateClient::doItemWearCostumeSuit*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_COST_WEAR_SUIT;
	packet->DBIndex = item->getDBIndex();
	packet->VIndex = item->getVIndex();
	msg->setSize(sizeof(UpdateClient::doItemWearCostumeSuit) + COSTUME2_SUIT_MAX_WEARING * (sizeof(UpdateClient::itemInfo)));
}
#endif
}


#ifndef _CLIENT_
//////////////////////////////////////////////////////////////////////////
// 아래 함수들은 서버 내부에서 do_Item()을 사용하기 위해서만 이용해야함
// 원리는 클라이언트가 보낸 패킷처럼 모양을 만들어 do_Item()를 사용하려고 했음 (미쳤지)
inline void insideServer_do_ItemDelete(CNetMsg::SP& msg, CItem* pItem)
{
	RequestClient::doItemDelete* packet = reinterpret_cast<RequestClient::doItemDelete*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_DELETE;
	packet->tab = pItem->tab();
	packet->invenIndex = pItem->getInvenIndex();
	packet->virualIndex = pItem->getVIndex();
}

inline void insideServer_do_Item_Make_Fortuen(CNetMsg::SP& msg, int tab, int invenIndex)
{
	RequestClient::doItemMakeFortune* packet = reinterpret_cast<RequestClient::doItemMakeFortune*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_COSTUME_SUIT_FORTUNE;
	packet->tab = tab;
	packet->invenIndex = invenIndex;
}

inline void insideServer_do_Item_Wear(CNetMsg::SP& msg, int wearPos, int tab = 0, int invenIndex = 0, int extra = -1)
{
	RequestClient::doItemWear* packet = reinterpret_cast<RequestClient::doItemWear*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR;
	packet->wearPos = wearPos;
	packet->tab = tab;
	packet->invenIndex = invenIndex;
}

inline void insideServer_do_Item_Wear_TakeOff(CNetMsg::SP& msg, int wearPos)
{
	RequestClient::doItemWearTakeOff* packet = reinterpret_cast<RequestClient::doItemWearTakeOff*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR_TAKE_OFF;
	packet->wearPos = wearPos;
	packet->tab = -1;
	packet->invenIndex = -1;
}

inline void insideServer_do_Item_Costume(CNetMsg::SP& msg, int wearPos)
{
	RequestClient::doItemWearCostume* packet = reinterpret_cast<RequestClient::doItemWearCostume*>(msg->m_buf);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_WEAR_COSTUME;
	packet->tab = -1;
	packet->invenIndex = -1;
	packet->wearPos = wearPos;
}

#endif
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif

#include "stdh.h"
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/CashShopData.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Network/CNetwork.h>

extern INDEX	g_iCountry;

#define NEW_SHOP
#define ITEM_ENABLE // wooss 060503 상품 등록을 용이하게(판매중지된 제품 장바구니에 표시)
//////////////////////////////////////////////////////////////////////////
// 상품 플래그 
#ifdef NEW_SHOP
#define		CATALOG_FLAG_NEW	(1 << 0)	// 신상품
#define		CATALOG_FLAG_POP	(1 << 1)	// 인기상품
#endif
//////////////////////////////////////////////////////////////////////////

CCashShopData::~CCashShopData()
{
	m_vShopItemArray.clear();
	
}

// 검색을 자주하게 되었음으로, 추후에 검색 개선이 필요 하겠다.
// 최신이 항상 먼저 나오게 하기로 하였으므로, reverse searching 하겠다.
CCashShopData::CASH_SHOP_DATA* CCashShopData::GetCashShopData(INDEX shopItemIndex)
{
// 	std::vector<CASH_SHOP_DATA>::reverse_iterator tmpItr;
// 
// 	for (tmpItr=m_vShopItemArray.rbegin(); tmpItr!=m_vShopItemArray.rend(); ++tmpItr)
// 	{
// 		CASH_SHOP_DATA tmpData = (*tmpItr);
// 
// 		if (tmpData.m_shopItemIndex == shopItemIndex)
// 		{
// 			return &(*tmpItr);
// 		}
// 	}
// 
// 	return NULL;
	CASH_SHOP_DATA findData;
	findData.m_shopItemIndex = shopItemIndex;
	std::vector<CASH_SHOP_DATA>::reverse_iterator rFindItr = std::find_if( m_vShopItemArray.rbegin(), m_vShopItemArray.rend(), findData );
	return ((rFindItr != m_vShopItemArray.rend())?&(*rFindItr):NULL);
}

void CCashShopData::CASH_SHOP_DATA::operator = (const CCashShopData::CASH_SHOP_DATA& csData)
{
	(*this).m_shopItemIndex = csData.m_shopItemIndex;
	(*this).m_type = csData.m_type;
	(*this).m_cash = csData.m_cash;
	(*this).m_mileage = csData.m_mileage;
	(*this).m_texID = csData.m_texID;
	(*this).m_texRow = csData.m_texRow;
	(*this).m_texCol = csData.m_texCol;
	(*this).m_flag = csData.m_flag;
	(*this).m_enable = csData.m_enable;
	(*this).m_itemName = csData.m_itemName;
	(*this).m_itemDesc = csData.m_itemDesc;
	(*this).m_itemListCnt = csData.m_itemListCnt;
	(*this).m_typeIndex = csData.m_typeIndex;
	(*this).m_limitCnt = csData.m_limitCnt;

	(*this).m_vItemDataArray.clear();

	// vector의 경우 1:1 copy가 이루어짐. [2/16/2011 rumist]
	// 이편이 속도가 더 빠름. 내부적으로 memcpy가 이루어지므로.
	(*this).m_vItemDataArray = csData.m_vItemDataArray;
// 	std::vector<CCashShopData::ITEM_DATA>::const_iterator Itr;
// 
// 	for (Itr = csData.m_vItemDataArray.begin(); Itr!=csData.m_vItemDataArray.end(); ++Itr)
// 	{
// 		(*this).m_vItemDataArray.push_back((*Itr));
// 	}
}

int CCashShopData::LoadShopDataFromFile(CCashShopData &apShopData, const char* FileName)
{
	FILE *fp = NULL;

	if ((fp = fopen(FileName, "rb")) == NULL)
	{
		ASSERT("Catalog File is not Exist.");
		return -1;
	}

	int CashGoodsCount = 0;
	int iLength = 0;
	int iReadBytes = 0;

	iReadBytes = fread(&CashGoodsCount, sizeof(int), 1, fp); // 상품 총 갯수

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION  
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	for (int i=0; i<CashGoodsCount; ++i)
	{
		int iRefItemIndex = 0; // 참조할 아이템(아이콘 정보)
		char name[1024];
		char desc[4096];

		CASH_SHOP_DATA GoodsData;
		LOADINT(GoodsData.m_shopItemIndex);
		LOADINT(GoodsData.m_type); // 이것은 서버로 부터 받을 데이터.. 추후 lod에서 삭제
		LOADINT(GoodsData.m_cash);
		LOADINT(GoodsData.m_mileage);
		LOADINT(GoodsData.m_flag);
		LOADCHAR(GoodsData.m_enable);

		memset(name,'\0',sizeof(name));
		LOADSTR(name);
		GoodsData.m_itemName = name;
		memset(desc,'\0',sizeof(desc));
		LOADSTR(desc);
		GoodsData.m_itemDesc = desc;

		LOADINT(GoodsData.m_itemListCnt);
		
		if (GoodsData.m_itemListCnt > 0)
		{
			for (int i=0; i<GoodsData.m_itemListCnt; ++i)
			{
				ITEM_DATA tmpData;
				LOADINT(tmpData.m_itemIndex);
				LOADINT(tmpData.m_flag);
				LOADINT(tmpData.m_plus);
				LOADINT(tmpData.m_option);
				LOADINT(tmpData.m_itemCnt);
				GoodsData.m_vItemDataArray.push_back(tmpData);
			}
		}

		LOADINT(iRefItemIndex);
		if (iRefItemIndex > 0)
		{
			CItemData* pItem =_pNetwork->GetItemData(iRefItemIndex);

			if (pItem)
			{
				GoodsData.m_texID = pItem->GetIconTexID();
				GoodsData.m_texRow = pItem->GetIconTexRow();
				GoodsData.m_texCol = pItem->GetIconTexCol();
			}
		}

		apShopData.m_vShopItemArray.push_back(GoodsData);
	}

	fclose(fp);

//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR

	return CashGoodsCount;
}
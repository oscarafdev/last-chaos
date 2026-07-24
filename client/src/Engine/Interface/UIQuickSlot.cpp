#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UIChildQuickSlot.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UICashShopEX.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Entities/InternalClasses.h>

static BOOL m_bLock = false;

extern int _aSummonSkill[5];
extern INDEX g_iCountry;


// ----------------------------------------------------------------------------
// Name : CUIQuickSlot()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIQuickSlot::CUIQuickSlot()
{
	m_bHorizontal = TRUE;
	m_nCurrentPage = 0;
	m_strCurrentPage = CTString( "1" );
	m_strPageNumber = CTString( "" );
	m_nSelSlotBtnID = -1;
	m_bShowSlotInfo = FALSE;
	m_bShowExSlotInfo = FALSE;
	
	m_bGiftRecv =FALSE;
	m_eGiftPos =GIFTPOS_TOP;
	m_ptdGiftTexture = NULL;
	// [12/12/2012 박훈]
	m_nCurChildSlot = 0;
	for(int i = 0; i < QS_END; i++)
	{
		m_pChildQuickSlot[i] = NULL;
	}
	
	m_bIsInside = FALSE;
	m_bWearing = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIQuickSlot()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIQuickSlot::~CUIQuickSlot()
{
	Destroy();
	STOCK_RELEASE(m_ptdGiftTexture);
	
	for (int i = 0; i < QSLOT_PAGE_COUNT; ++i)
	{
		for (int j = 0; j < QSLOT_BTN_COUNT; ++j)
			SAFE_DELETE(m_abtnItems[i][j]);
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	// Region of each part
#define SHIFT_VAL 38
	m_rcTitle2.SetRect( 416+SHIFT_VAL, 0, 430+SHIFT_VAL, 44 );
	m_rcSlot.SetRect( 18, 0, QSLOT_RCSLOT_AREA , 43 );
	
	// Create quick slot texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	m_ptdGiftTexture = CreateTexture( CTString( "Data\\Interface\\Gift.tex" ) );
		
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	m_rtShop.SetUV(101, 585, 272, 675, fTexWidth, fTexHeight);
	//  [12/10/2012 박훈] UI개편 SlotNumber
	m_rtSlotNumberText.SetUV(120, 566, 512, 573, fTexWidth, fTexHeight);
	m_rtSlotNumberText1.SetUV(120, 574, 517, 581, fTexWidth, fTexHeight);
	m_rtSlotNumberText2.SetUV(120, 557, 526, 564, fTexWidth, fTexHeight);
	// UV Coordinate of each part
	// Background
	m_rtBackground.SetUV( 272, 631, 729, 675, fTexWidth, fTexHeight );
	m_strPageNumber.PrintF("%d", m_nCurrentPage + 1);
	
	// Slot information region
	m_rtSlotInfoL.SetUV( 239, 253, 246, 269, fTexWidth, fTexHeight );
	m_rtSlotInfoM.SetUV( 246, 253, 328, 269, fTexWidth, fTexHeight );
	m_rtSlotInfoR.SetUV( 328, 253, 335, 269, fTexWidth, fTexHeight );
	
	// [12/10/2012 박훈] 개편 UI 버튼
	m_rtPrevOver.SetUV( 630, 516, 643, 526, fTexWidth, fTexHeight );
	m_rtPrevClick.SetUV( 647, 516, 660, 526, fTexWidth, fTexHeight );
	
	m_rtNextOver.SetUV( 630, 527, 643, 537, fTexWidth, fTexHeight );
	m_rtNextClick.SetUV( 647, 527, 660, 537, fTexWidth, fTexHeight );
	
	m_rtAddSlotOver.SetUV( 596, 516, 609, 529, fTexWidth, fTexHeight );
	m_rtAddSlotClick.SetUV( 613, 516, 626, 529, fTexWidth, fTexHeight );
	
	m_rtDeleteSlotOver.SetUV( 596, 530, 609, 543, fTexWidth, fTexHeight );
	m_rtDeleteSlotClick.SetUV( 613, 530, 626, 543, fTexWidth, fTexHeight );
	
	// Outline of buttons
	m_rtBtnOutline.SetUV( 478,  30, 512, 64, fTexWidth, fTexHeight );
	
	// Quest Book Button
	m_rtQuestBook.SetUV( 387, 70, 420, 103, fTexWidth, fTexHeight );
	m_rtQuestBookDown.SetUV( 0, 0, 34, 34, fTexWidth, fTexHeight );

	if (g_iCountry == RUSSIA)
	{
		m_rtChashShopIdle.SetUV( 469, 679, 525, 735, fTexWidth, fTexHeight );
		m_rtCashShopOver.SetUV( 530, 679, 586, 735, fTexWidth, fTexHeight );
		m_rtCashShopDown.SetUV( 591, 679, 647, 735, fTexWidth, fTexHeight );
	}
	else
	{
		// [12/10/2012 박훈] UI개편으로 인한 캐쉬샵 수정
		m_rtChashShopIdle.SetUV( 102, 679, 158, 735, fTexWidth, fTexHeight );
		m_rtCashShopOver.SetUV( 163, 679, 219, 735, fTexWidth, fTexHeight );
		m_rtCashShopDown.SetUV( 224, 679, 280, 735, fTexWidth, fTexHeight );
	}	
	
	//선물 도착 알림 버튼 UV	:Su-won
	fTexWidth = m_ptdGiftTexture->GetPixWidth();
	fTexHeight = m_ptdGiftTexture->GetPixHeight();
	m_rtGift[GIFTPOS_TOP][0].SetUV( 138,11,176,49, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_TOP][1].SetUV( 138,50,176,88, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_LEFT][0].SetUV( 50,10,96,40, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_LEFT][1].SetUV( 50,50,96,80, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_BOTTOM][0].SetUV( 98,2,136,40, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_BOTTOM][1].SetUV( 98,42,136,80, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_RIGHT][0].SetUV( 2,10,48,40, fTexWidth, fTexHeight );
	m_rtGift[GIFTPOS_RIGHT][1].SetUV( 2,50,48,80, fTexWidth, fTexHeight );
	
	// Prev slot button
	m_btnPrevSlot.Create( this, CTString( "" ), QSLOT_PREV_BTN_SX, QSLOT_PREV_BTN_SY, 13, 10 );
	m_btnPrevSlot.CopyUV( UBS_ON, m_rtPrevOver );
	m_btnPrevSlot.CopyUV( UBS_CLICK, m_rtPrevClick );
	m_btnPrevSlot.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Next slot button
	m_btnNextSlot.Create( this, CTString( "" ), QSLOT_NEXT_BTN_SX, QSLOT_NEXT_BTN_SY, 13, 10 );
	m_btnNextSlot.CopyUV( UBS_ON, m_rtNextOver );
	m_btnNextSlot.CopyUV( UBS_CLICK, m_rtNextClick );
	m_btnNextSlot.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// [12/10/2012 박훈] 개편UI Add, Delete 버튼 
	// Add slot button
	m_btnAddSlot.Create( this, CTString( "" ), QSLOT_ADD_BTN_SX, QSLOT_ADD_BTN_SY, 13, 13 );
	m_btnAddSlot.CopyUV( UBS_ON, m_rtAddSlotOver );
	m_btnAddSlot.CopyUV( UBS_CLICK, m_rtAddSlotClick );      
	m_btnAddSlot.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Delete slot button
	m_btnDeleteSlot.Create( this, CTString( "" ), QSLOT_DELETE_BTN_SX, QSLOT_DELETE_BTN_SY, 13, 13 );
	m_btnDeleteSlot.CopyUV( UBS_ON, m_rtDeleteSlotOver );
	m_btnDeleteSlot.CopyUV( UBS_CLICK, m_rtDeleteSlotClick );
	m_btnDeleteSlot.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Quest Book
	m_btnQuestBook.Create( this, CTString(""), 387, 6, 32, 32 );
	m_btnQuestBook.CopyUV( UBS_IDLE, m_rtQuestBook );
	m_btnQuestBook.CopyUV( UBS_CLICK, m_rtQuestBookDown );
	m_btnQuestBook.CopyUV( UBS_IDLE, UBS_ON );
	m_btnQuestBook.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// [12/12/2012 박훈] shop 버튼을 퀵슬롯과 같이 그리지 않았음( quickslot과 shop 따로 그림 )
	m_btnCashShop.Create( this, CTString(""), -87, m_nHeight - 54, 54, 54 );
	m_btnCashShop.CopyUV( UBS_IDLE, m_rtChashShopIdle );
	m_btnCashShop.CopyUV( UBS_ON, m_rtCashShopOver );
	m_btnCashShop.CopyUV( UBS_CLICK, m_rtCashShopDown );
	
	//선물 도착 알림 버튼	:Su-won
	m_btnGiftNotice.Create( this, CTString( "" ), m_btnCashShop.GetPosX(), m_btnCashShop.GetPosY()-64, 
		QSLOT_GIFT_BTN_X, QSLOT_GIFT_BTN_Y+QSLOT_GIFT_BTN_ARROW);
	m_btnGiftNotice.CopyUV( UBS_IDLE, m_rtGift[GIFTPOS_TOP][0] );
	m_btnGiftNotice.CopyUV( UBS_CLICK, m_rtGift[GIFTPOS_TOP][1] );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Buttons of quick slot
	int	iPage, iBtn;
	int	nPosX;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		nPosX = QSLOT_BTN_SX;
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
		{
			m_abtnItems[iPage][iBtn] = new CUIIcon;
			m_abtnItems[iPage][iBtn]->Create(NULL, m_nPosX + nPosX, m_nPosY + QSLOT_BTN_SY, BTN_SIZE, BTN_SIZE,
                                 UI_QUICKSLOT, UBET_SKILL, (iPage * QSLOT_BTN_COUNT) + iBtn);
		}
	}
	
	// [12/12/2012 박훈] UI 개편
	m_pChildQuickSlot[QS_1] = CUIManager::getSingleton()->GetUI(UI_QUICKSLOT2);
	m_pChildQuickSlot[QS_2] = CUIManager::getSingleton()->GetUI(UI_QUICKSLOT3);
	((CUIChildQuickSlot*)m_pChildQuickSlot[QS_1])->SetItem(&m_abtnItems);
	((CUIChildQuickSlot*)m_pChildQuickSlot[QS_2])->SetItem(&m_abtnItems);
	
	m_btnRotate.SetEnable(FALSE);					// Button for rotating this UI
	m_btnQuestBook.SetEnable(FALSE);				// Button for Quest Book

	SetDisableCheckSlotEX();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - m_nWidth, pixMaxJ - m_nHeight );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ShowSlotInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ShowExSlotInfo( BOOL bShowInfo, int nSlotIndex )
{
	// Hide slot information
	if( !bShowInfo || nSlotIndex < 0 )
	{
		m_bShowExSlotInfo = FALSE;
		return;
	}
	
	BOOL	bUpdateInfo = FALSE;
	int		nInfoPosX, nInfoPosY;
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	// Update slot information
	{
		m_bShowExSlotInfo = TRUE;
		bUpdateInfo = TRUE;
		
		// Get slot information
		switch( nSlotIndex )
		{
		case 0:
			{
				m_btnCashShop.GetAbsPos( nInfoPosX, nInfoPosY );
				m_strSlotInfo = _S(99,"퀘스트");					
			}
			break;
			
		case 1:
			{
				m_btnCashShop.GetAbsPos( nInfoPosX, nInfoPosY );
				m_strSlotInfo = _S(2572,"아이리스 상점");
			}
			break;
			
			//선물 도착 알림 :Su-won
		case 2:
			{
				m_btnGiftNotice.GetAbsPos( nInfoPosX, nInfoPosY );
				m_strSlotInfo = _S(3112, "선물 도착");
			}
			break;
			//이전 페이지 버튼
		case 3:
			{
				m_btnPrevSlot.GetAbsPos( nInfoPosX, nInfoPosY );
				int PrevPage = m_nCurrentPage>0 ? m_nCurrentPage : 3 ;
				CTString str;
				str.PrintF("%d %s(Alt+%d)", PrevPage, _S(3076, "페이지"), PrevPage);
				m_strSlotInfo =str;
				
			}
			break;
			//다음 페이지 버튼
		case 4:
			{
				m_btnNextSlot.GetAbsPos( nInfoPosX, nInfoPosY );
				int NextPage = (m_nCurrentPage+2)<=3 ? m_nCurrentPage+2 : 1;
				CTString str;
				str.PrintF("%d %s(Alt+%d)", NextPage, _S(3076, "페이지"), NextPage);
				m_strSlotInfo =str;
				
			}
			break;	
		}
		
		// NOTE : 툴팁 표시할때는 QUICKSLOT을 제일 위로 올라오게 함.
		pUIManager->RearrangeOrder( UI_QUICKSLOT, TRUE );
	}
	
	// Update slot information box
	if( bUpdateInfo )
	{
		int nInfoWidth;
#if defined(G_THAI)
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strSlotInfo);		
#else
		if (g_iCountry == RUSSIA)
		{
			extern CFontData *_pfdDefaultFont;
			nInfoWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, m_strSlotInfo.str_String) + 19;
		}
		else
		{
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strSlotInfo.Length() *
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		}
#endif
		int	nInfoHeight = 22;
		
		nInfoPosX += ( BTN_SIZE - nInfoWidth ) / 2;
		
		if( nInfoPosX < pUIManager->GetMinI() )
			nInfoPosX = pUIManager->GetMinI();
		else if( nInfoPosX + nInfoWidth > pUIManager->GetMaxI() )
			nInfoPosX += pUIManager->GetMaxI() - ( nInfoPosX + nInfoWidth );
		
		if( nInfoPosY - nInfoHeight < pUIManager->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE + 2;
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}
	
	if( !m_bShowExSlotInfo )
	{
		m_bShowExSlotInfo = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : RenderBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RenderBtns()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	
	// Outline of buttons
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	int	iPage, iBtn;
	int	nPosX;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		nPosX = QSLOT_BTN_SX;
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
		{
			m_abtnItems[iPage][iBtn]->SetPos( m_nPosX + nPosX, m_nPosY + QSLOT_BTN_SY);
		}
	}
	
	m_btnQuestBook.Render();
	
	RenderGiftButton();
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
	{
		if (m_abtnItems[m_nCurrentPage][iBtn]->IsEmpty())
			continue;
		
		m_abtnItems[m_nCurrentPage][iBtn]->Render(pDrawPort);
	}
		
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	//  [12/10/2012 박훈] 개편 UI
	int posX = m_nPosX + 20;
	int posY = m_nPosY + 8;
	switch(m_nCurrentPage)
	{
	case 0:
		{
			pDrawPort->AddTexture( posX, posY, posX + 392, posY + 7,
				m_rtSlotNumberText.U0, m_rtSlotNumberText.V0, m_rtSlotNumberText.U1, m_rtSlotNumberText.V1,
				0xFFFFFFFF );
		}
		break;
		
	case 1:
		{
			pDrawPort->AddTexture( posX, posY, posX + 398, posY + 7,
				m_rtSlotNumberText1.U0, m_rtSlotNumberText1.V0, m_rtSlotNumberText1.U1, m_rtSlotNumberText1.V1,
				0xFFFFFFFF );
		}
		break;
		
	case 2:
		{
			pDrawPort->AddTexture( posX, posY, posX + 406, posY + 7,
				m_rtSlotNumberText2.U0, m_rtSlotNumberText2.V0, m_rtSlotNumberText2.U1, m_rtSlotNumberText2.V1,
				0xFFFFFFFF );
		}
		break;
	}
	
	posX = m_nPosX - 171;
	posY = (m_nPosY + m_nHeight) - 90;
	pDrawPort->AddTexture(posX, posY, posX + 171, posY + 90
		, m_rtShop.U0, m_rtShop.V0
		, m_rtShop.U1, m_rtShop.V1, 0xFFFFFFFF );
	m_btnCashShop.Render();
	
	// Slot information
	if( /*m_bShowSlotInfo ||*/ m_bShowExSlotInfo)
	{
		pDrawPort->AddTexture( m_rcSlotInfo.Left, m_rcSlotInfo.Top,
			m_rcSlotInfo.Left + 7, m_rcSlotInfo.Bottom,
			m_rtSlotInfoL.U0, m_rtSlotInfoL.V0, m_rtSlotInfoL.U1, m_rtSlotInfoL.V1,
			0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSlotInfo.Left + 7, m_rcSlotInfo.Top,
			m_rcSlotInfo.Right - 7, m_rcSlotInfo.Bottom,
			m_rtSlotInfoM.U0, m_rtSlotInfoM.V0, m_rtSlotInfoM.U1, m_rtSlotInfoM.V1,
			0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcSlotInfo.Right - 7, m_rcSlotInfo.Top,
			m_rcSlotInfo.Right, m_rcSlotInfo.Bottom,
			m_rtSlotInfoR.U0, m_rtSlotInfoR.V0, m_rtSlotInfoR.U1, m_rtSlotInfoR.V1,
			0xFFFFFFFF );
		
		// Render all elements
		pDrawPort->FlushRenderingQueue();
		
		// Render item information
		pDrawPort->PutTextEx( m_strSlotInfo, m_rcSlotInfo.Left + 8, m_rcSlotInfo.Top + 3 );
		
		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	else
	{
		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	// Set quick slot texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	if( m_bHorizontal )
	{
		pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
			m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
			0xFFFFFFFF );
	}
	else
	{
		pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
			m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V0,
			m_rtBackground.U1, m_rtBackground.V1, m_rtBackground.U0, m_rtBackground.V1,
			0xFFFFFFFF );
	}
	
	m_strPageNumber.PrintF("%d", m_nCurrentPage + 1);
	
	// 퀵슬롯 페이지 출력 위치를 수동으로 계산 [12/20/2012 Ranma]
	pDrawPort->PutTextExCX(m_strPageNumber, m_nPosX + m_nWidth - 10, m_nPosY + m_nHeight - 27 );
	
	// Prev button
	m_btnPrevSlot.Render();
	
	// Next button
	m_btnNextSlot.Render();
	
	// [12/10/2012 박훈] Add, Delete Button 
	m_btnAddSlot.Render();
	m_btnDeleteSlot.Render();
	
	// Rotate button
	//m_btnRotate.Render();
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
	
	// Buttons
	RenderBtns();
}

// ----------------------------------------------------------------------------
// Name : RemoveSummonSkill()
// Desc :
// ----------------------------------------------------------------------------

void CUIQuickSlot::ResetQuickSLotSKill()
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			// 스킬이 아니면 
			if (m_abtnItems[iPage][iBtn]->getBtnType() != UBET_SKILL)
				continue;

// 			int iSkillIndex = m_abtnItems[iPage][iBtn].GetSkillIndex();
// 			CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
// 
// 			// 와이드 펫 스킬이면 제외
// 			if(rSelSkill.GetJob() == WILDPET_JOB)
// 				continue;
// 			
// 			// 펫 스킬일 경우 제외
// 			if( rSelSkill.GetType() == CSkill::ST_PET_SKILL_ACTIVE || 
// 				rSelSkill.GetType() == CSkill::ST_PET_COMMAND )
// 				continue;

			m_abtnItems[iPage][iBtn]->clearIconData();
		}
	}
}

void CUIQuickSlot::RemoveSummonSkill()
{
	const int iSummonSkillCount = sizeof( _aSummonSkill ) / sizeof(int);
	
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if (m_abtnItems[iPage][iBtn]->getBtnType() == UBET_SKILL)
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn]->getIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
				
				// 소환스킬 인덱스 찾구,
				// 소환수 사용 스킬.
				if( rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) )
				{			
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}
				else
				{
					for( int i = 0; i < iSummonSkillCount; ++i )
					{
						if( _aSummonSkill[i] == iSkillIndex )
						{					
							// Remove button
							SendAddBtn( iPage, iBtn, -1 );			
						}
					}
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RemoveElementalSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RemoveElementalSkill()
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if (m_abtnItems[iPage][iBtn]->getBtnType() == UBET_SKILL)
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn]->getIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
				
				// 소환스킬 인덱스 찾구,
				// 소환수 사용 스킬.
				if( rSelSkill.GetSorcererFlag() & ( SSF_USE_HELLOUND | SSF_USE_ELENEN ) )
				{
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}				
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RemovePetSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RemovePetSkill()
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if (m_abtnItems[iPage][iBtn]->getBtnType() == UBET_SKILL)
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn]->getIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
				
				// 애완동물 스킬을 찾아서 제거.
				if( rSelSkill.GetType() == CSkill::ST_PET_SKILL_ACTIVE || 
					rSelSkill.GetType() == CSkill::ST_PET_COMMAND )
				{
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}				
			}
		}
	}
}

void CUIQuickSlot::RemoveWildPetSkill()
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if (m_abtnItems[iPage][iBtn]->getBtnType() == UBET_SKILL)
			{				
				const int iSkillIndex = m_abtnItems[iPage][iBtn]->getIndex();
				CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
				
				// 애완동물 스킬을 찾아서 제거.
				if(rSelSkill.GetJob() == WILDPET_JOB)
				{
					// Remove button
					SendAddBtn( iPage, iBtn, -1 );			
				}				
			}
		}
	}
}



// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIQuickSlot::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL bTitleBarClick = FALSE;
	
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	m_bShowExSlotInfo = FALSE;
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			m_bIsInside = IsInside( nX, nY );
			if (m_bIsInside)
			{
				for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
				{
					int nPosX = QSLOT_BTN_SX;
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
					{
						m_abtnItems[iPage][iBtn]->SetPos( m_nPosX + nPosX, m_nPosY + QSLOT_BTN_SY);
					}
				}
			}
			
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Hold button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if( m_nSelSlotBtnID >= 0 )
				{
					ndX = abs( ndX );
					ndY = abs( ndY );
					
					static int nD = 7;
					
					CPrintF( "%d, %d", ndX, ndY );
					if( ndX > nD || ndY > nD )
					{
						int	nSelTab = m_nSelSlotBtnID / QSLOT_BTN_COUNT;
						int	nSelBtn = m_nSelSlotBtnID % QSLOT_BTN_COUNT;
						
						pUIManager->SetHoldBtn( m_abtnItems[nSelTab][nSelBtn] );
						bLButtonDownInBtn = FALSE;
					}
				}
				
			}
			
			// Move quick slot
			// [12/11/2012 박훈] UI개편으로 인해 1번째 Quickslot은 이동이 불가
			//if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			//{
			//nOldX = nX;	nOldY = nY;
			
			//	Move( ndX, ndY );
			
			//	SetGiftNoticePos();
			
			//	return WMSG_SUCCESS;
			//}
			// Prev button
			else if( m_btnPrevSlot.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// [070604: Su-won] 1차 펫 기능 개선
				// 퀵슬롯 이전 페이지 버튼 단축키 표시
				ShowExSlotInfo(TRUE, 3);
				return WMSG_SUCCESS;
			}
			// Next button
			else if( m_btnNextSlot.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// [070604: Su-won] 1차 펫 기능 개선
				// 퀵슬롯 다음 페이지 버튼 단축키 표시
				ShowExSlotInfo(TRUE, 4);
				return WMSG_SUCCESS;
			}
			// [12/10/2012 박훈] Add, Delete Button
			else if( m_btnAddSlot.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			else if( m_btnDeleteSlot.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			
			// Rotate button
			else if( m_btnRotate.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Slot
			/*	else if( IsInsideRect( nX, nY, m_rcSlot ) )
			{
			int	nSlotIndex = -1;
			for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
			{
			if( m_abtnItems[m_nCurrentPage][iBtn].MouseMessage( pMsg ) == WMSG_FAIL )
			{
			if(m_abtnItems[m_nCurrentPage][iBtn].GetShowInfo())
			{
			m_abtnItems[m_nCurrentPage][iBtn].SetShowInfo(FALSE);
			}
			//nSlotIndex = iBtn;
			}
			}
			
			  // Show tool tip
			  //ShowSlotInfo( TRUE, nSlotIndex );
			  
				return WMSG_SUCCESS;
			}*/
			// Show cash shop info
			else if( m_btnQuestBook.MouseMessage(pMsg) !=WMSG_FAIL ){
				// Show tool tip
				ShowExSlotInfo( TRUE, 0 );
				return WMSG_SUCCESS;
			}
			else if( m_btnCashShop.MouseMessage(pMsg) !=WMSG_FAIL ){
				// Show tool tip
				ShowExSlotInfo( TRUE, 1 );
				return WMSG_SUCCESS;
			}
			
			//Show Gift Recv info		:Su-won
			else if( m_btnGiftNotice.MouseMessage(pMsg) !=WMSG_FAIL )
			{
				// Show tool tip
				if( m_bGiftRecv )
					ShowExSlotInfo( TRUE, 2);
				return WMSG_SUCCESS;
			}
			
			for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
			{
				if (m_abtnItems[m_nCurrentPage][iBtn]->MouseMessage( pMsg ) != WMSG_FAIL)
					return WMSG_SUCCESS;
			}
		}
		break;
		
	case WM_LBUTTONDOWN:
		{
			//선물 도착 알림 버튼	:Su-won
			if( m_btnGiftNotice.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			
			m_bIsInside = IsInside( nX, nY );

			m_nSelSlotBtnID = -1;

			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				nOldX = nX;		nOldY = nY;
				
				// Title bar 1
				// [12/10/2012 박훈] UI개편으로 인해 DragArea는 필요 없어짐
				//if( IsInsideRect( nX, nY, m_rcTitle1 ) )
				//{
				//	bTitleBarClick = TRUE;
				//}
				// Prev button
				if( m_btnPrevSlot.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Next button
				else if( m_btnNextSlot.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnQuestBook.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//Nothing
				}
				// wooss 050815
				
				// [12/10/2012 박훈] Add, Delete Button
				else if( m_btnAddSlot.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//Nothing
				}
				else if( m_btnDeleteSlot.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//Nothing
				}
				// Slot
				else if( IsInsideRect( nX, nY, m_rcSlot ) )
				{
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
					{
						if (m_abtnItems[m_nCurrentPage][iBtn]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Update selected button
							m_nSelSlotBtnID = iBtn + m_nCurrentPage * QSLOT_BTN_COUNT;
							
							bLButtonDownInBtn = TRUE;
							
							pUIManager->RearrangeOrder( UI_QUICKSLOT, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				
				pUIManager->RearrangeOrder( UI_QUICKSLOT, TRUE );
				return WMSG_SUCCESS;
			}
			if( m_btnCashShop.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
		}
		break;
		
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			bLButtonDownInBtn = FALSE;
			
			m_bIsInside = IsInside( nX, nY );
			if (m_bIsInside)
			{
				for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
				{
					int nPosX = QSLOT_BTN_SX;
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
					{
						m_abtnItems[iPage][iBtn]->SetPos( m_nPosX + nPosX, m_nPosY + QSLOT_BTN_SY);
					}
				}
			}
			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;
				
				// If quick slot isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Prev button
				if( ( wmsgResult = m_btnPrevSlot.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nCurrentPage--;
						if( m_nCurrentPage < 0 )
							m_nCurrentPage = QSLOT_PAGE_COUNT - 1;
						m_strCurrentPage.PrintF( "%d", m_nCurrentPage + 1 );
					}
					
					return WMSG_SUCCESS;
				}
				// Next button
				else if( ( wmsgResult = m_btnNextSlot.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nCurrentPage++;
						if( m_nCurrentPage >= QSLOT_PAGE_COUNT )
							m_nCurrentPage = 0;
						m_strCurrentPage.PrintF( "%d", m_nCurrentPage + 1 );
					}
					
					return WMSG_SUCCESS;
				}
				
				// [12/10/2012 박훈] Add, Delete Button Event Handling
				else if( ( wmsgResult = m_btnAddSlot.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// 이벤트 처리
					CheckSlotEXBtn(QSLOT_ADD);
					SetDisableCheckSlotEX();
					return WMSG_SUCCESS;
				}
				else if ( ( wmsgResult = m_btnDeleteSlot.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// 이벤트 처리
					CheckSlotEXBtn(QSLOT_DEL);
					SetDisableCheckSlotEX();
					return WMSG_SUCCESS;
				}
				else if( (wmsgResult = m_btnQuestBook.MouseMessage( pMsg )) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						//TODO : NewQuestSystem
						//서버에서 목록을 받아야 하나?
						pUIManager->GetQuestBookList()->ToggleVisible();
					}
					
					return WMSG_SUCCESS;
				}
				// wooss 050815
				// Cash Shop Button 
				else if( (wmsgResult = m_btnCashShop.MouseMessage( pMsg )) != WMSG_FAIL)
				{
					// 테스트서버에는 상점을 열지 않는다
					extern UINT g_uiEngineVersion;
					if( wmsgResult == WMSG_COMMAND ) //&& g_uiEngineVersion < 10000)
					{
						if (!pUIManager->GetCashShopEX()->IsVisible())
						{
							if (pUIManager->GetCashShopEX()->GetCashShopLock())
							{
								pUIManager->GetCashShopEX()->Message(MSGCMD_GIFT_ERROR, _S(372, "점검"), _S(5296, "아이리스 상점이 일시 점검 중입니다. 잠시 후에 이용해 주십시오."), UMBS_OK);
							}
							else
							{
								if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsHudModel_Used() == TRUE )
									return WMSG_SUCCESS;

								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_SHOP_OPEN_REQ);
							}
						}
					}
					
					return WMSG_SUCCESS;
				}
				
				// 선물 도착 알림 버튼 클릭	:Su-won
				else if( (wmsgResult = m_btnGiftNotice.MouseMessage( pMsg )) != WMSG_FAIL )
				{
					//상점창 열기
					if( m_bGiftRecv )
					{
						if (!pUIManager->GetCashShopEX()->IsVisible())
						{
							if (pUIManager->GetCashShopEX()->GetCashShopLock())
							{
								pUIManager->GetCashShopEX()->Message(MSGCMD_GIFT_ERROR, _S(372, "점검"), _S(5296, "아이리스 상점이 일시 점검 중입니다. 잠시 후에 이용해 주십시오."), UMBS_OK);
							}
							else
							{
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_SHOP_OPEN_REQ);
							}
						}

						return WMSG_SUCCESS;
					}
				}
				
				// Slot
				else if( IsInsideRect( nX, nY, m_rcSlot ) )
				{
					m_nSelSlotBtnID = -1;
					
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
					{
						if ((wmsgResult = m_abtnItems[m_nCurrentPage][iBtn]->MouseMessage(pMsg)) != WMSG_FAIL)
						{
							if (wmsgResult == WMSG_SUCCESS)
								UseQuickSlot( iBtn, m_nCurrentPage );
							
							return WMSG_SUCCESS;
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					pUIManager->GetMessenger()->SetDragging(false);

					CUIIcon* pDrag = pUIManager->GetDragIcon();					
					
					// If holding button comes from quick slot
					if (pDrag->GetWhichUI() == UI_QUICKSLOT)
					{
						if( IsInsideRect( nX, nY, m_rcSlot ) )
						{
							for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
							{
								if (m_abtnItems[m_nCurrentPage][iBtn]->IsInside( nX, nY ) &&
									m_abtnItems[m_nCurrentPage][iBtn]->GetQuickSlotID() != pDrag->GetQuickSlotID())
								{
									int nBtnID = pDrag->GetQuickSlotID();

									int	nPageIndex = nBtnID / QSLOT_BTN_COUNT;
									int	nSlotIndex = nBtnID % QSLOT_BTN_COUNT;
									
									SwapBtnsInQuickSlot( nPageIndex, nSlotIndex,  m_nCurrentPage, iBtn );
									break;
								}
							}							
						}
					}
					// If holding button is skill and comes from skill window
					// If holding button is action and comes from action window
					// If holding button is item and comes from inventory
					else if ((pDrag->getBtnType() == UBET_SKILL &&
							 (pDrag->GetWhichUI() == UI_SKILL_NEW || pDrag->GetWhichUI() == UI_CHARACTERINFO)) ||
							 (pDrag->getBtnType() == UBET_SKILL &&
							  pDrag->GetWhichUI() == UI_PETINFO) ||
							 (pDrag->getBtnType() == UBET_ACTION &&
							  pDrag->GetWhichUI() == UI_CHARACTERINFO) ||
							 (pDrag->getBtnType() == UBET_ITEM &&
							  pDrag->GetWhichUI() == UI_INVENTORY) ||
							 (pDrag->getBtnType() == UBET_SKILL &&
							  pDrag->GetWhichUI() == UI_WILDPET_INFO) ||
							 (pDrag->getBtnType() == UBET_SKILL &&
							  pDrag->GetWhichUI() == UI_GUILD))
					{
						if( IsInsideRect( nX, nY, m_rcSlot ) )
						{
							if (pDrag->getBtnType() == UBET_ITEM)
							{
								CItems* pItems = pDrag->getItems();

								if (pItems == NULL)
									return WMSG_FAIL;

								CItemData* pItemData = pItems->ItemData;
								
								if( (pItemData->GetType() == CItemData::ITEM_ETC 					//Etc(x)
									// [070604: Su-won] 1차 펫 기능 개선
									// 생산품을 퀵슬롯에 넣을 수 있게...
									&&pItemData->GetSubType() !=CItemData::ITEM_ETC_PRODUCT
									) ||
									(pItemData->GetType() == CItemData::ITEM_ACCESSORY				// Accessory(x)
									// [070604: Su-won] 1차 펫 기능 개선
									// 펫을 퀵슬롯에 넣을 수 있게...
									&& pItemData->GetSubType() !=CItemData::ACCESSORY_PET
									&& pItemData->GetSubType() !=CItemData::ACCESSORY_WILDPET
									&& pItemData->GetSubType() != CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD
									) ||
									( pItemData->GetType() == CItemData::ITEM_BULLET &&				// Bullet - arrow(x)
									pItemData->GetSubType() == CItemData::ITEM_BULLET_ARROW )
									)
									
								{
									// Reset holding button
									pUIManager->ResetHoldBtn();
									
									return WMSG_SUCCESS;
								}
							}
							
							for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
							{
								if (m_abtnItems[m_nCurrentPage][iBtn]->IsInside(nX, nY))
								{
									AddBtnToQuickSlot( iBtn, m_nCurrentPage );
									
									break;
								}
							}
						}
					}
					
					// Reset holding button
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
			
	case WM_LBUTTONDBLCLK:
		{
			m_bIsInside = IsInside( nX, nY );
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
		}
		break;
	}
	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : UseQuickSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::UseQuickSlot( int nSlotIndex, int nCurrentPage)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	// if activate laca ball, disable quick slot.
	if( pUIManager->GetLacaBall()->IsVisible() )
		return;
	
	// If slot is empty
	if (m_abtnItems[nCurrentPage][nSlotIndex]->IsEmpty())
		return;
	
	// Use quick slot
	switch (m_abtnItems[nCurrentPage][nSlotIndex]->getBtnType())
	{
	case UBET_SKILL:
		{
			if (pUIManager->IsCSFlagOn(CSF_SKILLREADY))
			{
				return;
			}

			int		nSkillIndex = m_abtnItems[nCurrentPage][nSlotIndex]->getIndex();
			BOOL	bDelay = MY_INFO()->GetSkillDelay(nSkillIndex, m_abtnItems[nCurrentPage][nSlotIndex]->IsSpecial());
			
			if (bDelay == TRUE)
				return;
			
			if (_pNetwork->GetSkillData( nSkillIndex ).GetType() == CSkill::ST_PET_SKILL_ACTIVE )
			{
				pUIManager->GetPetInfo()->UseSkill( nSkillIndex );					
			}
			else if (_pNetwork->GetSkillData( nSkillIndex ).GetType() == CSkill::ST_PET_COMMAND )
			{
				pUIManager->GetPetInfo()->UseCommand( nSkillIndex );
			}
			else if (_pNetwork->GetSkillData(nSkillIndex).GetJob() == WILDPET_JOB)
			{
				pUIManager->GetWildPetInfoUI()->UseSkill(nSkillIndex);
			}
			else
			{
				if (_pNetwork->GetSkillData( nSkillIndex ).GetFlag() & SF_GUILD)
				{	// 길드 스킬일 경우, 사용 조건 체크
					if ( !pUIManager->GetGuild()->CheckUseGuildSkill( nSkillIndex ))
					{	// 조건 충족을 못할 경우 리턴
						return;
					}
				}

				pUIManager->GetCharacterInfo()->UseSkill( nSkillIndex );
			}
		}
		break;
		
	case UBET_ACTION:
		{
			SQUAD llTerm = _pTimer->GetHighPrecisionTimer().GetMilliseconds() - m_abtnItems[nCurrentPage][nSlotIndex]->GetUseItemStartTime();
			
			if( llTerm > 500 )
			{
				m_abtnItems[nCurrentPage][nSlotIndex]->SetUseItemStartTime( _pTimer->GetHighPrecisionTimer().GetMilliseconds() );
			}
			else 
			{ return; }// 0.5초 사이에 같은 버튼을 누른다면 return;
			
			int	nActionIndex = m_abtnItems[nCurrentPage][nSlotIndex]->getIndex();
			pUIManager->GetCharacterInfo()->UseAction( nActionIndex );
		}
		break;
		
	case UBET_ITEM:
		{
			if( pUIManager->IsCSFlagOn( CSF_WAREHOUSE ) )
				return;		

			int nItemIdx = m_abtnItems[nCurrentPage][nSlotIndex]->getIndex();

			CItems* pItems = m_abtnItems[nCurrentPage][nSlotIndex]->getItems();

			if (pItems == NULL)
				return;

			CItemData*	pItemData = pItems->ItemData;

			if(pItemData == NULL)
				return;

			if ( m_bWearing == TRUE )
			{	
				if (pItemData->GetType() == CItemData::ITEM_WEAPON ||
					pItemData->GetType() == CItemData::ITEM_SHIELD ||
					pItemData->GetType() == CItemData::ITEM_ACCESSORY )
				{
					_pNetwork->ClientSystemMessage( _S( 305, "장비를 착용할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}				
			}
			
			SQUAD llTerm = _pTimer->GetHighPrecisionTimer().GetMilliseconds() - MY_INFO()->GetUseItem(nItemIdx);
			
			if( llTerm > 500 )
			{
				MY_INFO()->SetUseItem(nItemIdx, _pTimer->GetHighPrecisionTimer().GetMilliseconds());
			}
			else 
			{ return; }// 0.5초 사이에 같은 버튼을 누른다면 return;

			// 퀘스트 아이템 사용제한 체크
			if (m_abtnItems[nCurrentPage][nSlotIndex]->IsRestritionItem() == true)
			{
				CTString strTitle;
				CTString strMessage1;
				CUIMsgBox_Info	MsgBoxInfo;

				strTitle	=	_S(191,"확인");
				strMessage1	=	_S( 4428, "여기서는 사용 할 수 없습니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage1);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				return;
			}
						
			if (m_abtnItems[nCurrentPage][nSlotIndex]->IsWearTab() == false)
			{
				int	nTab = pItems->Item_Tab;
				int	nIdx = pItems->InvenIndex;

				pUIManager->GetInventory()->SetUseItemSlotInfo(nTab, nIdx);
				pUIManager->GetInventory()->SendUseSlotItem(nTab, nIdx);
			}
			else
			{
				int	nType = m_abtnItems[nCurrentPage][nSlotIndex]->GetQuickSlotWearType();
				int nWear = pItems->Item_Wearing;
				int	nIdx = pItems->InvenIndex;
				int vIdx = pItems->Item_UniIndex;

				switch (nType)
				{
				case 0:
					_pNetwork->SendItemWearingMSG(MSG_ITEM_WEAR_TAKE_OFF, 
						nWear, -1, -1, 0);
					break;
				case 1:
					_pNetwork->SendItemWearingMSG(MSG_ITEM_WEAR_COSTUME_TAKEOFF,
						nWear, -1, -1, 0, vIdx);	
					break;
				case 2:
					_pNetwork->SendItemWearingMSG(MSG_ITEM_WEAR_COSTUME_SUIT_TAKEOFF,
						nWear, -1, -1, 0, vIdx);
					break;
				}				
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : AddBtnToQuickSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::AddBtnToQuickSlot( int nSlotIndex, int nCurrentPage )
{
	if( m_bLock == TRUE )
		return; // 현재 사용하지 않음
	
	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return;
	
	// Add button
	switch (pDrag->getBtnType())
	{
	case UBET_SKILL:
		{
			int	nSkillIndex = pDrag->getIndex();
			CSkill	&rSelSkill = _pNetwork->GetSkillData( nSkillIndex );
			if( !(rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) ) )
			{
				SendAddBtn( nCurrentPage, nSlotIndex, 0, nSkillIndex );
			}
			else
			{
				CTString strMessage = _S(2339,"소환수만 사용할 수 있는 스킬입니다.");
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
			}
		}
		break;
		
	case UBET_ACTION:
		{
			int	nActionIndex = pDrag->getIndex();

			if (g_iCountry == RUSSIA)
			{
				// [2011/05/18 : Sora] 러시아측 요청으로 P2줍기,공격 액션은 퀵슬롯에 등록되지 않게 처리
				if( nActionIndex == 44 /*P2 공격*/ || nActionIndex == 45 /*P2 줍기*/ )
				{
					return;
				}
			}
			
			SendAddBtn( nCurrentPage, nSlotIndex, 1, nActionIndex );
		}
		break;
		
	case UBET_ITEM:
		{
			CItems* pItems = pDrag->getItems();

			if (pItems == NULL)
				return;

			SWORD nTab, nIdx;
			int	nUniIndex = pItems->Item_UniIndex;
			pUIManager->GetInventory()->GetLocationOfNormalItem( nUniIndex, nTab, nIdx );
			
			if( nIdx != -1 )
				SendAddBtn( nCurrentPage, nSlotIndex, 2, nTab, nIdx );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SwapBtnsInQuickSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SwapBtnsInQuickSlot(int nPage1, int nSlot1, int nPage2, int nSlot2)
{

	SendSwapBtns(  nPage1, nSlot1, nPage2, nSlot2  );
}

// ----------------------------------------------------------------------------
// Name : RemoveBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::RemoveBtn( CUIIcon* pIcon )
{
	// Find button
	int	iPage, iBtn;
	for( iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			if (m_abtnItems[iPage][iBtn] == pIcon)
			{
				// Remove button
				SendAddBtn(iPage, iBtn, -1);
				return;
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ChangePage()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ChangePage( int nPage )
{
	m_nCurrentPage = nPage;
	m_strCurrentPage.PrintF( "%d", m_nCurrentPage + 1 );
}

// ----------------------------------------------------------------------------
// Name : StartSkillDelay()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIQuickSlot::StartSkillDelay( int nIndex )
{
	BOOL bResult = FALSE;
	for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( int iCol = 0; iCol < QSLOT_BTN_COUNT; iCol++ )
		{
			if (m_abtnItems[iPage][iCol]->getBtnType() == UBET_SKILL &&
				m_abtnItems[iPage][iCol]->getIndex() == nIndex)
			{				
				m_abtnItems[iPage][iCol]->setCooltime(true);
				bResult = TRUE;
			}
			else if (m_abtnItems[iPage][iCol]->getBtnType() == UBET_ITEM)
			{
				// [2010/10/20 : Sora] 사용 조건을 함수로 대체
				int nItemIndex = m_abtnItems[iPage][iCol]->getIndex();
				if( nItemIndex == -1 ) continue;
				
				CItemData* pItemData = _pNetwork->GetItemData( nItemIndex );
				
				if (pItemData->GetNum0() == nIndex)
				{
					m_abtnItems[iPage][iCol]->setCooltime(true);
					bResult = TRUE;
				}
			}
			
		}
	}
	
	return bResult;
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendAddBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SendAddBtn( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 )
{
	_pNetwork->AddQuickSlot( nPage, nSlotNum, nSlotType, nData0, nData1 );
}

// ----------------------------------------------------------------------------
// Name : SendSwapBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SendSwapBtns( int nPage1, int nSlot1, int nPage2, int nSlot2 )
{
	_pNetwork->SwapQuickSlot( nPage1, nSlot1, nPage2, nSlot2 );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : AddBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::AddBtn( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 )
{
	// 슬롯 초기화.
	if (nPage >= 0 && nPage < QSLOT_PAGE_COUNT &&
		nSlotNum >= 0 && nSlotNum < QSLOT_BTN_COUNT)
	{
		m_abtnItems[nPage][nSlotNum]->clearIconData();
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nSlotType )
	{
	case QUICKSLOT_TYPE_SKILL:
		{
			if (nPage < 0 || nPage >= QSLOT_PAGE_COUNT ||
				nSlotNum < 0 || nSlotNum >= QSLOT_BTN_COUNT)
				return;

			// 펫 액티브 스킬의 경우.
			if( _pNetwork->GetSkillData( nData0 ).GetType() == CSkill::ST_PET_SKILL_ACTIVE)
			{
				m_abtnItems[nPage][nSlotNum]->setSkill(nData0);

				if (MY_INFO()->GetPetSkillDelay(MY_PET_INFO()->lIndex, nData0) == true)
					m_abtnItems[nPage][nSlotNum]->setCooltime(true);
			}
			else if(_pNetwork->GetSkillData( nData0 ).GetJob() == WILDPET_JOB)
			{
				if(pUIManager->GetWildPetInfoUI()->IsLearnSkill(nData0))
				{
					m_abtnItems[nPage][nSlotNum]->setSkill(nData0);

					if (MY_INFO()->GetPetSkillDelay(0, nData0) == true)
						m_abtnItems[nPage][nSlotNum]->setCooltime(true);
				}
				else
				{
					SendAddBtn( nPage, nSlotNum, -1 );	
				}
			}
			else
			{
				SBYTE	sbSkillLevel;
				CSkill skillData = _pNetwork->GetSkillData( nData0 );
				if ( skillData.GetFlag() & SF_GUILD )
				{
					sbSkillLevel = pUIManager->GetGuild()->GetGuildSkillLevel(nData0);
				}
				else
				{
					sbSkillLevel = MY_INFO()->GetSkillLevel(nData0);
				}

				if (skillData.Skill_Data.index > 0)
					m_abtnItems[nPage][nSlotNum]->setSkill(nData0);

				if (MY_INFO()->GetSkillDelay(nData0) == true)
					m_abtnItems[nPage][nSlotNum]->setCooltime(true);
			}

			SetToggle(1, nData0, _pNetwork->GetSkillData( nData0 ).GetToggle());
		}
		break;
		
	case QUICKSLOT_TYPE_ACTION:
		{
			if (nPage >= 0 && nPage < QSLOT_PAGE_COUNT &&
				nSlotNum >= 0 && nSlotNum < QSLOT_BTN_COUNT)
			{
				m_abtnItems[nPage][nSlotNum]->setData(UBET_ACTION, nData0);
			}
		}
		break;
		
	case QUICKSLOT_TYPE_ITEM:
		{
			// 유효값 검사
			if (nData0 < 0 || nData0 >= INVEN_SLOT_TAB || 
				nData1 < 0 || nData1 >= ITEM_COUNT_IN_INVENTORY_NORMAL )
				return;

			//[100126 sora] 퀵슬롯에 아이템 옵션정보 표시
			CItems& rItems = _pNetwork->MySlotItem[nData0][nData1];

			if (rItems.Item_Index == -1)
				return;

			setQuickSlotItemData(nPage, nSlotNum, nData0, nData1, &rItems, false);
			SetToggle(1, rItems.Item_UniIndex, rItems.GetToggle());
		}
		break;
	case QUICKSLOT_TYPE_ITEM_WEAR:
		{
			CItems* pItem = NULL;

			if (nData0 == 0)
				pItem = &_pNetwork->MyWearItem[nData1];
			else if (nData0 == 1)
				pItem = &_pNetwork->MyWearCostItem[nData1];
			else if (nData0 == 2)
			{
				// 한벌의상
				//CItems Items(nData1);
				pItem = new CItems(nData1);

				pItem->Item_Index = nData1;

				setQuickSlotItemData(nPage, nSlotNum, nData0, 0, pItem, true, false);
				return;
			}
			
			setQuickSlotItemData(nPage, nSlotNum, nData0, nData1, pItem, true);
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SwapBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::SwapBtns( int nPage, int nSlotNum1, int nSlotNum2 )
{
	CUIIcon*	pIcon = new CUIIcon;
	pIcon->cloneItem( m_abtnItems[nPage][nSlotNum1] );
	m_abtnItems[nPage][nSlotNum1]->cloneItem(m_abtnItems[nPage][nSlotNum2]);
	m_abtnItems[nPage][nSlotNum2]->cloneItem(pIcon);
	SAFE_DELETE(pIcon);
}

// ----------------------------------------------------------------------------
// Name : ClearAllBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuickSlot::ClearAllBtns( int nPage)
{
	for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		m_abtnItems[nPage][iBtn]->clearIconData();
}

//선물 버튼 위치 설정	:Su-won
void CUIQuickSlot::SetGiftNoticePos()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	int Width = pDrawPort->GetWidth();
	int Height= pDrawPort->GetHeight();
	
	int nPosX, nPosY;
	int nAbsPosX, nAbsPosY;
				
	m_btnGiftNotice.GetPos(nPosX, nPosY);
	m_btnGiftNotice.GetAbsPos(nAbsPosX, nAbsPosY);
	
	if( m_bHorizontal )
	{
		m_btnGiftNotice.SetPos(nPosX, m_btnCashShop.GetPosY()-64);
	}
	else
	{
		if( nAbsPosX<0)
		{
			m_btnGiftNotice.SetPos(m_btnCashShop.GetPosX()+64, nPosY);
			m_eGiftPos =GIFTPOS_RIGHT;
		}
		if( nAbsPosX+QSLOT_GIFT_BTN_X+QSLOT_GIFT_BTN_ARROW>Width)
		{
			m_btnGiftNotice.SetPos(m_btnCashShop.GetPosX()-(QSLOT_GIFT_BTN_X+QSLOT_GIFT_BTN_ARROW), nPosY);
			m_eGiftPos =GIFTPOS_LEFT;
		}
	}
	
	m_btnGiftNotice.CopyUV( UBS_IDLE, m_rtGift[m_eGiftPos][0] );
	m_btnGiftNotice.CopyUV( UBS_CLICK, m_rtGift[GIFTPOS_TOP][0] );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
}

//선물 도착 알림 버튼 렌더링		:Su-won
void CUIQuickSlot::RenderGiftButton()
{
	if( m_bGiftRecv == FALSE )
		return;
	
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	pDrawPort->InitTextureData( m_ptdGiftTexture );
	
	// Timer for highlight buttons
	static BOOL		bHighlight = FALSE;
	static DOUBLE	dElapsedTime = 0.0;
	static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	dElapsedTime += dCurTime - dOldTime;
	dOldTime = dCurTime;
	if( dElapsedTime > 0.5 )
	{
		bHighlight = !bHighlight;
		m_btnGiftNotice.CopyUV( UBS_IDLE, m_rtGift[m_eGiftPos][bHighlight] );
		m_btnGiftNotice.CopyUV( UBS_CLICK, m_rtGift[m_eGiftPos][bHighlight] );
		m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_ON );
		m_btnGiftNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
		do
		{
			dElapsedTime -= 0.5;
		}
		while( dElapsedTime > 0.5 );
	}
	
	m_btnGiftNotice.Render();
	
	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : UpdateItemCount(int nUinIndex, SQUAD llCount)
// Desc : 퀵슬롯의 아이템 count update
// ----------------------------------------------------------------------------
void CUIQuickSlot::UpdateItemCount(int nUinIndex, SQUAD llCount)
{
	for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			CItems* pItems = m_abtnItems[iPage][iBtn]->getItems();

			if (pItems != NULL && pItems->Item_UniIndex == nUinIndex)
			{
				m_abtnItems[iPage][iBtn]->setCount(llCount);
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : IsInQuickSolt(UIBtnExType BtnType, int nIndex)
// Desc : 퀵슬롯에 등록이 되어 있는지 확인한다.
// ----------------------------------------------------------------------------
BOOL CUIQuickSlot::IsInQuickSolt(UIBtnExType BtnType, int nIndex)
{
	int iPage, iBtn;
	
	for (iPage = 0; iPage<QSLOT_PAGE_COUNT; ++iPage)
	{
		for (iBtn = 0; iBtn<QSLOT_BTN_COUNT; ++iBtn)
		{
			if (m_abtnItems[iPage][iBtn]->getBtnType() == BtnType &&
				m_abtnItems[iPage][iBtn]->getIndex() == nIndex)
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : ToggleVisibleEXQuickSlot()
// Desc : 확장 퀵 슬롯 버튼의 토글링 함수
// ----------------------------------------------------------------------------
void CUIQuickSlot::ToggleVisibleEXQuickSlot(BOOL bResultView)
{
	int nCurOpenUI = -1;
	
	if (bResultView)
	{
		nCurOpenUI = g_bQuickSlotEX1 >= 1 ? (g_bQuickSlotEX2 >= 1 ? -1 : 1) : 0;
	}
	else
	{
		nCurOpenUI = g_bQuickSlotEX1 >= 1 ? (g_bQuickSlotEX2 >= 1 ? 1 : 0) : -1;
	}
	
	
	if (nCurOpenUI <= -1)
		return;
	
	((CUIChildQuickSlot*)m_pChildQuickSlot[nCurOpenUI])->ToggleVisible(bResultView);
	
}

// ----------------------------------------------------------------------------
// Name : CheakSlotEXBtn
// Desc : 현재 퀵슬롯 다음을 체크 하여 상태를 변경해준다. 확장버튼의 상태를 리턴 한다 맥스 인지 아닌지
// ----------------------------------------------------------------------------
BOOL CUIQuickSlot::CheckSlotEXBtn(eQSlotEXBtn eBtn) // 더하기 버튼인지 빼기 버튼인지
{
	switch(eBtn)
	{
	case QSLOT_ADD:
		{
			if (g_bQuickSlotEX1 && g_bQuickSlotEX2)
				return TRUE;
			
			ToggleVisibleEXQuickSlot(TRUE);
			break;
		}
		
	case QSLOT_DEL:
		{
			if (!g_bQuickSlotEX1 && !g_bQuickSlotEX2)
				return TRUE;
			
			ToggleVisibleEXQuickSlot(FALSE);
			break;
		}		
	}
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : SetDisableCheckSlotEX
// Desc : 현재 퀵슬롯 다음을 체크 하여 상태를 변경해준다.
// ----------------------------------------------------------------------------
void CUIQuickSlot::SetDisableCheckSlotEX()
{
	if (g_bQuickSlotEX1 && g_bQuickSlotEX2)
	{
		m_btnAddSlot.SetBtnState(UBS_DISABLE);
		m_btnAddSlot.SetEnable(FALSE);
		m_btnDeleteSlot.SetBtnState(UBS_IDLE);
		m_btnDeleteSlot.SetEnable(TRUE);
	}
	else if (!g_bQuickSlotEX1 && !g_bQuickSlotEX2)
	{
		m_btnAddSlot.SetBtnState(UBS_IDLE);
		m_btnAddSlot.SetEnable(TRUE);
		m_btnDeleteSlot.SetBtnState(UBS_DISABLE);
		m_btnDeleteSlot.SetEnable(FALSE);
	}
	else
	{
		m_btnAddSlot.SetBtnState(UBS_IDLE);
		m_btnAddSlot.SetEnable(TRUE);
		m_btnDeleteSlot.SetBtnState(UBS_IDLE);
		m_btnDeleteSlot.SetEnable(TRUE);
	}
}

void CUIQuickSlot::setQuickSlotItemData( int nPage, int nSlot, int type, int idx, CItems* pItem, bool bWearTab, bool bItemRef /*= true*/ )
{
	if (pItem->Item_Index < 0)
		return;

	m_abtnItems[nPage][nSlot]->setData(pItem, bItemRef);
	m_abtnItems[nPage][nSlot]->SetQuickSlotWearType(type);

	if (bWearTab)
		m_abtnItems[nPage][nSlot]->setWearTab(true);
	else
		m_abtnItems[nPage][nSlot]->setWearTab(false);
}

void CUIQuickSlot::UpdateItemDurability(int nUinIdx,  UWORD now, UWORD max )
{
// 	for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
// 	{
// 		for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
// 		{
// 			CItems* pItems = m_abtnItems[iPage][iBtn]->getItems();
// 
// 			if (pItems != NULL && pItems->Item_UniIndex == nUinIdx)
// 			{
// 				m_abtnItems[iPage][iBtn]->SetItemDurNow(now);
// 				
// 				if (max > 0)
// 					m_abtnItems[iPage][iBtn].SetItemDurMax(max);
// 			}
// 		}
// 	}
}

BOOL CUIQuickSlot::IsInQuickSlot( int nUinIndex )
{
	for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			CItems* pItems = m_abtnItems[iPage][iBtn]->getItems();

			if (pItems != NULL && pItems->Item_UniIndex == nUinIndex)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CUIQuickSlot::SetToggle( int nType, int nIndex, bool bToggle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CSkill* pSkill = NULL;
	CItems* pItem = NULL;

	if (nType == 0)
	{
		pSkill = &_pNetwork->GetSkillData(nIndex);

		if (pSkill != NULL && !(pSkill->GetFlag() & SF_TOGGLE))
			return;
	}
	else
	{
		pItem = pUIManager->GetInventory()->GetItems(nIndex);
	
		if (pItem == NULL || !(pItem->ItemData->GetFlag() & ITEM_FLAG_TOGGLE))
			return;
	}

	for( int iPage = 0; iPage < QSLOT_PAGE_COUNT; iPage++ )
	{
		for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
		{
			// 스킬
			if (nType == 0)
			{
				if (m_abtnItems[iPage][iBtn]->getIndex() == nIndex)
					m_abtnItems[iPage][iBtn]->SetToggle(bToggle);
			}
			// 아이템
			else //if (nType == 1)
			{
				CItems* pItems = m_abtnItems[iPage][iBtn]->getItems();

				if (pItems != NULL && pItems->Item_UniIndex == nIndex)
					m_abtnItems[iPage][iBtn]->SetToggle(bToggle);
			}
		}
	}
}

void CUIQuickSlot::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	for (int i = 0; i < QSLOT_PAGE_COUNT; ++i)
	{
		for (int j = 0; j < QSLOT_BTN_COUNT; ++j)
			m_abtnItems[i][j]->Update(fDeltaTime, ElapsedTime);
	}
}

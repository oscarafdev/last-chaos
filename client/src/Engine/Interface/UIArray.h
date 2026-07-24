#ifndef UI_ARRAY_H_
#define UI_ARRAY_H_

////////////////////////////////////////////////////////////////////////////
//	기본 컨셉 : clone 형태. 
//  ArrayItem에 저장된 컨트롤들을 row * col 만큼 생성 한다.
////////////////////////////////////////////////////////////////////////////

class CUIBase;
class CUIArrayItem;

class ENGINE_API CUIArray : public CUIBase
{
public:
	CUIArray();
	~CUIArray();

	enum eIMG_STATE
	{
		eIMG_STATE_SELECT = 0,
		eIMG_STATE_OVER,
		eIMG_STATE_MAX,
	};

	CUIBase* Clone();
	void AddArrayItem( CUIArrayItem* pItem, bool bUpdate = true );
	CUIArrayItem* GetArrayItem( int idx );

	CUIArrayItem* GetArrayItemTemplate()		{ return m_pArrayTemp; }
	void SetArrayTemplate( CUIArrayItem* pItem ) { m_pArrayTemp = pItem; }

	int	GetArrayChildCount()					{ return m_vecArrayChild.size(); }

	void deleteChildList();
	void deleteChildItem(int pos);
	void deleteChildItem(CUIBase* pt);

	// 게임 재시작 등에 사용됨
	// ArrayItem 만 정리함.
	void ResetArray();

	void SetRectSurface(CUIRectSurface* pRS)	{	m_pMouseEventImage = pRS;	}
	CUIRectSurface* GetRectSurface()			{ return m_pMouseEventImage;	}
	
	void SetMouseEventImage( UIRect rc, UIRectUV uv, int type );

	void SetMouseOverItem( int idx);
	int  GetMouseOverItem()	{ return m_nMouseOverIdx; }

	int	GetSelectIdx() { return m_nSelectIdx; }
	void SetSelectIdx(int idx);
		 
	void SetRowCol(int row, int col)	{ m_nRow = row; m_nCol = col; }
	void GetRowCol(int& row, int& col)	{ row = m_nRow; col = m_nCol; }
		 
	void SetStartItem(int sx, int sy)	{ m_nStartX = sx; m_nStartY = sy; }
	void GetStartItem(int& sx, int& sy)	{ sx = m_nStartX; sy = m_nStartY; }
		 
	void SetGap(int x, int y)			{ m_nGapX = x; m_nGapY = y;	}
	void GetGap(int& x, int& y)			{ x = m_nGapX; y = m_nGapY;	}
	
	void SetScroll( CUIScrollBar* pScroll );
	void SetShowRowCount(int nShow)	{ m_nShowRow = nShow; }
	int	 GetShowRowCount()			{ return m_nShowRow;  }

	void UpdateItem();

	void OnPostRender(CDrawPort* pDraw);

	int	HitTest( int x, int y );

	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT	OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

#ifdef UI_TOOL
	void	GetMouseEventImage( UIRect& rc, UIRectUV& uv, int type )
	{
		rc = m_rcMouseEventImage[type];
		uv = m_uvMouseEventImage[type];
	};

private:
	UIRect	m_rcMouseEventImage[eIMG_STATE_MAX];
	UIRectUV m_uvMouseEventImage[eIMG_STATE_MAX];
#endif // UI_TOOL

private:

	typedef std::vector<CUIArrayItem*>	vecArray;
	typedef std::vector<CUIArrayItem*>::iterator	vecArrayIter;

	int		m_nRow;
	int		m_nCol;
	int		m_nStartX;
	int		m_nStartY;
	int		m_nGapX;
	int		m_nGapY;
	int		m_nSelectIdx;
	int		m_nMouseOverIdx;
	bool	m_bSelected;
	bool	m_bMouseOver;
	int		m_nShowRow;


	vecArray		m_vecArrayChild;

	CUIArrayItem*	m_pArrayTemp;
	CUIScrollBar*	m_pScroll;
	CUIRectSurface* m_pMouseEventImage;
	POINT			m_ptSelectOffSet;
	POINT			m_ptMouseOverOffset;

};

#endif	// UI_ARRAY_H_
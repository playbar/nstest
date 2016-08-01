// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx
#define OFFSET_FIRST    2
#define OFFSET_OTHER    6
#define SUBMIT_OFFSET  12
CListCtrlEx::CListCtrlEx()
{
	m_cxClient = 0;
	m_cxStateImageOffset = 0;

	m_bFullRowSel = TRUE;
	m_bClientWidthSel = FALSE;
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk = ::GetSysColor(COLOR_WINDOW);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

void CListCtrlEx::OnSize(UINT nType, int cx, int cy) 
{
	m_cxClient=cx;
	CListCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	//CListCtrl& ListCtrl=*this;
#ifdef _ANALY
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		_XCALLENTRY*pCall=(_XCALLENTRY*)lpDrawItemStruct->itemData;
		CRect rcItem(lpDrawItemStruct->rcItem);
		UINT uiFlags = ILD_TRANSPARENT;
		
		CImageList* pImageList;
		int nItem = lpDrawItemStruct->itemID;
		BOOL bFocus = TRUE;//(GetFocus() == this);
		COLORREF clrTextSave, clrBkSave;
		COLORREF clrImage = m_clrBkgnd;
		static _TCHAR szBuff[MAX_PATH];
		LPCTSTR pszText;

		int iFirst=0;
		if(pCall)
		{
			_XCALLENTRY*pp=pCall->pParent;
			while(pp)
			{
				iFirst+=SUBMIT_OFFSET;
				pp=pp->pParent;
			}
		}
	// get item data

		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = nItem;
		lvi.iSubItem = 0;
		lvi.pszText = szBuff;
		lvi.cchTextMax = sizeof(szBuff);
		lvi.stateMask = 0xFFFF;     // get all state flags
		GetItem(&lvi);

		BOOL bSelected = FALSE;//(bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
		bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

	// set colors if item is selected

		CRect rcAllLabels;
		GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);

		CRect rcLabel;
		GetItemRect(nItem, rcLabel, LVIR_LABEL);

		//rcAllLabels.left = rcLabel.left;
		if (m_bClientWidthSel && rcAllLabels.right<m_cxClient)
			rcAllLabels.right = m_cxClient;

		if (bSelected)
		{
			clrTextSave = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			clrBkSave = pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

			pDC->FillRect(rcAllLabels, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
		}
		else
		{
			if(nItem&1)
			{
				int r=(m_clrTextBk>>16)&0xff;
				int g=(m_clrTextBk>>8)&0xff;
				int b=(m_clrTextBk)&0xff;
				r-=10;
				g-=15;
				b-=15;
				if(r<0) r=0;
				if(g<0) g=0;
				if(b<0) b=0;
				pDC->FillRect(rcAllLabels, &CBrush((r<<16)|(g<<8)|b));
			}
			else
				pDC->FillRect(rcAllLabels, &CBrush(m_clrTextBk));
			if(pCall&&pCall->nPecent>=5)
				clrTextSave = pDC->SetTextColor(0x0000cf);
			else
				clrTextSave = pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}

	// set color and mask for the icon

		if (lvi.state & LVIS_CUT)
		{
			clrImage = m_clrBkgnd;
			uiFlags |= ILD_BLEND50;
		}
		else if (bSelected)
		{
			clrImage = ::GetSysColor(COLOR_HIGHLIGHT);
			uiFlags |= ILD_BLEND50;
		}

	// draw state icon
		if(pCall)
		{
			CPen pen(PS_DOT,1,0x5f5f5f);
			CPen*pOld=pDC->SelectObject(&pen);
			//UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
			if (pCall->pChild)//nStateImageMask)
			{
				int nImage = pCall->bExtern?1:0;
					//(nStateImageMask>>12) - 1;
				pImageList = GetImageList(LVSIL_STATE);
				if (pImageList)
				{
					pImageList->Draw(pDC, nImage,
						CPoint(rcItem.left+iFirst, rcItem.top), ILD_TRANSPARENT);
				}
			}
			else
			{
				int bx=rcItem.left+iFirst+6;
				int by=(rcItem.top+rcItem.bottom)>>1;
				pDC->MoveTo(bx+8,by);
				pDC->LineTo(bx,by);
				pDC->LineTo(bx,rcAllLabels.top);
				if(pCall->pNext)
					pDC->LineTo(bx,rcAllLabels.bottom);
			}
			_XCALLENTRY*pp=pCall->pParent;
			int bx=rcItem.left+iFirst+6-SUBMIT_OFFSET;
			//if(pCall->strFunction==_T("GetMember"))
			//	int v=0;
			while(pp)
			{
				if(pp->pNext)
				{
					pDC->MoveTo(bx,rcAllLabels.bottom);
					pDC->LineTo(bx,rcAllLabels.top);
				}
				pp=pp->pParent;
				bx-=SUBMIT_OFFSET;
			}
			pDC->SelectObject(pOld);
		}

	// draw normal and overlay icon

		CRect rcIcon;
		GetItemRect(nItem, rcIcon, LVIR_ICON);

		pImageList = GetImageList(LVSIL_SMALL);
		if (pImageList)
		{
			UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
			if (rcItem.left<rcItem.right-1)
			{
				ImageList_DrawEx(pImageList->m_hImageList, lvi.iImage,
						pDC->m_hDC,rcIcon.left,rcIcon.top, 16, 16,
						m_clrBkgnd, clrImage, uiFlags | nOvlImageMask);
			}
		}

	// draw item label

		GetItemRect(nItem, rcItem, LVIR_LABEL);
		rcItem.right -= m_cxStateImageOffset;

		

		pszText = MakeShortString(pDC, szBuff,
					rcItem.right-rcItem.left, iFirst);

		rcLabel = rcItem;
		rcLabel.left += iFirst;
		rcLabel.right -= OFFSET_FIRST;

		pDC->DrawText(pszText,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

	// draw labels for extra columns

		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH;

		for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
		{
			rcItem.left = rcItem.right;
			rcItem.right += lvc.cx;

			int nRetLen = GetItemText(nItem, nColumn,
							szBuff, sizeof(szBuff));
			if (nRetLen == 0)
				continue;

			pszText = MakeShortString(pDC, szBuff,
				rcItem.right - rcItem.left, 2*OFFSET_OTHER);

			UINT nJustify = DT_LEFT;

			if(pszText == szBuff)
			{
				switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
				{
				case LVCFMT_RIGHT:
					nJustify = DT_RIGHT;
					break;
				case LVCFMT_CENTER:
					nJustify = DT_CENTER;
					break;
				default:
					nJustify = DT_LEFT;
					break;
				}
			}

			rcLabel = rcItem;
			rcLabel.left += OFFSET_OTHER;
			rcLabel.right -= OFFSET_OTHER;

			pDC->DrawText(pszText, -1, rcLabel,
				nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
		}

	// draw focus rectangle if item has focus

		if (lvi.state & LVIS_FOCUSED && bFocus)
			pDC->DrawFocusRect(rcAllLabels);

	// set original colors if item was selected

		if (bSelected)
		{
			pDC->SetTextColor(clrTextSave);
			pDC->SetBkColor(clrBkSave);
		}
#endif
	//CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

LPCTSTR CListCtrlEx::MakeShortString(CDC *pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[] = _T("...");

	int nStringLen = lstrlen(lpszLong);

	if(nStringLen == 0 ||
		(pDC->GetTextExtent(lpszLong, nStringLen).cx + nOffset) <= nColumnLen)
	{
		return(lpszLong);
	}

	static _TCHAR szShort[MAX_PATH];

	lstrcpy(szShort,lpszLong);
	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i = nStringLen-1; i > 0; i--)
	{
		szShort[i] = 0;
		if((pDC->GetTextExtent(szShort, i).cx + nOffset + nAddLen)
			<= nColumnLen)
		{
			break;
		}
	}

	lstrcat(szShort, szThreeDots);
	return(szShort);
}


void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags = 0;
	int nHitItem = HitTest(point, &uFlags);

	// we need additional checking in owner-draw mode
	// because we only get LVHT_ONITEM
	BOOL bHit = FALSE;
	if (uFlags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))
	{
		CRect rect;
		GetItemRect(nHitItem, rect, LVIR_ICON);
		rect.left=0;
#ifdef _ANALY
		_XCALLENTRY*pCall=(_XCALLENTRY*)GetItemData(nHitItem);
		if(pCall)
		{
			_XCALLENTRY*pp=pCall->pParent;
			while(pp)
			{
				rect.left+=SUBMIT_OFFSET;
				rect.right+=SUBMIT_OFFSET;
				pp=pp->pParent;
			}
		}
#endif
		// check if hit was on a state icon
		//if (point.x < rect.left)
		if(rect.PtInRect(point))
			bHit = TRUE;
	}
	else if (uFlags & LVHT_ONITEMSTATEICON)
		bHit = TRUE;

	if (bHit)
	{
#ifdef _ANALY
		_XCALLENTRY*pCall=(_XCALLENTRY*)GetItemData(nHitItem);
		if(!pCall||!pCall->pChild) return;
		pCall->bExtern=!pCall->bExtern;
		GetParent()->PostMessage(WM_COMMAND,GetDlgCtrlID());
#endif
		//CheckItem(nHitItem);
	}
	else
		CListCtrl::OnLButtonDown(nFlags, point);
}

void CListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags = 0;
	int nHitItem = HitTest(point, &uFlags);

	// we need additional checking in owner-draw mode
	// because we only get LVHT_ONITEM
	BOOL bHit = FALSE;
	if (uFlags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))
	{
		CRect rect;
		GetItemRect(nHitItem, rect, LVIR_ICON);
		rect.left=0;
#ifdef _ANALY
		_XCALLENTRY*pCall=(_XCALLENTRY*)GetItemData(nHitItem);
		if(pCall)
		{
			_XCALLENTRY*pp=pCall->pParent;
			while(pp)
			{
				rect.left+=SUBMIT_OFFSET;
				rect.right+=SUBMIT_OFFSET;
				pp=pp->pParent;
			}
		}
#endif
		// check if hit was on a state icon
		//if (point.x < rect.left)
		if(!rect.PtInRect(point))
			bHit = TRUE;
	}
	else if (uFlags & LVHT_ONITEMSTATEICON)
		bHit = TRUE;

	if (bHit)
	{
#ifdef _ANALY
		_XCALLENTRY*pCall=(_XCALLENTRY*)GetItemData(nHitItem);
		if(!pCall||!pCall->pChild) return;
		pCall->bExtern=!pCall->bExtern;
		GetParent()->PostMessage(WM_COMMAND,GetDlgCtrlID());
#endif
		//CheckItem(nHitItem);
	}
	else
		CListCtrl::OnLButtonDblClk(nFlags, point);
}



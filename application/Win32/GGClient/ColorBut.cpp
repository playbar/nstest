// ColorBut.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "ColorBut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBut

CColorBut::CColorBut()
{
}

CColorBut::~CColorBut()
{
}


BEGIN_MESSAGE_MAP(CColorBut, CButton)
	//{{AFX_MSG_MAP(CColorBut)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBut message handlers

void CColorBut::DrawItem(LPDRAWITEMSTRUCT lpds) 
{
	// TODO: Add your code to draw the specified item
	CDC*pDC=CDC::FromHandle(lpds->hDC);
	CRect rect(lpds->rcItem);
	CBrush brush(m_nColor);
	pDC->FillRect(rect,&brush);
	CBrush black((COLORREF)0);
	pDC->FrameRect(rect,&black);
	
}

void CColorBut::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CColorDialog dlg(m_nColor);
	if(dlg.DoModal()!=IDOK) return;
	m_nColor=dlg.GetColor();
	Invalidate();
	//CButton::OnLButtonDown(nFlags, point);
}

LRESULT CColorBut::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_toolTip.GetSafeHwnd() != NULL )
	{ switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		{
			MSG msg;
			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;

			m_toolTip.RelayEvent(&msg);
		}
		}
	}	
	return CButton::WindowProc(message, wParam, lParam);
}

void CColorBut::OnDestroy() 
{
	CButton::OnDestroy();
	
	// TODO: Add your message handler code here
	m_toolTip.DestroyWindow();
}



void CColorBut::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_toolTip.Create( this ) )
	{
		CString m_str;
		GetWindowText(m_str);
		//提示内容为按钮标题
		m_toolTip.AddTool( this, m_str );
		m_toolTip.Activate(TRUE);
	}	
	CButton::PreSubclassWindow();
}

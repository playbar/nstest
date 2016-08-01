// XVScrollBar.h: interface for the XVScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XVSCROLLBAR_H__136C8BBF_AED4_4253_961E_E2C3DD1F3775__INCLUDED_)
#define AFX_XVSCROLLBAR_H__136C8BBF_AED4_4253_961E_E2C3DD1F3775__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "GGType.h"	// Added by ClassView

class _XEXT_CLASS XVScrollBar : public XWindow  
{
	enum {XS_NONE,XS_HAND,XS_UPLINE,XS_DOWNLINE,XS_UPPAGE,XS_DOWNPAGE};
public:
	XBOOL Create(XRect&rect,XWindow*pWnd,XU32 nCmd=XCMD_VSCROLL);
	void SetRange(XU32 nRange,int nPos=-1);
	void SetPos(int nPos);
	XINT GetPos(){return m_nPos;}
	XINT GetRange(){return m_nRange;}
	XBOOL OnPaint(XGraphics&g);
	XVScrollBar();
	virtual ~XVScrollBar();
	void SetSize(XU16 nl,XU16 np)
	{
		m_nLineSize=nl;
		m_nPageSize=np;
	}
protected:
	void OnClick(XPoint&){}
	void OnTimer();
	void Step();
	void NewPos(int nPos,XBOOL bScroll=XFALSE);
	void OnCreate();
	void OnMouseMove(XPoint&pt,int iPos);
	void OnMouseUp(XPoint&pt,int iPos);
	void OnMouseDown(XPoint&pt,int iPos);
	void CalcRect(XRect&r,int&pp);
	XINT m_nPos,m_nRange,m_nDownPos;
	XINT m_nPageSize,m_nLineSize;
	XU8  m_nStatus,m_nTick;
	XPoint m_down;
};

#endif // !defined(AFX_XVSCROLLBAR_H__136C8BBF_AED4_4253_961E_E2C3DD1F3775__INCLUDED_)

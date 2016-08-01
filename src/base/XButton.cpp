// XButton.cpp: implementation of the XButton class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XButton::XButton()
{
  m_nHotKey=0;
  m_pImage=XNULL;
  m_nIndex=0;
  m_bDown=XFALSE;
}

XButton::~XButton()
{

}

XBOOL XButton::Create(XRect &rect, XU32 nID, XPCWSTR strTitle,XWCHAR hotKey,XWindow*pWnd)
{
	m_strTitle=strTitle;
	m_nHotKey=hotKey;
	return XWindow::Create(rect,XES_VISIABLE|XES_FOCUS,pWnd,nID);
}

void XButton::SetImage(XImage *pImage)
{
	m_pImage=pImage;
	m_nIndex=0;
	Invalidate();
}

XBOOL XButton::OnPaint(XGraphics &g)
{

	XRect r;
	GetClientRect(r);
	XColor cc(GetSysColor(XCW_BUTTON));
	XU8 s=m_bDown?0:XBS_THICK;
	if(m_nStyle&XES_SETFOCUS) s|=XBS_BFRAME;
	//else {r.bottom--;r.right--;}
	//else s|=XBS_WFRAME;
	if(GetIcon(XICON_BUTTON==XNULL))
		DrawButton(g,r,cc,s);
	else
		g.DrawButton(r,cc,s);
	XSize sz;
	g.MeasureString(m_strTitle,sz);
	int bx=r.left+((r.Width()-sz.cx)>>1);
	int by=r.top+2+((r.Height()-sz.cy)>>1);
	if(m_bDown) {bx++;by++;}
	if(m_pImage!=XNULL)
	{
		bx-=m_pImage->Width()>>1;
		m_pImage->SetActive(m_nIndex);
		int dd=(m_nTextHeight-m_pImage->Height())>>1;
		g.DrawImage(*m_pImage,XPoint(bx,by+dd));
		bx+=m_pImage->Width();
	}
	g.SetColor(GetSysColor(XCW_TEXT));
	g.DrawString(m_strTitle,bx,by);
	return XTRUE;
}

XBOOL XButton::OnKeyDown(XKEYMSG&m)
{
	int c=m.nKeyCode;
    if(c==m_nHotKey||c==XK_RETURN)
	{	
		PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
		return XTRUE;
	}
	return XFALSE;
}

void XButton::OnClick(XPoint &pt,int iPos)
{

}

void XButton::OnMouseDown(XPoint &pt,int iPos)
{
	if(iPos) return;
	m_bDown=XTRUE;
	Invalidate();
	SetCapture(this);
}

void XButton::OnMouseUp(XPoint &pt,int iPos)
{
	if(iPos) return;
	SetCapture(XNULL);
	if(m_bDown)
		XWindow::OnClick(pt,0);
	m_bDown=XFALSE;
	Invalidate();
	
}

void XButton::OnMouseMove(XPoint &pt,int iPos)
{
	//if(!m_bDown) return;
	if(iPos) return;
	if(m_pCapture!=this) return;
	XRect r;
	GetClientRect(r);
	if(!r.PtInRect(pt)) 
	{
		if(m_bDown)
		{
			m_bDown=XFALSE;
			Invalidate();
		}
	}
	else if(!m_bDown)
	{
		m_bDown=XTRUE;
		Invalidate();
	}
}

void XButton::OnBlur()
{
	XWindow::OnBlur();
	Invalidate();
}

void XButton::SetTitle(XPCWSTR strTitle)
{
	m_strTitle=strTitle;
	Invalidate();
}
//#include "XData.h"

// XHScrollBar.cpp: implementation of the XHScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XHScrollBar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XHScrollBar::XHScrollBar()
{
   m_nRange=0;
   m_nPos=0;
}

XHScrollBar::~XHScrollBar()
{

}

XBOOL XHScrollBar::OnPaint(XGraphics &g)
{
	XRect rect;
	GetClientRect(rect);
	XColor c(XWindow::m_nColors[XCW_BUTTON]);
	XColor cc(c);
	cc.Bright(24);
	g.SetBackColor(cc);
	g.FillRectangle(rect);
	
	XRect r=rect;
	int pp;
	CalcRect(r,pp);
	if(m_nStatus==XS_UPPAGE&&r.left>rect.bottom)
	{
	   XColor cm(c);
	   cm.Bright(12);
	   g.SetBackColor(cm);
	   g.FillRectangle(XRect(0,0,r.left,rect.bottom));
	}
	if(m_nStatus==XS_DOWNPAGE&&r.right<rect.right-rect.bottom)
	{
	   XColor cm(c);
	   cm.Bright(12);
	   g.SetBackColor(cm);
	   g.FillRectangle(XRect(r.right,0,rect.right-rect.bottom,rect.bottom));
	}


	XColor hc(c);
	hc.Bright(10);
	XRect rl(0,0,rect.bottom,rect.bottom);
	XRect rr(rect.right-rect.bottom,0,rect.right,rect.bottom);
	g.DrawButton(rl,m_nStatus==XS_UPLINE?hc:c,XBS_BFRAME|XBS_WFRAME);
	g.DrawButton(rr,m_nStatus==XS_DOWNLINE?hc:c,XBS_BFRAME|XBS_WFRAME);

	cc=c;cc.Dark(60);
	g.DrawDirect(rl,XK_LEFT,cc);
	g.DrawDirect(rr,XK_RIGHT,cc);


	g.DrawButton(r,m_nStatus==XS_HAND?hc:c,XBS_BFRAME|XBS_WFRAME|XBS_VHAND);

	return XTRUE;
	//*/
}

void XHScrollBar::CalcRect(XRect &rect, int &pp)
{
	GetClientRect(rect);
	int w=rect.Width()-(rect.bottom<<1);
	int mw=rect.Height()<<1;
	pp=m_nRange==0?1000:XMIN(1000,(w-mw)*1000/m_nRange);
	int bx=m_nPos*pp/1000+rect.bottom-1;
	int ex=rect.right-((m_nRange-m_nPos)*pp/1000+rect.bottom)+1;
	rect=XRect(bx,0,ex,rect.bottom);
}

void XHScrollBar::SetRange(XU32 nRange, int nPos)
{
	m_nRange=nRange;
	if(nPos>=0) m_nPos=nPos;
	if(m_nPos>m_nRange)
		m_nPos=m_nRange;
	Invalidate();
}

void XHScrollBar::OnMouseDown(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(m_nRange<=0) return;
	XRect r;
	GetClientRect(r);
	XRect ru(0,0,r.bottom,r.bottom);
	XRect rd(r.right-r.bottom,0,r.right,r.bottom);
	int pp;
	CalcRect(r,pp);
	m_nStatus=XS_NONE;
	if(ru.PtInRect(pt))
		m_nStatus=XS_UPLINE;
	else if(rd.PtInRect(pt))
		m_nStatus=XS_DOWNLINE;
	else if(pt.x<r.left)
	{
		m_nStatus=XS_UPPAGE;
		m_nDownPos=(pt.x-((r.left+r.right)>>1))*1000/pp+m_nPos;
	}
	else if(pt.x>r.right)
	{
		m_nStatus=XS_DOWNPAGE;
		m_nDownPos=(pt.x-((r.left+r.right)>>1))*1000/pp+m_nPos;
	}
	else 
	{
		m_nStatus=XS_HAND;
		m_nDownPos=m_nPos;
	}
	m_nTick=XW_WAITTIME;
	Step();
	SetCapture(this);
	m_down=pt;
	Invalidate();
}

void XHScrollBar::OnMouseUp(XPoint &pt,int iPos)
{
	if(iPos) return;
	m_nStatus=XS_NONE;
    SetCapture(XNULL);
	Invalidate();
	//PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
}

void XHScrollBar::OnMouseMove(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(m_nStatus==XS_HAND)
	{
		int pp;
		XRect r;
		CalcRect(r,pp);
		if(pp<2) pp=2;
		int pos=(pt.x-m_down.x)*1000/pp+m_nDownPos;
		NewPos(pos,XTRUE);//XFALSE);
	}
}

void XHScrollBar::OnCreate()
{
	m_nLineSize=XWindow::m_nTextHeight;
	m_nPageSize=m_rect.Width()-m_nLineSize;
}

void XHScrollBar::NewPos(int nPos,XBOOL bScroll)
{
	nPos=XMAX(nPos,0);
	nPos=XMIN(nPos,(int)m_nRange);
	if(m_nPos==nPos) return;
	int nDelta=nPos-m_nPos;
	m_nPos=nPos;
	Invalidate();
	if(bScroll)
		PostMsg(XWM_HSCROLL,nDelta,(XU32)this,XMS_PARENT);
	else
		PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
	
}

void XHScrollBar::Step()
{
	switch(m_nStatus)
	{
	case XS_UPLINE:
	     NewPos(m_nPos-m_nLineSize);
		 break;
	case XS_DOWNLINE:
		 NewPos(m_nPos+m_nLineSize);
		 break;
	case XS_UPPAGE:
		 if(m_nPos-m_nPageSize>m_nDownPos)
			NewPos(m_nPos-m_nPageSize);
		 else
		 {
			m_nStatus=XS_HAND;
			NewPos(m_nDownPos);
		 }
		 break;
	case XS_DOWNPAGE:
		 if(m_nPos+m_nPageSize<m_nDownPos)
			NewPos(m_nPos+m_nPageSize);
		 else
		 {
			m_nStatus=XS_HAND;
			NewPos(m_nDownPos);
		 }
		 break;
	}
}

void XHScrollBar::OnTimer()
{
	if(m_nStatus<=XS_HAND) return;
	if(m_nTick>0) m_nTick--;
	else
	{
		Step();
		m_nTick=XW_REPTIME;
	}
}

XBOOL XHScrollBar::Create(XRect &rect, XWindow *pWnd, XU32 nCmd)
{
	return XWindow::Create(rect,XES_VISIABLE,pWnd,nCmd);
}

void XHScrollBar::SetPos(int nPos)
{
	if(nPos<0) nPos=0;
	if(nPos>m_nRange) nPos=m_nRange;
	m_nPos=nPos;
	Invalidate();
}

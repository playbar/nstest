// XVScrollBar.cpp: implementation of the XVScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XVScrollBar.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XVScrollBar::XVScrollBar()
{
    m_nRange=0;
	m_nPos=0;
	m_nStatus=XS_NONE;
	m_nTick=0;
}

XVScrollBar::~XVScrollBar()
{

}

XBOOL XVScrollBar::OnPaint(XGraphics &g)
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
	if(m_nStatus==XS_UPPAGE&&r.top>rect.right)
	{
	   XColor cm(c);
	   cm.Bright(12);
	   g.SetBackColor(cm);
	   g.FillRectangle(XRect(0,rect.right,rect.right,r.top));
	}
	if(m_nStatus==XS_DOWNPAGE&&r.bottom<rect.bottom-rect.right)
	{
	   XColor cm(c);
	   cm.Bright(12);
	   g.SetBackColor(cm);
	   g.FillRectangle(XRect(0,r.bottom,rect.right,rect.bottom-rect.right));
	}

	XColor hc(c);
	hc.Bright(10);
	
	XRect ru(0,0,rect.right,rect.right);
	XRect rd(0,rect.bottom-rect.right,rect.right,rect.bottom);

	g.DrawButton(ru,m_nStatus==XS_UPLINE?hc:c,XBS_BFRAME|XBS_WFRAME);
	g.DrawButton(rd,m_nStatus==XS_DOWNLINE?hc:c,XBS_BFRAME|XBS_WFRAME);

	cc=c;cc.Dark(50);
	g.DrawDirect(ru,XK_UP,cc);
	g.DrawDirect(rd,XK_DOWN,cc);
	//cc.Mix(c,0.4f);
	/*g.SetColor(cc);
	int dd=(rect.right-8)>>1;
	g.DrawLine(4,rect.right-5,4+dd,rect.right-5-dd);
	g.DrawLine(4+dd,rect.right-5-dd,4+(dd<<1),rect.right-5);
	
	g.DrawLine(4,rect.bottom-4-dd,4+dd,rect.bottom-4);
	g.DrawLine(4+dd,rect.bottom-4,4+(dd<<1),rect.bottom-4-dd);
	cc=c;
	cc.Dark(50);
	g.SetColor(cc);
	for(int i=1;i<3;i++)
	{
	   g.DrawLine(4,rect.right-5-i,4+dd,rect.right-5-dd-i);
	   g.DrawLine(4+dd,rect.right-5-dd-i,4+(dd<<1),rect.right-5-i);
	   g.DrawLine(4,rect.bottom-4-dd-i,4+dd,rect.bottom-4-i);
	   g.DrawLine(4+dd,rect.bottom-4-i,4+(dd<<1),rect.bottom-4-dd-i);
	}*/
	//cc.Mix(c


	g.DrawButton(r,m_nStatus==XS_HAND?hc:c,XBS_BFRAME|XBS_WFRAME|XBS_HHAND);

	return XTRUE;
}

void XVScrollBar::CalcRect(XRect &rect, int &pp)
{
	GetClientRect(rect);
	int h=rect.Height()-(rect.right<<1);
	int mh=rect.Width()<<1;
	pp=m_nRange==0?1000:XMIN(1000,(h-mh)*1000/m_nRange);
	int by=m_nPos*pp/1000+rect.right-1;
	int ey=rect.bottom-((m_nRange-m_nPos)*pp/1000+rect.right)+1;
	rect=XRect(0,by,rect.right,ey);
}

void XVScrollBar::SetPos(int nPos)
{
	if(nPos<0) nPos=0;
	if(nPos>m_nRange) nPos=m_nRange;

	m_nPos=nPos;
	
	Invalidate();
}

void XVScrollBar::SetRange(XU32 nRange, int nPos)
{
	m_nRange=nRange;
	if(nPos>=0) m_nPos=nPos;
	if(m_nPos>m_nRange)
		m_nPos=m_nRange;
	Invalidate();
}

void XVScrollBar::OnMouseDown(XPoint &pt,int iPos)
{
	if(iPos||m_nRange<=0) return;
	XRect r;
	GetClientRect(r);
	XRect ru(0,0,r.right,r.right);
	XRect rd(0,r.bottom-ru.bottom,r.right,r.bottom);
	int pp;
	CalcRect(r,pp);
	m_nStatus=XS_NONE;
	if(ru.PtInRect(pt))
		m_nStatus=XS_UPLINE;
	else if(rd.PtInRect(pt))
		m_nStatus=XS_DOWNLINE;
	else if(pt.y<r.top)
	{
		m_nStatus=XS_UPPAGE;
		m_nDownPos=(pt.y-((r.top+r.bottom)>>1))*1000/pp+m_nPos;
	}
	else if(pt.y>r.bottom)
	{
		m_nStatus=XS_DOWNPAGE;
		m_nDownPos=(pt.y-((r.top+r.bottom)>>1))*1000/pp+m_nPos;
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

void XVScrollBar::OnMouseUp(XPoint &pt,int iPos)
{
	if(iPos) return;
	m_nStatus=XS_NONE;
    SetCapture(XNULL);
	Invalidate();
	//PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
}

void XVScrollBar::OnMouseMove(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(m_nStatus==XS_HAND)
	{
		int pp;
		XRect r;
		CalcRect(r,pp);
		int pos=(pt.y-m_down.y)*1000/pp+m_nDownPos;
		NewPos(pos,XTRUE);
	}
}

void XVScrollBar::OnCreate()
{
	m_nLineSize=XWindow::m_nTextHeight;
	m_nPageSize=m_rect.Height()-m_nLineSize;
}

void XVScrollBar::NewPos(int nPos,XBOOL bScroll)
{
	nPos=XMAX(nPos,0);
	nPos=XMIN(nPos,(int)m_nRange);
	if(m_nPos==nPos) return;
	int nDelta=nPos-m_nPos;
	m_nPos=nPos;
	Invalidate();
	if(bScroll)
		PostMsg(XWM_VSCROLL,nDelta,(XU32)this,XMS_PARENT);
	else
		PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
}

void XVScrollBar::Step()
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

void XVScrollBar::OnTimer()
{
	if(m_nStatus<=XS_HAND) return;
	if(m_nTick>0) m_nTick--;
	else
	{
		Step();
		m_nTick=XW_REPTIME;
	}
}

XBOOL XVScrollBar::Create(XRect &rect, XWindow *pWnd, XU32 nCmd)
{
	return XWindow::Create(rect,XES_VISIABLE,pWnd,nCmd);
}

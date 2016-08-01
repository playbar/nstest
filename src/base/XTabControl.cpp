// XTabControl.cpp: implementation of the XTabControl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XTabControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define XBSIZE 4

XTabControl::XTabControl()
{
	m_nActive=0;
	m_nOffset=0;
}

XTabControl::~XTabControl()
{
    RemoveAll();
}

XBOOL XTabControl::OnPaint(XGraphics &g)
{
   if(m_bLayerout)
	   Layerout(g);
   int w=m_rect.Width(),h=m_rect.Height();
   XRect r(0,0,w,h);
   XColor c=XWindow::m_nColors[XCW_BUTTON];
   c.Dark(m_nStyle&XES_SETFOCUS?8:16);
   g.SetBackColor(c);
   g.FillRectangle(r);
   if(m_tabs.GetSize()<=0) return XTRUE;
   for(XU8 i=m_nOffset;i<m_tabs.GetSize();i++)
      if(i!=m_nActive) DrawTab(g,i,c,XBS_BFRAME);
   
   c=XWindow::m_nColors[XCW_BUTTON];
   //if(!(m_nStyle&XES_SETFOCUS))
//	   c.Dark(4);

   g.DrawButton(XRect(0,m_rect.Height()-XBSIZE,m_rect.Width(),m_rect.Height()),c,0);
   
   DrawTab(g,m_nActive,c,XBS_BFRAME|XBS_THICK);
   /*c.Dark(16);   
   g.SetColor(c);

   g.DrawLine(0,m_rect.Height()-1,m_rect.Width(),m_rect.Height()-1);*/
	   /*r=XRect(m_tabs[i]->nPosX-m_tabs[m_nOffset]->nPosX,
		       0,0,m_rect.bottom+1);
	   r.right=r.left+m_tabs[i]->nWidth;
	   g.DrawButton(r,c,i==m_nActive?XBS_BFRAME|XBS_THICK|XBS_WFRAME:XBS_BFRAME);*/
   //}
   return XTRUE;
}

XBOOL XTabControl::Insert(XPCWSTR strTitle, XImage *pImage,XU32 nData,XINT nPos,XBOOL bActive)
{
   if(m_tabs.GetSize()>=XMAXTABS) return XFALSE;
   if(nPos<0||nPos>(XINT)m_tabs.GetSize()) nPos=m_tabs.GetSize();
   TABLEINFO*pInfo=new TABLEINFO;
   pInfo->nData=nData;
   pInfo->pImage=pImage;
   pInfo->nWidth=0;
   pInfo->nPosX=0;
   pInfo->nMinWidth=0;
   pInfo->strTitle=strTitle;
   pInfo->strTitleLong=strTitle;
   pInfo->nActive=0;
   m_tabs.InsertAt(nPos,pInfo);
   if(bActive) m_nActive=nPos;
   m_bLayerout=XTRUE;
   if(m_tabs.GetSize()>1)
	   m_nStyle|=XES_FOCUS;
   Invalidate();
   return XTRUE;
}

XBOOL XTabControl::Remove(XU32 id)
{
   if(id>=m_tabs.GetSize()) return XFALSE;
   delete m_tabs[id];
   m_tabs.RemoveAt(id);
   if(m_nActive>=id&&m_nActive>0)
	   m_nActive--;
   m_bLayerout=XTRUE;
   if(m_tabs.GetSize()<2)
	   m_nStyle&=~XES_FOCUS;
   Invalidate();
   return XTRUE;
}

void XTabControl::RemoveAll()
{
	for(XU32 i=0;i<m_tabs.GetSize();i++)
	   delete m_tabs[i];
    m_tabs.RemoveAll();
	m_nActive=0;
	Invalidate();
}

void XTabControl::Layerout(XGraphics &g)
{
	XU32 i;
	m_bLayerout=XFALSE;
	int w=0,mw=0;
    for(i=0;i<m_tabs.GetSize();i++)
	{
		//m_tabs[i]->strTitleLong=m_tabs[i]->strTitle;
		FormatTitle(g,m_tabs[i]->strTitle);
		m_tabs[i]->nWidth=8;
		m_tabs[i]->nMinWidth=XWindow::m_nTextHeight*4;
		if(m_tabs[i]->pImage!=XNULL)
		{
			m_tabs[i]->nWidth+=(XU16)m_tabs[i]->pImage->Width();
			m_tabs[i]->nMinWidth+=m_tabs[i]->nWidth;
		}
		XSize sz;
		g.MeasureString(m_tabs[i]->strTitle,sz);
		m_tabs[i]->nWidth+=(XU16)sz.cx;
		w+=m_tabs[i]->nWidth;
		if(m_tabs[i]->nMinWidth>m_tabs[i]->nWidth)
			m_tabs[i]->nMinWidth=m_tabs[i]->nMinWidth;
		mw+=m_tabs[i]->nMinWidth;
	}
	//int pp=w>mw?
	//	XMAX(XMIN(m_rect.Width(),w)-mw,0)*1000/XMAX((w-mw),1):0;
	//int lw=w-mw;
	int nPosX=0;
	for(i=0;i<m_tabs.GetSize();i++)
	{
	   m_tabs[i]->nPosX=nPosX;
	   //if(m_tabs[i]->nWidth>m_tabs[i]->nMinWidth)
	   //	   m_tabs[i]->nWidth=m_tabs[i]->nMinWidth+(m_tabs[i]->nWidth-m_tabs[i]->nMinWidth)*pp/1000;
	   nPosX+=m_tabs[i]->nWidth-1;
	}
	SetOffset();
}

void XTabControl::SetOffset()
{
	if(m_nActive>=m_tabs.GetSize()) return;
	int w=m_rect.Width();
	if(m_tabs[m_nActive]->nPosX+m_tabs[m_nActive]->nWidth>w||
	   m_nActive+1<(int)m_tabs.GetSize())
	{
	while(m_tabs[m_nActive]->nPosX+m_tabs[m_nActive]->nWidth-
		  m_tabs[m_nOffset]->nPosX+XWindow::m_nTextHeight>w)
		{
		  if(m_nOffset>=m_nActive) break;
		  m_nOffset++;
		}
	}
	if(m_nOffset>=m_nActive&&m_nActive>0)
		m_nOffset=m_nActive-1;
	else if(m_nActive==0)
		m_nOffset=0;
}

void XTabControl::DrawTab(XGraphics&g,XU8 id,XU32 c,XU8 s)
{
	if(id>=m_tabs.GetSize()) return;
	TABLEINFO*p=m_tabs[id];
	int bx=p->nPosX-m_tabs[m_nOffset]->nPosX,by=3;
	XRect r(bx,0,0,m_rect.bottom+1);
	r.right=r.left+p->nWidth;
	if(id!=m_nActive){ r.top++;by++;}
	else
	{
		r.left--;
		bx--;
	}
	//r.bottom-=XBSIZE+1;
	int y=r.bottom-XBSIZE-1;
	g.SetClip(XRect(0,0,m_rect.Width(),y));
	//r.bottom+=XBSIZE+1;
	//XRect rt(r.left,r.bottom-6,r.right,r.bottom-2);
	g.DrawButton(r,c,s);
	g.SetClip(XRect(0,0,m_rect.Width(),m_rect.Height()));
	g.SetColor(c);	
	g.DrawLine(bx+1,y,bx+p->nWidth-2,y);
	bx+=4;
	if(p->pImage!=XNULL)
	{
		int dy=(m_nTextHeight-p->pImage->Height())>>1;
		p->pImage->SetActive(XMIN(p->pImage->FrameCount()-1,
								  p->nActive));
		g.DrawImage(*p->pImage,XPoint(bx,by+dy));
		bx+=p->pImage->Width();
	}
	XColor cc(XWindow::m_nColors[XCW_TEXT]);
	if(id==m_nActive)
	{
	  XColor cm(c);
	  cm.Bright(40);
	  cm-=cc;
	  g.SetColor(cm);
	  g.DrawString(p->strTitle,bx+1,by+1);
	}
	
	cc.Mix(XColor(c),id==m_nActive?0.3f:0.7f);
	//cc.Dark(id==m_nActive?70:40);
	g.SetColor(cc);
	g.DrawString(p->strTitle,bx,by);
}

void XTabControl::SetActive(XU8 nID)
{
    if(nID>=m_tabs.GetSize()||
	   nID==m_nActive) return;
	m_nActive=nID;
	SetOffset();
	Invalidate();
}

void XTabControl::OnMouseDown(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(m_tabs.GetSize()<=m_nOffset) return;
	int bx=m_tabs[m_nOffset]->nPosX;
    for(XU8 i=0;i<m_tabs.GetSize();i++)
	{
		if(pt.x>m_tabs[i]->nPosX-bx&&
		   pt.x<m_tabs[i]->nPosX-bx+m_tabs[i]->nWidth)
		{
			if(m_nActive!=i)
			{
				m_nActive=i;
				SetOffset();
				Invalidate();
				PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
			}
		}
	}
}

void XTabControl::OnBlur()
{
	XWindow::OnBlur();
	Invalidate();
}

void XTabControl::OnFocus()
{
	XWindow::OnFocus();
	Invalidate();
}

XBOOL XTabControl::OnKeyDown(XKEYMSG&m)
{
	switch(m.nKeyCode)
   {
   case XK_LEFT:
	    if(m_tabs.GetSize()<2) return XTRUE;
		if(m_nActive>0) m_nActive--;
		else m_nActive=m_tabs.GetSize()-1;
	    break;
   case XK_CLEAR:
	    PostMsg(XWM_COMMAND,XCMD_CLOSE,(XU32)this,XMS_PARENT);
	    return XTRUE;
   case XK_RIGHT:
	    if(m_tabs.GetSize()<2) return XTRUE;
		if(m_nActive+1<(XU8)m_tabs.GetSize())
			m_nActive++;
		else
			m_nActive=0;
		
		break;
   default:
	   {
		   int id=m.nKeyCode-XK_NUM1;
		   if(id>=0&&id<(XINT)m_tabs.GetSize())
		   { 
			   SetActive(id);
			   PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
			   return XTRUE;
		   }
	   }return XFALSE;
   //case XK_NUM8:
   case XK_WELL:
   case XK_RETURN:
   case XK_DOWN:
	    //PostMsg(XWM_SETACTIVE,0,(XU32)this);
	    if(m_pParent!=XNULL)
			m_pParent->ChangeActive();
		return XTRUE;
   }
   SetOffset();
   Invalidate();
   PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
   return XTRUE;
}

XBOOL XTabControl::SetTabTitle(XU8 nID, XPCWSTR strTitle)
{
	if(nID>=m_tabs.GetSize()) return XFALSE;
	m_tabs[nID]->strTitle=strTitle;
	if(m_tabs[nID]->strTitle.GetLength()>20)
		m_tabs[nID]->strTitle.RemoveAt(20,m_tabs[nID]->strTitle.GetLength()-20);
	m_tabs[nID]->strTitleLong=m_tabs[nID]->strTitle;//strTitle;
	m_bLayerout=XTRUE;
	Invalidate();
	return XTRUE;
}

void XTabControl::OnTimer()
{
	XBOOL bSet=XFALSE;
    for(XU32 i=0;i<m_tabs.GetSize();i++)
	{
		if(m_tabs[i]->pImage==XNULL||
		   m_tabs[i]->nActive==0) continue;
		m_tabs[i]->nActive--;
		if(m_tabs[i]->nActive>=m_tabs[i]->pImage->FrameCount()) continue;
		bSet=XTRUE;
		
	}
	if(bSet) Invalidate();
}

XBOOL XTabControl::ActiveStatus(XU8 nID)
{
   if(nID>=m_tabs.GetSize()) return XFALSE;
   if(m_tabs[nID]->pImage==XNULL||
	  m_tabs[nID]->pImage->FrameCount()<2) return XFALSE;
   m_tabs[nID]->nActive=XMAX(m_tabs[nID]->pImage->FrameCount(),5);
   
   Invalidate();
   return XTRUE;
}

XU32 XTabControl::GetTabData(XU8 nID)
{
	if(nID>=m_tabs.GetSize()) return 0;
	return m_tabs[nID]->nData;
}



XBOOL XTabControl::Create(XRect &rect, XWindow *pWnd, XU32 nCmd)
{
	return XWindow::Create(rect,XES_VISIABLE,pWnd,nCmd);
}

void XTabControl::FormatTitle(XGraphics&g,XString16 &strTitle)
{
	if(strTitle.GetLength()<=0) return;
	DOMPOSARRAY list;
	g.FormatString(strTitle,strTitle.GetLength(),0,m_nTextHeight<<1,list,XFMT_ONELINE);
	if(list.GetSize()>3||list[1]<(DOMPOS)strTitle.GetLength())
	{
		strTitle.RemoveAt(list[1],strTitle.GetLength()-list[1]);
		strTitle+=XString16("..");
	}
}

XPCWSTR XTabControl::GetTabTitle(XU8 nID)
{
	if(nID>=m_tabs.GetSize()) return XNULL;
	if(m_tabs[nID]->strTitleLong.GetLength()>0) return m_tabs[nID]->strTitleLong;
	return m_tabs[nID]->strTitle;
}

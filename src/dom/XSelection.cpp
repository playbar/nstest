// XSelection.cpp: implementation of the XSelection class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XSelection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSelection::XSelection()
{
   m_nSteps=1;
   m_nDirect=XK_DOWN;
}

XSelection::~XSelection()
{
   m_list.FreeAll();
}

void XSelection::Init(DRAWCONTEXT*pDraw,XDomItem *p, const XSIZE &sz,XU8 dir,XU8 nSteps)
{
	m_pDraw=pDraw;
   m_nDirect=dir;
   m_nSteps=nSteps;
   if(p!=XNULL)
	  p->GetRoundRect(m_select);
   else
   {
	  switch(dir)
	  {
	  case XK_DOWN:
		   m_select=XRect(0,-1,sz.cx,0);
		   break;
	  case XK_UP:
		   m_select=XRect(0,sz.cy,sz.cx,sz.cy+1);break;
	  case XK_RIGHT:
		   m_select=XRect(sz.cx,0,sz.cx+1,sz.cy);break;
	  case XK_LEFT:
		   m_select=XRect(-1,0,1,sz.cy);break;
	  }
   }
}

XU32 XSelection::Select(XDomItem *pItem)
{
	if(!(pItem->GetStyle()&XDomItem::DOM_SELECTABLE)) return XFALSE;
	XRect rect;
	pItem->GetRoundRect(rect);
	rect.DeflateRect(2,2);
	if(rect.IsRectEmpty()) return XFALSE;
	int nLength=0,nDelta=0;
	//int dd=XWindow::m_nTextHeight;
	XRect rc=rect;
	rc-=m_pDraw->offset;
	
	switch(m_nDirect)
	{
	case XK_LEFT:
	case XK_NUM4:
		 if(//rc.top>=m_pDraw->view.bottom||
			//rc.bottom<=m_pDraw->view.top||

		    rect.right>m_select.left||
			rect.bottom<m_select.top||
			rect.top>m_select.bottom) return XFALSE;
		 nLength=(m_select.left-rect.right)>>4;
		 nDelta=Delta(rect.top,rect.bottom,m_select.top,m_select.bottom);//  //XABS((rect.top-m_select.top));
		 break;
	case XK_NUM2:
	case XK_UP:
		 if(rc.left>=m_pDraw->view.right||
			rc.right<=m_pDraw->view.left||
		 
			rect.bottom>m_select.top/*||
			rect.left>m_select.right||
			rect.right<m_select.left*/) return XFALSE;
		 nLength=(m_select.top-rect.bottom)>>4;
		 nDelta=Delta(rect.left,rect.right,m_select.left,m_select.right);//XABS((rect.left-m_select.left));	
		 break;
	case XK_RIGHT:
	case XK_NUM6:
		 if(//rc.top>=m_pDraw->view.bottom||
			//rc.bottom<=m_pDraw->view.top||
		 
		    rect.left<m_select.right||
			rect.bottom<m_select.top||
			rect.top>m_select.bottom) return XFALSE;
		 nLength=(rect.left-m_select.right)>>4;
		 //nDelta=XABS((rect.top-m_select.top));
		 nDelta=Delta(rect.top,rect.bottom,m_select.top,m_select.bottom);
		 break;
	case XK_DOWN:
	case XK_NUM8:
		 if(rc.left>=m_pDraw->view.right||
			rc.right<=m_pDraw->view.left||
			rect.top<m_select.bottom/*||
			rect.left>m_select.right||
			rect.right<m_select.left*/) return XFALSE;
		 nLength=(rect.top-m_select.bottom)>>4;
		 //nDelta=XABS((rect.left-m_select.left));
		 nDelta=Delta(rect.left,rect.right,m_select.left,m_select.right);
		 break;
	}
	if(m_list.GetSize()>=m_nSteps&&
	   nLength>m_list[m_list.GetSize()-1]->nLength) return XFALSE;
	int nInsert=0;
	int id=Index((void*)nLength,nInsert);
	if(id>=0)
	{
	   if(m_list[id]->nDelta>nDelta)
	   {
		 m_list[id]->pItem=pItem;
		 m_list[id]->nDelta=nDelta;
	   }
	   else return XFALSE;
	}
	else
	{
		if(nInsert<(XINT)m_list.GetSize())
		{
			if(nDelta+
			   nLength>(m_list[nInsert]->nDelta)+m_list[nInsert]->nLength) return XFALSE;
		}
		XSI*pi=new XSI;
		pi->nDelta=nDelta;
		pi->nLength=nLength;
		pi->pItem=pItem;
		m_list.InsertAt(nInsert,pi);
		if(m_list.GetSize()>m_nSteps)
		{
			int id=m_list.GetSize()-1;
			delete m_list[id];
			m_list.RemoveAt(id);
		}
	}
	return XTRUE;
}

XDomItem* XSelection::GetSelect()
{
    if(m_list.GetSize()<=0) return XNULL;
	return m_list[m_list.GetSize()-1]->pItem;
}

XINT XSelection::Delta(int s0, int s1, int d0, int d1)
{
	if(s0<=d0)
	{
		if(s1>=d1) return 0;
		else if(s1>=d0) return 1;//1;
	}
	else
	{
		if(s1<=d1) return 0;
		else if(s0<=d1) return 1;//1;
	}
	return (XABS((s0-d0))>>4)+2;
}

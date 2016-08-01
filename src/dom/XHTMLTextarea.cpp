// XHTMLTextarea.cpp: implementation of the XHTMLTextarea class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLTextarea.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XHTMLTextarea::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_EVENT_ONMOUSEDOWN:
		 if(m_pEdit!=XNULL)
		 {
			m_bFocus=XTRUE;
			m_pEdit->SetFocus(XTRUE);
			m_pEdit->MouseDown(*(XPoint*)pData1,0);
			return SetPosition((XEVENTDATA*)pData2)|XRREPAINT;
		 }
		
		 break;
	case XDO_EVENT_ONCHAR:
		 if(m_pEdit==XNULL||!m_bFocus)
		 {
			 return 0;
		 }
		 break;
	case XDO_EVENT_DBLCLICK:
		{
			m_pEdit->OnDblClick(*(XPoint*)pData1,0);
			return SetRepaint((XEVENTDATA*)pData2,m_pEdit->GetWindowRect());
		}break;
	case XDO_EVENT_ONKEYDOWN:
		 if(m_pEdit==XNULL) break;
		 if(pData1==XK_RETURN)
		 {
			 XU32 s=0;
			 m_bFocus=!m_bFocus;
			 m_pEdit->SetFocus(m_bFocus);
			 XEVENTDATA*pData=(XEVENTDATA*)pData2;
			 if(!m_bFocus) 
				 s|=XRNEXT;
			 else
			   s|=SetPosition(pData);
			 return s|SetRepaint(pData,m_pEdit->GetWindowRect());
			 //return 0;
		 }
		 else
		 {
			 if(!m_bFocus) return 0;
			 return XDomInput::Handle(nOpera,pData1,pData2);
		 }break;
	case XDO_EVENT_ONBLUR:
		 if(m_pEdit!=XNULL)
		 {
			 m_bFocus=XFALSE;
			 m_pEdit->SetFocus(XFALSE);
			 return SetRepaint((XEVENTDATA*)pData1,m_pEdit->GetWindowRect());
		 }
		 return 0;
	case XDO_EVENT_ONFOCUS:
		 return 0;
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		{
		  XVar*p=(XVar*)pData1;
		  switch(p->nID)
		  {
		  case XEAB::ROWS:
		  case XEAB::COLS:
			   p->ToInt();
			   break;
		  }
		}break;*/
	case XDO_GET_STYLE:
		 if(m_bFocus)
			return DOM_SELECTABLE|DOM_INPUT|DOM_PAINT|DOM_NODE|DOM_EDITING;
		 else
			return DOM_SELECTABLE|DOM_INPUT|DOM_PAINT|DOM_NODE;
	case XDO_ADD_CHILD:
		 return XDomNode::Handle(nOpera,pData1,pData2);
	case XDO_LAYEROUT_CELLS:
		 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
	case XDO_LAYEROUT_CELL:
		 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
	case XDO_PAINT:
		 return Paint((DRAWCONTEXT*)pData1);
	}
	return XDomInput::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLTextarea::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData, XU8 bCells)
{
	XSize sz;
	LAYEROUTDATA margin;
	PreLayerout(pDraw,pData,&margin);
	if(m_pEdit==XNULL)
	{
		sz.cx=pDraw->win.Width()/pDraw->SPACE;
		sz.cy=XMAX(pDraw->win.Height()/pDraw->SPACE-3,6);
		sz.cx=FindAttrib(XEAB::COLS,sz.cx);
		sz.cy=FindAttrib(XEAB::ROWS,sz.cy);
		sz.cx=XMIN(sz.cx*pDraw->SPACE+4,pDraw->view.Width()-2);
		sz.cy=XMIN(sz.cy*pDraw->SPACE+4,pDraw->view.Height()-2);
	}
	else
	{
		sz=m_pEdit->GetWindowRect().Size();
		sz.cx+=4;
		sz.cy+=4;
	}

    //if(!pData->bReturn)
	if(bCells)
	   m_bReturn=NewRowIfMust(pDraw,pData,sz.cx);   
	else //if(m_bReturn)
		NewRowMust(pDraw,pData,m_bReturn);
	   
    XRect r(pData->cur.x+2,pData->cur.y+2,
		    pData->cur.x+sz.cx-2,pData->cur.y+sz.cy-2);
    SetRowSize(pDraw,pData,sz,XMAX(sz.cx>>1,pDraw->SPACE<<1),bCells);
   
    r.top=AlignCol(pDraw,pData,sz.cy,pDraw->ALIGNH)+1;
    r.bottom=r.top+sz.cy-4;
    if(m_pEdit==XNULL)//!m_edit.IsValid())
	{
		   XU8 nes=FindAttrib(XEAB::FORMAT,XEDS_ANY)|XEDS_MULTILINE;//m_edit.GetEditStyle();
		   m_pEdit=new XEdit();
		   m_pEdit->Create(r,XNULL,pDraw->pDraw,nes);
		   m_pEdit->SetFocus(XFALSE);		  
		   ResetText();
	}
	else
	   m_pEdit->MoveWindow(r);
	EndLayerout(pDraw,pData);
    return 0;
}

void XHTMLTextarea::ResetText()
{
	if(m_pEdit==XNULL) return;
	if(m_strTmp.GetLength()>0)
    {
	   m_pEdit->SetText(m_strTmp);
	   m_strTmp.Empty();
	   m_strTmp.FreeExtra();
    }
	else
	{
		XString16 strTxt;
		XDomList list;
		GetList(list,XDOM_TEXT);
		for(XU32 i=0;i<list.GetSize();i++)
			list[i]->GetText(strTxt);
		//XDomNode::GetText(strTxt);
		//for(XU32 i=0;i<m_childs.GetSize();i++)
		//	if(m_childs[i]->Handle(XDO_GET_TEXT,pData1,pData2))
		m_pEdit->SetText(strTxt);
	}
}

XBOOL XHTMLTextarea::Paint(DRAWCONTEXT *pDraw)
{
    if(m_pEdit==XNULL) return XFALSE;
	XBOOL bOK=XDomInput::Paint(pDraw);
	m_pEdit->SetFocus(m_bFocus);
	return bOK;
}

XU32 XHTMLTextarea::SetPosition(XEVENTDATA *pData)
{
	XRect&rt=m_pEdit->GetWindowRect();
	if(rt.bottom>=pData->viewRect.bottom||
	   rt.top<pData->viewRect.top||
	   rt.left<pData->viewRect.left||
	   rt.right>pData->viewRect.right)
	{
		pData->offset.x=rt.left;
		pData->offset.y=rt.top-
			XMAX((pData->viewRect.Height()-rt.Height())>>1,0);
		return XROFFSET;
	}
	return 0;
}

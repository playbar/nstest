// XHTMLButton.cpp: implementation of the XHTMLButton class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XHTMLButton::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_SELECT:
	      return (XU32)XDomItem::SelectItem((SELECTMARK*)pData1);
	case XDO_HITTEST:
		{
		  XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
		  if(rect.PtInRect(pData1,pData2))
			  return (XU32)this;
		}
		 return 0;
	case XDO_GET_STYLE:
		{
			XU32 s=XDomBlock::Handle(nOpera,pData1,pData2)|DOM_SELECTABLE;
			return s;
		}break;
	case XDO_PAINT:
		 return Paint((DRAWCONTEXT*)pData1);
	case XDO_EVENT_ONKEYDOWN:
		 if(pData1!=XK_RETURN) break;
	case XDO_EVENT_ONMOUSEUP:
		 return OnMouseUp((XEVENTDATA*)pData2);
	case XDO_EVENT_ONMOUSEOUT:
		 return OnMouseLeave((XEVENTDATA*)pData2);
	case XDO_EVENT_ONMOUSEDOWN:
		 return OnMouseDown(*(XPoint*)pData1,(XEVENTDATA*)pData2);
//	case XDO_ADD_CHILD:
//		 return XDomNode::Handle(nOpera,pData1,pData2);
//	case XDO_GET_STYLE:
//		 return XDomInput::Handle(nOpera,pData1,pData2)|DOM_NODE;
	case XDO_LAYEROUT_CELLS:
		 return LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
	case XDO_LAYEROUT_CELL:
		 return LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
	}
	return XDomBlock::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLButton::LayeroutCells(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	/*/if(m_nWidth<=0)
	{
		CELLDATA data;
		//pDraw->win.right-=8;
		data.Reset(0,0,pData->alignw,pData->alignh,pData->nowrap);
		XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
		m_nWidth=XMAX(data.max.cx+8,pDraw->nSpace);
		m_nHeight=XMAX(data.max.cy+7,pDraw->nSpace);
		//pDraw->win.right+=4;
	}*/
	//if(pData->cur.x+m_nWidth>pDraw->win.right&&!pData->bReturn&&pData->IsWrap())
	//m_bReturn=NewRowIfNeed(pDraw,pData,m_nWidth);
	//SetRowSize(pDraw,pData,XSize(m_nWidth,m_nHeight),m_nWidth,XTRUE);
	XU8 nPad=pDraw->PADDING;
//	XU8 nw=pData->nowrap;
	pDraw->PADDING+=3;
//	pData->nowrap=XEA::NOWRAP;
//	XRect rt=pDraw->win;
//	pDraw->win.left=pData->cur.x;
//	pDraw->win.right=pDraw->win.left+pDraw->view.Width();
	XDomBlock::LayeroutCells(pDraw,pData,XFALSE);
//	pDraw->win=rt;
	pDraw->PADDING=nPad;
//	pData->nowrap=nw;	
	return 0;
}

XU32 XHTMLButton::LayeroutCell(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	XU8 nPad=pDraw->PADDING;
//	XU8 nw=pData->nowrap;
	pDraw->PADDING+=3;
//	
	XDomBlock::LayeroutCell(pDraw,pData,XFALSE);
	
	pDraw->PADDING=nPad;
//	pData->nowrap=nw;	
	return 0;
}

XU32 XHTMLButton::Paint(DRAWCONTEXT *pDraw)
{
	if(m_nWidth==0) return XFALSE;
	XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	if(!rect.IsUnion(pDraw->paint)) return XFALSE;
	//
	rect.DeflateRect(1,1);

	XU32 bc=pDraw->pDraw->GetBackColor();
	XU32 tc=pDraw->pDraw->GetColor();
	XGraphics*p=pDraw->pDraw;
	
	XU8 s=XBS_BFRAME;
	rect.InflateRect(1,1);
	PaintSelect(rect,pDraw);
	rect.DeflateRect(1,1);
	int dx=1,dy=1;
	if(m_nStatus==0) {s|=XBS_THICK;dx=0;dy=0;}
	XColor cc(pDraw->DCBACK);
	if(pDraw->DCBACK==0)
		cc=XWindow::GetSysColor(XCW_BACK);
	cc.Dark(10);
	XRect rtt=rect;
	p->DrawButton(rect,cc,s);//pDraw->DCBUTTON,s);
	
	p->SetBackColor(cc);//pDraw->DCBUTTON);
	p->SetColor(tc);
	PaintBack(pDraw,rtt);
	XU32 s1=0;
	if(dx==0)
		s1=XDomNode::Handle(XDO_PAINT,(XU32)pDraw,0);//(dy<<8)|dx);
	else
	{
		int mx,my;
		p->GetOrigin(mx,my);
		mx+=dx;
		my+=dy;
		p->SetOrigin(mx,my);
		s1=XDomNode::Handle(XDO_PAINT,(XU32)pDraw,0);//(dy<<8)|dx);
		mx-=dx;
		my-=dy;
		p->SetOrigin(mx,my);
	}

	p->SetBackColor(bc);
	return s1;
}

XU32 XHTMLButton::OnMouseDown(XPoint &pt, XEVENTDATA *pData)
{
    m_nStatus=1;
	XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	XU32 s=XRREPAINT;
	pData->rect|=rect;
	return s;
}

XU32 XHTMLButton::OnMouseLeave(XEVENTDATA *pData)
{

	m_nStatus=0;
	XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	XU32 s=XRREPAINT;
	pData->rect|=rect;
	return s;
}

XU32 XHTMLButton::OnMouseUp(XEVENTDATA *pData)
{
	XU32 s=0;
	s=OnMouseLeave(pData);
	XDomItem*p=GetParent();
	if(p!=XNULL)
	{
		PostVariable(pData);
		s|=p->Handle(XDO_SUBMIT,
					 0,(XU32)pData);
	}
	return s;
}
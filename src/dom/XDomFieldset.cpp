// XDomFieldset.cpp: implementation of the XDomFieldset class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomFieldset.h"
#include "XEnumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XDomFieldset::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_GET_RECT:
		 if(!IsVisiable()) return 0;
		 *((XRect*)pData1)=XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
		 return XTRUE;
	case XDO_HITTEST:
		 if(!IsVisiable()) return 0;
		{
		  XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
		  if(!rect.PtInRect(pData1,pData2)) return 0;
		}break;
	case XDO_GET_ATTR_COUNT:
		 return XDomNode::Handle(nOpera,pData1,pData2)+1;
	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 case XEnumAttribs::TITLE:
			  m_strTitle=p->ToString();
			  return XTRUE;
		 }
		 break;
	case XDO_PAINT:
		 if(!IsVisiable()) return 0;
		 Paint((DRAWCONTEXT*)pData1);
		 break;
	case XDO_GET_ATTR:
		 switch(p->nID)
		 {
		    case XEnumAttribs::TITLE:
				 *p=m_strTitle;
				 return XTRUE;
		 }
		 break;
	case XDO_LAYEROUT_CELLS:
		 if(!IsVisiable()) return 0;
		 Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_LAYEROUT_CELL:
		 if(!IsVisiable()) return 0;
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE; 
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

void XDomFieldset::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
/*	CELLDATA data;
	XRect rwin=pDraw->win;
	XSize sz(0,8);
	if(m_strTitle.GetLength()>0)
		pDraw->pDraw->MeasureString(m_strTitle,sz);
	m_nTextHeight=sz.cy;
	m_nTextWidth=sz.cx;
	sz.cx+=pDraw->nSpace;
	m_nPosX=pData->cur.x;
	//pData->cur.y;

	int bx=m_nPosX+4,by=m_nPosY+sz.cy;
	pDraw->win=XRect(bx,by,
					 pDraw->win.right,pDraw->win.bottom);
	data.Reset(bx,by,pData->alignw,pData->alignh,pData->nowrap,data.ind);
	XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
	
	data.max.cy-=m_nPosY;
	data.max.cx-=m_nPosX;
	data.max.cx+=8;
	if(data.max.cx<sz.cx) data.max.cx=sz.cx;
	data.max.cy+=4;
	m_nWidth=data.max.cx;
	m_nHeight=data.max.cy;

	m_nPosY=AlignCol(pDraw,pData,m_nHeight,pData->alignh);

	if(data.max.cx>rwin.right&&!pData->bReturn&&pData->IsWrap())
		NewRow(pDraw->nRowSpace,pDraw,pData,bCells);		
	SetRow(pDraw,pData,data.max,data.nMin,bCells);
	if(!bCells)
	{
	  data.Reset(bx,by,data.alignw,data.alignh,data.nowrap,data.ind);
	  AlignRow(pDraw,&data);
	  XDomNode::Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
	}		
	pDraw->win=rwin;*/

}

void XDomFieldset::LayeroutCells(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
/*	CELLDATA data;
	XRect rwin=pDraw->win;
	XSize sz(0,8);
	if(m_strTitle.GetLength()>0)
		pDraw->pDraw->MeasureString(m_strTitle,sz);
	m_nTextHeight=sz.cy;
	m_nTextWidth=sz.cx;
	sz.cx+=pDraw->nSpace;
	m_nPosX=pData->cur.x;
	//pData->cur.y;

	int bx=m_nPosX+4,by=m_nPosY+sz.cy;
	pDraw->win=XRect(bx,by,
					 pDraw->win.right,pDraw->win.bottom);
	data.Reset(bx,by,pData->alignw,pData->alignh,pData->nowrap,data.ind);
	XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
	
	data.max.cy-=m_nPosY;
	data.max.cx-=m_nPosX;
	data.max.cx+=8;
	if(data.max.cx<sz.cx) data.max.cx=sz.cx;
	data.max.cy+=4;
	m_nWidth=data.max.cx;
	m_nHeight=data.max.cy;

	m_nPosY=AlignCol(pDraw,pData,m_nHeight,pData->alignh);

	if(data.max.cx>rwin.right&&!pData->bReturn&&pData->IsWrap())
		NewRow(pDraw->nRowSpace,pDraw,pData,bCells);		
	SetRow(pDraw,pData,data.max,data.nMin,bCells);
	if(!bCells)
	{
	  data.Reset(bx,by,data.alignw,data.alignh,data.nowrap,data.ind);
	  AlignRow(pDraw,&data);
	  XDomNode::Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
	}		
	pDraw->win=rwin;*/

}

void XDomFieldset::Paint(DRAWCONTEXT*pDraw)
{
    XU32 tc=pDraw->pDraw->GetColor();
	XU32 bc=pDraw->pDraw->GetBackColor();

	XColor cc(pDraw->DCBACK);
	XColor dc(cc);
	cc.Bright(30);
	dc.Dark(30);
	XRect rect(m_nPosX+2,m_nPosY+1+(m_nTextHeight>>1),m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	pDraw->pDraw->SetColor(cc);
	pDraw->pDraw->FrameRectangle(rect);
	pDraw->pDraw->SetColor(dc);
	rect-=XPoint(1,1);
	pDraw->pDraw->FrameRectangle(rect);
	
	if(m_strTitle.GetLength()>0)
	{
		pDraw->pDraw->SetBackColor(pDraw->DCBACK);
		pDraw->pDraw->SetColor(pDraw->DCTEXT);
		pDraw->pDraw->FillRectangle(
			XRect(m_nPosX+6,m_nPosY,m_nPosX+6+m_nTextWidth+4,m_nPosY+m_nTextHeight));
		pDraw->pDraw->DrawString(m_strTitle,m_nPosX+6,m_nPosY+1);
	}

	pDraw->pDraw->SetColor(tc);
	pDraw->pDraw->SetBackColor(bc);
}

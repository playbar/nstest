// XDomP.cpp: implementation of the XDomP class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomP.h"
#include "XEnumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomP::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(!IsVisiable()) return 0;
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_SET_FINAL:
		 return Final();
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 case XEnumAttribs::MODE:
		 case XEnumAttribs::ALIGN:
			  if(p->nType==VSTRING) 
				  *p=XEnumAlign().Index(p->strData,XSS_NOCASE);
			  break;
		 }
		 break;*/
	case XDO_LAYEROUT_CELL:
		 if(!IsVisiable()) return 0;
		 LayerOutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		 return XTRUE;
		 //break;
		 //return XTRUE;
	case XDO_LAYEROUT_CELLS:
		 if(!IsVisiable()) return 0;
		 LayerOutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
		 return XTRUE;
	}

	return XDomNode::Handle(nOpera,pData1,pData2);
}

//DEL void XDomP::LayerOutCell(DRAWCONTEXT *pDraw, CELLDATA *pData)
//DEL {
//DEL    	/*CELLDATA data;
//DEL 	data.nRows=data.nRow=data.nCol=0;
//DEL 	data.align=FindAttrib(XEnumAttribs::ALIGN,XEnumAlign::CENTER);
//DEL 	pData->cur.x=data.cur.x=pDraw->win.left;
//DEL 	pData->cur.y+=pDraw->nParaSpace>>1;
//DEL 	data.cur.y=pData->cur.y;
//DEL 	data.max.cx=data.max.cy=0;
//DEL 	for(XU32 i=0;i<m_childs.GetSize();i++)
//DEL 		m_childs[i]->LayeroutItem(pDraw,&data,XTRUE);//Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
//DEL 	data.max.cx=data.max.cy=0;
//DEL 	data.nRow=data.nCol=0; 
//DEL 	data.cur.y=pData->cur.y;
//DEL 	data.cur.x=pData->cur.x;
//DEL 	AlignRow(pDraw,&data);
//DEL 	for(i=0;i<m_childs.GetSize();i++)
//DEL 		m_childs[i]->LayeroutItem(pDraw,&data,XFALSE);//Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
//DEL 	/*if(data.nRows<=0)
//DEL 		pData->cur.y+=pDraw->nRowSpace;
//DEL 	else if(data.nRows<data.rows.GetSize())
//DEL 		pData->cur.y=data.rows[data.nRows-1];
//DEL 	else
//DEL 		pData->cur.y=data.cur.y+pDraw->nSpace;
//DEL 	pData->cur.y+=pDraw->nParaSpace>>1;
//DEL 	if(pData->max.cx<data.max.cx)
//DEL 		pData->max.cx=data.max.cx;
//DEL 	//if(pData->max.cy<data.max.cy)
//DEL 	pData->max.cy=pData->cur.y;
//DEL 	//NewRow(pDraw,pData,XFALSE);
//DEL 	//data.max.cy+=pDraw->nParaSpace>>1;
//DEL 	SetRow(pDraw,pData,data.max,XFALSE);
//DEL 	NewRow(pDraw,pData,XFALSE);
//DEL 	//pData->max.cx=XMAX(pData->max.cx,data.max.cx);
//DEL 	//pData->max.cy=XMAX(pData->max.cy,data.max.cy);
//DEL 	/*if(pData->max.cx<data.max.cx)
//DEL 		pData->max.cx=data.max.cx;
//DEL 	if(pData->max.cy<data.max.cy)
//DEL 		pData->max.cy=data.max.cy;*/
//DEL 	//NewRow(pDraw,pData,XFALSE);
//DEL 	XU8 align=pData->align;
//DEL 	pData->align=FindAttrib(XEnumAttribs::ALIGN,XEnumAlign::LEFT);
//DEL 
//DEL 	SetRow(pDraw,pData,XSize(pDraw->win.right-pDraw->win.right,
//DEL 						     pDraw->nRowSpace+pDraw->nSpace),XFALSE);
//DEL 	XDomNode::Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)pData);
//DEL 	NewRow(pDraw,pData,XFALSE);
//DEL 	SetRow(pDraw,pData,XSize(0,pDraw->nRowSpace+pDraw->nSpace),XFALSE);
//DEL 	pData->align=align;
//DEL 
//DEL }

void XDomP::LayerOutCells(DRAWCONTEXT *pDraw, CELLDATA *pData,XBOOL bCells)
{
	//XU8 align=pData->alignw,nowrap=pData->nowrap;
	//pData->alignw=FindAttrib(XEnumAttribs::ALIGN,XEnumAlign::LEFT);
	//pData->nowrap=FindAttrib(XEAB::MODE,XEA::WRAP);
	//LAYEROUTDATA margin;
	//PreLayerout(pDraw,pData,XNULL);//margin);

	if(!pData->bNewLine)
	  NewRow(0,pDraw,pData,pDraw->IND); 
	//else if(!bCells)
	//	AlignRow(pDraw,pData);
	//pData->bReturn=XFALSE;
	HandleChild(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)pData);

	if(!pData->bNewLine)
		NewRow(0,pDraw,pData,pDraw->IND);
	//EndLayerout(pDraw,pData);
}

//DEL XBOOL XDomP::Final()
//DEL {
//DEL 	return XTRUE;
//DEL }

XBOOL XDomP::Final()
{
	XU32 i;
	for(i=m_childs.GetSize();i>0;i--)
	{
		if(m_childs[i-1]->GetID()==XDOM_BR)
			m_childs.RemoveAt(i-1);
		else break;
	}
	i=0;
	while(i<m_childs.GetSize())
	{
	   if(m_childs[i]->GetID()==XDOM_BR)
		   m_childs.RemoveAt(i);
	   else break;
	}
	return XTRUE;
}

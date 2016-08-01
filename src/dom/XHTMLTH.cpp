// XHTMLTH.cpp: implementation of the XHTMLTH class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLTH.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XHTMLTH::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
		case XDO_PAINT_TABLE:
		case XDO_LAYEROUT_TAB:
		case XDO_LAYEROUT_TABS: 
			{
			  
			  DRAWCONTEXT*p=(DRAWCONTEXT*)pData1;
			  
			  //if(pData2!=0)
			  //{
			//	CELLDATA*pd=(CELLDATA*)pData2;
			//	pd->alignw=FindAttrib(XEAB::ALIGN,XEA::CENTER);
			 // }
			  //XU8 aw=p->ALIGNW;
			  //p->ALIGNW=XEA::CENTER;
			  XU32 s=p->pDraw->SetBold(XTRUE);
			  //XU32 ss=PrePaint(p);
			  XU32 r=XDomTD::Handle(nOpera,pData1,pData2);
			  ///EndPaint(p,ss);
			  //p->ALIGNW=aw;
			  p->pDraw->SetFontStyle(s);
			  
			  return r;
			}
			break;
	}
	return XDomTD::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLCaption::Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
{
	
	switch(nOpera)
	{
	   case XDO_PAINT_TABLE:
		    if(IsVisiable())
		   {
			   DRAWCONTEXT*p=(DRAWCONTEXT*)pData1;
			   XU8 n=p->TABBORDER;
			   p->TABBORDER=0;
			   XU32 r=XDomTD::Paint(p,pData2);
			   p->TABBORDER=n;
			   return r;
		   }break;
	   case XDO_LAYEROUT_TABS:
		    if(!IsVisiable()) return 0;
		    LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
			return 0;
	   case XDO_LAYEROUT_TAB:
		    if(!IsVisiable()) return 0;
		    LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
			return 0;
	}
	return XDomTD::Handle(nOpera,pData1,pData2);
}

void XHTMLCaption::LayeroutCell(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
//   XU8 aw=pData->alignw;
 //  XU8 ah=pData->alignh;
 //  pData->alignw=FindAttrib(XEAB::ALIGN,XEA::CENTER);
 //  pData->alignh=FindAttrib(XEAB::ALIGN,XEA::MIDDLE);
   //PreLayerout(pDraw,pData);
//   pDraw->ALIGNW=XEA::CENTER;
//   pDraw->ALIGNH=XEA::MIDDLE;
   pData->nCol=0;
   NewTabRow(pDraw,pData);
   pData->spans.RemoveAll();
   //int span=XMAX(pData->c
   XDomTD::LayeroutCell(pDraw,pData,2);
   //XDomNode::Handle(XDO_LAYEROUT_TAB,(XU32)pDraw,(XU32)pData);
   //RowSpan(pDraw,pData,XFALSE);
   pData->cur.x=pDraw->win.left;
   if(!pData->bEmpty)
	   pData->cur.y+=pData->rowws[pData->nRow];	
   pData->nRow++;
   pData->nCol=0;
   //pData->alignh=ah;
   //pData->alignw=aw;
}

void XHTMLCaption::LayeroutCells(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
//   XU8 aw=pData->alignw;
//   XU8 ah=pData->alignh;
//   pData->alignw=FindAttrib(XEAB::ALIGN,XEA::CENTER);
//   pData->alignh=FindAttrib(XEAB::ALIGN,XEA::MIDDLE);
//	pDraw->ALIGNW=XEA::CENTER;
//	pDraw->ALIGNH=XEA::CENTER;
	//LAYEROUTDATA margin;
	//PreLayerout(pDraw,pData,&margin);

   pData->nCol=0;
   //if(!pData->bFinal)
	NewTabRow(pDraw,pData);

   pData->spans.RemoveAll();

   int span=XMAX(pData->cols.GetSize(),2);
   XDomTD::LayeroutCells(pDraw,pData,span);

   //XDomNode::Handle(XDO_LAYEROUT_TABS,(XU32)pDraw,(XU32)pData);
   
   if(pData->bFinal&&(XINT)pData->rowws.GetSize()>pData->nRow)
   {
	  pData->cur.x=pDraw->win.left;
      pData->cur.y+=pData->rowws[pData->nRow];
	  pData->nRow++;
   }
  // EndLayerout(pDraw,pData);

}

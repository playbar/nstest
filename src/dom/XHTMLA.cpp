// XHTMLA.cpp: implementation of the XHTMLA class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLA.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XHTMLA::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{

	case XDO_GET_RECT:
		 if(IsInline()) return XDomA::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_HITTEST:
		 if(IsInline()) return XDomA::Handle(nOpera,pData1,pData2);
		 else //if(XDomBlock::Handle(nOpera,pData1,pData2))
		 {
			 XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
			 if(rect.PtInRect(pData1,pData2)) return (XU32)this;
		 }
		 return 0;
	case XDO_SET_FINAL:
		 if(IsInline()) return XDomA::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_LAYEROUT_CELL:
		 if(IsInline())
		 {
			return LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		 }	
		 else
			 return XDomBlock::LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		 break;
	case XDO_LAYEROUT_CELLS:
		 if(IsInline())
		 {
			return LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
		 }	
		 else 
			 return XDomBlock::LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		 break;
	case XDO_UPDATE_TABLE:
		 if(IsInline()) return XDomA::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_PAINT:
		 if(IsInline()) return XDomA::Handle(nOpera,pData1,pData2);
		 else
		 {
			 DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			 pDraw->nStatus=GetStatus(pDraw);
			 XU32 ss=XDomBlock::Handle(nOpera,pData1,pData2);
			 pDraw->nStatus=0;
			 return ss;
		 }
		 break;
		 //return 0;
	}
	return XDomBlock::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLA::LayeroutCell(DRAWCONTEXT *pDraw, CELLDATA *pData,XU8 bCells)
{
	if(m_childs.GetSize()<=0)
	{
		XVar var((int)XEAB::POSX,(int)pData->cur.x);
		XDomLeaf::SetAttrib(var);
		var.nID=XEAB::POSY;
		var.iData=pData->cur.y;
		XDomLeaf::SetAttrib(var);
		return 0;
	}
	LAYEROUTDATA margin;
	PreLayerout(pDraw,pData,&margin);
	pData->nSubWidth=pDraw->SETWIDTH;
	pData->nSubHeight=pDraw->SETHEIGHT;
	XU32 s=HandleChild(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,
				(XU32)pDraw,(XU32)pData);
	pData->nSubWidth=0;
	//pData->nSubHeight=0;
	EndLayerout(pDraw,pData);
	pData->pData=XNULL;
	return s;
}

XBOOL XHTMLA::IsInline()
{
	for(int i=0;i<(XINT)m_attribs.GetSize();i++)
	{
		switch(m_attribs[i].nID&0x3fff)
		{
		case XEAB::DISPLAY_MODE:
			 switch(m_attribs[i].iData)
			 {case XELT::DISPLAY_BLOCK:
			  case XELT::DISPLAY_LIST:
			  case XELT::DISPLAY_TABLE:return XFALSE;}
			 break;
		case XEAB::WIDTH:
		case XEAB::HEIGHT:return XFALSE;
		case XEAB::FLOAT:
			 if(m_attribs[i].iData!=XELT::FLOAT_NONE) return XFALSE;
			 break;
		//case XEAB::BGCOLOR:
		//	 if(m_attribs[i].iData) return XFALSE;
		//	 break;
		//case XEAB::BACKIMAGE:
		//	 return XFALSE;
		}
	}
	return XTRUE;
}

// XDomBR.cpp: implementation of the XDomBR class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomBR.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XDomBR XDomBR::m_br;

XU32 XDomBR::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_LAYEROUT_CELL:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		 return XTRUE;
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}

void XDomBR::LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA *pData,XBOOL bAdd)
{
	//if(!bAdd)
	//	int a=0;
	if(!pData->bEmpty)
	//SetRowSize(pDraw,pData,XSize(pDraw->SPACE,1),0,bAdd);
		NewRow(pDraw->SPACE,pDraw,pData,0);//pDraw->IND);
	//SetRow(pDraw,pData,XSize(0,pDraw->SPACE),bAdd);
}

//#if (__CORE_VERSION__>=0x02077000)
//XDomSBR XDomSBR::m_sbr;

//static XDomItem* _stdcall CreateObject(XU16 ID){return &m_sbr;}

XU32 XDomSBR::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_LAYEROUT_CELL:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		 return XTRUE;
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}

void XDomSBR::LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA *pData,XBOOL bAdd)
{
	//if(!bAdd)
	//	int a=0;
	if(!pData->bEmpty)
	//SetRowSize(pDraw,pData,XSize(pDraw->SPACE,1),0,bAdd);
		NewRow(pDraw->SPACE,pDraw,pData,0);//pDraw->IND);
	//SetRow(pDraw,pData,XSize(0,pDraw->SPACE),bAdd);
}

XDomSBR::XDomSBR(XU16 ID):XDomLeaf(XDOM_SBR)
{
	//int a = 0;
}

XDomSBR::~XDomSBR()
{
	//int a = 0;
}

//#endif

XDomSpace XDomSpace::m_space;

XU32 XDomSpace::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
		{
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			CELLDATA*pData=(CELLDATA*)pData2;
			if(!pData->bNewLine&&pData->cur.x+2<pDraw->win.right)
				SetRowSize(pDraw,pData,XSize(2,pDraw->SPACE),0,
						   nOpera==XDO_LAYEROUT_CELLS);
		}break;
	}
	return XDomItem::Handle(nOpera,pData1,pData2);
}

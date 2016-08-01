// XHTMLObject.cpp: implementation of the XHTMLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XHTMLObject::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_PAINT:
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
		{
			return 0;
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			CELLDATA*pData=(CELLDATA*)pData2;
			int w=pDraw->SETWIDTH;//FindAttrib(XEAB::WIDTH,100);
			int h=pDraw->SETHEIGHT;//FindAttrib(XEAB::HEIGHT,60);
			if(w<0) w=-w*pDraw->win.Width()/100;
			if(h<0) h=-h*pDraw->win.Height()/100;
			int ws=FindAttrib(XEAB::HSPACE,0);
			int hs=FindAttrib(XEAB::VSPACE,0);
			if(ws<0) ws=-ws*pDraw->win.Width()/100;
			if(hs<0) hs=-hs*pDraw->win.Height()/100;
			w+=ws<<1;
			h+=hs<<1;
			//if(!pData->bReturn&&pData->cur.x+w>pDraw->win.right)
			if(!pData->bNewLine)//Return)
				NewRow(0,pDraw,pData,pDraw->IND);//==XDO_LAYEROUT_CELLS);
			m_nPosX=pData->cur.x+ws;
			m_nPosY=pData->cur.y+hs;
			SetRowSize(pDraw,pData,XSize(w,h),w,nOpera==XDO_LAYEROUT_CELLS);
			
		}return XTRUE;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}



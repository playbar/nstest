// XHTMLSup.cpp: implementation of the XHTMLSup class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLSup.h"
#include "XDomView.h"
#include "XDomText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XHTMLSup::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_PAINT:

	case XDO_LAYEROUT_CELL:
	case XDO_LAYEROUT_CELLS:
		{
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			//XU32 ss=PrePaint(pDraw);
			//CELLDATA*pData=(CELLDATA*)pData2;
			int dx=0;
			if(m_nID==XDOM_SUP) dx=-(pDraw->SPACE>>1)-4;
			else dx=(pDraw->SPACE>>2)-4;
			XU32 nData=pDraw->DATA;
			XU32 st=pDraw->pDraw->GetFontStyle();
			XU8 sz=(XU8)pDraw->pDraw->GetFontSize();
			if(sz>2) sz-=2;
			else if(sz>1) sz--;
			pDraw->pDraw->SetFontSize(sz);
			pDraw->DATA=dx;
			XU32 s=XDomNode::Handle(nOpera,pData1,pData2);
			pDraw->pDraw->SetFontStyle(st);
			pDraw->DATA=nData;
			//EndPaint(pDraw,ss);
			return s;
			
		}break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLTitle::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_SET_FINAL:
		 if(pData1!=XNULL)
		 {
			 DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			 //XString16 strTitle;
			 //for(XU32 i=0;i<m_childs.GetSize();i++)
			 //	if(m_childs[i]->Handle(XDO_GET_TEXT,(XU32)&strTitle,0)) return XTRUE;
			 XDomList list;
			 GetList(list,XDOM_TEXT);
			 if(list.GetSize()>0)
			 {
				pDraw->pView->SetTitle(((XDomText*)list[0])->m_strTxt);
				//if(strTitle.GetLength()>0)
				//	pDraw->pView->SetTitle(strTitle);
			 }
		 }
		 break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLCenter::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
	case XDO_LAYEROUT_TAB:
	case XDO_LAYEROUT_TABS:
		if(IsVisiable()) 
		{
			DRAWCONTEXT*p=(DRAWCONTEXT*)pData1;
			CELLDATA*pData=(CELLDATA*)pData2;
			XU8 a=p->ALIGNW;
			p->ALIGNW=XEA::CENTER;
			if(nOpera==XDO_LAYEROUT_CELL||nOpera==XDO_LAYEROUT_TAB)
				AlignRow(p,pData);	
			//NewRow(0,p,(CELLDATA*)pData2,0);
			XDomNode::Handle(nOpera,pData1,pData2);
			p->ALIGNW=a;
		}return XTRUE;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLSpan::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{

	case XDO_GET_RECT:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_HITTEST:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_SET_FINAL:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_LAYEROUT_CELL:
	case XDO_LAYEROUT_CELLS:
//	    if(FindAttrib(XEAB::CLASS,0)==16386)
//		   int a=0;

		 if(IsInline())
		 {
			 //return XDomNode::Handle(nOpera,pData1,pData2);
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			CELLDATA*pData=(CELLDATA*)pData2;
			
			LAYEROUTDATA margin;
			PreLayerout(pDraw,pData,&margin);
			XRect win=pDraw->win;
			if(pDraw->SETWIDTH>0)
				pDraw->win.right=pDraw->win.left+pDraw->SETWIDTH;
			if(m_nID==XDOM_P&&!pData->bNewLine)
				NewRow(0,pDraw,pData,pDraw->IND);
			pData->pData=XNULL;
			pData->cur.y+=margin.nMargin[BORDERDATA::TOP];
			XU32 s=HandleChild(nOpera,
						(XU32)pDraw,(XU32)pData);
			pDraw->win=win;
			EndLayerout(pDraw,pData);
			pData->cur.y+=margin.nMargin[BORDERDATA::BOTTOM];
			if(m_nID==XDOM_P&&!pData->bNewLine)
				NewRow(0,pDraw,pData,pDraw->IND);
			pData->pData=XNULL;
			return s;

		 }
		 break;
	case XDO_UPDATE_TABLE:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_PAINT:
		 if(IsInline())
			 return XDomNode::Handle(nOpera,pData1,(XU32)this);
		 break;
		 //return 0;
	}
	return XDomBlock::Handle(nOpera,pData1,pData2);
}

//DEL XU8 XHTMLSpan::IsInline()
//DEL {
//DEL     if(IsFingerBlock()) return XFALSE;
//DEL 	XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
//DEL 	if(ft) return XFALSE;
//DEL 	if(FindAttrib(XEAB::WIDTH,0)>0) return XFALSE;
//DEL 	//if(FindAttrib(XEAB::WIDTH,
//DEL 	return XTRUE;
//DEL }

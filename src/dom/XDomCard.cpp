	// XDomCard.cpp: implementation of the XDomCard class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomCard.h"
#include "XEnumData.h"
#include "XDomTemplate.h"
#include "XDomView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomCard::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_SET_FINAL:
		 m_nWidth=0;
		 break;
	case XDO_UPDATE_TABLE:
		 m_nWidth=0;
		 break;
/*	case XDO_SET_TITLE:
		 if(m_strTitle.GetLength()>0)
			 ((XDomView*)pData1)->SetTitle(m_strTitle);*/
	case XDO_SET_START:
		 /*if(pData1!=XNULL&&m_strTitle.GetLength()>0)
		 {
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			if(pDraw->pView!=XNULL)
				pDraw->pView->SetTitle(m_strTitle);
		 }
		 return XTRUE;*/break;
	//case XDO_SELECT:
		 //return SelectItem((SELECTMARK*)pData1);
	case XDO_EVENT_ONTIMEROVER:
	case XDO_EVENT_ONUNLOAD:
	case XDO_EVENT_ONLOAD:
	case XDO_EVENT_ONENTERFORWARD:
	case XDO_EVENT_ONENTERBACKWARD:
		{

			XU32 s=XDomNode::Handle(nOpera,pData1,pData2);
			s|=OnEvent((XEVENTDATA*)pData1,nOpera);
			return s;
		}
		 //break;
	case XDO_GET_STYLE:
		 return XDomNode::Handle(nOpera,pData1,pData2)|DOM_CARD;
		 break;
	case XDO_ADD_ATTR:
	case XDO_SET_ATTR:
		 switch(p->nID)
		 {
		 //case XEnumAttribs::NEWCONTEXT:
		//	  p->ToLogic();
		//	  break;
		 case XEAB::TITLE:
			 XPCTSTR pTmp = p->ToString();
			  m_strTitle= pTmp;
			  return XTRUE;
		 }

		 break;
    case XDO_GET_TEXT:
		*((XString16*)pData1)=m_strTitle;
		return XTRUE;
   case XDO_SET_TEMPLATE:
	    if(pData1!=XNULL)
		{
			XDomTemplate*pt=(XDomTemplate*)pData1;
			XVar var(XEnumAttribs::ONENTERBACKWARD,0);
			if(pt->GetAttrib(var)) SetAttrib(var,XTRUE);
			var.nID=XEnumAttribs::ONENTERFORWARD;
			if(pt->GetAttrib(var)) SetAttrib(var,XTRUE);
			var.nID=XEnumAttribs::ONTIMER;
			if(pt->GetAttrib(var)) SetAttrib(var,XTRUE);
			m_childs.Append(pt->m_childs);
			return XTRUE;
		}
	    break;
	case XDO_LAYEROUT_CELL:
		 LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
		 //break;
		 //return XTRUE;
	case XDO_LAYEROUT_CELLS:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

void XDomCard::LayeroutCell(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	XU32 i;
   	CELLDATA data;
	data.Reset(pData->cur.x,pData->cur.y,XTRUE);
//	pData->cur.x=pDraw->win.left;
//	if(m_nWidth)//>pDraw->win.Width())
//		pDraw->win.right=pDraw->win.left+m_nWidth;
	for(i=0;i<m_childs.GetSize();i++)
		m_childs[i]->LayeroutItem(pDraw,&data,XTRUE);//Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
	pDraw->Init();
	//LAYEROUTDATA margin;
	//PreLayerout(pDraw,&data,&margin);

	data.Reset(pData->cur.x,pData->cur.y,XFALSE);
	//AlignRow(pDraw,&data);
	NewRow(0,pDraw,&data,pDraw->IND);
	data.bEmpty=XTRUE;
	for(i=0;i<m_childs.GetSize();i++)
		m_childs[i]->LayeroutItem(pDraw,&data,XFALSE);//Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
	//EndLayerout(pDraw,&data);
	m_nWidth=data.max.cx-pData->cur.x;
	data.max.cx-=pData->cur.x;
	data.max.cy-=pData->cur.y;
	SetRowSize(pDraw,pData,data.max,m_nWidth,XFALSE);
	NewRow(pDraw->SPACE,pDraw,pData,pDraw->IND);
}

void XDomCard::LayeroutCells(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	CELLDATA data;
	data.Reset(0,0,XTRUE);
//	if(m_nWidth)//>pDraw->win.Width())
//		pDraw->win.right=pDraw->win.left+m_nWidth;

	for(XU32 i=0;i<m_childs.GetSize();i++)
		m_childs[i]->LayeroutItem(pDraw,&data,XTRUE);//Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
	m_nWidth=data.max.cx-pData->cur.x;
	SetRowSize(pDraw,pData,data.max,m_nWidth,XTRUE);

	NewRow(pDraw->SPACE,pDraw,pData,pDraw->IND);
}

XU32 XDomCard::OnEvent(XEVENTDATA *pData,XU32 nOpera)
{
	XPCTSTR strURL=XNULL;
	switch(nOpera)
	{
	case XDO_EVENT_ONLOAD:
		 if(pData->pView!=XNULL&&m_strTitle.GetLength()>0)
		 {
			 pData->pView->SetTitle(m_strTitle);
			 pData->strTitle=m_strTitle;
		 }
		 break;
	case XDO_EVENT_ONENTERFORWARD:
		 strURL=FindAttrib(XEAB::ONENTERFORWARD,(XPCTSTR)XNULL);
		 break;
	case XDO_EVENT_ONENTERBACKWARD:
		 strURL=FindAttrib(XEAB::ONENTERBACKWARD,(XPCTSTR)XNULL);
		 break;
	case XDO_EVENT_ONTIMEROVER:
		 strURL=FindAttrib(XEAB::ONTIMER,(XPCTSTR)XNULL);
		 break;
	}
	XU32 s=XRNONE;
	if(strURL!=XNULL)
	{
		s|=XRCONNECT;
		pData->bAuto=XTRUE;
		pData->SetURL(strURL);//URL.SetURL(strURL);
		pData->nMethod=XEM::GET;
	}
	return s|XDomNode::Handle(nOpera,(XU32)pData,0);
}

/*XU32 XDomCard::SelectItem(SELECTINFO *pInfo)
{
	XU32 nData=XDomNode::Handle(XDO_SELECT,(XU32)pInfo,0);
	if(nData==0)
	{
		if(pInfo->nCount>=pInfo->nLength) return (XU32)pInfo->pLast;
	}
	return nData;
} */

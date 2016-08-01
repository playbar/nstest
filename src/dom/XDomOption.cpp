// XDomOption.cpp: implementation of the XDomOption class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomOption.h"
#include "XDomSelect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomOption::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_GET_RECT:
		{
			if(!IsVisiable()) return 0;
			XDomNode::Handle(nOpera,pData1,pData2);
			XRect*pRect=(XRect*)pData1;
			*pRect|=XRect(m_nPosX,m_nPosY,m_nPosX+XBOXSIZE+4,m_nPosY+XBOXSIZE+4);
		}return XTRUE;
	case XDO_EVENT_ONKEYDOWN:
		 if(!IsVisiable()) return 0;
		 if(pData1!=XK_RETURN) break;
	case XDO_EVENT_ONCLICK:
		 if(!IsVisiable()) return 0;
		 return OnClick((XEVENTDATA*)pData2);
	case XDO_HITTEST:
		{
			if(!IsVisiable()) return 0;
			XRect rt(m_nPosX,m_nPosY,m_nPosX+XBOXSIZE+4,m_nPosY+XBOXSIZE+4);
			if(rt.PtInRect(pData1,pData2)) return (XU32)this;
		}
		 if(XDomNode::Handle(nOpera,pData1,pData2)) return (XU32)this;
		 return XNULL;
	case XDO_EVENT_ONMOUSEMOVE:
		 if(!IsVisiable()) return 0;
		 return XDomNode::Handle(nOpera,pData1,pData2)|XRCANCHOR;
	case XDO_GET_STYLE:
		return XDomNode::Handle(XDO_GET_STYLE,0,0)|DOM_SELECTABLE;
	case XDO_GET_TEXT:
		{
		 XBOOL bOK;
		 for(XU32 i=0;i<m_childs.GetSize();i++)
			 bOK|=m_childs[i]->Handle(nOpera,pData1,pData2);

		 return bOK;
		}break;
	case XDO_SELECTION:
		  if(!IsVisiable()) return 0;
	      return XDomItem::Selection((XSelection*)pData1);
	case XDO_SELECT:
		if(!IsVisiable()) return 0;
		return (XU32)XDomItem::SelectItem((SELECTMARK*)pData1);
		
	case XDO_PAINT:
		if(!IsVisiable()) return 0;
		Paint((DRAWCONTEXT*)pData1);
		return XTRUE;
	case XDO_LAYEROUT_CELLS:
		if(!IsVisiable()) return 0;
		Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
		return XTRUE;
	case XDO_LAYEROUT_CELL:
		if(!IsVisiable()) return 0;
		Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		return XTRUE;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}



void XDomOption::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData, XBOOL bCells)
{
	if(!bCells)
	{
		m_nPosX=pData->cur.x+2;
		//int a=pData->alignh;
		//pData->alignh=XEA::MIDDLE;
		m_nPosY=AlignCol(pDraw,pData,XBOXSIZE,XEA::MIDDLE);
		//pData->alignh=a;
	}
	SetRowSize(pDraw,pData,XSize(XBOXSIZE+4,XBOXSIZE+4),XBOXSIZE+4,bCells);
	
	XDomNode::Handle(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,
						(XU32)pDraw,(XU32)pData);
	NewRow(0,pDraw,pData,pDraw->IND);
}

void XDomOption::Paint(DRAWCONTEXT *pDraw)
{
	//XU32 tc=pDraw->pDraw->GetColor();
	//XU32 bc=pDraw->pDraw->GetBackColor();

	XRect rt(m_nPosX,m_nPosY,m_nPosX+XBOXSIZE,m_nPosY+XBOXSIZE);

	XU32 c=pDraw->DCBACK;
	if(m_bCheckBox)
		pDraw->pDraw->DrawCheckBox(rt,c,IsSelect(pDraw));
	else
		pDraw->pDraw->DrawRadio(rt,c,IsSelect(pDraw));

    /*if(pDraw->pSelect==this)
	{
		pDraw->pDraw->SetBackColor(pDraw->DCSELECTBACK);
		pDraw->pDraw->SetColor(pDraw->DCSELECTTEXT);
	}
	else if(pDraw->pActive==this)
	{
		pDraw->pDraw->SetBackColor(bc);
		pDraw->pDraw->SetColor(pDraw->DCACTIVE);
	}
	else
	{
	    pDraw->pDraw->SetBackColor(bc);
		pDraw->pDraw->SetColor(tc);
	}*/

//	pDraw->bFillStyle|=DRAWCONTEXT::XDFS_FILLBACK;
	XU8 nStatus=pDraw->nStatus;
	pDraw->nStatus=GetStatus(pDraw);
	
	XDomNode::Handle(XDO_PAINT,(XU32)pDraw,(XU32)this);
	pDraw->nStatus=nStatus;
//	pDraw->bFillStyle&=~DRAWCONTEXT::XDFS_FILLBACK;



	//pDraw->pDraw->SetBackColor(bc);
	//pDraw->pDraw->SetColor(tc);
}

XBOOL XDomOption::IsSelect(DRAWCONTEXT *pDraw)
{
	XBOOL bSelect=XFALSE;
	if(pDraw->DATA!=0)
	{
		XString8*pStr=(XString8*)pDraw->DATA;
		XPCTSTR strValue=FindAttrib(XEAB::VALUE,(XPCTSTR)XNULL);
		if(strValue!=XNULL)
		{
			if(*pStr==strValue) bSelect=XTRUE;
			else
			{
				XString8Array list;
				list.SplitString(*pStr,';');
				//if(XEnum::Find(list,strValue)>=0) return XTRUE;
				if(list.Find(strValue)>=0) return XTRUE;
				//list.Find
			}
		}
	}
	return bSelect;
}

XU32 XDomOption::OnClick(XEVENTDATA *pData)
{
	XU32 s=XRNONE;
	XDomItem*p=this;
	XPCTSTR strValue=FindAttrib(XEAB::VALUE,(XPCTSTR)XNULL);
	if(strValue==XNULL) return s;
	while(XTRUE)
	{
		p=p->GetParent();
		if(p==XNULL||p->GetID()==XDOM_SELECT) break;

	}
	if(p==XNULL) return s;
	s|=((XDomSelect*)p)->Select(strValue,pData,this);
	return s;
}

//DEL XBOOL XDomOption::IsRadioCheck()
//DEL {
//DEL 	XU8 s=FindAttrib(XEAB::TYPE,XEIT::TEXT);
//DEL 	if(s==XEIT::RADIO) return 1;
//DEL 	else if(s==XEIT::CHECKBOX) return 2;
//DEL 	return 0;
//DEL }

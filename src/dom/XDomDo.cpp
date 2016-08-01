// XDomDo.cpp: implementation of the XDomDo class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomDo.h"
#include "XEnumData.h"
#include "XDomView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XDomDo::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_EVENT_ONLOAD:
		 return OnLoad((XEVENTDATA*)pData1);
	case XDO_EVENT_ONKEYDOWN:
		 if(pData1!=XK_RETURN) break;
	case XDO_EVENT_ONCLICK:
		 
		 return Connect((XEVENTDATA*)pData2);
	case XDO_CONNECT:
		 return Connect((XEVENTDATA*)pData1);
	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 /*case XEAB::TYPE:
			  if(p->nType==VSTRING)
				  *p=XET().Index(p->strData);
			  break;
		 case XEAB::OPTIONAL_X:
			  p->ToLogic();
			  break;*/
		 case XEAB::LABEL:
			  m_strLabel=p->strData;
			  break;
		 }break;

	case XDO_GET_ATTR_COUNT:
		return XDomNode::Handle(nOpera,pData1,pData2)+1;
		break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XDomDo::OnLoad(XEVENTDATA *pData)
{
	if(!FindAttrib(XEAB::OPTIONAL_X,XTRUE)) return XRNONE;
	if(pData->pView==XNULL||m_strLabel.GetLength()<=0) return XRNONE;
	pData->pView->AddDomMenu(m_strLabel,(XU32)this);
	//pData->pItem=this;
	//return XRCOMMAND;
	return XRNONE;
}

XU32 XDomDo::Connect(XEVENTDATA *pData)
{
	return XDomNode::Handle(XDO_CONNECT,(XU32)pData,0);
}

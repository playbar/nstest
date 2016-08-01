// XDomOnEvent.cpp: implementation of the XDomOnEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomOnEvent.h"
#include "XEnumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XDomOnEvent::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_EVENT_ONTIMEROVER:
	case XDO_EVENT_ONPICK:
	case XDO_EVENT_ONENTERFORWARD:
	case XDO_EVENT_ONENTERBACKWARD:	
		 return OnEvent((XEVENTDATA*)pData1,nOpera);
		 //break;
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 case XEnumAttribs::TYPE:
		 case XEnumAttribs::ALIGN:
			  if(p->nType==VSTRING) 
				  *p=XEAB().Index(p->strData);
			  break;
		 }
		 break;	*/
	}

	return XDomNode::Handle(nOpera,pData1,pData2);
}


XU32 XDomOnEvent::OnEvent(XEVENTDATA *pData,XU32 nOpera)
{
	XU32 nType=FindAttrib(XEAB::TYPE,0);
	XU32 s=XRNONE;
	switch(nType)
	{
	case XEAB::ONTIMER:
		 if(nOpera!=XDO_EVENT_ONTIMEROVER) return s;
		 pData->bAuto=XTRUE;
		 break;
	case XEAB::ONENTERBACKWARD:
		 if(nOpera!=XDO_EVENT_ONENTERBACKWARD) return s;
		 pData->bAuto=XTRUE;
		 break;
	case XEAB::ONENTERFORWARD:
		 if(nOpera!=XDO_EVENT_ONENTERFORWARD) return s;
		 pData->bAuto=XTRUE;
		 break;
	case XEAB::ONPICK:
		 if(nOpera!=XDO_EVENT_ONPICK) return s;
	}
	return XDomNode::Handle(XDO_CONNECT,(XU32)pData,0);
}

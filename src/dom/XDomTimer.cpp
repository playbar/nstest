// XDomTimer.cpp: implementation of the XDomTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomTimer.h"
#include "XEnumData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomTimer::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_EVENT_ONLOAD:
		 m_nTime=FindAttrib(XEAB::VALUE,0);
		 return 0;
	case XDO_EVENT_ONTIMER:
		 if(m_nTime>0)
		 {
			 m_nTime--;
			 if(m_nTime==0)
				 return XRTIMEOVER;
		 }
		 break;
	case XDO_ADD_ATTR:
	case XDO_SET_ATTR:
		 switch(p->nID)
		 {
		 case XEAB::VALUE:
			  m_nTime=(XU32)p->ToInt();			  
			  break;
		 }break;
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}


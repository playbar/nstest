// XDomTemplate.cpp: implementation of the XDomTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomTemplate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomTemplate::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
   switch(nOpera)
   {
   case XDO_GET_STYLE:
	   return XDomNode::Handle(nOpera,pData1,pData2)|DOM_TEMPLATE;
   }
   return XDomNode::Handle(nOpera,pData1,pData2);
}

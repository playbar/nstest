// XHTMLCode.cpp: implementation of the XHTMLCode class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLCode.h"
#include "XDomText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XHTMLCode::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_SET_DATA:
		 if(pData2>0)
		 {
			 
		 }return XTRUE;
		 //break;
		 //	m_data.Append((XU8*)pData1,pData2);
	case XDO_GET_STYLE:
		 return DOM_SCRIPT|DOM_NODE;
		//return XTRUE;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

// XDomPrev.cpp: implementation of the XDomPrev class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomPrev.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XDomPrev::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_CONNECT:
		 return XRBACKWORD;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

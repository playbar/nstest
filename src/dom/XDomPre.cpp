// XDomPre.cpp: implementation of the XDomPre class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomPre.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomPre::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_KEEP_FORMAT:
		 return 1;
		 break;
	}

	return XDomP::Handle(nOpera,pData1,pData2);
}

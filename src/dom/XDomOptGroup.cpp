// XDomOptGroup.cpp: implementation of the XDomOptGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomOptGroup.h"
#include "XEnumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomOptGroup::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	/*switch(nOpera)
	{
	}*/

	return XDomFieldset::Handle(nOpera,pData1,pData2);
}


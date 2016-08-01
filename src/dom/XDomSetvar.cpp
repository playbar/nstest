// XDomSetvar.cpp: implementation of the XDomSetvar class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomSetvar.h"
#include "XEnumData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomSetvar::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_CONNECT:
		 return Connect((XEVENTDATA*)pData1);
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}


XU32 XDomSetvar::Connect(XEVENTDATA *pData)
{
	XString8 strName=FindAttrib(XEAB::NAME,"");
	if(strName.IsEmpty()) return XRNONE;
	XString8 strValue=FindAttrib(XEAB::VALUE);
	XSortString8::SSetValue(pData->strValues,strName,strValue);
	pData->nData=(XU32)this;
	return XRSETVAR;
}

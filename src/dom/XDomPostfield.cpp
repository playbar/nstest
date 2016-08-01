// XDomPostfield.cpp: implementation of the XDomPostfield class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomPostfield.h"
#include "XEnumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomPostfield::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_CONNECT:
		 return Connect((XEVENTDATA*)pData1);
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}

XU32 XDomPostfield::Connect(XEVENTDATA *pData)
{
	XString8 strName=FindAttrib(XEAB::NAME,"");
	if(strName.IsEmpty()) return XRNONE;
	XString8 strValue=FindAttrib(XEAB::VALUE,"");	
	Translate(pData->strValues,strValue);
//	strValue.ConvertTo(XCHARSET_UTF8);
	XURL::Encode(strValue);
	XSortString8::SSetValue(pData->strDatas,strName,strValue);
	return XRSETVAR;
}

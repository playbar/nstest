// XVar.cpp: implementation of the XVar class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XVar.h"

XPCTSTR XVar::ToString()
{
	//FreeBuf();
	if(nType==VSTRING)
	{
		XPTSTR pTmp = strData;
		//XPTSTR pTmp = "test"
		return pTmp;
	}
	XString8 s;
	switch(nType)
	{
	case VSTRING:return strData;
	case VLOGIC:
		s.FromLogic((XBOOL)iData);
		break;
	case VINT:
		s.FromInt((XINT)iData);
		break;
	case VFLOAT:
		s.FromFloat(fData);
		break;
	}
	FreeBuf();
	nType=VSTRING;
	strData=s.DataTo();
	return strData;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

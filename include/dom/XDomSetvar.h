// XDomSetvar.h: interface for the XDomSetvar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMSETVAR_H__03C6E780_9D60_460F_A289_E743A7B36C14__INCLUDED_)
#define AFX_XDOMSETVAR_H__03C6E780_9D60_460F_A289_E743A7B36C14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XDomSetvar : public XDomLeaf  
{
	XDEC_CREATE(XDomSetvar)
public:
	XU32 Connect(XEVENTDATA*pData);
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomSetvar(XU16 ID=XDOM_SETVAR):XDomLeaf(ID)
	{
	}

};

#endif // !defined(AFX_XDOMSETVAR_H__03C6E780_9D60_460F_A289_E743A7B36C14__INCLUDED_)

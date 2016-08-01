// XDomEcho.h: interface for the XDomEcho class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMECHO_H__5B1075F7_76C8_46C1_9DDB_64866ABC6A0C__INCLUDED_)
#define AFX_XDOMECHO_H__5B1075F7_76C8_46C1_9DDB_64866ABC6A0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XDomEcho : public XDomData  
{
	XDEC_CREATE(XDomEcho)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomEcho(XU16 ID):XDomData(ID){}

};

#endif // !defined(AFX_XDOMECHO_H__5B1075F7_76C8_46C1_9DDB_64866ABC6A0C__INCLUDED_)

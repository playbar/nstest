// XDomPrev.h: interface for the XDomPrev class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMPREV_H__61188976_DB7F_4C8C_9B77_D5AB52DDBC13__INCLUDED_)
#define AFX_XDOMPREV_H__61188976_DB7F_4C8C_9B77_D5AB52DDBC13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomPrev : public XDomNode  
{
	XDEC_CREATE(XDomPrev)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomPrev(XU16 ID=XDOM_PREV):XDomNode(ID){}

};

#endif // !defined(AFX_XDOMPREV_H__61188976_DB7F_4C8C_9B77_D5AB52DDBC13__INCLUDED_)

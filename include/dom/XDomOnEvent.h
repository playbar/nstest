// XDomOnEvent.h: interface for the XDomOnEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMONEVENT_H__271EC7BB_FCE6_4931_9F07_801AD681D0A3__INCLUDED_)
#define AFX_XDOMONEVENT_H__271EC7BB_FCE6_4931_9F07_801AD681D0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomOnEvent : public XDomNode  
{
	XDEC_CREATE(XDomOnEvent)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomOnEvent(XU16 ID=XDOM_ONEVENT):XDomNode(ID)
	{
	}

protected:
	XU32 OnEvent(XEVENTDATA*pData,XU32 nEvent);
};

#endif // !defined(AFX_XDOMONEVENT_H__271EC7BB_FCE6_4931_9F07_801AD681D0A3__INCLUDED_)

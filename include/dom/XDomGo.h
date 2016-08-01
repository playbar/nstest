// XDomGo.h: interface for the XDomGo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMGO_H__5E81041E_8D94_45C8_9254_FD62D1DE229D__INCLUDED_)
#define AFX_XDOMGO_H__5E81041E_8D94_45C8_9254_FD62D1DE229D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XDomGo : public XDomNode  
{
	XDEC_CREATE(XDomGo)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomGo(XU16 ID=XDOM_GO):XDomNode(ID)
	{
	}
protected:
	XU32 Connect(XEVENTDATA*pData);
};

#endif // !defined(AFX_XDOMGO_H__5E81041E_8D94_45C8_9254_FD62D1DE229D__INCLUDED_)

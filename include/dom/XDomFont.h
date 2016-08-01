// XDomFont.h: interface for the XDomFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMFONT_H__68A72B3D_3DB8_4D26_893A_9DB060704308__INCLUDED_)
#define AFX_XDOMFONT_H__68A72B3D_3DB8_4D26_893A_9DB060704308__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomFont : public XDomNode  
{
	XDEC_CREATE(XDomFont)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomFont(XU16 ID):XDomNode(ID)
	{
	}
};

#endif // !defined(AFX_XDOMFONT_H__68A72B3D_3DB8_4D26_893A_9DB060704308__INCLUDED_)

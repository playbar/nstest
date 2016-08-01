// XHTMLCode.h: interface for the XHTMLCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLCODE_H__748B2D14_039E_4DA9_AE9D_3C6AEF47946D__INCLUDED_)
#define AFX_XHTMLCODE_H__748B2D14_039E_4DA9_AE9D_3C6AEF47946D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XHTMLCode : public XDomNode  
{
	XDEC_CREATE(XHTMLCode)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLCode(XU16 id):XDomNode(id){};
};

#endif // !defined(AFX_XHTMLCODE_H__748B2D14_039E_4DA9_AE9D_3C6AEF47946D__INCLUDED_)

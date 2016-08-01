// XHTMLStyle.h: interface for the XHTMLStyle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLSTYLE_H__2ECF4265_2000_414E_8F21_831F207566FB__INCLUDED_)
#define AFX_XHTMLSTYLE_H__2ECF4265_2000_414E_8F21_831F207566FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XHTMLScript.h"

class XHTMLStyle : public XHTMLScript  
{
	XDEC_CREATE(XHTMLStyle)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLStyle(XU16 id):XHTMLScript(id)
	{
	};
protected:
	XU32 StyleCSS(XDom*pDom);
};

#endif // !defined(AFX_XHTMLSTYLE_H__2ECF4265_2000_414E_8F21_831F207566FB__INCLUDED_)

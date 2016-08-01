// XHTMLSup.h: interface for the XHTMLSup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLSUP_H__28F3E876_7A89_477C_88FE_C79C9431A86E__INCLUDED_)
#define AFX_XHTMLSUP_H__28F3E876_7A89_477C_88FE_C79C9431A86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomBlock.h"

class XHTMLSup : public XDomNode  
{
	XDEC_CREATE(XHTMLSup)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLSup(XU16 nID):XDomNode(nID){}

};

class XHTMLTitle:public XDomNode
{
	XDEC_CREATE(XHTMLTitle)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLTitle(XU16 nID):XDomNode(nID){}
};

class XHTMLCenter:public XDomNode
{
	XDEC_CREATE(XHTMLCenter)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLCenter(XU16 id):XDomNode(id){}
};

class XHTMLSpan:public XDomBlock
{
	XDEC_CREATE(XHTMLSpan)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLSpan(XU16 id):XDomBlock(id){}
protected:
};
#endif // !defined(AFX_XHTMLSUP_H__28F3E876_7A89_477C_88FE_C79C9431A86E__INCLUDED_)

// XHTMLObject.h: interface for the XHTMLObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLOBJECT_H__1AB91949_0A64_42B3_B82A_20A13B2BD3AB__INCLUDED_)
#define AFX_XHTMLOBJECT_H__1AB91949_0A64_42B3_B82A_20A13B2BD3AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XHTMLObject : public XDomNode  
{
	XDEC_CREATE(XHTMLObject)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLObject(XU16 id):XDomNode(id){}
protected:
	XU32 m_nPosX,m_nPosY;
};
class XHTMLEmbed : public XDomNode
{
	XDEC_CREATE(XHTMLEmbed)
public:
	//XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLEmbed(XU16 id):XDomNode(id){}
};

#endif // !defined(AFX_XHTMLOBJECT_H__1AB91949_0A64_42B3_B82A_20A13B2BD3AB__INCLUDED_)

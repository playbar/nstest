// XHTMLFrame.h: interface for the XHTMLFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLFRAME_H__374BF87B_AE8F_40F9_BE44_DCC5C53E6500__INCLUDED_)
#define AFX_XHTMLFRAME_H__374BF87B_AE8F_40F9_BE44_DCC5C53E6500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XHTMLFrameset : public XDomNode  
{
	XDEC_CREATE(XHTMLFrameset)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLFrameset(XU16 id):XDomNode(id){}
};

class XHTMLFrame : public XDomNode  
{
	XDEC_CREATE(XHTMLFrame)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLFrame(XU16 id):XDomNode(id){}
};

class XHTMLIFrame : public XDomNode  
{
	XDEC_CREATE(XHTMLIFrame)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLIFrame(XU16 id):XDomNode(id){}
};

class XHTMLNOFrames : public XDomNode  
{
	XDEC_CREATE(XHTMLNOFrames)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLNOFrames(XU16 id):XDomNode(id){}
};


#endif // !defined(AFX_XHTMLFRAME_H__374BF87B_AE8F_40F9_BE44_DCC5C53E6500__INCLUDED_)

// XHTMLScript.h: interface for the XHTMLScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLSCRIPT_H__38535698_AB42_4250_9705_8CB1E96EC043__INCLUDED_)
#define AFX_XHTMLSCRIPT_H__38535698_AB42_4250_9705_8CB1E96EC043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XHTMLScript : public XDomLeaf  
{
	XDEC_CREATE(XHTMLScript);
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLScript(XU16 id):XDomLeaf(id)
	{
		m_pData=XNULL;
	}
public:
	XIMAGEDATA* m_pData;
	XString8 m_strTxt;
};

class XHTMLNOScript : public XDomLeaf  
{
	XDEC_CREATE(XHTMLScript);
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLNOScript(XU16 id):XDomLeaf(id){}
};

#endif // !defined(AFX_XHTMLSCRIPT_H__38535698_AB42_4250_9705_8CB1E96EC043__INCLUDED_)

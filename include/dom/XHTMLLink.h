// XHTMLLink.h: interface for the XHTMLLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLLINK_H__44DA9C94_3629_4BEA_95F5_F9602EFA30BB__INCLUDED_)
#define AFX_XHTMLLINK_H__44DA9C94_3629_4BEA_95F5_F9602EFA30BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XHTMLStyle.h"

class XHTMLLink : public XHTMLStyle  
{
	XDEC_CREATE(XHTMLLink)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLLink(XU16 id):XHTMLStyle(id)
	{
		m_pData=XNULL;
	}

protected:
	XIMAGEDATA* m_pData;
};

#endif // !defined(AFX_XHTMLLINK_H__44DA9C94_3629_4BEA_95F5_F9602EFA30BB__INCLUDED_)

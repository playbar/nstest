// XDomRefresh.h: interface for the XDomRefresh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMREFRESH_H__6F19F7CA_D367_4BDF_BA77_D34B35AC1190__INCLUDED_)
#define AFX_XDOMREFRESH_H__6F19F7CA_D367_4BDF_BA77_D34B35AC1190__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomRefresh : public XDomNode  
{
	XDEC_CREATE(XDomRefresh)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomRefresh(XU16 ID=XDOM_REFRESH):XDomNode(ID){}


};

#endif // !defined(AFX_XDOMREFRESH_H__6F19F7CA_D367_4BDF_BA77_D34B35AC1190__INCLUDED_)

// XDomTemplate.h: interface for the XDomTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMTEMPLATE_H__021FE939_7263_4614_A83D_60EF7FEA0FEB__INCLUDED_)
#define AFX_XDOMTEMPLATE_H__021FE939_7263_4614_A83D_60EF7FEA0FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomTemplate : public XDomNode  
{
	XDEC_CREATE(XDomTemplate)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomTemplate(XU16 ID=XDOM_TEMPLATE):XDomNode(ID)
	{
	}
};

#endif // !defined(AFX_XDOMTEMPLATE_H__021FE939_7263_4614_A83D_60EF7FEA0FEB__INCLUDED_)

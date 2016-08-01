// XDomAccess.h: interface for the XDomAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMACCESS_H__4E410F04_A7BC_4FDD_8A14_F57E89C25A60__INCLUDED_)
#define AFX_XDOMACCESS_H__4E410F04_A7BC_4FDD_8A14_F57E89C25A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomAccess : public XDomLeaf  
{
	XDEC_CREATE(XDomAccess)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomAccess(XU16 ID=XDOM_ACCESS):XDomLeaf(ID)
	{
	}
};

#endif // !defined(AFX_XDOMACCESS_H__4E410F04_A7BC_4FDD_8A14_F57E89C25A60__INCLUDED_)

// XDomPre.h: interface for the XDomPre class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMPRE_H__4885486B_F21A_41A0_81B5_6FBBF92AA6A5__INCLUDED_)
#define AFX_XDOMPRE_H__4885486B_F21A_41A0_81B5_6FBBF92AA6A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomP.h"

class XDomPre : public XDomP  
{
	XDEC_CREATE(XDomP)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomPre(XU16 ID=XDOM_PRE):XDomP(ID){}
};

#endif // !defined(AFX_XDOMPRE_H__4885486B_F21A_41A0_81B5_6FBBF92AA6A5__INCLUDED_)

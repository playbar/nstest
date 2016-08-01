// XDomTimer.h: interface for the XDomTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMTIMER_H__011467DE_19BF_44A2_95BB_DA4F8524435B__INCLUDED_)
#define AFX_XDOMTIMER_H__011467DE_19BF_44A2_95BB_DA4F8524435B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XDomTimer : public XDomLeaf  
{
	XDEC_CREATE(XDomTimer)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomTimer(XU16 ID=XDOM_TIMER):XDomLeaf(ID)
	{
		m_nTime=0;
	}
protected:
	XU32 m_nTime;
};

#endif // !defined(AFX_XDOMTIMER_H__011467DE_19BF_44A2_95BB_DA4F8524435B__INCLUDED_)

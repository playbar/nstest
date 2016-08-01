// XDomDo.h: interface for the XDomDo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMDO_H__733C876E_218B_4DBE_9FD9_EC1C1A8931A6__INCLUDED_)
#define AFX_XDOMDO_H__733C876E_218B_4DBE_9FD9_EC1C1A8931A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomDo : public XDomNode  
{
	XDEC_CREATE(XDomDo)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);

	XDomDo(XU16 ID=XDOM_DO):XDomNode(ID){}

protected:
	XU32 Connect(XEVENTDATA*pData);
	XU32 OnLoad(XEVENTDATA*pData);
	XString16 m_strLabel;
};

#endif // !defined(AFX_XDOMDO_H__733C876E_218B_4DBE_9FD9_EC1C1A8931A6__INCLUDED_)

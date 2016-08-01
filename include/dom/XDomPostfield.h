// XDomPostfield.h: interface for the XDomPostfield class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMPOSTFIELD_H__96137B0C_F7F7_44B0_9827_B4DA8D89B1AB__INCLUDED_)
#define AFX_XDOMPOSTFIELD_H__96137B0C_F7F7_44B0_9827_B4DA8D89B1AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomPostfield : public XDomLeaf  
{
	XDEC_CREATE(XDomPostfield)
public:
	XU32 Connect(XEVENTDATA*pData);
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomPostfield(XU16 ID=XDOM_POSTFIELD):XDomLeaf(ID){}

};

#endif // !defined(AFX_XDOMPOSTFIELD_H__96137B0C_F7F7_44B0_9827_B4DA8D89B1AB__INCLUDED_)

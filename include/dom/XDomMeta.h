// XDomMeta.h: interface for the XDomMeta class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMMETA_H__7EA5CDAE_9064_4783_AE3B_B8F0A3C05993__INCLUDED_)
#define AFX_XDOMMETA_H__7EA5CDAE_9064_4783_AE3B_B8F0A3C05993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomHead : public XDomNode
{
	XDEC_CREATE(XDomHead)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomHead(XU16 ID=XDOM_HEAD):XDomNode(ID)
	{
	}

};

class _XEXT_CLASS XDomMeta : public XDomLeaf  
{
	XDEC_CREATE(XDomMeta)
public:
	XU32 OnLoad(XEVENTDATA*pData);
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomMeta(XU16 ID=XDOM_META):XDomLeaf(ID)
	{
	}

};

#endif // !defined(AFX_XDOMMETA_H__7EA5CDAE_9064_4783_AE3B_B8F0A3C05993__INCLUDED_)

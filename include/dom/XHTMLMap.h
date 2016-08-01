// XHTMLMap.h: interface for the XHTMLMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLMAP_H__054D0D44_225A_46C2_8834_12C114F40108__INCLUDED_)
#define AFX_XHTMLMAP_H__054D0D44_225A_46C2_8834_12C114F40108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomAnchor.h"
#include "XDomItem.h"	// Added by ClassView

class XHTMLMap : public XDomNode  
{
	XDEC_CREATE(XHTMLMap)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLMap(XU16 ID):XDomNode(ID){}
};

class XHTMLArea : public XDomA  
{
	XDEC_CREATE(XHTMLArea)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLArea(XU16 ID):XDomA(ID){}
protected:
	XU32 HitTest(int x,int y);
	XU32 Paint(DRAWCONTEXT*pDraw);
	void FormatCoords(DRAWCONTEXT*pDraw);
	XRect m_rect;
	DOMPOSARRAY m_coords;

};

#endif // !defined(AFX_XHTMLMAP_H__054D0D44_225A_46C2_8834_12C114F40108__INCLUDED_)

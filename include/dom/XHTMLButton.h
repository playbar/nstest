// XHTMLButton.h: interface for the XHTMLButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLBUTTON_H__6C445AF4_3650_4615_94FD_82F1D63CEA71__INCLUDED_)
#define AFX_XHTMLBUTTON_H__6C445AF4_3650_4615_94FD_82F1D63CEA71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomBlock.h"

class XHTMLButton : public XDomBlock  
{
	XDEC_CREATE(XHTMLButton)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLButton(XU16 nID):XDomBlock(nID)
	{
	   m_nStatus=0;
	   XVar var(XEAB::TEXT,(XINT)0xff000000);
	   AddAttrib(var);
	}

protected:
	XU8 m_nStatus;
	XU32 Paint(DRAWCONTEXT*pDraw);
	XU32 LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData);
	XU32 LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
	//XU32 Connect(XEVENTDATA*pData);
	XU32 OnMouseUp(XEVENTDATA*pData);
	XU32 OnMouseLeave(XEVENTDATA*pData);
	XU32 OnMouseDown(XPoint&pt,XEVENTDATA*pData);
};

#endif // !defined(AFX_XHTMLBUTTON_H__6C445AF4_3650_4615_94FD_82F1D63CEA71__INCLUDED_)

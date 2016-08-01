// XHTMLInput.h: interface for the XHTMLInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLINPUT_H__FCB19424_3936_49A5_B517_03595C2CFEA9__INCLUDED_)
#define AFX_XHTMLINPUT_H__FCB19424_3936_49A5_B517_03595C2CFEA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomInput.h"

class XHTMLInput : public XDomInput  
{
	XDEC_CREATE(XHTMLInput)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLInput(XU16 id);
	virtual ~XHTMLInput();

protected:
	XU32 OnTimer(XEVENTDATA*pData);
	XImage* GetImage();
	XIMAGEDATA* m_pImage;
	void LayeroutImage(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells);
	void SetVariable(XEVENTDATA*pData);
	XU32 Connect(XEVENTDATA*pData);
	XU32 OnMouseUp(XEVENTDATA*pData);
	XU32 OnMouseLeave(XEVENTDATA*pData);
	XU32 OnMouseDown(XPoint&pt,XEVENTDATA*pData);
	XU32 HitTest(int x,int y);
	XString16 m_strTitle;
	DOMPOS m_nPosX,m_nPosY,m_nWidth,m_nHeight;
	XU8	 m_nStatus,m_nIndex,m_nTicks,m_bFinal,m_bReturn;
	XBOOL Paint(DRAWCONTEXT*pDraw);
	void LayeroutButton(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells);
	XU32 Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells);
};

#endif // !defined(AFX_XHTMLINPUT_H__FCB19424_3936_49A5_B517_03595C2CFEA9__INCLUDED_)

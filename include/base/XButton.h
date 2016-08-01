// XButton.h: interface for the XButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XBUTTON_H__60A0F53B_2AAB_4318_B8C1_3355299C8680__INCLUDED_)
#define AFX_XBUTTON_H__60A0F53B_2AAB_4318_B8C1_3355299C8680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "GGType.h"	    // Added by ClassView
#include "XString.h"	// Added by ClassView

class _XEXT_CLASS XButton : public XWindow
{
public:
	void SetTitle(XPCWSTR strTitle);
	void SetImage(XImage*pImage);
	XBOOL Create(XRect&rect,XU32 nID,XPCWSTR strTitle,XWCHAR hotKey,XWindow*pWnd);
	XButton();
	virtual ~XButton();

protected:
	void OnBlur();
	void OnMouseMove(XPoint&pt,int iPos);
	void OnMouseUp(XPoint&pt,int iPos);
	void OnMouseDown(XPoint&pt,int iPos);
	void OnClick(XPoint&pt,int iPos);
	XBOOL OnKeyDown(XKEYMSG& c);
	XBOOL OnPaint(XGraphics&g);
	XString16 m_strTitle;
	XWCHAR	  m_nHotKey;
	XImage*   m_pImage;
	XU8		  m_nIndex,m_bDown;
};

#endif // !defined(AFX_XBUTTON_H__60A0F53B_2AAB_4318_B8C1_3355299C8680__INCLUDED_)

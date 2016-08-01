// XDomViewExt.h: interface for the XDomViewExt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMVIEWEXT_H__6E895D47_28C6_4DF3_80B9_0102607AD9BA__INCLUDED_)
#define AFX_XDOMVIEWEXT_H__6E895D47_28C6_4DF3_80B9_0102607AD9BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomView.h"

#define XCMD_CLOSE_VIEWEX (XCMD_USER+5000)
class XFlashView;

class XDomViewExt : public XDomView  
{
public:
	//virtual XINT CalcPecent(){return XDomView::;}
	XBOOL Create(XRect&rect,XWindow*pParent,XPCTSTR strHtml,int len,XU8 nCharset);
	XDomViewExt(XGraphics*g,XClientApply*pa);
	virtual ~XDomViewExt();
	virtual void ToFlashView(XU32 nTotal,XClient*pClient){}
	XImage* GetCloseImage(){return &m_close;}
	XBOOL IsInstance(){return m_bInstance;}
	void SetFrameView(XFlashView*pView)
	{
		m_pView=pView;
	}
	virtual void SetPercent(int nPer);
protected:
	XFlashView* m_pView;
	XBOOL m_bInstance;
	void OnMouseUp(XPoint&pt,int iPos);
	void OnMouseMove(XPoint&pt,int iPos);
	void OnMouseDown(XPoint&pt,int iPos);
	void OnMove(XRect&rect);
	void OnClose();
	XImage m_close;
	XBOOL OnPaint(XGraphics&g);
};

#endif // !defined(AFX_XDOMVIEWEXT_H__6E895D47_28C6_4DF3_80B9_0102607AD9BA__INCLUDED_)

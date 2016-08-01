// XStatusBar.h: interface for the XStatusBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSTATUSBAR_H__97AE3320_AF15_47B0_810E_76065C7F90CC__INCLUDED_)
#define AFX_XSTATUSBAR_H__97AE3320_AF15_47B0_810E_76065C7F90CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XToolBar.h"

#define XCMD_VIEW_CLOSE XCMD_USER+1
#define XCMD_VIEW_FORWARD XCMD_USER+2
#define XCMD_CLEAR_CATCH XCMD_USER+3
#define XCMD_CLOSE_ALL	XCMD_USER+4
#define XCMD_DOM_ITEM	XCMD_USER+5
#define XCMD_DOM_HOME	XCMD_USER+6
#define XCMD_DOM_VIEW	XCMD_USER+7
#define XCMD_DOM_BOOKMARK XCMD_USER+8
#define XCMD_DOM_ADDBOOK XCMD_USER+9
#define XCMD_DOM_BACK	XCMD_USER+10
#define XCMD_DOM_STOP	XCMD_USER+11
#define XCMD_DOM_UPDATE XCMD_USER+12
#define XCMD_DOM_QUIT	XCMD_USER+13
#define XCMD_DOM_VIEWEX	XCMD_USER+14
#define XCMD_DOM_CONNECTEX XCMD_USER+15
#define XCMD_DOM_RECONNECT XCMD_USER+16
//#define XCMD_DOM_FORWARD XCMD_USER+13

#ifdef __ANDROID__
#else

#if defined(__TOUCHPAD__)||defined(__POINT__)
class XStatusBar : public XToolBar
#else
class _XEXT_CLASS XStatusBar : public XWindow  

#endif
{
public:
	enum {XS_NONE,XS_SELECT,XS_EDIT,XS_FINISH,XS_BACK=0x80};
public:
	void SetPecent(XU8 nPecent);
	XBOOL Create(XRect&rect,XWindow*pWnd);
	XStatusBar();
	virtual ~XStatusBar();
	void SetMode(XU8 nMode)
	{
		m_nMode&=0x80;
		m_nMode|=nMode;
		Invalidate();
	}
	void SetBack(XU8 bBack)
	{
		if(bBack) m_nMode|=XS_BACK;
		else      m_nMode&=~XS_BACK;
	}

protected:
	XImage* m_pBlock;
	XU8 m_nMode,m_nTicks;
	XU8 m_nMenuBar;
	void OnClick(XPoint&pt,int iPos);
	void OnTimer();
	XU8 m_nPecent,m_nActive;
	XImage *m_pAni;
	XBOOL OnPaint(XGraphics&g);
};

#endif

#endif // !defined(AFX_XSTATUSBAR_H__97AE3320_AF15_47B0_810E_76065C7F90CC__INCLUDED_)

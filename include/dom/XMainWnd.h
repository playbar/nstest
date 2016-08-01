// XMainWnd.h: interface for the XMainWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMAINWND_H__B451E185_171A_4ACC_A7D5_02AE1E5209E5__INCLUDED_)
#define AFX_XMAINWND_H__B451E185_171A_4ACC_A7D5_02AE1E5209E5__INCLUDED_

#include "XClientApply.h"	// Added by ClassView
#include "XDomView.h"	// Added by ClassView
#include "XURL.h"
#include "XClient.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define XMAX_ICONS 10

#ifdef _WINEMU

	#define XCMD_UPDATEPROFILE 3000
	#define XCMD_UPDATE_NETRATE   1
	#define XCMD_UPDATE_FRAMES	  2
	#define XCMD_UPDATE_MEMORY	  3

#endif

#define XCMD_OUTOFMEMORY 6000
#define XCMD_FLASHCLOSE  6001
#define XCMD_FLASHOPEN   6002

class _XEXT_CLASS XMainWnd : public XWindow,XBase  
{
public:
	virtual void QuitView()=0;
	XBOOL NeedStop()
		{
		#ifdef _SYMBIAN
		if(m_pThread == XNULL) return XTRUE;
		return m_pThread->NeedFree();
		#else
		return m_thread.NeedStop();
		#endif
		}
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XBOOL Create(const XRect&rect,XPCWSTR strTitle,XGraphics*p,XPCTSTR strConfig);
	XMainWnd();
	void GetClientRect(XRect&rect) {rect=m_viewRect;}
	virtual ~XMainWnd();
protected:
	void ChangeActiveView(XU8 nID);
	XBOOL OnKeyDown(XKEYMSG&m);
	XVector<XDomView*> m_views;
	XRect m_viewRect;
#ifdef __XTAB
	XTabControl m_tab;
#else
	XString16 m_strTitle;
	XImage *m_pBlock;
#endif
public:
	XSWFPlayer* GetPlayer();
	void FullScreen(XBOOL bFull);
	XPCWSTR GetTitle(XWindow*pWnd);
	XDomView* GetActiveView();
	XBOOL CreateView(CONNECTREQ&req,XPCWSTR strTitle,XImage*pIcon=XNULL);
	
protected:	   
#ifdef _SYMBIAN
	XThread *m_pThread;
#else
	XThread m_thread;
#endif
	virtual XUINT CallBack(XU32 nID);
	virtual void ReleaseCall();
	virtual void StartCall();
	XBOOL OnPaint(XGraphics&g);
	int m_nTitleHeight;
#ifdef __ANDROID__
#else
	XDialogClear* m_pClear;
	XMessageBox* m_pBox;
	XMenu m_menu;
	void TrackMenu();
#endif
	XBOOL m_bFull;
	void OnMove(XRect&rect);
	XGraphics* m_pDraw;
	void SetActiveView(XDomView*pView);
	void OnClose();
	XBOOL OnCommand(XU32 nCmd,XWindow*pWnd);
	XClientApply m_clientApply;
	//XBitmap* m_pBitmap;
	//MENUDATAS m_sysMenus;
	void SetTitle(XPCWSTR strTitle,XWindow*pWnd);
	int FindView(XWindow*pWnd);
	XBOOL DestoryChild(XWindow*pWnd);
	//static XImage m_icons[XMAX_ICONS];
};

#endif // !defined(AFX_XMAINWND_H__B451E185_171A_4ACC_A7D5_02AE1E5209E5__INCLUDED_)

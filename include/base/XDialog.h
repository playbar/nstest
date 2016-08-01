// XDialog.h: interface for the XDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDIALOG_H__1A014817_5C2D_441A_B335_858D354161D1__INCLUDED_)
#define AFX_XDIALOG_H__1A014817_5C2D_441A_B335_858D354161D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "GGType.h"	// Added by ClassView
#include "XString.h"	// Added by ClassView
#include "XButton.h"	// Added by ClassView

class _XEXT_CLASS XDialog : public XWindow  
{
public:
	XBOOL OnKeyUp(XKEYMSG& c);
	virtual void EndDialog(XU32 nCmd);
	static int GetTitleHeight(XImage*pImage,XPCWSTR strTitle);
	XBOOL OnPaint(XGraphics&g);
	//virtual XU32 DoModal();
	virtual XBOOL Create(const XRect&rect,XWindow*pWnd,XPCWSTR strTitle=XNULL,XImage*pImage=XNULL);

	virtual ~XDialog();
    void SetHandle(XWindow*pWnd,XU32 id)
	{
		m_nCommand=id;
		m_pHandle=pWnd;
	}
protected:	
	void OnClick(XPoint&pt,int iPos);
	XWindow* m_pHandle;
//	XBOOL IsStoped;
	XBOOL OnKeyDown(XKEYMSG& c);
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
//	XU32 m_nCommand;
	void GetClientRect(XRect&r);
	XDialog();
	XImage* m_pIcon;
	XU8		m_nIndex;
	XU32    m_nCommand;
	XString16 m_strTitle;
};

#define XMB_ICONQUESTION ((XICON_HOME)|(5<<8))
#define XMB_ICONHOMME	 (XOCN_HOME)
#define XMB_ICONDELETE	 ((XICON_HOME)|(4<<8))
//#define XMB_DELETESELF	 0x80000000

#define XIDOK                1
#define XIDCANCEL            2
#define XIDABORT             3
#define XIDRETRY             4
#define XIDIGNORE            5
#define XIDYES               6
#define XIDNO                7
#define XIDSTOP				 8
#define XIDSTART			 9
#define XIDQUIT				 -1

#define XMB_OK                       0x00000000
#define XMB_OKCANCEL                 0x00010000
#define XMB_ABORTRETRYIGNORE         0x00020000
#define XMB_YESNOCANCEL              0x00030000
#define XMB_YESNO                    0x00040000
#define XMB_RETRYCANCEL              0x00050000
#define XMB_STOP					 0x00060000
#define XMB_STARTSTOP			     0x00070000

#ifdef __ANDROID__
#else
class _XEXT_CLASS XMessageBox : public XDialog  
{
public:
	
	virtual ~XMessageBox();
	static XMessageBox* Message(XPCWSTR strPrompt,XWindow*pCallBack,XU32 ID,XU32 nStyle=XMB_YESNO,XPCWSTR strTitle=m_strAppTitle)
	{
		return new XMessageBox(strTitle,strPrompt,pCallBack,ID,nStyle);
				
		//return dlg.DoModal();
	}
	XMessageBox(XPCWSTR strTitle,XPCWSTR strPrompt,XWindow*pCallBack,XU32 ID,XU32 nStyle=XMB_OKCANCEL,XU16 nSpaceHeight=0);
protected:
	void EndDialog(XU32 nCmd);
//	XU8 m_bDeleteSelf;
	
	void CreateButtons(XU32 nStyle);
	XButton m_buts[3];
	XBOOL OnPaint(XGraphics&g);
	void OnIdle();
	XString16 m_strPrompt;
	DOMPOSARRAY m_nFormats;
	XImage*	  m_pImage;
	XU8		  m_nIndex;//m_bDelete;
};
#endif

#endif // !defined(AFX_XDIALOG_H__1A014817_5C2D_441A_B335_858D354161D1__INCLUDED_)

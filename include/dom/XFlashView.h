// XFlashView.h: interface for the XFlashView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFLASHVIEW_H__331C3F5C_83D9_4AB8_B069_27777ABB854A__INCLUDED_)
#define AFX_XFLASHVIEW_H__331C3F5C_83D9_4AB8_B069_27777ABB854A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomView.h"
#include "XToolBar.h"

#define XCMD_FLASH_HELP 		(XCMD_USER+101)
#define XCMD_FLASH_ORIG 		(XCMD_USER+102)
#define XCMD_FLASH_FIT			(XCMD_USER+103)
#define XCMD_FLASH_CLOSE 		(XCMD_USER+104)
#define XCMD_FLASH_SCORE 		(XCMD_USER+105)
//#define XCMD_FLASH_RESTART 		(XCMD_USER+106)
#define XCMD_FLASH_SAVE 		(XCMD_USER+107)
#define XCMD_FLASH_FULLSCREEN 	(XCMD_USER+108)
#define XCMD_FLASH_ZOOMMODE 	(XCMD_USER+109)
#define XCMD_BUTTON_SWITCH 		(XCMD_USER+110)
#define XCMD_UPDATE_SWITCH 		(XCMD_USER+111)
#define XCMD_FLASH_MOVEMODE 	(XCMD_USER+112)
//#define XCMD_FLASH_ZOOMINOUT 	(XCMD_USER+113)
#define XCMD_FLASH_SAVE_CANCEL 	(XCMD_USER+114)   // 取消保存flash，add by hgl
//#define XCMD_FLASH_POINTZOOM	(XCMD_USER+115)
//#define XCMD_FLASH_OBJZOOM		(XCMD_USER+116)
#define XCMD_FLASH_PAY			(XCMD_USER+117)
#define XCMD_OFFERWALLFLASH_PAY			(XCMD_USER+118)
#define XCMD_FLASH_START		(XCMD_USER+119)
#define XCMD_FLASH_LOADING_END		(XCMD_USER+120)
#define XCMD_FLASH_ZOOMIN		(XCMD_USER+121)
#define XCMD_FLASH_ZOOMOUT		(XCMD_USER+122)
#define XCMD_FLASH_STARTDRAG     (XCMD_USER+123)
#define XCMD_FLASH_DRAGING      (XCMD_USER+124)
#define XCMD_FLASH_ENDDRAG     (XCMD_USER+125)
#define XCMD_FLASH_MODE (XCMD_USER+200)

#define XFLASH_CNT_SCORE -2
#define XFLASH_CNT_FILE  -3

class XDomViewExt;
class XDialogSave;

#ifdef _THREADBACK
class XFlashView : public XWindow,XBase
#else
class XFlashView : public XWindow
#endif
{
	union UPosXY
	{
		XU32  iData;
		short xy[2];
	};

	friend class CAnalyDialog;
	friend class XDomView;

#ifdef __APPLE__
public:
	XBOOL m_flashSaveReady;
	XBOOL m_topScoreAvailable;
	friend class CMobileView;
#endif
#ifdef __ANDROID__
public:
	XBOOL m_flashSaveReady;
	XBOOL m_topScoreAvailable;
	friend class XMobileView;
	friend class XMobileViewGLES;
	friend class XMobileViewSurface;
#endif
protected:

/*#if defined(__TOUCHPAD__)||defined(__POINT__)
	XToolBar m_bar;
#endif*/	
	int		 m_nCallCount;
#ifdef _THREADBACK
	enum 
	{
		XAPPEND_DATA,XCLOSE_STREAM,XNEW_STREAM,XPAUSE,XRESUME,XRESTART,XFIT,XORG,XMOVE,XFULLSCREEN,XDESTROY_TEXT,
	};
	struct XDATA
	{
		XU8 nType;
		XU8 nCmd;
		XU32 nTotal;
		XU32 nLoaded;
		XU8* pData;
		XU32 nSize;
		XDATA*pNext;
		XClient*pClient;
		_XStreamData*pStream;
	};
	XDATA*m_pData;
#else
	enum 
	{
		XPAUSE,XRESUME,XRESTART,XFIT,XORG,XMOVE,XFULLSCREEN,XDESTROY_TEXT,/*XBEGINSCORE,*/XPUSHSCORE,XMOVE_VIEW,
		  XZOOMINOUT,XSEEK,XUPDATE,XRELOAD,XDEVICE,XSTARTDRAG,XDRAGING,XENDDRAG,XUPDATEBUTTONS,
	};
	struct XDATA
	{
		XU8 nType;
		XU8 nCmd;
		XU32 nTotal;
		XU32 nLoaded;
		XU8* pData;
		XU32 nSize;
		XDATA*pNext;
		XClient*pClient;
		_XStreamData*pStream;
	};
	//XDATA*m_pData;
	XU32 m_nCmds[MAX_CMDS];
	int  m_nCmdSet,m_nCmdGet;
	XBOOL AddCommand(XU32 nCmd,XU32 nType,XU32 nData,XBOOL bReplace=XFALSE);
	virtual void OnFocus()
	{	
		if(m_player)
			m_player->OnActivate();
		//AddCommand(XFOCUS,0,0);	
	}
	virtual void OnBlur()
	{	
		if(m_player)
			m_player->OnDeactivate();
		//AddCommand(XBLUR,0,0);	
	}
#endif

	XMutex m_dataLock,m_runLock;
	XBOOL  m_bConfigOK,m_nConnecting;
protected:
	XU32   m_nTimeCount,m_nACCDelay;
	XU32   m_nDownTime,m_nLastTickTime;
	enum   {DRAG_START=0,DRAG_ZOOM=1,DRAG_MOVE=2};
	XU32   m_nTwoPoint,m_bWait2,m_nLenDown,m_nLenPrev,m_nPecent0,m_nPecentBase,m_nDragSet;
	SPOINT m_downPt2,m_downPt1,m_nowPt1,m_nowPt2,m_centerPrev;
	XDOMFILTER*	m_pObjectFilter;
	bool	m_bHor;
	int Length(SPOINT&pt1,SPOINT&pt2,bool bHor);
	
	bool IsHor(SPOINT&pt1,SPOINT&pt2)
	{
		int dx=(pt2.x-pt1.x);
		int dy=(pt2.y-pt1.y);
		if(dx<0) dx=-dx;
		if(dy<0) dy=-dy;
		return dx>=dy?true:false;
	}
	void SetCursor(XU8 nType) 
	{
		_XMOUSEMODE*pMode=m_player->GetConfig()->m_pActiveMode;
		if(pMode&&pMode->nMode==_XMOUSEMODE::XNORMAL)
			m_pParent->SetCursor(XCUR_HIDE);
		else

			m_pParent->SetCursor(nType);
	}
	
	enum {XBAR_SHOW,XBAR_IN,XBAR_OUT,XBAR_PTOUT,XBAR_HIDE};
	//int m_nCntMode;
	XClient* m_pConfigClient;
public:
	void GetEditTexts(XSObjectArray&list) {m_player->GetEditTexts(XNULL,list);}
	XSWFPlayer* GetPlayer(){return m_player;}

	void CloseStream(XClient*pClient);
	XBOOL NewStream(XClient*pClient,const char*urlBase);
	XBOOL OnChar(XWCHAR nChar);
	void OnString(XPCWSTR buf)
	{
		m_player->OnString(buf);
	}
	XBOOL ProcHtmlText(XPCTSTR str,XXVar&strTxt,XU8 set);
	XBOOL AppendData(XU8Array&data,int nSize,XClient*pClient);//_XStreamData*pStream);
	XBOOL Create(XRect&rect,XWindow*pWnd,XU32 nTotal,XU32 nCmd);
	XFlashView(XDomView*pDom);
	virtual ~XFlashView();
	void Release();
	XBOOL OnKeyUp(XKEYMSG&m);
	XBOOL OnKeyDown(XKEYMSG&m);
protected:
	XBOOL SetData();
	void FreeData(XDATA*pData,XBOOL bSub);
#ifdef _THREADBACK
	XBOOL ProcData(XDATA*pInfo);
#endif
	XBOOL NeedStop();
	void GetWaitRect(XRect&rect);
	XImage* m_pWaitImg;
//#ifdef __ANDROID__
//#else
//	XMessageBox* m_pBox;
//	void AppendMenu(XMenu&menu);
//#endif
	XDialogSave* m_pSave;
	void DrawDib(XDib&dib,XPoint&pt,XRect&rect,float alpha,XDib*pLight);
	void LockKeyButtons();
	XU8Array m_configData;
	void DoSave();
	void PushScore();
	//XDomViewExt* m_pTxtView;
	void Pause();
	XBOOL m_bNetwork,m_bLoadOK,m_bInitOK;
	void MakeModeMenu();
	MENUDATAS m_modeMenu;
	void GetMouseRect(XRect&rect);
	void PaintCursor(XGraphics&g);
	XBOOL MouseAct(XU16 c,XBOOL bDown);
	_XBUTTONMAP*  CheckButtons(XPoint&pt,XRect&rect,int iPos,bool& bUpdate);
	XBOOL GetButtonPos(XRect&rect,int id);
	void PaintButtons();
//	XConfig m_config;
	XBOOL OnCommand(XU32 nCmd,XWindow*pWnd);
	XImage m_left;//,m_right;
//	XStatusBar m_statusBar;
	void LoadDefaultConfig();
	XPCTSTR m_strHelp;
	int  m_nHelpLength;
//	XU8	    m_nCharset;
//	XConfig* m_pConfig;
	void PhaseConfig();
	XDom* m_pCfgDom;
	void OnMove(XRect&rect);
	//void InitWindow();
	//XBOOL DestroyEdit(XU32 id);
	//XBOOL CreateEdit(XDTDefineEditText*pText,XRect&rect);
public:
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	//void SetSize(XSize&sz);
	void OnTimer();
//	XFlashBar m_controlBar;
	XU8	  m_nBarMode,m_nBarPos,m_nWaiting,m_nStartTime;//,m_nZoom;
	XBOOL m_bMouseDown,m_bPause,m_bMapDown;
	void OnMouseMove(XPoint&pt,int iPos);
	void OnMouseUp(XPoint&pt,int iPos);
	void OnMouseDown(XPoint&pt,int iPos);
	void OnClick(XPoint&pt,int iPos);
	void OnDblClick(XPoint&pt,int iPos);
	

	void PaintLowNetSpeedTip(XGraphics&g);
	//void PaintTxtView( XGraphics & g );
	void PaintBlackBG( XGraphics &g );
	void PaintFrameShow( XGraphics &g );
	XBOOL OnPaint(XGraphics&g);
	
	XSize &m_winSize;
	//XSize m_maxSize;
	XU32 m_nTotalDatas;
	//XPoint m_flashBase;
//	XPoint m_ctPoint;
//	XStream m_stream;
	XSWFPlayer* m_player;
	XU32	  m_nBarTime;
#ifdef _THREADBACK
	XThread m_thread;
	XUINT CallBack(XU32 nID);
#else
public:
	XUINT CallBack(XThread&thread);
#endif

protected:
	SRECT m_dragRect;
	void StartDrag();
	void SetDragRect(SRECT&pt,XPOINT&p1,XPOINT&p2);
	void Draging(bool moving);
	void EndDrag();
protected:
	XU32 m_nClickDown;
	bool m_bFlashDown;
	int  m_nFlashX,m_nFlashY;
	XBOOL GetScore(XXVar&var);
	void MouseAction(int nAct,int nStep,XBOOL bPress);
	void OnACC(int x,int y,int z);
	//void BeginScore(XBOOL bAuto);
	XBOOL IsEnableZoom();
	XBOOL CheckMenu(XPoint&pt);
	void UpdateToolBar();
	XDomView* m_pDom;
	int		  m_nMouseX,m_nMouseY;
	
public:
	XU32 GetCurrentTime1();
	XU32 GetTotalTime();
	XU32 GetCanPlayTime();
	void _Seek(XU32 nTime)
	{
		AddCommand(XSEEK,nTime,0);
	}
	void Seek(XU32 nTime,XBOOL bLastOP);
	float getCurrentScore();
protected:
	struct CLIENTINFO
	{
		XClient* pClient;
		CLIENTINFO* pNext;
	};
	CLIENTINFO* m_pClientInfo;
	XFastAlloc	m_infoAlloc;
	int m_nPecent;
	void AddClient(XClient*pClient)
	{
		CLIENTINFO*pInfo=m_pClientInfo;
		while(pInfo)
		{
			if(pInfo->pClient==pClient) return;
			pInfo=pInfo->pNext;
		}
		pInfo=(CLIENTINFO*)m_infoAlloc.Alloc();
		pInfo->pClient=pClient;
		pInfo->pNext=m_pClientInfo;
		m_pClientInfo=pInfo;
	}
	void RemoveClient(XClient*pClient)
	{
		CLIENTINFO**root=&m_pClientInfo;
		pClient->SetWindow(XNULL,0,0);
		while(*root)
		{
			CLIENTINFO*info=*root;
			if(info->pClient==pClient)
			{
				*root=info->pNext;
				m_infoAlloc.Free(info);
				return;
			}
			root=&info->pNext;
		}
	}
	void FreeInfo()
	{
		while(m_pClientInfo)
		{
			CLIENTINFO*pNext=m_pClientInfo->pNext;
			m_pClientInfo->pClient->Cancel();
			m_pClientInfo->pClient->SetWindow(XNULL,0,0);
			m_infoAlloc.Free(m_pClientInfo);
			m_pClientInfo=pNext;
		}
	}
public:
	//XBOOL GetCookies(XString8&strCookies);
	//XBOOL GetCookieOf(XString8&strName,XString8&strCookies);
	XBOOL GetVarOf(XString8&strName,XString8Array&args,XString8&strValue);
	XBOOL SetValueOf(XString8&strVar);
	XBOOL SetVarOf(XString8&strVar);
public://form zoom and move screen
//#ifdef _LOWSPEED
//	XBOOL StartDragOrZoom();
//	XBOOL StopDragOrZoom(bool bUpdate);	
//#endif
public:
//#if (__CORE_VERSION__>=0x02079000)
	XString8 m_strReloadURL;
	void FlashPostData(char*strData);
//#endif
	//for Score ContentID
	char firstBytes[10];
	char lastBytes[10];
	char m_nFirstLen,m_nLastLen;
	int  m_nFileLength;
	XClient* m_pFirstClient;
	XString8 m_strFlashFile;
	void SaveBytes(void*pData,int len,bool bFirst,XClient*pClient);
	void MakeContentID(const char*score,XString8&strOut);
	//void OpenWindow(const char*url);
	void CloseFlash();
	void Restart()
	{	if(!CanRestart()) return;
		AddCommand(XRESTART,0,0);}
	void ZoomIn();
	void ZoomOut();
	void SetZoomPecent(int v);
	bool CanSendScore();
	//bool CanPay();
	void SendScore(){AddCommand(XPUSHSCORE,0,0);}
	bool CanZoomin()
	{
		return this->m_nZoomPecent>0;
	}
	bool CanZoomout()
	{
		return this->m_nZoomPecent<400;
	}
	bool CanRestart();
	XBOOL IsMouseShow();
	int GetZoomPecent();  //add by hgl
	XBOOL CanPay();  //是否可以充值，add by hgl
	void DoPay(const char*url=NULL);
	XBOOL IsPause();// add by hgl
	void EndPost();
public:
	//int  m_nConPecent;
	//void SetLoadPercent(int nPer);
protected:
	//void  GetPointZoomRect(XRect&rect);
	int	  m_nZoomPecent;
	int	  m_nLineOffset;
	int	  m_nLockPtTime;
	int	  m_nLockOptTime;
	XImage m_optImage;
	XBOOL m_bSaveReady;
	//int	  m_nMaxPecent;
	//XBOOL m_bLockPoint;
	//XBOOL m_bSaveReady;
#ifdef __APPLE__
public:
    XRect m_showRect; //flash 当前显示的矩形区域的大小，ios顶层使用
#endif

};

#endif // !defined(AFX_XFLASHVIEW_H__331C3F5C_83D9_4AB8_B069_27777ABB854A__INCLUDED_)

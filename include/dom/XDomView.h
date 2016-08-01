// XDomView.h: interface for the XDomView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMVIEW_H__605F4B1E_D32B_42AC_AD68_1F981B87FF8E__INCLUDED_)
#define AFX_XDOMVIEW_H__605F4B1E_D32B_42AC_AD68_1F981B87FF8E__INCLUDED_

#include "XStatusBar.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define XSTATUSH 16
#define XSSIZE 14
//#include <pthread.h>
#include "XDomItem.h"
#include "XDom.h"	// Added by ClassView
#include "XURL.h"	// Added by ClassView
#include "XClient.h"	// Added by ClassView

#ifdef __APPLE__
#define XWM_APPLE_MESSAGE (XWM_COMMAND + 0x10000)
enum APPLE_MESSAGE
{
	XCMD_START_FLASH,
	XCMD_END_FLASH,
	XCMD_START_LOAD_PAGE,
	XCMD_END_LOAD_PAGE,
	XCMD_LOAD_PROCESS,
	XCMD_LOAD_PAGE_NOT_EMPTY,
	XCMD_TOP_SCORE_AVAILABLE,
	XCMD_FLASH_SAVE_PERCENT,
    //XCMD_SHOWBARANDONSALE,
	//XCMD_APP_STATUS,
	//XCMD_NEW_APP,
	XCMD_FLASH_OPENSUBVIEW,
	XCMD_FLASH_CLOSESUBVIEW,
	XCMD_FLASH_ZOOMINENABLE,
	XCMD_FLASH_ZOOMOUTENABLE,
    XCMD_UPDATE_CLICKOPT,
    XCMD_FLASH_CANSAVE,
    XCMD_FLASH_CANPAY,
    XCMD_UPDATE_BAR,
    XCMD_NOTICE_PAYURL,
    XCMD_SHOWWEBVIEW,
    XCMD_SHOWWEBBVIEW,
    XCMD_FLASHTODOMEXT,
    XCMD_DOMEXTTOFLASH,
    XCMD_LOADINDPAINT_START,
    XCMD_IOS_PID,
    XCMD_STOP_IOS_PID,
};
#endif


#ifdef __ANDROID__
//#define XWM_ANDROID_MESSAGE (XWM_COMMAND + 0x10500)
void thread_xdomview(void);
enum ANDOIRD_MESSAGE
{
	ANDROID_START_FLASH = XWM_COMMAND + 0x10500,
	ANDROID_END_FLASH,
	ANDROID_START_LOAD_PAGE,
	ANDROID_END_LOAD_PAGE,
	ANDROID_LOAD_PROCESS,
	ANDROID_LOAD_PAGE_NOT_EMPTY,
	ANDROID_TOP_SCORE_AVAILABLE,
	ANDROID_FLASH_SAVE_PERCENT,
	ANDROID_FLASH_WAIT_START,      // Waiting Start
	ANDROID_FLASH_WAIT_END,         // Waiting End
	ANDROID_FLASH_LOADING_END,
	ANDROID_FLASH_CLOSESUBVIEW,
	ANDROID_UPDATE_CLICKOPT,
	ANDROID_NOTICE_PAYURL,
	ANDROID_SHOWWEBVIEW
};
#endif
class XClientApply;
class XMainWnd;
class XFlashView;
void thread_xdomview(void *mpClient);
#define MAX_THREADS 10
#define MAX_CATCH	10
#define HOMEPAGE 0x80
#define _XSAVEDOM

#define MAX_CMDS 64*3

class _XEXT_CLASS XDomView : public XWindow
{
    struct XCOOKIE
	{
		XString8 strKey;
		XString8 strValue;
		XU32	 nCreateTime;
	};
	typedef XVector<XCOOKIE*> _XCOOKIES;
	struct XHOSTCOOKIE
	{
		XString8 strDomain;
		XString8 strPath;
		_XCOOKIES cookies;
		void Release()
		{
			int i,nc=cookies.GetSize();
			for(i=0;i<nc;i++)
			{
				cookies[i]->strKey.RemoveAll();
				cookies[i]->strValue.RemoveAll();
				delete cookies[i];
			}
			cookies.RemoveAll();
		}
	};
	typedef XVector<XHOSTCOOKIE*> _XHOSTCOOKIES;
	class XHOSTCOOKIES//:public XSort
	{
	public:
		XHOSTCOOKIES(){}
		virtual ~XHOSTCOOKIES()
		{
			Release();
		}
		void Clear()
		{
			Release();
		}
		XBOOL GetCookies(XURL&url,XString8&strCookies)
		{
			int i,nc=m_cookies.GetSize();
			_XCOOKIES sCookies;
			XBOOL bCookies=XFALSE;
			for(i=0;i<nc;i++)
			{
				if(IsCookieOf(m_cookies[i],url))
				{
					bCookies=XTRUE;
					_XCOOKIES& cookies=m_cookies[i]->cookies;
					int j,ncc=cookies.GetSize();
					for(j=0;j<ncc;j++)
					{
						int k,kc=sCookies.GetSize();
						bool bSet=false;
						for(k=0;k<kc;k++)
						{
							if(sCookies[k]->strKey==cookies[j]->strKey)
							{
								bSet=true;
								if(cookies[j]->nCreateTime>sCookies[k]->nCreateTime)
									sCookies[k]=cookies[j];

							}
						}
						if(!bSet)
							sCookies.Add(cookies[j]);
					}
				}
			}
			if(bCookies)
			{
				nc=sCookies.GetSize();
				for(i=0;i<nc;i++)
				{
					if(!strCookies.IsEmpty()) strCookies+=';';
					strCookies+=sCookies[i]->strKey;
					strCookies+='=';
					strCookies+=sCookies[i]->strValue;
				}
			}
			return bCookies;
		}
		XBOOL GetCookieOf(XURL&url,XString8&strName,XString8&strCookies)
		{
			int i,nc=m_cookies.GetSize();
			XCOOKIE* pCookie=NULL;
			XBOOL bCookie=XFALSE;
			for(i=0;i<nc;i++)
			{
				if(IsCookieOf(m_cookies[i],url))
				{
					_XCOOKIES& cookies=m_cookies[i]->cookies;
					int j,ncc=cookies.GetSize();
					for(j=0;j<ncc;j++)
					{
							if(cookies[j]->strKey==strName)
							{
								bCookie=XTRUE;
								if(pCookie==XNULL)
									pCookie=cookies[j];
								else if(cookies[j]->nCreateTime>pCookie->nCreateTime)
									pCookie=cookies[j];
							}
					}
				}
			}
			if(pCookie)
				strCookies=pCookie->strValue;
			return bCookie;
		}
		void SetCookies(XString8&strPath,XString8&strDomain,XString8&strKey,XString8&strValue,XU32 nTime)
		{
			int i,nc=m_cookies.GetSize();
			//_XHOSTCOOKIE*pCookie=NULL;
			for(i=0;i<nc;i++)
			{
				XHOSTCOOKIE*pc=m_cookies[i];
				if(pc->strDomain==strDomain&&
				   pc->strPath==strPath)
				{
					_XCOOKIES&cookies=pc->cookies;
					int j,jc=cookies.GetSize();
					for(j=0;j<jc;j++)
					{
						if(cookies[j]->strKey==strKey)
						{
							cookies[j]->strValue=strValue;
							cookies[j]->nCreateTime=nTime;
							return;
						}
					}
					XCOOKIE*pCookie=new XCOOKIE();
					pCookie->nCreateTime=nTime;
					pCookie->strKey=strKey;
					pCookie->strValue=strValue;
					cookies.Add(pCookie);
					return;
				}
			}
			XHOSTCOOKIE*pHost=new XHOSTCOOKIE();
			pHost->strDomain=strDomain;
			pHost->strPath=strPath;
			this->m_cookies.Add(pHost);
			XCOOKIE*pCookie=new XCOOKIE();
			pCookie->nCreateTime=nTime;
			pCookie->strKey=strKey;
			pCookie->strValue=strValue;
			pHost->cookies.Add(pCookie);
		}
	private:
		XBOOL IsCookieOf(XHOSTCOOKIE*cookie,XURL&url)
		{
			if(url.m_strPath.Find(cookie->strPath)!=0) return XFALSE;
			int iStart=url.m_strHost.Find(cookie->strDomain);
			if(iStart<0||iStart+cookie->strDomain.GetLength()!=url.m_strHost.GetLength())
				return XFALSE;

			return XTRUE;
		}
		void Release()
		{
			int i,nc=m_cookies.GetSize();
			for(i=0;i<nc;i++)
			{
				//m_cookies[i]->strCookies.RemoveAll();
				m_cookies[i]->Release();
				m_cookies[i]->strDomain.RemoveAll();
				m_cookies[i]->strPath.RemoveAll();
				delete m_cookies[i];
			}
			m_cookies.RemoveAll();
		}
		_XHOSTCOOKIES m_cookies;
	};
	XHOSTCOOKIES m_cookies;
	friend class XMainWnd;
	friend class XFlashView;
	friend class CAnalyDialog;
#ifdef __APPLE__
	friend class CMobileView;
#endif
	
#ifdef __ANDROID__
private:
    pthread_t _threadId;
    pthread_mutex_t _mutex;
	friend class XMobileView;
	friend class XMobileViewGLES;
	friend class XMobileViewSurface;
#endif

protected:
	XString8 m_strCnt;
	void StopContent();
	XDom* RefDom(XDom*pDom);
	void FreeDom(XDom*pDom);
	XU32 m_nCmds[MAX_CMDS];
	int  m_nCmdSet,m_nCmdGet;
	XBOOL AddCommand(XU32 nCmd,XU32 nType,XU32 nData);

protected:
	void ReConnect(const char*strURL);
	void ReConnect(XDOMFILTER*pFilter);
	XString8 m_strFilter;
	XU16 m_strShowBarAndOnSale;
    XString8 m_strPaySDKPid;// zhengjl 2012.06
#ifdef __APPLE__
    XString8 m_chargeUrl;
#endif


public:
	XFlashView* GetFlashView()
	{return m_pFlashView;}
	XString8& GetContent(){return m_strCnt;}
	void ReConnectEx(XDOMFILTER*pFilter)
	{
		AddCommand(XWM_COMMAND,XCMD_DOM_RECONNECT,(int)pFilter);
	}
	XBOOL IsEmpty()
	{
		return !m_pDom||m_pDom->IsEmpty();
	}
	XBOOL m_bHome;
	XBOOL m_bShowView,m_bBackList,m_bPushed,m_bLoaded;
	//XURL  m_localURL;
	CONNECTREQ m_localREQ;
#ifdef _SYMBIAN
	enum {XTYPE_DOM,XTYPE_TEXT,XTYPE_FLASH,XTYPE_MEDIA,XTYPE_APP,XTYPE_SIS};
#else
	enum {XTYPE_DOM,XTYPE_TEXT,XTYPE_FLASH,XTYPE_MEDIA,XTYPE_APP};
#endif
	void ReleaseClient();
	//for debug
//#ifdef _SOURCECODE
	XString8 m_strSource;
//#endif
	XDom*	GetDom(){return m_pDom;}

//	CONNECTREQ* GetConnectReq(){return &m_req;}
	XURL&GetConnectURL()
	{
		return m_pDom->GetURL();
	}
	XGraphics* GetGraphics()
	{
		return m_draw.pDraw;
	}
	bool IsImageClient(XClient*pClient);
	void MakeErrorPage(XU32 nCode,XPCTSTR strRetryURL,XU8 bBack);
	XBOOL JustCard(CONNECTREQ&req1,CONNECTREQ&req2);
	void Back(XU8 bForce=XFALSE,bool bStop=true,bool bLocal=true);
	void SetTitle(XPCWSTR strTitle);
	virtual XBOOL Connect(CONNECTREQ&req,XBOOL bSet=XFALSE);
	XBOOL Stop(XBOOL bFinal,XBOOL bForward);
	void ClientToDoc(XPoint&pt);
	void GetClientRect(XRect&rect) {rect=m_view;}
	XBOOL Layerout();
	bool CanMoveScreen();	//add by hgl
	void OffXChangeVal();
	void SetViewRegion(int left, int top, int right, int bottom );
//#ifdef __ANDROID__
//#else
//	virtual void AppendMenu(XMenu&menu);
//#endif
	XBOOL Create(XRect&rect,XWindow*pWnd,XU32 nCmd=XCMD_VIEW);
	XDomView(XGraphics*pDraw,XClientApply*pa);
	virtual ~XDomView();
	void InitContext(XU8 nParaSpace=2,XU8 nRowSpace=0,XU8 nColSpace=0);
	void  SetDocSize(int cx,int cy);
protected:
	void SetCursor(XU8 nType) {m_pParent->SetCursor(nType);}
	void  Release(XBOOL bFree);
#ifdef _SCROLLVIEW
	XHScrollBar m_hBar;
	XVScrollBar m_vBar;
#else
	int m_nOffX,m_nOffY,m_nMaxX,m_nMaxY,m_nSpeedX,m_nSpeedY;
	XU8 m_bScroll;
	XU32 m_nTickTime;
	//int m_nImageLoading;
	int m_nDownX,m_nDownY,m_nDownOffX,m_nDownOffY;
#endif
	XBOOL OnPaint(XGraphics&g);
	void PaintLowNetSpeedTip(XGraphics&g);
	void PaintPageLoading( XGraphics&g );
	XRect m_view;
	XBOOL m_bMouseDown,m_bCapture;
public:
	//XString8Array& GetCookies(){return m_strCookies;}
protected:
	void FreeList(REQARRAY&list);
#ifdef __ANDROID__
#else
	XMessageBox* m_pBox;
	XStatusBar m_status;
#endif
	void SetSpeed(const XPOINT&pt);
	void OnMove(XRect&rect);
	void InitTypeData(XClient*pClient);
	XMutex m_ptLock;
	XBOOL OnKeyUp(XKEYMSG&m);
	XBOOL IsTypeEvent();
	XBOOL SetTypeData(XU8Array&data,int nSize,XClient*pClient);
	void ToDomView(bool bStop=true,bool bBack=true);
	XFlashView* m_pFlashView;
	virtual void ToFlashView(XU32 nTotal,XClient*pClient);
    //static void* thread_xdomview(void *mpClient);
	XU8 GetDocType(RESPONSEINFO*pInfo,CONNECTINFO*pCon);
	void OnDblClick(XPoint&pt,int iPos);
	void OnBlur();
	void OnFocus();
	void ResetEventData();
	virtual XBOOL ProcSysURL(CONNECTREQ&req);
	XBOOL OnChar(XWCHAR c);
	void OnString(XPCWSTR strBuf);
	void AddClick(XPCWSTR strTitle,XBOOL bSameHost);
	XBOOL ProcHttpStatus(XClient*pClient,XBOOL bHtml,XBOOL&bLoadImage);
	XBOOL ProcJQScript(XClient*pClient,XDOMFILTER*pFilter);
	void SetCookies(CONNECTREQ&req);
	void SetCookHost(XURL&url1,XURL&url2);
	void SetCookie(RESPONSEINFO*pInfo,XURL&url);
	
	//XString8Array m_strCookies;
	void PushForward(XBOOL bCard);
	void Forward();
	void CloseView();
	REQARRAY m_reqList,m_forList;
	void OnTimer();
	void SetStatus();
	void SetOffset(int x,int y,bool bUpdate);
	XBOOL LoadImages(bool bImage);
	virtual void OnFinished(XClient*pClient,XBOOL bFinished=XTRUE);
	virtual void OnRecive(XClient*pClient);
	virtual void OnStarted(XClient*pClient);
	void ToDomViewEx();
	virtual void OnConnect(XClient*pClient);
	virtual XBOOL HandleSocket(XU32 nCmd,XClient*pClient);
	XINT CalcPecent(XClient&client);
	virtual XINT CalcPecent();
	void OnClose();
	CLIENTLIST m_clients;
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	CONNECTREQ m_req,m_saveReq;
	void PushREQ(XBOOL bCard,XBOOL bNewDom=XTRUE);
	void SaveREQ();
	XClient* m_pClient;
	void OnClick(XPoint&pt,int iPos);
	void Invalidate();
	XBOOL OnKeyDown(XKEYMSG&m);
	DRAWCONTEXT m_draw;
	XDom* m_pDom;
	void GetOffset(XPoint&pt);
	void ProcResult(XU32 s,bool bFinal=false,bool bAuto=false);
	void OnMouseMove(XPoint&pt,int iPos);
	XClientApply* m_pClientApply;
	void GetClipRect(XRect&rect,XBOOL bFull);
	XBOOL OnCommand(XU32 nCmd,XWindow*pWnd);
	XU8	 m_bStopreq,m_nMoveTimes;
	XU16 m_nStatus;
	XVector<XWindow*>m_popList;
	MENUDATAS m_domMenus;
#ifdef _SYMBIAN
	XDialogSave *m_hSaveDialog;
#endif
public:
	void SetURLCookie(XString8&strURL,XString8&strCookie,XU32 nTime,RESPONSEINFO *pInfo=NULL);
	XClientApply* GetClientApply();
	void CallBack(XThread&thread);
	void ReleaseCall();
	XSWFPlayer* GetPlayer();
	XBOOL GetTextEdits(XSObjectArray&list);
	XPCTSTR GetWorkPath();
//	XPCTSTR GetWorkPath;
	XClient* ConnectContext(XPCTSTR strURL,int method,XU8Array&data,void*notify,const char*refer=NULL,
							const char*strType=NULL,XString8Array*headers=XNULL,bool bCache=true);
	void ConnectPost(XPCTSTR strURL,int method,XU8Array&data);
	void Connect(XPCTSTR url,int method,XPCTSTR window,XU8Array&data,const char*strType=NULL,XString8Array*headers=XNULL);
	void ConnectEx(XPCTSTR url,int method,XPCTSTR window,XU8Array&data,const char*strType=NULL,XString8Array*headers=XNULL);
	XBOOL AddDomMenu(XPCWSTR strTitle,XU32 data);
	void OnMouseUp(XPoint&pt,int iPos);
	void OnMouseDown(XPoint&pt,int iPos);
	void AddPopup(XWindow*pWnd)
	{	m_popList.Add(pWnd); }
public:
	CONNECTREQ m_laterREQ;
	XBOOL GetCookies(XString8&strCookies);
	XBOOL GetCookieOf(XString8&strName,XString8&strCookies);
	XBOOL GetVarOf(XString8&strName,XString8Array&strArgs,XString8&strValue,const char*strDomain=NULL,bool bFun=false,bool bIndex=false,bool bMatch=false);
	XBOOL SetValueOf(XString8&strVar,const char*strArg=NULL,bool bURL=false);
	XBOOL SetVarOf(XString8&strVar,const char*strDomain=NULL,const char*pArg=NULL,bool bURL=false);
	XURL& GetReferURL();
	static XBOOL GetContextOf(XString8&strCnt,XString8&strResult,int iStart,bool bMatch=true,bool bTrim=true);
	static XBOOL GetMD5Of(XU8Array&data,XString8&out);
	XBOOL IsException(){return m_bException;}
protected:
	XU32  m_nLastClick;
	XBOOL m_bException;
	int m_nScrollX,m_nScrollY;
	XU32 DrawDoc(XRect&clip,XGraphics&g);
	void SetNewFilter(XURL&url);
	virtual void SetPercent(int nPer){}	
public:
	XString16 m_strTitle;
	DRAWCONTEXT* GetDraw(){return &m_draw;}
	XU32	  m_nLockClick;
	XBOOL	  m_bShowLoading;

	bool	  m_bShowPageLoading;
	XU32	  m_iPageLoadingIndex;
	XImage*	  m_pImgPageLoading;

	XBOOL	  m_bShowEnd;
	//XImage*   m_pImgGameHelp;
	//XU32	  m_iGamehelpIndex;
	//static XImage* m_pImgLowNetTip;
	//static bool	mbLowNetSpeed;
	XImage*	  m_pImgWaitting;
	XU32	  m_WaitingIndex;

	XString8  m_strFile0;
	XString8  m_strFile1;
	XU32	  m_nFileSize0;
	XU32	  m_nFileSize1;
	XU32	  m_nFileTotal0;
	XU32	  m_nFileTotal1;
	XU32	  m_nLoadingEnd;
	void	  LoadingStart(XDOMFILTER*pFilter);
	void	  LoadingEnd();
	void	  PaintLoading(XGraphics&g);
	int		  DrawFile(XGraphics&g,XString8&strFile,XU32 nSize,int nTotal,int y);
	void      StartFile(XString8&strFile);
	void	  LoadingData(XU32 nSize,XU32 nTotal);
	void	  UpdateLoading();
	XMutex	  m_loadingLock;
	XU32	m_nPecent;
//#if (__CORE_VERSION__>=0x02076000)
	XImage*	  m_pLoadingBG;
	int		  m_nLoadingBGIndex;
//#endif
};



#endif // !defined(AFX_XDOMVIEW_H__605F4B1E_D32B_42AC_AD68_1F981B87FF8E__INCLUDED_)

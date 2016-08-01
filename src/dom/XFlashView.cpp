// XFlashView.cpp: implementation of the XFlashView class.
//scaleMode
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XFlashView.h"
#include "XDomCard.h"
#include "XHTMLScript.h"
#include "XDomViewExt.h"
#include "XDialogSave.h"
#include "XBWSendMessageContent.h"
#include "WindowView.h"
#include "XSystem.h"

#include "Raster_gl.h"

#ifdef __APPLE__
#include "GlobalPortingFunction.h"
#include "IPhoneDeviceData.h"
extern iOSDeviceRelateData g_deviceData;
#endif
BWSendMessageContentClass *g_BaiWanPayUrlSavedData = NULL;

#define OPT_COUNT 6
#define POPWIN_TITLE 50

#ifdef _WINEMU
#include "XMainWnd.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MAXBARTIME 30
#define BARHEIGHT  20
#define MOVESTEP   20
#define TWOPTSTEP  200
#define TWOSTARTSTEP 50
#define ACCDELAY 2

#define MAXPECENT 120
#define LINESTEP  40
#define BLACKSTEP  10
#define LWHITECOLOR 0xffffffa0
#define LBLACKCOLOR 0xff000000

const XPCTSTR _strFlashConfig="FLASHCONFIG";

//#define _FRAMESHOW

#define __BLACKBACKGROUND__

#if defined( _FRAMESHOW )
static int _nLastUpdate=0;
static int _nActionCount=0;
static int _nUpdateCount=0;
static int _nUpdateFrames=0;
static int _nActionFrames=0;
#endif

#if defined (__APPLE__)
#if BWMEMORYSHOWMODE
#include "XSystem.h"
static int _nFreeMemory=0;
static int _nTotalMemory=0;
#endif  // BWMEMORYSHOWMODE
#endif  // __APPLE__

//static XDOMFILTER filter;

XFlashView::XFlashView(XDomView*pDom):m_infoAlloc(sizeof(CLIENTINFO),6)
	,m_winSize(RasterGL::sharedRasterGL()->m_WinSize)
{
	m_nLockOptTime=0;
//	m_nConPecent=0;
	m_bFlashDown=false;
	m_nLockPtTime=5;
	m_nLastTickTime=0;
	m_nLineOffset=0;
//m_nMaxPecent=MAXPECENT;
	m_nZoomPecent=0;
//	m_bLockPoint=0;
	m_nFileLength=0;
	m_pFirstClient=NULL;
	m_nFirstLen=0;
	m_nLastLen=0;
	m_nACCDelay=0;
	m_nStartTime=64;
	m_nTwoPoint=0;
	m_pConfigClient=NULL;
	//m_bZoomSet=DRAG_START;
	m_nDragSet=DRAG_START;
//	m_bMoveSet=false;
	m_bWait2=false;
	m_bHor=true;
	m_nDownTime=0;
	m_nTimeCount=0;
	m_bSaveReady=XFALSE;
#ifdef __APPLE__
	m_flashSaveReady = false;
	m_topScoreAvailable = false;
#endif
#ifdef __ANDROID__
	m_flashSaveReady = false;
	m_topScoreAvailable = false;
#endif

	XClientApply*pApply=pDom->GetClientApply();
	m_player=new XSWFPlayer(pApply->m_nDeltaX,pApply->m_nDeltaY);
	m_player->m_pDom=pDom->GetDom();
	m_player->m_nDomSize=pDom->GetDom()->m_nDataSize;
	m_player->m_argList.Append(pDom->GetDom()->m_argList);
	m_pClientInfo=NULL;

	//m_nCntMode=XCNT_MODE_LOCAL;//LOCAL;//LOCAL;

	//dushixiong --start
	m_nPecent=0;
	m_nLenDown=0;
	m_nLenPrev=0;
	m_pObjectFilter=XNULL;
	m_player->m_pClientApply = pDom->m_pClientApply;
	//dushixiong --end

//	m_bExtern=XFALSE;
	m_bMapDown=XFALSE;
	m_nConnecting=0;
	m_pDom=pDom;
	m_nCallCount=0;
	m_bConfigOK=XFALSE;
	m_nTotalDatas=0;
	m_bMouseDown=XFALSE;
	m_nBarMode= XBAR_SHOW; //XBAR_HIDE;
	m_nBarTime=MAXBARTIME;
	m_pCfgDom=XNULL;
//	m_pConfig=XNULL;
	m_strHelp=XNULL;
	m_nHelpLength=0;
	m_bLoadOK=XFALSE;
	m_nBarPos=0;
	m_bNetwork=XFALSE;
	m_nWaiting=0;
	m_bInitOK=XFALSE;
	m_nPecent=0;
//#ifdef __ANDROID__
//#else
//	m_pBox=XNULL;
//#endif
	//m_pData=XNULL;
	m_player->m_strWorkPath.SetString(pDom->GetWorkPath(),XString8::SafeStrlen(pDom->GetWorkPath()));
	m_player->SetFilePath(pDom->GetClientApply()->m_strFilePath);
	m_player->InitCachePath();
	XXVar path=m_player->m_strWorkPath;
	XXVar sh("/Shared");
	path.StringAdd(sh);
	XFile::CreateFolder(path.strTxt);
	m_player->m_strWorkPath.AddChar('/');

	m_bPause=XFALSE;
//	m_pTxtView=XFALSE;
	m_pSave=XNULL;
	m_nCmdSet=m_nCmdGet=0;
	m_bMapDown=XFALSE;
//	m_nZoom=0;
//	m_pEdit=XNULL;


	if ( ! RasterGL::sharedRasterGL()->mbInit )
	{
#ifdef __APPLE__
        if(WindowView::sharedWindowView()->InitOK()){
#endif
            WindowView::sharedWindowView()->initGL();
            RasterGL::sharedRasterGL()->mbInit = true;
            RasterGL::sharedRasterGL()->Init();
#ifdef __APPLE__
        }
#endif
	}
	else
	{
		WindowView::sharedWindowView()->MakeCurrent(true);
		MATRIX mat;
		::MatrixIdentity(&mat);
		RasterGL::sharedRasterGL()->SetCamera(mat);
	}

}

XFlashView::~XFlashView()
{
	if(m_pSave)
		delete m_pSave;
#ifdef __ANDROID__
//#else
//	if(m_pBox)
//		delete m_pBox;
    WindowView::sharedWindowView()->destroyGL();
    LOGWHERE();
#endif
	Release();
	if(m_player)
	{
		XClientApply*pApply=m_pDom->GetClientApply();
		pApply->m_nDeltaX=m_player->m_nClickDeltaX;
		pApply->m_nDeltaY=m_player->m_nClickDeltaY;
		m_player->FreeBuffer();
		delete m_player;
		m_player=XNULL;
	}
	FreeInfo();
	XSWFPlayer::ReleaseGlobal();
	RasterGL::ClearCache();
	//RasterGL:Uninit();//Release();
	//if ( RasterGL::sharedRasterGL()->mbInit )
	//	RasterGL::sharedRasterGL()->Uninit();
}

XBOOL XFlashView::Create(XRect &rect, XWindow *pWnd,XU32 nTotal, XU32 nCmd)
{
	m_pWaitImg=GetIcon(XICON_LOADING);
	m_left.CreateFromResource("LEFTSOFT");
	m_optImage.CreateFromResource("LIGHTS");
	m_nTotalDatas=nTotal;
	m_nMouseX=(rect.right+rect.left)>>1;
	m_nMouseY=(rect.top+rect.bottom)>>1;

#ifdef _THREADBACK
	m_thread.Create(this,100);
#endif
	//m_winSize=rect.Size();

//#if defined(__TOUCHPAD__)||defined(__POINT__)
//#ifdef __TOUCHPAD__
//	m_bar.Create(this,rect.Height(),rect.Width(),XTRUE,XICON_GREENBLOCK);
//#else
//	m_bar.Create(this,rect.Height(),rect.Width(),XFALSE,XICON_GREENBLOCK);
//#endif
//	m_bar.SetImage("ICONSSHOW","ICONSGRAY","ICONSACTIVE","ICONSCHECK");
//	if(m_nBarMode==XBAR_HIDE)
//	{
//		m_nBarPos = 0;
//		int h=m_bar.GetWindowRect().Height(),rh=rect.Height();
//		XRect rtt(0,rh-m_nBarPos,rect.Width(),rh+h-m_nBarPos);
//		m_bar.MoveWindow(rtt);
//	}
//	else
//	{
//		m_nBarPos=m_bar.GetWindowRect().Height();
//		//m_winSize.cy-=m_nBarPos;
//	}
//#endif

	m_nBarTime=MAXBARTIME;
	m_player->InitPlayer(m_winSize,this);
#ifdef __APPLE__ 
    m_showRect =rect;
#endif
	XBOOL bOK=XWindow::Create(rect,XES_VISIABLE|XES_FOCUS,pWnd,nCmd);
	//WindowView::sharedWindowView()->resize(rect.Width(),rect.Height());
	return bOK;
}


XBOOL XFlashView::AddCommand(XU32 nCmd,XU32 nType,XU32 nData,XBOOL bReplace )
{
	//if(nCmd==XBEGINSCORE)
	//	int v=0;
	if(bReplace)
	{
		int vi=m_nCmdGet;
		while(vi!=m_nCmdSet)
		{
			if(m_nCmds[vi]==nCmd)
			{
				/*switch(nCmd)
				{
				case XMOVECNT:
					m_nCmds[vi+1]+=nType;
					m_nCmds[vi+2]+=nData;
					break;
				default:*/
					m_nCmds[vi+1]=nType;
					m_nCmds[vi+2]=nData;
				/*	break;
				}
				return XTRUE;*/
			}
			vi+=3;
			if(vi>MAX_CMDS)
				vi-=MAX_CMDS;
		}
	}
	int vi=m_nCmdSet+3;
	if(vi>=MAX_CMDS)
		vi-=MAX_CMDS;
	if(vi==m_nCmdGet) 
		return XFALSE;

	m_nCmds[m_nCmdSet]=nCmd;
	m_nCmds[m_nCmdSet+1]=nType;
	m_nCmds[m_nCmdSet+2]=nData;
	m_nCmdSet=vi;
	//if(m_nCmdSet==m_nCmdGet) return XFALSE;
	return XTRUE;
}

XBOOL XFlashView::NeedStop()
{
#ifdef _THREADBACK
	return m_thread.NeedStop()||m_player->IsStopEvent();
#else
	return m_player->IsStopEvent();
#endif
}

XUINT XFlashView::CallBack(XThread&thread)
{
#ifdef _THREADBACK
	while(!m_thread.NeedStop())
#endif
	{
		if(thread.NeedStop())
			m_player->Stop();
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//		if(m_bConfigOK&&!m_player->GetConfig()->m_strScoreVar.IsEmpty())
//		{
//			if(m_nCallCount<50)
//				m_nCallCount++;
//			else
//			{
//				m_nCallCount=0;
//				XXVar var;
//				XBOOL bOK=GetScore(var);
//				if(bOK&&var.ToInt()!=0)
//					m_bar.SetItemStatus(XCMD_FLASH_SCORE,XTS_NORMAL);
//				else
//					m_bar.SetItemStatus(XCMD_FLASH_SCORE,XTS_DISABLE);
//			}
//		}
//#endif

//获取是否上传积分，目前该功能不需要 2013-09-04 hgl
//#if defined( __APPLE__ ) || defined( __ANDROID__ )
//	if(m_bConfigOK&&!m_player->GetConfig()->m_strScoreVar.IsEmpty())
//	{
//		if(m_nCallCount<50)
//			m_nCallCount++;
//		else
//		{
//			m_nCallCount=0;
//			//bScore=XTRUE;
//			XXVar var;
//			XBOOL bOK=GetScore(var);
//			if(bOK&&var.ToInt()!=0)
//				m_topScoreAvailable = true;
//			else
//				m_topScoreAvailable = false;
//		}
//	}
//#endif

#if defined (__APPLE__)
#if BWMEMORYSHOWMODE
        if (!_nTotalMemory)
            _nTotalMemory=(XSystem::GetTotalMemory()/1024)/1024;
        _nFreeMemory=(XSystem::GetFreeMemory()/1024)/1024;
#endif  // BWMEMORYSHOWMODE
#endif  // __APPLE__

#ifdef _FRAMESHOW
		if(!_nLastUpdate)
			_nLastUpdate=XTime::GetTickCount();
		_nActionCount++;
		XRect rectt(0,0,m_nTextHeight*8,m_nTextHeight+2);
		XU32 t=XTime::GetTickCount();
		if(t-_nLastUpdate>2000)
		{

			int delta=t-_nLastUpdate;
			_nActionFrames=(_nActionCount*1000+1000)/delta;
			_nUpdateFrames=(_nUpdateCount*1000+1000)/delta;
			InvalidateRect(rectt);
			//thread.Sleep(XWAITMONMENT);
			_nUpdateCount=0;
			_nActionCount=0;
			_nLastUpdate=t;
			//XWindow*pWnd=XWindow::m_pMainWnd;
			//if(pWnd)
			//	pWnd->Handle(XCMD_UPDATEPROFILE,XCMD_UPDATE_FRAMES,_nActionFrames);
		}
#endif

//		XThread::Sleep(XWAITMONMENT<<1);
		if(!SetData())
			return 0;
//#ifndef _THREADBACK
//		if(m_pTxtView&&m_pTxtView->IsInstance())
//			m_pTxtView->CallBack(thread);
//#endif
//		XThread::Sleep(1);
		if(m_player->IsEmpty())
		{
			if(m_pWaitImg)
			 {
				m_nWaiting++;
				if(m_nWaiting>=m_pWaitImg->FrameCount()+10)
					m_nWaiting=10;
				XRect rect;
				GetWaitRect(rect);
				if(m_nStartTime)
					Invalidate();
				else
					InvalidateRect(rect);
				thread.Sleep(XWAITMONMENT);
			 }

#ifdef _THREADBACK
			m_thread.Sleep(XSHORTTIME);
			continue;
#else
			thread.Sleep(XSHORTTIME);
			return 0;
#endif
		}
		if(m_player->IsStopEvent())
			return 0;

		if(m_bPause)
		{
#ifdef _THREADBACK
			m_thread.Sleep(XSHORTTIME);
			continue;
#else
			return 0;
#endif
		}

		//LockKeyButtons();

		XBOOL bSet=XTRUE;
		XRect clip;

		//XLock lock(&m_runLock);       // modify by hgl
		if(!m_runLock.Lock(XSHORTTIME))
		{
#ifdef _THREADBACK
			continue;
#else
			return 0;
#endif
		}
		int v = 0;
		v=m_player->DoPlay(clip,true);

		m_runLock.UnLock();

		switch(v)
		{
		case XSWFPlayer::PLAY_BREAK:
		case XSWFPlayer::PLAY_END:
			 bSet=XFALSE;
			 if(m_nWaiting)
			 {
				m_nWaiting=0;
				XRect rect;
				GetWaitRect(rect);
				if(m_nStartTime)
					Invalidate();
				else
					InvalidateRect(rect);
				thread.Sleep(XWAITMONMENT*4);
			 }
			 break;
		case XSWFPlayer::PLAY_HITNONE:
		case XSWFPlayer::PLAY_OK:
#ifdef _THREADBACK
			 //m_thread.Sleep(XWAITMONMENT);
#endif
			 if(m_nWaiting)
			 {
				m_nWaiting=0;
				XRect rect;
				GetWaitRect(rect);
				if(m_nStartTime)
					Invalidate();
				else
					InvalidateRect(rect);
				thread.Sleep(XWAITMONMENT*4);
			 }
			 break;
		case XSWFPlayer::PLAY_WAIT:
			 if(m_pWaitImg)
			 {
				m_nWaiting++;
				if(m_nWaiting>=m_pWaitImg->FrameCount()+10)
					m_nWaiting=10;
				XRect rect;
				GetWaitRect(rect);
				if(m_nStartTime)
					Invalidate();
				else
					InvalidateRect(rect);
				thread.Sleep(XWAITMONMENT*4);
			 }
#ifdef _THREADBACK
			 m_thread.Sleep(XSHORTTIME);
#endif
			 break;
		case XSWFPlayer::PLAY_WAITSHORT:
#ifdef _THREADBACK
			 m_thread.Sleep(2);
#endif
			 break;
		}
		if(!bSet)
		{
#ifdef _THREADBACK
			break;
#else
			return 0;
#endif
		}
		/*if(!clip.IsRectEmpty())
		{
			//clip+=m_flashBase;
			InvalidateRect(clip);
			//thread.Sleep(XWAITMONMENT);
		}*/
	}
	return 0;
}

void XFlashView::Release()
{
#ifdef _THREADBACK
	m_player->Stop();
	m_thread.Stop();
	m_thread.Release();
#endif
	if(m_pCfgDom)
		delete m_pCfgDom;
	//if(m_pTxtView)
	//	delete m_pTxtView;
	//m_pTxtView=XNULL;
	m_nCmdSet=m_nCmdGet=0;
	m_modeMenu.FreeAll();
	m_pCfgDom=XNULL;
}


XBOOL XFlashView::AppendData(XU8Array&data,int nSize,XClient*pClient)
{
	if(!pClient->GetNotifyID())
		return XFALSE;
	if(nSize<=0)
		return XTRUE;

	//int nSize=data.GetSize();
	RESPONSEINFO*pInfo=pClient->GetResponseInfo();
	if(pInfo->type.m_nType==XContentType::TXCONFIG)
	{
		if(m_pCfgDom)
		{
			m_configData.Append(data,nSize);
			if(!m_pCfgDom->PhaseXML(data,nSize,false,true))
				return XTRUE;
		}
		return XTRUE;
	}
	switch(pClient->m_nUserData)
	{
	case 1:
		 break;
	case 0:
		{
			XU32*nHeader=(XU32*)data.GetData();
			if(nHeader[0]==0xfffffff1)
			{
				pClient->m_nUserData=nHeader[1]+16;
				pClient->m_userData.Append(data.GetData(),nSize);
				nSize=0;
			}
			else
			{
				pClient->m_nUserData=1;
				break;
			}
		}
	default:
		if(nSize)
			pClient->m_userData.Append(data.GetData(),nSize);
		if(pClient->m_userData.GetSize()>=pClient->m_nUserData)
		{
			XString8 strConfig;
			if(XClientApply::BinDecode(pClient->m_userData.GetData(),
										pClient->m_userData.GetSize(),
										strConfig)>0)
			{
				m_configData.Append((XU8*)strConfig.GetData(),strConfig.GetLength());
				if(m_pCfgDom==XNULL)
					m_pCfgDom=new XDom();
				m_pCfgDom->Reset(XCHARSET_UTF8,XCNT::CTEXT,XCNT::TVND_WAP_WML,
								 &m_pDom->m_draw);
				m_pCfgDom->PhaseXML((XU8*)strConfig.GetData(),strConfig.GetLength(),false,true);

				m_player->GetConfig()->Reset();
				PhaseConfig();
				m_player->UpdateConfig();
				m_bConfigOK=XTRUE;
				m_player->GetConfig()->m_bConfigOK=XTRUE;
				UpdateToolBar();
				AddCommand(XMOVE, 0, 0);
				AddCommand(XUPDATEBUTTONS,0,0);
				//PaintButtons();
				//Invalidate();
			}
			else
			{
				pClient->m_userData.RemoveAll();
				return XFALSE;
			}
			nSize=pClient->m_userData.GetSize()-pClient->m_nUserData;
			if(nSize)
			{
				data.SetSize(nSize);
				memcpy(data.GetData(),pClient->m_userData.GetData()+pClient->m_nUserData,nSize);
			}
			pClient->m_nUserData=1;
			pClient->m_userData.RemoveAll();
		};
	}

	if(!nSize) 
		return XTRUE;
	
#ifdef _THREADBACK
	return AddCommand(XAPPEND_DATA,data.DataTo(),nSize,pClient);
#else

	XBOOL bSet=XTRUE,bInitOK=XFALSE;
	_XStreamData*pStream=(_XStreamData*)pClient->GetDataI();
	if(pStream->scriptPlayer==XNULL && pStream->pLoader==XNULL)
		return XFALSE;

	pStream->nLoadedBytes=pInfo->nTotalSize;
	pStream->nTotalBytes=pInfo->nLength;
Again:
	SaveBytes(data.GetData(),nSize,false,pClient);
	int nType=m_player->StreamInWrite(pStream,data.GetData(),nSize,pClient);

	//XSize sz;

//    XSize sz1 = m_rect.Size();
//    SetSize(sz1);
//    m_player->FullScreen();
    
	switch(nType)
	{
	case XSWFPlayer::LOAD_INITOK:
		 m_bInitOK=XTRUE;
		 break;
	case XSWFPlayer::LOAD_AGAIN:
		 if(m_player->IsStopEvent()|| pClient->IsStoped())
		 {
			 bSet=XFALSE;
			 break;
		 }
		 else
		 {
			 pClient->Sleep(XWAITMONMENT);
			 goto Again;
		 }
		 break;
	case XSWFPlayer::LOAD_FAILED:
		 m_player->Stop();
	case XSWFPlayer::LOAD_BREAK:
	case XSWFPlayer::LOAD_END:
		 bSet=XFALSE;break;
	case XSWFPlayer::LOAD_OK:
		 //m_hStop.SetEvent();
		 break;

		//if(!bSet) break;
	}
	return bSet;
#endif

}

//void XFlashView::SetLoadPercent(int nPer)
//{
//	//LOGWHEREVAL( nPer );
//	if(nPer==m_nConPecent)//||!m_pTxtView||!m_pTxtView->IsInstance())
//		return;
//	m_nConPecent=nPer;
//	XRect rt=m_pTxtView->GetWindowRect();
//	rt.bottom=rt.top-1;
//	rt.top-=7;
//	InvalidateRect(rt);
//	//LOGE("%d, left=%d,top=%d, right=%d, bottom=%d", __LINE__, rt.left, rt.top, rt.right, rt.bottom );
//}

void XFlashView::PaintLowNetSpeedTip(XGraphics&g)
{
	//XPoint pt(m_rect.Width()/2-XDomView::m_pImgLowNetTip->Width()/2, m_rect.Height() / 2 - XDomView::m_pImgLowNetTip->Height() / 2);
	//g.DrawImage(*XDomView::m_pImgLowNetTip,pt,0);
}

//void XFlashView::PaintTxtView( XGraphics & g )
//{
//	//XRect rt(m_rect.left,m_rect.top,m_rect.right,m_rect.top+POPWIN_TITLE);
//	XRect rt=m_pTxtView->GetWindowRect();
//	g.SetBackColor(0xfffffff0);
//	g.FillRectangle(rt);
//	XRect rtt(rt.left-3,rt.top-POPWIN_TITLE,rt.right+3,rt.top);		
//	g.SetBackColor(0xff212E37);
//	g.FillRectangle(rtt);
//	rtt.top=rt.bottom;
//	rtt.bottom=rt.bottom+7;
//	g.FillRectangle(rtt);
//	XDom*pDom=m_pTxtView->GetDom();
//	//if(pDom->GetTitl
//	XImage*pClose=m_pTxtView->GetCloseImage();
//	if(!pClose->IsEmpty())
//	{
//		int h=pClose->Height();
//		XPoint dst(rt.right-pClose->Width()-5,
//			rt.top-7-(POPWIN_TITLE-7-h)/2-h);
//		g.DrawImage(*pClose,dst);
//	}
//	if(m_nConPecent)
//	{
//		int w=rt.Width()*m_nConPecent/100;
//		if(w<10) w=10;
//		XRect rp(rt.left,rt.top-8,rt.left+w,rt.top);
//		g.SetBackColor(0xff39B272);
//		g.FillRectangle(rp);
//	}
//
//	XString16 strTitle=m_pTxtView->m_strTitle.IsEmpty()?"":m_pTxtView->m_strTitle;
//	XSize sz;
//	int nMaxLen=(rt.Width()-40)*2/3;
//	XU32 s=g.GetFontStyle();
//	g.SetFontSize(XGraphics::XS_HEAD4);
//	g.MeasureString(strTitle,sz);
//	int l=strTitle.GetLength()-3;
//	XPWSTR strTxt=strTitle;
//	while(sz.cx>nMaxLen&&l>0)
//	{
//		l--;
//		strTxt[l]='.';
//		strTxt[l+1]='.';
//		strTxt[l+2]='.';
//		strTxt[l+3]=0;
//		g.MeasureString(strTxt,sz,l+3);
//	}
//	{
//		XPoint dst((rt.left+rt.right-sz.cx)/2,rt.top-POPWIN_TITLE+(POPWIN_TITLE-sz.cy)/2);
//		g.SetColor(0xffffffff);
//		g.DrawString(strTitle,dst);
//	}
//	g.SetFontStyle(s);
//
//	g.SetColor(0xff434C55);
//	g.DrawLine(rt.left,rt.top-8,rt.right,rt.top-8);
//	g.DrawLine(rt.left,rt.top-1,rt.right,rt.top-1);
//	//g.DrawLine(rt.left,rt.bottom+1,rt.right,rt.bottom+1);
//
//	rtt=rt;
//	rtt.bottom+=7;
//	rtt.top-=POPWIN_TITLE-3;
//
//	rtt.InflateRect(1,1);
//	g.SetColor(0xff38393B);
//	g.FrameRectangle(rtt);
//
//	rtt.InflateRect(1,1);
//	g.SetColor(0xff47484A);
//	g.FrameRectangle(rtt);
//
//	rtt.InflateRect(1,1);
//	g.SetColor(0xff2C3033);
//	g.FrameRectangle(rtt);
//	//rt.bottom=m_rect.bottom-m_nBarPos;
//
//
//	//if(m_nConnecting>4&&m_pWaitImg)
//	//{
//	//	int w=m_pWaitImg->Width();
//	//	int h=m_pWaitImg->Height();
//	//	int cx=m_rect.Width()>>1,cy=m_rect.Height()>>1;
//	//	m_pWaitImg->SetActive(m_nConnecting%m_pWaitImg->FrameCount());
//	//	g.DrawImage(*m_pWaitImg,XPoint(cx-w/2,cy-h/2),XRect(0,0,w,h));
//	//	//InvalidateRect(XRect(cx-w-1,cy-h-1,cx+w+1,cy+h+1));
//	//	//}
//	//}
//
//}

void XFlashView::PaintBlackBG( XGraphics &g )
{
	if(m_nWaiting>=10)
	{
#ifdef __BLACKBACKGROUND__
		XRect rect;
		GetClientRect(rect);
		g.SetBackColor(0xff000000);
		GetWaitRect(rect);
		g.FillRectangle(rect);
		//g.SetColor(0xff000000);
		//g.FrameRectangle(rect);
		int by=rect.top+5;
		if(m_pWaitImg)
		{
			m_pWaitImg->SetActive(m_nWaiting-10);
			XPoint pt((((rect.Width()-m_pWaitImg->Width())>>1)+rect.left),by);
#ifdef __APPLE__
//			if( !g_deviceData.mbOnSale && g_deviceData.m_scale > 1.1 )
//			{
//				pt.x /= 2;
//				pt.y /= 2;
//				by+=(m_pWaitImg->Height() * 2+ 5);
//			}
//			else
#endif
			{
				by+=m_pWaitImg->Height() + 5;
			}
			g.DrawImage(*m_pWaitImg,pt);
		}
		g.SetColor(0xffa0a0a0);
		XSize sz;
		XString16 strTxt=XResource::LoadWString(XIDS_LOADING);
		strTxt+=XString16("(");
		XString16 is;
		is.FromInt(m_player->player->len/1024);
		strTxt+=is;
		strTxt+=XString16("/");
		is.FromInt(m_player->player->scriptLen/1024);
		strTxt+=is;
		strTxt+=XString16("k)");
		g.MeasureString(strTxt,sz);
		int bx = 0;
#ifdef __APPLE__
//		if (!g_deviceData.mbOnSale && g_deviceData.m_scale > 1.1)
//		{
//			bx= (rect.left+((rect.Width()-sz.cx * 2)>>1)) >> 1;
//			by /= 2;
//		}
//		else
#endif
		{
			bx=rect.left+((rect.Width()-sz.cx)>>1);
		}
		g.DrawString(strTxt,bx, by);

#else
		/*if(m_nStartTime)
		{
		int nColor=((m_nStartTime*4)&0xff)<<24;
		g.SetBackColor(nColor);
		XRect rect;
		GetClientRect(rect);
		g.FillRectangle(rect);
		}*/
		XRect rect;
		GetWaitRect(rect);
		g.SetBackColor(0xffffffef);
		g.FillRectangle(rect);
		g.SetColor(0xff000000);
		g.FrameRectangle(rect);
		int by=rect.top+5;
		//if(m_pWaitImg)
		//{
		//	m_pWaitImg->SetActive(m_nWaiting-10);
		//	XPoint pt(((rect.Width()-m_pWaitImg->Width())>>1)+rect.left,by);
		//	g.DrawImage(*m_pWaitImg,pt);
		//	by+=m_pWaitImg->Height()+5;
		//}
		g.SetColor(0xff3f3f3f);
		XSize sz;
		XString16 strTxt=XResource::LoadWString(XIDS_LOADING);
		strTxt+=XString16("(");
		XString16 is;
		is.FromInt(m_player->player->len/1024);
		strTxt+=is;
		strTxt+=XString16("/");
		is.FromInt(m_player->player->scriptLen/1024);
		strTxt+=is;
		strTxt+=XString16("k)");
		g.MeasureString(strTxt,sz);
		int bx=rect.left+((rect.Width()-sz.cx)>>1);
		g.DrawString(strTxt,bx,by);
#endif
	}
}

void XFlashView::PaintFrameShow( XGraphics &g )
{
#ifdef _FRAMESHOW
	_nUpdateCount++;
	XU32 t=XTime::GetTickCount();
	XPoint drawPoint(0,m_rect.Height() / 2-m_nTextHeight-2);
	//if(m_runLock.Lock())
	{
		XXVar str=_nActionFrames;
		str.ToString(XTRUE);
		str.AddChar(':');
		XXVar v=_nUpdateFrames;
		str.StringAdd(v);
		str.AddChar('(');
		v=m_player->player->frameRate>>16;
		str.StringAdd(v);
		str.AddChar(')');

		XString16 strTxt(str.strTxt);

		g.SetColor(0xffFF00FF);
		g.DrawString(strTxt,drawPoint);
		g.SetColor(0xff00FF00);
		g.DrawString(strTxt,drawPoint);
		//m_runLock.UnLock();
	}
#endif

#if BWMEMORYSHOWMODE
	XPoint memDrawPoint(0,m_rect.Height()/2 -2*(m_nTextHeight-2));
	//if(m_runLock.Lock())
	{
		XXVar str=_nFreeMemory;
		str.ToString(XTRUE);
		str.StringAdd(":");
		XXVar v=_nTotalMemory;
		str.StringAdd(v);
		XString16 strTxt(str.strTxt);
		g.SetColor(0xffFF00FF);
		g.DrawString(strTxt,memDrawPoint);
		g.SetColor(0xff00FF00);
		g.DrawString(strTxt,memDrawPoint);
		//m_runLock.UnLock();
	}
#endif  // BWMEMORYSHOWMODE

}

XBOOL XFlashView::OnPaint(XGraphics &g)
{
	//return XTRUE;
	//LOGWHERE();
	//if(m_pTxtView && m_pTxtView->IsInstance())
	//{
	//	PaintTxtView( g );
	//	return XTRUE;
	//}

	if(m_clip.IsRectEmpty())
		return XTRUE;

	g.SetBackColor(0xff000000);
	//g.FillRectangle(m_clip);

	XRect rc( m_rect );
 //   if(!m_bInitOK||m_player->IsEmpty())
	//{
	//	rc.SetRectEmpty();
	//}
	//else
	//{
	//	rc&=m_clip;
	//	if(!rc.IsRectEmpty() && m_nWaiting==0 )
	//	{
	//		XPoint dst=rc.TopLeft();
	//		rc-=m_flashBase;
	//		if(!m_player->Paint(g,dst.x,dst.y,rc))
	//		{
	//			return XFALSE;
	//		}
	//		rc+=m_flashBase;
	//	}
	//}

#ifdef __APPLE__
#else
	if(rc.IsRectEmpty())
		g.FillRectangle(m_clip);
	else
	{
		XRect rt(m_clip.left,m_clip.top,rc.left,m_clip.bottom);
		if(!rt.IsRectEmpty())
			g.FillRectangle(rt);
		rt=XRect(rc.right,m_clip.top,m_clip.right,m_clip.bottom);
		if(!rt.IsRectEmpty())
			g.FillRectangle(rt);
		rt=XRect(rc.left,m_clip.top,rc.right,rc.top);
		if(!rt.IsRectEmpty())
			g.FillRectangle(rt);
		rt=XRect(rc.left,rc.bottom,rc.right,m_clip.bottom);
		if(!rt.IsRectEmpty())
			g.FillRectangle(rt);
	}
#endif

	//if(m_bLockPoint)
	//{
	//	XRect rect;
	//	this->GetPointZoomRect(rect);
	//	rect.DeflateRect(2,2);
	//	int w=rect.Width();
	//	int h=rect.Height();
	//	int dp=m_nLineOffset+rect.left;
	//	if(dp>rect.left)
	//	{
	//		g.SetColor(LBLACKCOLOR);
	//		if(dp<rect.left+BLACKSTEP)
	//			g.DrawLine(rect.left,rect.top,dp,rect.top);
	//		else
	//		{
	//			g.DrawLine(dp-BLACKSTEP,rect.top,dp,rect.top);
	//			g.SetColor(LWHITECOLOR);
	//			g.DrawLine(rect.left,rect.top,dp-BLACKSTEP,rect.top);
	//		}
	//		//g.DrawLine(rect.left,rect.top,rect.left+dp,rect.top+);
	//	}
	//	while(dp<rect.right)
	//	{
	//		int ex=dp+LINESTEP-BLACKSTEP;
	//		if(ex>rect.right)
	//			ex=rect.right;
	//		g.SetColor(LWHITECOLOR);
	//		g.DrawLine(dp,rect.top,ex,rect.top);
	//		if(ex<rect.right)
	//		{
	//			int bx=ex;
	//			ex+=BLACKSTEP;
	//			if(ex>rect.right)
	//			ex=rect.right;
	//			g.SetColor(LBLACKCOLOR);
	//			g.DrawLine(bx,rect.top,ex,rect.top);
	//		}
	//		dp+=LINESTEP;
	//	}
	//	dp-=rect.right;
	//	dp+=rect.top;
	//	if(dp>rect.top)
	//	{
	//		g.SetColor(LBLACKCOLOR);
	//		if(dp<rect.top+BLACKSTEP)
	//			g.DrawLine(rect.right,rect.top,rect.right,dp);
	//		else
	//		{
	//			g.DrawLine(rect.right,dp-BLACKSTEP,rect.right,dp);
	//			g.SetColor(LWHITECOLOR);
	//			g.DrawLine(rect.right,rect.top,rect.right,dp-BLACKSTEP);
	//		}
	//		//g.DrawLine(rect.left,rect.top,rect.left+dp,rect.top+);
	//	}
	//	while(dp<rect.bottom)
	//	{
	//		int ey=dp+LINESTEP-BLACKSTEP;
	//		if(ey>rect.bottom)
	//			ey=rect.bottom;
	//		g.SetColor(LWHITECOLOR);
	//		g.DrawLine(rect.right,dp,rect.right,ey);
	//		if(ey<rect.bottom)
	//		{
	//			int by=ey;
	//			ey+=BLACKSTEP;
	//			if(ey>rect.bottom)
	//			ey=rect.bottom;
	//			g.SetColor(LBLACKCOLOR);
	//			g.DrawLine(rect.right,by,rect.right,ey);
	//		}
	//		dp+=LINESTEP;
	//	}
	//	dp-=rect.bottom;
	//	dp=rect.right-dp;
	//	if(dp<rect.right)
	//	{
	//		g.SetColor(LBLACKCOLOR);
	//		if(dp>rect.right-BLACKSTEP)
	//			g.DrawLine(dp,rect.bottom,rect.right,rect.bottom);
	//		else
	//		{
	//			g.DrawLine(dp,rect.bottom,dp+BLACKSTEP,rect.bottom);
	//			g.SetColor(LWHITECOLOR);
	//			g.DrawLine(dp+BLACKSTEP,rect.bottom,rect.right,rect.bottom);
	//		}
	//		//g.DrawLine(rect.left,rect.top,rect.left+dp,rect.top+);
	//	}
	//	while(dp>rect.left)
	//	{
	//		int bx=dp-LINESTEP+BLACKSTEP;
	//		if(bx<rect.left)
	//			bx=rect.left;
	//		g.SetColor(LWHITECOLOR);
	//		g.DrawLine(bx,rect.bottom,dp,rect.bottom);
	//		if(bx>rect.left)
	//		{
	//			int ex=bx;
	//			bx-=BLACKSTEP;
	//			if(bx<rect.left)
	//				bx=rect.left;
	//			g.SetColor(LBLACKCOLOR);
	//			g.DrawLine(bx,rect.bottom,ex,rect.bottom);
	//		}
	//		dp-=LINESTEP;
	//	}
	//	dp=rect.bottom+dp-rect.left;
	//	if(dp<rect.bottom)
	//	{
	//		g.SetColor(LBLACKCOLOR);
	//		if(dp>rect.bottom-BLACKSTEP)
	//			g.DrawLine(rect.left,dp,rect.left,rect.bottom);
	//		else
	//		{
	//			g.DrawLine(rect.left,dp,rect.left,dp+BLACKSTEP);
	//			g.SetColor(LWHITECOLOR);
	//			g.DrawLine(rect.left,dp+BLACKSTEP,rect.left,rect.bottom);
	//		}
	//		//g.DrawLine(rect.left,rect.top,rect.left+dp,rect.top+);
	//	}
	//	while(dp>rect.top)
	//	{
	//		int by=dp-LINESTEP+BLACKSTEP;
	//		if(by<rect.top)
	//			by=rect.top;
	//		g.SetColor(LWHITECOLOR);
	//		g.DrawLine(rect.left,by,rect.left,dp);
	//		if(by>rect.top)
	//		{
	//			int ey=by;
	//			by-=BLACKSTEP;
	//			if(by<rect.top)
	//				by=rect.top;
	//			g.SetColor(LBLACKCOLOR);
	//			g.DrawLine(rect.left,by,rect.left,ey);
	//		}
	//		dp-=LINESTEP;
	//	}

	//}

	//PaintButtons(g);

//#ifdef __ANDROID__
//#elif defined (__APPLE__)
//#else
//	if(m_nLockOptTime)
//	{
//		int w=m_optImage.Width();
//		int h=m_optImage.Height()/OPT_COUNT;
//		int x=m_rect.Width()-w-10;
//		int y=10;
//		int sx=0;
//		int sy=h*(OPT_COUNT-(m_nLockOptTime%OPT_COUNT)-1);//(OPT_COUNT-m_nLockOptTime+1);
//		XPoint dst(x,y);
//		XRect  rect(sx,sy,sx+w,sy+h);
//		g.DrawImage(m_optImage,dst,rect);
//	}
//#endif

	if(m_bPause)
	{
		g.SetBackColor(0x7f000000);
		g.FillRectangle(m_clip);
	}
//#ifdef __TOUCHPAD__
//	if(!m_nBarPos)
//	{
//		XRect rt(0,0,m_left.Width(),m_rect.bottom-m_left.Height());
//		rt&=m_clip;
//		if(!rt.IsRectEmpty())
//			g.DrawImage(m_left,XPoint(0,m_rect.bottom-m_left.Height()));
//	}
//#endif

	PaintCursor(g);
	PaintBlackBG( g );
	PaintFrameShow( g );
//Modify by xiepeifu
	/*
	if (  XDomView::mbLowNetSpeed )
	{
		PaintLowNetSpeedTip( g );
	}*/
	return XTRUE;

}

_XBUTTONMAP* XFlashView::CheckButtons(XPoint &pt,XRect&rect,int iPos,bool&bUpdate)
{
	if(!m_bConfigOK) return XNULL;
	XConfig*pConfig=m_player->GetConfig();
	if(!pConfig->m_bButtons) return XNULL;
	int i,nc=pConfig->m_buttonMaps.GetSize();
	if(nc<=0) return XNULL;
	_XBUTTONMAP*pMap=XNULL;
	for(i=0;i<nc;i++)
	{
		_XBUTTONMAP&map=pConfig->m_buttonMaps[i];
		XRect rd;
		if(!GetButtonPos(rd,i)) continue;
		if(!rd.PtInRect(pt)/*&&iPos */)
		{
			if(map.nShowTime&&map.nHitPos==iPos)
			{
				map.nShowTime=0;
				map.nHitPos=0;
				if(map.bLock!=KLK_RELEASE&&map.bLock!=KLK_LOCKED)
				{
					//m_player->OnKeyDown(map.nKeyMap);
					m_player->OnKeyUp(map.nKeyMap,map.nKeyMap,0);
					map.bLock=KLK_RELEASE;
					bUpdate = true;
					//InvalidateRect(rd);
					//PaintButtons();
					//m_player->UpdateAll();
				}
			}
			continue;
		}
		else if(pMap==XNULL)
		{
			rect=rd;
			pMap=&map;
			bUpdate=true;
		}
	}
	if(pMap)
		pMap->nHitPos=iPos;
	return pMap;
}

// zhengbin 2012.6.5 change DrawDIB for APPLE
//#if defined(__ANDROID__) || defined(__APPLE__)	// zhengjl 2011.10
//
//void XFlashView::PaintButtons(XGraphics &g)
//{
//	//enum {A,B,DOWN,LEFT,LEFT_KEY,RIGHT,RIGHT_KEY,UP};
//	if(!m_bConfigOK) return;
//	XConfig*pConfig=m_player->GetConfig();
//	if(pConfig->m_bButtons) //return;
//	{
//		int nc=pConfig->m_buttonMaps.GetSize();
//		if(nc<=0) return;
//		int i;
//
//		for(i=0;i<nc;i++)
//		{
//			//continue;
//			_XBUTTONMAP&map=pConfig->m_buttonMaps[i];
//			if(map.pDib==XNULL) continue;//||map.nButtonID>_XMOUSEMODE::UP) continue;
//
//			XRect rd;
//			if(!GetButtonPos(rd,i)) continue;
//
//			int w=map.pDib->Width(),
//				h=map.pDib->Height();
//			XPoint pt;
//#ifdef __APPLE__
//            if( !g_deviceData.mbOnSale && g_deviceData.m_scale > 1.1 )
//            {
//                pt.x=rd.left / 2;
//                pt.y=rd.top / 2;
//            }
//            else
//#endif
//            {
//                pt.x=rd.left;
//                pt.y=rd.top;
//            }
//
//			
//			rd&=m_clip;
//			if(rd.IsRectEmpty()) continue;
//			XRect rect(0,0,w,h);
//			if(map.pDib->m_nBits==32)
//			{
//				switch(map.bLock)
//				{
//				case KLK_PRESSED:
//					 DrawDib(g,*map.pDib,pt,rect,2,map.bLongPress?pConfig->m_pGrayDib:XNULL);
//					 break;
//				case KLK_LOCKED:
//					 DrawDib(g,*map.pDib,pt,rect,-2,map.bLongPress?pConfig->m_pGreenDib:XNULL);
//					 break;
//				default:
//					 DrawDib(g,*map.pDib,pt,rect,0,map.bLongPress?pConfig->m_pGrayDib:XNULL);
//					 break;
//				}
//
//			}
//			else
//				g.DrawDIB(*map.pDib,pt,rect,XTRUE);
//		}
//	}
//}
//
//void XFlashView::DrawDib(XGraphics &g,XDib&dib, XPoint &pt, XRect &rect, int alpha,XDib*pLight)
//{
//	int w=rect.Width(),h=rect.Height();
//	if(!pLight)
//	{
//		if(alpha<0) alpha=-alpha;
//		XU8*pTmp=new XU8[w*h];
//		if(pTmp)
//		{
//			XU8*pSrt=dib.GetBuffer()+3;
//			int is,iLen=w*h;
//			for(is=0;is<iLen;is++)
//			{
//				pTmp[is]=*pSrt;
//				*pSrt=*pSrt>>alpha;
//				pSrt+=4;
//			}
//			g.DrawDIB(dib,pt,rect,XTRUE);
//			pSrt=dib.GetBuffer()+3;
//			for(is=0;is<iLen;is++)
//			{
//				*pSrt=pTmp[is];
//				pSrt+=4;
//			}
//			delete pTmp;
//		}
//		else
//			g.DrawDIB(dib,pt,rect,XTRUE);
//	}
//	else
//	{
//		XDib tmp;
//		if(!tmp.Create(w,h,32))
//			g.DrawDIB(dib,pt,rect,XTRUE);
//		else
//		{
//			int ww=pLight->Width(),hh=pLight->Height();
//			XU8*pDst=tmp.GetBuffer();
//			XGlobal::Memcpy(pDst,dib.GetBuffer(),dib.m_nDataLength);
//			pDst+=(4+4*w)<<2;
//			XU8*pSrt=pLight->GetBuffer();
//			int x,y,iDst=w<<2,iSrt=ww<<2;
//
//			for(y=0;y<hh;y++)
//			{
//				for(x=0;x<iSrt;x+=4)
//				{
//				   int aa=pSrt[x+3];
//				   if(!aa) continue;
//				   aa++;
//				   int sa=256-aa;
//				   pDst[x]=((pDst[x]*sa)>>8)+((pSrt[x]*aa)>>8);
//				   pDst[x+1]=((pDst[x+1]*sa)>>8)+((pSrt[x+1]*aa)>>8);
//				   pDst[x+2]=((pDst[x+2]*sa)>>8)+((pSrt[x+2]*aa)>>8);
//
//				}
//				pDst+=iDst;
//				pSrt+=iSrt;
//			}
//			if(alpha<0)
//			{
//				alpha=-alpha;
//				pDst=tmp.GetBuffer()+3;
//				iDst=(w*h);
//				for(x=0;x<iDst;x++)
//				{
//					*pDst=*pDst-((*pDst)>>alpha);
//					pDst+=4;
//				}
//			}
//			else if(alpha)
//			{
//				pDst=tmp.GetBuffer()+3;
//				iDst=(w*h);
//				for(x=0;x<iDst;x++)
//				{
//					*pDst=(*pDst)>>alpha;
//					pDst+=4;
//				}
//			}
//			g.DrawDIB(tmp,pt,rect,XTRUE);
//		}
//	}
//}
//
//#else // android

void XFlashView::PaintButtons()
{
	//enum {A,B,DOWN,LEFT,LEFT_KEY,RIGHT,RIGHT_KEY,UP};
	if(!m_bConfigOK)
        return;
	XConfig*pConfig=m_player->GetConfig();
	m_player->ResetButtons();
	if(pConfig->m_bButtons) //return;
	{
		int nc=pConfig->m_buttonMaps.GetSize();
		if(nc<=0) return;
		int i;
		for(i=0;i<nc;i++)
		{
			//continue;
			_XBUTTONMAP&map=pConfig->m_buttonMaps[i];
			if(map.pDib==XNULL) continue;//||map.nButtonID>_XMOUSEMODE::UP) continue;

			XRect rd;
			if(!GetButtonPos(rd,i)) continue;

			int w=map.pDib->Width(),
				h=map.pDib->Height();
			XPoint pt;
			pt.x=rd.left;
			pt.y=rd.top;
			//rd&=m_clip;
			if(rd.IsRectEmpty()) continue;
			XRect rect(0,0,w,h);
			if(map.pDib->m_nBits==32)
			{
				switch(map.bLock)
				{
				case KLK_PRESSED:
					 if(map.bLongPress)
						DrawDib(*map.pDib,pt,rect,1,map.bLongPress?pConfig->m_pGrayDib:XNULL);
					 else
						 DrawDib(*map.pDib,pt,rect,1,XNULL);
						//g.DrawDIB(*map.pDib,pt,rect);
					 break;
				case KLK_LOCKED:
					 DrawDib(*map.pDib,pt,rect,0.4,map.bLongPress?pConfig->m_pGreenDib:XNULL);
					 break;
				default:
					 DrawDib(*map.pDib,pt,rect,0.6,map.bLongPress?pConfig->m_pGrayDib:XNULL);
					 break;
				}
			}
			else
			{
				DrawDib(*map.pDib,pt,rect,1,XNULL);
				//g.DrawDIB(*map.pDib,pt,rect);
			}
		}
	}
	m_player->Repaint();
	return;
}

void XFlashView::DrawDib(XDib&dib, XPoint &pt, XRect &rect,float alpha,XDib*pLight)
{
	int w=rect.Width(),h=rect.Height();
	m_player->AddPaintButton(&dib,pt,rect,alpha,pLight);
	/*if(!pLight)
	{
		if(alpha<0) alpha=-alpha;
		XU8*pTmp=new XU8[w*h];
		if(pTmp)
		{
			XU8*pSrt=dib.GetBuffer()+3;
			int is,iLen=w*h;
			for(is=0;is<iLen;is++)
			{
				pTmp[is]=*pSrt;
				*pSrt=*pSrt>>alpha;
				pSrt+=4;
			}
			g.DrawDIB(dib,pt,rect);
			pSrt=dib.GetBuffer()+3;
			for(is=0;is<iLen;is++)
			{
				*pSrt=pTmp[is];
				pSrt+=4;
			}
			delete pTmp;
		}
		else
			g.DrawDIB(dib,pt,rect);
	}
	else
	{
		XDib tmp;
		if(!tmp.Create(w,h,32))
			g.DrawDIB(dib,pt,rect);
		else
		{
			int ww=pLight->Width(),hh=pLight->Height();
			XU8*pDst=tmp.GetBuffer();
			XGlobal::Memcpy(pDst,dib.GetBuffer(),dib.m_nDataLength);
			pDst+=(4+4*w)<<2;
			XU8*pSrt=pLight->GetBuffer();
			int x,y,iDst=w<<2,iSrt=ww<<2;

			for(y=0;y<hh;y++)
			{
				for(x=0;x<iSrt;x+=4)
				{
				   int aa=pSrt[x+3];
				   if(!aa) continue;
				   aa++;
				   int sa=256-aa;
				   pDst[x]=((pDst[x]*sa)>>8)+((pSrt[x]*aa)>>8);
				   pDst[x+1]=((pDst[x+1]*sa)>>8)+((pSrt[x+1]*aa)>>8);
				   pDst[x+2]=((pDst[x+2]*sa)>>8)+((pSrt[x+2]*aa)>>8);

				}
				pDst+=iDst;
				pSrt+=iSrt;
			}
			if(alpha<0)
			{
				alpha=-alpha;
				pDst=tmp.GetBuffer()+3;
				iDst=(w*h);
				for(x=0;x<iDst;x++)
				{
					*pDst=*pDst-((*pDst)>>alpha);
					pDst+=4;
				}
			}
			else if(alpha)
			{
				pDst=tmp.GetBuffer()+3;
				iDst=(w*h);
				for(x=0;x<iDst;x++)
				{
					*pDst=(*pDst)>>alpha;
					pDst+=4;
				}
					//pDst[x]=
			}
			g.DrawDIB(tmp,pt,rect);
		}
	}*/
}
//#endif // android

XBOOL XFlashView::OnKeyDown(XKEYMSG&m)
{
	XWCHAR c=m.nKeyCode;
	//if(m_pTxtView&&m_pTxtView->IsInstance())
	//{
	//	return m_pTxtView->OnKeyDown(m);
	//	//return XFALSE;
	//}
	if(m_bConfigOK)
	{
		XConfig*pConfig=m_player->GetConfig();
		int i,nc=pConfig->m_keyMaps.GetSize();
		XBOOL bSet=XFALSE;
		for(i=0;i<nc;i++)
		{
			_XKEYMAP&key=pConfig->m_keyMaps[i];
			if(key.nKeyCode!=c) continue;

			if(key.bLongPress!=KLK_LOCKED)
			{
				if(!MouseAct(key.nKeyMap,XTRUE))
					m_player->OnKeyDown(key.nKeyMap,key.nKeyMap,0);
			}
			key.bLock=KLK_PRESSED;
			key.nPressTime=XTime::GetTimeMSec();

			bSet=XTRUE;
		}
		if(!bSet)
		{
			if(!MouseAct(c,XTRUE))
			{
				if(!m_bPause)
					m_player->OnKeyDown(m.nKeyCode,m.nCharCode,m.nLocation);
			}
		}
	}
	return XTRUE;
}

XBOOL XFlashView::OnKeyUp(XKEYMSG&m)
{
	XWCHAR c=m.nKeyCode;
	if(c==XK_RIGHTSOFT)
	{
		return XTRUE;
	}
	//if(m_pTxtView&&m_pTxtView->IsInstance())
	//{
	//	return m_pTxtView->OnKeyUp(m);
	//}
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	if(c==XK_LEFTSOFT)
//	{
//		if(m_nBarPos)
//			m_nBarMode=XBAR_OUT;
//		else
//			m_nBarMode=XBAR_IN;
//	}
//#endif

	//if(m_pTxtView&&m_pTxtView->IsInstance()) 
	//	return XFALSE;
	if(m_bConfigOK)
	{
		XConfig*pConfig=m_player->GetConfig();
		int i,nc=pConfig->m_keyMaps.GetSize();
		XBOOL bSet=XFALSE;
		for(i=0;i<nc;i++)
		{
			_XKEYMAP&key=pConfig->m_keyMaps[i];
			if(key.nKeyCode!=c) continue;

			if(key.bLongPress!=KLK_LOCKED)
			{
				if(!MouseAct(key.nKeyMap,XFALSE))
					m_player->OnKeyUp(key.nKeyMap,key.nKeyMap,0);
				key.bLock=KLK_RELEASE;
			}
			bSet=XTRUE;
		}
		if(!bSet)
		{
			if(!MouseAct(c,XFALSE))
			{
				if(!m_bPause)
					m_player->OnKeyUp(m.nKeyCode,m.nCharCode,m.nLocation);
			}
		}
	}
	return XTRUE;
}

void XFlashView::SetDragRect(SRECT&r,XPOINT&p1,XPOINT&p2)
{
	r.xmin = p1.x;
	r.ymin = p1.y;
	r.xmax = p2.x;
	r.ymax = p2.y;
	int t;
	if(r.xmin>r.xmax)
	{
		t = r.xmin;
		r.xmin = r.xmax;
		r.xmax = t;
	}
	if(r.ymin>r.ymax)
	{
		t = r.ymin;
		r.ymin = r.ymax;
		r.ymax = t;
	}
}
void XFlashView::StartDrag()
{
	SetDragRect(m_dragRect,m_downPt1,m_downPt2);
	
	AddCommand(XSTARTDRAG,0,0);
}

void XFlashView::OnMouseDown(XPoint &pt,int iPos)
{
	//LOGE("%s, %s, %d, x=%d, y=%d, pos=%d", __FILE__, __FUNCTION__, __LINE__, pt.x, pt.y, iPos );
//	if(m_bPause) {Pause();return;}
//	if(m_bLockPoint) return;
	m_nACCDelay=ACCDELAY;
	if(iPos>=2) return;
//	if(m_pTxtView&&m_pTxtView->IsInstance())
//		return;
	if(pt.y>m_rect.bottom-m_nBarPos)
		return;
	if(m_nTwoPoint)
	{
		//m_player->UnlockZoomBitmap(true);
		//StopDragOrZoom(true);
		//m_bTwoPoint=false;
		if(m_nDragSet==DRAG_MOVE)
		{
			switch(m_nTwoPoint)
			{
			case 2://Point 1 has released
				if(iPos==1)
				{
					m_downPt2.x=pt.x;
					m_downPt2.y=pt.y;
					m_nowPt2=m_downPt2;
					m_nTwoPoint=1;
				}
				break;
			case 3://Point 0 has released
				if(iPos==0)
				{
					m_downPt1.x=pt.x;
					m_downPt1.y=pt.y;
					m_nowPt1=m_downPt1;
					m_nTwoPoint=1;
				}break;
			}
		}
		return;
	}
	//m_bTwoPoint=false;
	if(iPos==0)
	{
		m_bWait2=false;
		m_downPt1.x=pt.x;
		m_downPt1.y=pt.y;
		m_nowPt1=m_downPt1;
		m_nClickDown=0;//this->m_player->m_nLoopCount;
		m_nMouseX=pt.x;
		m_nMouseY=pt.y;
		m_bMouseDown=XTRUE;
		m_bMapDown=XFALSE;
		if(CheckMenu(pt)) return;
	}
	else //if(m_bWait2)
	{
		if(m_nTwoPoint)
		{
			return;
		}
		else if(m_bWait2)
		{
			m_bWait2=false;
			m_nTwoPoint=1;
			m_nDragSet=DRAG_START;
			m_downPt2.x=pt.x;
			m_downPt2.y=pt.y;
			m_nowPt2=m_downPt2;
			m_bHor=IsHor(m_downPt1,m_downPt2);
			m_nLenDown=Length(m_downPt1,m_downPt2,m_bHor);
			m_nPecent0=m_nPecent;
			m_nPecentBase=m_player->GetPecentBase();
			m_nLenPrev=m_nLenDown;
			m_centerPrev.x=(m_nowPt2.x+m_nowPt1.x)/2;
			m_centerPrev.y=(m_nowPt2.y+m_nowPt1.y)/2;
			//m_player->LockZoomBitmap();
			StartDrag();
			//StartDragOrZoom();
			return;
		}
	}
	XRect rect;
	bool bUpdate=false;
	_XBUTTONMAP*map=CheckButtons(pt,rect,iPos,bUpdate);
	
	if(map)
	{
		if(map->bLock!=KLK_LOCKED)
			m_player->OnKeyDown(map->nKeyMap,map->nKeyMap,0);
		map->bLock=KLK_PRESSED;
		map->nShowTime=MAXBARTIME;
		map->nPressTime=XTime::GetTimeMSec();
		//InvalidateRect(rect);
		m_bMapDown=XTRUE;
		//if(bUpdate)
		//PaintButtons();
		AddCommand(XUPDATEBUTTONS,0,0);
		return;
	}
	else if(bUpdate)
		//PaintButtons();
		AddCommand(XUPDATEBUTTONS,0,0);
	if(iPos)
	{
		if(m_bWait2)
		{
			m_nTwoPoint=1;
			m_nDragSet=DRAG_START;
			m_downPt2.x=pt.x;
			m_downPt2.y=pt.y;
			//m_player->LockZoomBitmap();
			//StartDragOrZoom();
			StartDrag();
		}
		return;
	}

	//if( IsEnableZoom() )
	//{
	//	m_bWait2=true;
	//}
	//else
	//{
		m_bWait2=true;
		/*if(!m_bPause)
		{
			m_player->OnMouseDown(pt.x, pt.y,iPos);
		}*/
	//}
	m_nClickDown=m_player->m_nLoopCount;
	m_nDownTime=XTime::GetTimeMSec();
	m_bFlashDown=true;
	m_nFlashX=pt.x;
	m_nFlashY=pt.y;
}

void XFlashView::EndDrag()
{
	AddCommand(XENDDRAG,0,0);
}

void XFlashView::OnMouseUp(XPoint &pt,int iPos)
{
	//LOGE("%s, %s, %d, x=%d, y=%d, pos=%d", __FILE__, __FUNCTION__, __LINE__, pt.x, pt.y, iPos );
//	if(m_bLockPoint)
//	{
//		UPosXY data;
//		XRect rect;
//		GetPointZoomRect(rect);
//		int w=rect.Width();
//		int h=rect.Height();
//		int x0=pt.x;
//		int y0=pt.y;
//		x0-=m_winSize.cx/2;
//		y0-=m_winSize.cy/2;
//		x0=x0*(m_winSize.cx+w)/m_winSize.cx;
//		y0=y0*(m_winSize.cy+h)/m_winSize.cy;
//		x0+=m_winSize.cx/2;
//		y0+=m_winSize.cy/2;
//		data.xy[0]=x0;
//		data.xy[1]=y0;
//			//(((short)center.x<<16)|(center.y);
//#ifdef __APPLE__
//		PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_ZOOMOUTENABLE, (XU32)this, XMS_TOPMOST);
//#endif
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////		m_bar.CheckItem(XCMD_FLASH_POINTZOOM,XFALSE);
////#endif
//		AddCommand(XZOOMINOUTC,100,data.iData);
//		m_bLockPoint=false;
//		return;
//	}
	m_nACCDelay=ACCDELAY;
	if(iPos>=2) return;
	if(m_nTwoPoint)
	{
		switch(m_nDragSet)
		{
		case DRAG_START:
		case DRAG_ZOOM:
			// StopDragOrZoom(true);
			 EndDrag();
			 m_nTwoPoint=0;
			 return;
		default:
			 switch(m_nTwoPoint)
			 {
			 case 1:
				 if(iPos==0)
				 {
					 m_nTwoPoint=3;
					 m_centerPrev=m_nowPt2;
				 }
				 else
				 {
					 m_nTwoPoint=2;
					 m_centerPrev=m_nowPt1;
				 }
				 break;
			 case 3:
			 case 2:
				 //StopDragOrZoom(true);
				 EndDrag();
				 m_nTwoPoint=0;
				 break;
			 }break;
		}return;
	}
	if(iPos==0)
	{
		if(m_bWait2)
		{
			m_bWait2=false;
			m_player->OnMouseDown(m_downPt1.x,
								  m_downPt1.y,
								  0);
		}
		if(m_nTwoPoint)
		{
			//m_player->UnlockZoomBitmap(true);
			//StopDragOrZoom(true);
			EndDrag();
			m_nTwoPoint=false;
			return;
		}
	}
	m_bWait2=false;
	if(m_nTwoPoint) return;
	bool bFlashDown=m_bFlashDown;
	m_bFlashDown=false;
	//if(m_pTxtView&&m_pTxtView->IsInstance())
	//	return;
	m_bMapDown=XFALSE;
	m_bMouseDown=XFALSE;
	if(m_bPause)
	{
		Pause();
		return;
	}
	if(pt.y>m_rect.bottom-m_nBarPos)
		return;
	m_nMouseX=pt.x;
	m_nMouseY=pt.y;
	//XU8 bUse=XFALSE;
//#ifdef __TOUCHPAD__
//	//if(m_nBarPos==0&)
//	if(CheckMenu(pt))
//	{
//		m_nBarMode=XBAR_IN;
//		return;
//	}
//	/*{
//	   XRect left(0,m_rect.bottom-m_nBarPos,m_left.Width(),m_rect.bottom);
//	   if(left.PtInRect(pt))
//	   {
//		   PostMsg(XWM_KEYDOWN,XK_LEFTSOFT,(XU32)this,XMS_NORMAL);
//		   PostMsg(XWM_KEYUP,XK_LEFTSOFT,(XU32)this,XMS_NORMAL);
//		   return;
//	   }
//
//	}*/
//#endif
	//if(m_nBarMode!=XBAR_OUT)
	//	m_nBarMode=XBAR_OUT;

	XRect rect;

	/*if(GetOnOffRect(rect))
	{
		if(rect.PtInRect(pt))
		{
			XConfig*pConfig=m_player->GetConfig();
			pConfig->m_bButtons=!pConfig->m_bButtons;
			Invalidate();
		}
	}*/
	bool bUpdate=false;
	_XBUTTONMAP*map=CheckButtons(pt,rect,iPos,bUpdate);
	
	if(map)
	{
		if(map->bLock!=KLK_LOCKED)
		{
			XKEYMSG msg={map->nKeyMap,map->nKeyMap,0,false,false,false};
			OnKeyUp(msg);
			map->bLock=KLK_RELEASE;
		}
		map->nShowTime=0;//MAXBARTIME;
		map->nHitPos=0;
		//InvalidateRect(rect);
		//PaintButtons();
		AddCommand(XUPDATEBUTTONS,0,0);
		return;
	}
	if(bUpdate)
		AddCommand(XUPDATEBUTTONS,0,0);
		//PaintButtons();
	if(iPos) return;
	if(!m_bPause)
	{
		if(bFlashDown)
		{
            XU32 t=XTime::GetTimeMSec();
			if(t-m_nDownTime<5000)
			{
				m_player->OnMouseUp(pt.x, pt.y,iPos|0x80000000);
				m_player->OnClick(m_nFlashX,m_nFlashY,iPos,m_nClickDown);
			}
			else
				m_player->OnMouseUp(pt.x,pt.y,iPos);
		}
		else
			m_player->OnMouseUp(pt.x,pt.y,iPos);

	}

}

void XFlashView::OnClick(XPoint&pt,int iPos)
{
	if(m_nTwoPoint)
	{
		if(m_nDragSet==DRAG_MOVE) return;
		//StopDragOrZoom(true);
		EndDrag();
		//m_player->UnlockZoomBitmap(true);
	}
	m_bWait2=false;
	m_nTwoPoint=0;
	//if(m_pTxtView&&m_pTxtView->IsInstance())
	//	return;
	if(m_bPause) {return;}
	if(pt.y>m_rect.bottom-m_nBarPos) return;

}

void XFlashView::OnDblClick(XPoint&pt,int iPos)
{
	if(m_nTwoPoint)
	{
		//StopDragOrZoom(true);
		EndDrag();
		//m_player->UnlockZoomBitmap(true);
	}
	m_bWait2=false;
	m_nTwoPoint=0;
	//if(m_pTxtView&&m_pTxtView->IsInstance()) return;
	if(m_bPause) {return;}
	if(pt.y>m_rect.bottom-m_nBarPos) return;
	m_player->OnDoubleClick(pt.x,pt.y,iPos);
}

int XFlashView::Length(SPOINT&pt1,SPOINT&pt2,bool bHor)
{
	int dx=(pt2.x-pt1.x);
	int dy=(pt2.y-pt1.y);
	if(dx<0) dx=-dx;
	if(dy<0) dy=-dy;
	return bHor?dx:dy;
}

//void  XFlashView::GetPointZoomRect(XRect&rect)
//{
//	//int wx=t
//	int nStageW=m_player->GetStageWidth();
//	int nStageH=m_player->GetStageHeight();
//
//
//	//int w=RectWidth(&player->frame)/20;
//	//	int h=RectHeight(&player->frame)/20;
//
//	int nDevW=m_winSize.cx;
//	int nDevH=m_winSize.cy;
//
//	SFIXED fx=FixedDivS(nDevW,nStageW);
//	SFIXED fy=FixedDivS(nDevH,nStageH);
//	SFIXED f;
//	/*if(m_player->m_nMaxPecent<=100)
//	{
//		f=XMIN(XMIN(fx,fy),fixed_1);
//		f=(f*100+(fixed_1-f)*m_player->m_nMaxPecent)/fixed_1;
//	}
//	else*/
//	{
//		f=m_player->m_nMaxPecent;
//	}
//
//	int w=(nDevW*nDevW*100/nStageW/2)/f;//m_nMaxPecent;
//	int h=(nDevH*nDevH*100/nStageH/2)/f;//m_nMaxPecent;
//	if(w>nDevW/2) w=nDevW/2;
//	if(h>nDevH/2) h=nDevH/2;
//	rect.left=m_nowPt1.x-w;
//	rect.top=m_nowPt1.y-h;
//	rect.right=m_nowPt1.x+w;
//	rect.bottom=m_nowPt1.y+h;
//	if(rect.left< 0 )
//	{
//		rect.left=0;
//		rect.right=rect.left+w*2;
//	}
//	if(rect.top<0)
//	{
//		rect.top = 0;
//		rect.bottom=rect.top+h*2;
//	}
//	if(rect.right>nDevW)
//	{
//		rect.right=nDevW;
//		rect.left=rect.right-w*2;
//	}
//	if(rect.bottom>nDevH)
//	{
//		rect.bottom=nDevH;
//		rect.top=rect.bottom-h*2;
//	}
//}

void XFlashView::Draging(bool moving)
{
	SRECT r;
	SetDragRect(r,m_nowPt1,m_nowPt2);
	if(moving)
	{
		int cx=(r.xmin+r.xmax)/2;
		int cy=(r.ymin+r.ymax)/2;
		int w=(m_dragRect.xmax-m_dragRect.xmin)/2;
		int h=(m_dragRect.ymax-m_dragRect.ymin)/2;
		r.xmin=cx-w;
		r.xmax=cx+w;
		r.ymin=cy-h;
		r.ymax=cy+h;
		/*int dx=(r.xmin+r.xmin)/2-(m_dragRect.xmin+m_dragRect.xmax)/2;
		int dy=(r.ymin+r.ymin)/2-(m_dragRect.ymin+m_dragRect.ymax)/2;
		r.xmin-=dx;
		r.ymin-=dy;
		r.xmax-=dx;
		r.ymax-=dy;*/
    }
//    LOGE("r.xmin----->%d",r.xmin);
//    LOGE("r.ymin----->%d",r.ymin);
//    LOGE("r.xmax----->%d",r.xmax);
//    LOGE("r.ymax----->%d",r.ymax);
    
	m_dragRect = r;
	AddCommand(XDRAGING,0,0);
}

void XFlashView::OnMouseMove(XPoint &pt,int iPos)
{
	//LOGE("%s, %s, %d, x=%d, y=%d, pos=%d", __FILE__, __FUNCTION__, __LINE__, pt.x, pt.y, iPos );
	//if(m_bLockPoint)
	//{
	//	XRect r1,rect;
	//	GetPointZoomRect(r1);
	//	m_nowPt1.x=pt.x;
	//	m_nowPt1.y=pt.y;
	//	GetPointZoomRect(rect);
	//	rect|=r1;
	//	this->InvalidateRect(rect);
	//	return;
	//}
	m_nACCDelay=ACCDELAY;
	if(iPos>=2) return;
	if(iPos==0)
	{
		m_nowPt1.x=pt.x;
		m_nowPt1.y=pt.y;
		if(m_bWait2)
		{
			int dx=pt.x-m_downPt1.x;
			int dy=pt.y-m_downPt1.y;
			if(dx<0) dx=-dx;
			if(dy<0) dy=-dy;
			if(dx+dy>10)
			{
				m_bWait2=false;
				m_player->OnMouseDown(m_downPt1.x,
									  m_downPt1.y,0);
			}
		}

	}
	else
	{
		m_nowPt2.x=pt.x;
		m_nowPt2.y=pt.y;
		if(m_bWait2)
		{
			m_bWait2=false;
			m_nTwoPoint=1;
			m_nDragSet=DRAG_START;
			m_downPt2.x=pt.x;
			m_downPt2.y=pt.y;
			m_bHor=IsHor(m_downPt1,m_downPt2);
			m_nLenDown=Length(m_downPt1,m_downPt2,m_bHor);
			m_nPecent0=m_nPecent;
			m_nPecentBase=m_player->GetPecentBase();
			m_nLenPrev=m_nLenDown;
			m_centerPrev.x=(m_nowPt2.x+m_nowPt1.x)/2;
			m_centerPrev.y=(m_nowPt2.y+m_nowPt1.y)/2;
			//StartDragOrZoom();
			StartDrag();
			return;
		}
		//else if(!m_bTwoPoint) return;
	}
	//if(m_pTxtView&&m_pTxtView->IsInstance()) return;
	if(pt.y>m_rect.bottom-m_nBarPos) return;

	if(m_nTwoPoint)
	{
		int nowLen=Length(m_nowPt1,m_nowPt2,m_bHor);
		SPOINT center;//={(m_nowPt1.x+m_nowPt2.x)/2,
					  // (m_nowPt1.y+m_nowPt2.y)/2};
		switch(m_nTwoPoint)
		{
		case 1://Two Point
			center.x=(m_nowPt1.x+m_nowPt2.x)/2;
			center.y=(m_nowPt1.y+m_nowPt2.y)/2;
			break;
		case 2://Point 1 has released
			center.x=m_nowPt1.x;
			center.y=m_nowPt1.y;
			break;
		case 3://Point 0 has released
			center.x=m_nowPt2.x;
			center.y=m_nowPt2.y;
			break;
		}
		//LOGE("%s, %d, %d, %d ", __FILE__, __LINE__, center.x, center.y );
		if(nowLen==m_nLenPrev&&
		   center.x==m_centerPrev.x&&
		   center.y==m_centerPrev.y)
		   return;
		int dl=(nowLen-m_nLenPrev);
		if(dl<0) dl=-dl;
		if(m_nDragSet==DRAG_START)
		{

            if(dl>=TWOSTARTSTEP){
				//m_bZoomSet=true;
				m_nDragSet=DRAG_ZOOM;
             LOGWHERE();
            }
			else
			{
				int dx=pt.x-(iPos==0?m_downPt1.x:m_downPt2.x);
				int dy=pt.y-(iPos==0?m_downPt1.y:m_downPt2.y);
				if(dx<0) dx=-dx;
				if(dy<0) dy=-dy;
				if(XMAX(dx,dy)>TWOSTARTSTEP)
					m_nDragSet=DRAG_MOVE;
			}

		}
		if(m_nDragSet==DRAG_ZOOM)
		{

			if(dl>10&&iPos&&m_nLenDown)
			{
#ifndef __ANDROID__
                Draging(false);
#endif
			}
		}
		if(m_nDragSet==DRAG_MOVE)
		{
			/*SPOINT move={center.x-m_centerPrev.x,center.y-m_centerPrev.y};
			if(move.x||move.y)
			{
				m_centerPrev=center;
				AddCommand(XMOVECNT,move.x,move.y,XTRUE);
			}*/
#ifdef __ANDROID__
			Draging(true);
#else
            Draging(false);
#endif
		}
		return;
	}

	if(!m_rect.PtInRect(pt))
		return;

//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	m_nMouseX=pt.x;
//	m_nMouseY=pt.y;
//#else
//	if(m_bConfigOK)
//	{
//		_XMOUSEMODE*pMode=m_player->GetConfig()->m_pActiveMode;
//		if(pMode&&pMode->nMode==_XMOUSEMODE::XNORMAL)
//		{
//			XRect ri,rn;
//			GetMouseRect(ri);
//			m_nMouseX=pt.x;
//			m_nMouseY=pt.y;
//			GetMouseRect(rn);
//			ri|=rn;
//			InvalidateRect(ri);
//		}
//	}
//#endif
	///if(CheckMenu(pt)) return;
	if(CheckMenu(pt))
		return;

	//if(CheckButtons(pt)) return;
	XRect rect;
	bool bUpdate=false;
	_XBUTTONMAP*map=CheckButtons(pt,rect,iPos,bUpdate);
	if(bUpdate)
		AddCommand(XUPDATEBUTTONS,0,0);
		//PaintButtons();
	if(map)//||m_bMapDown||iPos)
	{
		//map->nShowTime=MAXBARTIME;
		//PaintButtons();
		//InvalidateRect(rect);
		return;
	}
	
//	if(m_bMapDown)

	if(!m_bPause)
	{
		//LOGE("%s, %s, %d, x=%d, y=%d, pos=%d", __FILE__, __FUNCTION__, __LINE__, pt.x-m_flashBase.x, pt.y-m_flashBase.y, iPos );
		m_player->OnMouseMove( pt.x,pt.y,iPos,m_bMouseDown );
	}
	if(m_bFlashDown)
	{
		int dx=pt.x-m_nFlashX;
		int dy=pt.y-m_nFlashY;
		if(dx<0) dx=-dx;
		if(dy<0) dy=-dy;
		if(dx>20||dy>20)
			m_bFlashDown=false;
	}

	switch(m_player->GetCursorType())
	{

	case CURSOR_TYPE_HAND:
		 SetCursor(XCUR_HAND);
		 break;
	default:
	case CURSOR_TYPE_ARROW:
		 SetCursor(XCUR_ARROW);
		 break;
	case CURSOR_TYPE_BUTTON:
		 SetCursor(XCUR_ANCHOR);
		 break;
	case CURSOR_TYPE_BEAM:
		 SetCursor(XCUR_INPUT);
		 break;
	case CURSOR_TYPE_HIDE:
		 SetCursor(XCUR_HIDE);
		 break;
	}

//	m_player->PointMove(pt.x-m_flashBase.x,pt.y-m_flashBase.y,m_bMouseDown);
}

void XFlashView::OnTimer()
{
	if(m_nACCDelay&&!m_bMouseDown)
		m_nACCDelay--;
//#ifdef __ANDROID__
//#elif defined (__APPLE__)
//#else
//	if(m_nLockOptTime)
//	{
//		m_nLockOptTime--;
//		int w=m_optImage.Width();
//		int h=m_optImage.Height()/OPT_COUNT;
//		XRect rect(m_rect.Width()-w-10,
//				   10,
//				   m_rect.Width()-10,
//				   10+h);
//		InvalidateRect(rect);
//	}
//#endif
	//if(m_bLockPoint)
	//{
	//	//if(m_nLockPtTime) m_nLockPtTime--;
	//	//else
	//	{
	//		m_nLockPtTime=1;
	//		XRect rect;
	//		if(m_nLineOffset<LINESTEP)
	//			m_nLineOffset++;
	//		else
	//			m_nLineOffset=0;
	//		GetPointZoomRect(rect);
	//		InvalidateRect(rect);
	//	}
	//}
	if(m_bWait2)
	{

		XU32 t=XTime::GetTimeMSec();
		if(t-this->m_nDownTime>TWOPTSTEP)
		{
			m_bWait2=false;
			m_player->OnMouseDown(m_downPt1.x,
								  m_downPt1.y,
								  0);
		}

	}
	if(m_nStartTime)
		m_nStartTime--;
	if(m_nTimeCount<50)
	{
		m_nTimeCount++;
	}
	else
	{
		m_nTimeCount=0;
		this->m_player->InterruptIfTimeout();
		PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_FLASHBAR,(XU32)this, XMS_TOPMOST);
	}
	LockKeyButtons();
	if(m_bConfigOK)
	{
		XConfig*pConfig=m_player->GetConfig();
		if(pConfig->m_bButtons) //return XNULL;
		{
			int i,nc=pConfig->m_buttonMaps.GetSize();
			bool bUpdate=false;
			for(i=0;i<nc;i++)
			{
				_XBUTTONMAP&map=pConfig->m_buttonMaps[i];
				/*if(map.bLock==KLK_RELEASE)
				{
					if(map.nShowTime)
					{
						map.nShowTime--;
					}
					continue;
				} */
				//if(map.bLongPress||map.bLock!=KLK_PRESSED) continue;
				if(map.bLock==KLK_RELEASE) continue;
				if(map.nShowTime>MAXBARTIME-4)
					map.nShowTime--;
				else
				{
					map.nShowTime=MAXBARTIME-3;
					m_player->OnKeyDown(map.nKeyMap,map.nKeyMap,0);
					bUpdate=true;
				}
			}
			if(bUpdate)
				AddCommand(XUPDATEBUTTONS,0,0);
				//PaintButtons();
		}
	}

	//if(m_nConnecting&&m_pTxtView)
	//{
	//	if(!m_pTxtView->IsEmpty())
	//	{
	//		m_nConnecting=0;
	//		m_pTxtView->Invalidate();
	//	}
	//	else
	//	{
	//		m_nConnecting++;
	//		//if(m_pWaitImg)
	//		//{
	//		//	int w=m_pWaitImg->Width()>>1;
	//		//	int h=m_pWaitImg->Height()>>1;
	//		//	int cx=m_rect.Width()>>1,cy=m_rect.Height()>>1;
	//		//	m_pTxtView->InvalidateRect(XRect(cx-w-1,cy-h-1,cx+w+1,cy+h+1));
	//		//}
	//	}
	//}
	//if(m_nConnecting

//	return XNULL;

//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	switch(m_nBarMode)
//	{
//	case XBAR_OUT:
//		{
//		 //if(m_nBarPos>5) m_nBarPos-=5;
//		 //else
//		 {
//			m_nBarMode=XBAR_HIDE;
//			m_nBarPos=0;
//		 }
//		 XRect rt=m_bar.GetWindowRect();
//		 int dh=rt.Height();
//		 rt.top=m_rect.bottom-m_nBarPos;
//		 rt.bottom=rt.top+dh;
//		 m_bar.MoveWindow(rt);
//		 AddCommand(XMOVE,0,0);
////		 InvalidateRect(XRect(0,m_rect.bottom-m_nBarPos-2,m_rect.right,m_rect.bottom));
////		 if(m_ctPoint.y<m_rect.bottom)
////			 m_ctPoint.y+=2;
////		 else
////			 m_nBarMode=XBAR_HIDE;
////		 InvalidateRect(XRect(0,m_ctPoint.y-2,m_rect.right,m_rect.bottom));
//		}
//		 break;
//	case XBAR_IN:
//		{
//		 XRect rt=m_bar.GetWindowRect();
//		 int dh=rt.Height();
//		 //if(m_nBarPos<dh)
//		 //	 m_nBarPos+=5;
//		 //else
//		 {
//			 m_nBarMode=XBAR_SHOW;
//			 m_nBarPos=dh;
//		 }
//		 rt.top=m_rect.bottom-m_nBarPos;
//		 rt.bottom=rt.top+dh;
//		 rt.left = 0;
//		 rt.right = m_rect.Width();
//		 m_bar.MoveWindow(rt);
//		 AddCommand(XMOVE,0,0);
//		 //InvalidateRect(XRect(0,m_rect.bottom-m_nBarPos,m_rect.right,m_rect.bottom));
//		 break;
//		}
//	}
//#endif
}

bool XFlashView::CanSendScore()
{
	if(!m_bConfigOK) return false;

	XConfig*pConfig=m_player->GetConfig();
	XXVar var;
	XBOOL bOK=GetScore(var);//m_player->GetVariable(cnt,pConfig->m_strScoreVar,var);
	//m_player->ReleaseContext(cnt);
	//lock.UnLock();

	if(bOK&&var.IsObject()&&var.pObject)
	{
		SObject*pObj=var.pObject->m_pObject;
		if(pObj&&pObj->character&&pObj->character->type==editTextChar)
		{
			//var.Release();
			pObj->editText->GetBuffer(var);
			//var.SetString(txt,XString8::SafeStrlen(txt));
			//var.nType=XCT_STRING;
			//var.strData=txt;
		}

	}
	if(!bOK||var.ToFloat()<=0)
		return false;
	
	return true;

}

//void XFlashView::BeginScore(XBOOL bAuto)//XBOOL bAuto)
//{
//	if(!m_bConfigOK||m_pTxtView)
//		return;
//	XConfig*pConfig=m_player->GetConfig();
//	XXVar var;
//	//XSWFCONTEXT cnt;
//	//XLock lock(&m_runLock);
//	//if(!lock.Lock(XSHORTTIME)) return;
//	//m_player->InitContext(cnt);
//	XBOOL bOK=GetScore(var);//m_player->GetVariable(cnt,pConfig->m_strScoreVar,var);
//	//m_player->ReleaseContext(cnt);
//	//lock.UnLock();
//
//	if(bOK&&var.IsObject()&&var.pObject)
//	{
//		SObject*pObj=var.pObject->m_pObject;
//		if(pObj&&pObj->character&&pObj->character->type==editTextChar)
//		{
//			//var.Release();
//			pObj->editText->GetBuffer(var);
//			//var.SetString(txt,XString8::SafeStrlen(txt));
//			//var.nType=XCT_STRING;
//			//var.strData=txt;
//		}
//
//	}
//	if(!bOK||var.ToFloat()<=0)
//		return;
//	XString16 txt=XResource::LoadWString(XIDS_PUSHSCORE);
//
//	if(bAuto)
//	{
//		float v=var.ToFloat();
//		if(v<=pConfig->m_fMaxScore)
//			return;
//	}
//	pConfig->m_fMaxScore=var.ToFloat();
//	var.ToString(XTRUE);
//	char* ii=strchr(var.strTxt,'.');
//	if(ii&&ii-var.strTxt+2<var.GetLength())
//		ii[3]=0;
//	//int l=ii?ii-var.strTxt+2:var.GetLength();
//	txt.FormatTxt(XString16(var.strTxt));
//	//if(m_pBox)
//	//	delete m_pBox;
//#ifdef __APPLE__
////  ?≥È????‰∏??Áß??
////	PostMsg(XWM_APPLE_MESSAGE, XCMD_TOP_SCORE_AVAILABLE, (XU32)this, XMS_TOPMOST);
//#elif defined( __ANDROID__ )
//#else
//	if(!m_bPause)
//	{
//		m_bPause=XTRUE;
//		AddCommand(XPAUSE,0,0);
//		Invalidate();
//		//m_player->Pause(!m_bPause);
//		//m_bPause=!m_bPause;
//	}
//	m_pBox=XMessageBox::Message(txt,this,XCMD_FLASH_SCORE,XMB_YESNO|XMB_ICONQUESTION);
//#endif
//}

void XFlashView::PushScore()
{
	if(!m_bConfigOK) return;
	XConfig*pConfig=m_player->GetConfig();
	XXVar var;

	XBOOL bOK=GetScore(var);//m_player->GetVariable(cnt,pConfig->m_strScoreVar,var);

	var.ToString(XFALSE);
	XU8Array data;
	data.Append((XU8*)"Score=",6);
	data.Append((XU8*)var.strTxt,XString8::SafeStrlen(var.strTxt));
	data.Add('&');//.Append('&');
	XPCTSTR str=XEAB()[XEAB::CONTENT_ID];
	data.Append((XU8*)str,XString8::SafeStrlen(str));
	data.Add('=');
	XString8 strCntID;
	this->MakeContentID(NULL,strCntID);
	if(strCntID.IsEmpty())
		data.Append((XU8*)pConfig->m_strContentID.GetData(),pConfig->m_strContentID.GetLength());
	else
	{
		data.Append((XU8*)strCntID.GetData(),16);//strCntID.GetLength());
	}
	//XSystem::m_strPostData.SetString((XPCTSTR)data.GetData(),data.GetSize());
	XSystem::ResetHTMLData();
    XString8 str8((XPCTSTR)data.GetData(), data.GetSize());
	XSystem::SetPostData( str8 );
	XSystem::LaunchHTML(pConfig->m_strScoreURL);
	//XSystem::SetURL(pConfig->m_strScoreURL);
/*	if(!m_pTxtView)
	{
		m_pTxtView=new XDomViewExt(m_pDom->m_draw.pDraw,m_pDom->m_pClientApply);
		if(m_pTxtView)
		{

			XRect rt(m_rect.left+3,m_rect.top+POPWIN_TITLE,m_rect.right-3,m_rect.bottom-10);
            
#ifdef __APPLE__
//            if (!g_deviceData.mbOnSale)
//            {
//                rt.right = m_rect.right/2-3;
//                rt.bottom = m_rect.bottom/2-10;
//            }
#endif
		   m_pTxtView->SetFrameView(this);
		   m_nConPecent=0;
		   m_pTxtView->Create(rt,this,XNULL,0,XCHARSET_UTF8);
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//		   m_bar.ShowWindow(XFALSE);
//#endif
#ifdef __APPLE__
            g_deviceData.m_isDomExtView = true;
            g_deviceData.m_isDomView = false;
            PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASHTODOMEXT, (XU32)0, XMS_TOPMOST); //主线程调用
#endif
		}
	}
	m_nConnecting=1;
	m_pTxtView->Connect(pConfig->m_strScoreURL,XEM::POST,XNULL,data);
	Invalidate();*/
}


void XFlashView::DoPay(const char*url)
{
	if(url)
	{
		XSystem::ResetHTMLData();
		XSystem::LaunchHTML(url);
		return;
	}
	if(!m_player->m_pDomFilter) return;
	XSystem::LaunchHTML(m_player->m_pDomFilter->strPayURL);
}

//void XFlashView::OpenWindow(const char*url)
//{
//	if(!m_pTxtView)
//	{
//		m_pTxtView=new XDomViewExt(m_pDom->m_draw.pDraw,m_pDom->m_pClientApply);
//		if(m_pTxtView)
//		{
//            XRect rt(m_rect.left+3,m_rect.top+POPWIN_TITLE,m_rect.right-3,m_rect.bottom-10);
//#ifdef __APPLE__
////            if (!g_deviceData.mbOnSale)
////            {
////                rt.right = m_rect.right/2-3;
////                rt.bottom = m_rect.bottom/2-10;
////            }
//#endif
//           m_pTxtView->SetFrameView(this);
//		   m_nConPecent=0;
//		   m_pTxtView->Create(rt,this,XNULL,0,XCHARSET_UTF8);
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////		   m_bar.ShowWindow(XFALSE);
////#endif
//#ifdef __APPLE__
//            g_deviceData.m_isDomExtView = true;
//            g_deviceData.m_isDomView = false;
//            PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASHTODOMEXT, (XU32)0, XMS_TOPMOST); //主线程调用
//#endif
//		}
//	}
//	this->Pause();//ause();
//	//AddCommand(XPAUSE,0,0);
//
//	m_nConnecting=1;
//
//
////#ifdef __ANDROID__
////	CONNECTREQ req;
////	req.bForward = true;
////	req.nMethod = XEnumMethod::GET;
////	req.URL.SetURL(url);
////	m_pTxtView->Connect(req, true);
////#else
//	XU8Array data;
//	m_pTxtView->Connect(url,XEM::GET, NULL,data,NULL,NULL);
////#endif
//	Invalidate();
//}

void XFlashView::ZoomIn()
{
	if(m_nZoomPecent<=0) return;
	m_nZoomPecent-=10;
	if(m_nZoomPecent<0) m_nZoomPecent=0;
	AddCommand(XZOOMINOUT,100,m_nZoomPecent);
}

void XFlashView::SetZoomPecent(int v)
{
	if(v==m_nZoomPecent) return;
	m_nZoomPecent = v;
	AddCommand(XZOOMINOUT,100,m_nZoomPecent);
}

void XFlashView::ZoomOut()
{
	if(m_nZoomPecent>=400) return;
	m_nZoomPecent+=10;
	if(m_nZoomPecent>400) m_nZoomPecent=400;
	AddCommand(XZOOMINOUT,100,m_nZoomPecent);
}

XBOOL XFlashView::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
//	case XCMD_DESTORYFLASHEDIT:
//		 return DestroyEdit(pData1);
//	case XCMD_CREATEFLASHEDIT:
//		 return CreateEdit((XDTDefineEditText*)pData1,*(XRect*)pData2);
	case XCMD_FLASHSHOWMODE:
		 AddCommand(XDEVICE,0,0);
		 break;
	case XWM_COMMAND:
		 switch(pData1)
		 //if(pData1==XCMD_FLASH_ZOOMINOUT)
		 {

		 case XCMD_FLASH_PAY:
			 if(m_player->m_pDomFilter&&!m_player->m_pDomFilter->strPayURL.IsEmpty())
				 DoPay();
				 //OpenWindow(m_player->m_pDomFilter->strPayURL);
			 break;
         case XCMD_FLASH_ZOOMIN:
                 if(m_nZoomPecent<=0) return false;
                 m_nZoomPecent-=3;
                 if(m_nZoomPecent<0) m_nZoomPecent=0;
                 AddCommand(XZOOMINOUT,100,m_nZoomPecent);
                 break;
         case XCMD_FLASH_ZOOMOUT:
                 if(m_nZoomPecent>=400) return false;
                 m_nZoomPecent+=1;
                 if(m_nZoomPecent>400) m_nZoomPecent=400;
                 AddCommand(XZOOMINOUT,100,m_nZoomPecent);
                 break;
         case XCMD_FLASH_STARTDRAG:
                 if ((m_nZoomPecent>0&&m_nZoomPecent<=500)&&(m_nTwoPoint)) {
                     StartDrag();
                 }
                 break;
         case XCMD_FLASH_DRAGING:
                 if ((m_nZoomPecent>0&&m_nZoomPecent<=500)&&(m_nTwoPoint)) {
                     Draging(true);
                 }
                 break;
         case XCMD_FLASH_ENDDRAG:
                 EndDrag();
                 break;
		 //case XCMD_FLASH_OBJZOOM:
			// AddCommand(XAUTOOBJECT,0,0);
			// break;
//		 case XCMD_FLASH_POINTZOOM:
//			 if(m_bLockPoint)
//			 {
//				 m_bLockPoint=false;
//				 XRect rect;
//				 GetPointZoomRect(rect);
//				 InvalidateRect(rect);
//				 //m_bar.SetItemStatus(XCMD_FLASH_POINTZOOM,XTS_NORMAL);
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////				 m_bar.CheckItem(XCMD_FLASH_POINTZOOM,XFALSE);
////#endif
//			 }
//			 else if(m_nZoomPecent)
//			 {
//				 AddCommand(XZOOMINOUT,0,0);
//                 //-----------------------
//#ifdef __APPLE__
//				 PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_ZOOMINENABLE, (XU32)this, XMS_TOPMOST);
//#endif
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////				 m_bar.CheckItem(XCMD_FLASH_POINTZOOM,XFALSE);
////				 m_bar.SetItemSlider(XCMD_FLASH_ZOOMINOUT,0);
////#endif
//			 }
//			 else
//			 {
//#if defined(__ANDROID_TV_ON_)	// zhengjl 2012.04
//				 AddCommand(XZOOMINOUT,0,100);
//#else
//				 m_nowPt1.x=(this->m_rect.left+m_rect.right)/2;
//				 m_nowPt1.y=(this->m_rect.top+m_rect.bottom)/2;
//				 m_bLockPoint=XTRUE;
//				 XRect rect;
//				 GetPointZoomRect(rect);
//				 InvalidateRect(rect);
//                  
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////				 m_bar.CheckItem(XCMD_FLASH_POINTZOOM,XTRUE);
////#endif
//#endif
//			 }
//			 break;
//		 case XCMD_FLASH_ZOOMINOUT:
//			 if(pData2>=100)
//				 AddCommand(XZOOMINOUT,0,pData2-100);
//#ifdef _LOWSPEED
//			 else
//			 {
//				 switch(pData2)
//				 {
//				 case XTOOLBAR_MV_DOWN:
//					  AddCommand(XLOCKZOOMINOUT,0,0);
//					  break;
//				 case XTOOLBAR_MV_UP:
//					  AddCommand(XUNLOCKZOOMINOUT,0,0);
//					  break;
//				 }
//			 }
//#endif
//			 return XTRUE;
		 }
		 break;
	//case XWM_ENDDIALOG:

	//	 {
	//		 switch(pData2)
	//		 {

	//		 case XCMD_FLASH_SAVE:
	//			 if(!m_pSave)
	//				 break;
	//			 m_pSave->Release();
	//			 //m_pSave=XNULL;
	//			 m_bPause=XFALSE;
	//			 AddCommand(XRESUME,XNULL,0);
	//			 Invalidate();
	//			 break;
	//		 case XCMD_FLASH_SCORE:
	//			 //PushScore();
	//			 //if(m_pBox)
	//			 //	 delete m_pBox;
	//			 //m_pBox=XNULL;
	//			 if(pData1==XIDYES)
	//				AddCommand(XPUSHSCORE,0,0);
	//			 else if(m_bPause)
	//				 Pause();
	//			 break;
	//		 case XCMD_FLASH_RESTART:
	//			 if(pData1==XIDYES)
	//			 {
	//				 m_player->Interrupt();
	//				 m_player->Stop();
	//				 m_pDom->ReConnectEx(m_player->GetFilter());
	//			 }
	//			 else if(m_bPause)
	//			 {
	//				 m_bPause=XFALSE;
	//				 AddCommand(XRESUME,0,0);
	//				 Invalidate();
	//			 }
	//			 break;
	//		 case XCMD_FLASH_CLOSE:

	//			 if(pData1==XIDYES)
	//			 {
	//				 m_player->Interrupt();
	//				 m_player->Stop();
	//				 m_pDom->PostMsg(XWM_COMMAND,XCMD_FLASH_CLOSE,0,XMS_THIS);
	//			 }
	//			 else if(m_bPause)
	//			 {
	//				 m_bPause=XFALSE;
	//				 AddCommand(XRESUME,0,0);
	//				 Invalidate();
	//			 }

	//			 break;
	//		 }
	//	 }
	//	 return XTRUE;
	case XCMD_UPDATECLICKOPT:
#ifdef __ANDROID__
		PostMsg(ANDROID_UPDATE_CLICKOPT, 0, (XU32)this, XMS_TOPMOST);
#elif defined (__APPLE__)
            PostMsg(XWM_APPLE_MESSAGE, XCMD_UPDATE_CLICKOPT, (XU32)this, XMS_TOPMOST);
#else
		 if(!m_optImage.IsEmpty())
		 {
			m_nLockOptTime=12;
			int w=m_optImage.Width();
			int h=m_optImage.Height()/OPT_COUNT;
			XRect rect(m_rect.Width()-w-10,
					   10,
					   m_rect.Width()-10,
					   10+h);
			InvalidateRect(rect);
		 }
#endif
		 break;
	case XCMD_UPDATEOBJ:
#ifdef __APPLE__
			PostMsg(XWM_APPLE_MESSAGE, (pData1-2)?XCMD_FLASH_ZOOMOUTENABLE:XCMD_FLASH_ZOOMOUTENABLE, (XU32)this, XMS_TOPMOST);
#endif
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	#ifdef _SYMBIAN
//	#else
//		 if(pData1>=2)
//		 {
//			 m_bar.SetItemStatus(XCMD_FLASH_MOVEMODE,pData1==2?XTS_DISABLE:XTS_NORMAL);
//			 m_bar.CheckItem(XCMD_FLASH_MOVEMODE,XFALSE);
//			 m_bar.SetItemSlider(XCMD_FLASH_ZOOMINOUT,pData1-2);
//			 m_nZoomPecent=pData1-2;
//		 }
//		 else if(pData1)
//			 m_bar.SetItemStatus(XCMD_FLASH_OBJZOOM,XTS_NORMAL);
//		 else
//			 m_bar.SetItemStatus(XCMD_FLASH_OBJZOOM,XTS_DISABLE);
//	#endif
//#endif
		 break;
	case XCMD_GETFLASHURL:
		 if(pData1)
		 {
			 LoadInfo*p=(LoadInfo*)pData1;

			 int nMethod=XEM::GET;
			 if((p->nType&kHttpSendUsePost))//||p->postData.GetSize())
				 nMethod=XEM::POST;

			 if(!p->IsTargetLoader()&&p->layer<0)
			 {  //Get Nomal URL
				 switch(p->nTarget)
				 {
				 case XPREVFILTER::_SELF:
				 case XPREVFILTER::_NULL:
					  //m_pDom->ToDomView();
					  m_player->Interrupt();
					  m_player->Stop();
					  m_pDom->ToDomViewEx();
					  m_pDom->ConnectEx(p->strURL.strTxt,nMethod,
						 p->strTarget.strTxt,p->postData,p->strContext.IsNull()?XNULL:p->strContext.strTxt,&p->strHeaders);
					  break;
				 case XPREVFILTER::_POPUP:
					 {
                       //  #ifdef WEBVIEWPAYMODE
                         CONNECTREQ reqWebView;
                         reqWebView.URL.SetURL(p->strURL.strTxt);
                         if(reqWebView.URL.m_nProto == XPROTO_WEBV)
                         {
                             //this->Pause();
                             //Invalidate();
                             if(!g_BaiWanPayUrlSavedData)
                                 g_BaiWanPayUrlSavedData = new BWSendMessageContentClass;
                             XLock lock(&(g_BaiWanPayUrlSavedData->m_lock));
                             if(!g_BaiWanPayUrlSavedData->parameter1)
                             {
                                 g_BaiWanPayUrlSavedData->parameter1 = (char *)malloc(reqWebView.URL.m_strURL.GetLength()); //接收方负责释放空间
                                 g_BaiWanPayUrlSavedData->parameter1Len = reqWebView.URL.m_strURL.GetLength();
                                 memcpy(g_BaiWanPayUrlSavedData->parameter1, reqWebView.URL.m_strURL.GetData(), reqWebView.URL.m_strURL.GetLength());
                             }
                             lock.UnLock();
                             #ifdef __APPLE__
                                PostMsg(XWM_APPLE_MESSAGE, XCMD_SHOWWEBVIEW, (XU32)0, XMS_TOPMOST); //主线程调用
                             #endif
							#ifdef __ANDROID__
							   PostMsg(ANDROID_SHOWWEBVIEW, 0,(XU32)0, XMS_TOPMOST); //主线程调用
							   #endif
                             break;
                         }

                        // #endif
                         // pop webView end
                         
//						if(!m_pTxtView)
//						{
//							m_pTxtView=new XDomViewExt(m_pDom->m_draw.pDraw,m_pDom->m_pClientApply);
//							if(m_pTxtView)
//							{
//                                XRect rt(m_rect.left+3,m_rect.top+POPWIN_TITLE,m_rect.right-3,m_rect.bottom-10);
//#ifdef __APPLE__
////                                if (!g_deviceData.mbOnSale)
////                                {
////                                    rt.right = m_rect.right/2-3;
////                                    rt.bottom = m_rect.bottom/2-10;
////                                }
//#endif
//							   m_pTxtView->SetFrameView(this);
//							   m_nConPecent=0;
//							   m_pTxtView->Create(rt,this,XNULL,0,XCHARSET_UTF8);
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////							   m_bar.ShowWindow(XFALSE);
////#endif
//							}
//						}
						this->Pause();//ause();
						//AddCommand(XPAUSE,0,0);

						m_nConnecting=1;

						XSystem::ResetHTMLData();
						if(p->postData.GetSize()>0)
						{
                            XString8 str8((XPCTSTR)p->postData.GetData(),p->postData.GetSize());
							XSystem::SetPostData( str8 );
						}
						if(p->strHeaders.GetSize()>0)
						{
							int i=0;
							XString8 str;
							for(i=0;i<p->strHeaders.GetSize()-1;i+=2)
							{
								str+=(p->strHeaders[i]);
								str+=(":");
								str+=(p->strHeaders[i+1]);
							}
							XSystem::SetHeaders(str);
						}
						XSystem::LaunchHTML(p->strURL.strTxt);
						//m_pTxtView->Connect(p->strURL.strTxt,nMethod,
						//NULL,p->postData,p->strContext.IsNull()?XNULL:p->strContext.strTxt,&p->strHeaders);
#ifdef __APPLE__
                         g_deviceData.m_isDomExtView = true;
                         g_deviceData.m_isDomView = false;
                         PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASHTODOMEXT, (XU32)0, XMS_TOPMOST); //主线程调用
#endif

						Invalidate();
					 }
					  break;
				 /*case XPREVFILTER::_SELF:
					 {
						 XClient*pClient=
						 m_pDom->ConnectContext(p->strURL.strTxt,nMethod,
							 p->postData,p,
							 p->strRefer.IsNull()?NULL:p->strRefer.strTxt,
							 p->strContext.IsNull()?XNULL:p->strContext.strTxt,
							 &p->strHeaders);
						if(pClient)
							AddClient(pClient);
					 }*/
					 break;
				 }
				 //XReleaseINFO(p);
				 //if(p->pLoader)

				 //delete p;
			 }
			 else
			 {
				 XClient*pClient=
					 m_pDom->ConnectContext(p->strURL.strTxt,nMethod,
					 p->postData,p,
					 p->strRefer.IsNull()?NULL:p->strRefer.strTxt,
					 p->strContext.IsNull()?XNULL:p->strContext.strTxt,
					 &p->strHeaders,p->nType&(kHttpLoadRPC)?false:true);
				 if(pClient)
				 {
					 if(p->nType&(kHttpLoadRPC))
					 {
						 pClient->SetAlive(true);
						 pClient->SetNeedCache(false);
					 }
					 AddClient(pClient);
					 _XStreamData*stream=(_XStreamData*)pClient->GetDataI();
					 stream->loadInfo=p;
				 }
//#if (__CORE_VERSION__>=0x02077000)
				 p->pClient=pClient;
//#endif
			 }

		 }return XTRUE;
	//case XCMD_FLASHSCORE:
	//	 //BeginScore();
	//	 AddCommand(XBEGINSCORE,XTRUE,0);
	//	 break;
	case XCMD_CLOSECLIENT:
		 if(pData1)
		 {
			 XClient*pClient=(XClient*)pData1;
			 CLIENTINFO*pInfo=m_pClientInfo;
			 while(pInfo)
			 {
				 if(pInfo->pClient==pClient)
				 {
					 pClient->SetWindow(XNULL,0,0);
					 pClient->Cancel();
					 //m_infoAlloc.Free(pInfo);
					 break;
				 }
				 pInfo=pInfo->pNext;
			 }
		 }break;
	case XCMD_REMOVECLIENT:
		if(pData1)
		{
			RemoveClient((XClient*)pData1);
		}break;
	case XCMD_FLASHCOMMAND:
		 switch(pData1)
		 {
		 case XFLASH_QUIT:
			  CloseFlash();
			  //m_pDom->ToDomView();//Back(XTRUE);
			  break;
		 case XFLASH_REFLASH:
			  //if(!m_pTxtView)
				AddCommand(XRESTART,0,0);
			  break;
		 case XFLASH_RELOAD:
			  if(pData2)
			  {
				  m_strReloadURL=(char*)pData2;
				  m_pDom->SetValueOf(m_strReloadURL);
				  AddCommand(XRELOAD,0,0);
			  }
			  else
				  m_strReloadURL.Empty();
			  break;
		 case XFLASH_POSTDATA:
			  if(pData2)
				FlashPostData((char*)pData2);
			  break;
		 }break;
	//case XCMD_
	case XCMD_PROCFLASHTEXT:
		 return ProcHtmlText((XPCTSTR)pData1,*(XXVar*)pData2,XCHARSET_UTF8);//(XU8)pData2);
	}
	return XWindow::Handle(nOpera,pData1,pData2);
}

//#if (__CORE_VERSION__>=0x02079000)
void XFlashView::FlashPostData(char*strData)
{
	 XString8 strLine;
	 int nMethod=XEM::GET;
	 XU8Array strPost;
	 XString8 strURL;
	 for(;;)
	 {
		 char c=*strData;
		 strData++;
		 if(!c) break;
		 switch(c)
		 {
		 case '\r':
		 case '\n':
			 if(!strLine.IsEmpty())
			 {
				 int iSet=strLine.Find(':');
				 if(iSet>0)
				 {
					 XString8 strKey,strCnt;
					 strLine.Left(strKey,iSet);
					 strLine.Mid(strCnt,iSet+1);
					 strKey.TrimLeft();
					 strKey.TrimRight();
					 strCnt.TrimLeft();
					 strCnt.TrimRight();
					 switch(strKey[0])
					 {
					 case 'm':
					 case 'M':
						 if(strCnt[0]=='P'||strCnt[0]=='p')
							 nMethod=XEM::POST;
						 break;
					 case 'u':
					 case 'U':
						 strURL=strCnt;
						 break;
					 case 'd':
					 case 'D':
						 //strPost=strCnt;
						 strPost.Append((XU8*)strCnt.GetData(),strCnt.GetLength());
						 break;
					 }
				 }
				 strLine.Empty();
			 }
			 break;
		 default:
			 strLine+=c;
			 break;
		 }
	 }
	 if(!strURL.IsEmpty())
	 {
		 m_pDom->ConnectPost(strURL,nMethod,strPost);
	 }
}
//#endif

XBOOL XFlashView::ProcHtmlText(XPCTSTR str,XXVar&strTxt,XU8 set)
{
   if(str!=XNULL)//!strTxt.IsEmpty())
   {
	   XDom dom;
	   dom.Reset(set,XCNT::CTEXT,XCNT::TVND_WAP_WML,XNULL);
	   dom.AddTag(new XDomNode(XDOM_WML));
	   dom.AddTag(new XDomCard());
	   dom.PhaseXML((XU8*)str,strlen(str),false,true);
	   XString16 str;
	   dom.GetText(str);
	   strTxt.SetString(XNULL,str.GetLength()*2+1);
	   XPCWSTR buf=str.GetData();
	   for(int i=0;i<(int)str.GetLength();i++)
		   strTxt.AddWChar(buf[i]);
	   //strTxt=str;
   }
   return XTRUE;
}

/*XBOOL XFlashView::CreateEdit(XDTDefineEditText *pText, XRect &rect)
{
	rect+=m_flashBase;
	if(m_pEdit!=XNULL)
	{
		DestoryChild(m_pEdit);
		delete m_pEdit;
		m_pEdit=XNULL;
	}

    m_pEdit=new XFlashEdit;
	XU8 s=XEDS_ANY;
	m_pEdit->Create(rect,this,m_pDom->GetGraphics(),pText);
	m_pEdit->OnFocus();
	//m_edits.Add(pEdit);

	return XTRUE;
}

XBOOL XFlashView::DestroyEdit(XU32 id)
{
	if(m_pEdit==XNULL) return XFALSE;
	if(m_pEdit->GetID()!=id) return XFALSE;
	DestoryChild(m_pEdit);
	delete m_pEdit;
	m_pEdit=XNULL;

	return XTRUE;
}*/

XBOOL XFlashView::OnChar(XWCHAR nChar)
{
/*	if(m_pEdit)
		return m_pEdit->OnChar(nChar);
	else*/
	/*if ( nChar != VK_TAB && nChar != VK_RETURN && nChar != VK_BACK)
	{
				m_player->HandleKeyDown( nChar, 0 );
	}*/
	//if(m_pTxtView&&m_pTxtView->GetParent()==this)
	//	m_pTxtView->OnChar(nChar);
	//else
		m_player->OnChar(nChar);

	return XWindow::OnChar(nChar);
}

/*void XFlashView::InitWindow()
{
	XSIZE sz;
	m_player->GetShowSize(sz);
	CRect rect;
	GetClientRect(rect);
	m_flashBase.x=(rect.Width()-sz.cx)>>1;
	m_flashBase.y=(rect.Height()-sz.cy)>>1;
	//m_flashRect=CRect(nx,ny,nx+sz.cx,ny+sz.cy)
} */

XBOOL XFlashView::NewStream(XClient*pClient,const char*urlBase)
{
	//return XFALSE;
	//if(!(pClient->GetCallID()&1)) return XFALSE;
#ifdef _WINEMU
	//m_pDom->Trace(XString16("Stream In Begin"));
#endif
	if(!pClient->GetNotifyID()) return XFALSE;
	RESPONSEINFO*pInfo=pClient->GetResponseInfo();
	if(pInfo->nErrorCode!=XHTTP_STATUS_OK)
	{
		if(pClient==m_pConfigClient)
		{
			m_bConfigOK=XTRUE;
			m_player->GetConfig()->m_bConfigOK=XTRUE;
			//m_pConfigClient=NULL;
			AddCommand(XUPDATEBUTTONS,0,0);
			//PaintButtons();
		}
		return XFALSE;
	}
	/*XDOMFILTER* pFilter=pClient->m_pCurFilter;
	if(!pFilter)
	{
		XDom*pDom=m_pDom->GetDom();
		//if(pDom->m_filter
		pFilter=pClient->m_pReferFilter;
		if(!pFilter)
			pFilter=pDom->m_pFilter;
	}*/
	//Modify FILTER
	XDOMFILTER*pFilter=m_pDom->GetDom()->m_pFilter;
	if(pFilter)
	{
		//if(pFilter->nMaxPecent)
		//	m_player->m_nMaxPecent=pFilter->nMaxPecent;
		if(!m_pObjectFilter)
			m_pObjectFilter=pFilter;
		m_player->SetFilter(pFilter);
#ifdef __APPLE__
#ifdef __HDShowPay
        if (pFilter->strPayURL.GetLength()!=0)
            PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_CANPAY, 0, XMS_TOPMOST);
#endif  // __HDShowPay
#endif  // __APPLE__
		//if(pFilter->nContentMode!=m_nCntMode)
		{
			//m_nCntMode=pFilter->nContentMode;
			UpdateToolBar();
		}
	}
	AddClient(pClient);
	//RESPONSEINFO*pInfo=pClient->GetResponseInfo();

	if(pInfo->type.m_nType==XContentType::TXCONFIG)
	{
		if(m_pCfgDom==XNULL)
			m_pCfgDom=new XDom;
		if(m_pCfgDom)
		{
			m_pCfgDom->Reset(pInfo->type.m_nCharset,XCNT::CTEXT,XCNT::TVND_WAP_WML,
					     &m_pDom->m_draw);
		}
		m_pConfigClient=NULL;
		return XTRUE;
	}

	if(!pInfo->strConfigURL.IsEmpty())
	{
		if(m_pConfigClient==NULL)
		{
			m_bConfigOK=XFALSE;
			XU8Array data;
			//m_player->m_nScreenDirect=0;
	#ifdef _WINEMU
			XString16 s("---->Loading Config:");
			s+=XString16(pInfo->strConfigURL);
			m_pDom->Trace(s);
	#endif
			m_pConfigClient=m_pDom->ConnectContext(pInfo->strConfigURL,XEM::GET,data,(void*)-1);
		}
	}
	else if (this->m_player->firstStream)
	{
		m_bConfigOK=XTRUE;
		m_player->GetConfig()->m_bConfigOK=XTRUE;
		//m_player->m_nScreenDirect=2;
		LoadDefaultConfig();
#ifdef __APPLE__
		PostMsg(XWM_APPLE_MESSAGE, XCMD_UPDATE_BAR, (XU32)this, XMS_TOPMOST);
#endif
#ifdef __ANDROID__
		PostMsg(ANDROID_START_FLASH, 0, (XU32)this, XMS_TOPMOST);
#endif
		//PaintButtons();
		AddCommand(XUPDATEBUTTONS,0,0);
	}

#ifdef _THREADBACK
	return AddCommand(XNEW_STREAM,XNULL,0,pClient);
#else
	XString8Array list;
	
	//XString8 strBase=urlBase;//pClient->GetURL();

	if(m_pDom->GetDom()->m_argList.GetSize())
	{
		//XString8Array&aglist=m_pDom->GetDom()->m_argList;
		//int i,nc=aglist.GetSize();
		//for(i=0;i<nc;i+=2)
		//{
		//	if(aglist[i].Compare("base",XTRUE)==0)
		//	{
		//		XString8 strBase=aglist[i+1];
		//		int l=strBase.GetLength();
		//		if(l)
		//		{
		//			if(strBase[l-1]!='/')
		//				strBase+="//";
		//			this->m_player->SetURLBase(strBase);
		//		}

		//		//XURL url;
		//		//url.SetURL(urlBase);
		//		//url.SetURL(aglist[i+1]);
		//	//	strBase=url.m_strURL;
		//	}
		//}
		list.Append(m_pDom->GetDom()->m_argList);
	}
	char*streamUrl=(char*)pClient->GetURL();
	if(urlBase!=streamUrl)
		XDom::GetArgList(urlBase,NULL,list);
	XDom::GetArgList(streamUrl,pInfo,list);
	if(m_player->firstStream)
	{
		streamUrl=(char*)m_pDom->m_pDom->GetDomURL().m_strURL;
		if(urlBase!=streamUrl)
			XDom::GetArgList(streamUrl,pInfo,list);
		/*if(pFilter&&!pFilter->strCookies)
		{
			XString8 strValue=pFilter->strCookies;
			SetValueOf(strValue);
			XURL url;
			url.SetURL(pFilter->strHost);
			m_pDom->SetURLCookie(url.m_strHost,strValue,XTime::GetTimeMSec());
		}*/
	}


	if(!pFilter)
	{
		if(pInfo->type.m_nType==XCNT::TX_SHOCK_WAVE_FLASH)
		{
			pFilter=m_pObjectFilter;
		}
	}
	if(pFilter)
	{
		//this->m_player->SetFilter(pFilter);
		int nSize=pFilter->strParams.GetSize();
		for(int i=0;i<nSize;i+=2)
		{
			//XString8 strName=pFilter->strParams[i];
			//if(strcmp(pFilter->strParams[i],"flag")==0)
			//	int v=0;
			int ie=XDom::IsExist(list,pFilter->strParams[i]);//) continue;
			if(!ie)
			{
				list.Add(pFilter->strParams[i]);
				ie=list.GetSize()+1;
				XString8 strValue=pFilter->strParams[i+1];
				SetValueOf(strValue);
				list.Add(strValue);//pFilter->strParams[i+1]);
			}
			else
			{
				XString8 strValue=pFilter->strParams[i+1];
				SetValueOf(strValue);
				list[ie]=strValue;
			}
			
		}
	}
#ifdef _WINEMU
	if(m_player->firstStream)
	{
		XString8 strOut("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<xml>");
		Trace(XString16("<------------------- Arguments --------------->"));
		for(int i=0;i<list.GetSize();i+=2)
		{
			XString8&strKey=list[i];
			XString8&strValue=list[i+1];
			XString16 strMsg("Key: ");
			strMsg+=XString16(strKey);
			strMsg+=XString16("  Value: ");
			strMsg+=XString16(strValue);
			Trace(strMsg);
			strOut+=XString8("<");
			strOut+=strKey;
			strOut+=XString8(" value=\"");
			strOut+=strValue;
			strOut+=XString8("\"/>\r\n");
		}
		Trace(XString16("<----------------- Arguments end ------------->"));
		strOut+=XString8("</xml>");
		XFile file;
		if(file.Open("D://BXMJCS/key.xml",XFile::XBINARY|XFile::XWRITE|XFile::XCREATE))
		{
			file.Write(strOut.GetData(),strOut.GetLength());
			file.Close();
		}
	}
#endif
	//m_pClientApply->GetFilter(m_req.URL.m_strURL)
	_XStreamData*pStream=(_XStreamData*)pClient->GetDataI();
	if(!m_bInitOK)
		m_bNetwork=pClient->m_nProtoType<XAPP_FILE;
	pStream->nLoadedBytes=pInfo->nTotalSize;
	pStream->nTotalBytes=pInfo->nLength;
	if(m_player->firstStream)
		UpdateToolBar();
	m_player->StreamInNew(pStream,
						(char*)pClient->GetURL(),
						//strBase,
						(char*)urlBase,
						pInfo->type.m_strType,
						(void*)(pClient->GetNotifyID()&~1),
						list,
						pInfo->nErrorCode,
						pClient);
    ////if (m_nCntMode == XCNT_MODE_LOCAL && m_player->GetConfig()->m_nContentType == XConfig::GAME)
    //{
    //    //XSize sz = m_rect.Size();
    //    //SetSize(sz);
    //    m_player->FullScreen();
    //}
	return XTRUE;
#endif
    

}

void XFlashView::CloseStream(XClient *pClient)
{
	//if(!(pClient->GetCallID()&1)) return;
	if(!pClient->GetNotifyID())
		return;

	if(!pClient->IsAlive()||pClient->IsFinished())
		RemoveClient(pClient);
	RESPONSEINFO*pInfo=pClient->GetResponseInfo();
	if(pClient==m_pConfigClient)
	{
		m_pConfigClient=NULL;
		if(pInfo->nErrorCode!=XHTTP_STATUS_OK)
		{
			m_bConfigOK=true;
			m_player->GetConfig()->m_bConfigOK=XTRUE;
			//PaintButtons();
			AddCommand(XUPDATEBUTTONS,0,0);
			return;
		}
	}

		
	if(pInfo->type.m_nType==XContentType::TXCONFIG)
	{
		if(m_pCfgDom)
		{
			m_player->GetConfig()->Reset();
			PhaseConfig();
			m_player->UpdateConfig();
			m_bConfigOK=XTRUE;
			m_player->GetConfig()->m_bConfigOK=XTRUE;
			UpdateToolBar();

//			PostMsg(XWM_APPLE_MESSAGE, XCMD_START_FLASH, (XU32)this, XMS_TOPMOST);
            XConfig* config = m_player->GetConfig();
			PostMsg(XWM_UPDATEMSG,XCMD_FLASH_START,(XU32)this,XMS_TOPMOST);
//#ifdef __ANDROID__
//			PostMsg(ANDROID_START_FLASH, 0, (XU32)this, XMS_TOPMOST);
//#endif
            //if(m_nCntMode != XCNT_MODE_NET && m_player->GetConfig()->m_nContentType == XConfig::MEDIA)
            //    ;
            //else
            //{
            //    //XSize sz = m_rect.Size();
            //    //SetSize(sz);
            //    m_player->FullScreen();
            //}
            AddCommand(XMOVE, 0, 0);

			//Invalidate();
			//PaintButtons();
			AddCommand(XUPDATEBUTTONS,0,0);
		}
		return;
	}
#ifdef _THREADBACK
	AddCommand(XCLOSE_STREAM,XNULL,0,pClient);
#else
	//memset(pClient->GetDataI(),0,ICSIZE);
	XBOOL bOK=pInfo->nErrorCode==XHTTP_STATUS_OK;
	_XStreamData*pStream=(_XStreamData*)pClient->GetDataI();
	const char* msg=XEnumHttpStatus::GetErrorString(pInfo->nErrorCode);

	pStream->nLoadedBytes=pInfo->nTotalSize;
	pStream->nTotalBytes=pInfo->nLength;
	m_player->StreamInDestroy(pStream,bOK,msg,pClient);
	m_bLoadOK=XTRUE;
	SetFocus(XTRUE);

	if(!pClient->IsAlive())
	{
		memset(pClient->GetDataI(),0,ICSIZE);
		pClient->SetWindow(NULL,0,0);
	}
#endif
	if(pClient==m_pFirstClient)
	{
		m_bSaveReady=XTRUE;
		PostMsg(XWM_UPDATEMSG,XCMD_FLASH_LOADING_END, (XU32)this, XMS_TOPMOST);
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	if(m_bNetwork)
//		m_bar.SetItemStatus(XCMD_FLASH_SAVE,XTS_NORMAL);
//#endif

//#ifdef __ANDROID__
//	if (m_bNetwork)
//	{
//		m_flashSaveReady = true;
//		PostMsg(ANDROID_FLASH_LOADING_END, 0, (XU32)this, XMS_TOPMOST);
//	}
//#endif
//#ifdef __APPLE__
//        if (m_bNetwork)
//        {
//            m_flashSaveReady = true;
//            PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_CANSAVE, 0,XMS_TOPMOST);
//        }
//#endif
	}
    //if (m_nCntMode == XCNT_MODE_LOCAL && m_player->GetConfig()->m_nContentType == XConfig::GAME)
    //{
    //    //XSize sz = m_rect.Size();
    //    //SetSize(sz);
    //    m_player->FullScreen();
    //}

}

void XFlashView::OnMove(XRect &rect)
{
	m_rect=rect;
	m_nMouseX=rect.Width()/2;
	m_nMouseY=rect.Height()/2;
	//WindowView::sharedWindowView()->resize(rect.Width(),rect.Height());
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	int h=m_bar.GetWindowRect().Height(),rh=m_rect.Height();
//	XRect rtt(0,rh-m_nBarPos,m_rect.Width(),rh+h-m_nBarPos);
//	m_bar.MoveWindow(rtt);
//#endif
	AddCommand(XMOVE,0,0);
	
}

void XFlashView::EndPost()
{
	if(!m_player) return;
	XString8 strURL,strContentID;
	if(m_player->m_pDomFilter)
	{
		strURL=m_player->m_pDomFilter->strEndURL;
		strURL=m_player->m_pDomFilter->strContentID;
	}
	else
	{
		strURL=m_player->GetConfig()->m_strEndURL;
		strURL=m_player->GetConfig()->m_strContentID;
	}
	if(strURL.IsEmpty())
		return;
	//XClient* XDomView::ConnectContext(XPCTSTR strURL, int method, XU8Array &data, void *notify,
	//							  const char*refer,const char*strType,XString8Array*headers,bool bCache)
	XU8Array data;
	data.Append((XU8*)"contentID=",10);
	if(strContentID.GetLength())
		data.Append((XU8*)strContentID.GetData(),strContentID.GetLength());
	m_pDom->ConnectPost(strURL,XEM::POST,data);

}

void XFlashView::PhaseConfig()
{
	XU32 i;
	if(m_pCfgDom->m_cardList.GetSize()<=0) 
		return;
	XDomItem*pRoot=m_pCfgDom->m_cardList[0];
	//if(m_pConfig==XNULL)
	//	m_pConfig=new XConfig;
	//if(m_pConfig==XNULL) return;
	XConfig*m_pConfig=m_player->GetConfig();//&m_config;

	XU32 id=0;
	int v;
	XDomItem*pInfo=pRoot->FindByIndex(0,id,XDOM_FILEINFO);
	if(pInfo)
	{
		pInfo->GetAttrib(XEAB::CONTENT_ID,m_pConfig->m_strContentID);
		if(pInfo->GetAttrib(XEAB::CONTENT_LEVEL,v))
			m_pConfig->m_nContentLevel=(XU16)v;
		if(pInfo->GetAttrib(XEAB::HOTKEY_FOR_ZOOM,v))
			m_pConfig->m_nZoomHotKey=(XU16)v;
		//if(pInfo->GetAttrib(XEAB::ZCONTENT_TYPE,v))
		//	m_pConfig->m_nContentType=v;
		//else
		//	m_pConfig->m_nContentType=XConfig::GAME;
        
//        m_pConfig->m_nContentType=XConfig::GAME;
		{
			XString8 strValue;
			if(pInfo->GetAttrib(XEAB::ZWAITFOR,strValue))
			{
				XString8Array list;
				list.SplitString(strValue,',');
				m_pConfig->m_strWaitFor=list[0];//strWord;
				if(list.GetSize()>=2)
				   m_pConfig->m_nWaitID=list[1].ToInt();
			}
		}
		id=0;
		XDomItem*pAD=pInfo->FindByIndex(0,id,XDOM_ADSET);
		if(pAD)
		{
			pAD->GetAttrib(XEAB::SRC,m_pConfig->m_strADURL);
			if(pAD->GetAttrib(XEAB::ALIGN,v))
			{
				switch(v)
				{
				default:
				case XEA::TOP:
					m_pConfig->m_nADMode=XDOMFILTER::AD_TOP;break;
				case XEA::BOTTOM:
					m_pConfig->m_nADMode=XDOMFILTER::AD_BOTTOM;break;
				case XEA::LEFT:
					m_pConfig->m_nADMode=XDOMFILTER::AD_LEFT;break;
				case XEA::RIGHT:
					m_pConfig->m_nADMode=XDOMFILTER::AD_RIGHT;break;
				}
			}
		}
		pAD=pInfo->FindByIndex(0,id,XDOM_ENDSET);
		if(pAD)
			pAD->GetAttrib(XEAB::SRC,m_pConfig->m_strEndURL);
	}

	id=0;
	XDomItem*pDefine=pRoot->FindByIndex(0,id,XDOM_OPRATEDEFINE);
	if(pDefine)
	{
		if(pDefine->GetAttrib(XEAB::DEFAULT_MOUSE_MOVE_SPACE,v))
			m_pConfig->m_nMouseStep=(XU16)v;
		if(pDefine->GetAttrib(XEAB::DEFAULT_LONG_PRESS_LOCK_TIME,v))
			m_pConfig->m_nLongLockTime=(XU16)v;
		if(pDefine->GetAttrib(XEAB::TOUCHSIZE,v))
			m_pConfig->m_nAutoCap=(XU8)v;
		if(pDefine->GetAttrib(XEAB::ZACC,v))
			m_pConfig->m_bACCMap=v;
		if(pDefine->GetAttrib(XEAB::ZBUTTON,v))
			m_pConfig->m_bButtons=v;
	}

	id=0;
	pDefine=pRoot->FindByIndex(0,id,XDOM_TOUCHDEFINE);
	if(pDefine)
	{
		if(pDefine->GetAttrib(XEAB::ZMOUSE_POS,v))
			m_pConfig->m_bMouseShow=v;
	}

	id=0;
	pDefine=pRoot->FindByIndex(0,id,XDOM_DISPLAYDEFINE);
	if(pDefine)
	{
		//if(pDefine->GetAttrib(XEAB::SCREEN_DIRECTION,v))
		//	m_pConfig->m_nScreenDirect=(XU8)v;

		if(pDefine->GetAttrib(XEAB::ZOOM_SIZE,v))
		{
			m_pConfig->m_nZoomMode=(XU8)v;
			//m_nZoom=m_pConfig->m_nZoomMode;
		}
		//if(pDefine->GetAttrib(XEAB::QUALITY,v))
        //    m_pConfig->m_nQuality = v;

		pDefine->GetAttrib(XEAB::ZADURL,m_pConfig->m_strADURL);
		if(pDefine->GetAttrib(XEAB::ZADPOS,v))
			m_pConfig->m_nADMode=v;

		XDomList list;
		pDefine->GetList(list,XDOM_SHOWTAG);
		int i,nc=list.GetSize();
		for(i=0;i<nc;i++)
		{
			//int zoom=fixed_1,align=_TAGSHOW::CENTER;
			_TAGSHOW show={0,0,fixed_1,0,0};
			XDomItem*p=list[i];
			XString8 tags;
			if(!p->GetAttrib(XEAB::ZTAGS,tags)) continue;
			if(p->GetAttrib(XEAB::ZOOM,show.nZoom))
				show.nZoom=show.nZoom*fixed_1/100;
			if(p->GetAttrib(XEAB::ZOFFX,show.offsetx))
			{
				if(show.offsetx<-1||show.offsetx>1)
					show.offsetx=show.offsetx*fixed_1/100;
			}
			if(p->GetAttrib(XEAB::ZOFFY,show.offsety))
			{
				if(show.offsety>1||show.offsety<-1)
					show.offsety=show.offsety*fixed_1/100;
			}
			//p->GetAttrib(XEAB::ZALIGN,show.align);
			XString8Array ts;
			ts.SplitStringS(tags,",; ");
			int ii,ncc=ts.GetSize();
			for(ii=0;ii<ncc;ii++)
			{
			   XString8&str=ts[ii];
			   int id=str.Find('@');
			   if(id==0) continue;
			   if(id<0)
			   {
				   show.nPlace=0;
				   show.nCode=str.ToInt();
			   }
			   else
			   {
				   XString8 strCode,strPlace;
				   str.Mid(strCode,0,id);
				   str.Mid(strPlace,id+1);
				   show.nPlace=strPlace.ToInt();
				   show.nCode=strCode.ToInt();
			   }

			   m_pConfig->AddMap(show);
			}
		}
	}

	id=0;
	pDefine=pRoot->FindByIndex(0,id,XDOM_SOUNDDEFINE);
	if(pDefine)
	{
		if(pDefine->GetAttrib(XEAB::MUTE,v))
			m_pConfig->m_bMute=(XU8)v;
		if(pDefine->GetAttrib(XEAB::VOLUME,v))
			m_pConfig->m_nVolume=(XU8)v;
		if(pDefine->GetAttrib(XEAB::SOUND_DEVICE,v))
			m_pConfig->m_nSoundDevice=(XU8)v;
	}
	id=0;
	pDefine=pRoot->FindByIndex(0,id,XDOM_NETWORKDEFINE);
	if(pDefine)
	{
		pDefine->GetAttrib(XEAB::SCORE_VAR_NAME,m_pConfig->m_strScoreVar);
		pDefine->GetAttrib(XEAB::SUBMIT_URL,m_pConfig->m_strScoreURL);
		if(pDefine->GetAttrib(XEAB::SCORE_TYPE,v))
		   m_pConfig->m_nScoreType=(XU8)v;
		XString8 str;
		if(pDefine->GetAttrib(XEAB::AUTOTAG,str))
		{
			XString8Array list;
			list.SplitStringS(str,"@, -:");
			if(list.GetSize()>0)
				m_pConfig->m_nAutoTag=list[0].ToInt();
			if(list.GetSize()>1)
				m_pConfig->m_nTagPlace=list[1].ToInt();
		}
	}
	pDefine=pRoot->FindByIndex(0,id,XDOM_TXTDEFINE);
	if(pDefine)
	{
		m_strHelp=((XHTMLScript*)pDefine)->m_strTxt;

		//m_nCharset=m_pCfgDom->m_nCha
	}


	XDomList list;

	pRoot->GetList(list,XDOM_ACC);
	for(i=0;i<list.GetSize();i++)
	{
		XDomItem*p=list[i];
		_ACCMAP map;
		map.nMapKey=0;
		map.nMapStep=0;
		map.nValue=5;
		map.nOnlyOne=0;
		map.nStatus=KLK_RELEASE;
		map.nMapDirect=_ACCMAP::X;
		if(!p->GetAttrib(XEAB::DIRECTION,v)) continue;
		map.nDirect=v;
		if(p->GetAttrib(XEAB::ACCSPEED,v))
			map.nValue=v;
		if(p->GetAttrib(XEAB::KEY_MAP_TO,v))
			map.nMapKey=v;
		if(p->GetAttrib(XEAB::STEP,v))
			map.nMapStep=v;
		if(p->GetAttrib(XEAB::MOUSE_ACT,v))
			map.nMapDirect=v;
		if(p->GetAttrib(XEAB::ONLYONE,v))
			map.nOnlyOne=v;
		m_pConfig->m_ACCMaps.Add(map);
	}
	list.RemoveAll();

	pRoot->GetList(list,XDOM_KEYMAP);
	for(i=0;i<list.GetSize();i++)
	{
		XDomItem*p=list[i];
		_XKEYMAP keyMap;
		keyMap.nPressTime=0;
		if(!p->GetAttrib(XEAB::KEY_PRESSED,v)) continue;
		keyMap.nKeyCode=(XU16)v;
		if(!p->GetAttrib(XEAB::KEY_MAP_TO,v)) continue;
		keyMap.nKeyMap=(XU16)v;
		if(!p->GetAttrib(XEAB::LONG_PRESS,v))
			keyMap.bLongPress=XFALSE;
		else
			keyMap.bLongPress=(XU8)v;
		if(!p->GetAttrib(XEAB::LONG_PRESS_LOCK_TIME,v))
			keyMap.nLongLockTime=m_pConfig->m_nLongLockTime;
		else
			keyMap.nLongLockTime=(XU16)v;
		keyMap.bLock=0;
		m_pConfig->m_keyMaps.Add(keyMap);
	}
	list.RemoveAll();
	pRoot->GetList(list,XDOM_TOUCHMAP);
	for(i=0;i<list.GetSize();i++)
	{
		XDomItem*p=list[i];
		_XBUTTONMAP map;
		map.bLock=0;
		map.nOrg=0;
		map.pDib=XNULL;
		map.nHitPos=0;
		map.nShowTime=0;
		map.nPressTime=0;
		if(!p->GetAttrib(XEAB::BUTTON_NAME,v)) continue;
		map.nButtonID=(XU8)v;
		if(!p->GetAttrib(XEAB::BUTTON_MAP_TO,v)) continue;
		map.nKeyMap=(XU16) v;
		if(p->GetAttrib(XEAB::BUTTON_HOR_POS,v))
			map.nButtonPosX=(XS16)v;
		else
			map.nButtonPosX=0;
		if(p->GetAttrib(XEAB::BUTTON_VER_POS,v))
			map.nButtonPosY=(XS16)v;
		else
			map.nButtonPosY=0;
		if(p->GetAttrib(XEAB::BUTTON_LONG_PRESS,v))
			map.bLongPress=(XU8)v;
		else
			map.bLongPress=XFALSE;
		if(p->GetAttrib(XEAB::BUTTON_LONG_PRESS_LOCK_TIME,v))
			map.nLongLockTime=(XU16)v;
		else
			map.nLongLockTime=m_pConfig->m_nLongLockTime;
		if(p->GetAttrib(XEAB::ORG,v))
			map.nOrg=v;
		m_pConfig->m_buttonMaps.Add(map);
	}

	list.RemoveAll();
	pRoot->GetList(list,XDOM_MOUSENORMALDEFINE);
	for(i=0;i<list.GetSize();i++)
	{
		XU32 j;
		_XMOUSEMODE* mode=new _XMOUSEMODE;
		if(mode==XNULL) break;
		XDomItem*pItem=list[i];
		mode->nMode=_XMOUSEMODE::XNORMAL;
		pItem->GetAttrib(XEAB::NAME,mode->strName);

		XDomList maps;
		pItem->GetList(maps,XDOM_MOUSENORMALMAP);

		for(j=0;j<maps.GetSize();j++)
		{
			pItem=maps[j];
			_XMOUSEACT act;
			act.nTimes=0;
			if(!pItem->GetAttrib(XEAB::VIRTUAL_KEY,v)) continue;
			act.nKeyCode=(XU16)v;
			if(!pItem->GetAttrib(XEAB::MOUSE_ACT,v)) continue;
			act.nAct=(XU16)v;
			mode->acts.Add(act);
		}
		mode->acts.FreeExtra();

		m_pConfig->m_mouseModes.Add(mode);
	}

	list.RemoveAll();
	pRoot->GetList(list,XDOM_MOUSETABLEDEFINE);
	for(i=0;i<list.GetSize();i++)
	{
		XU32 j;
		_XMOUSEMODE* mode=new _XMOUSEMODE;
		if(mode==XNULL) break;
		XDomItem*pItem=list[i];
		mode->nMode=_XMOUSEMODE::XTABLE;
		pItem->GetAttrib(XEAB::NAME,mode->strName);

		XDomList maps;
		pItem->GetList(maps,XDOM_MOUSETABLEMAP);

		for(j=0;j<maps.GetSize();j++)
		{
			pItem=maps[j];
			_XMOUSEACT act;
			act.nTimes=0;
			if(!pItem->GetAttrib(XEAB::VIRTUAL_KEY,v)) continue;
			act.nKeyCode=(XU16)v;
			if(!pItem->GetAttrib(XEAB::MOUSE_ACT,v)) continue;
			act.nAct=(XU16)v;
			mode->acts.Add(act);
		}
		mode->acts.FreeExtra();

		m_pConfig->m_mouseModes.Add(mode);
	}

	XString8 strMode;
	if(pInfo&&m_pConfig->m_mouseModes.GetSize())
		pInfo->GetAttrib(XEAB::DEFAULT_OPERATE_MODE,strMode);

	m_pConfig->SetMouseMode(strMode,XTRUE);
	m_pConfig->FreeExtra();
	MakeModeMenu();

}

void XFlashView::LoadDefaultConfig()
{
	XString8 strTxt;
	if(!XResource::LoadText(strTxt,_strFlashConfig)) return;

	if(m_pCfgDom==XNULL)
		m_pCfgDom=new XDom;
	if(m_pCfgDom)
	{
		m_pCfgDom->Reset(XCHARSET_GB2312,XCNT::CTEXT,XCNT::TVND_WAP_WML,
						 &m_pDom->m_draw);
		m_pCfgDom->PhaseXML((XU8*)strTxt.GetData(),strTxt.GetSize(),false,true);
		PhaseConfig();

        
		/*if(nQuality)
		{
			XConfig*pConfig=m_player->GetConfig();
			pConfig->m_nQuality=nQuality-1;
		}*/
		m_player->UpdateConfig();
	}
}

//#ifdef __APPLE__
//static XMENUDATA _flashMenus[]=
//{
//	{XCMD_FLASH_SAVE,0,XResource::LoadWString(XIDS_SAVE),XNULL,0,0},	//0
//	{XCMD_FLASH_ORIG,0,XResource::LoadWString(XIDS_ORIG),XNULL,0,0},//1
//	{XCMD_FLASH_FIT,0,XResource::LoadWString(XIDS_FIT),XNULL,0,0},//2
//	{XCMD_FLASH_SCORE,0,XResource::LoadWString(XIDS_SCORE),XNULL,0,0},//3
//	{XCMD_FLASH_RESTART,0,XResource::LoadWString(XIDS_RESTART),XNULL,0,0},//4
//	{XCMD_FLASH_MODE,0,XResource::LoadWString(XIDS_MOUSEMODE),XNULL,0,XMS_POPUP},//5
//	{XCMD_FLASH_CLOSE,0,XResource::LoadWString(XIDS_ENDPLAY),XNULL,0,0},//5
//	//{0,0,XNULL,XNULL,0,XMS_SEPARATOR},
//};
//#else
//static XMENUDATA _flashMenus[]=
//{
//	{XCMD_FLASH_SAVE,0,XResource::LoadWString(XIDS_SAVE),XNULL,0,0},	//0
//	{XCMD_FLASH_ORIG,0,XResource::LoadWString(XIDS_ORIG),XNULL,0,0},//1
//	{XCMD_FLASH_FIT,0,XResource::LoadWString(XIDS_FIT),XNULL,0,0},//2
//	{XCMD_FLASH_SCORE,0,XResource::LoadWString(XIDS_SCORE),XNULL,0,0},//3
//	{XCMD_FLASH_RESTART,0,XResource::LoadWString(XIDS_RESTART),XNULL,0,0},//4
//	{XCMD_FLASH_MODE,0,XResource::LoadWString(XIDS_MOUSEMODE),XNULL,0,XMS_POPUP},//5
//	{XCMD_FLASH_CLOSE,0,XResource::LoadWString(XIDS_ENDPLAY),XNULL,0,0},//6
//	{XCMD_FLASH_FULLSCREEN,0,XResource::LoadWString(XIDS_FULLSCREEN),XNULL,0,0},//7
//	//{0,0,XNULL,XNULL,0,XMS_SEPARATOR},
//};
//#endif

//#ifdef __ANDROID__
//#else
//void XFlashView::AppendMenu(XMenu &menu)
//{
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//#else
//	//XBOOL bSet=XFALSE;
//	//menu.AddMenu(&_flashMenus[4]);
//	int nc=0;
//	if(m_bConfigOK)
//	{
//		XConfig*pConfig=m_player->GetConfig();
//		switch(pConfig->m_nZoomMode)
//		{
//		case 0:
//			/*if(m_player->NeedOriginal())
//			{
//				nc++;
//				menu.AddMenu(&_flashMenus[1]);
//				//bSet=XTRUE;
//			}
//			else*/
//			{
//				int w=m_player->GetStageWidth();
//				int h=m_player->GetStageHeight();
//				if(w<m_rect.Width()&&h<m_rect.Height())
//					menu.AddMenu(&_flashMenus[7]);
//			}
//			break;
//		case 2:
//		case 1:
//			nc++;
//			menu.AddMenu(&_flashMenus[2]);
//			break;
//		}
//		if(!pConfig->m_strScoreURL.IsEmpty())
//		{
//			XSWFCONTEXT cnt;
//			m_player->InitContext(cnt);
//			XXVar var;
//			XBOOL bOK=m_player->GetVariable(cnt,pConfig->m_strScoreVar,var,XFALSE);
//			m_player->ReleaseContext(cnt);
//			if(bOK)
//			{
//				nc++;
//				menu.AddMenu(&_flashMenus[3]);
//			}
//		}
//		if(m_modeMenu.GetSize())
//		{
//			_flashMenus[5].nData=(XU32)&m_modeMenu;
//			nc++;
//			menu.AddMenu(&_flashMenus[5]);
//			int i;
//			int nc=m_modeMenu.GetSize();
//			for(i=0;i<nc;i++)
//			{
//				XMENUDATA*p=m_modeMenu[i];
//				int id=p->nCommand-XCMD_FLASH_MODE;
//				p->nStyle=0;
//				int nms=pConfig->m_mouseModes.GetSize();
//				if(id<nms&&pConfig->m_mouseModes[id]==pConfig->m_pActiveMode)
//					p->nStyle=XMS_CHECKED;
//			}
//		}
//	}
//	if(nc>1)
//		menu.AddSeparator();
//	if(m_bNetwork&&m_bLoadOK)
//	{
//		//if(m_bLoadOK)
//		//	_flashMenus[0].nStyle&=~XMS_GRAY;
//		//else
//		//	_flashMenus[0].nStyle|=XMS_GRAY;
//		menu.AddMenu(&_flashMenus[0]);
//	}
//	menu.AddMenu(&_flashMenus[6]);
//#endif
//	//if(m_strHelp!=XNULL)
//
//	//	menu.AddMenu(&_flashMenus[0]);
//	//if(bSet)
//	//	menu.AddSeparator();
//}
//#endif


//DEL XBOOL XFlashView::CheckMenu(XPoint &pt)
//DEL {
//DEL 	XBOOL bIn=XFALSE;
//DEL
//DEL 	if(pt.y>m_rect.bottom-BARHEIGHT)
//DEL 	{
//DEL 		int lw=m_left.Width(),rw=m_right.Width();
//DEL 		bIn=pt.x<lw||pt.x>m_rect.right-rw;
//DEL 		if(m_nBarMode==XBAR_HIDE||m_nBarMode==XBAR_PTOUT)
//DEL 		{
//DEL 			int nh=XMAX(10,m_nBarPos);
//DEL 			if(pt.y>m_rect.bottom-nh)
//DEL 			{
//DEL 			//int sx=m_rect.Width()/3;
//DEL 				if(bIn)
//DEL 				{
//DEL 					m_nBarMode=XBAR_IN;
//DEL 					bIn=XTRUE;
//DEL 					//m_controlBar.nSelect=0;
//DEL 					SetCursor(XCUR_ARROW);
//DEL 				}
//DEL 			}
//DEL 		}
//DEL 		else if(m_nBarMode==XBAR_SHOW)
//DEL 		{
//DEL 			if(bIn)
//DEL 			{
//DEL 				m_nBarTime=MAXBARTIME;
//DEL 			//if(!(pt.x<lw||pt.x>m_rect.right-rw))
//DEL 			//	m_nBarMode=XBAR_PTOUT;
//DEL 				SetCursor(XCUR_ARROW);
//DEL 			}
//DEL 		}
//DEL 		//else if(bIn)
//DEL 		//	m_nBarMode=XBAR_SHOW;
//DEL 	}
//DEL 	/*if(!bIn&&m_nBarMode==XBAR_SHOW)
//DEL 	{
//DEL 		m_nBarTime=MAXBARTIME>>2;
//DEL 		m_nBarMode=XBAR_PTOUT;
//DEL 	}*/
//DEL 	return bIn;
//DEL }

XBOOL XFlashView::OnCommand(XU32 nCmd, XWindow *pWnd)
{
	switch(nCmd)
	{
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//
//	case XCMD_BUTTON_SWITCH:
//		 m_player->GetConfig()->m_bButtons=!m_player->GetConfig()->m_bButtons;
//		 //m_player->GetConfig()->m_bACCMap=!m_player->GetConfig()->m_bButtons;
//#ifdef _SYMBIAN
//#else
//		 m_bar.CheckItem(XCMD_BUTTON_SWITCH,!m_player->GetConfig()->m_bButtons);
//		 //m_bar.CheckItem(XCMD_UPDATE_SWITCH,!m_player->GetConfig()->m_bACCMap);
//#endif
//		 Invalidate();
//		 break;
//	case XCMD_UPDATE_SWITCH:
//		{
//			XConfig*pConfig=m_player->GetConfig();
//			pConfig->m_bACCMap=!m_player->GetConfig()->m_bACCMap;
//			int i,nCount=pConfig->m_ACCMaps.GetSize();
//			for(i=0;i<nCount;i++)
//			{
//				_ACCMAP&map=pConfig->m_ACCMaps[i];
//				if(map.nMapKey&&map.nStatus!=KLK_RELEASE)
//					m_player->OnKeyUp(map.nMapKey,map.nMapKey,0);
//			}
//			//m_player->GetConfig()->m_bButtons=!m_player->GetConfig()->m_bACCMap;
//			//m_bar.CheckItem(XCMD_BUTTON_SWITCH,!m_player->GetConfig()->m_bButtons);
//#ifdef _SYMBIAN
//#else
//			m_bar.CheckItem(XCMD_UPDATE_SWITCH,!m_player->GetConfig()->m_bACCMap);
//#endif
//		}
//		 break;
//#else
//	default:
//		 if(nCmd>=XCMD_FLASH_MODE)
//		 {
//			 int id=nCmd-XCMD_FLASH_MODE,nc;
//			 XConfig*pConfig=m_player->GetConfig();
//			 nc=pConfig->m_mouseModes.GetSize();
//			 if(id<nc)
//			 {
//				 pConfig->m_pActiveMode=pConfig->m_mouseModes[id];
//				 Invalidate();
//			 }
//		 }
//		 break;
//#endif
//	case XCMD_CLOSE_VIEWEX:
//		LOGWHERE();
//		 //if(m_bPause)
//		 if(m_pTxtView)
//		 {
//#ifdef __APPLE__
//             g_deviceData.m_isDomExtView = false;
//             g_deviceData.m_isDomView = false;
//             PostMsg(XWM_APPLE_MESSAGE, XCMD_DOMEXTTOFLASH, (XU32)0, XMS_TOPMOST); //主线程调用
//#endif
//			Pause();
//             
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////				m_bar.ShowWindow(XTRUE);
////#endif
//		 }
//		 break;
	case XCMD_FLASH_HELP:
		 //if(!m_pTxtView)
		 Pause();
		 break;
	case XCMD_FLASH_SAVE:
		 //if(!m_pTxtView)
		 DoSave();
		 return XTRUE;
//#ifdef __ANDROID__
//	case XCMD_FLASH_SAVE_CANCEL:
//		 if( m_pSave == NULL )
//			 break;
//		 m_pSave->Release();
//		 delete m_pSave;
//		 m_pSave = NULL;
//		 m_bPause=XFALSE;
//		 AddCommand(XRESUME,XNULL,0);
//		 break;
//#endif

	//case XCMD_FLASH_SCORE:
	//	 //BeginScore();
	//	 if(!m_pTxtView)
	//		AddCommand(XBEGINSCORE,0,0);
	//	 return XTRUE;
	case XCMD_FLASH_CLOSE:
		 //m_pDom->Back(XTRUE);
		 //m_pDom->ToDomView();
		 //if(m_pTxtView) break;
		 if(m_pDom->m_bHome)
		 {
			 PostMsg(XWM_COMMAND,XCMD_QUIT,0,XMS_TOPMOST);
			 break;
		 }
//#ifdef __ANDROID__
//#else
//		 if(m_pBox)
//			 delete m_pBox;
//#endif

		 if(!m_bPause)
		 {
			 m_bPause=XTRUE;
			 AddCommand(XPAUSE,0,0);
			 Invalidate();
		 }
//#ifdef __ANDROID__
//#else
//		 m_pBox=XMessageBox::Message(XResource::LoadWString(XIDS_QUITFLASH),
//									 this,XCMD_FLASH_CLOSE,XMB_YESNO|XMB_ICONQUESTION);
//#endif
		 //m_pDom->PostMsg(XWM_COMMAND,XCMD_FLASH_CLOSE,0,XMS_THIS);
		 return XTRUE;
//	case XCMD_FLASH_RESTART:
		 //m_player->Restart();
//#ifdef _WINEMU
//		if(m_pTxtView) break;
//#ifdef __ANDROID__
//#else
//		if(m_pBox)
//			delete m_pBox;
//		m_pBox=XMessageBox::Message(XResource::LoadWString(XIDS_RESETFLASH),
//									 this,XCMD_FLASH_RESTART,XMB_YESNO|XMB_ICONQUESTION);
//#endif
//#else
//		 if(!m_pTxtView)
//			 m_pDom->ReConnectEx(m_player->GetFilter());
//#endif
		 //AddCommand(XRESTART,0,0);
//		 return XTRUE;
	case XCMD_FLASH_FULLSCREEN:
		 //if(!m_pTxtView)
			AddCommand(XFULLSCREEN,0,0);
		 break;
	case XCMD_FLASH_FIT:
		 //if(!m_pTxtView)
			AddCommand(XFIT,0,0);
		 //break;
		 return XTRUE;
	case XCMD_FLASHSHOWMODE:
		 AddCommand(XDEVICE,0,0);
		 break;
	case XCMD_FLASH_ORIG:
		 //if(!m_pTxtView)
			AddCommand(XORG,0,0);
		 return XTRUE;
	case XCMD_HIDE_TOOLBAR:
		 //if(m_pTxtView) break;
		 if(m_nBarPos)
			m_nBarMode=XBAR_OUT;
		 else
			m_nBarMode=XBAR_IN;
		 break;
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	case XCMD_FLASH_MOVEMODE:
//		 if(m_player->m_nLockMode==XSWFPlayer::LK_NONE)
//			 m_player->m_nLockMode=XSWFPlayer::LK_SELECT;
//		 else
//			  m_player->m_nLockMode=XSWFPlayer::LK_NONE;
//#if defined(_SYMBIAN)
//#else
//		 m_bar.SetItemStatus(XCMD_FLASH_MOVEMODE,(m_player->m_nLockMode==XSWFPlayer::LK_OBJECT?XTS_ACTIVE:XTS_NORMAL));
//		 m_bar.CheckItem(XCMD_FLASH_MOVEMODE,m_player->m_nLockMode==XSWFPlayer::LK_SELECT);
//#endif
//		 break;
//	case XCMD_FLASH_ZOOMMODE:
//		 if(!m_pTxtView) //break;
//		{
//			XConfig*pConfig=m_player->GetConfig();
//			switch(pConfig->m_nZoomMode)
//			{
//			case 0:
//				/*if(m_player->NeedOriginal())
//				{
//					pConfig->m_nZoomMode=1;
//					break;
//				}*/
//				pConfig->m_nZoomMode=1;
//				//if(m_nCntMode==XCNT_MODE_NET)  //comment by hgl
//				//{
//#if defined(_SYMBIAN)
//#else
//					m_bar.SetItemStatus(XCMD_FLASH_MOVEMODE,XTS_NORMAL);
//					m_bar.CheckItem(XCMD_FLASH_MOVEMODE,m_player->m_nLockMode==XSWFPlayer::LK_SELECT);
//#endif
//				//}
//					//m_bar.SetItemStatus(XCMD_FLASH_MOVEMODE,XTS_NORMAL);
//				break;
//			case 1:
//				//if(m_nCntMode==XCNT_MODE_NET)  //comment by hgl
//				//{
//					pConfig->m_nZoomMode=0;
//#if defined(_SYMBIAN)
//#else
//					m_bar.SetItemStatus(XCMD_FLASH_MOVEMODE,XTS_DISABLE);
//#endif
//				//}
//				//else
////				{
////				int w=m_player->GetStageWidth();
////				int h=m_player->GetStageHeight();
////				if(w<m_rect.Width()&&h<m_rect.Height())
////					pConfig->m_nZoomMode=2;
////				else
////					pConfig->m_nZoomMode=0;
////				}break;
//			case 2:
//				pConfig->m_nZoomMode=0;
//				break;
//			}
//			AddCommand(XMOVE,0,0);
//
//		}break;
//#elif defined(__APPLE__) || defined(__ANDROID__)	// zhengjl 2012.04
//		case XCMD_FLASH_ZOOMMODE:
//		{
//			AddCommand(XMOVE, 0, 0);
//		}
//			break;
//#endif
		 //break;
	}
	return XWindow::OnCommand(nCmd,pWnd);
}

XBOOL XFlashView::GetButtonPos(XRect &rect, int i)
{
	//return XFALSE;
	if(!m_bConfigOK) return XFALSE;
	int pos[][8]=
	{
		{1,-1,1,-1,   1,-2,2,-1}, //0
		{1,-1,1,-1,	  1,-1,1,-1}, //1
		{0, 1,2, 1,   1,-1,1,-1}, //2
		{0, 0,1, 1,   1,-1,1,-1}, //3
		{0,0,0,0,0,0,0,0},//Undefine//4
		{0,0,0,0,0,0,0,0},//Undefine//4
		{0,0,0,0,0,0,0,0},//Undefine//4
		{0, 2,3, 1,   1,-1,1,-1},//5
		{0,0,0,0,0,0,0,0},		 //6
		{0, 1,2, 1,	  1,-2,2,-1} //7
	};
	//if(i>_XMOUSEMODE::UP) return XFALSE;
	//i=0;
	int ww=m_rect.right,hh=m_rect.bottom-BARHEIGHT;
	_XBUTTONMAP&map=m_player->GetConfig()->m_buttonMaps[i];
	int id=map.nButtonID;
	if(/*id>_XMOUSEMODE::UP||*/id<0)
		return XFALSE;


	if(map.pDib==XNULL) return XFALSE;
	//return XFALSE;


	int w=map.pDib->Width();
	int h=map.pDib->Height();
    
#ifdef __APPLE__
//    if( g_deviceData.mbOnSale || g_deviceData.m_scale < 1.1 )
//    {
//        w /= 2;
//        h /= 2;
//    }
#endif

	//int a=w;
	//return w;
	rect.left=ww*pos[id][0]+pos[id][1]*w;//+(ww-pos[id][2]*w)*pos[id][3]*map.nButtonPosX/100;
	rect.top=hh*pos[id][4]+pos[id][5]*h;//+(hh-pos[id][6]*h)*pos[id][7]*map.nButtonPosY/100;
	//rect.left=h;

	//return XFALSE;

	if(map.nButtonPosX)
	{
		switch(map.nOrg)
		{
		default:
		case _XBUTTONMAP::BOTTOMLEFT:
		case _XBUTTONMAP::TOPLEFT:
			rect.left=map.nButtonPosX;
			break;
		case _XBUTTONMAP::TOPRIGHT:
		case _XBUTTONMAP::BOTTOMRIGHT:
			rect.left=ww-w-map.nButtonPosX;
			break;
		}
	}
	if(map.nButtonPosY)
	{
		switch(map.nOrg)
		{
		default:
		case _XBUTTONMAP::TOPLEFT:
		case _XBUTTONMAP::TOPRIGHT:
			rect.top=map.nButtonPosY;
			break;
		case _XBUTTONMAP::BOTTOMLEFT:
		case _XBUTTONMAP::BOTTOMRIGHT:
			rect.top=hh-h-map.nButtonPosY;
			break;
		}
	}
	rect.right=rect.left+w;
	rect.bottom=rect.top+h;
	return XTRUE;
}



XBOOL XFlashView::MouseAct(XU16 c,XBOOL bPress)
{
	if(!m_bConfigOK) return XFALSE;
	XConfig*pConfig=m_player->GetConfig();
	if(pConfig->m_pActiveMode==XNULL) return XFALSE;
	_XMOUSEMODE*pMode=pConfig->m_pActiveMode;
	int i,nc=pMode->acts.GetSize();
	for(i=0;i<nc;i++)
	{
		_XMOUSEACT&act=pMode->acts[i];
		if(act.nKeyCode!=c) continue;

		if(pMode->nMode==_XMOUSEMODE::XTABLE)
		   //(c!=_XMOUSEMODE::A&&c!=_XMOUSEMODE::B))
		{
			if(bPress)
			{
				switch(act.nAct)
				{
				default:
					m_player->TabDirect(act.nAct);
					break;
				case _XMOUSEMODE::A:
				case _XMOUSEMODE::B:
					{
						XPoint pt(m_nMouseX,m_nMouseY);
						m_player->OnMouseDown(pt.x,pt.y,0);//OnMouseDown(pt);
						m_player->OnMouseUp(pt.x,pt.y,0);
						//OnMouseUp(pt);
					}
					break;
				}
			}
		}
		else
		{
			int nStep=pConfig->m_nMouseStep;
			if(act.nTimes>0)
				nStep+=XMIN(act.nTimes,10)*2;
			MouseAction(act.nAct,nStep,bPress);
			//XRect ri;
			//GetMouseRect(ri);
			//int dx=0,dy=0;
			//int nx=m_nMouseX,ny=m_nMouseY;
			/*XPoint pt(m_nMouseX,m_nMouseY);
			switch(act.nAct)
			{
			case _XMOUSEMODE::LEFT:
				 //if(bPress)
				 // dx=-nStep;
				 if(bPress)
				 {
					 if(pt.x-nStep>=0)
						 pt.x-=nStep;
					 else
					 {
						 XPoint delta(-nStep,0);
						 if(!m_player->MoveView(delta))
							 pt.x=0;
					 }
					 OnMouseMove(pt);
					 //m_nMouse
					 //m_player->OnMouseMove(pt.x,pt.y,m_bMouseDown);
				 }
				 break;
			case _XMOUSEMODE::RIGHT:
				 //if(bPress)
				 //	 dx=nStep;
				 if(bPress)
				 {
					 if(pt.x+nStep<m_rect.right)
						 pt.x+=nStep;
					 else
					 {
						 XPoint delta(nStep,0);
						 if(!m_player->MoveView(delta))
							 pt.x=m_rect.right-1;
					 }
					 OnMouseMove(pt);
				 }
				 break;
			case _XMOUSEMODE::UP:
				 //if(bPress)
				 if(bPress)
				 {
					 if(pt.y-nStep>=0)
						 pt.y-=nStep;
					 else
					 {
						 XPoint delta(0,-nStep);
						 if(!m_player->MoveView(delta))
							 pt.y=0;
					 }
					 OnMouseMove(pt);
				 }
				 break;
			case _XMOUSEMODE::DOWN:
				 if(bPress)
				 {
					 if(pt.y+nStep<m_rect.bottom)
						 pt.y+=nStep;
					 else
					 {
						 XPoint delta(0,nStep);
						 if(!m_player->MoveView(delta))
							 pt.y=m_rect.bottom-1;
					 }
					 OnMouseMove(pt);
				 }
				 break;
			case _XMOUSEMODE::A:
			case _XMOUSEMODE::B:
			case _XMOUSEMODE::LEFT_KEY:
				 if(bPress)
					 OnMouseDown(pt);
				 else
					 m_player->OnMouseUp(pt.x,pt.y);
					 OnMouseUp(pt);
				 break;
			case _XMOUSEMODE::RIGHT_KEY:
				 //if(bPress)

				 break;
			}*/
		}

		if(bPress)
			act.nTimes++;
		else
			act.nTimes=0;
	}
	return XFALSE;
}

void XFlashView::PaintCursor(XGraphics &g)
{
	if(!m_bConfigOK&&!IsMouseShow()) return;
#if defined(__TOUCHPAD__)||defined(__POINT__)
	if(!IsMouseShow())
		return;
#endif
#ifdef __APPLE__
	return;
#endif

#ifdef __ANDROID__
	return;
#endif

	XConfig*pConfig=m_player->GetConfig();
	_XMOUSEMODE*pMode=pConfig->m_pActiveMode;
	if(!IsMouseShow())
	{
		if(pMode==XNULL||pMode->nMode!=_XMOUSEMODE::XNORMAL) return;
	}


	int id=XConfig::XCUR_ARROW;
	switch(m_player->GetCursorType())
	{

	case CURSOR_TYPE_HAND:
		 id=XConfig::XCUR_MOVE;
		 break;
	default:
	case CURSOR_TYPE_ARROW:
		 id=XConfig::XCUR_ARROW;
		 break;
	case CURSOR_TYPE_BUTTON:
		 id=XConfig::XCUR_HAND;
		 break;
	case CURSOR_TYPE_BEAM:
		 //SetCursor(XCUR_INPUT);
		 id=XConfig::XCUR_INPUT;
		 break;
	case CURSOR_TYPE_HIDE:
		 return;
	}
	XRect rt;//;(,0,0,24);
	rt.left=XConfig::_nCurData[id][0];
	rt.top=0;
	rt.right=rt.left+XConfig::_nCurData[id][1];
	rt.bottom=pConfig->m_cursor.Height();
	XPoint dt;//(0,0);
	dt.x=m_nMouseX-XConfig::_nCurData[id][2];
	dt.y=m_nMouseY-XConfig::_nCurData[id][3];
	//XRect rc=rt;
	//rc&=m_clip;
	//if(!rc.IsRectEmpty())
	g.DrawImage(pConfig->m_cursor,dt,rt);
}

void XFlashView::GetMouseRect(XRect &rect)
{
	if(!m_bConfigOK) return;
	XConfig*pConfig=m_player->GetConfig();
	_XMOUSEMODE*pMode=pConfig->m_pActiveMode;
	if(pMode==XNULL||pMode->nMode!=_XMOUSEMODE::XNORMAL) return;

	int w=pConfig->m_cursor.Height();
	rect.left=m_nMouseX-w;
	rect.top=m_nMouseY-w;
	rect.right=rect.left+(w<<1);
	rect.bottom=rect.top+(w<<1);
	/*int id=XConfig::XCUR_ARROW;
	switch(m_player->GetCursorType())
	{

	case CURSOR_TYPE_HAND:
		 id=XConfig::XCUR_MOVE;
		 break;
	default:
	case CURSOR_TYPE_ARROW:
		 id=XConfig::XCUR_ARROW;
		 break;
	case CURSOR_TYPE_BUTTON:
		 id=XConfig::XCUR_HAND;
		 break;
	case CURSOR_TYPE_BEAM:
		 //SetCursor(XCUR_INPUT);
		 id=XConfig::XCUR_INPUT;
		 break;
	case CURSOR_TYPE_HIDE:
		 return;
	}
	rect.left=XConfig::_nCurData[id][0];
	rect.top=0;
	rect.right=rect.left+XConfig::_nCurData[id][1];
	rect.bottom=pConfig->m_cursor.Height();
	XPoint dt;//(0,0);
	dt.x=m_nMouseX-XConfig::_nCurData[id][2];
	dt.y=m_nMouseY-XConfig::_nCurData[id][3];
	rect+=dt; */
}

void XFlashView::MakeModeMenu()
{
	if(!m_bConfigOK) return;
	XConfig*pConfig=m_player->GetConfig();
	int i,nc=pConfig->m_mouseModes.GetSize();
	if(nc<2) return;
	m_modeMenu.FreeAll();
	for(i=0;i<nc;i++)
	{
		_XMOUSEMODE*pMode=pConfig->m_mouseModes[i];
		if(pMode->strName.IsEmpty()) continue;
		XMENUDATA*pMenu=new XMENUDATA;
		if(pMenu==XNULL) continue;
		pMenu->nCommand=XCMD_FLASH_MODE+i;
		pMenu->nData=0;
		pMenu->nIndex=0;
		pMenu->nStyle=0;
		pMenu->pImage=XNULL;
		pMode->strName16=pMode->strName;
		pMenu->strTitle=pMode->strName16;
		m_modeMenu.Add(pMenu);
	}
}


void XFlashView::DoSave()
{
	if(m_pSave)
		delete m_pSave;
	AddCommand(XPAUSE,XNULL,0);
	m_bPause=XTRUE;
	Invalidate();

	XString16 strFile;
	XString8  strPath=m_player->strURL.strTxt;

	XDialogSave::GetPrompt(strPath,strFile);
	XPCWSTR chFile = strFile.GetData();
	m_pSave=new XDialogSave(m_pDom->m_pClientApply, strPath, chFile, m_configData);
#ifdef __APPLE__
	m_pSave->_setParent(this);
#endif
#ifdef __ANDROID__
	m_pSave->_setParent(this);
#endif
	if(m_pSave)
	{
		m_pSave->DoModal(this);
	}
	return;
}

void XFlashView::Pause()
{
	//XLock lock(&m_runLock);          // modify by hgl
	if(!m_runLock.Lock(XSHORTTIME)) return;



	//AddCommand(m_bPause?XPAUSE:XRESUME,0,0);
	m_player->Pause(!m_bPause);
	m_bPause=!m_bPause;

	m_runLock.UnLock();

//	if(m_bPause)
//	{
//		if(m_strHelp)
//		{
//			m_pTxtView=new XDomViewExt(m_pDom->m_draw.pDraw,m_pDom->m_pClientApply);
//			if(m_pTxtView)
//			{
//                XRect rt(m_rect.left+3,m_rect.top+POPWIN_TITLE,m_rect.right-3,m_rect.bottom-10);
//#ifdef __APPLE__
////                if (!g_deviceData.mbOnSale)
////                {
////                    rt.right = m_rect.right/2-3;
////                    rt.bottom = m_rect.bottom/2-10;
////                }
//#endif
//			   m_pTxtView->Create(rt,this,m_strHelp,m_nHelpLength,XCHARSET_UTF8);
//			   //m_pTxtView->ShowWindow(
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////				m_bar.ShowWindow(XFALSE);
////#endif
//#ifdef __APPLE__
//                g_deviceData.m_isDomExtView = false;
//                g_deviceData.m_isDomView = false;
//                PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASHTODOMEXT, (XU32)0, XMS_TOPMOST); //主线程调用
//#endif
//			}
//		}
//	}
//	else if(m_pTxtView)
//	{
//		m_pTxtView->Stop( true, true );
//		DestoryChild(m_pTxtView);
//		AddCommand(XDESTROY_TEXT,0,0);
//#ifdef __APPLE__
//        PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_CLOSESUBVIEW, (XU32)this, XMS_TOPMOST);
//#endif
//	}
//
//	Invalidate();
}



void XFlashView::LockKeyButtons()
{
	if(!m_bConfigOK) return;
	XConfig*pConfig=m_player->GetConfig();
	int i,nc=pConfig->m_keyMaps.GetSize();
	XU32 nTime=XTime::GetTimeMSec();
	for(i=0;i<nc;i++)
	{
		_XKEYMAP&map=pConfig->m_keyMaps[i];
		if(map.bLock!=KLK_PRESSED||!map.bLongPress) continue;
		if(nTime-map.nPressTime<map.nLongLockTime) continue;
		map.bLock=KLK_LOCKED;
		//XRect rt;
		//GetButtonPos(rt,i);
	}
	//if(pConfig->m_pActiveMode==XNULL) return;
	nc=pConfig->m_buttonMaps.GetSize();//m_pActiveMode->acts.GetSize();
	for(i=0;i<nc;i++)
	{
		_XBUTTONMAP&map=pConfig->m_buttonMaps[i];
		if(map.bLock!=KLK_PRESSED||!map.bLongPress) continue;
		if(nTime-map.nPressTime<map.nLongLockTime) continue;
		map.bLock=KLK_LOCKED;
		XRect rt;
		if(GetButtonPos(rt,i))
			InvalidateRect(rt);
		//_MOUSEACT&act=pConfig->m_pActiveMode->acts[i];

	}
}



void XFlashView::GetWaitRect(XRect &rect)
{
//#ifdef __APPLE__
//#else
	XString16 str=XResource::LoadWString(XIDS_LOADING);
	int iw=(str.GetLength()+10)*(m_nTextHeight)+10;
	int ih=m_nTextHeight+8;
	if(m_pWaitImg)
	{
		iw=XMAX(iw,(int)m_pWaitImg->Width()+10);
		ih+=m_pWaitImg->Height()+5;
	}
	rect.left=(m_rect.Width()-iw)>>1;
	rect.top=(m_rect.Height()-ih)>>1;
	rect.right=(rect.left+iw);
	rect.bottom=(rect.top+ih);
//#endif
}

#ifdef _THREADBACK
XBOOL XFlashView::ProcData(XDATA *pInfo)
{
	int nSize=pInfo->nSize;

	XBOOL bSet=XTRUE,bInitOK=XFALSE;
	_XStreamData*pStream=pInfo->pStream;//data;
	if(pStream->scriptPlayer==XNULL) return XTRUE;

	pStream->nLoadedBytes=pInfo->nTotal;
	pStream->nTotalBytes=pInfo->nLoaded;
Again:
	int nType=m_player->StreamInWrite(pStream,pInfo->pData,nSize);
	XSize sz;
		//m_buffer,nSize,0);
	//XINT nType=m_player->AppendData(XNULL,m_buffer,nSize,bInitOK);
	//if(bInitOK)

	switch(nType)
	{
	case XSWFPlayer::LOAD_INITOK:
		 m_player->GetShowSize(sz);
		 SetSize(sz);
		 m_bInitOK=XTRUE;
		 //InitWindow();
		 break;
	case XSWFPlayer::LOAD_AGAIN:
		 //Sleep(XWAITMONMENT);
		 if(NeedStop())
			 return XFALSE;
		 else
		 {
			 m_thread.Sleep(XWAITMONMENT);
			 goto Again;
		 }
		 break;
	//case XObject::LOAD_INITOK:
	//	 InitWindow();
	//	 break;
	case XSWFPlayer::LOAD_FAILED:
		 m_player->Stop();
	case XSWFPlayer::LOAD_BREAK:
	case XSWFPlayer::LOAD_END:
		 bSet=XFALSE;break;
	case XSWFPlayer::LOAD_OK:
		 //m_hStop.SetEvent();
		 break;

		//if(!bSet) break;
	}
	return XTRUE;
}
#endif
void XFlashView::FreeData(XDATA *pData,XBOOL bSub)
{
	while(pData)
	{
		XDATA*pNext=pData->pNext;
		if(pData->pData)
			delete pData->pData;
		delete pData;
		pData=pNext;
		if(!bSub) break;
	}
}

XBOOL XFlashView::SetData()
{
	//if(!m_pData) return XTRUE;
	//XLock lock(&m_dataLock);
	//if(!lock.Lock(XWAITMONMENT)) return XTRUE;
	//XDATA*pData=m_pData;
	//m_pData=XNULL;
	//lock.UnLock();
	while(m_nCmdGet!=m_nCmdSet)//pData)
	{
		//XDATA*pNext=pData->pNext;
		switch(m_nCmds[m_nCmdGet])//pData->nCmd)
		{
		/*case XFOCUS:
			 m_player->Activate();
			 break;
		case XBLUR:
			 m_player->Deactivate();
			 break;*/
		case XSEEK:
			Seek(m_nCmds[m_nCmdGet+1],XTRUE);
			break;
//#ifdef _LOWSPEED
//		case XLOCKZOOMINOUT:
//			m_player->LockZoomBitmap();
//			break;
//		case XUNLOCKZOOMINOUT:
//			m_player->UnlockZoomBitmap(true);
//			break;
//		case XUPDATE:
//			m_player->display.Update();
//			break;
//#endif
		//case XAUTOOBJECT:
		//	{
		//		XSIZE sz=m_rect.Size();
		//		if(m_player->AutoObject(sz))
		//		{
		//			//XSize size(sz.cx,sz.cy);
		//			//SetSize(size);
		//			Invalidate();
		//		}
		//	}
		//	break;
//		case XZOOMINOUTC:
//			{
//				UPosXY data;
//				data.iData=m_nCmds[m_nCmdGet+2];
//				SPOINT ct={data.xy[0],data.xy[1]};
//				XSize sz=m_rect.Size();
//				sz.cy-=m_nBarPos;
//				//m_nPecent=m_nCmds[m_nCmdsGet+2];
//				int nPecent=m_nCmds[m_nCmdGet+1];
//#ifdef __APPLE__
//				PostMsg(XWM_APPLE_MESSAGE, nPecent?XCMD_FLASH_ZOOMOUTENABLE:XCMD_FLASH_ZOOMINENABLE, (XU32)this, XMS_TOPMOST);
//#endif
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////				m_bar.SetItemSlider(XCMD_FLASH_ZOOMINOUT,nPecent);
////				//if(m_nPecent)
////				m_bar.SetItemStatus(XCMD_FLASH_MOVEMODE,nPecent?XTS_NORMAL:XTS_DISABLE);
////				m_bar.CheckItem(XCMD_FLASH_MOVEMODE,XFALSE);
////#endif
//				//if(m_nMaxPecent!=100)
//				//	nPecent=nPecent*m_nMaxPecent/100;
//				m_nZoomPecent=nPecent;
//				//LOGWHEREVAL(nPecent);
//				this->m_player->SetZoomPecent(nPecent,sz,ct,true);
//
//				//SetSize(sz);
//				if(nPecent==0)
//					m_player->m_nLockMode=XSWFPlayer::LK_NONE;
//				m_nPecent=nPecent;
//				Invalidate();
//			}
//			 break;
		case XSTARTDRAG:
			m_player->StartZoom(m_dragRect);
			break;
		case XDRAGING:
			m_nZoomPecent=m_player->SetZoom(m_dragRect);
			PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_FLASHBAR,(XU32)this, XMS_TOPMOST);
			break;
		case XENDDRAG:
			m_player->EndZoom();
			break;
		//case XMOVECNT:
		//	{
		//		SPOINT ct={m_nCmds[m_nCmdGet+1],m_nCmds[m_nCmdGet+2]};
		//		m_player->NewCenter(ct);
		//	}break;
		//	 break;
		case XZOOMINOUT:
			{
				//LOGWHEREVAL(m_nZoomPecent);
				XSize sz=m_rect.Size();
				sz.cy-=m_nBarPos;
				SPOINT pt;
				pt.x=(m_rect.right+m_rect.left)/2;
				pt.y=(m_rect.top+m_rect.bottom)/2;
				//m_nPecent=m_nCmds[m_nCmdsGet+2];
				//this->m_player->SetZoomPecent(m_nCmds[m_nCmdGet+2],sz,pt,true);
				m_nPecent=m_nCmds[m_nCmdGet+2];
				float zoom = 1 + (float)m_nPecent / m_nCmds[m_nCmdGet+1];
				pt.x = m_winSize.cx / 2;
				pt.y = m_winSize.cy / 2;
				m_player->SetZoomPecent( zoom, pt );
				//if(m_nMaxPecent!=100)
				//	m_nPecent=m_nPecent*m_nMaxPecent/100;
				m_nZoomPecent=m_nPecent;
				//SetSize(sz);
				//if(m_nPecent==0)
				//	m_player->m_nLockMode=XSWFPlayer::LK_NONE;
				PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_FLASHBAR,(XU32)this, XMS_TOPMOST);
				Invalidate();
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//				m_bar.SetItemStatus(XCMD_FLASH_MOVEMODE,m_nCmds[m_nCmdGet+2]>0?XTS_NORMAL:XTS_DISABLE);
//#endif
			}
			//break;
			break;
		case XMOVE_VIEW:
			{
				XPoint pt((int)m_nCmds[m_nCmdGet+1],(int)m_nCmds[m_nCmdGet+2]);
				if(pt.x||pt.y)
					m_player->MoveView(pt);
			}break;
		//case XBEGINSCORE:
		//	 //BeginScore(m_nCmds[m_nCmdGet+1]);
		//	 break;
		case XPUSHSCORE:
			 PushScore();
			 break;
		//case XDESTROY_TEXT:
		//	if(m_pTxtView)
		//	{
		//		delete m_pTxtView;
		//		m_pTxtView=XNULL;
		//		Invalidate();
		//	}break;
		case XUPDATEBUTTONS:
			PaintButtons();
			break;
		case XMOVE:
			m_nZoomPecent = 0;
			m_player->SetCamera(true);
			PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_FLASHBAR,(XU32)this, XMS_TOPMOST);
			
//			if (m_nPecent )
//			{
//				XSize sz=m_rect.Size();
//				sz.cy-=m_nBarPos;
//				m_nBarTime=MAXBARTIME;
//				SPOINT pt={0,0};
//				m_player->SetZoomPecent(m_nPecent,sz,pt,false);
//				//SetSize(sz);
//				//Invalidate();
//			}
//			else
//			{
//				//RasterGL::sharedRasterGL()->setOrthoProjection( m_winSize.cx, m_winSize.cy );
//				//RasterGL::sharedRasterGL()->setViewPort( 0, 0, m_winSize.cx, m_winSize.cy );
//				m_player->SetCamera( 0 );
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////				m_bar.SetItemStatus(XCMD_FLASH_ZOOMMODE,IsEnableZoom()?XTS_NORMAL:XTS_DISABLE);
////#endif
//				Invalidate();
//			}
			break;

		//case XFULLSCREEN:
		//	 m_player->GetConfig()->m_nZoomMode=2;
		//	 //m_player->SetZoom(3,
		//	 {
		//		XSize sz=m_rect.Size();
		//		 if(m_player->SetZoom(4,sz))
		//		   {
		//			   //SetSize(sz);
		//			   Invalidate();
		//		   }
		//	 }
		//	 break;
		//case XORG:
		//	 m_player->GetConfig()->m_nZoomMode=1;
		//	 //m_player->SetZoom(3,
		//	 {
		//		XSize sz=m_rect.Size();
		//		 if(m_player->SetZoom(3,sz))
		//		   {
		//			   //SetSize(sz);
		//			   Invalidate();
		//		   }
		//	 }break;
		//case XDEVICE:
		//	{
		//		 XSize sz=m_rect.Size();
		//		 if(m_player->SetZoom(0,sz))
		//		   {
		//			   //etSize(sz);
		//			   Invalidate();
		//		   }
		//	}
		//	 break;
		//case XFIT:
		//	 m_player->GetConfig()->m_nZoomMode=0;

		// //m_player->SetZoom(3,
		//	 {
		//		 XSize sz=m_rect.Size();
		//		 if(m_player->SetZoom(0,sz))
		//		   {
		//			   //SetSize(sz);
		//			   Invalidate();
		//		   }
		//	 }break;
		case XRELOAD:
			if(!m_strReloadURL.IsEmpty())
			{
				XDOMFILTER*pFilter=m_player->GetFilter();
				XClientApply*pApply=m_pDom->GetClientApply();
				pApply->m_nDeltaX=m_player->m_nClickDeltaX;
				pApply->m_nDeltaY=m_player->m_nClickDeltaY;
				delete m_player;
				m_nFileLength=0;
				m_pFirstClient=NULL;
				m_nFirstLen=0;
				m_nLastLen=0;
				
				//XClientApply*pApply=m_pDom->GetClientApply();
				m_player=new XSWFPlayer(pApply->m_nDeltaX,pApply->m_nDeltaY);
				
				m_player->m_pDom=m_pDom->GetDom();

				m_player->m_nDomSize=this->m_pDom->GetDom()->m_nDataSize;
				m_player->m_argList.Append(m_pDom->GetDom()->m_argList);
				m_player->m_strWorkPath.SetString(m_pDom->GetWorkPath(),XString8::SafeStrlen(m_pDom->GetWorkPath()));
				m_player->SetFilePath(m_pDom->GetClientApply()->m_strFilePath);
				m_player->InitCachePath();
				m_player->m_pClientApply = m_pDom->m_pClientApply;
				m_player->m_strWorkPath.StringAdd("/");
				m_player->InitPlayer(m_winSize,this);
				m_player->SetFilter(pFilter);
				FreeInfo();
				m_pDom->ReConnect(m_strReloadURL);
				m_strReloadURL.Empty();
				Invalidate();			
			}break;
		case XRESTART:
			 //m_player->Restart();
				// case XCMD_FLASH_RESTART:
			 {
				 
				XDOMFILTER*pFilter=m_player->GetFilter();
				XClientApply*pApply=m_pDom->GetClientApply();
				pApply->m_nDeltaX=m_player->m_nClickDeltaX;
				pApply->m_nDeltaY=m_player->m_nClickDeltaY;
				delete m_player;
				m_nFileLength=0;
				m_pFirstClient=NULL;
				m_nFirstLen=0;
				m_nLastLen=0;
				
				//XClientApply*pApply=m_pDom->GetClientApply();
				m_player=new XSWFPlayer(pApply->m_nDeltaX,pApply->m_nDeltaY);
				
				m_player->m_pDom=m_pDom->GetDom();

				m_player->m_nDomSize=this->m_pDom->GetDom()->m_nDataSize;
				m_player->m_argList.Append(m_pDom->GetDom()->m_argList);
				m_player->m_strWorkPath.SetString(m_pDom->GetWorkPath(),XString8::SafeStrlen(m_pDom->GetWorkPath()));
				m_player->SetFilePath(m_pDom->GetClientApply()->m_strFilePath);
				m_player->InitCachePath();
				m_player->m_pClientApply = m_pDom->m_pClientApply;
                 m_player->m_pClientApply = m_pDom->m_pClientApply;
                 m_player->m_strWorkPath.StringAdd("/");
				m_player->InitPlayer(m_winSize,this);
				m_player->SetFilter(pFilter);

				FreeInfo();
				/////////////////////////////////
				//RasterGL::sharedRasterGL()->Uninit();
				//RasterGL::sharedRasterGL()->Init();
				m_pDom->ReConnect(pFilter);
				Invalidate();
			 }
			 break;
		case XRESUME:
			 m_player->Pause(XFALSE);
			 break;
		case XPAUSE:
			 m_player->Pause(XTRUE);
			 break;
#ifdef _THREADBACK
		case XNEW_STREAM:
			{
				XString8Array list;
				RESPONSEINFO*pInfo=pData->pClient->GetResponseInfo();
				list.Add("contextType");//XEH()[XEH::CONTENT_TYPE]);
				list.Add(pInfo->type.m_strType);
				list.Append(pInfo->strHeaders);
				_XStreamData*pStream=pData->pStream;
				if(!m_bInitOK)
					m_bNetwork=pData->pClient->m_nProtoType<XAPP_FILE;
				pStream->nLoadedBytes=pInfo->nTotalSize;
				pStream->nTotalBytes=pInfo->nLength;
				m_player->StreamInNew(pStream,
				(char*)pData->pClient->GetURL(),(void*)pData->pClient->GetCallID(),
				list,
				pInfo->nErrorCode);

			}break;
		case XCLOSE_STREAM:
			{
			XBOOL bOK=pData->nType==XHTTP_STATUS_OK;
			_XStreamData*pStream=pData->pStream;//&pData->data;
			pStream->nLoadedBytes=pData->nTotal;//pInfo->nTotalSize;
			pStream->nTotalBytes=pData->nLoaded;//pInfo->nLength;
			m_player->StreamInDestroy(pStream,bOK);
			XGlobal::Memset(pStream,0,ICSIZE);
			m_bLoadOK=XTRUE;
			//
			}break;
		case XAPPEND_DATA:
			if(!ProcData(pData))
			{
				FreeData(pData,XTRUE);
				return XFALSE;
			}break;
#endif
		}
		//FreeData(pData,XFALSE);
		//pData=pNext;
		m_nCmdGet+=3;
		if(m_nCmdGet>=MAX_CMDS)
			m_nCmdGet-=MAX_CMDS;
	}
	return XTRUE;
}

bool XFlashView::CanRestart()
{
	XDOMFILTER*pFilter=m_player->GetFilter();
	if(!pFilter) return true;
	return !pFilter->strRefeshURL.IsEmpty();
}

void XFlashView::UpdateToolBar()
{

    
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//#ifdef _SYMBIAN
//#else
//	//m_bar.RemoveItem
//	m_bar.RemoveAllItems();
//
//	//switch(m_nCntMode)
//	//{
//	//case XCNT_MODE_VIDEO:
//	//	 break;
//	//case XCNT_MODE_NET:
//		 //m_bar.AddItem(XCMD_FLASH_ZOOMMODE,TICON_ZOOM,m_player->NeedOriginal()?XTS_NORMAL:XTS_DISABLE);
//		 m_bar.AddItem(XCMD_FLASH_MOVEMODE,TICON_ENUACC,m_player->NeedMove()?(m_player->m_nLockMode==XSWFPlayer::LK_OBJECT?XTS_ACTIVE:XTS_NORMAL):XTS_DISABLE);
//		 m_bar.AddItem(XCMD_FLASH_RESTART,TICON_UPDATE,XTS_NORMAL);
//		 m_bar.AddItem(XCMD_FLASH_ZOOMINOUT,TICON_SLIDER,this->IsEnableZoom()?XTS_NORMAL:XTS_DISABLE);
//		 //if(this->IsEnableZoom())
//		 m_bar.AddItem(XCMD_FLASH_POINTZOOM,TICON_BOOKMARK,XTS_NORMAL);
//		 m_bar.AddItem(XCMD_FLASH_PAY,TICON_ZOOM,(m_player->m_pDomFilter&&
//									  !m_player->m_pDomFilter->strPayURL.IsEmpty())?XTS_NORMAL:XTS_DISABLE);
//		 m_bar.AddItem(XCMD_FLASH_SAVE,TICON_SAVE,XTS_DISABLE);
//		 
//		 XConfig*pConfig=m_player->GetConfig();
//		 if(pConfig->m_buttonMaps.GetSize())
//			 m_bar.AddItem(XCMD_BUTTON_SWITCH,TICON_ENUKEY,XTS_NORMAL);
//		 if(pConfig->m_ACCMaps.GetSize())
//		{
//			pConfig->m_bButtons=XFALSE;
//			m_bar.CheckItem(XCMD_BUTTON_SWITCH,XTRUE);
//			m_bar.AddItem(XCMD_UPDATE_SWITCH,TICON_ENUACC,XTS_NORMAL);
//		}
//		 //break;
//	//case XCNT_MODE_LOCAL:
//	//	{
//	//		XConfig*pConfig=m_player->GetConfig();
//	//		//if(m_bNetwork)
//	//		XBOOL bEnable=IsEnableZoom();
//	//		m_bar.AddItem(XCMD_FLASH_SAVE,TICON_SAVE,XTS_DISABLE);
//	//		//m_bNetwork?XTS_NORMAL:XTS_DISABLE);
//
//
//	//		m_bar.AddItem(XCMD_FLASH_ZOOMMODE,TICON_ZOOM,bEnable?XTS_NORMAL:XTS_DISABLE);
//	//		m_bar.AddItem(XCMD_FLASH_SCORE,TICON_SCORE,XTS_DISABLE);
//	//		m_bar.AddItem(XCMD_FLASH_HELP,TICON_PAUSE);
//
//	//		if(pConfig->m_buttonMaps.GetSize())
//	//			m_bar.AddItem(XCMD_BUTTON_SWITCH,TICON_ENUKEY,XTS_NORMAL);
//	//		if(pConfig->m_ACCMaps.GetSize())
//	//		{
//	//			pConfig->m_bButtons=XFALSE;
//	//			m_bar.CheckItem(XCMD_BUTTON_SWITCH,XTRUE);
//	//			m_bar.AddItem(XCMD_UPDATE_SWITCH,TICON_ENUACC,XTS_NORMAL);
//	//		}
//	//	}break;
//	//}
//	m_bar.AddItem(XCMD_FLASH_CLOSE,TICON_EXIT,XTS_NORMAL,XTP_RIGHT);
//#if defined(__TOUCHPAD__)||defined(__POINT__)
//	if(m_bNetwork)
//		m_bar.SetItemStatus(XCMD_FLASH_SAVE,m_bSaveReady?XTS_NORMAL:XTS_DISABLE);
//#endif
//	m_bar.UpdateToolBar();
//	m_bar.Invalidate();
//#endif
//#endif

#ifdef __APPLE__
    PostMsg(XWM_APPLE_MESSAGE, XCMD_UPDATE_BAR, (XU32)this,XMS_TOPMOST);
#endif

}

XBOOL XFlashView::CheckMenu(XPoint &pt)
{
//	if(m_nBarPos) 
//		return XFALSE;
//	if(m_left.IsEmpty()) 
//		return XFALSE;
//    
//#if defined (__APPLE__) && defined (__HDCANTSHOWCHATLIST)
//    return XFALSE;
//#endif
//	XRect rect(0,m_rect.bottom-m_left.Height(),m_left.Width(),m_rect.bottom);
//	if(rect.PtInRect(pt))
//	{
//		//m_nBarMode=XBAR_IN;
//		return XTRUE;
//	}
	return XFALSE;
}

XBOOL XFlashView::IsEnableZoom()
{
	XBOOL bEnable=XFALSE;
	switch(m_player->GetConfig()->m_nZoomMode)
	{
	case 0:
			/*if(m_player->NeedOriginal())
				bEnable=XTRUE;
			else*/

			bEnable=XTRUE;  //modify by hgl
//			if(m_nCntMode==XCNT_MODE_NET)
//			{
//				int w=m_player->GetStageWidth();
//				int h=m_player->GetStageHeight();
//				if(w>m_rect.Width()||h>m_rect.Height())
//					bEnable=XTRUE;
//			}
//			else
//			{
//				int w=m_player->GetStageWidth();
//				int h=m_player->GetStageHeight();
//				if(w<m_rect.Width()&&h<m_rect.Height())
//					bEnable=XTRUE;
//			}
			break;
		case 2:
		case 1:
			bEnable=XTRUE;
			break;
	}
	return bEnable;
}



void XFlashView::OnACC(int x, int y, int z)
{
	if(!m_bConfigOK||m_bMouseDown||m_nACCDelay)
		return;
	//LOGE("%s, %d, %s, x=%d, y=%d, z=%d ", __FILE__, __LINE__, __FUNCTION__, x, y, z );
	XConfig*pConfig=m_player->GetConfig();
	if(!pConfig->m_bACCMap) return;
	int nSize=pConfig->m_ACCMaps.GetSize();
	bool bMouse=false;
	int nMouseX=m_rect.Width()>>1,nMouseY=m_rect.Height()>>1;
	for(int i=0;i<nSize;i++)
	{
		_ACCMAP&map=pConfig->m_ACCMaps[i];
		if(!map.nValue) continue;
		if(map.nMapDirect==_XMOUSEMODE::POSX||
		   map.nMapDirect==_XMOUSEMODE::POSY)
		{
			int nx=m_rect.Width()>>1,
				ny=m_rect.Height()>>1,delta=0;
			switch(map.nDirect)
			{
			case _ACCMAP::RX:
			case _ACCMAP::X:
				 delta=x;break;
			case _ACCMAP::Y:
			case _ACCMAP::RY:
				 delta=y;break;
			case _ACCMAP::Z:
			case _ACCMAP::RZ:
				 delta=z;
				 break;
			}
			//map.nValue*=100;
			if(map.nMapDirect==_XMOUSEMODE::POSX)
			{
				nx+=delta*map.nMapStep*nx/100/map.nValue;
				nMouseX=nx;
			}
			else
			{
				ny+=delta*map.nMapStep*ny/100/map.nValue;
				nMouseY=ny;
			}
			bMouse=true;
			/*if(nx!=m_nMouseX||ny!=m_nMouseY)
			{
				XPoint pt(nx,ny);
				OnMouseMove(pt);
			}*/
			continue;
		}
		XBOOL bMap=XFALSE;
		int nDelta=1;
		switch(map.nDirect)
		{
		case _ACCMAP::X:
			 if(x>=map.nValue)
			 {
				 bMap=1;
				 nDelta=x/map.nValue;
			 }
			 break;
		case _ACCMAP::RX:
			 if(x<=-map.nValue)
			 {
				 bMap=1;
				 nDelta=-x/map.nValue;
			 }break;
		case _ACCMAP::Y:
			 if(y>=map.nValue)
			 {
				 bMap=1;
				 nDelta=y/map.nValue;
			 }break;
		case _ACCMAP::RY:
			if(y<=-map.nValue)
			{
				bMap=1;
				nDelta=-y/map.nValue;
			}break;
		case _ACCMAP::Z:
			if(z>=map.nValue)
			{
				bMap=1;
				nDelta=z/map.nValue;
			}break;
		case _ACCMAP::RZ:
			if(z<=-map.nValue)
			{
				bMap=1;
				nDelta=-z/map.nValue;
			}
		}
		if(!bMap)
		{
			if(map.nStatus!=KLK_RELEASE)
			{
				map.nStatus=KLK_RELEASE;
				m_player->OnKeyUp(map.nMapKey,map.nMapKey,0);
			}
			continue;
		}
		if(map.nMapKey)
		{

			if(!map.nOnlyOne||map.nStatus==KLK_RELEASE)
			//||map.nStatus==KLK_RELEASE)
			{	m_player->OnKeyDown(map.nMapKey,map.nMapKey,0);
				map.nStatus=KLK_PRESSED;
			}
			else
			{
			    m_player->OnKeyUp(map.nMapKey,map.nMapKey,0);
			    //map.nStatus=KLK_PRESSED;
			}
			//m_player->OnKeyUp(map.nMapKey,0);
		}
		else
		{
			MouseAction(map.nMapDirect,map.nMapStep*nDelta,XTRUE);
			//m_player->OnMouseMove(m_nMouseX,m_nMouseY,m_bMouseDown);
		}
	}
	if(bMouse&&(nMouseX!=m_nMouseX||nMouseY!=m_nMouseY))
	{
		XPoint pt(nMouseX,nMouseY);
		OnMouseMove(pt,0);
	}
}

void XFlashView::MouseAction(int nAct, int nStep,XBOOL bPress)
{
	XPoint pt(m_nMouseX,m_nMouseY);
	switch(nAct)
	{
	case _XMOUSEMODE::LEFT:
		 //if(bPress)
		 // dx=-nStep;
		 if(bPress)
		 {
			 if(pt.x-nStep>=0)
				 pt.x-=nStep;
			 else
			 {
				 XPoint delta(-nStep,0);
				 //if(!m_player->MoveView(delta))
				 AddCommand(XMOVE_VIEW,-nStep,0);
				 pt.x=0;
			 }
			 if(pt.x!=m_nMouseX||pt.y!=m_nMouseY)
				OnMouseMove(pt,0);
			 //m_nMouse
			 //m_player->OnMouseMove(pt.x,pt.y,m_bMouseDown);
		 }
		 break;
	case _XMOUSEMODE::RIGHT:
		 //if(bPress)
		 //	 dx=nStep;
		 if(bPress)
		 {
			 if(pt.x+nStep<m_rect.right)
				 pt.x+=nStep;
			 else
			 {
				 XPoint delta(nStep,0);
				 //if(!m_player->MoveView(delta))
				 AddCommand(XMOVE_VIEW,nStep,0);
				 pt.x=m_rect.right-1;
			 }
			 if(pt.x!=m_nMouseX||pt.y!=m_nMouseY)
				OnMouseMove(pt,0);
		 }
		 break;
	case _XMOUSEMODE::UP:
		 //if(bPress)
		 if(bPress)
		 {
			 if(pt.y-nStep>=0)
				 pt.y-=nStep;
			 else
			 {
				 XPoint delta(0,-nStep);
				 //if(!m_player->MoveView(delta))
				 AddCommand(XMOVE_VIEW,0,-nStep);
				 pt.y=0;
			 }
			 if(pt.x!=m_nMouseX||pt.y!=m_nMouseY)
				OnMouseMove(pt,0);
		 }
		 break;
	case _XMOUSEMODE::DOWN:
		 if(bPress)
		 {
			 if(pt.y+nStep<m_rect.bottom)
				 pt.y+=nStep;
			 else
			 {
				 XPoint delta(0,nStep);
				 //if(!m_player->MoveView(delta))
				 AddCommand(XMOVE_VIEW,0,nStep);
				 pt.y=m_rect.bottom-1;
			 }
			 if(pt.x!=m_nMouseX||pt.y!=m_nMouseY)
				OnMouseMove(pt,0);
		 }
		 break;
	case _XMOUSEMODE::A:
	case _XMOUSEMODE::B:
	case _XMOUSEMODE::LEFT_KEY:
		 if(bPress)
			 OnMouseDown(pt,0);
		 else
			 m_player->OnMouseUp(pt.x,pt.y,0);
			 OnMouseUp(pt,0);
		 break;
	case _XMOUSEMODE::RIGHT_KEY:
		 //if(bPress)

		 break;
	}
}

XBOOL XFlashView::GetScore(XXVar &var)
{
	//XLock lock(&m_runLock);       // modify by hgl
	if(!m_runLock.Lock(XSHORTTIME))
		return XFALSE;

	XSWFCONTEXT cnt;
	m_player->InitContext(cnt);
	XBOOL bOK=m_player->GetVariable(cnt,m_player->GetConfig()->m_strScoreVar,var,XFALSE);
	m_player->ReleaseContext(cnt);

	if(bOK&&var.IsObject()&&var.pObject)
	{
		SObject*pObj=var.pObject->m_pObject;
		if(pObj&&pObj->character&&pObj->character->type==editTextChar)
		{
			//var.Release();
			pObj->editText->GetBuffer(var);
			//var.SetString(txt,XString8::SafeStrlen(txt));
			//var.nType=XCT_STRING;
			//var.strData=txt;
		}

	}
	m_runLock.UnLock();
	return bOK;
}

float XFlashView::getCurrentScore()
{
	XXVar var;
	if (!GetScore(var)) return 0;
	return var.ToFloat();
}

XU32 XFlashView::GetTotalTime()
{
	if(m_player==NULL) return 0;
	ScriptPlayer*p=m_player->player;
	int nRate=p->frameRate;
	int nFrames=p->numFrames;
	if (nFrames<0) {
		nFrames=0;
	}
	nRate>>=8;
	if(nRate<1) nRate=1;
	return (nFrames<<8)/nRate;
}

XU32 XFlashView::GetCanPlayTime()
{
	if(m_player==NULL) return 0;
	ScriptPlayer*p=m_player->player;
	int nRate=p->frameRate;
	int nFrames=p->numFrames<p->numFramesComplete?p->numFrames:p->numFramesComplete;
	if (nFrames<0) {
		nFrames=0;
	}
	nRate>>=8;
	if(nRate<1) nRate=1;
	return (nFrames<<8)/nRate;
}

XU32 XFlashView::GetCurrentTime1()
{
	if(m_player==NULL) return 0;
	ScriptPlayer*p=m_player->player;
	int nRate=p->frameRate;
	int nFrames=XMIN(p->curFrame,p->numFrames);
	nRate>>=8;
	if(nRate<1) nRate=1;
	return (nFrames<<8)/nRate;
}

void XFlashView::Seek(XU32 nTime,XBOOL bLastOP)
{
	if(m_player==NULL) return;
	ScriptPlayer*p=m_player->player;
	XSWFPlayer* splayer =p->splayer;
	int nRate=p->frameRate;
	nRate>>=8;
	int nCurFrame=(nTime*nRate)>>8;
//	p->Seek(nCurFrame);
	splayer->GotoFrame(nCurFrame);
	if(bLastOP)
	{
		splayer->display.Update();
		p->Play();
	}
	else
		p->StopPlay();
}

/*XBOOL XFlashView::GetCookies(XString8&strCookies)
{
	return m_pDom->GetCookies(strCookies);
}
XBOOL XFlashView::GetCookieOf(XString8&strName,XString8&strCookies)
{
	return m_pDom->GetCookieOf(strName,strCookies);
}*/
XBOOL XFlashView::GetVarOf(XString8&strName,XString8Array&strArgs,XString8&strValue)
{
	const char*pArg=NULL;
	if(strArgs.GetSize())
		pArg=strArgs[0];
	XBOOL bOK=m_pDom->SetValueOf(strName,pArg);
	if(bOK)
		strValue=strName;
	return bOK;
	//return m_pDom->GetVarOf(strName,strArgs,strValue,NULL,bFun);
}
XBOOL XFlashView::SetValueOf(XString8&strVar)
{
	return m_pDom->SetValueOf(strVar);
}
XBOOL XFlashView::SetVarOf(XString8&strVar)
{
	return m_pDom->SetVarOf(strVar);
}

#define MD5PACK 10
void XFlashView::MakeContentID(const char*score,XString8&strOut)
{
	if(m_strFlashFile.IsEmpty()) return;
	XU8Array data;
	data.Append((XU8*)m_strFlashFile.GetData(),m_strFlashFile.GetLength());
	data.Append((XU8*)firstBytes,10);
	data.Append((XU8*)lastBytes,10);
	XU8 v=(XU8)(m_nFileLength&0xff);
	data.Add(v);
	v=(XU8)((m_nFileLength>>8)&0xff);
	data.Add(v);
	v=(XU8)((m_nFileLength>>16)&0xff);
	data.Add(v);
	v=(XU8)((m_nFileLength>>24)&0xff);
	data.Add(v);
	//data.Append((XU8*)&m_nFileLength,4);
	data.Append((XU8*)"jqbar",5);
	if(score)
	{
		data.Append((XU8*)score,XString8::SafeStrlen(score));
	}
	XDomView::GetMD5Of(data,strOut);
	strOut[16]=0;
}
void XFlashView::SaveBytes(void*pData,int len,bool bFirst,XClient*pClient)
{
	if(m_pFirstClient==NULL)
	{
		m_pFirstClient=pClient;
		XString8 strURL=pClient->GetConnectInfo()->URL.m_strURL;
		XURL::Decode(strURL);
		XFileFinder::GetFileName(strURL,m_strFlashFile,XFALSE);//XTRUE);
		if(pClient->GetConnectInfo()->URL.m_nProto==XPROTO_FILE)
		{
			XString8 str;
			str.ConvertFrom(m_strFlashFile,XCHARSET_GB2312);
			m_strFlashFile=str;
		}
	}
	else if(m_pFirstClient!=pClient)
		return;



	if(m_nFirstLen<MD5PACK)
	{
		int cLen=MD5PACK-m_nFirstLen;
		if(cLen>len) cLen=len;
		memcpy(firstBytes+m_nFirstLen,pData,cLen);
		m_nFirstLen+=cLen;
	}
	if(len>=MD5PACK)
	{
		memcpy(lastBytes,(char*)pData+len-10,10);
		m_nLastLen=MD5PACK;
	}
	else if(!m_nLastLen)
	{
		memcpy(lastBytes,pData,len);
		m_nLastLen=len;
	}
	else if(m_nFirstLen+len<=MD5PACK)
	{
		memcpy(lastBytes+m_nFirstLen,pData,len);
		m_nFirstLen+=len;
	}
	else
	{
		int i,nCount=MD5PACK-len;
		int nOff=m_nFirstLen-nCount;
		for(i=0;i<nCount;i++)
			lastBytes[i]=lastBytes[i+nOff];
		memcpy(lastBytes+nCount,pData,len);
		m_nFirstLen=MD5PACK;
	}
	m_nFileLength+=len;
}

XBOOL XFlashView::IsMouseShow()
{
	if(m_player->m_pDomFilter&&
	   m_player->m_pDomFilter->bMouseShow) return XTRUE;
	return m_player->GetConfig()->m_bMouseShow;
}
void XFlashView::CloseFlash()
{
	m_player->Interrupt();
	m_player->Stop();
	m_pDom->PostMsg(XWM_COMMAND,XCMD_FLASH_CLOSE,0,XMS_THIS);
}

int XFlashView::GetZoomPecent()
{
	return m_nZoomPecent;
}

XBOOL XFlashView::CanPay()
{
	XBOOL bre = m_player->m_pDomFilter && !m_player->m_pDomFilter->strPayURL.IsEmpty();
	return bre;
}

//if m_bPause = true, window is opened, else m_bPause =false, window is close.
XBOOL XFlashView::IsPause()
{
	return m_bPause;
}

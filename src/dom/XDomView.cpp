// XDomView.cpp: implementation of the XDomView class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomView.h"
#include "XDomItem.h"
#include "XDomText.h"
#include "XDomPhase.h"
#include "XDomPhaseImage.h"
#include "XContentType.h"
#include "XMainWnd.h"
#include "XWAPText.h"
#include "XFlashView.h"
#include "XSystem.h"
#include "XHTMLScript.h"
#include "WindowView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _strCloseView XResource::LoadWString(XIDS_CLOSEWINDOW)
//#undefine _WINEMU

#define _XBACKIMAGEID_  0x80000001

#ifdef __ANDROID__
#include "GlobalFun.h"
#include <unistd.h>
static XClient *mmpclient=NULL;
#endif

//#define SHOWPROGRESS

#ifdef __APPLE__
	#include "GlobalPortingFunction.h"
	#include "IPhoneDeviceData.h"
	#include "XBWSendMessageContent.h"
	BWSendMessageContentClass *g_BaiWanWeBBUrlSavedData = NULL;
	BWSendMessageContentClass *g_BaiWanIAPUrlSavedData = NULL;
	extern BWSendMessageContentClass * g_BaiWanPayUrlSavedData;
	extern iOSDeviceRelateData g_deviceData;
#endif


static XMENUDATA _viewMenus[]=
{
	{XCMD_VIEW_FORWARD,0,XResource::LoadWString(XIDS_FORWARD),XNULL,0,0},
	//{XCMD_VIEW_CLOSE,0,XResource::LoadWString(XIDS_CLOSE),XNULL,0,0},
	//{0,0,XNULL,XNULL,0,XMS_SEPARATOR},
};

//bool XDomView::mbLowNetSpeed = false;
//XImage* XDomView::m_pImgLowNetTip = NULL;


XDomView::XDomView(XGraphics*pDraw,XClientApply*pa)
{
   //m_pFilter=NULL;
   m_nLockClick=0;
   //m_pImgGameHelp=NULL;
    //if ( m_pImgLowNetTip == NULL )
    //{
    //    m_pImgLowNetTip = GetIcon( XICON_NETTIP );
    //}
  
   m_bShowLoading=XFALSE;
   m_bShowPageLoading = false;
   //m_iGamehelpIndex=0;
   m_WaitingIndex = 0;
   m_iPageLoadingIndex = 0;

   m_bShowEnd=XFALSE;
   m_bException=XFALSE;
   m_nScrollX=0;
   m_nScrollY=0;
   m_pDom=new XDom;
   m_bShowView=XTRUE;
   m_bBackList=XTRUE;
   m_nLastClick=0;
#ifdef __ANDROID__
#else
   m_pBox=XNULL;
#endif
   m_bCapture=XFALSE;
//   m_bNeedDown=XFALSE;
   m_draw.pDraw=pDraw;
   m_draw.pContext=XNULL;
   m_pClientApply=pa;
   m_pClient=XNULL;
   m_bStopreq=XFALSE;
   m_nStatus=0;
   m_pFlashView=XNULL;
   m_bMouseDown=XFALSE;
  // Release(XFALSE);
   m_bHome=XTRUE;
   m_bPushed=XFALSE;
   m_bLoaded=XFALSE;
   m_nCmdSet=m_nCmdGet=0;
#ifdef _SCROLLVIEW
#else
   m_nOffX=0;
   m_nOffY=0;
   m_nMaxX=0;
   m_nMaxY=0;
   m_bScroll=XFALSE;
   m_nSpeedX=0;
   m_nSpeedY=0;
#endif
#ifdef _SYMBIAN
	m_hSaveDialog = XNULL;
#endif
    m_nPecent = 5;
//#if (__CORE_VERSION__>=0x02076000)
   m_pLoadingBG=NULL;
   m_nLoadingBGIndex=0;
//#endif

   m_pImgWaitting=GetIcon(XICON_LOADING);
   m_pImgPageLoading = GetIcon( XICON_PAGELOADING );

}

XDomView::~XDomView()
{
#ifdef __ANDROID__
#else
   if(m_pBox != XNULL )
   {
	   delete m_pBox;
	   m_pBox=XNULL;
   }
#endif
   Release(XTRUE);
   if(m_pDom != XNULL )
   {
	   delete m_pDom;
	   m_pDom = XNULL;
   }
}

XBOOL XDomView::Create(XRect &rect,XWindow *pWnd,XU32 nCmd)
{
	//XBOOL bOK=XWindow::Create(rect,XES_FOCUS,pWnd,nCmd);
	XImage* p =	GetIcon(XICON_GREENBLOCK);
	int nh = XMAX((int)p->Height(), (int)m_nTextHeight + 4);
//	int nh=22;
	m_rect=rect;
	m_view=rect;
	m_view-=rect.TopLeft();

#ifdef __APPLE__
#elif defined(_SYMBIAN)
#elif defined(__ANDROID__ )
//	m_view.bottom -= GFunGetBrowerHeight();
#else
	m_view.bottom-=nh;
	XRect rectview(0,rect.Height()-nh,rect.right,rect.Height());
	m_status.Create(rectview,this);
#endif

#ifdef _SCROLLVIEW
	m_view.right-=XSSIZE;
	m_vBar.Create(XRect(m_view.right,0,m_rect.Width(),m_view.bottom),this);
	//int y=m_rect.Height()-XSTATUSH-XSHSIZE;
	m_hBar.Create(XRect(0,m_view.bottom - nh,m_view.right+1,m_view.bottom),this);
#endif
	//m_docSize=XSize(m_view.right,m_view.bottom+m_nTextHeight);
	SetDocSize(m_view.right,m_view.bottom+m_nTextHeight);
	InitContext();
	return XWindow::Create(rect,XES_FOCUS,pWnd,nCmd);
}

XU32 XDomView::DrawDoc(XRect&clip,XGraphics&g)
{
	int mx,my;
	g.GetOrigin(mx,my);
	GetOffset(m_draw.offset);
	g.SetOrigin(mx-m_draw.offset.x,my-m_draw.offset.y);
	XRect r=clip;
	r+=m_draw.offset;
	g.SetClip(r);

	//XGraphics* temp_g = m_draw.pDraw;
	m_draw.pContext=m_draw.pDraw;
	m_draw.pDraw=&g;



	//g.SetBackColor(m_draw.DCBACK);
	//g.FillRectangle(r);
	//g.SetColor(m_draw.DCTEXT);

	//GetViewRect(m_draw.view);
	//GetClipRect(m_draw.view,XTRUE);
	GetClipRect(m_draw.paint,XFALSE);

	//int mx,my;
	//g.GetOrigin(mx,my);
	//g.SetOrigin(mx-m_draw.view.left,my-m_draw.view.top,XTRUE);

	XU32 s=m_pDom->Paint(m_draw,m_req,XTRUE,m_bShowView);//m_nStyle&XES_SETFOCUS);//Handle(XDO_PAINT,(XU32)&draw,(XU32)m_pItem)

	XBOOL bFocus=m_nStyle&XES_SETFOCUS;
	for(XBOOL bPopup=0;bPopup<=1;bPopup++)
	{
	  for(XU32 i=0;i<m_popList.GetSize();i++)
	  {
		 m_popList[i]->ClearMessage();
		 if((m_popList[i]->IsPopup()&&!bPopup)||
			(!m_popList[i]->IsPopup()&&bPopup))
			 continue;
		 XU8 s=m_popList[i]->GetStyle();
		 //XRect rp=m_popList[i]->GetWindowRect();
		 r=m_view;//m_clip;
		 r+=m_draw.offset;
		 r-=m_popList[i]->GetWindowRect().TopLeft();

		 //r&=rp;
		 //m_popList[i]->GetWindowRect()=r;
		 //int by=m_popList[i]->GetWindowRect().top-m_draw.offset.y;

		 m_popList[i]->GetClipRect()&=r;
		 if(!bFocus)
			 m_popList[i]->SetFocus(XFALSE);
		 m_popList[i]->Paint(g);
		 //m_popList[i]->GetWindowRect()=rp;
		 m_popList[i]->SetStyle(s);
	  }
	}
	m_popList.RemoveAll();

	g.SetOrigin(mx,my);
	g.SetClip(m_rect);
	//m_draw.pDraw = temp_g;
	m_draw.pDraw=m_draw.pContext;
	return s;
}

void XDomView::PaintLowNetSpeedTip(XGraphics&g)
{
	//XPoint pt(m_view.Width()/2-m_pImgLowNetTip->Width()/2, m_view.Height() / 2 - m_pImgLowNetTip->Height() / 2);
	//g.DrawImage(*m_pImgLowNetTip,pt,0);
}


void XDomView::PaintPageLoading( XGraphics&g )
{
	m_pImgPageLoading->SetActive( m_iPageLoadingIndex );
	XPoint pt(m_view.Width()/2-m_pImgPageLoading->Width()/2, m_view.Height() / 2 - m_pImgPageLoading->Height() / 2);
	g.DrawImage(*m_pImgPageLoading,pt,0);
}

XBOOL XDomView::OnPaint(XGraphics &g)
{
	//return XTRUE;
	if(m_bShowLoading)
	{
		PaintLoading(g);
		//if ( mbLowNetSpeed )
		//{
		//	PaintLowNetSpeedTip(g);
		//}
		return XTRUE;
	}
	if(m_pFlashView)
		return XTRUE;

	if(!m_ptLock.Lock(XLONGTIME))
		return XFALSE;
	XU32 s=0;

#ifdef _SYMBIAN
	int nh = 0;
#elif defined( __ANDROID__ )
	int nh = 0;
#else
	int nh=m_status.GetWindowRect().Height();
#endif
	if(m_view.bottom<m_rect.Height()-nh&&m_view.right<m_rect.Width())
	{
		XColor c(XWindow::m_nColors[XCW_BUTTON]);
		g.SetBackColor(c);
		XRect r(m_view.right,m_view.bottom,m_view.right+XSSIZE,m_view.bottom+XSSIZE);
		g.FillRectangle(r);
		c.Bright(40);
		g.SetColor(c);
		g.FrameRectangle(r);
	}

	//DWORD t1=GetTickCount();

	m_clip&=m_view;
	if(m_clip.IsRectEmpty())
	{
		m_ptLock.UnLock();
		return XTRUE;
	}
	XRect clip=m_clip;
	s|=DrawDoc(clip,g);
	m_ptLock.UnLock();

	//DWORD t2=GetTickCount();
	//t2-=t1;
	ProcResult(s);
	//if ( mbLowNetSpeed )
	//{
	//	PaintLowNetSpeedTip(g);
	//}
	if( m_bShowPageLoading )
		PaintPageLoading( g );
	return XTRUE;
	//g.SetClip(m_clip);
}

XDom* XDomView::RefDom(XDom*pDom)
{
	if(pDom==XNULL) return XNULL;
	pDom->m_nRefs++;
	return pDom;
}

void XDomView::FreeDom(XDom*pDom)
{
	if(!pDom) return;
	if(pDom->m_nRefs>1)
		pDom->m_nRefs--;
	else
		delete pDom;
}

XBOOL XDomView::AddCommand(XU32 nCmd,XU32 nType,XU32 nData)
{
	//if(nType==XCMD_DOM_VIEW)
	//	int vv=0;
	int vi=m_nCmdSet+3;
	if(vi>=MAX_CMDS)
		vi-=MAX_CMDS;
	if(vi==m_nCmdGet) return XFALSE;
	m_nCmds[m_nCmdSet]=nCmd;
	m_nCmds[m_nCmdSet+1]=nType;
	m_nCmds[m_nCmdSet+2]=nData;
	m_nCmdSet=vi;
	return XTRUE;
}

void XDomView::SetDocSize(int cx, int cy)
{
   /*if(cy<=m_view.bottom)
   {
	   m_view.right=m_rect.Width();
	   if(m_vBar.IsVisiable())
	   {
		m_vBar.ShowWindow(XFALSE);
		m_hBar.MoveWindow(XRect(0,m_view.bottom,m_rect.Width(),m_view.bottom+XSSIZE));
		Layerout();


	   }
   }
   else
   {
	   m_view.right=m_rect.Width()-XSSIZE;
	   if(!m_vBar.IsVisiable())
	   {

		m_vBar.ShowWindow(XTRUE);
		m_hBar.MoveWindow(XRect(0,m_view.bottom,m_rect.Width()-XSSIZE,m_view.bottom+XSSIZE));
		Layerout();
	   }
   }*/
#ifdef _SCROLLVIEW
   int nh=m_status.GetWindowRect().Height();
   if(cx<=m_view.right)
   {
	   m_hBar.ShowWindow(XFALSE);
	   m_view.bottom=m_rect.Height()-nh;
	   m_vBar.MoveWindow(XRect(m_view.right,m_view.top,m_rect.Width(),m_view.bottom));
	   //m_vBar.SetRange(XMAX(cy-m_view.bottom,0),0);
   }
   else
   {
	   //if((cx>>1)>m_view.right)
	//	   m_draw.win=XRect(0,0,1024,768);
	  // if(m_draw.win.right<cx)
		//   m_draw.win.right=cx;
	   m_hBar.ShowWindow(XTRUE);
	   m_view.bottom=m_rect.Height()-nh-XSSIZE;
	   m_vBar.MoveWindow(XRect(m_view.right,m_view.top,
							   m_rect.Width(),m_view.bottom+1));
   }
   m_vBar.SetRange(XMAX(cy-m_view.Height(),0));
   m_hBar.SetRange(XMAX(cx-m_view.Width(),0));
#else
   m_nMaxX=XMAX(cx-m_view.Width(),0);
   m_nMaxY=XMAX(cy-m_view.Height(),0);
#endif
   //m_draw.view=m_view;
}
//
//#ifdef __ANDROID__
//#else
//void XDomView::AppendMenu(XMenu &menu)
//{
////#if defined(__TOUCHPAD__)||defined(__POINT__)
////
////#else
////   if(m_pFlashView)
////   {
////	   m_pFlashView->AppendMenu(menu);
////   }
////   else
////   {
////	   if(m_domMenus.GetSize()>0)
////	   {
////		  for(XU8 i=0;i<m_domMenus.GetSize();i++)
////			  menu.AddMenu(m_domMenus[i]);
////		  menu.AddSeparator();
////	   }
////	   if(m_forList.GetSize())
////	   {
////	   for(int i=0;i<sizeof(_viewMenus)/sizeof(XMENUDATA);i++)
////	   {
////		   /*switch(_viewMenus[i].nCommand)
////		   {
////		   case XCMD_VIEW_FORWARD:
////				if(m_forList.GetSize()>0)
////					_viewMenus[i].nStyle&=~XMS_GRAY;
////				else
////					_viewMenus[i].nStyle|=XMS_GRAY;
////				break;
////		   }*/
////		   menu.AddMenu(&_viewMenus[i]);
////	   }
////	   }
////   }
////#endif
//}
//#endif

void XDomView::Release(XBOOL bFree)
{
#ifdef _SYMBIAN
	if(m_hSaveDialog)
		{
		delete m_hSaveDialog;
		m_hSaveDialog = XNULL;
		}
#endif
	//ToDomView();
	if(m_pFlashView)
	{
		DestoryChild(m_pFlashView);
		AddCommand(XWM_COMMAND,XCMD_DOM_VIEW,0);
	}
	m_domMenus.FreeAll();
	ReleaseClient();
	//Modify Cookie MODE
	m_cookies.Clear();
	//m_strCookies.RemoveAll();
	XCatch ch(m_pClientApply->m_strWorkPath);
	ch.RemoveCatch(m_reqList);
	ch.RemoveCatch(m_forList);
	/*int i,nc=m_reqList.GetSize();
	for(i=0;i<nc;i++)
	{
		if(m_reqList[i]->pDom)
			delete m_reqList[i]->pDom;
	}
	nc=m_forList.GetSize();
	for(i=0;i<nc;i++)
	{
		if(m_forList[i]->pDom)
			delete m_forList[i]->pDom;
	}
	m_reqList.FreeAll();
	m_forList.FreeAll(); */
	FreeList(m_reqList);
	FreeList(m_forList);
	m_clients.RemoveAll();
	while(m_pFlashView!=XNULL)
	{
		AddCommand(XWM_COMMAND,XCMD_DOM_VIEW,0);
		XThread::Sleep(200);
	}
	/*if(m_pFlashView!=XNULL)
	{
		DestoryChild(m_pFlashView);
		if(bFree)
		{

			delete m_pFlashView;
			m_pFlashView=XNULL;
		}
	}*/

	m_nMoveTimes=0;
//#if (__CORE_VERSION__>=0x02076000)
	if(m_pLoadingBG)
		delete m_pLoadingBG;
	m_nLoadingBGIndex=0;
	m_pLoadingBG=NULL;
//#endif
}

void XDomView::InitContext(XU8 nParaSpace,XU8 nRowSpace,XU8 nColSpace)
{
    m_pDom->m_data.pView=this;
	//GetClientRect(m_draw.view);
	m_draw.nLayerMode=LA_SAME;
	m_draw.pView=this;
	m_draw.view=m_view;
	m_draw.pDom=m_pDom;//m_pDom;
	m_draw.Init();
	//m_draw.nBorder=0;
	//m_draw.ind=0;
	//m_draw.nPadding=0;
	//m_draw.nSpacing=0;
//	m_draw.nTrans=0;
//	m_draw.nDecLink=(XELT::DEC_UNDERLINE)|(XELT::DEC_UNDERLINE<<8)|(XELT::DEC_NONE<<24);
	m_draw.win=m_view;//XRect(0,0,1024,768);//m_view;
//	m_draw.nFixWin=0;//m_draw.win.Width();
//	m_draw.pDraw=m_pDraw;

//	m_draw.nPadding=0;
//	m_draw.nSpacing=0;
#ifdef _SCROLLVIEW
	int ls=m_draw.SPACE;
	int ps=m_draw.win.Height()-ls;
	m_vBar.SetSize(ls,ps);
	ps=m_draw.win.Width()-ls;
	m_hBar.SetSize(ls,ps);
#endif
}

XBOOL XDomView::Layerout()
{
//	GetViewRect(m_draw.view);
//	DWORD t1=GetTickCount();

//	m_draw.pDraw->SetOrigin(0,0);
//	m_draw.win=m_view;
//	ResetEventData();
	m_pDom->SetLayerout();

	//SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);

	//SetStatus();

	return XTRUE;

}

bool XDomView::CanMoveScreen()
{
	//LOGE("offX = %d, MaxX = %d", m_nOffX, m_nMaxX );
	if( (m_nOffX + m_view.Width() < m_pDom->m_docSize.cx) && m_nOffX > 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void XDomView::OffXChangeVal()
{
	if( m_nOffX + m_view.Width() >= m_pDom->m_docSize.cx )
		m_nOffX--;
	if( m_nOffX == 0 )
		m_nOffX++;
	return;
}

void XDomView::SetViewRegion(int left, int top, int right, int bottom )
{
	//m_view.left = left;
	m_view.top = top;
	//m_view.right = right;
	//m_view.bottom = bottom;
}

//#if (__CORE_VERSION__>=0x02079000)
void XDomView::ReConnect(const char*strURL)
{
/*	m_localREQ.URL.SetURL(strURL);
	m_saveReq.bForward=XCT_FORWARD;
	m_localREQ.bForward=XCT_DIR;
	Connect(m_localREQ,XFALSE);
#ifdef _WINEMU
	XString16 s("---->Reconnect URL:");
	s+=XString16(m_localREQ.URL.m_strURL);
	Trace(s);
#endif*/
	m_req.URL.SetURL(strURL);
	SetCookies(m_req);
	m_req.data.SetSize(0);
	m_req.nMethod=XEM::GET;

	//if(!pInfo->strClientID.IsEmpty())
	//	m_pClientApply->SetXID(pClient->GetConnectInfo()->URL.m_strHost,pInfo->strClientID,true);//p->strClientID);
	m_saveReq.bForward=XCT_DIR;
	Connect(m_req,XFALSE);
#ifdef _WINEMU
	 XString16 str("ReConnect to URL:");
	 str+=XString16(m_req.URL.m_strURL);
	this->Trace(str);
#endif

}
//#endif

void XDomView::ReConnect(XDOMFILTER*pFilter)
{
	/*m_saveReq.bForward=XCT_FORWARD;
	XURL url=m_pDom->GetURL();
	//CONNECTREQ req;
	//req.Copy(m_req);
	m_req.bForward=XCT_DIR;
	//req.URL=url;
	Connect(m_req,XFALSE);
	m_pDom->SetURL(url);
	if(m_pClient)
		m_pClient->SetFilter(pFilter);
#ifdef _WINEMU
	XString16 s("---->Reconnect URL:");
	s+=XString16(m_req.URL.m_strURL);
	Trace(s);
#endif*/
	//m_pDom->Reset(m_pDom->m_nCharset
	if(pFilter&&!pFilter->strRefeshURL.IsEmpty())
		m_localREQ.URL.SetURL(pFilter->strRefeshURL);
	m_saveReq.bForward=XCT_FORWARD;
	//XURL url=m_pDom->GetURL();
	/*if(pFilter)
	{
		if(pFilter->strRefeshURL)
			url.SetURL(pFilter->strRefeshURL);
	}*/
	//CONNECTREQ req;
	//req.Copy(m_req);
	m_localREQ.bForward=XCT_DIR;
	//req.URL=url;
	Connect(m_localREQ,XFALSE);
	//m_pDom->SetURL(url);
	//Modify FILTER
	//if(m_pClient&&m_pClient->GetFilter()==NULL)
	//	m_pClient->SetFilter(pFilter);
#ifdef _WINEMU
	XString16 s("---->Reconnect URL:");
	s+=XString16(m_localREQ.URL.m_strURL);
	Trace(s);
#endif
}

XBOOL XDomView::OnCommand(XU32 nCmd, XWindow *pWnd)
{
	if(!IsVisiable()) return XFALSE;
    switch(nCmd)
	{
	case XCMD_CLEAR_CATCH:
		 PostMsg(XWM_COMMAND,XCMD_CLEAR_CATCH,(XU32)this,XMS_PARENT);
		 break;
	case XCMD_FLASH_CLOSE:
		 //ToDomView();
		 if(m_pFlashView)
		 {
			DestoryChild(m_pFlashView);
			AddCommand(XWM_COMMAND,XCMD_DOM_VIEW,0);
		 }
		 break;
	case XCMD_DOM_ADDBOOK:
		if(m_bShowView)
		{
#ifdef __ANDROID__
#else
			if(m_pBox) delete m_pBox;
			XString16 strPrompt=XResource::LoadWString(XIDS_SAVEMARK);
			strPrompt+=XString16("-");
			strPrompt+=((XMainWnd*)GetParent())->GetTitle(this);
			strPrompt+=XString16("?");
			m_pBox=XMessageBox::Message(strPrompt,this,XCMD_DOM_ADDBOOK,XMB_YESNO|XMB_ICONQUESTION);
#endif
		}break;
	case XCMD_DOM_QUIT:
		 //PostQuitMessage();
		 PostMsg(XWM_COMMAND,XCMD_QUIT,0,XMS_TOPMOST);
		 return XTRUE;
	case XCMD_DOM_ITEM:
		 if(pWnd!=XNULL)
		 {
			 XDomItem*pItem=(XDomItem*)pWnd;
			 ResetEventData();
			 ProcResult(pItem->Connect(&m_pDom->m_data),false,false);
		 }break;
	case XCMD_HSCROLL:
		 Invalidate();

		 return XTRUE;
	case XCMD_VSCROLL:
		 Invalidate();
		 return XTRUE;
	case XCMD_VIEW_CLOSE:
		 CloseView();
		 return XTRUE;
	case XCMD_DOM_UPDATE:
		 m_saveReq.bForward=XCT_FORWARD;
		 Connect(m_req,XFALSE);
		 break;
	case XCMD_DOM_STOP:
		 m_bShowPageLoading = false;
		 m_bShowLoading=XFALSE;
		 Stop(XTRUE,XFALSE);
		 break;
	case XCMD_DOM_BACK:
		 Back();
		 break;
	case XCMD_VIEW_FORWARD:
		 Forward();
		 break;
	}
	return XWindow::OnCommand(nCmd,pWnd);
}

void XDomView::GetClipRect(XRect &rect,XBOOL bFull)
{
	rect=bFull?m_view:m_clip;
	XPoint pt;
	GetOffset(pt);
	rect+=pt;
}



void XDomView::OnMouseMove(XPoint &pt,int iPos)
{
	if(IsTypeEvent()) return;
	if(iPos) return;
	if(!m_view.PtInRect(pt))
	{
		m_bScroll=XFALSE;
		SetCursor(XCUR_ARROW);
		return;
	}

	//XLock lock(&m_ptLock);          // modify by hgl
	if(!m_ptLock.Lock(XLONGTIME))
		return;


#ifdef _SCROLLVIEW
#else
	if(m_bMouseDown&&!m_bCapture)//m_bScroll)
	{
		int dx=-pt.x+m_nDownX;
		int dy=-pt.y+m_nDownY;
		if(!m_bScroll)
		{
			int v=XABS(dx)+XABS(dy);
			if(v>20)
				m_bScroll=XTRUE;
		}

		if(m_bScroll)
		{
			SetSpeed(pt);

			int ox=m_nOffX;
			int oy=m_nOffY;
			SetOffset(m_nDownOffX+dx,m_nDownOffY+dy,true);
			//if(ox!=m_nOffX||oy!=m_nOffY)
			//	Invalidate();
			SetCursor(XCUR_HAND);
			m_ptLock.UnLock();
			return;
		}
	}
#endif
	ClientToDoc(pt);
	ResetEventData();
	XU32 s=m_pDom->OnMove(pt.x,pt.y);
	if(s&XRCINPUT)
		SetCursor(XCUR_INPUT);
	else if(s&XRCANCHOR)
		SetCursor(XCUR_ANCHOR);
	else
		SetCursor(XCUR_ARROW);
	//SetCursor(nCurType);
	ProcResult(s,false,false);

	m_ptLock.UnLock();
}

void XDomView::ProcResult(XU32 s,bool bFinal,bool bAuto)
{

#ifdef _SCROLLVIEW
#else
//	if(s)
//		m_bNeedDown=0;
#endif
	XPoint pt=m_pDom->m_data.offset;
	XEVENTDATA*pData=m_pDom->GetEventData();

	if(s&XRACTIVE)
	{
	   GetParent()->ChangeActive();
	}

//	return;
	if(s&XRREPAINT)
	{
	   XRect rect=pData->rect;
	   rect.InflateRect(2,2);
	   XPoint pt;
	   GetOffset(pt);
	   rect-=pt;
	   rect&=m_view;
	   InvalidateRect(rect);
	}
	if(s&XRLAYEROUT)
	{
		//m_pDom->Layerout(m_drawContext);
		Layerout();
		Invalidate();
	}
	if(s&XROFFSET)
	{
	   SetOffset(pt.x,pt.y,false);//pData->offset.x,pData->offset.y);
	   Invalidate();
	}
	if(s&XRBACKWORD)
	{
		Stop(XFALSE,XFALSE);
		Back();
		return;
	}
	if(s&XRFORWARD)
	{
		Stop(XFALSE,XFALSE);
		Forward();
		return;
	}
	if(s&XRVARCHANGE)
	{
		XU32 nData=m_pDom->m_data.nData;
		ResetEventData();
		m_pDom->m_data.nData=nData;
		XU32 s=m_pDom->Refresh();
		if(s) PostMsg(XWM_RESULT,s,(XU32)this,XMS_THIS);
	}
	if(s&XRNEXT)
	{
	   ResetEventData();
	   XU32 s=m_pDom->OnKeyDown(SELECTMARK::XNEXT,&m_draw,XTRUE);
	   if(s) PostMsg(XWM_RESULT,s,(XU32)this,XMS_THIS);
	   return;
	}
	if(s&XRCONNECT)
	{
		XURL url0=m_pDom->GetURL0();
		Stop(XFALSE,XFALSE);
		XEVENTDATA*pData=m_pDom->GetEventData();
		if(pData->tempURL.m_strCard.Compare("back",XTRUE)==0)
		{
			Back();
			return;
		}
		SetCookHost(m_req.URL,pData->tempURL);
		//m_forList.FreeAll();
		XCatch ch(m_pClientApply->m_strWorkPath);
		ch.RemoveCatch(m_forList);
		FreeList(m_forList);
		m_req.strValues.Copy(pData->strValues);
//		PushREQ(pData->bAuto);
		SaveREQ();
//		if(bFinal)
//			int v=0;
		m_req.bForward=bFinal?XCT_DIR:XCT_CONNECT;
//		m_req.time=pData->time;
		m_req.URL=pData->tempURL;
		m_req.nMethod=pData->nMethod;
		m_req.strHeaders.Copy(pData->strHeaders);

		m_req.strRefer=pData->strRefer;
		m_req.bAuto=pData->bAuto;
		if(pData->tempURL.m_nProto>=XPROTO_RES)
		{
		   if(!ProcSysURL(m_req)) return;
		}

		if(pData->tempURL.m_bJustCard)
		{
			//if(m_req.URL.m_strCard.IsEmpty()) return;
			if(m_pDom->m_pFilter==XNULL)
				PushREQ(XFALSE,XFALSE);
			m_domMenus.FreeAll();
			s=m_pDom->SetCard(m_req);
			SetOffset(0,0,false);
			Invalidate();
			if(s) PostMsg(XWM_RESULT,s,(XU32)this,XMS_THIS);
			return;
		}
		//CONNECTm_req m_req;

		for(XU32 i=0;i<pData->strDatas.GetSize();i+=2)
		{
			if(m_req.data.GetSize()>0)
				m_req.data.Add(XAND);
			m_req.data.Append((XU8*)pData->strDatas[i].GetData(),pData->strDatas[i].GetLength());
			m_req.data.Add(XEQU);
			m_req.data.Append((XU8*)pData->strDatas[i+1].GetData(),pData->strDatas[i+1].GetLength());
		}
		if(m_req.nMethod==XEM::GET&&m_req.data.GetSize()>0)
		{
			XString8 strURL=m_req.URL.m_strURL;
		    int id=strURL.Find(XQUEST);
			strURL+=id<0?XQUEST:XAND;
			strURL+=XString8((XPCTSTR)m_req.data.GetData(),m_req.data.GetSize());
			//int nProto=pData->URL
			pData->SetURL(strURL);
			int nProto=m_req.URL.m_nProto;
			m_req.URL=pData->tempURL;
			m_req.URL.m_nProto=m_req.URL.m_nProto;
			m_req.data.RemoveAll();
		}

		//m_bStopreq=XFALSE;
		/*if(m_pDom->JustCard())
		   ProcResult(m_pDom->SetCard());
	    else*/
		//m_nLastClick=100;//XTime::GetTimeMSec();

		{
			XURL url=m_pDom->GetURL();
			if(!bAuto)
			{
				//m_localURL=m_req.URL;
				m_localREQ.Copy(m_req);
				m_bPushed=false;
			}
			Connect(m_req);
			SetPercent(1);
			if(s&XREFRESH)
				m_pDom->SetURL(url0);
			else if(bFinal)//&&!pFilter->
			{

				m_pDom->SetURL(url);
			}

			//if(s&S
			//m_pClient->Connect(m_req);
			//GetParent()->PostMsg(WM_SET_PECENT,0,TRUE);
		}
	}
}



void XDomView::GetOffset(XPoint &pt)
{
#ifdef _SCROLLVIEW
    pt.y=m_vBar.GetPos()-m_view.top;
	pt.x=m_hBar.GetPos();
#else
	pt.x=m_nOffX;
	pt.y=m_nOffY;
#endif
}

void XDomView::ClientToDoc(XPoint &pt)
{
#ifdef _SCROLLVIEW
	pt.y+=m_vBar.GetPos()-m_view.top;
	pt.x+=m_hBar.GetPos();
#else
	pt.x+=m_nOffX;
	pt.y+=m_nOffY;
#endif
}


XBOOL XDomView::OnKeyDown(XKEYMSG&m)
{
	if(IsTypeEvent())

	{
		if(m_pFlashView)
			m_pFlashView->OnKeyDown(m);
		return XWindow::OnKeyDown(m);;
	}

	if(!m_ptLock.Lock(XLONGTIME)) 
		return XFALSE;

	XWCHAR c=m.nKeyCode;

	ResetEventData();
	XU32 s=m_pDom->OnKeyDown(c,&m_draw);
	ProcResult(s,false,false);
	if(s) SetStatus();
	else
	{
		switch(c)
		{
		case XK_RIGHTSOFT:
			 Back();
			 m_ptLock.UnLock();
			 return XTRUE;
		case XK_CLEAR:
			 OnCommand(XCMD_VIEW_CLOSE,this);
			 m_ptLock.UnLock();
			 return XTRUE;
		case XK_NUM0:
			 GetParent()->ChangeActive();
			 m_ptLock.UnLock();
			 return XTRUE;
		case XK_NUM3:
			 Forward();
			 m_ptLock.UnLock();
			 return XTRUE;
		}
	}

	m_ptLock.UnLock();

	return XWindow::OnKeyDown(m);
}

void XDomView::Invalidate()
{
	XRect rect=m_view;
	InvalidateRect(rect);
}



void XDomView::OnClick(XPoint &pt,int iPos)
{
	if(m_pFlashView)
		m_pFlashView->OnClick(pt,iPos);
}

void XDomView::StopContent()
{
	for(int i=0;i<m_clients.GetSize();i++)
	{
		m_clients[i]->Cancel();
		m_pClientApply->Catch(m_clients[i]);
		m_clients[i]->SetWindow(NULL,0,0);
	}
	m_clients.RemoveAll();
}

XBOOL XDomView::Stop(XBOOL bFinal,XBOOL bForward)
{
	//m_bShowLoading=XFALSE;
	XU16 i;
	XBOOL bSet=XFALSE;
	m_bStopreq=XTRUE;
	if(m_pClient!=XNULL)
	{
		bSet=XTRUE;
		//Modify FILTER
		//m_pClient->SetFilter(XNULL);
		m_pClient->Cancel();
		m_pClientApply->Catch(m_pClient);
		m_pClient->SetWindow(NULL,0,0);
		m_pClient->SetUsing(false);
		m_pClient=XNULL;
	}
	for( i=0;i<m_clients.GetSize();i++)
	{
		bSet=XTRUE;
		m_clients[i]->Cancel();
		m_pClientApply->Catch(m_clients[i]);
		m_clients[i]->SetWindow(NULL,0,0);
	}
	m_clients.RemoveAll();
	int nCount=m_pDom->m_imgList.GetCount();//GetImageCount();
	//XBOOL bLayerout=XFALSE;
	for(i=0;i<nCount;i++)
	{
	   XIMAGEDATA*p=m_pDom->GetImageData(i);
	   if(p->nStatus<=XL_LOADING)
	   {
		   p->Final(m_pDom);//m_pDom);
		   //UpdateImage(p,XL_FAILED);
		   //p->nStatus=XL_FAILED;
		   //bLayerout=XTRUE;
		   Layerout();
	   }
	}

	if(bFinal)
	{
		//ScrollToPosition(CPoint(m_req.nOffsetX,m_req.nOffsetY));
		ResetEventData();
		XU32 s=m_pDom->Final(m_req,m_draw,bForward,NULL);
		if(m_req.bForward!=XCT_CONNECT)
		{
			SetOffset(m_req.offset.x,m_req.offset.y,false);
			Invalidate();
		}
		ProcResult(s,true,true);
		//if(!(s&XRCONNECT)&&m_req.bForward==XCT_CONNECT)
		//	AddClick();
		Invalidate();
	}

	if(m_bShowView)
	{
#ifdef __APPLE__
		PostMsg(XWM_APPLE_MESSAGE, XCMD_END_LOAD_PAGE, (XU32)this, XMS_TOPMOST);
#elif defined( __ANDROID__)
		PostMsg(ANDROID_END_LOAD_PAGE, 0, (XU32)this, XMS_TOPMOST);
#elif defined(  _SYMBIAN )
		PostMsg(XWM_SYMBIAN_COMMAND,XCMD_END_LOAD,0,XMS_TOPMOST);
#else
		m_status.SetPecent(0);
#endif
		
	}
	m_bShowPageLoading = false;
	SetPercent(0);
	return bSet;
}

void XDomView::SaveREQ()
{
	m_req.bForward=XCT_BACK;
	m_saveReq.Copy(m_req);
	m_req.strHeaders.RemoveAll();
	m_req.data.RemoveAll();

	SetStatus();
}

void XDomView::PushREQ(XBOOL bCard,XBOOL bNewDom)//XBOOL bAuto)
{

	if(!m_bBackList||m_saveReq.bForward!=XCT_BACK)
		return;
#if defined( __APPLE__ ) || defined( __ANDROID__ ) // modify hgl by 2011-09-27
	if (m_saveReq.nMethod == HOMEPAGE)
	{
		return;
	}
#endif

//#if defined(_OPEN_WEB_HOME_) || !defined(__APPLE__) || !defined( __ANDROID__ )
//
//#else
//	if (m_saveReq.nMethod == HOMEPAGE)
//	{
//		return;
//	}
//#endif

	m_bPushed=XTRUE;
	CONNECTREQ*pReq=new CONNECTREQ;
	pReq->Copy(m_saveReq);//req;
	GetOffset(pReq->offset);
	pReq->nSelect=m_pDom->GetSelectID(XTRUE);
	pReq->nText=m_pDom->GetSelectID(XFALSE);
	m_reqList.Add(pReq);
#ifdef _XSAVEDOM
	if(!bCard)
	{
		pReq->pDom=RefDom(m_pDom);
		if(bNewDom)
		{
			XDOMFILTER*pFilter=m_pDom->m_pFilter;
			FreeDom(m_pDom);
			m_pDom=new XDom;
			m_pDom->SetFilter(pFilter);
			m_pDom->SetURL(pReq->pDom->GetURL());
			//LoadingStart();
		}
		m_draw.pDom=m_pDom;
	}
#endif


//#ifdef __ANDROID__
//	static bool sbfirst = true;
//	if(m_reqList.GetSize()>MAX_CATCH || (m_reqList.GetSize() > 0 && sbfirst))
//	{
//		sbfirst = false;
//		if (m_reqList[0]->URL.m_nProto < XPROTO_FILE)
//		{
//			XCatch ch(m_pClientApply->m_strWorkPath);
//			ch.RemoveCatch(*m_reqList[0]);
//		}
//		if(m_reqList[0]->pDom)
//			FreeDom(m_reqList[0]->pDom);
//		delete m_reqList[0];
//		m_reqList.RemoveAt(0);
//	}
//#else
	if(m_reqList.GetSize()>MAX_CATCH)
	{
		if (m_reqList[0]->URL.m_nProto < XPROTO_FILE)
		{
			XCatch ch(m_pClientApply->m_strWorkPath);
			ch.RemoveCatch(*m_reqList[0]);
		}
		if(m_reqList[0]->pDom)
			FreeDom(m_reqList[0]->pDom);
		delete m_reqList[0];
		m_reqList.RemoveAt(0);
	}
//#endif
	if(m_pDom)
		SetStatus();
}

XBOOL XDomView::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	//case XWM_LOWNETSPEED:
	//	//LOGWHERE();
	//	{
	//		XRect rect;
	//		rect.left = (m_view.Width() - m_pImgLowNetTip->Width()) / 2;
	//		rect.top = ( m_view.Height() - m_pImgLowNetTip->Height() ) / 2;
	//		rect.right = (m_view.Width() + m_pImgLowNetTip->Width()) / 2;
	//		rect.bottom = ( m_view.Height() + m_pImgLowNetTip->Height() ) / 2;
	//		InvalidateRect( rect );
	//		//mbLowNetSpeed = true;
	//	}
	//	break;
	case XWM_FINISHEDEDIT:
		 if(pData1)
		 {
			 XPCWSTR strBuf=(XPCWSTR)pData1;
			 OnString(strBuf);
////#if defined( __APPLE__ ) || defined( __ANDROID__ )
//			 int ilen = XWCSLEN( strBuf );
//             if( ilen == 0 )
//             {
//                 OnString(strBuf);
//                 if ( m_pFlashView != NULL )
//                 {
//                     m_pFlashView->OnChar( 8 );
//                 }
//                 else
//                 {
//                     m_pDom->OnChar( 8 );
//                 }
//             }
//			 else if( m_pFlashView != NULL )
//			 {
//				 for( int i = 0; i < ilen; i++ )
//				 {
//					 XWCHAR wch = strBuf[i];
//					 XKEYMSG msg = { (int)wch, (int)wch, 0, 0, 0, 0 };
//					//m_pFlashView->eyDown(msg);
//                     m_pFlashView->OnChar(wch);
//                      //m_pFlashView->OnKeyUp(msg);
//				 }
//			 }
//             else
//             {
//                 for( int i = 0; i < ilen; i++ )
//                 {
//                     ResetEventData();
//                     XWCHAR wch = strBuf[i];
//                     XU32 s = m_pDom->OnChar( wch );
//                     ProcResult( s, false, true );
//                     if (s )
//                     {
//                         SetStatus();
//                     }
//                 }
//             }
//             //加上 XDomView::OnMouseUp点击空白处，显示全部
//             //XPoint pt;
//             //	XU32 ss=m_pDom->OnClick(pt.x,pt.y);
////            	LOGWHERE();
////#endif

//#if defined( __APPLE__ ) || defined( __ANDROID__ )
//			 if( m_pFlashView != NULL )
//			 {
//				 for( int i = 0; i < ilen; i++ )
//				 {
//					 XWCHAR wch = strBuf[i];
//					 XKEYMSG msg = { (int)wch, (int)wch, 0, 0, 0, 0 };
//					 m_pFlashView->OnKeyUp(msg);
//				 }
//			 }
//#endif
		 }
		 break;
	case XWM_ENDDIALOG:
		 switch(pData1)
		 {
		 case XIDOK:
		 case XIDYES:
			 switch(pData2)
			 {
			 case XCMD_VIEW_CLOSE:
				 {
					XMainWnd*pWnd=(XMainWnd*)GetParent();
					pWnd->FullScreen(XFALSE);
					Stop(XFALSE,XFALSE);
					PostDestory();
				 }break;
			 case XCMD_DOM_ADDBOOK:
				 if(m_bShowView)
				 {
					 XPCWSTR strTitle=((XMainWnd*)GetParent())->GetTitle(this);
					 if(strTitle)
					 {
						 m_pClientApply->AddMarker(XNULL,strTitle,m_req.URL.m_strURL,0,XFALSE);
					 }
				 }
				 break;
			 }break;
		 }
		 break;
	case XWM_HSCROLL:
		 Invalidate();
		 break;
	case XWM_VSCROLL:
		 Invalidate();
		 break;
	case XWM_RESULT:
		 ProcResult(pData1,false,true);
		 return XTRUE;
	case XWM_SOCKET:
		 return AddCommand(XWM_SOCKET,pData1,pData2);
		 //return HandleSocket(pData1,(XClient*)pData2);
		 //return XTRUE;
		 break;
	}
	return XWindow::Handle(nOpera,pData1,pData2);
}

XBOOL XDomView::Connect(CONNECTREQ &req,XBOOL bSet)
{
	//if(m_pFlashView) return XFALSE;
	//XDOMFILTER*pFilter=m_pClient?m_pClient->GetFilter():XNULL;
	//if(!pFilter)
	//	pFilter=m_pDom->m_pFilter;
	if(req.strRefer.IsEmpty())
	{
		XURL&url=m_pDom->GetURL0();
		req.strRefer=url.m_strURL;
		//int v=0;
	}
	if(req.URL.m_nProto==XPROTO_APP)
	{
		XSystem::LaunchApplication(req.URL.m_strURL);
		return XTRUE;
	}
	else if(req.URL.m_nProto==XPROTO_EXT)
	{
		XSystem::LaunchContext(req.URL.m_strURL,0);
		return XTRUE;
	}
	else if(req.URL.m_nProto==XPROTO_WIN)
	{
		XSystem::OpenWindow(req.URL.m_strURL,0);
        return XTRUE;
	}
    else if(req.URL.m_nProto==XPROTO_WEBV)
    {
        XSystem::OpenWebView(req.URL.m_strURL, req.URL.m_nProto );
#ifdef __APPLE__
        if(!g_BaiWanPayUrlSavedData)
            g_BaiWanPayUrlSavedData = new BWSendMessageContentClass;
        XLock lock(&(g_BaiWanPayUrlSavedData->m_lock));
        if(!g_BaiWanPayUrlSavedData->parameter1)
        {
            g_BaiWanPayUrlSavedData->parameter1 = (char *)malloc(req.URL.m_strURL.GetLength());
            g_BaiWanPayUrlSavedData->parameter1Len = req.URL.m_strURL.GetLength();
            memcpy(g_BaiWanPayUrlSavedData->parameter1, req.URL.m_strURL.GetData(), req.URL.m_strURL.GetLength());
        }
        lock.UnLock();
        PostMsg(XWM_APPLE_MESSAGE, XCMD_SHOWWEBVIEW, (XU32)0, XMS_TOPMOST);
#endif
        return XTRUE;
    }
    else if(req.URL.m_nProto==XPROTO_WEBB)
    {
        XSystem::OpenWebView(req.URL.m_strURL, req.URL.m_nProto );
#if defined(__APPLE__)
        if(!g_BaiWanWeBBUrlSavedData)
            g_BaiWanWeBBUrlSavedData = new BWSendMessageContentClass;
        XLock lock(&(g_BaiWanWeBBUrlSavedData->m_lock));
        if(!g_BaiWanWeBBUrlSavedData->parameter1)
        {
            g_BaiWanWeBBUrlSavedData->parameter1 = (char *)malloc(req.URL.m_strURL.GetLength());
            g_BaiWanWeBBUrlSavedData->parameter1Len = req.URL.m_strURL.GetLength();
            memcpy(g_BaiWanWeBBUrlSavedData->parameter1, req.URL.m_strURL.GetData(), req.URL.m_strURL.GetLength());
        }
        lock.UnLock();
        PostMsg(XWM_APPLE_MESSAGE, XCMD_SHOWWEBBVIEW, (XU32)0, XMS_TOPMOST);
#endif
        return XTRUE;
    }
    else if(req.URL.m_nProto==XPROTO_IAPPAY)
    {
#if defined(__APPLE__)
        if(!g_BaiWanIAPUrlSavedData)
            g_BaiWanIAPUrlSavedData = new BWSendMessageContentClass;
        XLock lock(&(g_BaiWanIAPUrlSavedData->m_lock));
        if(!g_BaiWanIAPUrlSavedData->parameter1)
        {
            g_BaiWanIAPUrlSavedData->parameter1 = (char *)malloc(req.URL.m_strURL.GetLength());
            g_BaiWanIAPUrlSavedData->parameter1Len = req.URL.m_strURL.GetLength();
            memcpy(g_BaiWanIAPUrlSavedData->parameter1, req.URL.m_strURL.GetData(), req.URL.m_strURL.GetLength());
        }
        lock.UnLock();
        PostMsg(XWM_APPLE_MESSAGE, XCMD_IOS_PID, (XU32)0, XMS_TOPMOST);
#endif
        return XTRUE;
    }
	LoadingStart(m_pClientApply->GetFilter(req.URL.m_strURL));
	this->StartFile(req.URL.m_strURL);

	Stop(XFALSE,XFALSE);
	m_bLoaded=XFALSE;

	//if(m_pClientApply->m_info.m_bModifyToken)
	//{
	//	req.strHeaders.Add(XEH()[XEH::XTOKENID]);
	//	req.strHeaders.Add(m_pClientApply->m_info.strXToken);
	//	//m_pClientApply->m_bModifyToken=XFALSE;
	//}

	//req.strRefer=m_saveReq.URL.m_strURL;
	if(m_pClient==NULL)
		m_pClient=m_pClientApply->CreateClient(req,-1,0,this);
	if(m_pClient==XNULL) return XFALSE;
	m_pClient->SetUsing(false);
	//if(pFilter)
	//	m_pClient->SetFilter(pFilter);
	m_nMoveTimes=0;
	m_bStopreq=XFALSE;
	if(bSet)
	{
		if(m_req!=req)
		{
			SetCookHost(m_req.URL,req.URL);
			SaveREQ();
			//PushREQ(req.bAuto);
		}
		XCatch ch(m_pClientApply->m_strWorkPath);
		ch.RemoveCatch(m_forList);
		//m_forList.FreeAll();
		FreeList(m_forList);
		m_req.Copy(req);
	}
	m_pDom->SetURL(req.URL);
	SetCookies(req);
//	m_pClient->SetFilter(m_pClientApply->GetFilter(req.URL.m_strURL));
//	if(m_pClient->GetFilter())
//		m_pDom->SetFilter(m_pClient->GetFilter());
	m_pClientApply->ResetXID(req.URL.m_strHost);
	m_pClient->Connect(req,XCHARSET_GB2312);

	if(m_bShowView)
	{
#ifdef __APPLE__
		PostMsg(XWM_APPLE_MESSAGE, XCMD_START_LOAD_PAGE, (XU32)this, XMS_TOPMOST);
#elif defined(  __ANDROID__ )
		PostMsg(ANDROID_START_LOAD_PAGE, 0, (XU32)this, XMS_TOPMOST);
#else
		m_status.SetPecent(1);
#endif
		
	}
	m_bShowPageLoading = true;
	SetPercent(1);
	//SetStatus();
	return XTRUE;
}


void XDomView::OnClose()
{
	Release(XFALSE);
}

XINT XDomView::CalcPecent()
{
	if(m_pFlashView)
	{
		return 0;
	}
	XU32 i;
	XU16 nPecent=m_pClient==XNULL?50:CalcPecent(*m_pClient)>>1;
	if(nPecent<=0)
		nPecent = 50;
	if(m_pClient==XNULL||m_pClient->GetStatus()>=XPROC_HEADER)
	{
	    XU32 nCount=m_pDom->GetImageCount(),
			 nFinished=0,nLoading=0;
		if(nCount<=0)
		{
			return nPecent;
		}
		//else if(nPecent<=0) nPecent=50;
		for( i=0;i<nCount;i++)
		{
			XIMAGEDATA* pData=
				m_pDom->GetImageData(i);
			if(pData==NULL) continue;
			switch(pData->nStatus)
			{
			case XL_OK:
			case XL_FAILED:
				 nFinished++;
				 break;
			case XL_LOADING:
				 nLoading++;
				 break;
			}
		}
		nPecent+=(XU16)(nFinished*50/nCount);
		XU32 nl=0;
		for(i=0;i<m_clients.GetSize();i++)
			nl+=CalcPecent(*m_clients[i]);
		nPecent+=(XU16)((nl/nCount)>>1);
	}

	if(nPecent>100)
		nPecent=100;
	else if(nPecent<1)
		nPecent=1;
#ifdef __APPLE__
	PostMsg(XWM_APPLE_MESSAGE, XCMD_LOAD_PROCESS, nPecent, XMS_TOPMOST);
#endif
	return nPecent;
}

XINT XDomView::CalcPecent(XClient &client)
{
	if(!client.IsConnecting())
		return 0;
	RESPONSEINFO*pInfo=client.GetResponseInfo();

	switch(client.GetStatus())
	{
	default:
	case XPROC_BEGIN:return 2;
	case XPROC_HEADER:return 5;
	case XPROC_CONTENT:
		 if(pInfo->nLength>0)
			return (XU16)(10+90*pInfo->nTotalSize/pInfo->nLength);
		 else
		 {
			XU16 nPecent=(XU16)(10+90*pInfo->nTotalSize/MAX_PACK);
			if(nPecent>100) nPecent=100;
			return nPecent;
		 }
	case XPROC_FINISHED:return 100;
	}
	return 0;
}

XBOOL XDomView::HandleSocket(XU32 nCmd, XClient *pClient)
{
	//XLock lock(&m_ptLock);           // modify by hgl
	if(!m_ptLock.Lock(XSHORTTIME))
		return XFALSE;

	switch(nCmd)
	{
	case XClient::CONNECTED:
		OnConnect(pClient);
		break;
	case XClient::STARTED:
		OnStarted(pClient);
		break;
	case XClient::RECIVE:
		OnRecive(pClient);
		break;
	case XClient::FINISHED:
		OnFinished(pClient);
		break;
	case XClient::CANCELED:
		OnFinished(pClient, XFALSE);
		break;
	}

	m_ptLock.UnLock();
	pClient->PostOK();
	return XTRUE;
}

void XDomView::OnConnect(XClient *pClient)
{
	if(pClient->Get_CallID()==(XU32)-1||pClient->GetNotifyID())
		return;
   	XBOOL bSet=XFALSE;
	if(pClient==m_pClient)
		bSet=XTRUE;
	if(bSet)//&&m_bShowView)
	{
		int nPecent=CalcPecent();//XMAX(CalcPecent(),1);
		if(m_bShowView)
		{
#ifdef  __ANDROID__
			if( m_nPecent < nPecent )
				m_nPecent = nPecent;
			PostMsg(ANDROID_LOAD_PROCESS, m_nPecent, NULL, XMS_TOPMOST);
#endif
#ifdef _WIN32
			PostMsg(XWM_SETSTATUS,0,(XU32)this,XMS_PARENT);
			m_status.SetPecent(nPecent);
#endif
		}
		if( nPecent > 90 )
			m_bShowPageLoading = false;
		SetPercent(nPecent);
	}
}

void XDomView::ToDomViewEx()
{
	if(m_pFlashView)
	 {
		DestoryChild(m_pFlashView);
		AddCommand(XWM_COMMAND,XCMD_DOM_VIEWEX,0);
	 }
}
//void* XDomView::thread_xdomview(void *xdomview){
//    XDomView *mxdomview = (XDomView*)xdomview;
//    XClient *mmpClient = mmpclient;
//    RESPONSEINFO*pInfo=mmpClient->GetResponseInfo();
//    LOGE("XDomView::thread_xdomview L:1619 mmpClient==%p ",mmpClient);
//    LOGE("XDomView::thread_xdomview L:1620 pInfo==%p",pInfo);
//    mxdomview->ToFlashView(pInfo->nTotalSize,mmpClient);
//    
//        
//}

void XDomView::OnStarted(XClient *pClient)
{
	RESPONSEINFO*pInfo=pClient->GetResponseInfo();
	CONNECTINFO*pcc=pClient->GetConnectInfo();
	char* str=pcc->URL.m_strFile.GetData();
	
//	if(strstr(str,"Loading.swf")!=XNULL)
//		int vv=0;

	if(pClient&&pClient->Get_CallID()==_XBACKIMAGEID_)
	{
		XBOOL bLoadImage=XFALSE;
		if(ProcHttpStatus(pClient,XFALSE,bLoadImage))
		{
			if(m_pLoadingBG==NULL)
				m_pLoadingBG=new XImage();
			else
				m_pLoadingBG->Release();
			m_nLoadingBGIndex=0;
		}
		return;
	}
	if(pInfo->type.m_nType==XCNT::TXFILTER)
		return;
    if ( pInfo->type.m_nType==XCNT::TPAYSDKPID )
     {
     	m_strPaySDKPid.Empty();
     	return;
     }

#ifdef __APPLE__
    const char* url = pClient->GetConnectInfo()->URL.m_strURL.GetData();
    if (m_chargeUrl.GetData()&&(!strcmp(m_chargeUrl.GetData(), url)))
    {
        m_strPaySDKPid.Empty();
        return;
    }
#endif
    
#ifdef _WIN32
	PostMsg(XWM_SETSTATUS,0,(XU32)this,XMS_PARENT);
#endif
	//stype  gtype  showad showbar onsale
	//  5     4      3      2       1 
    if ((pInfo->nShowBar!=2)||(pInfo->nOnSale!=2)||(pInfo->nShowAD!=2) ||( pInfo->nGType != 2 ) ||( pInfo->nStype != 2 ) )
    {
        m_strShowBarAndOnSale = 0;
        //LOGWHEREVAL( pInfo->nOnSale );
        if (pInfo->nOnSale!=2)
        {
            m_strShowBarAndOnSale |= (pInfo->nOnSale - 48);
        }
        
        if (pInfo->nShowBar!=2)
        {
            m_strShowBarAndOnSale |= ((pInfo->nShowBar -48) << 1);
        }
        
        if (pInfo->nShowAD!=2)
        {
            m_strShowBarAndOnSale |= ((pInfo->nShowAD - 48 ) << 2);
        }
        
		if ( pInfo->nGType != 2 )
		{
			m_strShowBarAndOnSale |= ((pInfo->nGType - 48) << 3);
		}

		if ( pInfo->nStype != 2 )
		{
			m_strShowBarAndOnSale |= ((pInfo->nStype - 48 ) << 4 );
		}
		
        PostMsg( XWM_SHOWBARANDONSALE, m_strShowBarAndOnSale, 0, XMS_TOPMOST);
    }
	if (pInfo->nAppStatus!=0)
		PostMsg( XWM_APP_STATUS, (XU32)&(pInfo->nAppStatus),NULL, XMS_TOPMOST);

	if (!pInfo->nNewApp.IsEmpty())
		PostMsg( XWM_NEW_APP, (XU32)&(pInfo->nNewApp), NULL, XMS_TOPMOST);

	if( !pInfo->nNewApp.IsEmpty() && !pInfo->mStrAppDesc.IsEmpty())
	{
		pInfo->nNewApp += "|";
		pInfo->nNewApp += pInfo->mStrAppDesc;
		PostMsg( XWM_NEW_APP, (XU32)&(pInfo->nNewApp), NULL, XMS_TOPMOST);
	}

    if (pInfo->nOutLogin!=2)
        PostMsg( XWM_OUTLOGIN, (XU32)&(pInfo->nOutLogin), NULL, XMS_TOPMOST);

	SetCookie(pInfo,pcc->URL);
    
	if(pClient==m_pClient)
	{

		switch(GetDocType(pInfo,pcc))
		{
		case XTYPE_FLASH:
			 //SetCookie(pInfo);
#ifdef __APPLE__
             PostMsg(XWM_APPLE_MESSAGE, XCMD_START_FLASH, (XU32)this, XMS_TOPMOST);
#elif defined( __ANDROID__)
              PostMsg(ANDROID_START_FLASH, ANDROID_START_FLASH, (XU32)this, XMS_TOPMOST);
#endif
			
            if(m_pDom->m_pFilter==NULL)
				SetNewFilter(pcc->URL);
            ToFlashView(pInfo->nTotalSize,pClient);
			 break;
		default:
			StopContent();
			//Modify FILTER
			if(pInfo->nErrorCode==XHTTP_STATUS_OK&&!m_pFlashView&&!m_pDom->m_pFilter)//pClient->GetFilter())
			 {
				 if(!m_pDom->TestContent(pInfo->type.m_nCharset,
							pInfo->type.m_nClass,
							pInfo->type.m_nType,&m_draw))
				 {
					 Stop(XTRUE,XFALSE);
					 XSystem::LaunchContext(pcc->URL.m_strURL,0);
					 break;
				 }
			 }

#ifdef __APPLE__
			PostMsg(XWM_APPLE_MESSAGE, XCMD_LOAD_PAGE_NOT_EMPTY, (XU32)this, XMS_TOPMOST);
#endif
			 if(!m_bPushed)
			 {
				 if(!m_bPushed)
					 m_req.bForward=XCT_CONNECT;
				 PushREQ(XFALSE);

			}
			 m_pDom->SetReferURL(pcc->strRefer);
			if(m_pFlashView)
			{
				DestoryChild(m_pFlashView);
				AddCommand(XWM_COMMAND,XCMD_DOM_VIEW,0);
			}
			if(pInfo->nErrorCode==XHTTP_STATUS_OK)	
			{
			 m_strTitle.Empty();
			 if(!m_pDom->Reset(pInfo->type.m_nCharset,
							pInfo->type.m_nClass,
							pInfo->type.m_nType,&m_draw))
			 {
				InitContext();
			 }
			}

			 m_pDom->m_nDataSize=pInfo->nLength;
			 m_bHome=XFALSE;
			 SetOffset(0,0,false);
			 Invalidate();
			 SetStatus();
			 break;
		}
		m_domMenus.FreeAll();
#ifdef _SOURCECODE
		m_strSource=pInfo->type.GetTypeString();
		m_strSource+="\r\n";
		//m_strSource.Empty();
#endif
		m_strCnt.Empty();
	}
	//else
	{
		if(m_pFlashView&&(pClient->GetNotifyID()))
		{
			m_pFlashView->NewStream(pClient,m_pDom->GetURL().m_strURL);
		}
#ifdef SHOWPROGRESS
        PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_DATASTART,pInfo->nLength,XMS_TOPMOST);
#endif
        //PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_DATASET,pInfo->nTotalSize,XMS_TOPMOST);
	}

	int nPecent=CalcPecent();//XMAX(CalcPecent(),1);
	if(m_bShowView)
	{
#ifdef  __ANDROID__
		if( m_nPecent < nPecent )
			m_nPecent = nPecent;
		PostMsg(ANDROID_LOAD_PROCESS, m_nPecent, NULL, XMS_TOPMOST);
#else
		m_status.SetPecent(nPecent);
#endif
		
	}
	SetPercent(nPecent);
}


bool XDomView::IsImageClient(XClient*pClient)
{
	if(!m_clients.GetSize()) return false;
	if(pClient->Get_CallID()==-1||pClient->GetNotifyID()) return false;
	//if(pClient->GetCallID()&1) return false;
	int i,iCount=m_clients.GetSize();
	for(i=0;i<iCount;i++)
	{
		if(m_clients[i]==pClient)
            return true;
	}
	return false;
}

void XDomView::OnRecive(XClient *pClient)
{
   RESPONSEINFO*pInfo=pClient->GetResponseInfo();
#ifdef __APPLE__
    const char* url = pClient->GetConnectInfo()->URL.m_strURL.GetData();
    bool bStopIosPid = m_chargeUrl.GetData()&&(!strcmp(m_chargeUrl.GetData(), url));
#endif
	if(pClient&&pClient->Get_CallID()==_XBACKIMAGEID_)
	{
		if(m_pLoadingBG)
		{
			 XLock lock;
			if(pInfo->recBuf.LockRead(lock,0))
			{
				int nSize=pInfo->recBuf.GetDataSize();
				if(nSize>0)
				{
					XU8Array data;
					data.SetSize(nSize+1);
					pInfo->recBuf.Read(data,nSize);
					m_pLoadingBG->Append(data,nSize);
				}
				lock.UnLock();
			}
		}
		return;
	}
   bool bFilter=pInfo->type.m_nType==XCNT::TXFILTER;

#ifdef _WIN32 
   if(!bFilter)
		PostMsg(XWM_SETSTATUS,0,(XU32)this,XMS_PARENT);
#endif

   XLock lock;
   if(pInfo->recBuf.LockRead(lock,0))
   {
	 int nSize=pInfo->recBuf.GetDataSize();
	 LoadingData(nSize,pInfo->nLength);
	 if(nSize<=0)
	 {
		 lock.UnLock();
		 return;
	 }
	 XU8Array data;
	 data.SetSize(nSize+1);
	 pInfo->recBuf.Read(data,nSize);
	 lock.UnLock();
//#ifdef __ANDROID__
//	 FILE * pfile = fopen("/sdcard/test.txt", "w" );
//	 fwrite( data.GetData(), nSize, 1, pfile );
//	 fclose( pfile );
//#endif

	 if(bFilter)
	 {
		 m_strFilter+=(char*)data.GetData();
	 }
     else if ( pInfo->type.m_nType==XCNT::TPAYSDKPID )
	 {
		 m_strPaySDKPid+=(char*)data.GetData();
	 }
#ifdef __APPLE__
     else if ( bStopIosPid )
	 {
		 m_strPaySDKPid+=(char*)data.GetData();
	 }
#endif
	 else if(pClient==m_pClient)
	 {
		 m_nLockClick=0;
#ifdef _SOURCECODE
		 XString8 strTxt;
		 strTxt.ConvertFrom((XPCTSTR)data.GetData(),pInfo->type.m_nCharset);
		 m_strSource+=strTxt;
#endif
		 if(!SetTypeData(data,nSize,pClient))
		 {
			 if(!m_pDom->IsInitOK())
			 {
				 SetNewFilter(pClient->GetConnectInfo()->URL);
			 }
			 XDOMFILTER*pFilter=m_pDom->m_pFilter;//pClient->GetFilter();
			 if(pFilter&&pFilter->pRetGO)
			 {
				XString8 strTxt;
				strTxt.ConvertFrom((XPCTSTR)data.GetData(),pInfo->type.m_nCharset);
				m_strCnt+=strTxt;
			 }
			 if(!m_pDom->PhaseXML(data,nSize))
			 {
				 pClient->Cancel();
			 }
			 LoadImages(true);
			 if(!m_bLoaded)
			 {
				 Layerout();
			 }
		 }
	 }
	 else
	 {
		if(!SetTypeData(data,nSize,pClient)&&IsImageClient(pClient))
		{
			XIMAGEDATA* pData=(XIMAGEDATA*)(pClient->Get_CallID());
			if(pData!=NULL)
			{
				pData->AppendData(data,nSize);//,nSize);
			}
		}
	 }
   }
   int nPecent=CalcPecent();
#ifdef SHOWPROGRESS
    PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_DATASET,pInfo->nTotalSize,XMS_TOPMOST);
#endif
   if(!m_bLoaded&&m_bShowView)
   {
#ifdef  __ANDROID__ 
		if( m_nPecent < nPecent )
			m_nPecent = nPecent;
	   PostMsg(ANDROID_LOAD_PROCESS, m_nPecent, NULL, XMS_TOPMOST);
#else
	   m_status.SetPecent(nPecent);   
#endif
   }
   SetPercent(nPecent);
   return;
}

void XDomView::OnFinished(XClient *pClient,XBOOL bFinished)
{
	XBOOL bExist=XFALSE;
	_RESPONSEINFO*p=pClient->GetResponseInfo();
#ifdef _WINEMU
	XString16 strt("Load OK:");
	//if(pClient->GetConnectInfo()->URL.m_strURL.Find("dailyRewardBtn.png")>=0)
	//	int v=0;
	strt+=XString16(pClient->GetConnectInfo()->URL.m_strURL);
	Trace(strt);
#endif

	if(pClient&&pClient->Get_CallID()==_XBACKIMAGEID_)
	{
		if(m_pLoadingBG)
		{
			m_pLoadingBG->Final();
			m_nLoadingBGIndex=0;
			Invalidate();
		}
		return;
	}
	if(p->type.m_nType==XCNT::TXFILTER)
	{
		this->m_pClientApply->AddFilters(m_strFilter,this);
		m_strFilter.Empty();
		pClient->SetWindow(NULL,0,0);
		return;
	}

    if ( p->type.m_nType==XCNT::TPAYSDKPID )
    {
//        printf("------------%s--------------\n",(char *)m_strPaySDKPid.GetData());
        PostMsg(XWM_PAYSDK_PID, (XU32)m_strPaySDKPid.GetData(), NULL,  XMS_TOPMOST);
    }
#ifdef SHOWPROGRESS
    PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_DATAEND,0,XMS_TOPMOST);
#endif
#ifdef __APPLE__
    const char* url = pClient->GetConnectInfo()->URL.m_strURL.GetData();
    if (m_chargeUrl.GetData()&&(!strcmp(m_chargeUrl.GetData(), url)))
    {
        printf("%s\n", m_strPaySDKPid.GetData());
        PostMsg(XWM_APPLE_MESSAGE, XCMD_STOP_IOS_PID, (XU32)m_strPaySDKPid.GetData(), XMS_TOPMOST);
    }
#endif
	if(m_pFlashView)
	{
			RESPONSEINFO*pInfo=pClient->GetResponseInfo();
			switch(pInfo->nErrorCode)
			{
				case XHTTP_STATUS_REDIRECT_KEEP_VERB:
				case XHTTP_STATUS_MOVED:
				case XHTTP_STATUS_REDIRECT:
				case XHTTP_STATUS_REDIRECT_METHOD:
				case XHTTP_STATUS_USE_PROXY:		
				if(!pInfo->strLocation.IsEmpty()&&m_nMoveTimes<5)
				{
				 m_req.URL.SetURL(pInfo->strLocation);
				 SetCookies(m_req);
				 m_req.data.SetSize(0);
				 m_req.nMethod=XEM::GET;

				 m_saveReq.bForward=XCT_DIR;
				 StopContent();
				 pClient->Connect(m_req,pClient->GetResponseInfo()->type.m_nCharset);
				 m_nMoveTimes++;
				 return;
				}
			}
		m_pFlashView->CloseStream(pClient);
		if(!p->strClientID.IsEmpty())
				m_pClientApply->SetXID(pClient->GetConnectInfo()->URL.m_strHost,p->strClientID,true);//p->strClientID);
		if(m_pClient==pClient)
		{
			//Modify FILTER
			if(m_bShowView&&m_req.bForward==XCT_CONNECT&&m_pDom->m_pFilter==XNULL)//pClient->m_pCurFilter==NULL&&pClient->m_pReferFilter==NULL)
				/*&&(pInfo->type.m_nType==XCNT::TX_SHOCK_WAVE_FLASH)*/
			{
				RESPONSEINFO*pInfo=pClient->GetResponseInfo();
				XString8 strTitle;
				XString16 str;
				if(pInfo->type.m_nType==XCNT::TX_SHOCK_WAVE_FLASH)
				{
					if(m_req.URL.m_nProto<XPROTO_FILE)
					{
						XString8 strTmp=m_req.URL.m_strFile;
						XURL::Decode(strTmp);
						//strTmp.ConvertToGB();
						XFileFinder::GetFileName(strTmp,strTitle);
						str=strTitle;
						//str.ConvertFrom(strTitle,XCHARSET_GB2312);
					}
					else
					{
						XFileFinder::GetFileName(m_req.URL.m_strFile,strTitle);
						str.ConvertFrom(strTitle,XCHARSET_GB2312);
					}
					if(!str.IsEmpty())
						AddClick(str,XFALSE);
				}
			}
			m_pClientApply->Catch(m_pClient);
			m_pClient->SetWindow(NULL,0,0);
			m_pClient->SetUsing(false);
			m_pClient=XNULL;
		}
		return;
//		m_status.SetPecent(0);
		//return;
	}

	XBOOL bLoadImage=XTRUE;
	if(pClient==m_pClient)
	{

		XU32 nStatus=p->nErrorCode;
		if(m_pDom->m_pFilter==NULL)
			m_pDom->SetFilter(m_pClientApply->GetFilter(m_req.URL.m_strURL));
	
		if(ProcHttpStatus(pClient,XTRUE,bLoadImage))
		{
			if(!p->strClientID.IsEmpty())
				m_pClientApply->SetXID(pClient->GetConnectInfo()->URL.m_strHost,p->strClientID,true);//p->strClientID);
			m_pClientApply->Catch(m_pClient);
			m_pClient->SetWindow(NULL,0,0);
			m_pClient->SetUsing(false);
			m_pClient=XNULL;
			Invalidate();
			SetStatus();
		}
	}

	if(!m_pFlashView)
	{
		for(XU16 i=0;i<m_clients.GetSize();i++)
		{
			if(m_clients[i]==pClient)
			{
				XIMAGEDATA* pData=(XIMAGEDATA*)(pClient->Get_CallID());
				if(!ProcHttpStatus(pClient,XFALSE,bLoadImage))
				{
					if(pData!=XNULL) pData->ResetStream();
					break;
				}

				if(pData!=NULL)
				{
					if(pData->Final(m_pDom))
					{
						Layerout();
						Invalidate();
					}
				}

				m_pClientApply->Catch(pClient);
				m_clients.RemoveAt(i);
				bExist=XTRUE;
				break;
			}
		}
	}
	if(m_bStopreq||!LoadImages(bLoadImage))
	{
		//return;
		XBOOL bLoad=(m_pClient!=NULL&&m_pClient->IsConnecting())||m_clients.GetSize()>0;
		if(!bLoad||m_pFlashView)
		{
			if(m_bShowView)
			{
#ifdef __APPLE__
				PostMsg(XWM_APPLE_MESSAGE, XCMD_END_LOAD_PAGE, (XU32)this, XMS_TOPMOST);
#elif defined(  __ANDROID__ )
				PostMsg(ANDROID_END_LOAD_PAGE, 0, (XU32)this, XMS_TOPMOST);
#else
				m_status.SetPecent(0);
#endif
				
			}
			m_bShowPageLoading = false;
			SetPercent(0);
			if(!m_pFlashView&&!m_bLoaded)//&&p->nErrorCode==XHTTP_STATUS_OK)
				Layerout();
			ResetEventData();
			//XDOMFILTER*pFilter=m_pDom->m_pFilter;
			XDOMFILTER*pFilter=m_pClient?m_pClientApply->GetFilter(m_pClient->GetConnectInfo()->URL.m_strURL):NULL;
			if(m_pClient)
			{
				if(ProcJQScript(m_pClient,pFilter))
				{
					m_pClientApply->Catch(m_pClient);
					m_pClient->SetWindow(NULL,0,0);
					m_pClient->SetUsing(false);
					m_pClient=XNULL;
					Invalidate();
					SetStatus();
				}
				else
				{
					//StopContent();
					return;
				}
			}
			XU32 s=(m_pDom->Final(m_req,m_draw,XTRUE,pFilter));
			if(m_req.bForward!=XCT_CONNECT)
			{
				SetOffset(m_req.offset.x,m_req.offset.y,false);
				Invalidate();
			}
			ProcResult(s,true,true);

			//Modify FILTER
			//if(m_pClient&&m_pClient->m_pCurFilter==XNULL&&m_pClient!=pClient)
			//	m_pClient->SetFilter(pClient->GetFilter());
			//XString16 strTitle=((XMainWnd*)GetParent())->GetTitle(this);
			//m_pClientApply->Click(m_req,strTitle);
			RESPONSEINFO*pInfo=pClient->GetResponseInfo();
			if(m_bShowView&&!(s&XRCONNECT)&&m_req.bForward==XCT_CONNECT
				/*&&(pInfo->type.m_nType==XCNT::TX_SHOCK_WAVE_FLASH)*/)
			{
				XString8 strTitle;
				XString16 str;
				if(pInfo->type.m_nType==XCNT::TX_SHOCK_WAVE_FLASH)
				{
					if(m_req.URL.m_nProto<XPROTO_FILE)
					{
						XString8 strTmp=m_req.URL.m_strFile;
						XURL::Decode(strTmp);
						//strTmp.ConvertToGB();
						XFileFinder::GetFileName(strTmp,strTitle);
						str=strTitle;
						//str.ConvertFrom(strTitle,XCHARSET_GB2312);
					}
					else
					{
						XFileFinder::GetFileName(m_req.URL.m_strFile,strTitle);
						str.ConvertFrom(strTitle,XCHARSET_GB2312);
					}
					if(!str.IsEmpty())
						AddClick(str,XFALSE);
				}
				else if(!m_pFlashView&&
					     pInfo->type.m_nType!=XCNT::TX_SHOCK_WAVE_FLASH&&m_req.URL.m_nProto<XPROTO_FILE)
				{
					XString16 strTitle=((XMainWnd*)GetParent())->GetTitle(this);
					if(!strTitle.IsEmpty())
					{
						AddClick(strTitle,XTRUE);
					}
				}

			}
			//Invalidate();
			return;
		}
		m_pClientApply->UpdateClient();
	}
	//return;
	//int nPecent=
	int nPecent=(m_pFlashView||m_bLoaded)?0:CalcPecent();
	if(m_bShowView)
	{
#ifdef _SYMBIAN
		PostMsg(XWM_SYMBIAN_COMMAND,XCMD_LOAD_PERCENT,nPecent,XMS_TOPMOST);
#elif defined( __ANDROID__ )
		//LOGWHEREVAL( nPecent );
		//m_nPecent = nPecent;
		//PostMsg(ANDROID_LOAD_PROCESS, nPecent, NULL, XMS_TOPMOST);
#else
		m_status.SetPecent(nPecent);
#endif
	}
	SetPercent(nPecent);
}

XBOOL XDomView::LoadImages(bool bImageLoad)
{
	//return XFALSE;
	//return XTRUE;
//#ifdef (_WINEMU
//#else
	if(m_pDom->m_pFilter&&!m_pDom->m_pFilter->bLoadScript)
	{
		StopContent();
		return XFALSE;
	}
//#endif
   if(m_clients.GetSize()>=MAX_THREADS) return XTRUE;
   XBOOL bImage=XFALSE;
   XU16 nCount=(XU16)m_pDom->GetImageCount();
   for(XU16 i=0;i<nCount;i++)
   {
	  XIMAGEDATA* pData=m_pDom->GetImageData(i);
	  if(pData==NULL) continue;

	  if(!bImageLoad&&pData->nType==XIT_IMAGE)
		  continue;

	  if(pData->nStatus==XL_WAIT)
	  {
		 CONNECTREQ req;
		 req.bForward=m_req.bForward;
		 req.nMethod=pData->nMethod;
		 req.URL=pData->pDom->GetURL();
//#ifdef __ANDROID__	// zhengjl 2011.09.10
//		 pData->strURL.ConvertToGB();
//#endif
		 req.URL.SetURL(pData->strURL);
		 if(req.URL.m_nProto==XPROTO_RES)
		 {
			 pData->FromResource(m_pDom,req.URL.m_strFile);
			 //pData->image.CreateFromResource(req.URL.m_strFile);
			 //LoadResImage(pData->image,CString(req.URL.m_strFile));
			 //UpdateImage(pData,XL_OK);
			 //pData->nStatus=XL_OK;
		 }
		 else
		 {
			SetCookies(req);
			XClient*pClient=m_pClientApply->CreateClient(req,(XU32)pData,0,this);//new CClient(XFALSE);
			if(pClient!=XNULL)
			{

				if(pClient->Connect(req,XTRUE,XTRUE)==2)
				{
#ifdef _WINEMU
					XString16 strTmp("Load WEB resource(Cached):");
					strTmp+=XString16(req.URL.m_strURL);
					Trace(strTmp);
#endif
					RESPONSEINFO*pInfo=pClient->GetResponseInfo();
					//if(pInfo->nLength>0)
					//	pData->nSize=pInfo->nLength;
					pData->AppendData(pInfo->recBuf.GetAddres(),
										pInfo->recBuf.GetDataSize());//,nSize);OnRecive(pClient);
					/*if(pData->image.Final()&&!pData->image.IsEmpty())
						UpdateImage(pData,XL_OK);
					else
					{
						pData->image.Release();
						UpdateImage(pData,XL_FAILED);
					}*/
					if(pData->Final(m_pDom))
					{
						Layerout();
						//Invalidate();
					}
					Invalidate();
					//m_pClientApply->Catch(pClient);

				}
				else
				{
#ifdef _WINEMU
					XString16 strTmp("Load WEB resource:");
					strTmp+=XString16(req.URL.m_strURL);
					Trace(strTmp);
#endif					
					bImage=XTRUE;
					m_clients.Add(pClient);
					pData->nStatus=XL_LOADING;
					//pData->SetStatus(XL_LOADING);
					//UpdateImage(pData,XL_LOADING);
				}

				//pData->nStatus=XL_LOADING;
			}
		 }

	  }
	  if(m_clients.GetSize()>=MAX_THREADS)
          break;
   }
   return bImage;
}

void XDomView::SetOffset(int x, int y,bool bUpdate)
{
	int ox=0,oy=0,nx=0,ny=0;
#ifdef _SCROLLVIEW
	ox=m_vBar.GetPos();
	oy=m_hBar.GetPos();
    m_vBar.SetPos(y);
	m_hBar.SetPos(x);
	nx=m_vBar.GetPos();
	ny=m_hBar.GetPos();
#else
	ox=m_nOffX;
	oy=m_nOffY;
	m_nOffX=x;
	m_nOffY=y;
	if(m_nOffX>m_nMaxX) m_nOffX=m_nMaxX;
	if(m_nOffY>m_nMaxY) m_nOffY=m_nMaxY;
	if(m_nOffX<0) m_nOffX=0;
	if(m_nOffY<0) m_nOffY=0;
	nx=m_nOffX;
	ny=m_nOffY;
//	if(m_nOffY==0)
//		int v=0;
#endif
	if(!bUpdate||(ox==nx&&oy==ny)) return;
#ifdef __ANDROID__
	Invalidate();
	return;
#elif defined(__APPLE__)
	Invalidate();
	return;
#elif defined(_SYMBIAN)
	Invalidate();
	return;
#else
	int dx=nx-ox;
	int dy=ny-oy;
	if(dx<0) dx=-dx;
	if(dy<0) dy=-dy;
	XRect rect=m_rect;
	int w=rect.Width();
	int h=rect.Height();
	if(dx>w/4||dy>h/4)
	{
		Invalidate();
	}
	else
	{
		dx=nx-ox;
		dy=ny-oy;
		m_nScrollX=dx;
		m_nScrollY=dy;
		Invalidate();
	}
#endif
}

void XDomView::SetTitle(XPCWSTR strTitle)
{
	m_strTitle=strTitle;
//	if(m_pParent)
//		m_pParent->Invalidate();
	if(m_pDom->m_bNoTitle) return;
	XEVENTDATA*p=m_pDom->GetEventData();

	if(!p) return;
	//if(p->pDom!=m_pDom)
	//	return;
	p->strTitle=strTitle;
	//PostMsg(XWM_SETTITLE,(XU32)strTitle,(XU32)this,XMS_PARENT);
	GetParent()->Handle(XWM_SETTITLE,(XU32)strTitle,(XU32)this);//,XMS_PARENT);
	//XMainWnd*p=(XMainWnd*)GetParent();
	//if(p!=XNULL) p->SetTitle(strTitle,this);
}

void XDomView::SetStatus()
{
#ifdef __ANDROID__
#else

#if defined(__TOUCHPAD__)||defined(__POINT__)
	if(!m_bShowView) return;
#ifdef _SYMBIAN
#else
	XBOOL bHome=m_req.nMethod==HOMEPAGE||m_bHome;
	if(bHome)
		m_bShowLoading=false;
	m_status.SetItemStatus(XCMD_DOM_BACK,m_reqList.GetSize()?XTS_NORMAL:XTS_DISABLE);
	m_status.SetItemStatus(XCMD_VIEW_FORWARD,m_forList.GetSize()?XTS_NORMAL:XTS_DISABLE);
	m_status.SetItemStatus(XCMD_DOM_UPDATE,
		(m_req.URL.m_nProto<XPROTO_FILE&&!m_req.URL.m_strURL.IsEmpty()&&!bHome)?XTS_NORMAL:XTS_DISABLE);
	XPCWSTR strTitle=((XMainWnd*)GetParent())->GetTitle(this);
	if(!strTitle||!strTitle[0])
		m_status.SetItemStatus(XCMD_DOM_ADDBOOK,XTS_DISABLE);
	else
		m_status.SetItemStatus(XCMD_DOM_ADDBOOK,m_req.URL.m_strURL.IsEmpty()||bHome?XTS_DISABLE:XTS_NORMAL);
	m_status.SetItemStatus(XCMD_DOM_HOME,bHome?XTS_DISABLE:XTS_NORMAL);
#endif
#else
	XU32 s=m_pDom->m_pSelect==XNULL?0:m_pDom->m_pSelect->GetStyle();
    if(s&XDomItem::DOM_INPUT)
		m_status.SetMode(s&XDomItem::DOM_EDITING?XStatusBar::XS_FINISH:XStatusBar::XS_EDIT);
	else if(s&XDomItem::DOM_SELECTABLE)
		m_status.SetMode(XStatusBar::XS_SELECT);
	else
		m_status.SetMode(XStatusBar::XS_NONE);
	m_status.SetBack(m_reqList.GetSize()>0);

	//m_status.SetItemStatus(XCMD_DOM_STOP,
#endif
#endif
}


void XDomView::OnTimer()
{
	//if ( mbLowNetSpeed )
	//{
	//	static int icont = 0;
	//	icont ++;
	//	if ( icont > 60 )
	//	{
	//		mbLowNetSpeed = false;
	//		icont = 0;
	//		XRect rect;
	//		rect.left = (m_view.Width() - m_pImgLowNetTip->Width()) / 2;
	//		rect.top = ( m_view.Height() - m_pImgLowNetTip->Height() ) / 2;
	//		rect.right = (m_view.Width() + m_pImgLowNetTip->Width()) / 2;
	//		rect.bottom = ( m_view.Height() + m_pImgLowNetTip->Height() ) / 2;
	//		InvalidateRect( rect );
	//	}
	//}

	if( m_bShowPageLoading )
	{
		m_iPageLoadingIndex++;
		if( m_iPageLoadingIndex >= m_pImgPageLoading->FrameCount() )
			m_iPageLoadingIndex = 0;
	}

	//if(m_nLastClick)
	//	m_nLastClick--;
	if(m_bShowLoading)
		UpdateLoading();
	if(m_pFlashView)
		return;
	if(m_nLockClick)
		m_nLockClick--;	
	//XLock lock(&m_ptLock);          // modify by hgl
	
	if(!m_ptLock.Lock(XSHORTTIME)) return;

	if(!m_bScroll&&(m_nSpeedX||m_nSpeedY))
	{
		int ox=m_nOffX;
		int oy=m_nOffY;
		SetOffset(m_nOffX+m_nSpeedX,m_nOffY+m_nSpeedY,true);
		if(ox!=m_nOffX||oy!=m_nOffY)
		//	Invalidate();
		{

		}
		else
		{
			if(ox==m_nOffX) m_nSpeedX=0;
			if(oy==m_nOffY) m_nSpeedY=0;
		}
		m_nSpeedX=m_nSpeedX*6/10;
		m_nSpeedY=m_nSpeedY*6/10;
		if(XABS(m_nSpeedX)<10)
			m_nSpeedX=0;
		if(XABS(m_nSpeedY)<10)
			m_nSpeedY=0;
		m_ptLock.UnLock();
		return;
	}

	ResetEventData();

	ProcResult(m_pDom->OnTimer(),false,true);

	m_ptLock.UnLock();

	XWindow::OnTimer();
}

void XDomView::Back(XU8 bForce,bool bStop,bool bLocal)
{
	m_bShowPageLoading = false;
	m_bShowLoading=false;
	if(bStop)
	{
		if(Stop(XTRUE,XFALSE)) {if(!bForce) return;}
	}
   if(m_reqList.GetSize()<=0) return;
   else
   {

#ifdef _XSAVEDOM
	   int id=m_reqList.GetSize()-1;
	   CONNECTREQ*pReq=m_reqList[id];
	   XBOOL bJustCard=pReq->pDom==XNULL;
	   PushForward(bJustCard);
	   if(!bJustCard)
	   {
		    if(m_pDom)
				FreeDom(m_pDom);
				//delete m_pDom;
			SetCookHost(m_req.URL,pReq->URL);
			m_pDom=pReq->pDom;
			m_draw.pDom=m_pDom;
	   }
	   m_req.Copy(*pReq);
	   m_bHome = pReq->nMethod==HOMEPAGE;
	   delete pReq;
	   m_reqList.RemoveAt(id);
	   //GetParent()->SendMessage(WM_SET_URL,(WPARAM)(XPCTSTR)m_req.URL.m_strURL,0);
	   m_pDom->SetURL(m_req.URL);
	   m_req.bForward=XCT_BACK;
	   m_bStopreq=XFALSE;
	   if(bJustCard)
	   {
		  ResetEventData();
		  m_domMenus.FreeAll();
		  ProcResult(m_pDom->SetCard(m_req),false,true);
		  SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);
		  SetOffset(m_req.offset.x,m_req.offset.y,false);
		  Invalidate();
		  SetStatus();
	   }
	   else
	   {
		  if(m_req.nMethod==HOMEPAGE)
			  Connect(m_req,XFALSE);
		  SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);
		  SetOffset(m_req.offset.x,m_req.offset.y,false);
		  XEVENTDATA*pData=m_pDom->GetEventData();
		  PostMsg(XWM_SETTITLE,(XU32)pData->strTitle,(XU32)this,XMS_PARENT);
		  Invalidate();
	   }
	   m_bPushed=XFALSE;
	   m_bLoaded=XTRUE;
	   if(bLocal)
			m_localREQ.Copy(m_req);
	   //m_localURL=m_pDom->GetURL0();
	   SetStatus();
#else
	   m_saveReq.bForward=XCT_FORWARD;
	   PushForward(XFALSE);
	   int id=m_reqList.GetSize()-1;
	   SetCookHost(m_req.URL,m_reqList[id]->URL);
	   XBOOL bJustCard=JustCard(m_req,*m_reqList[id]);
	   m_req=*m_reqList[id];
	   delete m_reqList[id];
	   //m_reqList[id]->bForward=XCT_FORWARD;
	   //m_forList.Add(m_reqList[id]);
	   m_reqList.RemoveAt(id);
	   //GetParent()->SendMessage(WM_SET_URL,(WPARAM)(XPCTSTR)m_req.URL.m_strURL,0);
	   m_pDom->SetURL(m_req.URL);
	   m_req.bForward=XCT_BACK;
	   m_bStopreq=XFALSE;
	   if(bJustCard)
	   {
		  ResetEventData();
		  m_domMenus.FreeAll();
		  ProcResult(m_pDom->SetCard(m_req));
		  SetOffset(m_req.offset.x,m_req.offset.y);
		  SetStatus();
	   }
	   else
	   {
		  Connect(m_req,XFALSE);
		  SetOffset(m_req.offset.x,m_req.offset.y);
	   }
#endif
	   SetStatus();
   }
}

void XDomView::CloseView()
{
#ifdef __ANDROID__
#else
	XString16 str=_strCloseView;
	str+=((XMainWnd*)GetParent())->GetTitle(this);
	str+=XString16("?");
	if(m_pBox) delete m_pBox;
	m_pBox=XMessageBox::Message(str,this,XCMD_VIEW_CLOSE,XMB_YESNO|XMB_ICONQUESTION);//!=XIDYES)
#endif
	//return;
	/*XMainWnd*pWnd=(XMainWnd*)GetParent();
		pWnd->FullScreen(XFALSE);
	Stop(XFALSE);
	PostDestory();*/
}

void XDomView::Forward()
{
   if(Stop(XFALSE,XFALSE)) return;;
   if(m_forList.GetSize()<=0) return;
   else
   {
	   //PushREQ(XFALSE);
	   SaveREQ();
	   int id=m_forList.GetSize()-1;
#ifdef _XSAVEDOM
	   CONNECTREQ*pReq=m_forList[id];
	   XBOOL bCard=pReq->pDom==XNULL;
	   PushREQ(bCard,XFALSE);
	   if(!bCard)
	   {
	  	  SetCookHost(m_req.URL,pReq->URL);
		  if(m_pDom)
			  FreeDom(m_pDom);
		  m_pDom=pReq->pDom;
		  m_draw.pDom=m_pDom;
	   }
	   m_bHome=pReq->nMethod==HOMEPAGE;
	   m_req.Copy(*pReq);
	   delete pReq;
	   m_forList.RemoveAt(id);
	   m_pDom->SetURL(m_req.URL);
	   m_req.bForward=XCT_FORWARD;
	   m_bStopreq=XFALSE;
	   if(bCard)//m_pDom->JustCard())
	   {
		  ResetEventData();
		  m_domMenus.FreeAll();
		  ProcResult(m_pDom->SetCard(m_req),false,true);
		  SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);
		  SetOffset(m_req.offset.x,m_req.offset.y,false);
		  SetStatus();
		  Invalidate();
	   }
	   else
	   {
		  SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);
		  SetOffset(m_req.offset.x,m_req.offset.y,false);
		  XEVENTDATA*pData=m_pDom->GetEventData();
		  PostMsg(XWM_SETTITLE,(XU32)pData->strTitle,(XU32)this,XMS_PARENT);
		  Invalidate();
		  SetStatus();
		  //Connect(m_req,XFALSE);
	   }
	   m_bPushed=XFALSE;
	   m_bLoaded=XTRUE;
	   m_localREQ.Copy(m_req);
	   //m_localURL=m_pDom->GetURL0();
	   //SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);
#else
	   SetCookHost(m_req.URL,m_forList[id]->URL);
	   XBOOL bJustCard=JustCard(m_req,*m_forList[id]);
	   m_req=*m_forList[id];
	   delete m_forList[id];
	   m_forList.RemoveAt(id);
	   m_pDom->SetURL(m_req.URL);
	   m_req.bForward=XCT_FORWARD;
	   m_bStopreq=XFALSE;
	   if(bJustCard)//m_pDom->JustCard())
	   {
		  ResetEventData();
		  m_domMenus.FreeAll();
		  ProcResult(m_pDom->SetCard(m_req));
		  SetOffset(m_req.offset.x,m_req.offset.y);
		  SetStatus();
	   }
	   else
	   {
		  Connect(m_req,XFALSE);
	   }
#endif
   }

}

void XDomView::PushForward(XBOOL bCard)
{
	if(!m_bBackList) return;
 	m_req.bForward=XCT_FORWARD;
	if(m_req.URL.m_strURL.IsEmpty()||m_req.nStatus!=XHTTP_STATUS_OK)
	{
#ifdef _XSAVEDOM
		if(m_pDom)
			FreeDom(m_pDom);
			//delete m_pDom;
		m_pDom=XNULL;
		m_draw.pDom=XNULL;
#endif
	}
	else
	//if(!m_req.URL.m_strURL.IsEmpty())
	{
		CONNECTREQ*pReq=new CONNECTREQ;
		pReq->Copy(m_req);
		GetOffset(pReq->offset);
		pReq->nSelect=m_pDom->GetSelectID(XTRUE);
		pReq->nText=m_pDom->GetSelectID(XFALSE);
		m_forList.Add(pReq);
#ifdef _XSAVEDOM
		//if(!bCard)
		{
			pReq->pDom=RefDom(m_pDom);
			FreeDom(m_pDom);
			m_pDom=XNULL;
			m_draw.pDom=XNULL;
		}
#endif

	}
}

XBOOL XDomView::JustCard(CONNECTREQ &req1, CONNECTREQ &req2)
{
	if(req1.URL.m_strCard.IsEmpty()&&
	   req2.URL.m_strCard.IsEmpty()) return XFALSE;
	if(req1.URL.m_strURL!=req2.URL.m_strURL) return XFALSE;
	if(req1.nMethod!=req2.nMethod) return XFALSE;
	if(req1.nMethod==XEM::POST)
	{
		if(req1.data.GetSize()!=req1.data.GetSize()) return XFALSE;
	}
	return XTRUE;
}

//static XPCTSTR _strCookKey[]={"path","domain","expires"};

void XDomView::SetURLCookie(XString8&strURL,XString8&strCookies,XU32 nTime,RESPONSEINFO *pInfo)
{
	   XString8Array list;
	   list.SplitString(strCookies,';');
	   XString8 strPath="/",strDomain="",strCookie=list[0];
	   //if(list.GetSize()<2)
	   //	   strKey=pInfo->strCookies[i];
		   //XEnum::SetValue(m_strCookies,pInfo->strCookies[i],strPath);
	   //else
	   
	   if(list.GetSize()>1)
	   {
		   for(XU32 j=1;j<list.GetSize();j++)
		   {
			   int id=list[j].Find('=');
			   if(id>=4)
			   {
				   XString8 strKey;
				   list[j].Left(strKey,id);
				   if(strKey.Compare("path",XTRUE)==0)
					   list[j].Mid(strPath,5);
				   else if(strKey.Compare("domain",XTRUE)==0)
					   list[j].Mid(strDomain,7);
			   }
		   }
	   }
	   
	   int id=strCookie.Find('=');
	   XString8 strKey,strValue;//strLeft;
	   if(id>0)
	   {
		   strCookie.Mid(strValue,id+1);
		   strCookie.Left(strKey,id);
	   }
	   else
	   {
           strKey=strCookie;
	   }
	   if(pInfo)
	   {
		   XEH eh;
		   switch(eh.Index(strKey,XString8::XLS_NOCASE))//==XEH::CLIENT_ID)
			   {
			   case XEH::CONFIG_URL:
					{
					  //	if(!strValue.IsEmpty())
					  pInfo->strConfigURL=strValue;
					 //m_pClientApply->SetXID(strValue);
					}
					break;
			   case XEH::CLIENT_ID:
					m_pClientApply->SetXID(strURL,strValue,true);
					break;
			   case XEH::MOBILE_TYPE:
			   		{ if(!strValue.IsEmpty())
						 m_pClientApply->m_info.strUserAgent=strValue;
					 //m_pClientApply->SetXID(strValue);
				   }break;
			   }
	   }
	   if(strDomain.IsEmpty())
		   m_cookies.SetCookies(strPath,strURL,strKey,strValue,nTime);
	   else
		   m_cookies.SetCookies(strPath,strDomain,strKey,strValue,nTime);

}

void XDomView::SetCookie(RESPONSEINFO *pInfo,XURL&url)
{
	if(!pInfo->strFilterURL.IsEmpty())
	{
		XU8Array data;
		m_strFilter.Empty();
#ifdef _WINEMU
		XString16 s("---->Loading Filter:");
		s+=XString16(pInfo->strFilterURL);
		Trace(s);
#endif
		ConnectContext(pInfo->strFilterURL,XEM::GET,data,(void*)-1);
	}
	if(!m_pClientApply->m_bCookie) return;
	XU32 nc=pInfo->strCookies.GetSize();
	if(nc<=0) return;
	//XTime t;
    //t.SetCurrentTime();
	XU32 nTime=XTime::GetTickCount();
	for(XU32 i=0;i<nc;i++)
	{
	   SetURLCookie(url.m_strHost,pInfo->strCookies[i],nTime,pInfo);
	   //if(pInfo->strCookies[i].Find("COOKIE")>=0)
		//	  int v=0;
	   /*XString8Array list;
	   XString8 strPath="/",strCookie,strDomain="";
	   list.SplitString(pInfo->strCookies[i],';');
	   strCookie=list[0];
	   //if(list.GetSize()<2)
	   //	   strKey=pInfo->strCookies[i];
		   //XEnum::SetValue(m_strCookies,pInfo->strCookies[i],strPath);
	   //else
	   XU8 bOut=XFALSE;
	   if(list.GetSize()>1)
	   {
		   for(XU32 j=1;j<list.GetSize();j++)
		   {
			   int id=list[j].Find('=');
			   if(id>=4)
			   {
				   XString8 strKey;
				   list[j].Left(strKey,id);
				   if(strKey.Compare("path",XTRUE)==0)
					   list[j].Mid(strPath,5);
				   else if(strKey.Compare("domain",XTRUE)==0)
					   list[j].Mid(strDomain,7);
			   }
		   }
	   }
	   if(bOut) continue;
	   int id=strCookie.Find('=');
	   XString8 strKey,strValue;//strLeft;
	   if(id>0)
	   {
		   //if(id+1>=(int)strKey.GetLength())
		   //  continue;
		   strCookie.Mid(strValue,id+1);
		   strCookie.Left(strKey,id);
	   }
	   else
	   {   strKey=strCookie;
	   }
	   XEH eh;
	   switch(eh.Index(strKey,XString8::XLS_NOCASE))//==XEH::CLIENT_ID)
		   {
		   case XEH::CONFIG_URL:
			    {
				  //	if(!strValue.IsEmpty())
				  pInfo->strConfigURL=strValue;
			     //m_pClientApply->SetXID(strValue);
				}
			    break;
		   case XEH::CLIENT_ID:
			    m_pClientApply->SetXID(url.m_strHost,strValue,true);
				break;
		   case XEH::MOBILE_TYPE:
			   	{ if(!strValue.IsEmpty())
					 m_pClientApply->m_info.strUserAgent=strValue;
			     //m_pClientApply->SetXID(strValue);
			   }break;
		   }
	   if(strDomain.IsEmpty())
		   m_cookies.SetCookies(strPath,url.m_strHost,strKey,strValue,nTime);
	   else
		   m_cookies.SetCookies(strPath,strDomain,strKey,strValue,nTime);*/

	}
}

void XDomView::SetCookHost(XURL&url,XURL&url2)
{
	/*if(url.m_strHost==url2.m_strHost) return;
	if(url.m_strHost.IsEmpty()) return;
	//m_strCookies.RemoveAll();
	XString8Array list;
	for(int i=0;i<m_strCookies.GetSize();i+=3)
	{
		XString8&domain=m_strCookies[i+2];
		if(domain.IsEmpty()) continue;
		//int iStart=url2.m_strHost.Find(domain);
		//if(iStart<0||iStart+domain.GetLength()!=url.m_strHost.GetLength())
		//	continue;
		list.Add(m_strCookies[i]);
		list.Add(m_strCookies[i+1]);
		list.Add(m_strCookies[i+2]);
	}
	m_strCookies.RemoveAll();
	if(list.GetSize())
		m_strCookies.Append(list);*/
}

void XDomView::SetCookies(CONNECTREQ &req)
{
	if(!m_pClientApply->m_bCookie) return;
	XString8 strCookies;
	m_cookies.GetCookies(req.URL,strCookies);
	//if(!strCookies.IsEmpty())
	//	strCookies+=";COOKIEENABLE=true";
	/*XString8&strHost=req.URL.m_strHost;
	for(XU32 i=0;i<m_strCookies.GetSize();i+=3)
	{
		XString8&domain=m_strCookies[i+2];
		if(!domain.IsEmpty())
		{
			int iStart=strHost.Find(domain);
			if(iStart<0||iStart+domain.GetLength()!=strHost.GetLength())
				continue;
		}
		if(req.URL.m_strPath.Find(m_strCookies[i+1])!=0) continue;
		if(!strCookies.IsEmpty()) strCookies+=';';
		strCookies+=m_strCookies[i];
		//XEnumHeader h;
		//XEnum::SetValue(req.strHeaders,
		//				h[XEH::COOKIE],m_strCookies[i]);
	}*/
	if(!strCookies.IsEmpty())
	{
	   XEnumHeader h;
	   XSortString8::ESetValue(req.strHeaders,
		  			   h[XEH::COOKIE],strCookies);
	}
}

XBOOL XDomView::ProcJQScript(XClient*pClient,XDOMFILTER*pFilter)
{
	    if(!pFilter) return XTRUE;
		RESPONSEINFO*pInfo=pClient->GetResponseInfo();
		if(pInfo->nErrorCode==XHTTP_STATUS_OK)
		{
			//Modify FILTER
			//XDOMFILTER*pFilter=this->m_pClientApply->GetFilter(pClient->GetConnectInfo()->URL.m_strURL,false);
				//m_pDom->m_pFilter;//pClient->m_pCurFilter;//m_pClientApply->GetFilter(pClient->GetURL());
			if(!pFilter) return XTRUE;

			if(pFilter->pRetGO)
			{
				XRETGO*pGo=pFilter->pRetGO;
				int i,nc=pGo->strOptions.GetSize();
				XString8 strURL,strPost;
				int nMethod=XEM::GET;
				bool bDefault=false;
				for(i=0;i<nc;i++)
				{
					if(strURL.IsEmpty()&&pGo->strOptions[i].Compare("default",XTRUE)==0)
					{
						strURL=pGo->strURLs[i];
						nMethod=pGo->nMethods[i];
						strPost=pGo->strPosts[i];
						this->SetValueOf(strURL);
						this->SetValueOf(strPost);
						m_pDom->SetFilter(NULL);
						LoadingEnd();
						bDefault=true;
					}
					if(pGo->strOptions[i][0]=='*')
					{
						if(m_strCnt.Find(pGo->strOptions[i]+1,0)!=NULL)
						{
							strURL=pGo->strURLs[i];
							nMethod=pGo->nMethods[i];
							strPost=pGo->strPosts[i];
							this->SetValueOf(strURL);
							this->SetValueOf(strPost);
						}
					}
					else if(pGo->strOptions[i].Compare(m_strCnt,XTRUE)==0)
					{
						strURL=pGo->strURLs[i];
						nMethod=pGo->nMethods[i];
						strPost=pGo->strPosts[i];
						this->SetValueOf(strURL);
						this->SetValueOf(strPost);
					}
				}

				if(!strURL.IsEmpty())
				{
					#ifdef _WINEMU
					  XString16 si("--->Return Value(");
					  si+=XString16(m_strCnt);
					  si+=(XString16(") go:"));
					  si+=XString16(strURL);
					  Trace(si);
					#endif
					this->StopContent();
					if(bDefault)
					{
						XU8Array data;
						data.Append((XU8*)strPost.GetData(),strPost.GetSize());
						this->Connect(strURL,nMethod,NULL,data,NULL,NULL);
						return XFALSE;
					}
					 m_req.URL.SetURL(strURL);

					 if(pFilter->bModifyDomain)
					 {
						//m_pDom->m_argList.RemoveAll();
						XDom::GetArgList(m_pDom->GetURL().m_strURL,NULL,m_pDom->m_argList);
						m_pDom->m_bFrame=XFALSE;
						m_pDom->SetURL(m_req.URL);
					 }
					 SetCookies(m_req);
					 m_req.data.SetSize(0);
					 m_req.nMethod=nMethod;
					 if(!strPost.IsEmpty())
						  m_req.data.Append((XU8*)strPost.GetData(),strPost.GetLength());
					 if(pFilter->strAutoHeaders.GetSize())
						  m_req.strHeaders.Append(pFilter->strAutoHeaders);
					 m_req.strRefer=pFilter->strHost;
					 if(!pInfo->strClientID.IsEmpty())
						m_pClientApply->SetXID(pClient->GetConnectInfo()->URL.m_strHost,pInfo->strClientID,true);//p->strClientID);
					 m_saveReq.bForward=XCT_DIR;
					  //pClient->m_pFilter=pFilter;
					 //Modify FILTER
					 //pClient->SetFilter(m_pClientApply->GetFilter(strURL));
					 //if(pClient->m_pCurFilter)
					 //	  m_pDom->SetFilter(pClient->m_pCurFilter);
					  //StopContent();
					 pClient->Connect(m_req,pClient->GetResponseInfo()->type.m_nCharset);
					 //bLoadImage=XFALSE;
					 return XFALSE;
				}
			}
			if(pFilter&&!pFilter->strAutoLocation.IsEmpty())
			{
					  XString8 strURL=pFilter->strAutoLocation;
					  //if(strURL[0]=='$') return 0;
					  SetValueOf(strURL,NULL,XTRUE);
					#ifdef _WINEMU
					  XString16 si("--->Auto location:");
					  si+=XString16(strURL);
					  Trace(si);
					#endif
					  this->StopContent();
					  m_req.URL.SetURL(strURL);

					  if(pFilter->bModifyDomain)
					  {
						  //m_pDom->m_argList.RemoveAll();
						  XDom::GetArgList(m_pDom->GetURL().m_strURL,NULL,m_pDom->m_argList);
						  m_pDom->m_bFrame=XFALSE;
						  m_pDom->SetURL(m_req.URL);
					  }
					  SetCookies(m_req);
					  m_req.data.SetSize(0);
					  m_req.nMethod=pFilter->nMethod;
					  if(!pFilter->strAutoPost.IsEmpty())
						  m_req.data.Append((XU8*)pFilter->strAutoPost.GetData(),pFilter->strAutoPost.GetLength());
					  if(pFilter->strAutoHeaders.GetSize())
						  m_req.strHeaders.Append(pFilter->strAutoHeaders);
					  m_req.strRefer=pFilter->strHost;
					  if(!pInfo->strClientID.IsEmpty())
						m_pClientApply->SetXID(pClient->GetConnectInfo()->URL.m_strHost,pInfo->strClientID,true);//p->strClientID);
					  m_saveReq.bForward=XCT_DIR;
					  //pClient->m_pFilter=pFilter;
					  //Modify FILTER
					  //pClient->SetFilter(m_pClientApply->GetFilter(strURL));
					  //if(pClient->m_pCurFilter)
						//  m_pDom->SetFilter(pClient->m_pCurFilter);
					  //StopContent();
					  pClient->Connect(m_req,pClient->GetResponseInfo()->type.m_nCharset);
					  //bLoadImage=XFALSE;
					  return XFALSE;

			}
		}
		return XTRUE;
}

XBOOL XDomView::ProcHttpStatus(XClient *pClient,XBOOL bHtml,XBOOL&bLoadImage)
{
	RESPONSEINFO*pInfo=pClient->GetResponseInfo();
	if(bHtml)
	{
		m_nStatus=(XU16)pInfo->nErrorCode;
		m_req.nStatus=m_nStatus;
	}
	switch(pInfo->nErrorCode)
	{
	case XERR_USER_CANCELED:
		 if(pClient==m_pClient)
			bLoadImage=XFALSE;
		 return XTRUE;
	case XHTTP_STATUS_OK:
		 //Modify FILTER
		 if(pClient==m_pClient&&m_pDom->m_pFilter)//pClient->GetFilter())
		 {
			 m_pClient->SetUsing(true);
			 return XFALSE;
		 }
		 break;
	case XHTTP_STATUS_REDIRECT_KEEP_VERB:
	case XHTTP_STATUS_MOVED:
	case XHTTP_STATUS_REDIRECT:
	case XHTTP_STATUS_REDIRECT_METHOD:
	case XHTTP_STATUS_USE_PROXY:
		 if(pClient==m_pClient)
			bLoadImage=XFALSE;
		 if(pInfo->strLocation.IsEmpty())
		 {
			 if(bHtml)
			 //{
				MakeErrorPage(pInfo->nErrorCode,XNULL,XTRUE);
			//	SetStatus();
			 //}
			 bLoadImage=XFALSE;
		 }
		 else if(m_nMoveTimes<5)
		 {
#ifdef _SOURCECODE
			 if(bHtml) m_strSource.Empty();
#endif
#ifdef _WINEMU
			 XString16 str("--->Redirect to URL:");
			 str+=XString16(pInfo->strLocation);
			 this->Trace(str);
#endif
			 m_req.URL.SetURL(pInfo->strLocation);
			 if(bHtml)
			 {
				 //if(m_pDom->m_pFilter)
					// m_pDom->AddArgList(m_req.URL.m_strURL);
				 m_pDom->SetURL(m_req.URL);
			 }
			 SetCookies(m_req);
			 m_req.data.SetSize(0);
			 m_req.nMethod=XEM::GET;

			 if(!pInfo->strClientID.IsEmpty())
				m_pClientApply->SetXID(pClient->GetConnectInfo()->URL.m_strHost,pInfo->strClientID,true);//p->strClientID);
			 m_saveReq.bForward=XCT_DIR;
			 //Modify FILTER
			 //if(pClient==m_pClient)
			 //{
			 //	 pClient->SetFilter(m_pClientApply->GetFilter(m_req.URL.m_strURL));
			 //	 if(pClient->m_pCurFilter)
			 //		m_pDom->SetFilter(pClient->m_pCurFilter);
			  //}
			 StopContent();
			 pClient->Connect(m_req,pClient->GetResponseInfo()->type.m_nCharset);
			 m_nMoveTimes++;
			 return XFALSE;
		 }
		 else if(bHtml)
			 MakeErrorPage(pInfo->nErrorCode,pInfo->strLocation,XTRUE);
		 break;
	default:
		if(pClient==m_pClient)
			bLoadImage=XTRUE;
		if(bHtml)
			MakeErrorPage(pInfo->nErrorCode,m_req.URL.m_strURL,XTRUE);
		break;
	}
	return XTRUE;
}


void XDomView::MakeErrorPage(XU32 nCode, XPCTSTR strRetryURL, XU8 bBack)
{
	PostMsg(XWM_MAKEERRORPAGE, 0, 0, XMS_TOPMOST);
	if(!m_bPushed)
	{
		if(m_pDom&&m_pDom->m_pFilter==XNULL)
			PushREQ(XFALSE);
	}
	XString8 str;
	XResource::LoadText(str,"ERRORPAGE");
	int id=str.Find("%1");
	if(id>=0)
	{
	   XString8 strTmp;
	   if(!m_req.URL.m_strHost.IsEmpty())
		   strTmp=m_req.URL.m_strHost;
	   else if(!m_req.URL.m_strFile.IsEmpty())
		   strTmp=m_req.URL.m_strFile;
	   str.Delete(id,2);
	   str.Insert(id,strTmp);
	}
	id=str.Find("%2");
	if(id>=0)
	{
		XPCTSTR pStr=XEnumHttpStatus::GetErrorString((XU16)nCode);
		//if(pStr==XNULL)
		//	pStr=XResource::LoadWString(XIDS_UNKNOW);
		str.Delete(id,2);
		if(pStr) str.Insert(id,pStr);
		else
		{
			XPCWSTR strw=XResource::LoadWString(XIDS_UNKNOW);
			XString8 str8(strw);
			str.Insert(id,str8);
		}
	}
	id=str.Find("%3");
	if(id>=0)
	{
		 str.Delete(id,2);
		 if(strRetryURL)
			str.Insert(id,strRetryURL);
	}
	id=str.Find("%4");
	if(id>=0)
	{
		str.Delete(id,2);
		if(m_req.URL.m_strPath.GetLength())
		{
			XString8 strURL;
			m_req.URL.MakeHomeURL(strURL);
			str.Insert(id,strURL);
		}
	}
#ifdef _SOURCECODE
	m_strSource=str;
#endif
	m_domMenus.FreeAll();
	m_strTitle.Empty();
	m_pDom->Reset(XCHARSET_UTF8,XCNT::CTEXT,XCNT::TVND_WAP_WML,&m_draw);
	m_pDom->m_nStatus=nCode;
	m_pDom->PhaseXML((XU8*)str.GetData(),str.GetLength(),false,true);
	//ToDomView();
	if(m_pFlashView)
	{
		DestoryChild(m_pFlashView);
		AddCommand(XWM_COMMAND,XCMD_DOM_VIEW,0);
	}
	m_pDom->SetFilter(NULL);
	LoadingEnd();
	Layerout();
	m_bHome=XFALSE;
	SetStatus();
	Invalidate();
}

void XDomView::AddClick(XPCWSTR strTitle,XBOOL bSameHost)
{
	//if(m_req.bAuto) return;
	if(!m_bShowView) return;
	if(m_pDom->GetActiveCard()==XNULL) return;
	if(m_nStatus==XHTTP_STATUS_OK||
	   m_nStatus==XERR_USER_CANCELED)
	{
		//XString16 strTitle=((XMainWnd*)GetParent())->GetTitle(this);
		if(strTitle==XNULL)
			strTitle=m_pDom->GetEventData()->strTitle;
		m_pClientApply->Click(m_pDom->GetURL(),strTitle,bSameHost);
	}
}

void XDomView::ReleaseClient()
{
	/*if(m_pClient!=NULL)
	{
		m_pClient->Cancel();//Release(XTRUE);//bWait);
		//delete m_pClient;
		//m_waitClients.Add(m_pClient);
	}
	m_pClient=NULL;
	for(XU16 i=0;i<m_clients.GetSize();i++)
	{
		//m_clients[i]->SetWindow(NULL,0);
		m_clients[i]->Cancel();
		//m_clients[i]->Release(XTRUE);//bWait);
		//delete m_clients[i];
		//m_oldClients.Add(m_clients[i]);
	}*/
}

void XDomView::OnString(XPCWSTR buf)
{
	if(m_pFlashView)
		m_pFlashView->OnString(buf);
	else

	{
		//XLock lock(&m_ptLock);            // modify by hgl
		if(!m_ptLock.Lock(XLONGTIME)) return;
		ResetEventData();
		XU32 s=m_pDom->OnString(buf);
		ProcResult(s,false,true);
		if(s) SetStatus();
		m_ptLock.UnLock();
	}
}

XBOOL XDomView::OnChar(XWCHAR c)
{
	//LOGWHERE();

	if(IsTypeEvent())
	{
		if(m_pFlashView)
			m_pFlashView->OnChar(c);
		return XWindow::OnChar(c);
	}
	//XLock lock(&m_ptLock);               // modify by hgl
	LOGWHERE();
	if(!m_ptLock.Lock(XLONGTIME)) return XFALSE;
	ResetEventData();
	XU32 s=m_pDom->OnChar(c);
	ProcResult(s,false,true);
	if(s) SetStatus();
	m_ptLock.UnLock();
	return XWindow::OnChar(c);
}

void XDomView::OnMouseDown(XPoint &pt,int iPos)
{
	if(m_nLockClick)
		return;
	if(IsTypeEvent())
		return;

	if(iPos) return;
	XPoint ptt=pt;
//	m_bNeedDown=XFALSE;

	//XLock lock(&m_ptLock);            // modify by hgl
	if(!m_ptLock.Lock(XLONGTIME))
		return;

    ClientToDoc(pt);
	ResetEventData();
	XU32 s=m_pDom->OnMouseDown(pt.x,pt.y);
	if(s&XRCINPUT)
		m_bCapture=XTRUE;
	ProcResult(s,false,false);
	SetStatus();
#ifdef _SCROLLVIEW
#else
	if((m_nMaxX>0||m_nMaxY>0)&&!m_bCapture)
	{
	   //m_bScroll=XTRUE;
	   //if(!s)
		 //  m_bScroll=XTRUE;
	   m_bMouseDown=XTRUE;
	   m_nDownX=ptt.x;
	   m_nDownY=ptt.y;
	   m_nDownOffX=m_nOffX;
	   m_nDownOffY=m_nOffY;
	   m_nSpeedX=0;
	   m_nSpeedY=0;
	   m_nTickTime=XTime::GetTimeMSec();
	}
//	else
//		int v=0;
#endif
	m_ptLock.UnLock();

}

void XDomView::OnMouseUp(XPoint &pt,int iPos)
{
	if(m_nLockClick) 
		return;
//	if(iPos) return;
	m_bCapture=XFALSE;
	if(m_pFlashView) return;
	m_bMouseDown=XFALSE;
#ifdef _SCROLLVIEW
#else
	if(m_bScroll&&!m_bCapture)
	{
		SetSpeed(pt);
		SetCursor(XCUR_ARROW);
		m_bScroll=XFALSE;
		return;
	}
#endif

	if(IsTypeEvent()) return;

	if(iPos) return;
//	if(m_bNeedDown) return;

	//XLock lock(&m_ptLock);          // modify by hgl
	if(!m_ptLock.Lock(XLONGTIME)) return;

	ClientToDoc(pt);
	ResetEventData();
	m_pDom->OnMouseUp(pt.x,pt.y);
	XU32 s=m_pDom->OnClick(pt.x,pt.y);
	if(s&XRCONNECT)
		m_nLockClick=50;
	ProcResult(s,false,false);
	SetStatus();

	m_ptLock.UnLock();
}

XBOOL XDomView::ProcSysURL(CONNECTREQ&req)
{
    XURL url;//=req.URL;
	XString8 str=req.URL.m_strFile;
	//url.m_nProto=0;
	url.SetURL(str);
	//if(url.m_nProto>=XPROTO_RES||
	//   url.m_strHost.GetLength()<=0)
	{
		req.URL.SetURL(m_pClientApply->m_strHost);
		req.URL.SetURL(m_pClientApply->m_strSearch);
		XURL::Encode(str);
		req.URL.m_strFile+=str;
		req.URL.m_strURL+=str;
	}
	//else
	//{
	//	req.URL=url;
	//}
	return XTRUE;
}

void XDomView::ResetEventData()
{
	m_pDom->m_data.nData=0;
	m_draw.DATA=0;
//	m_pDom->m_data.strValues.RemoveAll();
	m_pDom->m_data.rect.SetRectEmpty();
	GetClipRect(m_pDom->m_data.viewRect,XTRUE);
	GetOffset(m_pDom->m_data.offset);
	GetClipRect(m_draw.paint,XTRUE);
}

XBOOL XDomView::AddDomMenu(XPCWSTR strTitle, XU32 data)
{
	if(m_domMenus.GetSize()>2) return XFALSE;
	XPMENUDATA p=new XMENUDATA;
	p->nCommand=XCMD_DOM_ITEM;
	p->nData=data;
	p->nIndex=0;
	p->nStyle=0;
	p->pImage=XNULL;
	p->strTitle=strTitle;
	m_domMenus.Add(p);
	return XTRUE;
}

void XDomView::OnFocus()
{

	XWindow::OnFocus();
	SetStatus();
	ResetEventData();
	if(m_pFlashView)
	{
		m_pFlashView->SetFocus(XTRUE);
		return;
	}
	ProcResult(m_pDom->OnFocus(),false,true);
}

void XDomView::OnBlur()
{
	XWindow::OnBlur();
	ResetEventData();
	if(m_pFlashView)
	{
		m_pFlashView->SetFocus(XFALSE);
		return;
	}
	ProcResult(m_pDom->OnBlur(),false,true);
}


void XDomView::OnDblClick(XPoint &pt,int iPos)
{
	if(m_pFlashView)
	{
		m_pFlashView->OnDblClick(pt,iPos);
		return;
	}
	if(iPos) return;
	//XLock lock(&m_ptLock);        // modify by hgl
	if(!m_ptLock.Lock(XLONGTIME)) return;
    ClientToDoc(pt);
	ResetEventData();
	ProcResult(m_pDom->OnDblClick(pt.x,pt.y),false,false);
	SetStatus();
	m_ptLock.UnLock();
}

XU8 XDomView::GetDocType(RESPONSEINFO*pInfo,CONNECTINFO*pCon)
{
	if(pInfo->type.m_nType==XCNT::TSWF) return XTYPE_FLASH;
	switch(pInfo->type.m_nClass)
	{
	case XCNT::CTEXT:
		 if(pInfo->type.m_nType==XCNT::TPLAIN)
		 {
		    XString8 str=pCon->URL.m_strURL;
			str.MakeLower();
			int index=str.Find(".swf");//str.ReverseFind('.');
			int idq=str.ReverseFind('?');
			if(index>0&&(idq>index||idq<0))
				return XTYPE_FLASH;
			/*if(index>0)
			{
				if(idq-index>=4||idq<0)
				{
					XString8 strExt;
					str.Mid(strExt,index+1,3);
					strExt.MakeLower();
					if(strExt=="swf")
							return XTYPE_FLASH;
				}
			}*/
			return XTYPE_DOM;
		 }break;
	case XCNT::CAPPLICATION:
		 switch(pInfo->type.m_nType)
		 {
		 case XCNT::TX_SHOCK_WAVE_FLASH:
				  return XTYPE_FLASH;
		 case XCNT::TOCTET_STREAM:
			 {
					XString8 str=pCon->URL.m_strURL;
					str.MakeLower();
					int index=str.Find(".swf");//str.ReverseFind('.');
					int idq=str.ReverseFind('?');
					if(index>0&&(idq>index||idq<0))
						return XTYPE_FLASH;

			 }
		 default:
			 return XTYPE_APP;
#ifdef _SYMBIAN
		 case XCNT::TVND_SYMBIAN_INSTALL:
				  return XTYPE_SIS;
#endif
		 }break;
	case XCNT::CAUDIO:
	case XCNT::CVIDEO:
		 return XTYPE_MEDIA;
	}
	return XTYPE_DOM;
}

void XDomView::ToFlashView(XU32 nTotal,XClient*pClient)
{
    
    //static bool sbinit = false;
    //if ( !sbinit )
    //{
    //
    //	sbinit = true;
    //}
#ifdef __ANDROID__
    PostMsg(XWM_UNBINGMAKECURRENT, 0, (XU32) this, XMS_THIS);//通知上个线程解绑
    WindowView::sharedWindowView()->initGL();
    RasterGL::sharedRasterGL()->Init();
    LOGE("XDomView::ToFlashView  L:3533 ");
#else
    if ( ! RasterGL::sharedRasterGL()->mbInit )
    {
#ifdef __APPLE__
        if(WindowView::sharedWindowView()->InitOK()){
#endif
            WindowView::sharedWindowView()->initGL();
            RasterGL::sharedRasterGL()->Init();
            RasterGL::sharedRasterGL()->mbInit = true;
#ifdef __APPLE__
        }
#endif
    }
#endif
    
#ifdef __APPLE__
    if (g_deviceData.mbOnSale && g_deviceData.m_scale > 1.1 && g_deviceData.m_isPad)
    {
        XRect reT(m_rect);
        reT.left *= g_deviceData.m_scale;
        reT.right *= g_deviceData.m_scale;
        reT.top *= g_deviceData.m_scale;
        reT.bottom *= g_deviceData.m_scale;
        //reT += reT;
        MoveWindow(reT);
    }
#endif
    
	m_bException=XFALSE;
	m_bShowEnd=XTRUE;
	m_bShowPageLoading =false;
#ifdef _SYMBIAN
#elif defined( __ANDROID__ )
	//m_view.bottom += GFunGetBrowerHeight();
#else
	m_status.ShowWindow(XFALSE);
#endif
#ifdef _SCROLLVIEW
	m_vBar.ShowWindow(XFALSE);
	m_hBar.ShowWindow(XFALSE);
#endif
	if(!m_pFlashView)
	{
		XMainWnd*pWnd = (XMainWnd*) GetParent();
		pWnd->FullScreen(XTRUE);
		m_bLoaded = XTRUE;
		m_bShowLoading = XFALSE;
		m_pFlashView = new XFlashView(this);
		PostMsg(XCMD_FLASHOPEN,0,0,XMS_TOPMOST);
		XRect rect = m_rect;
		rect -= rect.TopLeft();
#ifdef __APPLE__
        if( !g_deviceData.mbOnSale && g_deviceData.m_scale > 1.1)
        {
            rect.right = g_deviceData.m_screenWidth * g_deviceData.m_scale;
            rect.bottom = g_deviceData.m_screenHeight * g_deviceData.m_scale;
        }
        else
        {
            rect.right = g_deviceData.m_screenWidth;
            rect.bottom = g_deviceData.m_screenHeight;
        }
#endif
		m_pFlashView->Create(rect, this, nTotal, XCMD_FLASH);
		/*if(m_bShowLoading)
			m_pFlashView->ShowWindow(XFALSE);*/
		//else
		//	WindowView::sharedWindowView()->ShowView(true);
	}
	pClient->SetCallID(0);
	pClient->SetNotifyID(1);
#ifdef __ANDROID__
#else
	m_status.SetPecent(0);
#endif
	SetPercent(0);
	//m_pFlashView->NewStream(pClient,m_pDom->GetURL().m_strURL);
	//m_pFlashView->SetFocus(XTRUE);
    //LOGE("XDomView::ToFlashView  L:3599");
}

void XDomView::ToDomView(bool bStop,bool bBack)
{
	
	if(!m_pFlashView)
		return;
	//WindowView::sharedWindowView()->ShowView(false);
	m_pFlashView->EndPost();

#ifdef __APPLE__
	PostMsg(XWM_APPLE_MESSAGE, XCMD_END_FLASH, (XU32)this, XMS_TOPMOST);
#endif
//#ifdef __ANDROID__
//	PostMsg(ANDROID_END_FLASH, 0, (XU32)this, XMS_TOPMOST);
//#endif

	//XLock lock(&m_ptLock);        // modify by hgl
	if(m_ptLock.Lock(XLONGTIME))
	{
		DestoryChild(m_pFlashView);
		delete m_pFlashView;
		m_pFlashView=XNULL;
		//RasterGL::sharedRasterGL()->Release();
		//RasterGL::sharedRasterGL()->mbInit = false;
		//TextureCache::shareTexCache()->Release();
		
		XMainWnd*pWnd=(XMainWnd*)GetParent();
		pWnd->PostMsg(XCMD_FLASHMODECMD,0,0,XMS_TOPMOST);
		m_ptLock.UnLock();
	}
	else
	{
		PostMsg(XWM_COMMAND,XCMD_FLASH_CLOSE,0,XMS_THIS);
		return;
	}
	if(bStop&&m_pClient)
	{
		//Modify FILTER
		//m_pClient->SetFilter(XNULL);
		m_pClient->Cancel();
		m_pClient->SetUsing(false);
		m_pClient=XNULL;
	}

	if(bStop)
		m_clients.RemoveAll();
	XMainWnd*pWnd=(XMainWnd*)GetParent();
	pWnd->FullScreen(XFALSE);
#ifdef __ANDROID__
//	m_view.bottom -= GFunGetBrowerHeight();
#else
	if(m_bShowView)
	{
		m_status.SetPecent(0);
	}
#endif
	SetPercent(0);
	SetStatus();

	int nCT=m_req.bForward;
	if(bStop)
	{
		m_req.URL=m_saveReq.URL;
		m_req.Copy(m_saveReq);
	}
	if(bBack&&(nCT==XCT_DIR||m_req.bForward==XCT_DIR))
	{
		Back(XTRUE,bStop,false);
		FreeList(m_forList);
		SetStatus();
	}

	Invalidate();

#ifdef __ANDROID__
#else
	m_status.ShowWindow(XTRUE);
	m_status.Invalidate();
#endif
#ifdef _SCROLLVIEW
	m_vBar.ShowWindow(XTRUE);
#endif
	SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);
	//SetDocSize(m_view.right,m_view.bottom+m_nTextHeight);
#ifdef _SYMBIAN
	PostMsg(XWM_SYMBIAN_COMMAND, XCMD_FLASH_TO_DOM_VIEW, (XU32)this, XMS_TOPMOST);
#endif
	PostMsg(XCMD_FLASHCLOSE,0,0,XMS_TOPMOST);
	m_bShowPageLoading = false;
}

void XDomView::InitTypeData(XClient *pClient)
{
	if(m_pFlashView)
		m_pFlashView->NewStream(pClient,m_pDom->GetURL().m_strURL);
}

XBOOL XDomView::SetTypeData(XU8Array&data,int nSize,XClient*pClient)
{
	if(m_pFlashView)
	{
		if(!m_pFlashView->AppendData(data,nSize,pClient))
			pClient->Cancel();
		return XTRUE;
	}
	return XFALSE;
}

XBOOL XDomView::IsTypeEvent()
{
	if(m_pFlashView) return XTRUE;
	return XFALSE;
}

XBOOL XDomView::OnKeyUp(XKEYMSG&m)
{
	if(m_pFlashView)
		return m_pFlashView->OnKeyUp(m);
	switch(m.nKeyCode)
	{
	case XK_LEFTSOFT:
	case XK_RIGHTSOFT:
		 return XFALSE;
	}
	return XTRUE;
}

/*static void SetRequest(CONNECTREQ&req,XPCTSTR url,int method,XU8Array&data,const char*strType,XString8Array*headers)
{

	req.nMethod=method;
	if(strType)
	{
		req.strContentType=strType;
		req.nMethod=XEM::POST;
	}
	req.URL=m_pDom->GetURL();
	req.URL.SetURL(url);
	if(headers)
		req.strHeaders.Append(*headers);

	if(data.GetSize())
	{
		req.data.Append(data);
		const char* strCntTypeURL="Content-type: application/x-www-form-urlencoded";
		const char* strCntType="Content-type: ";
		const char* strCntLength="\nContent-length: ";
		//const char* strCntType=XEAB::Get
	    //XU8Array ub;
		if(info->strContext.IsNull())
			req.data.Append((XU8*)strCntTypeURL,strlen(strCntTypeURL));
		else
		{
			req.data.Append((XU8*)strCntType,strlen(strCntType));
			req.data.Append(info->strContext.strTxt,info->strContext.GetLength());
		}
		req.data.Append((XU8*)strCntLength,strlen(strCntLength));
		XXVar s=_strEmpty;
		s.AppendInt(info->postData.GetSize());
		req.data.Append(s.strTxt,s.GetLength());
		req.data.Append("\n\n",2);
		req.data.Append(info->postData.GetData());
		req.data.Append("\n",1);
	}
}*/

void XDomView::ConnectEx(XPCTSTR url, int method, XPCTSTR window, XU8Array &data,const char*strType,XString8Array*headers)
{
	CONNECTREQ& req=m_laterREQ;
	req.bForward=m_req.bForward;
	req.nMethod=method;
	if(strType)
	{
		req.strContentType=strType;
		req.nMethod=XEM::POST;
	}
	req.URL=m_pDom->GetURL();
	req.URL.SetURL(url);
	if(headers)
		req.strHeaders.Append(*headers);

	if(data.GetSize())
	{
		req.data.Append(data);
	}
	AddCommand(XWM_COMMAND,XCMD_DOM_CONNECTEX,0);
	/*if(!window||!*window)
	{
		//ToDomView();
		Connect(req,XTRUE);
	}
	else
	{
		if(m_reqList.GetSize()>0)
		{
			int id=m_reqList.GetSize()-1;
			delete m_reqList[id];
			m_reqList.RemoveAt(id);
			SetStatus();
		}
		XMainWnd*p=(XMainWnd*)GetParent();
		p->CreateView(req,XNULL);
	}*/
}

void XDomView::Connect(XPCTSTR url, int method, XPCTSTR window, XU8Array &data,const char*strType,XString8Array*headers)
{
	CONNECTREQ req;
	req.bForward=m_req.bForward;
	req.nMethod=method;
	if(strType)
	{
		req.strContentType=strType;
		req.nMethod=XEM::POST;
	}
	req.URL=m_pDom->GetURL();
	req.URL.SetURL(url);
	if(headers)
		req.strHeaders.Append(*headers);

	if(data.GetSize())
	{
		req.data.Append(data);
	}
	if(!window||!*window)
	{
		//ToDomView();
		Connect(req,XTRUE);
	}
	else
	{
		if(m_reqList.GetSize()>0)
		{
			int id=m_reqList.GetSize()-1;
			delete m_reqList[id];
			m_reqList.RemoveAt(id);
			SetStatus();
		}
		XMainWnd*p=(XMainWnd*)GetParent();
		p->CreateView(req,XNULL);
	}
}

void XDomView::ConnectPost(XPCTSTR strURL, int method, XU8Array &data)
{
	CONNECTREQ req;
	req.bForward=m_req.bForward;
	if(!data.GetSize())
		method=XEM::GET;
	req.nMethod=method;
	req.URL=m_pDom->GetURL();
	req.URL.SetURL(strURL);

	if(data.GetSize())
	{
		if(req.nMethod==XEM::POST)
			req.data.Append(data);
		else
		{
			XString8 url=req.URL.m_strURL;
			if(url.Find('?')>=0)
				url+="&";
			else url+="?";
			url+=XString8((char*)data.GetData(),data.GetSize());
			req.URL.SetURL(url);
		}
	}
	this->StartFile(req.URL.m_strURL);
	//this->SetCook
	this->SetCookies(req);
	XClient*pClient=m_pClientApply->CreateClient(req,0,0,this);//new CClient(XFALSE);
	pClient->SetPostOnly();
	if(pClient!=XNULL)
	{
		pClient->Connect(req,XTRUE);
		//pData->nStatus=XL_LOADING;
	}
}

XClient* XDomView::ConnectContext(XPCTSTR strURL, int method, XU8Array &data, void *notify,
								  const char*refer,const char*strType,XString8Array*headers,bool bCache)
{
	CONNECTREQ req;
	req.bForward=m_req.bForward;
	if(!data.GetSize())
	{
		method=XEM::GET;
//#if (__CORE_VERSION__>=0x02080000)
		strType=NULL;
//#endif
	}
	req.nMethod=method;
	if(refer)
		req.strRefer=refer;
	if(strType)
	{
		req.strContentType=strType;
		req.nMethod=XEM::POST;
	}
	req.URL=m_pDom->GetURL();
	req.URL.SetURL(strURL);
	if(headers)
		req.strHeaders.Append(*headers);

	if(data.GetSize())
	{
		if(req.nMethod==XEM::POST)
			req.data.Append(data);
		else
		{
			XString8 url=req.URL.m_strURL;
			if(url.Find('?')>=0)
				url+="&";
			else url+="?";
			url+=XString8((char*)data.GetData(),data.GetSize());
			req.URL.SetURL(url);
		}
	}
	this->StartFile(req.URL.m_strURL);
	//this->SetCook
	this->SetCookies(req);
	XClient*pClient=m_pClientApply->CreateClient(req,0,((XU32)notify),this);//new CClient(XFALSE);
	if(pClient!=XNULL)
	{
		if(!bCache)
			pClient->SetNeedCache(false);
		if(pClient->Connect(req,XTRUE,XTRUE)==2)
		{
			OnStarted(pClient);
			OnRecive(pClient);
			OnFinished(pClient);
			//m_pClientApply->Catch(pClient);

		}
		else
		{	//bImage=XTRUE;
			//m_clients.Add(pClient);
			//pData->nStatus=XL_LOADING;
			//pData->SetStatus(XL_LOADING);
			//UpdateImage(pData,XL_LOADING);
		}

		//pData->nStatus=XL_LOADING;
	}

	return pClient;
}

void XDomView::OnMove(XRect &rect)
{
	if(m_pFlashView)
	{
		XRect rect=m_rect;
		rect-=rect.TopLeft();
		m_pFlashView->MoveWindow(rect);
		m_rect=rect;
		m_view=rect;
		m_view-=rect.TopLeft();
		return;
	}
	XImage* p =	GetIcon(XICON_GREENBLOCK);

	m_rect=rect;
	m_view=rect;
	m_view-=rect.TopLeft();
#ifdef __APPLE__
#elif defined(_SYMBIAN)
#elif defined(__ANDROID__ )
//	m_view.bottom -= GFunGetBrowerHeight();
#else
	int nh = XMAX((int)p->Height(), m_nTextHeight + 4);
	m_view.bottom-=nh;
	m_status.MoveWindow(XRect(0,rect.Height()-nh,rect.right,rect.Height()));
#endif

	//m_pDom->m_docSize;
#ifdef _SCROLLVIEW
	m_view.right-=XSSIZE;
	m_vBar.MoveWindow(XRect(m_view.right,0,m_rect.Width(),m_view.bottom));
	//int y=m_rect.Height()-XSTATUSH-XSHSIZE;
	m_hBar.MoveWindow(XRect(0,m_view.bottom-XSSIZE,m_view.right+1,m_view.bottom));
	SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);//m_view.right,m_view.bottom+m_nTextHeight);
#else
	SetDocSize(m_pDom->m_docSize.cx,m_pDom->m_docSize.cy);
	//SetDocSize(m_view.right,m_view.bottom);
#endif

	InitContext();
	Invalidate();
	return;
}

XPCTSTR XDomView::GetWorkPath()
{
	return m_pClientApply->m_strWorkPath;
}


XBOOL XDomView::GetTextEdits(XSObjectArray &list)
{
	if(!m_pFlashView) return XFALSE;


	m_pFlashView->GetEditTexts(list);

	return list.GetSize();
}

XSWFPlayer* XDomView::GetPlayer()
{
	if(!m_pFlashView) return XNULL;
	return m_pFlashView->GetPlayer();
}

void XDomView::SetSpeed(const XPOINT&pt)
{
	int t=XTime::GetTimeMSec()-m_nTickTime;
	int dx=m_nDownX-pt.x;
	int dy=m_nDownY-pt.y;

	if(!t)
	{
		m_nSpeedX=dx/2;
		m_nSpeedY=dy/2;
	}
	else if(t<300)
	{
		m_nSpeedX=dx*200/t;
		m_nSpeedY=dy*200/t;
	}
	else
	{
		m_nSpeedX=0;
		m_nSpeedY=0;
	}

	if(XABS(m_nSpeedX)<20) m_nSpeedX=0;
	if(XABS(m_nSpeedY)<20) m_nSpeedY=0;
	if(m_nSpeedX||m_nSpeedY)
		int v=0;
}

void XDomView::FreeList(REQARRAY &list)
{
	int i,nc=list.GetSize();
	for(i=0;i<nc;i++)
	{
		if(list[i]->pDom)
			FreeDom(list[i]->pDom);
		delete list[i];
	}
	list.RemoveAll();
}

void XDomView::ReleaseCall()
{
	ToDomView();
	RasterGL::Release();
	//RasterGL::Uninit();
}

void XDomView::CallBack(XThread&thread)
{
//    if ( ! RasterGL::sharedRasterGL()->mbInit )
//    {
//        WindowView::sharedWindowView()->initGL();
//        RasterGL::sharedRasterGL()->mbInit = true;
//        RasterGL::sharedRasterGL()->Init();
//    }
    //static bool bInit = false;
#ifdef __APPLE__
    if ( ! RasterGL::sharedRasterGL()->mbInit )
    {

        if(WindowView::sharedWindowView()->InitOK()){

            WindowView::sharedWindowView()->initGL();
            RasterGL::sharedRasterGL()->Init();
            RasterGL::sharedRasterGL()->mbInit = true;
            //bInit = true;

        }

    }
#endif
	if(m_ptLock.Lock(XLONGTIME))
	{
		while(m_nCmdGet!=m_nCmdSet)
		{
			switch(m_nCmds[m_nCmdGet])
			{
			case XWM_COMMAND:
				switch(m_nCmds[m_nCmdGet+1])
				{
				case XCMD_DOM_CONNECTEX:
					Connect(m_laterREQ);
					break;
				case XCMD_DOM_VIEW:
					ToDomView();
					break;
				case XCMD_DOM_VIEWEX:
					ToDomView(false);
					break;
				case XCMD_DOM_RECONNECT:
					m_pDom->Reset(XCHARSET_GB2312,0,0,&this->m_draw);
					ToDomView(true,false);
					ReConnect((XDOMFILTER*)m_nCmds[m_nCmdGet+2]);
				}break;
			case XWM_SOCKET:
				HandleSocket(m_nCmds[m_nCmdGet+1],(XClient*)m_nCmds[m_nCmdGet+2]);
				break;
			}
			m_nCmdGet+=3;
			if(m_nCmdGet>=MAX_CMDS)
				m_nCmdGet-=MAX_CMDS;
		}
		//if(m_pDom->Layerout(m_draw))
		//	Invalidate();
		if(m_pFlashView)
			m_pFlashView->CallBack(thread);
		m_ptLock.UnLock();
	}


}

XBOOL XDomView::GetCookies(XString8&strCookies)
{
	return m_cookies.GetCookies(m_pDom->GetURL(),strCookies);
	/*XString8Array&cookies=GetCookies();
	if(cookies.GetSize()<=0) return XFALSE;
	for(XU32 i=0;i<cookies.GetSize();i+=3)
	{
		//if(req.URL.m_strPath.Find(m_strCookies[i+1])!=0) continue;
		if(!strCookies.IsEmpty()) strCookies+=';';
		strCookies+=cookies[i];
		//XEnumHeader h;
		//XEnum::SetValue(req.strHeaders,
		//				h[XEH::COOKIE],m_strCookies[i]);
	}
	return XTRUE;*/

}
XBOOL XDomView::GetCookieOf(XString8&strName,XString8&strCookie)
{
	return m_cookies.GetCookieOf(m_pDom->GetURL(),strName,strCookie);
	/*XString8Array&cookies=GetCookies();
	if(cookies.GetSize()<=0) return XFALSE;
	strName.MakeLower();
	for(XU32 i=0;i<cookies.GetSize();i+=3)
	{
		XString8&str=cookies[i];
		XString8Array list;
		list.SplitString(str,';');
		for(XU32 j=0;j<list.GetSize();j++)
		{
			XString8& s=list[j];
			int id=s.Find('=');
			if(id<=0) continue;
			XString8 strKey;
			s.Left(strKey,id);
			strKey.MakeLower();
			strKey.TrimRight();
			strKey.TrimLeft();
			if(strKey==strName)
			{
				s.Mid(strCookie,id+1);
				strCookie.TrimLeft();
				strCookie.TrimRight();
				XURL::Decode(s);
				return XTRUE;
			}
		}
	}
	return XFALSE;*/
}


XBOOL XDomView::SetValueOf(XString8&strVar,const char*pArgs,bool bURL)
{
	XString8 strNew,strWord;
	char*buf=strVar;
	int i,nCount=strVar.GetLength();
	bool bStart=false,bSet=false,bVar=false;
	for(i=0;i<nCount;i++)
	{
		switch(buf[i])
		{
		case ')':
			if(bStart)
			{
				int ida=strWord.Find('@',2);
				if(ida<=0)
				{
					SetVarOf(strWord,NULL,pArgs,bURL);
					strNew+=strWord;
				}
				else
				{
					XString8 strDomain;
					strWord.Mid(strDomain,ida+1);
					strWord.GetData()[ida]=0;
					SetVarOf(strWord,strDomain,pArgs,bURL);
					strNew+=strWord;
				}
				bSet=true;
				bStart=false;
			}
			else
				strNew+=buf[i];
			break;
		case '@':
		case '#':
		case '$':
			if(bStart)
				strWord+=buf[i];
			else if(buf[i+1]=='(')
			{
				bStart=true;
				strWord.Empty();
				strWord+=buf[i];
				i++;
				break;
			}
			else //bVar=true;
			{
				//bVar=true;
				bSet=true;


				//SetVarOf(strVar);
				strWord.SetString(buf+i,nCount-i);
				int ida=strWord.Find('@',1);
				if(ida<=0)
				{
					SetVarOf(strWord,NULL,pArgs,bURL);
				}
				else
				{
					XString8 strDomain;
					strWord.Mid(strDomain,ida+1);
					strWord.GetData()[ida]=0;
					SetVarOf(strWord,strDomain,pArgs,bURL);
				}

				strNew+=strWord;
				i=nCount;
			}break;
		default:
			if(!bStart)
				strNew+=buf[i];
			else
				strWord+=buf[i];
			break;
		}
	}
	//if(bSet)
	strVar=strNew;
	//if(bVar)
	//	SetVarOf(strVar);
	return bSet;
}

XBOOL XDomView::SetVarOf(XString8&strVar,const char*strDomain,const char*pArg,bool bURL)
{

	//XString8 str="CommonRequest.a?req=ErrorMsg&EM=%E4%BA%B2%E7%88%B1%E7%9A%84%E7%8E%A9%E5%AE%B6%EF%BC%8C%E6%82%A8%E8%BF%98%E6%9C%AA%E7%99%BB%E5%BD%95%E6%9C%AC%E6%B8%B8%E6%88%8F%EF%BC%8C%E8%AF%B7%E5%9C%A8%E6%B8%B8%E6%88%8F%E9%A6%96%E9%A1%B5%E7%99%";
	
	//XURL::Decode(str);
	//XString16 str16(str);

	int i,iEnd=strVar.GetLength();
	XString8Array args;
	if(pArg)
		args.Add(pArg);
	for(i=0;i<iEnd;i++)
	{
		char c=strVar[i];
		if(c<=' ') continue;
		bool bEQ=false;
		bool bEN=false;
		bool bDE=false;
		bool bNum=false;
		bool bIndex=false;
		bool bArg=false;
		bool bMatch=false;
//#if (__CORE_VERSION__>=0x02080000)
		bool bAD=false;
//#endif
		//bool bStart=false;
		int iOff=1;
		switch(c)
		{
		case '$':bEQ=false;bEN=false;
				 switch(strVar[i+1])
				 {
				 case '=':
					 bMatch=true;
					 iOff=2;
					 break;
				 case '^':
					 bArg=true;
					 iOff=2;
					 break;
				 case '@':
					 bEN=true;iOff=2;break;
				 case '#':
					 bDE=true;iOff=2;break;
				 case '$':
					 bIndex=true;iOff=2;break;
				 case '&':
					 bNum=true;iOff=2;break;
//#if (__CORE_VERSION__>=0x02080000)
				 case '*':
					 bAD=true;iOff=2;break;
//#endif
				 }
				 break;
		case '@':bEQ=false;bEN=true;
			     switch(strVar[i+1])
				 {
				 case '=':
					 bMatch=true;
					 iOff=2;
					 break;
				 case '^':
					 bArg=true;
					 iOff=2;
					 break;
				 case '@':
					 bEN=true;iOff=2;break;
				 case '#':
					 bDE=true;iOff=2;break;
				 case '$':
					 bIndex=true;iOff=2;break;
				 case '&':
					 bNum=true;iOff=2;break;
//#if (__CORE_VERSION__>=0x02080000)
				 case '*':
					 bAD=true;iOff=2;break;
//#endif
				 }
				 break;
				 break;
		case '#':bEQ=true;bEN=false;
			    switch(strVar[i+1])
				 {
				 case '=':
					 bMatch=true;
					 iOff=2;
					 break;
				 case '^':
					 bArg=true;
					 iOff=2;
					 break;
				 case '@':
					 bEN=true;iOff=2;break;
				 case '#':
					 bDE=true;iOff=2;break;
				 case '$':
					 bIndex=true;iOff=2;break;
				 case '&':
					 bNum=true;iOff=2;break;
//#if (__CORE_VERSION__>=0x02080000)
				 case '*':
					 bAD=true;iOff=2;break;
//#endif
				 }
				 break;
				break;
		default:return XFALSE;
		}
		XString8 name;
		if(bArg&&pArg)
		{
			XString8 temp;
			strVar.Mid(temp,i+iOff);
			if(temp.IsEmpty())
				name=pArg;
			else
			{
				name=temp;
				name+=pArg;
				name+=temp;
			}
		}
		else
			strVar.Mid(name,i+iOff);
		strVar.Empty();
		GetVarOf(name,args,strVar,strDomain,bEQ,bIndex,bMatch);
		if(bNum)
			XURL::DecodeNum(strVar);
		else if(bEN)
		{		
			if(bURL)
				XURL::Encode0(strVar);
			else
				XURL::Encode(strVar);
		}
		else if(bDE)
		{
			XURL::Decode(strVar);
		}
//#if (__CORE_VERSION__>=0x02080000)
	    if(bAD)
		{
			int i,nc=strVar.GetLength()-1;
			for(i=nc;i>=0;i--)
			{
				char c=strVar[i];
				if(c=='\\')
				{
					strVar.Delete(i,1);
				}
			}
		}
//#endif
		//else if(bURL)
		//	XURL::Encode0(strVar);
		return XTRUE;
		/*else if(c=='$')
		{
			XString8 name;
			strVar.Mid(name,i+1);
			strVar.Empty();
			GetVarOf(name,args,strVar);
			return XTRUE;
		}
		else if(c=='#')
		{
			XString8 name;
			strVar.Mid(name,i+1);
			strVar.Empty();
			GetVarOf(name,args,strVar,true);
			return XTRUE;
		}
		else break;*/
	}
	return XFALSE;
}


static const char* _strSystemVar[]=
{
	"XVAR_ALERT",
	"XVAR_ARG",
	"XVAR_ARGS",	
	"XVAR_BASE",
	"XVAR_CALL",
//#if (__CORE_VERSION__>=0x02079000)
	"XVAR_CALLARG",
//#endif
	"XVAR_CLOSE",
	"XVAR_COOKIE",
	"XVAR_COOKIES",
	"XVAR_DATE",
	"XVAR_DOMAIN",
	"XVAR_HOST",
	"XVAR_OPENWINDOW",
	"XVAR_PASSWORD",
	"XVAR_PATH",
	"XVAR_PORT",
	"XVAR_PROTO",
	"XVAR_RAND",
	"XVAR_RANDNAME",
	"XVAR_REFER",
	"XVAR_REFERBASE",
	"XVAR_REFERDOMAIN",
	"XVAR_REFERHOST",
	"XVAR_REFERPATH",
	"XVAR_REFERROOT",
	"XVAR_REFERSH",
	"XVAR_ROOT",
	"XVAR_TIME",
	"XVAR_TRACE",
	"XVAR_URL",
	"XVAR_URL0",
	"XVAR_USER",
};

enum
{
	XVAR_ALERT,
	XVAR_ARG,
	XVAR_ARGS,	
	XVAR_BASE,
	XVAR_CALL,
//#if (__CORE_VERSION__>=0x02079000)
	XVAR_CALLARG,
//#endif
	XVAR_CLOSE,
	XVAR_COOKIE,
	XVAR_COOKIES,
	XVAR_DATE,
	XVAR_DOMAIN,
	XVAR_HOST,
	XVAR_OPENWINDOW,
	XVAR_PASSWORD,
	XVAR_PATH,
	XVAR_PORT,
	XVAR_PROTO,
	XVAR_RAND,
	XVAR_RANDNAME,
	XVAR_REFER,
	XVAR_REFERBASE,
	XVAR_REFERDOMAIN,
	XVAR_REFERHOST,
	XVAR_REFERPATH,
	XVAR_REFERROOT,
	XVAR_REFERSH,
	XVAR_ROOT,
	XVAR_TIME,
	XVAR_TRACE,
	XVAR_URL,
	XVAR_URL0,
	XVAR_USER,
};

XURL& XDomView::GetReferURL()
{
	//int nSize=m_reqList.GetSize();
	//if(nSize<=1) return NULL;
	//return m_reqList[nSize-1];
	return m_pDom->m_referURL;
}

//#if (__CORE_VERSION__>=0x02071000)
static XBOOL IsFunctionDefine(XString8&strCnt,const char*strKey,int iSet)
{	
	int len=strlen(strKey);
	char*str=strCnt;
	iSet--;
	while(iSet>=len)
	{
		switch(str[iSet])
		{
		case ' ':
		case '\r':
		case '\n':
			break;
		case '/':
			if(str[iSet-1]=='*')
			{
				iSet-=2;
				XBOOL bEcho=XFALSE;
				while(iSet>=len)
				{
					if(str[iSet]=='*'&&
					   str[iSet-1]=='/')
					{
						bEcho=XTRUE;
						iSet--;
						break;
					}
					iSet--;
				}
				if(!bEcho) return XFALSE;
			}
			else return XFALSE;
		default:
			{
				XString8 strTmp;
				strCnt.Mid(strTmp,iSet-len+1,len);
				strTmp.MakeLower();
				if(strTmp==strKey)
					return XTRUE;
				return XFALSE;
			}
		}
		iSet--;
	}
	return XFALSE;
	/*int iBegin=0,iLast=-1;
	while(iBegin<iSet)
	{
		iBegin=strCnt.Find(strKey,iBegin);
		if(iBegin<0||iBegin>=iSet) break;
		iLast=iBegin+8;
		iBegin+=8;
	}
	if(iLast<0) return XFALSE;
	char* chars=strCnt;
	while(iLast<iSet)
	{
		switch(chars[iLast])
		{
		case ' ':
		case '\r':
		case '\n':
			break;
		case '/':
			if(chars[iLast+1]=='*')
			{
				iLast+=2;
				XBOOL bEcho=XFALSE;
				while(iLast<iSet)
				{
					if(chars[iLast]=='*'&&chars[iLast+1]=='/')
					{
						iLast++;
						bEcho=XTRUE;
						break;
					}
					iLast++;
				}
				if(!bEcho) return XFALSE;
			}
			else
				return XFALSE;
			break;
		default:
			return XFALSE;
		}
		iLast++;
	}
	return XTRUE;*/

}
//#endif

XBOOL XDomView::GetContextOf(XString8&strCnt,XString8&strResult,int iSet,bool bMatch,bool bTrim)
{
		char cMark=0,cMarkEnd=0;
		int cBegin=0,cEnd=0,iEnd=strCnt.GetLength(),iPush=0;
		bool bMust=true;
		while(iSet<iEnd)
		{
			char c=strCnt[iSet];
			switch(c)
			{
			case ' ':
			case '=':
				if(!bMatch)
					bMust=false;
				break;
			default:
				if(bMust&&!iPush)
					iSet=iEnd;
				break;
			case '(':
			case '[':
			case '{':
				if(iPush==0)
				{
					cMark=c;
					iPush++;
					cBegin=iSet+1;
					switch(c)
					{
					case '(':cMarkEnd=')';break;
					case '[':cMarkEnd=']';break;
					case '{':cMarkEnd='}';break;
					}
				}
				else if(c==cMark)
					iPush++;
				break;
			case ')':
			case '}':
			case ']':
				if(iPush&&c==cMarkEnd)
				{
					iPush--;
					if(!iPush)
					{
						cEnd=iSet;
						iSet=iEnd;
					}
				}
				else
					iSet=iEnd;
				break;

			}
			iSet++;
		}
		if(cEnd>cBegin)
		{
			strCnt.Mid(strResult,cBegin,cEnd-cBegin);
			if(bTrim)
			{
				strResult.TrimLeft('\'');
				strResult.TrimRight('\'');
				strResult.TrimLeft('"');
				strResult.TrimRight('"');
			}
			return XTRUE;
		}
		return XFALSE;
}

XBOOL XDomView::GetVarOf(XString8&strName,XString8Array&args,XString8&strValue,const char*strDomain,bool bFun,bool bIndex,bool bMatch)
{
	if(strName.IsEmpty()) return XFALSE;
	if(!bFun)
	{
		XSortConstStr sort(_strSystemVar,sizeof(_strSystemVar)/sizeof(XPCTSTR));
		int idd=strName.Find(':');
		//XString8Array list;
		//XString8Array list;
		//list.SplitString(strName,':');
		XString8 strKey,strArg;
		int id=-1;
		XString8Array argList;
		if(idd>0)
		{
			//XString8 strArg;
			strName.Left(strKey,idd);
			strName.Mid(strArg,idd+1);
			id=sort.Index(strKey,XSS_NOCASE);
			args.InsertAt(0,strArg);
			//argList.Add(strArg);
		}
		else
	
		{
			id=sort.Index(strName,XSS_NOCASE);
		}
		if(strArg.IsEmpty()&&args.GetSize())
			strArg=args[0];
		//if(args.GetSize())
		//{
		//	argList.Append(args);
		//}
		//int id=sort.Index(list[0],XSS_NOCASE);
		if(id>=0)
		{
			switch(id)
			{
			case XVAR_ALERT:
				 //break;
			case XVAR_TRACE:
				if(!strArg.IsEmpty())
				 {
					 XString16 s(strArg);
					 this->Trace(s);
				 }
				 break;
			case XVAR_CLOSE:
				 if(m_pFlashView)
				 {
					 m_pFlashView->CloseFlash();
				 }break;
			case XVAR_OPENWINDOW:
				 if(!strArg.IsEmpty()&&m_pFlashView)
				 {
					m_pFlashView->DoPay(strArg);//OpenWindow(strArg);//list[1]);
				 }
				 break;
//#if (__CORE_VERSION__>=0x02079000)
			case XVAR_CALLARG:
				 if(args.GetSize()>=2)
				 {
					 int nArg=args[0].ToInt();
					 if(args.GetSize()>nArg)
						 strValue=args[nArg];					 
				 }
				 break;
//#endif
			case XVAR_CALL:
				 break;
			case XVAR_REFERSH:
				 this->ReConnectEx(m_pDom->m_pFilter);
				 break;
			case XVAR_RAND:
				{
					strValue.FromInt(XGlobal::RandInt());
				}break;
			case XVAR_REFER:
				{
					XURL&url=GetReferURL();
					//if(pReq)
					{
						strValue=url.m_strURL;
					}
				}break;
			case XVAR_REFERROOT:
				{
					XURL&url=GetReferURL();
					//if(pReq)
					{
						//XURL&url=pReq->URL;
						strValue=url.m_strProtocol;
						strValue+=url.m_strHost;
						if(url.m_uPort!=80)
						{
							XString8 s;
							s.FromInt(url.m_uPort);
							strValue+=":";
							strValue+=s;
						}
					}
				}
				break;
			case XVAR_REFERBASE:
				{
					XURL&url=GetReferURL();
					//if(pReq)
					{
						//XURL&url=pReq->URL;
						strValue=url.m_strProtocol;
						strValue+=url.m_strHost;
						if(url.m_uPort!=80)
						{
							XString8 s;
							s.FromInt(url.m_uPort);
							strValue+=":";
							strValue+=s;
						}
					}
					strValue+=url.m_strPath;
				}
				break;
			case XVAR_REFERDOMAIN:
			case XVAR_REFERHOST:
				{
					{
					XURL&url=GetReferURL();
					//if(pReq)
					{
						strValue=url.m_strHost;
					}
				}break;
				}break;
			case XVAR_REFERPATH:
				{
					XURL&url=GetReferURL();
					//if(pReq)
					{
						strValue=url.m_strPath;
					}
				}break;
			case XVAR_URL0:
				{
					XString8&strURL=GetDom()->GetDomURL().m_strURL;
					int id=strURL.Find('?');
					if(id<=0)
						strValue=strURL;
					else
						strURL.Left(strValue,id);
				}break;
			case XVAR_URL:
				 strValue=GetDom()->GetDomURL().m_strURL;
				 break;
			case XVAR_ARG:
				if(!strArg.IsEmpty())//list.GetSize()>=2)
				{
					strName=strArg;//list[1];
					strName+='=';
					XString8&url=GetDom()->GetDomURL().m_strURL;
					int id=url.Find('?');
					if(id>=0)
					{
						id=url.Find(strName,id);
						if(id>=0)
						{
							id+=strName.GetLength();
							int ie=url.Find('&',id);
							if(id==ie)
								strValue.Empty();
							else if(ie>id)
								url.Mid(strValue,id,ie-id);
							else
								url.Mid(strValue,id);
						}
					}
				}break;
			case XVAR_ARGS:
				{
					XURL&url=GetDom()->GetDomURL();
					int id=url.m_strURL.Find('?');
					if(id>=0)
						url.m_strURL.Mid(strValue,id+1);
				}break;
			case XVAR_COOKIE:
				if(!strArg.IsEmpty())//list.GetSize()>=2||args.GetSize()>=1)
				{
					//if(list.GetSize()>=2)
						//this->GetCookieOf(list[1],strValue);
					//else
						GetCookieOf(strArg,strValue);//args[0],strValue);
				}break;
			case XVAR_COOKIES:
				{
					this->GetCookies(strValue);
				}break;
			case XVAR_DOMAIN:
			case XVAR_HOST:
				{
					XURL&url=GetDom()->GetDomURL();
					strValue=url.m_strHost;
				}break;
			case XVAR_PASSWORD:
				{
					XURL&url=GetDom()->GetDomURL();
					strValue=url.m_strPasswd;
				}break;
			case XVAR_BASE:
				{
					XURL&url=GetDom()->GetDomURL();
					strValue=url.m_strProtocol;
					strValue+=url.m_strHost;
					if(url.m_uPort!=80)
					{
						XString8 s;
						s.FromInt(url.m_uPort);
						strValue+=":";
						strValue+=s;
					}
					strValue+=url.m_strPath;
				}break;
			case XVAR_PATH:
				{
					XURL&url=GetDom()->GetDomURL();
					//strValue=url.m_strProtocol;
					//strValue+=url.m_strHost;
					strValue+=url.m_strPath;
				}break;
			case XVAR_PORT:
				{
					XURL&url=GetDom()->GetDomURL();
					strValue.FromInt(url.m_uPort);//=url.m_strProtocol;
					//strValue+=url.m_strHost;
				}break;
			case XVAR_PROTO:
				{
					XURL&url=GetDom()->GetDomURL();
					strValue=url.m_strProtocol;
					//strValue+=url.m_strHost;
				}break;
			case XVAR_ROOT:
				{
					XURL&url=GetDom()->GetDomURL();
					strValue=url.m_strProtocol;
					strValue+=url.m_strHost;
					if(url.m_uPort!=80)
					{
						XString8 s;
						s.FromInt(url.m_uPort);
						strValue+=":";
						strValue+=s;
					}
				}
				break;
			case XVAR_DATE:
			case XVAR_TIME:
				{
					XTime t;
					t.SetCurrentTime();
					t.GetTime(strValue);
				}break;
			case XVAR_USER:
				 strValue=GetDom()->GetDomURL().m_strUser;
				 break;
			}
			return XTRUE;
		}
	}

	XDomItem*pItem=GetDom()->m_pDoc;//GetActiveCard();
	//if(pItem==NULL)
	//	pItem=GetDom()->m_pDoc;
	if(!pItem) return XFALSE;
	XDomList itemList;
	pItem->GetList(itemList,XDOM_SCRIPT);
	int i,nCount=itemList.GetSize();
	bool bQst=false;
	/*if(bFun)
	{
		if(strName[0]!='\''||strName[0]!='"')
		{
			XString8 s=strName;
			strName="\"";
			strName+=s;
			strName+="\"";
		}
	}
	else*/
	if(strName[0]=='?')
	{
		bQst=true;
		strName.Delete(0);
	}

	int nIndex=0;
	if(bIndex)
		nIndex=strName.ToInt();

	for(i=0;i<nCount;i++)
	{
		XDomItem*pItem=itemList[i];
		int nType=0;
		pItem->GetAttrib(XEAB::TYPE,nType);//) continue;
		if(nType==XEIT::TEXT_BAIWAN) continue;
		XString8&strCnt=((XHTMLScript*)pItem)->m_strTxt;
			//m_pDom->GetContent();
		int iBegin=0,iEnd=strCnt.GetLength();
		if(strDomain)
		{
			iBegin=strCnt.Find(strDomain,0);
			if(iBegin<0) continue;
//#if (__CORE_VERSION__>=0x02071000)
			XBOOL bFunDef=IsFunctionDefine(strCnt,"function",iBegin);
//#endif
			iBegin+=strlen(strDomain);
			int iSet=iBegin,iPush=0;
			if(bIndex)
			{
//#if (__CORE_VERSION__>=0x02071000)
				if(bFunDef)
					continue;
//#endif
				XString8 strMark;
				if(GetContextOf(strCnt,strMark,iSet,false,false))
				{
					char cMark=0,cMarkEnd=0;
					XString8Array list;
					list.SplitString(strMark,',');
					if(list.GetSize())
					{
						if(nIndex<0)
						{
							int id=XGlobal::RandInt()%list.GetSize();
							strValue=list[id];
							strValue.TrimLeft('\'');
							strValue.TrimRight('\'');
							strValue.TrimLeft('"');
							strValue.TrimRight('"');
						}
						else if(nIndex<list.GetSize())
						{
							strValue=list[nIndex];
							strValue.TrimLeft('\'');
							strValue.TrimRight('\'');
							strValue.TrimLeft('"');
							strValue.TrimRight('"');
							return XTRUE;
						}
					}
				}
				continue;
			}

			
			while(iSet<iEnd)
			{
				char c=strCnt[iSet];
				switch(c)
				{
				case 0:break;
				case '{':
					iPush++;
					break;

				case '}':
					iPush--;
					if(iPush<=0)
					{
						iEnd=iSet;
					}break;
				}
				iSet++;
			}
		}
		while(iBegin<iEnd)
		{
			iBegin=strCnt.Find(strName,iBegin);
			if(iBegin<0||iBegin>=iEnd) break;
			if(iBegin)
			{
				char ch=strCnt[iBegin-1];
				bool bSet=true;
				if(ch=='?'||ch=='_'||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9'))
					bSet=false;
				if(bSet)
				{
					int i=iBegin-1;
					int iCount0=0,iCount1=0;
					bool bEcho=false;
					while(i)
					{
						char c=strCnt[i];
						switch(c)
						{
						case '\r':
						case '\n':i=0;break;
						case '\'':if(bEcho) iCount1++;break;
						case '\"':if(bEcho) iCount0++;break;
						case '/':
							if(strCnt[i-1]=='/')
							{
								bEcho=true;
								i--;
							}
							break;
						}
						if(i)
							i--;
					}
					if(bEcho&&!(iCount0&1)&&!(iCount1&1))
						bSet=false;
				}
				if(!bSet){iBegin+=strName.GetLength(); continue;}
			}

			int is=iBegin;
			iBegin+=strName.GetLength();
			bool bSet=false;
			if(bQst&&strCnt[iBegin]!='(')
				continue;
			if(bFun)
			{
					if(GetContextOf(strCnt,strValue,iBegin))
						return XTRUE;
					continue;
					/*{
						//iBegin++;
						while(iBegin<iEnd)
						{
								char c1=strCnt[iBegin];
								if(c1==' ') {iBegin++;continue;}
								else if(c1=='(') {iBegin++;bSet=true;break;}
								else if(c1==','||c1==':') {iBegin++;bSet=true;break;}
								else break;
								iBegin++;
						}
					}*/
			}
			else
			{
				while(iBegin<iEnd)
				{
					char c=strCnt[iBegin];
					if(bMatch)
					{
						if(c<=' ') {iBegin++;continue;}
						else if(c=='='){iBegin++;bSet=true;break;}
						else break;
					}
					else
					{
						if(strDomain&&c==' ')
						{
							if(strName=="return")
							{
								bSet=true;
								break;
							}
						}
						if(c<=' ') {iBegin++;continue;}
						else if(c=='"'||c=='\'')
							{
								if(strDomain||strName.Find('(')>0)
								{
									bSet=true;
									break;
								}
							}
						else if(c=='='||c==':'||c=='+'||c==',') {bSet=true;iBegin++;break;}
						else if(c=='('&&bQst) {bSet=true;iBegin++;break;}
						else break;
					}
					iBegin++;
				}
			}
			if(!bSet) continue;
			char sp=0;
			bSet=true;
			while(bSet&&iBegin<iEnd)
			{
				char c=strCnt[iBegin];
				switch(c)
				{
					case ' ':
						if(sp==1)
							bSet=false;
						else if(sp)
							strValue+=c;
							//strValue+=c;
						break;
					case ')':
						if(sp==1)
							bSet=false;
						else
							strValue+=c;
						break;
					case ';':
					case ',':
					case '+':
					case '&':
					case '\r':
					case '\n':
					case '=':
						if(sp==1)
							bSet=false;
						else if(sp)
							strValue+=c;
						else
						{
							bSet=false;
						}
						break;
					case '\'':
						if(!sp)
							sp=c;
						else if(c==sp||sp==1)
							bSet=false;
						else
							strValue+=c;
						break;
					case '"':
						if(!sp)
							sp=c;
						else if(c==sp||sp==1)
							bSet=false;
						else
							strValue+=c;
						break;
					default:
						if(c<=' '&&c>=0)
						{
							if(!sp) break;
							else
								strValue+=c;
						}
						else if(sp)
							strValue+=c;
						else
						{
							sp=1;
							strValue+=c;
						}break;
							//bSet=false;
				}
				iBegin++;
			}
			if(!strValue.IsEmpty()||!bMatch)
				return XTRUE;
		}

	}
	return XFALSE;
}

void XDomView::SetNewFilter(XURL&url)
{
	XDOMFILTER*pFilter=m_pClientApply->GetFilter(url.m_strURL);
	if(pFilter)
	{
		m_pDom->SetFilter(pFilter);
		//LoadingStart();
	}
}

XClientApply* XDomView::GetClientApply()
{
	return this->m_pClientApply;
}

XBOOL XDomView::GetMD5Of(XU8Array&data,XString8&strOut)
{
	XU8 md5Buf[16];
	MD5 md5;
	md5.MakeMD5((char*)data.GetData(),data.GetSize(),(char*)md5Buf);
	int i;
	for(i=0;i<16;i++)
	{
		XU8 c=md5Buf[i];
		XU8 hi=c>>4;
		XU8 lo=c&0xf;
		if(hi>=10)
			strOut+=(char)('a'+hi-10);
		else
			strOut+=(char)('0'+hi);
		if(lo>=10)
			strOut+=(char)('a'+lo-10);
		else
			strOut+=(char)('0'+lo);
	}
	return XTRUE;
}

void	XDomView::LoadingStart(XDOMFILTER*pFilter)
{
	if(m_bShowLoading) 
		return;
	if(!pFilter) 
		return;
	if(!pFilter->bBegin)
		return;
	if(!m_loadingLock.Lock(XSHORTTIME)) 
		return;
	if(m_pFlashView)
		m_pFlashView->ShowWindow(XFALSE);
	m_nLoadingEnd=pFilter->nEnd;

	if(!pFilter->strLoadingBG.IsEmpty())
	{//Loading backup image
		CONNECTREQ req;
		req.bForward=m_req.bForward;		
		req.nMethod=XEM::GET;
		req.URL=m_pDom->GetURL();
		req.URL.SetURL(pFilter->strLoadingBG);
		this->SetCookies(req);
		XClient*pClient=m_pClientApply->CreateClient(req,_XBACKIMAGEID_,_XBACKIMAGEID_,this);//new CClient(XFALSE);
		if(pClient!=XNULL)
		{
			if(pClient->Connect(req,XTRUE,XTRUE)==2)
			{
				OnStarted(pClient);
				OnRecive(pClient);
				OnFinished(pClient);
				//m_pClientApply->Catch(pClient);
			}			
		}
	}
	m_nLoadingEnd*=20;
	m_strFile0.Empty();
	m_strFile1.Empty();
	m_nFileSize0=0;
	m_nFileSize1=0;
	m_nFileTotal0=0;
	m_nFileTotal1=0;
	//m_nLoadingEnd=0;
	m_bShowLoading=XTRUE;
	m_bShowEnd=XFALSE;
	Invalidate();
#ifdef __APPLE__
    PostMsg(XWM_APPLE_MESSAGE, XCMD_LOADINDPAINT_START, (XU32)this, XMS_TOPMOST);
#endif
	//m_pImgGameHelp = GetIcon( XICON_GAMEHELP );
	m_loadingLock.UnLock();
}

void XDomView::LoadingEnd()
{
//#if (__CORE_VERSION__>=0x02076000)
	if(m_pLoadingBG)
	{
		delete m_pLoadingBG;
		m_pLoadingBG=NULL;
	}
	m_nLoadingBGIndex=0;
//#endif
	m_bShowLoading=XFALSE;
	m_bShowPageLoading = false;
	Invalidate();
	m_strFile0.Empty();
	m_strFile1.Empty();
	m_nFileSize0=0;
	m_nFileSize1=0;
	m_nFileTotal0=0;
	m_nFileTotal1=0;
	Invalidate();
	if(m_pFlashView)
	{
		//WindowView::sharedWindowView()->ShowView(true);
		m_pFlashView->ShowWindow(XTRUE);
	}
}

void  XDomView::PaintLoading(XGraphics&g)
{
	if(!m_bShowLoading) 
		return;
	if(!m_loadingLock.Lock(XSHORTTIME)) return;
	int ih=m_pLoadingBG?m_pLoadingBG->Height():30;
	XRect client;
	GetClientRect(client);

	int y=client.Height()/2-ih-18;
	g.SetColor(0xff000000);
	g.SetBackColor(0xff000000);
	g.FillRectangle(client);

	if(m_pLoadingBG&&!m_pLoadingBG->IsEmpty())
	{
		ih=m_pLoadingBG->Height();
		y=(client.Height()-ih)/4;
		if(y+ih<client.Height()/2)
			y=client.Height()/2-ih;
		if(m_pLoadingBG->FrameCount()>1)
		{
			int nFrame=(m_nLoadingBGIndex/64)%m_pLoadingBG->FrameCount();
			m_pLoadingBG->SetActive(nFrame);
		}
		XPoint pt(client.Width()/2-m_pLoadingBG->Width()/2,y);
		g.DrawImage(*m_pLoadingBG,pt,0);
	}
	else if(m_pImgWaitting)
	{
		//加载圆形进度
		m_pImgWaitting->SetActive(m_WaitingIndex);
		XPoint ptw( client.Width() - m_pImgWaitting->Width() - 40, 40 );
		g.DrawImage(*m_pImgWaitting,ptw,0);

		////加载gif
		//m_pImgGameHelp->SetActive(m_iGamehelpIndex);
		//XPoint pt(client.Width()/2-m_pImgGameHelp->Width()/2, client.Height() / 2 - m_pImgGameHelp->Height() / 2);
		//g.DrawImage(*m_pImgGameHelp,pt,0);
	}
	y = client.Height() / 2; // + m_pImgWaitting->Height() / 2;
	XPCWSTR strTmp = XResource::LoadWString( XIDS_WAITTIME );
	XString8 strTime( strTmp );
	y += DrawFile( g, strTime, 0, -1, y );
	//if ( strTmp )
	//{
	//	g.SetColor( 0xff0f0f0f );
	//	XString8 strTime( strTmp );
	//	XString8Array list;
	//	list.SplitString( strTime, '|' );
	//	int i = 0;
	//	int ic = list.GetSize();
	//	for ( i = 0; i < ic; i++ )
	//	{
	//		y += DrawFile( g, list[i], 0, -1, y );
	//	}
	//	
	//}
	
	g.SetColor(0xff7f7f3f);
	
	y+=DrawFile(g,m_strFile0,m_nFileSize0,m_nFileTotal0,y);
	y+=DrawFile(g,m_strFile1,m_nFileSize1,m_nFileTotal1,y);
	m_loadingLock.UnLock();

}
void  XDomView::StartFile(XString8&strFile)
{
	if(!m_bShowLoading)
		return;
	if(!m_loadingLock.Lock(XSHORTTIME)) return;
	m_strFile0=m_strFile1;
	m_nFileSize0=m_nFileSize1;
	m_nFileTotal0=m_nFileTotal1;
	m_nFileTotal1=0;
	
	m_nFileSize1=0;
	//int id=strFile.Find('/',7);
	int id=strFile.ReverseFind('/');
	//int ie=strFile.ReverseFind('?');
	if(id>0)
		m_strFile1.ConvertFrom(strFile.GetData()+id+1,XCHARSET_GB2312);
	else
		m_strFile1.ConvertFrom(strFile.GetData(),XCHARSET_GB2312);

	//id=m_strFile1.Find('.');
	//if(id<0)
	id=m_strFile1.Find('?');
	if(id>0)
		m_strFile1[id]=0;

	id=m_strFile1.ReverseFind('?');
	if(id>0)
	{
		for(int i=1;i<=4;i++)
		{
			if(i+id>=m_strFile1.GetLength()) break;
			if(i==4)
				m_strFile1[i+id]=0;
			else
				m_strFile1[i+id]='.';
		}
	}

	if(m_strFile1.GetLength()>20)
		m_strFile1[20]=0;

	XString8 strTmp=m_strFile1;
	strTmp.MakeLower();
	id=strTmp.Find(".swf",0);
	if(id>0)
	{
		char*pData=m_strFile1.GetData();
		pData[id+1]='b';
		pData[id+2]='a';
		pData[id+3]='i';		
	}
	

	Invalidate();
	m_loadingLock.UnLock();
}
void  XDomView::LoadingData(XU32 nSize,XU32 nTotal)
{
	if(!m_bShowLoading)
		return;
	if(!m_loadingLock.Lock(XSHORTTIME))
		return;
	m_nFileSize1+=nSize;
	m_nFileTotal1=nTotal;
	Invalidate();
	m_loadingLock.UnLock();
}

void  XDomView::UpdateLoading()
{
	if(!m_bShowLoading)
		return;
	if(!m_loadingLock.Lock(XSHORTTIME))
		return;
	if(m_bShowEnd)
	{
		if(m_nLoadingEnd) 
			m_nLoadingEnd--;
		if(!m_nLoadingEnd)
			LoadingEnd();
	}

	if(m_pImgWaitting)
	{
		m_WaitingIndex++;
		if ( m_WaitingIndex >= m_pImgWaitting->FrameCount() )
		{
			m_WaitingIndex = 0;
		}
		
		//static int i = 0;
		//i++;
		//if ( i > 30 )
		//{
		//	m_iGamehelpIndex++;
		//	i= 0;
		//}
		//
		//if(m_iGamehelpIndex>=m_pImgGameHelp->FrameCount())
		//	m_iGamehelpIndex=0;
	}
//#if (__CORE_VERSION__>=0x02076000)
	if(m_pLoadingBG&&m_pLoadingBG->FrameCount()>0)
	{
		m_nLoadingBGIndex++;
	}
//#endif
	m_loadingLock.UnLock();
	Invalidate();
}


int XDomView::DrawFile(XGraphics&g,XString8&strFile,XU32 nSize,int nTotal,int y)
{
	if(strFile.IsEmpty())
		return 0;
	XString16 strTxt=strFile.GetData();	
	XSIZE sz={0,0};
	g.MeasureString(strTxt,sz,strTxt.GetLength());
	XRect client;
	XWindow::GetClientRect(client);
	//if(nTotal<0)
	int nMaxW=client.Width()-20;
	if(nTotal>=0)
		nMaxW=nMaxW/6;
	for(;;)
	{
		if(sz.cx<=nMaxW) break;
		int iLen=strTxt.GetLength();
		if(iLen<=16) break;
		iLen=6;
		int iEnd=strTxt.Find('/',iLen+4);
		if(iEnd<=0) break;
		XString16 strTmp,str;
		strTxt.Left(str,iLen);
		str+=XString16("...");
		strTxt.Mid(strTmp,iEnd);
		str+=strTmp;
		strTxt=str;
		g.MeasureString(strTxt,sz,strTxt.GetLength());
	}
	if(nTotal<0)
	{
		XPoint pt((nMaxW-sz.cx)/2,y);
		g.SetColor(0xffafafaf);
		g.DrawString(strTxt,pt,strTxt.GetLength());
	}
	else
	{
		int ww=client.Width()/3;
		if(ww>200) ww=200;

		int x=client.Width()/2-sz.cx-ww/2-4;
		XPoint pt(x,y+2);
		g.SetColor(0xffafafaf);
		g.DrawString(strTxt,pt,strTxt.GetLength());
		if(nSize)
		{
			strTxt=XString16("(");
			if(nSize>1024000)
			{
				XString16 strInt;
				strInt.FromInt(nSize/1024);
				strTxt+=strInt;
				strTxt+=XString16("K 字节)");
			}
			else
			{
				XString16 strInt;
				strInt.FromInt(nSize);
				strTxt+=strInt;
				strTxt+=XString16(" 字节)");
			}
			
			int ddd=0;
			if(sz.cy>16)
				ddd=(sz.cy-10)/2;

			x=client.Width()/2-ww/2;
			pt.x=x+ww+2;
			g.DrawString(strTxt,pt);

			XRect rect(x,y+4+ddd,x+ww+1,y+sz.cy-3-ddd);
			g.DrawBar(rect, 0xFFFFFFFF, 2);
			int len=0;
			if(nTotal)
				len=nSize*ww/nTotal;
			else
			{
				len=nSize*ww/128000;
				
			}
			if(len>ww) len=ww;
			XRect rect1(x,y+4+ddd,x+len,y+sz.cy-3-ddd);
			g.DrawBar(rect1,0xFF0289FF,2);
		}
	}

	return sz.cy+2;
}


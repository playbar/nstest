// XMainWnd.cpp: implementation of the XMainWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XMainWnd.h"
#include "XHomeView.h"
#include "XDialogClear.h"
#ifdef _WIN32
#include "WindowView.h"
#else

#endif
#ifdef _SYMBIAN
#include "XThreadPool.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _strHomePage XResource::LoadWString(XIDS_HOMEPAGE)
#define _strCloseAll XResource::LoadWString(XIDS_CLOSEALLMSG)

#ifdef __APPLE__
static XMENUDATA _systemMenus[]=
{
	{XCMD_CLOSE_ALL,0,XResource::LoadWString(XIDS_CLOSEALL),XNULL,0,0},
	//{XCMD_HELP,0,XResource::LoadWString(XIDS_HELP),XMainWnd::GetIcon(XICON_HOME),5,0},
	{XCMD_CLEAR_CATCH,0,XResource::LoadWString(XIDS_CLEARCMD),XNULL,0,0},
	{0,0,XNULL,XNULL,0,XMS_SEPARATOR},
//	{XCMD_QUIT,0,XResource::LoadWString(XIDS_QUIT),XNULL,0,0},
};

#elif defined( __ANDROID__ )
static XMENUDATA _systemMenus[]=
{
	{XCMD_CLOSE_ALL,0,XResource::LoadWString(XIDS_CLOSEALL),XNULL,0,0},
	//{XCMD_HELP,0,XResource::LoadWString(XIDS_HELP),XMainWnd::GetIcon(XICON_HOME),5,0},
	{XCMD_CLEAR_CATCH,0,XResource::LoadWString(XIDS_CLEARCMD),XNULL,0,0},
	{0,0,XNULL,XNULL,0,XMS_SEPARATOR},
	//{XCMD_QUIT,0,XResource::LoadWString(XIDS_QUIT),XNULL,0,0},
};
#else
static XMENUDATA _systemMenus[]=
{
	{XCMD_CLOSE_ALL,0,XResource::LoadWString(XIDS_CLOSEALL),XNULL,0,0},
	{XCMD_HELP,0,XResource::LoadWString(XIDS_HELP),XMainWnd::GetIcon(XICON_HOME),5,0},
	{XCMD_CLEAR_CATCH,0,XResource::LoadWString(XIDS_CLEARCMD),XNULL,0,0},
	{0,0,XNULL,XNULL,0,XMS_SEPARATOR},
	{XCMD_QUIT,0,XResource::LoadWString(XIDS_QUIT),XNULL,0,0},
};

#endif

XMainWnd::XMainWnd()
{
    m_pDraw=XNULL;
	m_bFull=XFALSE;
#ifdef __ANDROID__
#else
	m_pBox=XNULL;
	m_pClear=XNULL;
#endif
	m_nTitleHeight=0;
#ifdef _SYMBIAN
	m_pThread = XNULL;
#endif
#ifdef __XTAB
#else
	m_pBlock=XNULL;
	//m_strTitle=XResource::LoadWString(XIDS_HOMEPAGE);
#endif
	XWindow::SetMainWnd(this);
}

XMainWnd::~XMainWnd()
{
#ifdef _SYMBIAN
	XThreadPool *pool = XThreadPool::GetInstance();
	pool->FreeThread(m_pThread);
#else
  m_thread.Stop();
#endif

#ifdef __ANDROID__
#else
  if(m_pBox)
	  delete m_pBox;
  if(m_pClear)
	  delete m_pClear;
#endif
//  m_pBox=XNULL;
  for(XU32 i=0;i<m_views.GetSize();i++)
	  delete m_views[i];
  //for(i=0;i<m_sysMenus.GetSize();i++)
  //	  delete m_sysMenus[i];
  //m_sysMenus.RemoveAll();
  m_views.RemoveAll();
#ifdef _SYMBIAN

#else
  m_thread.Release();
#endif
  m_clientApply.Release();
}

XBOOL XMainWnd::Create(const XRect &rect,XPCWSTR strTitle,XGraphics*pDraw,XPCTSTR strWorkPath)
{
	if(m_pTopmost!=XNULL)
	{
		return XFALSE;
	}
	m_clientApply.m_strWorkPath=strWorkPath;

	m_clientApply.Create();
	//m_clientApply.ResetConfig(XFALSE);

	m_strAppTitle=strTitle;
	m_pDraw=pDraw;

	XBOOL bOK=XWindow::Create(rect,XES_VISIABLE|XES_TOPMOST,XNULL);
#ifdef __XTAB
	m_nTitleHeight=XWindow::m_nTextHeight+5;
#else
	m_pBlock=GetIcon(XICON_BLUEBLOCK);
	//m_block.CreateFromResource("BLUEBLOCK");
	if(m_pBlock&&!m_pBlock->IsEmpty())
	{
		m_nTitleHeight=m_pBlock->Height();
		m_nTitleHeight = XMAX(m_nTitleHeight, m_nTextHeight + 4);
	}
	else
		m_nTitleHeight=m_nTextHeight+4;
#endif

	XRect r(0,0,rect.Width(),m_nTitleHeight);
	m_viewRect=rect;
	m_viewRect-=rect.TopLeft();

#ifdef __APPLE__
#elif __ANDROID__
#elif __NOTITLE__
#elif defined(_SYMBIAN)
//	m_viewRect.top+=r.Height();
#else
	m_viewRect.top+=r.Height();
#endif
	XDomView*pView=new XHomeView(m_pDraw,&m_clientApply);
	pView->Create(m_viewRect,this);
	m_views.Add(pView);
#ifdef __XTAB
	m_tab.Create(r,this);
	m_tab.Insert(_strHomePage,GetIcon(XICON_HOME),(XU32)pView,-1,XTRUE);
	ChangeActiveView(m_tab.GetActiveID());
#else
	ChangeActiveView(0);
#endif
	Invalidate();
#ifdef _SYMBIAN
	XThreadPool *pool = XThreadPool::GetInstance();
	m_pThread = pool->GetThread(this);
	m_pThread->SetName(_L("MainWndThread"));
#else
	m_thread.Create(this,100);
#endif

	return bOK;
}


XBOOL XMainWnd::CreateView(CONNECTREQ &req, XPCWSTR strTitle,XImage*pIcon)
{
#ifdef __XTAB
	for(XU32 i=1;i<m_views.GetSize();i++)
	{
		XDomView*pView=(XDomView*)m_views[i];
		if(pView->m_req.URL.m_strHost==req.URL.m_strHost)
		{
			SetActiveView(pView);
			//SetActive(pView);
			pView->Connect(req,XTRUE);
			return XTRUE;
		}
	}
	if(m_views.GetSize()>=XMAXTABS)
	{
		XDomView*pView=(XDomView*)m_views[1];
		SetActiveView(pView);
		//SetActive(pView);
		pView->Connect(req,XTRUE);
		return XTRUE;
		//return XFALSE;
	}
	XDomView*pView=new XDomView(m_pDraw,&m_clientApply);
	pView->Create(m_viewRect,this);
	XString16 str;
	if(strTitle==XNULL)
	{
		XString8Array list;
		list.SplitString(req.URL.m_strHost,XPT);
		if(list.GetSize()>1)
		{
			int iMax=0,iLen=list[0].GetLength();
			for(XU32 i=1;i<list.GetSize()-1;i++)
			{
				int l=list[i].GetLength();
				if(iLen<l) {iLen=l;iMax=i;}
			}
			str=list[iMax];
			//str=list[list.GetSize()-2];
		}
		else
			str=req.URL.m_strHost;
		strTitle=str;
	}
	m_tab.Insert(strTitle,
			     pIcon==XNULL?GetIcon(XICON_CONNECT):pIcon,(XU32)pView,-1,XTRUE);
	m_views.Add(pView);
	ChangeActiveView(m_tab.GetActiveID());
	//SetActive(pView);
	pView->Connect(req,XTRUE);
#endif
	return XTRUE;
}


XBOOL XMainWnd::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
   switch(nOpera)
   {
   case XWM_ENDDIALOG:
	    switch(pData1)
		{
		case XIDYES:
		case XIDOK:
			 switch(pData2)
			 {
			 case XCMD_CLOSE_ALL:
				 {
				  int nCount=m_views.GetSize();
				   for(XU32 i=nCount-1;i>0;i--)
					   DestoryChild(m_views[i]);
				   FullScreen(XFALSE);
				 }
				  break;
			 case XCMD_QUIT:
				  PostQuitMessage();
				  break;
			 }break;
		}
	    return XTRUE;
   case XWM_SETSTATUS:
	   {
#ifdef __XTAB
		   int id=m_tab.FindData(pData2);
		   if(id>=0)
		     m_tab.ActiveStatus(id);
#endif
	   }return XTRUE;
   case XWM_SETTITLE:
	    SetTitle((XPCWSTR)pData1,(XWindow*)pData2);
	    return XTRUE;
   }
   return XWindow::Handle(nOpera,pData1,pData2);
}

void XMainWnd::ChangeActiveView(XU8 nID)
{
#ifdef __XTAB
   if(nID>=m_views.GetSize()) return;
   XWindow*p=(XWindow*)m_tab.GetTabData(nID);
#else
   XWindow*p=m_views[0];
#endif
   if(p==XNULL) return;
   for(XU32 i=0;i<m_views.GetSize();i++)
	   if(m_views[i]!=p)
	   {
		   p->SetFocus(XFALSE);
		   m_views[i]->ShowWindow(XFALSE);
	   }
   p->ShowWindow(XTRUE);

   p->SetFocus(XTRUE);

	//m_views[nID]->ShowWindow(XTRUE);
#ifdef __XTAB
   if(m_pActive!=&m_tab)
   {
		SetActive(p);//m_views[nID]);
   }
   else p->SetFocus(XFALSE);
#else
   SetActive(p);
#endif
   p->Invalidate();

}



XBOOL XMainWnd::DestoryChild(XWindow *pWnd)
{

	XBOOL bOK=XWindow::DestoryChild(pWnd);
#ifdef __XTAB
	int id=m_tab.FindData((XU32)pWnd);
	if(id>=0)
		m_tab.Remove(id);
	else
	{
		SetActive(GetActiveView());
		return bOK;
	}

	id=FindView(pWnd);
	if(id>=0)
	{
		delete m_views[id];
		m_views.RemoveAt(id);
	}

	ChangeActiveView(m_tab.GetActiveID());
	//SetActive(GetActiveView());
#endif
	return bOK;
}

int XMainWnd::FindView(XWindow *pWnd)
{
#ifdef __XTAB
	for(XU32 i=0;i<m_views.GetSize();i++)
		if(m_views[i]==pWnd) return i;
	return -1;
#else
	return 0;
#endif
}

void XMainWnd::SetTitle(XPCWSTR strTitle, XWindow *pWnd)
{
#ifdef __XTAB
	int id=m_tab.FindData((XU32)pWnd);
	if(id<0) return;
	m_tab.SetTabTitle(id,strTitle);
#else
	m_strTitle=strTitle;
	XRect rect(0,0,m_rect.Width(),m_nTitleHeight);
	InvalidateRect(rect);
#endif
}

XBOOL XMainWnd::OnKeyDown(XKEYMSG&m)
{
#if defined(__TOUCHPAD__)||defined(__POINT__)
	return XFALSE;
#else
	switch(m.nKeyCode)
	{
	case XK_LEFTSOFT:
#ifdef __ANDROID__
#else
		TrackMenu();
#endif
		return XTRUE;
	}
#endif
	return XFALSE;
}

#ifdef __ANDROID__
#else
void XMainWnd::TrackMenu()
{
#if defined(__TOUCHPAD__)||defined(__POINT__)
	return;
#else
	if(m_menu.GetCount()>0) return;
	XPoint pt(0,m_rect.Height());
	if(!m_bFull)
		pt.y-=m_nTitleHeight;

	//MENUDATAS data;
	//data.Append(m_sysMenus);
	XDomView*p=GetActiveView();
	//if(p!=XNULL)
	//	p->AppendMenu(m_menu);
	if(m_menu.GetCount()>0)
		m_menu.AddSeparator();
	int id=1;
	if(m_views.GetSize()>1) id--;
	for(;id<sizeof(_systemMenus)/sizeof(XMENUDATA);id++)
	{
		m_menu.AddMenu(&_systemMenus[id]);
	}
	m_menu.Create(pt,XMPS_LEFT|XMPS_BOTTOM,this);
	Invalidate();
#endif
}
#endif

XDomView* XMainWnd::GetActiveView()
{
#ifdef __XTAB
   XU8 nID=m_tab.GetActiveID();
   XDomView*p=(XDomView*)m_tab.GetTabData(nID);
   return p;
#else
   return m_views[0];
#endif
}


XBOOL XMainWnd::OnCommand(XU32 nCmd, XWindow *pWnd)
{
	    switch(nCmd)
		{
		case XCMD_CLOSE_ALL:
#ifdef __ANDROID__
#else
			 if(m_pBox)
				 delete m_pBox;
			 m_pBox=XMessageBox::Message(_strCloseAll,this,XCMD_CLOSE_ALL,XMB_YESNO|XMB_ICONQUESTION);
#endif
			return XTRUE;
		case XCMD_CLEAR_CATCH:
			{
#ifdef __ANDROID__
#else
				if(m_pClear)
					delete m_pClear;
				m_pClear=XDialogClear::Clear(m_clientApply.m_strWorkPath,XTRUE);
#endif
				//dlg.DoModal();
			}
			 return XTRUE;
		case XCMD_TABLE:
#ifdef __XTAB
			 ChangeActiveView(m_tab.GetActiveID());
#endif
			 break;
		case XCMD_CLOSE:
			{
				XDomView*p=GetActiveView();
				if(p!=XNULL)
					p->PostMsg(XWM_COMMAND,XCMD_VIEW_CLOSE,(XU32)this,XMS_THIS);
			}
			 break;
		case XCMD_QUIT:
			QuitView();
			break;
		//case XCMD_QUIT:
		//	 if(m_pBox)
		//		 delete m_pBox;
		//	 m_pBox=XMessageBox::Message(_strQuitSystem,this,XCMD_QUIT,XMB_YESNO|XMB_ICONQUESTION);
			 //if(XMessageBox::Message(_strQuitSystem,this,XMB_YESNO|XMB_ICONQUESTION)==XIDYES)
			 //{
				//XDialogClear dlg(m_clientApply.m_strWorkPath,XFALSE);
				//dlg.DoModal();
				//PostQuitMessage();
			 //}
			 break;
		default:
			return XFALSE;
		}
	    return XTRUE;

}



void XMainWnd::OnClose()
{
#ifdef _SYMBIAN
	XThreadPool *pool = XThreadPool::GetInstance();
	pool->FreeThread(m_pThread);
#else
   m_thread.Stop();
#endif
   m_clientApply.Stop();
   //m_thread.Release();
   //m_clientApply.Release();
}

void XMainWnd::SetActiveView(XDomView *pView)
{
#ifdef __XTAB
	int id=m_tab.FindData((XU32)pView);
	m_tab.SetActive(id);
	ChangeActiveView(id);
#else
	ChangeActiveView(0);
#endif
}

XPCWSTR XMainWnd::GetTitle(XWindow *pWnd)
{
#ifdef __XTAB
    int id=m_tab.FindData((XU32)pWnd);
	if(id<0) return XNULL;
	return m_tab.GetTabTitle(id);
#else
	return m_strTitle;
#endif
}

void XMainWnd::FullScreen(XBOOL bFull)
{
#ifdef __APPLE__
	return;
#endif
#ifdef __ANDROID__
	return;
#endif
	//if(m_tab.IsVisiable()==!bFull) return;
	if(m_bFull==bFull) return;
	m_bFull=bFull;
#ifdef __XTAB
	m_tab.ShowWindow(!bFull);
#endif
	XRect rect(m_viewRect);
	if(bFull) rect.top=0;
	for(XU32 i=0;i<m_views.GetSize();i++)
	{
		if(m_views[i]!=XNULL)
			m_views[i]->MoveWindow(rect);
	}
	//	if(m_views[i]==pWnd) return i;
	//return -1;
}

void XMainWnd::OnMove(XRect &rect)
{
	m_viewRect=rect;
	m_viewRect-=rect.TopLeft();
#if defined(__APPLE__)||defined(__NOTITLE__) ||defined(__ANDROID__)
#else
	m_viewRect.top=m_nTitleHeight;
#endif

#ifdef __XTAB
	XRect r=m_tab.GetWindowRect();
	r.right=rect.Width();
	m_tab.MoveWindow(r);
#endif
	XRect vr(m_viewRect);
	if(m_bFull) vr.top=0;
	for(XU32 i=0;i<m_views.GetSize();i++)
		m_views[i]->MoveWindow(vr);
	//XRect r(0,0,rect.Width(),XWindow::m_nTextHeight+5);
}

XBOOL XMainWnd::OnPaint(XGraphics &g)
{
#ifdef __APPLE__
	return XTRUE;
#elif defined(_SYMBIAN)
	return true;
#elif defined( __ANDROID__ )
	return true;
#elif defined(__NOTITLE__)
	return XTRUE;
#endif
	if(m_bFull) return XTRUE;
	XRect clip;
	g.GetClip(clip);
	clip&=XRect(0,0,m_rect.Width(),m_rect.Height());
	if(clip.IsRectEmpty()) return XTRUE;

	if(m_pBlock==XNULL||m_pBlock->IsEmpty())
	{
		//XRect rt=GetClipRect();
		//int dh=m_rect.Height();
		XRect rt(clip.left,0,clip.right,m_nTitleHeight);
		//g.DrawBar(rt,0xff6f6f8f,12);
		//rt=XRect(clip.left,0,clip.right,dh);
		g.DrawBar(rt,0xff8f8fbf,8);
	}
	else
	{
		int dw=m_pBlock->Width();//,dh=m_pBlock->Height;
		int x;
		XRect rect(0,0,dw,m_nTitleHeight);
		for(x=0;x<clip.right;x++)
		{
			XPoint pt(x,0);
			g.DrawImage(*m_pBlock,pt,rect);
			x+=dw-1;
		}
	}
	XSize sz;
	g.MeasureString(m_strTitle,sz);

	XString16 str=m_strAppTitle;
	if(!m_strTitle.IsEmpty())
	{
		str+=XString16("-");
		str+=m_strTitle;
		int l=str.GetLength();
		XWCHAR* strBuf=str;
		if(l>16)
		{
			strBuf[14]='.';
			strBuf[15]='.';
			strBuf[16]=0;
		}
	}

	int by=((m_nTitleHeight-sz.cy)>>1)+2;
	g.SetColor(0xff3463b9);
	g.DrawString(str,3,by+1);
	g.SetColor(0xffe5eff3);
	g.DrawString(str,2,by);
	return XTRUE;
}

XSWFPlayer* XMainWnd::GetPlayer()
{
	return GetActiveView()->GetPlayer();
}

XUINT XMainWnd::CallBack(XU32 nID)
{

#ifdef _SYMBIAN
	m_views[0]->CallBack(*m_pThread);
#else
	//LOGWHERE();
	m_views[0]->CallBack(m_thread);
#endif
	return nID;
}

void XMainWnd::StartCall()
{
	//if ( ! WindowView::sharedWindowView()->mbInit )
	//{
	//	WindowView::sharedWindowView()->initGL();
	//	WindowView::sharedWindowView()->mbInit = true;
	//	//WindowView::sharedWindowView()->ShowView( true );
	//	//WindowView::sharedWindowView()->MakeCurrent( true );
	//	RasterGL::sharedRasterGL()->Init();
	//	ccV2F_C4F_T2F p1 = { {100, 100},{ 0.8, 0, 0, 1 }, {0, 0 } };
	//	ccV2F_C4F_T2F p2 = { {200, 200}, {0, 0.5, 0, 1}, {0, 0}};
	//	ccV2F_C4F_T2F p3 = { {300, 100}, {0, 0, 1, 1}, {0, 0}};
	//	RasterGL::sharedRasterGL()->drawTriangle( p1, p2, p3 );
	//	//WindowView::sharedWindowView()->MakeCurrent( false );
	//}
}

void XMainWnd::ReleaseCall()
{
	if(m_views.GetSize())
		m_views[0]->ReleaseCall();
}

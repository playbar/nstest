// MobileView.cpp: implementation of the CMobileView class.
//
//////////////////////////////////////////////////////////////////////

#include "GGBase.h"
#include "GGDom.h"

#include "MobileView.h"
#include "GlobalPortingFunction.h"
#include "KeyBoardMiddleLayer.h"
#include "XFlashView.h"
#include "XDomViewExt.h"

#include "XDialogClear.h"
#include "XDialogSave.h"

#include <assert.h>
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

const char* iosPidPostUrl = "http://t.jqbar.com/testszf/testios/testios.aspx";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static CMobileView* s_mobileView = NULL;

CMobileView::CMobileView()
:m_empty(true)
{
   m_nCurType=0;
    sendDomView = XNULL;
    s_mobileView = this;
}
CMobileView* CMobileView::s_getView()
{
    return s_mobileView;
}
CMobileView::~CMobileView()
{
    if (sendDomView)
    {
        delete sendDomView;
        sendDomView = XNULL;
    }
}

XBOOL CMobileView::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
    
	switch(nOpera)
	{
        case XWM_XSHOWHIDEMENU:
            GFunShowHideMenu(m_pWnd, (bool)pData1 );
            break;
	case XWM_STARTEDIT:
			//Create Full Edit Control Here
		{
			int nMaxLength=pData1>>16;
			XU8 nStyle=pData1&0xff;
			XPCWSTR pText=(XPCWSTR)pData2;
			KeyBoardMiddleLayer::getInstance()->startInput(pText, nMaxLength, nStyle);
			
			//Notify:Post Message(XWM_FINISHEDEDIT (XU32)WCHARS) //where edit finished
		}
			break;
	case XCMD_FLASHMODECMD:
		{
			g_setViewMode(m_pWnd, pData1);
		}
		break;
	case XWM_REPAINT:
		if(m_pWnd!=XNULL)
		{
			XRect clip = m_clip;	
			
			g_invalidateView(m_pWnd, &clip);
		}break;
	case XWM_QUITAPP:
        g_wanttoQuit(m_pWnd);
		break;
	case XWM_APPLE_MESSAGE:
        handleAppleMessage(pData1, pData2);
        break;
            
    case XWM_SHOWBARANDONSALE:
        g_noticeClientShowBarAndOnSale(m_pWnd,pData1);
        break;
            
    case XWM_APP_STATUS:
        g_noticeClientAppStatus(m_pWnd, pData1);
        break;
    case XWM_NEW_APP:
        g_noticeClientNewApp(m_pWnd, pData1);
        break;
    case XWM_OUTLOGIN:
        g_noticeClientOutLogin(m_pWnd, pData1 );
        break;
    case XWM_PAYSDK_PID:
        g_noticeClientThirdPartPid(m_pWnd, pData1);
		break;
    case XWM_WXSENDTEXT:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDTEXT);
        break;
    case XWM_WXSENDPHOTO:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDPHOTO);
        break;
    case XWM_WXSENDLINK:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDLINK);
        break;
    case XWM_WXSENDMUSIC:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDMUSIC);
        break;
    case XWM_WXSENDVEDIO:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDVEDIO);
        break;
    case XWM_WXSENDAPPMESSAGE:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDAPPMESSAGE);
        break;
    case XWM_WXSENDNOTGIFCONTENT:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDNOTGIFCONTENT);
        break;
    case XWM_WXSENDGIFCONTENT:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDGIFCONTENT);
        break;
    case XWM_WXSENDFILECONTENT:
        g_noticeWeiXinSendSession(m_pWnd, XWM_WXSENDFILECONTENT);
        break;
    case XWM_YMSHOWOFFERWALL:
        g_noticeShowOfferWall(m_pWnd, 0);
        break;
    case XWM_UPDATEMSG:
        switch (pData1){
        case XCMD_UPDATE_DATASTART:
            g_noticeClientUpdateStart( m_pWnd, (int)pData2 );
            break;
        case XCMD_UPDATE_DATASET:
            g_noticeClientUpdateSet( m_pWnd, (int)pData2 );
            break;
        case XCMD_UPDATE_DATAEND:
            g_noticeClientUpdateEnd( m_pWnd );
            break;
        }
	}
	return XMainWnd::Handle(nOpera,pData1,pData2);
}

void CMobileView::handleAppleMessage(XU32 pData1, XU32 pData2)
{
	switch (pData1)
    {
		case XCMD_START_FLASH:
			g_noticeClientFlashStart(m_pWnd);
			break;
		case XCMD_END_FLASH:
			g_noticeClientFlashFinish(m_pWnd);
			break;
		case XCMD_START_LOAD_PAGE:
			g_noticeClientPageLoadingStart(m_pWnd);
			break;
		case XCMD_END_LOAD_PAGE:
			g_noticeClientPageLoadingFinish(m_pWnd);
			break;
		case XCMD_LOAD_PROCESS:
			g_noticeClientPageLoadingPercent(m_pWnd, pData2);
			break;
		case XCMD_LOAD_PAGE_NOT_EMPTY:
			g_noticeClientPageNotEmpty(m_pWnd);
			m_empty = false;
			break;
		case XCMD_FLASH_OPENSUBVIEW:
			g_noticeClientOpenSubView(m_pWnd);
			break;
		case XCMD_FLASH_CLOSESUBVIEW:
			g_noticeClientCloseSubView(m_pWnd);
			break;
		case XCMD_FLASH_ZOOMINENABLE:
			g_noticeClientZoomInEnable(m_pWnd);
			break;
		case XCMD_FLASH_ZOOMOUTENABLE:
			g_noticeClientZoomOutEnable(m_pWnd);
			break;
		case XCMD_TOP_SCORE_AVAILABLE:
			g_noticeClientTopScoreAvailable(m_pWnd);
			break;
		case XCMD_FLASH_SAVE_PERCENT:
			g_noticeFlashSave(m_pWnd, pData2);
			break;
  
        case XCMD_IOS_PID:
            g_noticeClientIosPid(m_pWnd, pData2);
            break;
        case XCMD_STOP_IOS_PID:
            g_noticeClientStopIosPid(m_pWnd, pData2);
            break;
//        case XCMD_91_PID:
//            g_noticeClient91Pid(m_pWnd, pData2);
//            break;
            
        case XCMD_UPDATE_CLICKOPT:
            g_noticeClientUpdateClickOpt(m_pWnd);
            break;
        case XCMD_FLASH_CANSAVE:
            g_noticeClientFCanSave(m_pWnd);
            break;
        case XCMD_FLASH_CANPAY:
            g_noticeClientFCanPay(m_pWnd);
            break;
        case XCMD_UPDATE_BAR:
            xgg_mm_changeBar(m_pWnd);
            break;
        case XCMD_NOTICE_PAYURL:
            g_noticeClientPayUrl(m_pWnd, (unsigned)pData2);
            break;
        case XCMD_SHOWWEBVIEW:
            g_noticeShowWebView(m_pWnd, (unsigned)pData2);
            break;
        case XCMD_SHOWWEBBVIEW:
            g_noticeShowWebBView(m_pWnd, (unsigned)pData2);
            break;
        case XCMD_FLASHTODOMEXT:
            g_noticeFlashToDomTouch(m_pWnd);
            break;
        case XCMD_DOMEXTTOFLASH:
            g_noticeDomToFlashTouch(m_pWnd);
            break;
        case XCMD_LOADINDPAINT_START:
            g_noticeChangeToOriginalView(m_pWnd);
            break;
		default:
			break;
	}
}

XBOOL CMobileView::Create(XRect &rect,XGraphics*p,void*pWnd,
						  XPCWSTR strTitle,XPCTSTR strPath)
{
	//m_draw.FromImage(*p);
	m_pWnd=pWnd;
	XString8 str;
	XRect rt(rect.left,rect.top,rect.right,rect.bottom);

	XMainWnd::Create(rt,strTitle,p,strPath);
    m_clientApply.SetCID( BWIOSCID );
    return true;
}

void CMobileView::PeekMessage()
{
	
//	printf("want to peek message? No Implementation!\n");
}

void CMobileView::SetCursor(XU8 nType)
{

}

void CMobileView::SetConfig(BROWNSINFO &info)
{
	XU32 i;
	
	m_clientApply.ReleaseClient();
	
	
	m_clientApply.m_info.uPort=info.nPort;
	m_clientApply.m_info.nProtocol=info.nProtcol;
	m_clientApply.m_info.nRetries=info.nRetries;
	m_clientApply.m_info.nTimeout=info.nTimeout;
	m_clientApply.m_info.strAccepts.RemoveAll();
	for(i=0;i<(XU32)info.strAccepts.size();i++)
	{
		m_clientApply.m_info.strAccepts.Add(info.strAccepts[i]);
	}
	m_clientApply.m_info.strGateway=info.strGameway;
	m_clientApply.m_info.strProfile=info.strProfile;
	m_clientApply.m_info.strUserAgent=info.strUserAgent;
	m_clientApply.m_info.nLanguages.Add(XEnumLan::CHINESE);
	m_clientApply.m_info.nLanguages.Add(XEnumLan::ENGLISH);
	XU32 nColors[XCW_BUTTON+1];
	for(i=0;i<XCW_BUTTON+1;i++)
		nColors[i]=TRUECOLOR(info.nColor[i]);
	XWindow::SetColors(nColors);
}

void CMobileView::GetConfig(BROWNSINFO &info)
{
	XU32 i;
	info.nPort=m_clientApply.m_info.uPort;
	info.nProtcol=m_clientApply.m_info.nProtocol;
	info.nRetries=m_clientApply.m_info.nRetries;
	info.nTimeout=m_clientApply.m_info.nTimeout;
	info.strAccepts.clear();
	for(i=0;i<m_clientApply.m_info.strAccepts.GetSize();i++)
	{
		info.strAccepts.push_back((XPCTSTR)m_clientApply.m_info.strAccepts[i]);
	}
	info.strGameway=m_clientApply.m_info.strGateway;
	info.strProfile=m_clientApply.m_info.strProfile;
	info.strUserAgent=m_clientApply.m_info.strUserAgent;
	m_clientApply.m_info.nLanguages.Add(XEnumLan::CHINESE);
	m_clientApply.m_info.nLanguages.Add(XEnumLan::ENGLISH);
	for(i=0;i<=XCW_BUTTON;i++)
		info.nColor[i]=TRUECOLOR(XWindow::GetSysColor(i));
}


void CMobileView::ViewSource()
{
#ifdef _SOURCECODE
	XDomView*pView=GetActiveView();
	TCHAR strTemp[MAX_PATH],strFile[MAX_PATH];
	::GetTempPath(MAX_PATH,strTemp);
	::GetTempFileName(strTemp,_T("sr"),100,strFile);
	CFile file;
	if(!file.Open(strFile,CFile::modeWrite|CFile::modeCreate))
	   return;
	file.Write(pView->m_strSource,pView->m_strSource.GetLength());
	file.Close();
	XString8 strCmd="Notepad ";
	strCmd+=strFile;
	WinExec(strCmd,SW_SHOW);
#endif
}

void CMobileView::ViewURL()
{
//	printf("viewURL? NO IMPLEMENTATION!\n");

}

XBOOL CMobileView::OnPaint(XGraphics &g)
{
	return XMainWnd::OnPaint(g);
	
}

void CMobileView::ViewTree()
{
//	printf("No view tree\n");
}

void CMobileView::Layerout()
{
    GetActiveView()->Layerout();
}

MARKLIST* CMobileView::getHistory()
{
	return &m_clientApply.m_lNearest;
}

MARKLIST* CMobileView::getConfigBookmark()
{
    assert(m_clientApply.m_marks.GetSize() == 2);
	return &(m_clientApply.m_marks[0]->lMarks);
}

MARKLIST* CMobileView::getCustomBookmark()
{
	assert(m_clientApply.m_marks.GetSize() == 2);
	return &(m_clientApply.m_marks[1]->lMarks);
}

void CMobileView::updateCustomBookmark(int* ar, int arSize)
{
	assert(m_clientApply.m_marks.GetSize() == 2);
	
	MARKLIST l;
	MARKLIST& old_l = m_clientApply.m_marks[1]->lMarks;
	
	std::vector<bool> temp;
	for (int i = 0; i < old_l.GetSize(); ++i)
	{
		temp.push_back(true);
	}

	for (int i = 0; i < arSize; ++i)
	{
		l.Add(old_l[ar[i]]);
		temp[ar[i]] = false;
	}
	
	for (int i = 0; i < old_l.GetSize(); ++i)
	{
		if (temp[i])
		{
			delete old_l[i];
		}
	}
	
	old_l = l;
}

bool CMobileView::isEmpty() const
{
	return m_empty;
}

bool CMobileView::connectUrl(const char* url, bool isRes)
{
	XDomView* view = GetActiveView();
	CONNECTREQ req;
	req.bForward=true;
	req.nMethod=XEnumMethod::GET;
	req.URL.SetURL(url);
//    if (isRes)
//        req.URL.m_nProto = XPROTO_RES;
	return view->Connect(req, true);
}

bool CMobileView::canBack() const
{
	XDomView* view = m_views[0];
	if (!view->m_bBackList) return false;
	if (view->m_pFlashView) return false;
	return view->m_reqList.GetSize() > 0;
}

bool CMobileView::canForward() const 
{
	XDomView* view = m_views[0];
	if (!view->m_bBackList) return false;
	if (view->m_pFlashView) return false;
	return view->m_forList.GetSize() > 0;
}

bool CMobileView::isFlashStatus() const 
{
	XDomView* view = m_views[0];
	return view->m_pFlashView != 0;
}

void CMobileView::back()
{
	XDomView* view = GetActiveView();
	view->Back();
}

void CMobileView::forward()
{
	XDomView* view = GetActiveView();
	view->Forward();
}

void CMobileView::addCurrentBookmark()
{
	XDomView* view = GetActiveView();
	// add later.
	XPCWSTR strTitle= GetTitle(this);
	if(strTitle)
	{
		view->m_pClientApply->AddMarker(XNULL,strTitle,view->m_req.URL.m_strURL,0,XFALSE);
	}
}

int CMobileView::clearCache()
{
	XCatch c;
	c.Create(m_clientApply.m_strWorkPath);
	return c.Clear(true);
}

void CMobileView::cleanHistory()
{
	XDomView* view = GetActiveView();
	XClientApply* apply = view->m_pClientApply;
	apply->RemoveAllClick();
}

void CMobileView::saveConfig()
{
	XDomView* view = GetActiveView();
	XClientApply* apply = view->m_pClientApply;
	if(apply->m_marks.GetSize()>0) 
		apply->SaveConfig();
}

bool CMobileView::isFlashGame()
{
    return true;
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return false;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return false;
//	}
//	
//	bool bre;
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	//bre = (config->m_nContentType==XConfig::GAME);
//	domView->m_ptLock.UnLock();
//	return bre;
}

//bool CMobileView::hasFlashACC()
//{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return false;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return false;
//	}
//	
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	bool bre = config->m_ACCMaps.GetSize() > 0;
//	domView->m_ptLock.UnLock();
//	return bre;
//}

//bool CMobileView::hasFlashButton()
//{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return false;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return false;
//	}
//	
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	bool bre = config->m_buttonMaps.GetSize() > 0;
//	domView->m_ptLock.UnLock();
//	return bre;
//}

bool CMobileView::canFlashSave()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	
	bool bre = flashView->m_flashSaveReady;
	domView->m_ptLock.UnLock();
	return bre;
}

bool CMobileView::canFlashSize()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	bool bre = flashView->IsEnableZoom();
	domView->m_ptLock.UnLock();
	return bre;
}

bool CMobileView::isFlashScoreAvailable()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	bool bre = flashView->m_topScoreAvailable;
	domView->m_ptLock.UnLock();
	return bre;
}

bool CMobileView::isFlashPause()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	bool bre = flashView->m_bPause;
	domView->m_ptLock.UnLock();
	return bre;
}

//bool CMobileView::isFlashAccEnable()
//{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return false;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return false;
//	}
//	
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	bool bre = config->m_bACCMap;
//	domView->m_ptLock.UnLock();
//	return bre;
//}

//bool CMobileView::isFlashKeyBoardEnable()
//{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return false;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return false;
//	}
//    
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	bool bre = config->m_bButtons;
//	domView->m_ptLock.UnLock();
//	return bre;
//}

bool CMobileView::isCurrentHomePage()
{
	XDomView* domView = GetActiveView();
	
	if (domView->m_req.nMethod == HOMEPAGE)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CMobileView::saveConfirmOK()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	if (flashView->m_pSave)
	{ 
		flashView->PostMsg(XWM_COMMAND, XIDOK, (XU32)this, XMS_THIS);
	}
	
	domView->m_ptLock.UnLock();
}

void CMobileView::flashSave()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_SAVE, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
}

void CMobileView::cancelFlashSave()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	if (flashView->m_pSave)
	{
		flashView->m_pSave->PostMsg(XWM_COMMAND, XIDCANCEL, (XU32)this, XMS_THIS);
	}
	domView->m_ptLock.UnLock();
}

void CMobileView::flashChangeSize()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMMODE, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
}

void CMobileView::topScore()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}

	flashView->PushScore();
	domView->m_ptLock.UnLock();
}

float CMobileView::getCurrentScore()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return 0;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return 0;
	}
	
	float score = flashView->getCurrentScore();
	domView->m_ptLock.UnLock();
	return score;
}

void CMobileView::switchPause()
{
	XDomView* domView = GetActiveView();
	XFlashView* flashView = domView->m_pFlashView;
	
	flashView->m_bPause = !flashView->m_bPause;
	flashView->AddCommand(flashView->m_bPause ? XFlashView::XPAUSE : XFlashView::XRESUME, 0, 0);
	
	flashView->Invalidate();
}

void CMobileView::flashPause()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}

	if (flashView->m_bPause) 
	{
		domView->m_ptLock.UnLock();
		return;
	}
	switchPause();
	domView->m_ptLock.UnLock();
}

void CMobileView::switchHelp()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	flashView->Pause();
	domView->m_ptLock.UnLock();
}

void CMobileView::flashResume()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	if (!flashView->m_bPause) 
	{
		domView->m_ptLock.UnLock();
		return;
	}
	switchPause();
	domView->m_ptLock.UnLock();
}

//void CMobileView::switchAcc()
//{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return;
//	}
//	
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	config->m_bACCMap = !config->m_bACCMap;
//	domView->m_ptLock.UnLock();
//}

//void CMobileView::switchButton()
//{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return;
//	}
//	
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	config->m_bButtons = !config->m_bButtons;
//	
//	flashView->Invalidate();
//	domView->m_ptLock.UnLock();
//}

void CMobileView::exitFlash()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	
	domView->DestoryChild(flashView);
	domView->AddCommand(XWM_COMMAND,XCMD_DOM_VIEW,0);
	domView->m_ptLock.UnLock();
}

const char* CMobileView::getFlashLocalPath()
{
	XDomView* domView = GetActiveView();

	return domView->m_pClientApply->m_strLocalPath.GetData();
}

int CMobileView::flashContentMode()
{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return -1;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return -1;
//	}
//	int mode = flashView->m_nCntMode;
//	domView->m_ptLock.UnLock();
	
    int mode = 0;
	return mode;
}

void CMobileView::flashZoomInOut(int zoom)
{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return;
//	}
//	
//	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMINOUT, zoom+100, XMS_THIS);
//	domView->m_ptLock.UnLock();
}

int CMobileView::flashLockMode()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return 0;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return 0;
	}
	bool bre = flashView->m_player->m_nLockMode==XSWFPlayer::LK_SELECT;
	domView->m_ptLock.UnLock();
	
	return bre;
}

void CMobileView::setFlashLockMode(bool bLock)
{
	XDomView* domView = GetActiveView();          
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	if (!bLock)
		flashView->m_player->m_nLockMode=XSWFPlayer::LK_NONE;
	else 
		flashView->m_player->m_nLockMode=XSWFPlayer::LK_SELECT;
	
	domView->m_ptLock.UnLock();
}

void CMobileView::flashLockZoomInOut()
{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return;
//	}
//	
//	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMINOUT, 0, XMS_THIS);
//	
//	domView->m_ptLock.UnLock();
}

void CMobileView::flashUnLockZoomInOut()
{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView == NULL )
//	{
//		domView->m_ptLock.UnLock();
//		return;
//	}
//	
//	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMINOUT, 0, XMS_THIS);
//	domView->m_ptLock.UnLock();
}

void CMobileView::flashRestart()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	//flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_RESTART, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
}
									   
void CMobileView::setFlashZoomMode(int mode)
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* pConfig = player->GetConfig();
	pConfig->m_nZoomMode = mode;
	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMMODE, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
}

void CMobileView::saveDeviceToken(const char* deviceToken)
{
	XDomView* domView = GetActiveView();
	XClientApply* apply = domView->m_pClientApply;
	XString8 deviceTokenStr8 = deviceToken;
	apply->SetToken(deviceTokenStr8);
}
void CMobileView::flashPointZoomIn()
{
//	XDomView* domView = GetActiveView();
//	if(!domView->m_ptLock.Lock(XLONGTIME))
//		return;
//	XFlashView* flashView = domView->m_pFlashView;
//	if(flashView==NULL)
//	{
//		domView->m_ptLock.UnLock();
//		return;
//	}
//	
//	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_POINTZOOM, 0, XMS_THIS);
//	domView->m_ptLock.UnLock();
}

bool CMobileView::flashPay()
{
	XDomView* domView = GetActiveView();
    bool post = XFALSE;
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return XFALSE;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView==NULL)
	{
		domView->m_ptLock.UnLock();
		return XFALSE;
	}
	
	post = flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_PAY, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
    return post;
}

bool CMobileView::OfferWallflashPay()
{
	XDomView* domView = GetActiveView();
    bool post = XFALSE;
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return XFALSE;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView==NULL)
	{
		domView->m_ptLock.UnLock();
		return XFALSE;
	}
	
	post = flashView->PostMsg(XWM_COMMAND, XCMD_OFFERWALLFLASH_PAY, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
    return post;
}

bool CMobileView::canFlashPay()
{
	XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView==NULL)
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	bool b;
	b = flashView->CanPay();
	domView->m_ptLock.UnLock();
	return b;
}

void CMobileView::repaint()
{
	this->PostMsg(XWM_REPAINT,0,(XU32)this,XMS_THIS);
}

void CMobileView::iosPidPost(const char* oriderid, const char* status, const char* description, const char* chargedata, const char* chargeUrl)
{   
    XDomView* domView = GetActiveView();
    
        XU8Array data;
        XString8 orideridStr(oriderid);
        XString8 statusStr(status);
        XString8 descriptionStr(description);
        XString8 chargedataStr(chargedata);
        data.Append((XU8*)"orderid=",8);
        if (orideridStr.GetLength())
            data.Append((XU8*)orideridStr.GetData(), orideridStr.GetLength());
        else
            data.Append((XU8*)"none", 4);
        data.Append((XU8*)"&status=",8);
        if (statusStr.GetLength())
            data.Append((XU8*)statusStr.GetData(), statusStr.GetLength());
        else
            data.Append((XU8*)"none", 4);
        data.Append((XU8*)"&description=",13);
        if (descriptionStr.GetLength())
            data.Append((XU8*)descriptionStr.GetData(), descriptionStr.GetLength());
        else
            data.Append((XU8*)"none", 4);
        data.Append((XU8*)"&chargedata=",12);
        if (chargedataStr.GetLength())
            data.Append((XU8*)chargedataStr.GetData(), chargedataStr.GetLength());
        else 
            data.Append((XU8*)"none", 4);
        
    void (*P)();
    P = CMobileView::finishFeedback;
    domView->m_chargeUrl = chargeUrl;
    domView->ConnectContext(chargeUrl, XEM::POST, data, (void*)P);
}

void CMobileView::closeSubView()
{
    this->PostMsg(XWM_COMMAND,XCMD_CLOSE_VIEWEX,(XU32)this,XMS_TOPMOST);
}

void CMobileView::finishFeedback()
{
}

bool CMobileView::enc91UID(const char* strCnt)
{
    strUID = strCnt;
    XDomView* domView = GetActiveView();
	XClientApply* apply = domView->m_pClientApply;
    return apply->Enc91UID(strUID);
}

const char* CMobileView::getEnc91UID()
{
    return strUID.GetData();
}

void CMobileView::setMAC(const char* strMAC)
{
    XDomView* domView = GetActiveView();
    XClientApply* apply = domView->m_pClientApply;
    apply->m_info.strMAC.SetString(strMAC, strlen(strMAC));
}

void CMobileView::setEip( const char *strEip )
{
    XDomView *domView = GetActiveView();
    XClientApply *apply = domView->m_pClientApply;
    apply->m_info.strEip.SetString( strEip, strlen( strEip) );
}

 void CMobileView::setIDFV( const char *strIdfv )
{
    XDomView *domView = GetActiveView();
    XClientApply *apply = domView->m_pClientApply;
    //apply->m_info.strIDFV.SetString( strIdfv, strlen( strIdfv) );
    apply->setIDFV( strIdfv );
}

void CMobileView::setSysVer( const char *strSysVer )
{
    XDomView *domView = GetActiveView();
    XClientApply *apply = domView->m_pClientApply;
    apply->setSystemVersion( strSysVer );
}


// 虚拟按键
bool CMobileView::hasFlashButton()
{
    XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	bool bre = config->m_buttonMaps.GetSize() > 0;
	domView->m_ptLock.UnLock();
	return bre;
}

bool CMobileView::isFlashButtonEnable()
{
    XDomView* domView = GetActiveView();
    if(!domView->m_ptLock.Lock(XLONGTIME))
        return false;
    XFlashView* flashView = domView->m_pFlashView;
    if(flashView == NULL )
    {
        domView->m_ptLock.UnLock();
        return false;
    }
   
    XSWFPlayer* player = flashView->GetPlayer();
    XConfig* config = player->GetConfig();
    bool bre = config->m_bButtons;
    domView->m_ptLock.UnLock();
    return bre;
}

void CMobileView::setFlashButtonEnable(bool aEnable)
{
    XDomView* domView = GetActiveView();
    if(!domView->m_ptLock.Lock(XLONGTIME))
        return;
    XFlashView* flashView = domView->m_pFlashView;
    if(flashView == NULL )
    {
        domView->m_ptLock.UnLock();
        return;
    }

    XSWFPlayer* player = flashView->GetPlayer();
    XConfig* config = player->GetConfig();
    config->m_bButtons = aEnable;

    flashView->Invalidate();
    domView->m_ptLock.UnLock();
}

// 重力感应
bool CMobileView::hasFlashAcc()
{
    XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	bool bre = config->m_ACCMaps.GetSize() > 0;
	domView->m_ptLock.UnLock();
	return bre;
}

bool CMobileView::isFlashAccEnable()
{
    XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return false;
	}
	
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	bool bre = config->m_bACCMap;
	domView->m_ptLock.UnLock();
	return bre;
}

void CMobileView::setFlashAccEnable(bool aEnable)
{
    XDomView* domView = GetActiveView();
	if(!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if(flashView == NULL )
	{
		domView->m_ptLock.UnLock();
		return;
	}
	
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	config->m_bACCMap = aEnable;
	domView->m_ptLock.UnLock();
}

// 得到内容的正确状态
int CMobileView::xgg_contentMode()
{
    int mode = EWebGame;
//    XDomView *d_view = GetActiveView();
//    if (!d_view->m_ptLock.Lock(XLONGTIME)) {
//        return mode;
//    }
//    
//    XFlashView *f_view = d_view->m_pFlashView;
//    if(!f_view)
//    {
//        d_view->m_ptLock.UnLock();
//        return mode;
//    }
//    
//    if(f_view->m_nCntMode == XCNT_MODE_NET)
//    {
//        mode = EWebGame;
//    }
//    else 
//    {
//        XSWFPlayer *player = f_view->m_player;
//        if(!player)
//        {
//            d_view->m_ptLock.UnLock();
//            return mode;
//        }
//        if(player->GetConfig()->m_nContentType == XConfig::GAME)
//            mode = ESingleGame;
//        else
//            mode = EVideo;
//    }
//    
//    
//    d_view->m_ptLock.UnLock();
    return mode;
}

bool CMobileView::xgg_getShowRect(int &left,
                                  int &top,
                                  int &width,
                                  int &height)
{
    XDomView *d_view = GetActiveView();
    if(!d_view->m_ptLock.Lock(XLONGTIME))
        return false;
    XFlashView *f_view = d_view->m_pFlashView;
    if(f_view == NULL)
    {
        d_view->m_ptLock.UnLock();
        return false;
    }
    left = f_view->m_showRect.left;
    top = f_view->m_showRect.top;
    width = f_view->m_showRect.Width();
    height = f_view->m_showRect.Height();
    if(width<=0 || height<=0)
    {
        d_view->m_ptLock.UnLock();
        return false;
    }
    d_view->m_ptLock.UnLock();
    return true;
}


const char* CMobileView::getAppUA()
{
    XDomView* domView = GetActiveView();
    XClientApply* apply = domView->m_pClientApply;
    return apply->m_info.strUserAgent.GetData();
}
const char* CMobileView::getAppPushToken()
{
    XDomView* domView = GetActiveView();
    XClientApply* apply = domView->m_pClientApply;
    return apply->m_info.strXToken.GetData();
}
const XString8Array* CMobileView::getAppHeaders()
{
    XDomView* domView = GetActiveView();
    XClientApply* apply = domView->m_pClientApply;
    return &(apply->m_info.strHeaders);
}

void CMobileView::SetSilence(bool bsilence)
{
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
        //		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}
	flashView->GetPlayer()->theSoundMix.SetSilence(bsilence);
	domView->m_ptLock.UnLock();
	return;
}

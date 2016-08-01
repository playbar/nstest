/*
 *  GlobalPortingFunction.cpp
 *  jqbar
 *
 *  Created by lining on 10-10-21.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GlobalPortingFunction.h"

#include "GGBase.h"
#include "GGDom.h"
#include "MobileView.h"

#include <assert.h>
#include <math.h>

#include <CoreGraphics/CoreGraphics.h>
#include "XAcc.h"

#include "Raster_gl.h"
#include "WindowView.h"


XPoint  __gpt;
#define ZC_ABC(x) ((x)<0 ? -(x) : (x))


void* g_newMobileView(int x, int y, int width, int height, void* graphics, void* view, const unsigned short* title, const char* workPath)
{
	CMobileView* mobileView =  new CMobileView();
	XRect rect = XRect(x, y, x + width, y + height);
    //rect += rect;
	mobileView->Create(rect, (XGraphics*)graphics, view, title, workPath);
	mobileView->Handle(XWM_REPAINT, 0, 0);
	return mobileView;
}

void g_deleteMobileView(void* view)
{
	CMobileView* v = (CMobileView*)view;
	v->Close();
	delete (CMobileView*)view;
}

void* g_newGraphics(int width, int height)
{
	XGraphics* g = new XGraphics();
	g->Create(width, height, 32);
	return g;
}

void g_deleteGraphics(void* g)
{
	delete (XGraphics*)g;
}

void g_viewMoveWindow(void* view, void* rect)
{
	XRect* r = (XRect*)rect;
	CMobileView* mobileView = (CMobileView*)view;
	mobileView->MoveWindow(*r);
}

void g_beginMouse( int x, int y, int pos )
{
    CMobileView* mv = CMobileView::s_getView();
    if (mv)
    {
        g_sendBeginMouse( mv, x, y, pos );
    }
}

void g_sendBeginMouse(void* view, int x, int y, int pos)
{
    CMobileView* mobileView = (CMobileView*)view;
	XPoint pt(x, y);
    if(ZC_ABC(pt.x-__gpt.x)<5 && ZC_ABC(pt.y-__gpt.y)<5)
        mobileView->MouseDown(pt, pos);
    else
    {
        if (pos == 0 )
        {
            XPoint pt1(x, y);
           mobileView->MouseMove(pt1, pos);
        }
        mobileView->MouseDown(pt, pos);
    }
}

void g_sendCancelMouse(void* view, int x, int y, int pos)
{

}
void g_moveMouse( int x, int y, int pos )
{
    CMobileView* mv = CMobileView::s_getView();
    if (mv)
    {
        g_sendMoveMouse( mv, x, y, pos );
    }
}

void g_sendMoveMouse(void* view, int x, int y, int pos)
{
	CMobileView* mobileView = (CMobileView*)view;
	XPoint pt(x, y);
	mobileView->MouseMove(pt, pos);
}

void g_endMouse( int x, int y, int pos )
{
    CMobileView* mv = CMobileView::s_getView();
    if (mv)
    {
        g_sendEndMouse( mv, x, y, pos );
    }
}

void g_sendEndMouse(void* view, int x, int y, int pos)
{
	CMobileView* mobileView = (CMobileView*)view;
	XPoint pt(x, y);
	mobileView->MouseUp(pt, pos);
    __gpt=pt;
}

void g_sendClick(void* view, int x, int y, int pos)
{
	CMobileView* mobileView = (CMobileView*)view;
	XPoint pt(x, y);
	mobileView->Click(pt, pos);
}

void g_sendDblClick(void* view, int x, int y, int pos)
{
	CMobileView* mobileView = (CMobileView*)view;
	XPoint pt(x, y);
	mobileView->DblClick(pt, pos);
}

void g_sendInputTextResultToMobileView(void* view, const unsigned short* text)
{
	CMobileView* mobileView = (CMobileView*)view;
	mobileView->PostMsg(XWM_FINISHEDEDIT, (XU32)text, 0, XMS_NORMAL);
}

void g_invokeOnTimer(void* view)
{
	CMobileView* mobileView = (CMobileView*)view;
	mobileView->TimeTick();
}

void g_sendOnIdle(void* view)
{
	CMobileView* mobileView = (CMobileView*)view;
	mobileView->OnIdle();
}
 
void g_sendAccelerometer(void* view, float x, float y, float z)
{
	CMobileView* mobileView = (CMobileView*)view;
	// identity.
	mobileView->ACC(x * 10000, y * 10000, z * 10000);
}



void g_setAcc(float x,float y,float z)
{
    XAcc::SetAcc(x,y,z);//, <#double y#>, <#double z#>)
}



void g_drawToContext(void* c, int width, int height, void* mobileView, int mode)   
{
	CMobileView* view = (CMobileView*)mobileView;
	CGContextRef context = (CGContextRef)c;

	XGraphics g(context, width, height);
	
//	g.begin();
	CGContextSaveGState(context);

	view->Paint(g);
	CGContextRestoreGState(context);
//	g.end();
}

void* g_getHistoryArray(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->getHistory();
}

void* g_getConfigBookmarkArray(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->getConfigBookmark();
}

void* g_getCustomBookmarkArray(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->getCustomBookmark();
}

void g_mobileViewUpdateCustomBookmark(void* mobileView, int* ar, int arSize)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->updateCustomBookmark(ar, arSize);
}

bool g_isMobileViewEmpty(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->isEmpty();
}

bool g_mobileViewConnect(void* mobileView, const char* url)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->connectUrl(url);
}

bool g_mobileViewConnectRes(void* mobileView, const char* url)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->connectUrl(url, true);
}

void g_mobileStop(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->PostMsg(XWM_COMMAND, XCMD_DOM_STOP, 0, XMS_TOPMOST);
}

float g_mobileViewGetCurrentScore(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->getCurrentScore();
}

void g_mobileRefresh(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->PostMsg(XWM_COMMAND, XCMD_DOM_UPDATE, 0, XMS_TOPMOST);
}

void g_mobileLocalHome(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->PostMsg(XWM_COMMAND, XCMD_DOM_HOME, 0, XMS_TOPMOST);
}

void g_mobileBack(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->back();
}

int g_mobileViewCleanCache(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->clearCache();
}

void g_mobileViewCleanHistort(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->cleanHistory();
}

bool g_canMobileViewBack(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->canBack();
}

void g_mobileForward(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->forward();
}

void g_mobileViewAddCurrentBookmark(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->addCurrentBookmark();
}

bool g_canMobileViewForward(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->canForward();
}

void g_mobileAddBookmark(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->addCurrentBookmark();
}

bool g_isMobileViewContainFlash(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->isFlashStatus();
}

void g_mobileViewSaveConfig(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->saveConfig();
}

bool g_isFlashGame(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->isFlashGame();
}

bool g_isFlashStatus(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->isFlashStatus();
}

//bool g_hasFlashACC(void* mobileView)
//{
//	CMobileView* view = (CMobileView*)mobileView;
//	return view->hasFlashACC();
//}

//bool g_hasFlashButton(void* mobileView)
//{
//	CMobileView* view = (CMobileView*)mobileView;
//	return view->hasFlashButton();
//}

bool g_canFlashSave(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->canFlashSave();
}

bool g_canFlashSize(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->canFlashSize();
}

bool g_isFlashScoreAvailable(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->isFlashScoreAvailable();
}

bool g_isFlashPause(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->isFlashPause();
}

//bool g_isFlashAccEnable(void* mobileView)
//{
//	CMobileView* view = (CMobileView*)mobileView;
//	return view->isFlashAccEnable();
//}

//bool g_isFlashKeyBoardEnable(void* mobileView)
//{
//	CMobileView* view = (CMobileView*)mobileView;
//	return view->isFlashKeyBoardEnable();
//}

bool g_isCurrentHomePage(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->isCurrentHomePage();
}

void g_flashSave(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->flashSave();
}

void g_cancelFlashSave(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->cancelFlashSave();
}

void g_flashSwitchHelp(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->switchHelp();
}

void g_flashChangeSize(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->flashChangeSize();
}

void g_topScore(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->topScore();
}

void g_flashSwitchPause(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->switchPause();
}

void g_flashPause(void* mobileView, bool pause)
{
	CMobileView* view = (CMobileView*)mobileView;
	if (pause)
		view->flashPause();
	else
		view->flashResume();
}

//void g_flashSwitchAcc(void* mobileView)
//{
//	CMobileView* view = (CMobileView*)mobileView;
//	view->switchAcc();
//}

//void g_flashSwitchButton(void* mobileView)
//{
//	CMobileView* view = (CMobileView*)mobileView;
//	view->switchButton();
//}

void g_flashExit(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->exitFlash();	
}

void g_flashSaveConfirmOK(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->saveConfirmOK();	
}

const char* g_getFlashLocalPath(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->getFlashLocalPath();
}

//const char* g_getFlashSharedPath(void* mobileView)
//{
//	CMobileView* view = (CMobileView*)mobileView;
//	return view->getFlashSharedPath();
//}

int g_flashContentMode(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->flashContentMode();
}

void g_flashZoomInOut(void* mobileView, int data)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->flashZoomInOut(data);
}

int g_flashLockMode(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->flashLockMode();
}

void g_setFlashLockMode(void* mobileView, bool bLock)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->setFlashLockMode(bLock);
}

void g_flashLockZoomInOut(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->flashLockZoomInOut();
}

void g_flashUnLockZoomInOut(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->flashUnLockZoomInOut();
}

void g_setFlashZoomMode(void* mobileView, int mode)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->setFlashZoomMode(mode);
}

void g_flashRestart(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->flashRestart();
}

void g_saveDeviceToken(void* mobileView, const char* deviceToken)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->saveDeviceToken(deviceToken);
}

void g_flashPointZoomIn(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	view->flashPointZoomIn();
}

bool g_flashPay(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->flashPay();
}

bool g_offerWallFlashPay(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->flashPay();
}

bool g_canFlashPay(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->canFlashPay();
}

void g_repaint(void* mobileView)
{
	CMobileView* view = (CMobileView*)mobileView;
	return view->repaint();
}

void g_iosPidPost(void* mobileView, const char* orderid, const char* status, const char* description, const char* chargedata, const char* chargeUrl)
{
    CMobileView* view = (CMobileView*)mobileView;
    view->iosPidPost(orderid, status, description, chargedata, chargeUrl);
}

void g_closeSubView(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    view->closeSubView();
}

bool g_enc91UID(void* mobileView, const char* strCnt)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->enc91UID(strCnt);
}

const char* g_getEnc91UID(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->getEnc91UID();
}

void g_setMAC(void* mobileView, const char* strMAC)
{
    CMobileView* view = (CMobileView*)mobileView;
    view->setMAC(strMAC);
}

void g_setEip( void *mobileView, const char *strEip )
{
    CMobileView *view = (CMobileView *)mobileView;
    view->setEip( strEip );
}


void g_setIDFV( void *mobileView, const char *strIdfv )
{
    CMobileView *view = (CMobileView *)mobileView;
    view->setIDFV( strIdfv );
}

void g_setSysVer( void *mobileView, const char *strSysVer)
{
    CMobileView *view = (CMobileView *)mobileView;
    view->setSysVer( strSysVer );
}

// 虚拟按键
bool g_hasFlashButton(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->hasFlashButton();
}

bool g_isFlashButtonEnable(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->isFlashButtonEnable();
}

void g_setFlashButtonEnable(void* mobileView,bool aEnable)
{
    CMobileView* view = (CMobileView*)mobileView;
    view->setFlashButtonEnable(aEnable);
}

// 重力感应
bool g_hasFlashAcc(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->hasFlashAcc();
}

bool g_isFlashAccEnable(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->isFlashAccEnable();
}

void g_setFlashAccEnable(void* mobileView,bool aEnable)
{
    CMobileView* view = (CMobileView*)mobileView;
    view->setFlashAccEnable(aEnable);

}

int xgg_contentMode(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->xgg_contentMode();

}

const char* g_getAppUA(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->getAppUA();
}
const char* g_getAppPushTocken(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->getAppPushToken();
}
void* g_getAppHeaders(void* mobileView)
{
    CMobileView* view = (CMobileView*)mobileView;
    return (void*)(view->getAppHeaders());

}

bool xgg_getShowRect(void* mobileView,
                     int *left,
                     int *top,
                     int *width,
                     int *height)
{
    CMobileView* view = (CMobileView*)mobileView;
    return view->xgg_getShowRect(*left, *top, *width, *height);
}

void g_setSoundMixSilence(void* mobileView, bool isSilence)
{
    CMobileView* view = (CMobileView*)mobileView;
    view->SetSilence(isSilence);
}
void g_setGLSize (int w, int h)
{
    RasterGL::sharedRasterGL()->SetWinSize( w, h );
}

void g_setViewPos(int left, int top, int width, int height)
{
    WindowView::sharedWindowView()->SetViewPos(left, top, width, height);
    RasterGL::sharedRasterGL()->setViewPort(left, top, width, height);
}
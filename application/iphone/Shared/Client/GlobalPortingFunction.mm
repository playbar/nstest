/*
 *  GlobalPortingFunction.mm
 *  BaiWan
 *
 *  Created by lining on 10-10-21.
 *  Modified by handong on 11-05-10.
 *  Copyright 2010 JQBar. All rights reserved.
 *
 */

#include "GlobalPortingFunction.h"
#include "GGBase.h"

#include "BaiWanViewController_ios.h"
#include "ClientView_ios.h"
#import "XBWSendMessageContent.h"
#import "OpenGLView.h"
//#import "EAGLView.h"
extern BaiWanViewController_ios* g_explorerController_ios;
extern BWSendMessageContentClass * g_BaiWanPayUrlSavedData;
extern iOSDeviceRelateData g_deviceData;
extern BWSendMessageContentClass *g_BaiWanWeBBUrlSavedData;
extern BWSendMessageContentClass *g_BaiWanIAPUrlSavedData;

void g_setViewMode(void* view, int mode)
{
//#ifdef __IPHONE__
    ClientView_ios* v = (ClientView_ios*)view;
    [v setMode:mode];
//#endif
}

inline void translateValueByDefaultRotateMode(XS32& x, XS32& y, int width, int height)
{
	x -= width;
	XS32 temp = y;
	y = -x;
	x = temp;
}

void g_invalidateView(void* view, void* r)
{
    XRect rect = *((XRect*)r);
    
//#ifdef __IPHONE__
//        ClientView_iPhone* v = (ClientView_iPhone*)view;
//        [[v layer] setNeedsDisplayInRect:CGRectMake(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top)];
//#elif defined(__IPAD__)
        ClientView_ios* v = (ClientView_ios*)view;
        [[v layer] setNeedsDisplayInRect:CGRectMake(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top)];
//#endif
}

void g_copyGraphicsToCurrentContext(void* c, int width, int height, void* graphics, int )
{
	XGraphics* g = (XGraphics*)graphics;
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSaveGState(context);
	XGraphics view_g(context, width, height);
	view_g.Copy(*g, XPoint(0, 0), XRect(0, 0, width, height));
	CGContextRestoreGState(context);
}

void g_wanttoQuit(void* view)
{
}	

void* g_getGlobalGraphics()
{
//#ifdef __IPHONE__
//    return [g_explorerController_iPhone.clientView graphics];
//#elif defined(__IPAD__)
    return [g_explorerController_ios.clientView graphics];
//#endif
}

void g_noticeClientFlashStart(void* view)
{
    
    // 取消自动锁屏
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] noticeFlashStart];
//    
//#if BWMOBISAGE
//    [g_explorerController_iPhone.banner removeFromSuperview];
//    [g_explorerController_iPhone.btn removeFromSuperview];
//#endif
//    
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeFlashStart];
//#endif
}

void g_noticeClientFlashFinish(void* view)
{
    // 允许自动锁屏
    [[UIApplication sharedApplication] setIdleTimerDisabled:NO];
    
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] noticeFlashFinish];
//    
//#if BWMOBISAGE
//    [g_explorerController_iPhone.view addSubview:g_explorerController_iPhone.banner];
//    [g_explorerController_iPhone.view addSubview:g_explorerController_iPhone.btn];
//#endif
    
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeFlashFinish];
//#endif
}

void g_noticeClientPageLoadingStart(void* view)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] noticePageLoadingStart];
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticePageLoadingStart];
//#endif
}

void g_noticeClientPageLoadingFinish(void* view)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] noticePageLoadingFinish];
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticePageLoadingFinish];
//#endif
}

void g_noticeClientPageLoadingPercent(void* view, int percent)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] noticePageLoadingPercent:percent];
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticePageLoadingPercent:percent];
//#endif
}

void g_noticeClientPageNotEmpty(void* view)
{
//#ifdef __IPHONE__
//        ClientView_iPhone* v = (ClientView_iPhone*)view;
//        [[v viewController] pageNotEmpty];
//#elif defined(__IPAD__)
//        ClientView_ios* v = (ClientView_ios*)view;
//        [[v viewController] pageNotEmpty];
//#endif
}

void g_noticeClientTopScoreAvailable(void* view)
{
//#ifdef __IPHONE__
//        ClientView_iPhone* v = (ClientView_iPhone*)view;
//        [[v viewController] topScoreAvailable];
//#elif defined(__IPAD__)
//        ClientView_iPad* v = (ClientView_iPad*)view;
//        [[v viewController] topScoreAvailable];
//#endif
}

void g_noticeFlashSave(void* view, int percent)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    if ([[v viewController] respondsToSelector:@selector(noticeFlashSavingPercent:)])
//        [[v viewController] noticeFlashSavingPercent:percent];
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(noticeFlashSavingPercent:)])
        [[v viewController] noticeFlashSavingPercent:percent];
//#endif
}

void g_noticeClientShowBarAndOnSale(void* view, unsigned int value)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    if ([[v viewController] respondsToSelector:@selector(noticeShowBarAndOnSale:)]) {
//        [[v viewController] noticeShowBarAndOnSale:value];
//    }
//#elif defined (__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(noticeShowBarAndOnSale:)])
        [[v viewController] noticeShowBarAndOnSale:value];
//#endif
}

void g_noticeClientOutLogin(void* view, unsigned int value)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    if ([[v viewController] respondsToSelector:@selector(noticeOutLogin:)]) {
//        [[v viewController] noticeOutLogin:value];
//    }
//#elif defined (__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(noticeOutLogin:)])
        [[v viewController] noticeOutLogin:value];
//#endif 
}

void g_noticeClientAppStatus(void* mobileView, unsigned int status)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)mobileView;
//    if ([[v viewController] respondsToSelector:@selector(noticeNewAppStatus:)])
//        [[v viewController] noticeNewAppStatus:status];
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)mobileView;
    if ([[v viewController] respondsToSelector:@selector(noticeNewAppStatus:)])
        [[v viewController] noticeNewAppStatus:status];
//#endif
}

void g_noticeClientNewApp(void* mobileView, unsigned int url)
{
//#ifdef __IPHONE__ 
//    ClientView_iPhone* v = (ClientView_iPhone*)mobileView;
//    if ([[v viewController] respondsToSelector:@selector(noticeNewAppUrl:)])    
//        [[v viewController] noticeNewAppUrl:url];
//#elif defined(__IPAD__)
    ClientView_ios* v = (ClientView_ios*)mobileView;
    if ([[v viewController] respondsToSelector:@selector(noticeNewAppUrl:)])
        [[v viewController] noticeNewAppUrl:url];
//#endif
}

void g_noticeClientIosPid(void* view, unsigned int value)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    if ([[v viewController] respondsToSelector:@selector(noticeIosPid:)]) {
//        [[v viewController] noticeIosPid:value];
//    }
//#elif defined (__IPAD__)
    if (!g_BaiWanIAPUrlSavedData->parameter1)
    {
        return;
    }
    ClientView_ios* v = (ClientView_ios*)view;
    XLock lock(&(g_BaiWanIAPUrlSavedData->m_lock));
    int lenth = g_BaiWanIAPUrlSavedData->parameter1Len;
    NSString * urlTemp = [[NSString alloc]initWithBytesNoCopy:(void *)g_BaiWanIAPUrlSavedData->parameter1 length:lenth  encoding:NSUTF8StringEncoding freeWhenDone:YES];
    if ([[v viewController] respondsToSelector:@selector(noticeIosPid:)]);
        //[[v viewController] noticeIosPid:urlTemp];
    g_BaiWanIAPUrlSavedData->parameter1 = NULL;
    lock.UnLock();
//#endif
}

void g_noticeClientStopIosPid(void* view, unsigned int value)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    if ([[v viewController] respondsToSelector:@selector(noticeStopIosPid:)]) {
//        [[v viewController] noticeStopIosPid:value];
//    }
//#elif defined (__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(noticeStopIosPid:)])
        [[v viewController] noticeStopIosPid:value];
//#endif
}

void g_noticeClient91Pid(void* view, unsigned int value)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    if ([[v viewController] respondsToSelector:@selector(notice91Pid:)]) {
//        [[v viewController] notice91Pid:value];
//    }
//#elif defined (__IPAD__)
//    ClientView_ios* v = (ClientView_ios*)view;
//    if ([[v viewController] respondsToSelector:@selector(notice91Pid:)])
//        [[v viewController] notice91Pid:value];
//#endif
}

void g_noticeClientThirdPartPid(void* view, unsigned int value)
{
//#ifdef __IPHONE__
//    ClientView_iPhone* v = (ClientView_iPhone*)view;
//    if ([[v viewController] respondsToSelector:@selector(noticeThirdPartPid:)]) {
//        [[v viewController] noticeThirdPartPid:value];
//    }
//#elif defined (__IPAD__)
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(noticeThirdPartPid:)]) {
        [[v viewController] noticeThirdPartPid:value];
    }
//#endif
}

void g_noticeClientOpenSubView(void* mobileView)
{
//#ifdef __IPHONE__
//	ClientView_iPhone* v = (ClientView_iPhone*)mobileView;
//	if ([[v viewController] respondsToSelector:@selector(noticeSubViewOpen)])
//        [[v viewController] noticeSubViewOpen];
//#elif defined(__IPAD__)
	ClientView_ios* v = (ClientView_ios*)mobileView;
    if ([[v viewController] respondsToSelector:@selector(noticeSubViewOpen)])
        [[v viewController] noticeSubViewOpen];
//#endif
}

void g_noticeClientCloseSubView(void* mobileView)
{
//#ifdef __IPHONE__
//	ClientView_iPhone* v = (ClientView_iPhone*)mobileView;
//	if ([[v viewController] respondsToSelector:@selector(noticeSubViewClose)])
//        [[v viewController] noticeSubViewClose];
//#elif defined(__IPAD__)
	ClientView_ios* v = (ClientView_ios*)mobileView;
    if ([[v viewController] respondsToSelector:@selector(noticeSubViewClose)])
        [[v viewController] noticeSubViewClose];
//#endif
}

void g_noticeClientZoomInEnable(void* mobileView)
{
//#ifdef __IPHONE__
//	ClientView_iPhone* v = (ClientView_iPhone*)mobileView;
//	if ([[v viewController] respondsToSelector:@selector(noticeZoominEnable)])
//        [[v viewController] noticeZoominEnable];
//#elif defined(__IPAD__)
	ClientView_ios* v = (ClientView_ios*)mobileView;
    if ([[v viewController] respondsToSelector:@selector(noticeZoominEnable)])
        [[v viewController] noticeZoominEnable];
//#endif
}

void g_noticeClientZoomOutEnable(void* mobileView)
{
//#ifdef __IPHONE__
//	ClientView_iPhone* v = (ClientView_iPhone*)mobileView;
//    if ([[v viewController] respondsToSelector:@selector(noticeZoomoutEnable)])
//        [[v viewController] noticeZoomoutEnable];
//#elif defined(__IPAD__)
	ClientView_ios* v = (ClientView_ios*)mobileView;
	if ([[v viewController] respondsToSelector:@selector(noticeZoomoutEnable)])
        [[v viewController] noticeZoomoutEnable];
//#endif
}

void g_noticeClientUpdateClickOpt(void* mobileView)
{
}

//通知客户端flash可以保存
void g_noticeClientFCanSave(void* view)
{
//#ifdef __IPHONE__
//	ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] noticeCallBackFlashCanSave];
//#elif defined(__IPAD__)
	ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeCallBackFlashCanSave];
//#endif
}
void g_noticeClientFCanPay(void* view)
{
//#ifdef __IPHONE__
//	ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] noticeCallBackFlashCanPay];
//#elif defined(__IPAD__)
	ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeCallBackFlashCanPay];
//#endif
}
void g_noticeClientPayUrl(void* view, unsigned int url)
{
	ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeClientPayUrl:url];
}
void g_noticeFlashToDomTouch(void* view)
{
    ClientView_ios* v = (ClientView_ios*)view;
    [v changeToDomTouch];
}
void g_noticeDomToFlashTouch(void* view)
{
    ClientView_ios* v = (ClientView_ios*)view;
    [v changeToFlashTouch];
}
void g_noticeShowWebView(void* view, unsigned int url)
{
    if ((!g_BaiWanPayUrlSavedData) || (!g_BaiWanPayUrlSavedData->parameter1))
    {
        return;
    }
	ClientView_ios* v = (ClientView_ios*)view;
    XLock lock(&(g_BaiWanPayUrlSavedData->m_lock));
    int lenth = g_BaiWanPayUrlSavedData->parameter1Len;
    NSString * urlTemp = [[NSString alloc]initWithBytesNoCopy:(void *)g_BaiWanPayUrlSavedData->parameter1 length:lenth  encoding:NSUTF8StringEncoding freeWhenDone:YES];
    if(![urlTemp isEqualToString:[v viewController].payUrlWithNum])
    {
        [v viewController].payUrlWithNum = urlTemp;
    }
    [[v viewController] openWebViewWithUrl:[v viewController].payUrlWithNum withIsWebB:NO];
    g_BaiWanPayUrlSavedData->parameter1 = NULL;
    lock.UnLock();
    [urlTemp release];
    
}

void g_noticeShowWebBView(void* view, unsigned int url)
{
    if ((!g_BaiWanWeBBUrlSavedData) || (!g_BaiWanWeBBUrlSavedData->parameter1))
    {
        return;
    }
	ClientView_ios* v = (ClientView_ios*)view;
    XLock lock(&(g_BaiWanWeBBUrlSavedData->m_lock));
    int lenth = g_BaiWanWeBBUrlSavedData->parameter1Len;
    NSString * urlTemp = [[NSString alloc]initWithBytesNoCopy:(void *)g_BaiWanWeBBUrlSavedData->parameter1 length:lenth  encoding:NSUTF8StringEncoding freeWhenDone:YES];

    [[v viewController] openWebViewWithUrl:urlTemp withIsWebB:YES];
    g_BaiWanWeBBUrlSavedData->parameter1 = NULL;
    lock.UnLock();
    [urlTemp release];
    
}

void GFunShowHideMenu(void *view, bool bShow )
{
//#ifdef __IPHONE__
//    //error
//#elif defined(__IPAD__)
    ClientView_ios *v = (ClientView_ios *)view;
    if([[v viewController] respondsToSelector:@selector(NoticeShowHideMenu:)] )
        [[v viewController] NoticeShowHideMenu:bShow];
    return;
//#endif
}

//本地调用，改变工具栏
void xgg_mm_changeBar(void *view)
{
//#ifdef __IPHONE__
//	ClientView_iPhone* v = (ClientView_iPhone*)view;
//    [[v viewController] xgg_changeBar];
//#elif defined(__IPAD__)
	ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] xgg_changeBar];
//#endif
}
void g_noticeWeiXinSendSession(void* view, unsigned int value)
{
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(g_noticeWeiXinSendSession:)]) {
        [[v viewController] g_noticeWeiXinSendSession:value];
    }
}

void g_noticeShowOfferWall(void* view, unsigned int value)
{
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(presentOfferWall)])
    {
        NSString * valueS;
        
        if (value == 0)
            valueS = @"http://BW_DOMOB_OFFERWALL";
        else if (value == 1)
            valueS = @"http://BW_LiMei_OFFERWALL";
        else if (value == 2)
            valueS = @"http://BW_YouMi_OFFERWALL";
        else
            valueS = @"http://BW_Mobisage_OFFERWALL";
        [[v viewController] presentOfferWall: valueS];
    }
}

void g_noticeChangeToOriginalView(void* view)
{
    ClientView_ios* v = (ClientView_ios*)view;
    if ([[v viewController] respondsToSelector:@selector(changeToOriginalView)])
        [[v viewController] changeToOriginalView];
}

void g_noticeClientUpdateStart(void* view, int nMax)
{
    
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeUpdateStart:nMax];
}
void g_noticeClientUpdateSet(void* view, int n)
{
    
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeUpdateSet:n];
}
void g_noticeClientUpdateEnd(void* view)
{
    
    ClientView_ios* v = (ClientView_ios*)view;
    [[v viewController] noticeUpdateEnd];
}

void g_makeCurrent()
{
   [[OpenGLView SharedGLView] makeCurrent ];
}

void g_initGles()
{
    [[OpenGLView SharedGLView] initOpengl ];
}

void g_freshGles()
{
    //[[[OpenGLView SharedGLView] layer ] setNeedsDisplay ];
    [[OpenGLView SharedGLView] swapBuffers ];
}

bool g_openGLViewInitOK()
{
    return [[OpenGLView SharedGLView] initOk];
}

//bool g_checkForExtension()
//{
//    return [OpenGLView CheckForExtension:@"GL_IMG_texture_compression_pvrtc"];
//}

/*
 *  GlobalPortingFunction.h
 *  BaiWan
 *
 *  Created by lining on 10-10-21.
 *  Modified by handong on 11-05-10.
 *  Copyright 2010 JQBar. All rights reserved.
 *
 */

#ifndef _GLOBAL_PORTING_FUNCTION_H_
#define _GLOBAL_PORTING_FUNCTION_H_

// to ClientView .
// ClientView is UIView.
#ifdef __cplusplus
extern "C" {
#endif
	// 获取GC
void* g_getGlobalGraphics();
    // 设置视图方向（mode:0 正常 mode:1 右旋转）
void g_setViewMode(void* view, int mode);
void g_invalidateView(void* view, void* rect);
    // 应用程序要退出
void g_wanttoQuit(void* view);
    // 
void g_copyGraphicsToCurrentContext(void* context, int width, int height, void* graphics, int m_mode);
void g_drawToContext(void* context, int width, int height, void* mobileView, int m_mode);

    // 通知客户端Flash开始播放
void g_noticeClientFlashStart(void* view);
    // 通知客户端Flash结束播放
void g_noticeClientFlashFinish(void* view);
    // 通知客户端开始读取页面
void g_noticeClientPageLoadingStart(void* view);
    // 通知客户端页面读取进度
void g_noticeClientPageLoadingPercent(void* view, int percent);
    // 通知客户端结束读取页面
void g_noticeClientPageLoadingFinish(void* view);
    // 没明白，通知客户端页面非空？
void g_noticeClientPageNotEmpty(void* view);
    // 通知客户端可以上传积分
void g_noticeClientTopScoreAvailable(void* view);
    // 通知客户端Flash保存百分比
void g_noticeFlashSave(void* controller, int percent);
    // 通知客户端showbar和onsale  // 通知是否显示广告
void g_noticeClientShowBarAndOnSale(void* view, unsigned int value);
    
    // 通知客户端服务器对客户端的升级状态
void g_noticeClientAppStatus(void* view, unsigned int value);
	// 通知客户端新版客户端的升级地址
void g_noticeClientNewApp(void* view, unsigned int value);
    // 通知客户端发起向appstore的支付
void g_noticeClientIosPid(void* view, unsigned int value);
    // 通知客户端停止反馈
void g_noticeClientStopIosPid(void* view, unsigned int value);
    // 通知客户端发起向91的支付
void g_noticeClient91Pid(void* view, unsigned int value);
    // 通知客户端发起向UC的支付
void g_noticeClientThirdPartPid(void* view, unsigned int value);
    // 微信消息处理函数
void g_noticeWeiXinSendSession(void* view, unsigned int value);
    // 多盟消息处理
void g_noticeShowOfferWall(void* view, unsigned int value);
    // 通知支付链接地址
void g_noticeClientPayUrl(void* view, unsigned int url);
    // 通知打开webView
void g_noticeShowWebView(void* view, unsigned int url);
    // 通知打开webView
void g_noticeShowWebBView(void* view, unsigned int url);
	// 通知客户端已经打开帮助，传分，付费等子界面
void g_noticeClientOpenSubView(void* mobileView);
	// 通知客户端已经关闭子界面
void g_noticeClientCloseSubView(void* mobileView);
	// 通知客户端可以显示缩小按钮
void g_noticeClientZoomInEnable(void* mobileView);
	// 通知客户端可以显示放大按钮
void g_noticeClientZoomOutEnable(void* mobileView);
    //
void g_noticeClientUpdateClickOpt(void* mobileView);

    // 通知客户端outlogin参数
void g_noticeClientOutLogin(void* view, unsigned int value);
    
//通知客户端flash可以保存
void g_noticeClientFCanSave(void* view);
//通知客户端可以支付
void g_noticeClientFCanPay(void* view);
    
void g_noticeClientUpdateStart(void* view, int nMax);
void g_noticeClientUpdateSet(void* view, int n);
void g_noticeClientUpdateEnd(void* view);
    
void GFunShowHideMenu(void *view, bool bShow );
//切换触摸坐标
void g_noticeFlashToDomTouch(void* view);
void g_noticeDomToFlashTouch(void* view);
//切换到原来的视图
void g_noticeChangeToOriginalView(void* view);
// to mobileView.
// mobileView is XWindow.
void* g_newMobileView(int x, int y, int width, int height, void* graphics, void* view, const unsigned short* title, const char* workPath);
void* g_newGraphics(int width, int height);
void g_deleteMobileView(void* );
void g_deleteGraphics(void*);
void g_viewMoveWindow(void* view, void* rect);

// mouse input
#pragma mark -
#pragma mark Mouse Input
void g_sendBeginMouse(void* view, int x, int y, int pos);
void g_sendCancelMouse(void* view, int x, int y, int pos);
void g_sendMoveMouse(void* view, int x, int y, int pos);
void g_sendEndMouse(void* view, int x, int y, int pos);
void g_sendClick(void* view, int x, int y, int pos);
void g_sendDblClick(void* view, int x, int y, int pos);
// send input text
void g_sendInputTextResultToMobileView(void* view, const unsigned short* text);

void g_invokeOnTimer(void* view);
void g_sendOnIdle(void* view);
void g_sendAccelerometer(void* view, float x, float y, float z);

// mobile view visit function.
#pragma mark -
#pragma mark MobileView visit function
void* g_getHistoryArray(void* mobileView);
void* g_getConfigBookmarkArray(void* mobileView);
void* g_getCustomBookmarkArray(void* mobileView);
void g_mobileViewUpdateCustomBookmark(void* mobileView, int* ar, int arSize);
bool g_isMobileViewEmpty(void* mobileView);
bool g_mobileViewConnect(void* mobileView, const char* url);
bool g_mobileViewConnectRes(void* mobileView, const char* url);    
void g_mobileStop(void* mobileView);

void g_mobileRefresh(void* mobileView);
void g_mobileLocalHome(void* mobileView);
void g_mobileBack(void* mobileView);
bool g_canMobileViewBack(void* mobileView);
void g_mobileForward(void* mobileView);
bool g_canMobileViewForward(void* mobileView);
void g_mobileAddBookmark(void* mobileView);
bool g_isMobileViewContainFlash(void* mobileView);
void g_mobileViewAddCurrentBookmark(void* mobileView);
void g_mobileViewSaveConfig(void* mobileView);
int g_mobileViewCleanCache(void* mobileView);
void g_mobileViewCleanHistort(void* mobileView);

float g_mobileViewGetCurrentScore(void* mobileView);

bool g_isFlashGame(void* mobileView);
bool g_isFlashStatus(void* mobileView);
//bool g_hasFlashACC(void* mobileView);
//bool g_hasFlashButton(void* mobileView);

bool g_isCurrentHomePage(void* mobileView);	
bool g_canFlashSave(void* mobileView);
bool g_canFlashSize(void* mobileView);
bool g_isFlashScoreAvailable(void* mobileView);
bool g_isFlashPause(void* mobileView);
//bool g_isFlashAccEnable(void* mobileView);
//bool g_isFlashKeyBoardEnable(void* mobileView);
    
//------------------
    // 虚拟按键
    bool g_hasFlashButton(void* mobileView);
    bool g_isFlashButtonEnable(void* mobileView);
    void g_setFlashButtonEnable(void* mobileView,bool aEnable);
    // 重力感应
    bool g_hasFlashAcc(void* mobileView);
    bool g_isFlashAccEnable(void* mobileView);
    void g_setFlashAccEnable(void* mobileView,bool aEnable);
    // 获取应用程序信息
    const char* g_getAppUA(void* mobileView);
    const char* g_getAppPushTocken(void* mobileView);
    void* g_getAppHeaders(void* mobileView);
    
//------------------
//Modify by xiepeifu

    void g_setAcc(float x,float y,float z);


void g_flashSave(void* mobileView);
void g_cancelFlashSave(void* mobileView);
void g_flashChangeSize(void* mobileView);
void g_topScore(void* mobileView);
void g_flashSwitchPause(void* mobileView);
//void g_flashSwitchAcc(void* mobileView);
//void g_flashSwitchButton(void* mobileView);
void g_flashExit(void* mobileView);
void g_flashPause(void* mobileView, bool pause);
void g_flashSaveConfirmOK(void* mobileView);
void g_flashSwitchHelp(void* mobileView);
	
const char* g_getFlashLocalPath(void* mobileView);
int g_flashContentMode(void* mobileView);
void g_flashZoomInOut(void* mobileView, int data);
int g_flashLockMode(void* mobileView);
void g_setFlashLockMode(void* mobileView, bool bLock);
	void g_flashLockZoomInOut(void* mobileView);
	void g_flashUnLockZoomInOut(void* mobileView);
	void g_setFlashZoomMode(void* mobileView, int mode);
	void g_flashRestart(void* mobileView);
	
	void g_saveDeviceToken(void* mobileView, const char* deviceToken);
	
	void g_flashPointZoomIn(void* mobileView);
	bool g_flashPay(void* mobileView);
    
    bool g_offerWallFlashPay(void* mobileView);
	bool g_canFlashPay(void* mobileView);
	void g_repaint(void* mobileView);
    
    void g_iosPidPost(void* mobileView, const char* orderid, const char* status, const char* description, const char* chargedata, const char* chargeUrl);
    void g_closeSubView(void* mobileView);
    bool g_enc91UID(void* mobileView, const char* strCnt);
    const char* g_getEnc91UID(void* mobileView);
    void g_setMAC(void* mobileView, const char* strMAC);
    void g_setEip( void *mobileView, const char *strEip );
    void g_setIDFV( void *mobileView, const char *strIdfv );
    void g_setSysVer( void *mobileView, const char *strSysVer);
//-----------------------------------------Naughty cut-off rule-------------------------------------
    //获取内容当前状态状态
    int xgg_contentMode(void* mobileView);
    //本地调用，改变工具栏
    void xgg_mm_changeBar(void *view);
    //获得flash显示区域
    bool xgg_getShowRect(void *mobileView,
                         int *left,
                         int *top,
                         int *width,
                         int *height);
    void g_setSoundMixSilence(void* mobileView, bool isSilence);
    
    void g_makeCurrent();
    void g_initGles();
    void g_freshGles();
    void g_setGLSize( int w, int h );
    void g_setViewPos(int left, int top, int width, int height);
    bool g_openGLViewInitOK();
    
    void g_beginMouse( int x, int y, int pos );
    void g_moveMouse( int x, int y, int pos );
    void g_endMouse( int x, int y, int pos );
    //bool g_checkForExtension();
    
#ifdef __cplusplus
}
#endif

#endif

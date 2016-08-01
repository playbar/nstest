    //
//  BaiWanViewController_iPhone.m
//  BaiWan
//
//  Created by handong on 11-5-11.
//  Copyright 2011 JQBar. All rights reserved.
//

#import "BaiWanViewController_ios.h"
#import "ClientView_ios.h"
#import "ClientWebView.h"
#import "HDToolBarItem.h"
#import "HDToolBarView.h"
#import "HDWidget.h"
#import "HDWidgetView.h"
#import "UIViewController+popController.h"
#import "HDTipsView.h"
#import "web_localViewController_ios.h"
#import "PayWebViewController.h"
#import "webHelpAboutViewController_ios.h"
#include "KeyBoardMiddleLayer.h"
#include "GlobalPortingFunction.h"
#include "XTime.h"
#import "GTMBase64.h"
#include "XString.h"
#include <vector>
#include "XSystem.h"
#import "Reachability.h"
#import "inputView_ios.h"
#import "IPhoneDeviceData.h"
#import "historyViewController_ios.h"
#define ADDLOCAL //本地按钮
#if BWTHIRDPARTSDKISON
#import "ShareSDKInstance.h"
#endif
#ifdef BWWXSDKMODE
#import "UIImage+thumbnailProcessMethod.h"
#import "XBWSendMessageContent.h"
extern BWSendMessageContentClass *g_WeiXinShareSessionSavedData;
extern int g_WeiXinSendType;
#endif
//#ifdef BWALIPAYSDKMODE
#import "PartnerConfig.h"
#import "DataSigner.h"
#import "AlixPayResult.h"
#import "DataVerifier.h"
#import "AlixPayOrder.h"
#import "AlixLibService.h"
#import <Availability.h>
#import "AlixPaylib+extra.h"
#import "AlixViewController.h"
#import "ALPToastView+extra.h"
#import "UIView+BWactivity.h"
//#endif
#define degreeToRadians(x) (3.1415926 * (x) / 180.0 )

#define FIRST_ITEM_TAG  1
#define SECOND_ITEM_TAG 2
#define THIRD_ITEM_TAG  3
#define FORTH_ITEM_TAG  4
#define FIFTH_ITEM_TAG  5
#define LAST_ITEM_TAG   6
#define BAR_BACKGROUND_TAG 10



extern iOSDeviceRelateData g_deviceData;

@interface BaiWanViewController_ios (PrivateMethod)

// 初始化工具栏
- (void)initBar;
// 切换工具栏
- (void)changeBarMode:(EBARMODE)aMode;
// 显示/隐藏更多工具栏
- (void)showMoreBar:(BOOL)aShowEnable;
// 更多工具栏已经隐藏
- (void)moreBarDidHide;
// 初始化widget
- (void)initWidget;
// 读取widget位置
- (void)loadWidgetPosition;
// 存储widget位置
- (void)saveWidgetPosition;
// 切换工具栏
- (void)changeWidgetMode:(EWIDGETMODE)aMode;

// 开始处理消息队列和重力感应
- (void)start;
// 停止处理消息队列和重力感应
- (void)stop;

// 点击浏览器主页选项
- (void)onWebBarHome;
// 点击浏览器本地选项
- (void)onWebBarLocal;
// 点击浏览器后退选项
- (void)onWebBarBack;
// 点击浏览器停止
- (void)onWebBarStop;
// 点击浏览器刷新选项
- (void)onWebBarRefresh;
// 点击浏览器更多选项
- (void)onWebBarMore;
// 点击浏览器更多鼠标指示选项
- (void)onWebMoreBarPoint;
// 点击浏览器更多反馈选项
- (void)onWebMoreBarFeedback;
// 点击浏览器更多清除缓存选项
- (void)onWebMoreBarCleanCache;
// 点击浏览器更多帮助选项
- (void)onWebMoreBarHelp;
// 点击浏览器更多关于选项
- (void)onWebMoreBarAbout;

// 切换浏览器更多鼠标指示图标
- (void)switchWebMoreBarPoint:(BOOL)aShowEnable;

// 单机游戏工具栏
- (void)onGameBarMode;
- (void)onGameBarSetting;
- (void)onGameBarSave;
- (void)onGameBarZoom;
- (void)onGameBarHelp;
- (void)onGameBarClose;

// 短片工具栏
- (void)onVideoBarMode;
- (void)onVideoBarSetting;
- (void)onVideoBarSave;
- (void)onVideoBarZoom;
- (void)onVideoBarClose;

// 网页游戏工具栏
- (void)onNetBarMode;
- (void)onNetBarSetting;
- (void)onNetBarZoom;
- (void)onNetBarRefresh;
- (void)onNetBarPay;
- (void)onNetBarClose;

// widget
- (void)onWidgetMore;
- (void)onWidgetZoom;
- (void)onWidgetSave;
- (void)onWidgetSetting;
- (void)onWidgetHelp;
- (void)onWidgetBack;
- (void)onWidgetPay;
- (void)onWidgetRefresh;
- (void)onWidgetPoint;
- (void)onWidgetButton;
- (void)onWidgetMode;
- (void)onWidgetClose;

// 从设置模式返回时调用
- (void)changeSettingWidgetMode:(EWIDGETMODE)aMode;

// 切换widget鼠标指示图标
- (void)switchWidgetPoint:(BOOL)aShowEnable;
// 切换widget虚拟按键图标
- (void)switchWidgetButton:(BOOL)aShowEnable;
// 过渡动画结束
- (void)tmpAnimationOver;
// 处理机制计时器
//#if BWONSALEMODE
//- (void)handleOnSaleModeTimer;
//#endif
//#if BWOUTLOGINMODE
//- (void)handleOutLoginModeTimer;
//#endif
// 播放本地文件
- (void)playBoutiqueGame;
- (void)playLocalFile;

- (void)waitLoading;


#if BWIAPMODE
- (void)showCurrentStep;
- (void)hideCurrentStep;
- (void)setCurrentStepTitle:(NSString*)title;

- (void)saveIAPData;
- (void)loadIAPData;

- (void)checkStart;
- (void)startFeedBack;
- (void)feedBack;
//- (void)finishFeedBack;
#endif
- (void)initView;

@end




const char* HomeAddress_iPhone = "http://m.jqbar.com/product/Default.aspx";
const char* FeedBack_iPhone = "http://m.jqbar.com/feedback/Default.aspx";


@implementation BaiWanViewController_ios
@synthesize glView = _glView;

#ifdef BWSHOWBARMODE
@synthesize showBarModeEnable;
#endif//BWSHOWBARMODE

@synthesize m_bIsSubview;
@synthesize currentMode;
@synthesize isNetRefreshing;
@synthesize clientView;
@synthesize inputView;
@synthesize launchUrl;
#if BWPUSHMODE
@synthesize isPushUrlOpen;
#endif

#if BWMOBISAGE
@synthesize banner;
@synthesize btn;
#endif

#pragma mark - BaiWanViewController Life Cycle

const float iosversion = [[[UIDevice currentDevice] systemVersion] floatValue];

static const char* __jb_app = NULL;
+ (BOOL)isJailBroken
{
    static const char* __jb_apps[] =
    {
        "/Application/Cydia.app",
        "/Application/limera1n.app",
        "/Application/greenpois0n.app",
        "/Application/blackra1n.app",
        "/Application/blacksn0w.app",
        "/Application/redsn0w.app",
        NULL
    };
    
    __jb_app = NULL;
    
    for ( int i = 0; __jb_apps[i]; ++i )
    {
        if ( [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:__jb_apps[i]]] ) {
            __jb_app = __jb_apps[i];
            return YES;
        }
    }
    if ( 0 == system("ls") )
    {
        return YES;
    }
    return NO;
}

-(id)init
{
    id returned = [super init];
    if (returned)
    {
    #if BWPUSHMODE
        isResignActive = NO;
    #endif
        isWebBViewMode = NO;
        isClientWebViewShow = NO;
    #ifdef BWYMSDKMODE
        self.youMiInstance = nil;
    #endif
    #ifdef BWMBSDKMODE
        self.mobisageInstance = nil;
    #endif
    #ifdef BWDMSDKMODE
        self.domobInstance = nil;
    #endif
    #ifdef BWLMSDKMODE
        self.liMeiInstance = nil;
    #endif
        self.isStypeValid = YES;
        _adViewController = nil;
        _bannerView = nil;
        showADModeEnable = YES;
        isPayWebViewPresented = NO;
        payViewControllor = nil;
        isAutoOrientation = NO;
        if([[[UIDevice currentDevice] systemVersion] hasPrefix:@"8"])
            isAutoOrientation = YES;
        isInited = NO;
        isSaving = NO;
        currentBarView = nil;
        currentBarMode = EBARMODE_NONE;
        currentMode = EMODE_BAR;
        currentBarShowStatus = NO;
        currentMoreBarShowStatus = NO;
        currentWidgetView = nil;
        currentWidgetMode = EWIDGETMODE_NONE;
        currentWidgetShowStatus = NO;
        localViewController = nil;
        iScrollView = NULL;
        m_bIsSubview = false;
        self.isNetRefreshing = FALSE;
#if BWSHOWBARMODE
        showBarModeEnable = NO;
#endif
#if BWONSALEMODE
        onSaleModeEnable = NO;
        onSaleModeTimer = nil;
        showBoutiqueGame = NO;
#endif
        CGSize sz = [[UIScreen mainScreen]bounds].size;
        if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
        {
            g_deviceData.m_screenWidth = sz.width<sz.height ? sz.width : sz.height;
            g_deviceData.m_screenHeight = sz.height>sz.width ? sz.height : sz.width;
        }
        else
        {
            g_deviceData.m_screenWidth = sz.width>sz.height ? sz.width : sz.height;
            g_deviceData.m_screenHeight = sz.height<sz.width ? sz.height : sz.width;
        }
        
        self.clientView = [[ClientView_ios alloc]initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
        [self.clientView setViewController:self];
    }
    

    
    
    // 读取存储的mode
    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSNumber* modeNumber = (NSNumber*)[bwDictionary objectForKey:BWMODEKEY];
    if (modeNumber)
        currentMode = (EMODE)[modeNumber intValue];
#if BWSHOWBARMODE
    // 读取存储的showBarMode
    NSNumber* showBarModeNumber = (NSNumber*)[bwDictionary objectForKey:BWSHOWBARKEY];
    if (showBarModeNumber)
        showBarModeEnable = [showBarModeNumber boolValue];

#endif
    
#if BWONSALEMODE
    // 读取存储的onSaleMode
    NSNumber* onSaleModeNumber = (NSNumber*)[bwDictionary objectForKey:BWONSALEKEY];
    if (onSaleModeNumber)
        onSaleModeEnable = [onSaleModeNumber boolValue];

#endif

    NSNumber* webGameModeNumber = (NSNumber*)[bwDictionary objectForKey:BWWEBGAMEKEY];
    if (webGameModeNumber)
    {
        showBoutiqueGame = ![webGameModeNumber boolValue];
        g_deviceData.m_isWebGame = [webGameModeNumber boolValue];
    }

    NSNumber* deviceOriModeNumber = (NSNumber*)[bwDictionary objectForKey:BWDEVICEORIKEY];
    if (deviceOriModeNumber)
    {
        if ([deviceOriModeNumber boolValue])
            g_deviceData.m_deviceOrientation = UIInterfaceOrientationPortrait;
        else
            g_deviceData.m_deviceOrientation = UIInterfaceOrientationLandscapeRight;
    }
//    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
//    {
//        g_deviceData.m_deviceOrientation = UIInterfaceOrientationPortrait;
//    }
    Reachability *r = [Reachability reachabilityWithHostName:@"www.apple.com"];
#if BWPUSHMODE
#if BWONSALEMODE
    if (onSaleModeEnable && !isPushUrlOpen)
    {
        g_mobileViewConnect(clientView.mobileView, HomeAddress_iPhone);
        onSaleModeTimer = [NSTimer scheduledTimerWithTimeInterval:BWDEFAULTTIMEINTERVAL target:self selector:@selector(handleOnSaleModeTimer) userInfo:nil repeats:NO];
    }
    else if ([r currentReachabilityStatus] != NotReachable && !onSaleModeEnable && !isPushUrlOpen)
    {
        g_mobileViewConnect(clientView.mobileView, HomeAddress_iPhone);
        onSaleModeTimer = [NSTimer scheduledTimerWithTimeInterval:BWDEFAULTTIMEINTERVAL target:self selector:@selector(handleOnSaleModeTimer) userInfo:nil repeats:NO];
    }
    else if ([r currentReachabilityStatus] == NotReachable && !onSaleModeEnable)
    {
        [self playLocalFile];
    }
#else
    if (!isPushUrlOpen)
    {
        g_mobileViewConnect(clientView.mobileView, HomeAddress_iPhone);
    }
#endif
#else
#if BWONSALEMODE
    if (onSaleModeEnable && !isPushUrlOpen) {
        g_mobileViewConnect(clientView.mobileView, HomeAddress_iPhone);
    }else if ([r currentReachabilityStatus] != NotReachable && !onSaleModeEnable && !isPushUrlOpen) {
        g_mobileViewConnect(clientView.mobileView, HomeAddress_iPhone);
        onSaleModeTimer = [NSTimer scheduledTimerWithTimeInterval:BWDEFAULTTIMEINTERVAL target:self selector:@selector(handleOnSaleModeTimer) userInfo:nil repeats:NO];
    }
    else if ([r currentReachabilityStatus] == NotReachable && !onSaleModeEnable)
    {
        [self playLocalFile];
    }
#else
    g_mobileViewConnect(clientView.mobileView, HomeAddress_iPhone);
#endif
#endif
    CGSize sz = [[UIScreen mainScreen]bounds].size;
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        g_deviceData.m_screenWidth = sz.width<sz.height ? sz.width : sz.height;
        g_deviceData.m_screenHeight = sz.height>sz.width ? sz.height : sz.width;
    }
    else
    {
        g_deviceData.m_screenWidth = sz.width>sz.height ? sz.width : sz.height;
        g_deviceData.m_screenHeight = sz.height<sz.width ? sz.height : sz.width;
    }
    [self.clientView setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
    self.glView = [[OpenGLView alloc] initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight) AndDelegate:self];
//    [self.view addSubview:self.clientView];
    [self.view addSubview:self.glView];
    [self.glView setViewController:self];
    self.glView.hidden = YES;
    [self.glView initOpengl];


    return returned;
}
#pragma mark-OpenGLViewDelegate
-(void)successIntoGame:(NSInteger )index
{
    switch (index) {
        case 2:
            [self onNetBarClose];
            
            break;
        case 1:
            [self onNetBarPay];
            
            
            break;
        case 0:
            
            [self onNetBarRefresh];
            
            break;
            
        default:
            break;
    }
}
-(void)OpneGlenTouch
{
    if (timer)
    {
        
        [timer invalidate];
        timer=nil;
    }
    
}
-(void)BarAlphaChange
{
    timer=nil;
    self.glView.bar.alpha=0.5;
}
- (void)viewDidLoad
{
    [super viewDidLoad];
    _currentInterfaceOrientation = g_deviceData.m_deviceOrientation;
    _naviBarInterfaceOrientation = g_deviceData.m_deviceOrientation;
    CGSize sz = [[UIScreen mainScreen]bounds].size;
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        g_deviceData.m_screenWidth = sz.width<sz.height ? sz.width : sz.height;
        g_deviceData.m_screenHeight = sz.height>sz.width ? sz.height : sz.width;
    }
    else
    {
        g_deviceData.m_screenWidth = sz.width>sz.height ? sz.width : sz.height;
        g_deviceData.m_screenHeight = sz.height<sz.width ? sz.height : sz.width;
    }
    [self.view setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
    [self.clientView setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
    
    [self.view addSubview:self.clientView];
//    if(!self.glView)
//        self.glView = [[OpenGLView alloc] initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight) AndDelegate:self];
    //    [self.view addSubview:self.clientView];
//    [self.view addSubview:self.glView];
//    [self.glView setViewController:self];
//    self.glView.hidden = YES;
//    [self.glView initOpengl];
    [self initBar];
    //[self initWidget];
    [self changeBarMode:EBARMODE_WEB];
    
    KeyBoardMiddleLayer::createInstance(self);
    
#if BWTHIRDPARTSDKISON
    outLoginModeEnable = NO;
    viewIsAppeared = NO;
//    outLoginModeTimer = nil;
#endif
#if BWNEWAPPMODE
    newAppStatus = -1;
    newAppUrl = nil;
#endif
    
#if BWMOBISAGE
    
    btn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    btn.frame = CGRectMake(440, 0, 40, 40);
    [btn setTitle:@"关闭" forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(onRemoveAd:) forControlEvents:UIControlEventTouchUpInside];
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onAdViewHideAdWindow:) name:MobiSageAdView_Hide_AD_Window object:nil];

    //此处设置PublishID
    [[MobiSageManager getInstance] setPublisherID:@"cbb8995948fb4c7dbb6f18c3d123b101"];
    
    // 创建一个MobiSageAdBanner实例
    banner = [[MobiSageAdBanner alloc] initWithAdSize:Ad_480X40];
    // 设置广告轮显时间
    [banner setInterval:Ad_Refresh_15];
    // 调整MobiSageAdBanner的位置和大小,前面的两个参数是表示距离左上角的X,Y距离,广告所 要显示的位置
    [banner setFrame:CGRectMake(0,0, 480, 40)];
    
    // 显示MobiSageAdBanner 
    [self.view addSubview:banner];
    if (![banner isHidden]) {
        [self.view addSubview:btn];
    }
//    [self.view addSubview:btn];
    
    [banner setSwitchAnimeType:Random];
#endif
    
    
    [self start];
    

    
//#if BWOUTLOGINMODE
//    // 读取存储的outLoginMode
//    NSNumber* outLoginModeNumber = (NSNumber*)[bwDictionary objectForKey:BWOUTLOGINKEY];
//    if (outLoginModeNumber)
//        outLoginModeEnable = [outLoginModeNumber boolValue];
//#endif

    [self initView];
#ifdef BWMBSDKMODE
    self.mobisageInstance =[[MobisageSDKInstance alloc]init];
    [self.mobisageInstance initOfferWallWithDelegate:self];
#endif
}

#if BWMOBISAGE
- (void)onRemoveAd:(id)sender
{
    [banner removeFromSuperview];
    [btn removeFromSuperview];
}
#endif

- (void)initView
{
    if (!isInited)
    {
        
        [self changeBarMode:EBARMODE_WEB];
        [self changeWidgetMode:EWIDGETMODE_SMALL];

#if BWIAPMODE
        
//        IAPCurrentStepView = [[UIView alloc] initWithFrame:CGRectMake(0,0,g_deviceData.m_screenWidth,g_deviceData.m_screenHeight)];
//        [IAPCurrentStepView setBackgroundColor:[UIColor grayColor]];
//        CGRect stepLabelRect = CGRectMake(g_deviceData.m_screenWidth/3, g_deviceData.m_screenHeight/4, g_deviceData.m_screenWidth/3, g_deviceData.m_screenHeight/4);
//        IAPCurrentStepLabel = [[UILabel alloc] initWithFrame:stepLabelRect];
//        [IAPCurrentStepLabel setBackgroundColor:[UIColor clearColor]];
//        [IAPCurrentStepLabel setTextColor:[UIColor whiteColor]];
//        [IAPCurrentStepLabel setTextAlignment:UITextAlignmentCenter];
//        [IAPCurrentStepView addSubview:IAPCurrentStepLabel];
//        [IAPCurrentStepLabel release];
//        UIButton* cancelButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
//        [cancelButton setFrame:CGRectMake(0, 0, 80, 30)];
//        [cancelButton setTitle:@"取消" forState:UIControlStateNormal];
//        [cancelButton addTarget:self action:@selector(onCurrentStepCancel:) forControlEvents:UIControlEventTouchUpInside];
//        [cancelButton setCenter:CGPointMake(g_deviceData.m_screenWidth/2, (g_deviceData.m_screenHeight*4)/5)];
//        [IAPCurrentStepView addSubview:cancelButton];
//        
//        // 检查存盘的订单信息
          [self loadIAPData];
//        if (IAPStepID==BWIAPSTATUSGETPRODUCTID)
//        {
//            [self showCurrentStep];
//            [self setCurrentStepTitle:@"正在查询产品信息，请稍候..."];
//            [self checkStart];
//        }
//        else if ((IAPStepID==BWIAPSTATUSCHECKSTART)||(IAPStepID==BWIAPSTATUSCHECKOVER))
//        {
//            IAPStepID = BWIAPSTATUSGETPRODUCTID;
//            [self saveIAPData];
//            [self showCurrentStep];
//            [self setCurrentStepTitle:@"正在查询产品信息，请稍后..."];
//            [self checkStart];
//        }
//        else if (IAPStepID==BWIAPSTATUSCHECKSTART)
//        {
//            IAPStepID = BWIAPSTATUSGETPRODUCTID;
//            [self saveIAPData];
//            [self showCurrentStep];
//            [self setCurrentStepTitle:@"正在查询产品信息，请稍后..."];
//            [self checkStart];
//        }
//        else if (IAPStepID==BWIAPSTATUSCHECKOVER)
//        {
//            [self feedBack];
//            [self startFeedBack];
//        }
//        else if (IAPStepID==BWIAPSTATUSFEEDBACK)
//        {
//            [self feedBack];
//            [self startFeedBack];
//        }
        if ((IAPStepID==BWIAPSTATUSCHARGEFINISH)||(IAPStepID==BWIAPSTATUSFEEDBACKSTART))
        {
//            [self feedBack];
            [self startFeedBack];
        }
        else
            IAPStepID=BWIAPSTATUSREADY;
#endif
        
        isInited = YES;
    }

}
//

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
#if BWTHIRDPARTSDKISON // 在游戏界面显示的时候显示SDK的登录窗口
    viewIsAppeared = YES;
    if (outLoginModeEnable)
    {
        [[ShareSDKInstance sharedSDK] login];
    }
#endif
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (_currentInterfaceOrientation == UIInterfaceOrientationPortrait)
    {
        return interfaceOrientation == UIInterfaceOrientationPortrait;
    }
    else
    {
        return interfaceOrientation == UIInterfaceOrientationLandscapeRight;
    }
}

- (BOOL)shouldAutorotate
{
    return isAutoOrientation;
}

- (NSUInteger)supportedInterfaceOrientations
{
    if (_currentInterfaceOrientation == UIInterfaceOrientationPortrait)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    else
    {
        return UIInterfaceOrientationMaskLandscapeRight;
    }
}

//-(UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
//{
//    if (currentInterfaceOrientation == UIInterfaceOrientationPortrait)
//    {
//        return UIInterfaceOrientationPortrait;
//    }
//    else
//    {
//        return UIInterfaceOrientationLandscapeRight;
//    }
//}

- (void)viewDidUnload
{
    BWRELEASE(webBarView);
    BWRELEASE(webBarHomeItem);
    BWRELEASE(webBarLocalItem);
    BWRELEASE(webBarBackItem);
    BWRELEASE(webBarRefreshItem);
    BWRELEASE(webBarMoreItem);
    BWRELEASE(webMoreBarView);
    BWRELEASE(webMoreBarPointItem);
//    BWRELEASE(webMoreBarFeedbackItem);
    BWRELEASE(webMoreBarCleanCacheItem);
    BWRELEASE(webMoreBarHelpItem);
    BWRELEASE(webMoreBarAboutItem);
    BWRELEASE(gameBarView);
    BWRELEASE(gameBarBackgroundImageView);
    BWRELEASE(gameBarModeItem);
    BWRELEASE(gameBarSettingItem);
    BWRELEASE(gameBarSaveItem);
    BWRELEASE(gameBarZoomItem);
    BWRELEASE(gameBarHelpItem);
    BWRELEASE(gameBarCloseItem);
    BWRELEASE(videoBarView);
    BWRELEASE(videoBarBackgroundImageView);
    BWRELEASE(videoBarModeItem);
    BWRELEASE(videoBarSettingItem);
    BWRELEASE(videoBarSaveItem);
    BWRELEASE(videoBarZoomItem);
    BWRELEASE(videoBarCloseItem);
//    BWRELEASE(netBarView);
//    BWRELEASE(netBarBackgroundImageView);
//    BWRELEASE(netBarModeItem);
//    BWRELEASE(netBarSettingItem);
//    BWRELEASE(netBarZoomItem);
//    BWRELEASE(netBarPayItem);
//    BWRELEASE(netBarRefreshItem);
//    BWRELEASE(netBarCloseItem);
    BWRELEASE(widgetView);
    BWRELEASE(loadingView);
    BWRELEASE(timeView);
    BWRELEASE(tmpWidgetView);
    BWRELEASE(moreWidget);
    BWRELEASE(zoomWidget);
    BWRELEASE(moreWidgetView);
    BWRELEASE(moreSaveWidget);
    BWRELEASE(moreSettingWidget);
    BWRELEASE(moreHelpWidget);
    BWRELEASE(morePayWidget);
    BWRELEASE(moreReloadWidget);
    BWRELEASE(moreBackWidget);
    BWRELEASE(moreCloseWidget);
    BWRELEASE(settingWidgetView);
    BWRELEASE(payViewControllor);
    BWRELEASE(morePointWidget);
    BWRELEASE(moreButtonWidget);
    BWRELEASE(localViewController);
    BWRELEASE(inputView);
    BWRELEASE(launchUrl);
    BWRELEASE(defaultSwfPath);
    BWRELEASE(iScrollView);
#if BWNEWAPPMODE
    BWRELEASE(newAppUrl);
#endif
#if BWIAPMODE
    self.IAPChargeData = nil;
    self.IAPProductID = nil;
    self.IAPOrderID = nil;
    self.IAPDestription = nil;
    self.IAPChargeUrlData = nil;
#endif
    
#if BWMOBISAGE
    // 释放banner
    BWRELEASE(banner);
#endif
    
    KeyBoardMiddleLayer::destoryInstance();
    [self stop];
}
-(void)dealloc
{
    [_adViewController release];
    [clientWebView release];
#if BWTHIRDPARTSDKISON
    [[ShareSDKInstance sharedSDK] setDelegate:nil];
#endif
#ifdef BWYMSDKMODE
    self.youMiInstance = nil;
#endif
#ifdef BWMBSDKMODE
    self.mobisageInstance = nil;
#endif
#ifdef BWDMSDKMODE
    self.domobInstance = nil;
#endif
#ifdef BWLMSDKMODE
    self.liMeiInstance = nil;
#endif
 
    if (_bannerView)
    {
        [_bannerView release];
    }
    [super dealloc];
}

#pragma mark - BaiWanViewController PublicMethod

- (void)hideMoreBar
{
    if (currentMoreBarShowStatus)
        [self showMoreBar:NO];
}

- (void)needScrollV:(bool) isNeed
{
    CGRect rt = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight-30);
    if(isNeed)
    {
        iScrollView = [[UIScrollView alloc] initWithFrame:rt];
        
        [self.view addSubview:iScrollView];
        
        [self.clientView removeFromSuperview];
        [iScrollView addSubview:self.clientView];
        iScrollView.contentSize=self.clientView.frame.size;
        BWLOG(@"x=%f,y=%f,width=%f,height=%f",self.clientView.frame.origin.x,self.clientView.frame.origin.y, self.clientView.frame.size.width,self.clientView.frame.size.height);
    }
    else
    {
        if (self.clientView.superview!=self.view)
        {
            [self.clientView removeFromSuperview];
            [self.view addSubview:self.clientView];
        }
        if (iScrollView.superview==self.view)
        {
            [iScrollView removeFromSuperview];
            [iScrollView release];
            iScrollView = nil;
        }
    }
}

- (void)setKeyBoardFrame:(CGRect)frame withDuration:(NSTimeInterval)duration isKeyBoard:(int)aKeyBoard
{
    inputView.hidden = NO;
    CGPoint offsetPoint = CGPointZero;
    if (frame.origin.y <= (self.clientView.lastPoint.y + 30))
        offsetPoint.y = self.clientView.lastPoint.y-frame.origin.y+60;
    //        offsetPoint.y = self.clientView.lastPoint.y -newFrame.origin.y-10;
    
    bool isCenter = frame.origin.y+frame.size.height<g_deviceData.m_screenHeight;
    
    CGRect newFrame;
    if(isCenter)
    {
        newFrame.origin.y = g_deviceData.m_screenHeight-30;
        newFrame.origin.x = 0;
        newFrame.size.width = g_deviceData.m_screenWidth;
        newFrame.size.height = 30;
        CGRect poprt = CGRectMake(0, 0, 320, g_deviceData.m_screenHeight-(frame.origin.y+frame.size.height)-30);
        [inputView.historyViewController.view setFrame:poprt];
        if(inputView.popoverController!=NULL)
        {
            [inputView.popoverController setPopoverContentSize:poprt.size];
            [inputView.popoverController dismissPopoverAnimated:true];
        }
    }
    else
    {
        newFrame.origin.y = frame.origin.y-30;
        newFrame.origin.x = 0;
        newFrame.size.width = g_deviceData.m_screenWidth;
        newFrame.size.height = 30;
        CGRect poprt = CGRectMake(0, 0, 320, 320);
        [inputView.historyViewController.view setFrame:poprt];
        if(inputView.popoverController!=NULL)
        {
            [inputView.popoverController setPopoverContentSize:poprt.size];
            [inputView.popoverController dismissPopoverAnimated:true];
        }
    }
    [inputView setFrame:newFrame];
    
    CGRect temprt;
    if(isCenter)
    {
        temprt = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight-30);
    }
    else
    {
        temprt = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight-frame.size.height-newFrame.size.height);
    }
    if(iScrollView != NULL)
    {
        //              [UIView beginAnimations:nil context:nil];
        [iScrollView setFrame:temprt];
        //              [UIView commitAnimations];
        [iScrollView setContentOffset:offsetPoint animated:YES];
    }
    
}


//设置clientview不可点击
- (void)setNoneTouch:(bool) isTouch
{
    [self.clientView setUserInteractionEnabled:!isTouch];
}

- (void)startEdit
{
    [inputView setFrame:CGRectMake(0, g_deviceData.m_screenHeight, g_deviceData.m_screenWidth, inputView.frame.size.height)];
    if (!inputView.superview) {
        [self.view addSubview:inputView];
        inputView.hidden = YES;
    }
}

- (void)finishEdit
{
    [inputView setFrame:CGRectMake(0, g_deviceData.m_screenHeight, g_deviceData.m_screenWidth, inputView.frame.size.height)];
    [inputView removeFromSuperview];
}

#if BWIAPMODE
//- (IBAction)onCurrentStepCancel:(id)sender
//{
//    if ((IAPStepID==BWIAPSTATUSGETPRODUCTID)||(IAPStepID==BWIAPSTATUSCHECKSTART)||(IAPStepID==BWIAPSTATUSCHECKOVER))
//    {
//        IAPStepID = BWIAPSTATUSFEEDBACK;
//        IAPCodeID = BWIAPERRORCANCELLED;
//        self.IAPDestription = [[NSString alloc] initWithString:@"Cancelled By User"];
//        [self saveIAPData];
//        [self hideCurrentStep];
//        [self feedBack];
//        [self startFeedBack];
//    }
//    else
//    {
//        [self hideCurrentStep];
//        return;
//    }
//}
#endif

- (void)noticeFlashStart
{
    timer= [NSTimer scheduledTimerWithTimeInterval:4 target:self selector:@selector(BarAlphaChange) userInfo:nil repeats:NO];

    //[self showBar:NO];
    [self.glView glviewHide:NO];
    //self.glView.hidden = NO;

    //[self.view addSubview:self.eaglView];
    if(currentMoreBarShowStatus)
        [self showMoreBar:false];
    if(isWebBViewMode && clientWebView.superview)
    {
        [clientWebView removeFromSuperview];
        isClientWebViewShow = NO;
    }
    [self noticePageLoadingFinish];
    
    if (g_deviceData.m_scale > 1.1)
    {
        [clientView noticeFlashStart];
    }

    if (currentMode==EMODE_BAR)
    {
        switch (g_flashContentMode([clientView mobileView]))
        {
            case 0:
            {
                if (g_isFlashGame([clientView mobileView]))
                    [self changeBarMode:EBARMODE_GAME];
                else
                    [self changeBarMode:EBARMODE_VIDEO];
            }
                break;
            case 1:
                [self changeBarMode:EBARMODE_NET];
                break;
        }
    }
    else if (currentMode==EMODE_WIDGET)
    {
        switch (g_flashContentMode([clientView mobileView]))
        {
            case 0:
            {
                [videoBarSaveItem setEnabled:YES];
                if (g_isFlashGame([clientView mobileView]))
                    [self changeBarMode:EBARMODE_GAME];
                else
                    [self changeBarMode:EBARMODE_VIDEO];
            }
                break;
            case 1:
                [self changeBarMode:EBARMODE_NET];
                break;
        }
        [self showBar:NO];
        [self changeWidgetMode:EWIDGETMODE_SMALL];
        [self showWidget:YES];
    }
    [self noticeZoominEnable];
}

- (void)noticeFlashFinish
{
//   [self showBar:YES];
    //self.glView.hidden = YES;
    [self.glView glviewHide:YES];
    if(isWebBViewMode && clientWebView)
    {
       [self.view addSubview:clientWebView];
        isClientWebViewShow = YES;
    }
    if (currentMode==EMODE_BAR)
    {
        [self changeBarMode:EBARMODE_WEB];
    }
    else if (currentMode==EMODE_WIDGET)
    {
        [self showWidget:NO];
        [self changeWidgetMode:EWIDGETMODE_SMALL];
        [self changeBarMode:EBARMODE_WEB];
        [self showBar:YES];
    }
    currentMode = EMODE_BAR;
    currentBarMode = EBARMODE_WEB;
    currentBarView.hidden = NO;
    [moreSaveWidget setEnabled:NO];
    
    [gameBarSaveItem setEnabled:NO];
    
    [videoBarSaveItem setEnabled:NO];
    [morePayWidget setEnabled:NO];
//    [netBarPayItem setEnabled:NO];
    
    if (g_deviceData.m_scale > 1.1)
    {
        [clientView noticeFlashFinish];
    }
}

- (void)noticePageLoadingStart
{
    if (webBarView)
    {
        [webBarView setPercent:0];
        [webBarView showPercent:YES];
    }
    if (isInited)
        [self changeBarMode:EBARMODE_LOADING];
}

- (void)noticePageLoadingPercent:(int)percent
{
    if (webBarView)
        [webBarView setPercent:percent];
}

- (void)noticePageLoadingFinish
{
    if (webBarView)
    {
        [webBarView setPercent:100];
        [webBarView showPercent:NO];
    }
    if (isInited)
        [self changeBarMode:EBARMODE_WEB];
}

- (void)noticeFlashSavingPercent:(int)percent
{
    if (isSaving)
    {
        if (percent>=100)
        {
            if (g_isFlashPause([clientView mobileView]))
                g_flashPause([clientView mobileView], NO);
            
            HDTipsView* tips = [[HDTipsView alloc] initWithText:@"保存完毕"];
            [tips setAutoHide:YES];
            [tips showInView:clientView];
            [tips release];
            isSaving = NO;
        }
    }
}

- (void)noticeSubViewOpen
{
    m_bIsSubview = true;
    if (currentMode==EMODE_BAR)
        [self showBar:NO];
    else if (currentMode==EMODE_WIDGET)
        [self showWidget:NO];
}

- (void)noticeSubViewClose
{
    m_bIsSubview = false;
    if (currentMode==EMODE_BAR)
        [self showBar:YES];
    else if (currentMode==EMODE_WIDGET)
        [self showWidget:YES];
}

- (void)noticeZoominEnable
{
    [zoomWidget setImage:[UIImage imageNamed:@"zoominPlay.png"] forState:UIControlStateNormal];
    [zoomWidget setImage:[UIImage imageNamed:@"zoominPlay_touch.png"] forState:UIControlStateHighlighted];
    [gameBarZoomItem setImage:[UIImage imageNamed:@"zoomin.png"] forState:UIControlStateNormal];
    [gameBarZoomItem setImage:[UIImage imageNamed:@"zoomin_touch.png"] forState:UIControlStateHighlighted];
    [videoBarZoomItem setImage:[UIImage imageNamed:@"zoomin.png"] forState:UIControlStateNormal];
    [videoBarZoomItem setImage:[UIImage imageNamed:@"zoomin_touch.png"] forState:UIControlStateHighlighted];
//    [netBarZoomItem setImage:[UIImage imageNamed:@"zoomin.png"] forState:UIControlStateNormal];
//    [netBarZoomItem setImage:[UIImage imageNamed:@"zoomin_touch.png"] forState:UIControlStateHighlighted];
}

- (void)noticeZoomoutEnable
{
    [zoomWidget setImage:[UIImage imageNamed:@"zoomoutPlay.png"] forState:UIControlStateNormal];
    [zoomWidget setImage:[UIImage imageNamed:@"zoomoutPlay_touch.png"] forState:UIControlStateHighlighted];
    [gameBarZoomItem setImage:[UIImage imageNamed:@"zoomout.png"] forState:UIControlStateNormal];
    [gameBarZoomItem setImage:[UIImage imageNamed:@"zoomout_touch.png"] forState:UIControlStateHighlighted];
    [videoBarZoomItem setImage:[UIImage imageNamed:@"zoomout.png"] forState:UIControlStateNormal];
    [videoBarZoomItem setImage:[UIImage imageNamed:@"zoomout_touch.png"] forState:UIControlStateHighlighted];
//    [netBarZoomItem setImage:[UIImage imageNamed:@"zoomout.png"] forState:UIControlStateNormal];
//    [netBarZoomItem setImage:[UIImage imageNamed:@"zoomout_touch.png"] forState:UIControlStateHighlighted];
}

//可以保存回调
- (void)noticeCallBackFlashCanSave
{
    [moreSaveWidget setEnabled:YES];
    
    [gameBarSaveItem setEnabled:YES];
    
    [videoBarSaveItem setEnabled:YES];
    
}
//可以支付
- (void)noticeCallBackFlashCanPay
{
    [morePayWidget setEnabled:YES];
//    [netBarPayItem setEnabled:YES];
}

- (void)noticeNewAppStatus:(unsigned int)aStatus
{
#if BWNEWAPPMODE
    newAppStatus = *(XU8*)aStatus-48;
#endif
}

- (void)noticeNewAppUrl:(unsigned int)aUrl
{
#if BWNEWAPPMODE
    BWRELEASE(newAppUrl);
    
    XString8 url = *(XString8*)aUrl;
    newAppUrl = [[NSString alloc] initWithCString:url.GetData() encoding:NSUTF8StringEncoding];
    
    if (newAppStatus==1||newAppStatus==2)
    {
        UIAlertView* alertView;
        if (newAppStatus==1)
            alertView = [[UIAlertView alloc] initWithTitle:@"发现新版本" message:@"立刻升级？" delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
        else 
            alertView = [[UIAlertView alloc] initWithTitle:@"发现新版本" message:@"立刻升级" delegate:self cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alertView setTag:EALERTVIEWMODE_UPDATE];
        [alertView show];
        [alertView release];
    }
#endif
}

-(void)setClientViewSize
{
    _currentInterfaceOrientation = g_deviceData.m_deviceOrientation;
    _naviBarInterfaceOrientation = g_deviceData.m_deviceOrientation;
    CGSize sz = [[UIScreen mainScreen]bounds].size;
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        g_deviceData.m_screenWidth = sz.width<sz.height ? sz.width : sz.height;
        g_deviceData.m_screenHeight = sz.height>sz.width ? sz.height : sz.width;
    }
    else
    {
        g_deviceData.m_screenWidth = sz.width>sz.height ? sz.width : sz.height;
        g_deviceData.m_screenHeight = sz.height<sz.width ? sz.height : sz.width;
    }
    [self.view setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
    [self.clientView setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
    [webBarView setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, BWWEBBARVIEWHEIGHT)];
    [self.glView setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight) rotation:false];
}
- (void)noticeShowBarAndOnSale:(unsigned int)value
{
    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSMutableDictionary* bwMutableDictionary = [NSMutableDictionary dictionaryWithDictionary:bwDictionary];
    uint deviceOri = (value&0x10) >> 4;
    if (self.isStypeValid && (deviceOri == 1)) //启动方式，0横屏,1竖屏
    {
        g_deviceData.m_deviceOrientation = UIInterfaceOrientationPortrait;
        [self setClientViewSize];
        [bwMutableDictionary setObject:[NSNumber numberWithBool:YES] forKey:BWDEVICEORIKEY];
    }
    else if (self.isStypeValid && (deviceOri == 0))
    {
        g_deviceData.m_deviceOrientation = UIInterfaceOrientationLandscapeRight;
        [self setClientViewSize];
        [bwMutableDictionary setObject:[NSNumber numberWithBool:NO] forKey:BWDEVICEORIKEY];
    }
    else{} //默认启动方式
    
    showADModeEnable = (value&0x4) >> 2;
    showBoutiqueGame =  (value&0x8) >> 3;
    
    if (onSaleModeTimer)
        BWINVALIDATE(onSaleModeTimer);
 
    if (onSaleModeEnable==NO)
        onSaleModeEnable = (value&0x1);
    if (showBarModeEnable==NO)
        showBarModeEnable = (value&0x2) >> 1;
    
    if (onSaleModeEnable==NO)
    {
        showBarModeEnable = NO;
        [self playLocalFile];
    }
    else 
    {
        
        [bwMutableDictionary setObject:[NSNumber numberWithBool:onSaleModeEnable] forKey:BWONSALEKEY];
        
        if (showBoutiqueGame == 0 && ([self isFileExist:@"game.dat"]))
        {
            //精品
            [bwMutableDictionary setObject:[NSNumber numberWithBool:NO] forKey:BWWEBGAMEKEY];
            showBarModeEnable = NO;
            [self playBoutiqueGame ];
        }
        else
        {
            //网游
            [bwMutableDictionary setObject:[NSNumber numberWithBool:YES] forKey:BWWEBGAMEKEY];
            
            if (g_deviceData.m_isPad)
            {
                g_deviceData.mbOnSale = true;
                [clientView SetNormalScale];
            }
            g_deviceData.m_isWebGame = YES;
            g_deviceData.m_isDomView = true;
            g_deviceData.m_isDomExtView = false;
            
            [bwMutableDictionary setObject:[NSNumber numberWithBool:YES] forKey:BWWEBGAMEKEY];
            if (showBarModeEnable==YES)
            {
                [bwMutableDictionary setObject:[NSNumber numberWithBool:showBarModeEnable] forKey:BWSHOWBARKEY];
                [self showBar:YES];
            }

        }
    }
    [[NSUserDefaults standardUserDefaults] setObject:bwMutableDictionary forKey:BWUSERDEFAULTKEY];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [self showADStart];
}
-(void)showADStart
{
    if (showADModeEnable)
    {
        //百玩广告方式
        if (!_adViewController)
        {
            _adViewController = [[DMAdViewController alloc] init];
            [_adViewController loadAdView];
            _adViewController.view.backgroundColor = [UIColor clearColor];
        }
        
        //        //iad广告方式
        //        if (!_bannerView)
        //        {
        //            if ([ADBannerView instancesRespondToSelector:@selector(initWithAdType:)]) {
        //                _bannerView = [[ADBannerView alloc] initWithAdType:ADAdTypeBanner];
        //            } else {
        //                _bannerView = [[ADBannerView alloc] init];
        //            }
        //            _bannerView.delegate = self;
        //            [self layoutAnimated:YES];
        //            [self.view addSubview:_bannerView];
        //        }
    }
}
- (void)noticeOutLogin:(unsigned int)value
{
#if BWTHIRDPARTSDKISON
        outLoginModeEnable = *(XU8*)value-48; // 是否允许显示登录界面
    
        if (outLoginModeEnable && viewIsAppeared)
        {
            [[ShareSDKInstance sharedSDK] login];
        }
#endif
}

- (void)noticeIosPid:(NSString *)value
{
#if BWIAPMODE
    // 判断客户端状态，非IAPSTATUSOK直接返回
    if (IAPStepID!=BWIAPSTATUSREADY)
    {
//        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"上次充值未完成" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
//        [alertView setTag:EALERTVIEWMODE_UPDATE];
//        [alertView show];
//        [alertView release];
//        BWLOG(@"当前状态不为BWIAPSTATUSREADY,直接丢掉数据");
        NSString* msg = [[NSString alloc] initWithFormat:@"加载中 请等候！"];
        HDTipsView* tipsView = [[HDTipsView alloc] initWithText:msg];
        [tipsView setAutoHide:YES];
        [tipsView showInView: payViewControllor.view];
        [tipsView release];
        [msg release];
        return;
    }

    // 分析数据，如果格式不正确直接返回
    NSArray* pidArray = [value componentsSeparatedByString:@"|"];
    if ([pidArray count]!=4)
    {
        [value release];
        return;
    }
    self.IAPProductID = [pidArray objectAtIndex:0];
    self.IAPOrderID = [pidArray objectAtIndex:1];
    self.IAPChargeUrlData = [pidArray objectAtIndex:2];
//    self.IAPFeedBackParameter = [pidArray objectAtIndex:4];
    [value release];
    
    // 存盘，提示用户，进入流程2
    [self showCurrentStep];
//    [self setCurrentStepTitle:@"收到产品ID和订单ID"];
//    [self saveIAPData];
    [self checkStart];
#endif
}

- (void)noticeStopIosPid:(unsigned int)value
{
#if BWIAPMODE
    NSString* msg = [NSString stringWithCString:(const char*)value encoding:NSUTF8StringEncoding];
    if ([msg isEqualToString:@"iapstatus:0"])
    {
        IAPStepID = BWIAPSTATUSFEEDBACKFINISH;
        [self.IAPFeedBackTimer invalidate];
        self.IAPFeedBackTimer = nil;
        [self saveIAPData];
        
//        NSString* msg = [NSString stringWithCString:(const char*)value encoding:NSUTF8StringEncoding];
//        HDTipsView* tips = [[HDTipsView alloc] initWithText:msg];
//        [tips setAutoHide:YES];
//        [tips showInView:clientView];
//        [tips release];
        
        if(g_isFlashPause(clientView.mobileView))
            g_flashPause(clientView.mobileView, XFALSE);
        IAPStepID = BWIAPSTATUSREADY;
    }
#endif
}
- (void)noticeUpdateStart:(int)nMax
{
    //
}
- (void)noticeUpdateSet:(int)nProgValue
{
    //self.progressValue += 1;
//    self.progressValue = nProgValue;
//    self.pProgress.progress = (float)self.progressValue/(float)self.progressMax;
//    self.pProgressValueLabel.text = [NSString stringWithFormat:@"数据处理:%d/%d",self.progressValue, self.progressMax];
    //if(self.progressValue == self.progressMax)
    //    [self noticeUpdateEnd];
}
- (void)noticeUpdateEnd
{
//    self.pProgress.progress = 0;
//    self.progressMax = 0;
//    self.progressValue = 0;
//    self.pProgress.hidden = YES;
//    self.pProgressValueLabel.hidden = YES;
    //停止计时器
    //[pTimer invalidate];
    //显示按钮
    //pBtton.hidden = NO;
}


#pragma mark - BaiWanViewController PrivateMethod

- (void)initwebBarView
{
//    CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    CGFloat viewWidth = [self.view bounds].size.width;
    webBarView = [[HDToolBarView alloc] initWithFrame:CGRectMake(0, 0, viewWidth, BWWEBBARVIEWHEIGHT)];
    [webBarView setPercentTint:[UIColor greenColor]];
    [webBarView setTrackTint:[UIColor grayColor]];
#ifdef ADDLOCAL
    [webBarView setItemsCount:6];
#else
    [webBarView setItemsCount:5];
#endif
    [webBarView setBackgroundColor:[UIColor clearColor]];
    [webBarView setBackgroundImage:[UIImage imageNamed:@"toolbarBackground.png"]];
    CGRect webBarItemFrame = CGRectMake(0, 0, BWWEBBARITEMWIDTH, BWWEBBARITEMHEIGHT);
    webBarHomeItem = [[HDToolBarItem alloc] initWithFrame:webBarItemFrame];
    [webBarHomeItem setImage:[UIImage imageNamed:@"home.png"] forState:UIControlStateNormal];
    [webBarHomeItem setImage:[UIImage imageNamed:@"home_touch.png"] forState:UIControlStateHighlighted];
    [webBarHomeItem setImage:[UIImage imageNamed:@"home_untouch.png"] forState:UIControlStateDisabled];
    [webBarHomeItem addTarget:self action:@selector(onWebBarHome) forControlEvents:UIControlEventTouchUpInside];
    [webBarView addItem:webBarHomeItem];
#ifdef ADDLOCAL
    webBarLocalItem = [[HDToolBarItem alloc] initWithFrame:webBarItemFrame];
    [webBarLocalItem setImage:[UIImage imageNamed:@"local.png"] forState:UIControlStateNormal];
    [webBarLocalItem setImage:[UIImage imageNamed:@"local_touch.png"] forState:UIControlStateHighlighted];
    [webBarLocalItem setImage:[UIImage imageNamed:@"local_untouch.png"] forState:UIControlStateDisabled];
    [webBarLocalItem addTarget:self action:@selector(onWebBarLocal) forControlEvents:UIControlEventTouchUpInside];
    [webBarView addItem:webBarLocalItem];
#endif
    webBarBackItem = [[HDToolBarItem alloc] initWithFrame:webBarItemFrame];
    [webBarBackItem setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
    [webBarBackItem setImage:[UIImage imageNamed:@"back_touch.png"] forState:UIControlStateHighlighted];
    [webBarBackItem setImage:[UIImage imageNamed:@"back_touch.png"] forState:UIControlStateDisabled];
    [webBarBackItem addTarget:self action:@selector(onWebBarBack) forControlEvents:UIControlEventTouchUpInside];
    [webBarView addItem:webBarBackItem];
    
    webBarRefreshItem = [[HDToolBarItem alloc] initWithFrame:webBarItemFrame];
    [webBarRefreshItem setImage:[UIImage imageNamed:@"refresh.png"] forState:UIControlStateNormal];
    [webBarRefreshItem setImage:[UIImage imageNamed:@"refresh_touch.png"] forState:UIControlStateHighlighted];
    [webBarRefreshItem setImage:[UIImage imageNamed:@"refresh_untouch.png"] forState:UIControlStateDisabled];
    [webBarRefreshItem addTarget:self action:@selector(onWebBarRefresh) forControlEvents:UIControlEventTouchUpInside];
    [webBarView addItem:webBarRefreshItem];
    
    webMoreBarCleanCacheItem = [[HDToolBarItem alloc] initWithFrame:webBarItemFrame];
    [webMoreBarCleanCacheItem setImage:[UIImage imageNamed:@"cleanup.png"] forState:UIControlStateNormal];
    [webMoreBarCleanCacheItem setImage:[UIImage imageNamed:@"cleanup_touch.png"] forState:UIControlStateHighlighted];
    [webMoreBarCleanCacheItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [webMoreBarCleanCacheItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [webMoreBarCleanCacheItem addTarget:self action:@selector(onWebMoreBarCleanCache) forControlEvents:UIControlEventTouchUpInside];
    [webBarView addItem:webMoreBarCleanCacheItem];
    
    webMoreBarAboutItem = [[HDToolBarItem alloc] initWithFrame:webBarItemFrame];
    [webMoreBarAboutItem setImage:[UIImage imageNamed:@"about.png"] forState:UIControlStateNormal];
    [webMoreBarAboutItem setImage:[UIImage imageNamed:@"about_touch.png"] forState:UIControlStateHighlighted];

    [webMoreBarAboutItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [webMoreBarAboutItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [webMoreBarAboutItem addTarget:self action:@selector(onWebMoreBarAbout) forControlEvents:UIControlEventTouchUpInside];
    [webBarView addItem:webMoreBarAboutItem];
    
    
    
//    webBarMoreItem = [[HDToolBarItem alloc] initWithFrame:webBarItemFrame];
//    [webBarMoreItem setImage:[UIImage imageNamed:@"more.png"] forState:UIControlStateNormal];
//    [webBarMoreItem setImage:[UIImage imageNamed:@"more_touch.png"] forState:UIControlStateHighlighted];
//    [webBarMoreItem setImage:[UIImage imageNamed:@"more_untouch.png"] forState:UIControlStateDisabled];
//    [webBarMoreItem addTarget:self action:@selector(onWebBarMore) forControlEvents:UIControlEventTouchUpInside];
//    [webBarView addItem:webBarMoreItem];
    return;
}

- (void)initwebMoreBarView
{
    CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    CGFloat viewWidth = bounds.size.width;

    CGRect webMoreBarItemFrame = CGRectMake(0, 0, BWWEBMOREBARITEMWIDTH, BWWEBMOREBARITEMHEIGHT);
    webMoreBarView = [[HDToolBarView alloc] initWithFrame:CGRectMake(0, 0, viewWidth, BWWEBMOREBARITEMWIDTH)];
    int itemCount = 4;
#if BWTHIRDPARTSDKISON
    itemCount ++;
#endif
    [webMoreBarView setItemsCount:itemCount];
    [webMoreBarView setBackgroundImage:[UIImage imageNamed:@"toolMoreBarBackground.png"]];
    
    webMoreBarPointItem = [[HDToolBarItem alloc] initWithFrame:webMoreBarItemFrame];
    [self switchWebMoreBarPoint:[self.clientView mouseShowEnable]];
    [webMoreBarPointItem setTitle:@"鼠标指示" forState:UIControlStateNormal];
    [webMoreBarPointItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [webMoreBarPointItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [webMoreBarPointItem addTarget:self action:@selector(onWebMoreBarPoint) forControlEvents:UIControlEventTouchUpInside];
    [webMoreBarView addItem:webMoreBarPointItem];
    
    webMoreBarCleanCacheItem = [[HDToolBarItem alloc] initWithFrame:webMoreBarItemFrame];
    [webMoreBarCleanCacheItem setImage:[UIImage imageNamed:@"cleanup.png"] forState:UIControlStateNormal];
    [webMoreBarCleanCacheItem setImage:[UIImage imageNamed:@"cleanup_touch.png"] forState:UIControlStateHighlighted];
    [webMoreBarCleanCacheItem setTitle:@"清理缓存" forState:UIControlStateNormal];
    [webMoreBarCleanCacheItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [webMoreBarCleanCacheItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [webMoreBarCleanCacheItem addTarget:self action:@selector(onWebMoreBarCleanCache) forControlEvents:UIControlEventTouchUpInside];
    [webMoreBarView addItem:webMoreBarCleanCacheItem];
    
//    webMoreBarFeedbackItem = [[HDToolBarItem alloc] initWithFrame:webMoreBarItemFrame];
//    [webMoreBarFeedbackItem setImage:[UIImage imageNamed:@"feedback.png"] forState:UIControlStateNormal];
//    [webMoreBarFeedbackItem setImage:[UIImage imageNamed:@"feedback_touch.png"] forState:UIControlStateHighlighted];
//    [webMoreBarFeedbackItem setTitle:@"反馈" forState:UIControlStateNormal];
//    [webMoreBarFeedbackItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
//    [webMoreBarFeedbackItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
//    [webMoreBarFeedbackItem addTarget:self action:@selector(onWebMoreBarFeedback) forControlEvents:UIControlEventTouchUpInside];
//    [webMoreBarView addItem:webMoreBarFeedbackItem];
    
#if BWTHIRDPARTSDKISON
    webMoreBarSDKCenterItem = [[HDToolBarItem alloc] initWithFrame:webMoreBarItemFrame];
    [webMoreBarSDKCenterItem setImage:[UIImage imageNamed:@"userCenter.png"] forState:UIControlStateNormal];
    [webMoreBarSDKCenterItem setImage:[UIImage imageNamed:@"userCenter_touch.png"] forState:UIControlStateHighlighted];
    [webMoreBarSDKCenterItem setTitle:@"用户中心" forState:UIControlStateNormal];
    [webMoreBarSDKCenterItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [webMoreBarSDKCenterItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [webMoreBarSDKCenterItem addTarget:self action:@selector(onWebMoreBarSDKCenter) forControlEvents:UIControlEventTouchUpInside];
    [webMoreBarView addItem:webMoreBarSDKCenterItem];
#endif
    webMoreBarHelpItem = [[HDToolBarItem alloc] initWithFrame:webMoreBarItemFrame];
    [webMoreBarHelpItem setImage:[UIImage imageNamed:@"morehelp.png"] forState:UIControlStateNormal];
    [webMoreBarHelpItem setImage:[UIImage imageNamed:@"morehelp_touch.png"] forState:UIControlStateHighlighted];
    [webMoreBarHelpItem setTitle:@"帮助" forState:UIControlStateNormal];
    [webMoreBarHelpItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [webMoreBarHelpItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [webMoreBarHelpItem addTarget:self action:@selector(onWebMoreBarHelp) forControlEvents:UIControlEventTouchUpInside];
    [webMoreBarView addItem:webMoreBarHelpItem];
//    
//    webMoreBarAboutItem = [[HDToolBarItem alloc] initWithFrame:webMoreBarItemFrame];
//    [webMoreBarAboutItem setImage:[UIImage imageNamed:@"about.png"] forState:UIControlStateNormal];
//    [webMoreBarAboutItem setImage:[UIImage imageNamed:@"about_touch.png"] forState:UIControlStateHighlighted];
//    [webMoreBarAboutItem setTitle:@"关于" forState:UIControlStateNormal];
//    [webMoreBarAboutItem setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
//    [webMoreBarAboutItem setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
//    [webMoreBarAboutItem addTarget:self action:@selector(onWebMoreBarAbout) forControlEvents:UIControlEventTouchUpInside];
//    [webMoreBarView addItem:webMoreBarAboutItem];
    return;
}

- (void) initgameBarView
{
    CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    CGFloat viewWidth = bounds.size.width;
//    CGFloat viewHeight = bounds.size.height;
    CGFloat webbarItemFrameWidth = viewWidth / 6;

    CGRect webBarItemFrame = CGRectMake(0, 0, webbarItemFrameWidth, BWWEBBARITEMHEIGHT);

    gameBarView = [[HDToolBarView alloc] initWithFrame:CGRectMake(0,0,viewWidth,BWWEBBARVIEWHEIGHT)];
    [gameBarView setDelegate:self];
    [gameBarView setBackgroundColor:[UIColor clearColor]];
    gameBarBackgroundImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"toolbarBackground.png"]];
    [gameBarBackgroundImageView setContentMode:UIViewContentModeScaleToFill];
    [gameBarBackgroundImageView setFrame:CGRectMake(0,0,viewWidth,BWWEBBARVIEWHEIGHT)];
    gameBarBackgroundImageView.tag = BAR_BACKGROUND_TAG;
    [gameBarView addSubview:gameBarBackgroundImageView];
    
    gameBarModeItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [gameBarModeItem setFrame:webBarItemFrame];
    [gameBarModeItem setImage:[UIImage imageNamed:@"mode.png"] forState:UIControlStateNormal];
    [gameBarModeItem setImage:[UIImage imageNamed:@"mode_touch.png"] forState:UIControlStateHighlighted];
    [gameBarModeItem addTarget:self action:@selector(onGameBarMode) forControlEvents:UIControlEventTouchUpInside];
    gameBarModeItem.tag = FIRST_ITEM_TAG;
    [gameBarView addSubview:gameBarModeItem];
    
    gameBarSettingItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [gameBarSettingItem setFrame:CGRectMake(webbarItemFrameWidth,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [gameBarSettingItem setImage:[UIImage imageNamed:@"setting.png"] forState:UIControlStateNormal];
    [gameBarSettingItem setImage:[UIImage imageNamed:@"setting_touch.png"] forState:UIControlStateHighlighted];
    [gameBarSettingItem addTarget:self action:@selector(onGameBarSetting) forControlEvents:UIControlEventTouchUpInside];
    gameBarSettingItem.tag = SECOND_ITEM_TAG;
    [gameBarView addSubview:gameBarSettingItem];
    
    gameBarSaveItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [gameBarSaveItem setFrame:CGRectMake(webbarItemFrameWidth*2,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [gameBarSaveItem setImage:[UIImage imageNamed:@"save.png"] forState:UIControlStateNormal];
    [gameBarSaveItem setImage:[UIImage imageNamed:@"save_touch.png"] forState:UIControlStateHighlighted];
    [gameBarSaveItem setImage:[UIImage imageNamed:@"save_untouch.png"] forState:UIControlStateDisabled];
    [gameBarSaveItem addTarget:self action:@selector(onGameBarSave) forControlEvents:UIControlEventTouchUpInside];
    gameBarSaveItem.tag = THIRD_ITEM_TAG;
    [gameBarSaveItem setEnabled:NO];
    [gameBarView addSubview:gameBarSaveItem];
    
    gameBarZoomItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [gameBarZoomItem setFrame:CGRectMake(webbarItemFrameWidth*3,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [gameBarZoomItem setImage:[UIImage imageNamed:@"zoomout.png"] forState:UIControlStateNormal];
    [gameBarZoomItem setImage:[UIImage imageNamed:@"zoomout_touch.png"] forState:UIControlStateHighlighted];
    [gameBarZoomItem addTarget:self action:@selector(onGameBarZoom) forControlEvents:UIControlEventTouchUpInside];
    gameBarZoomItem.tag = FORTH_ITEM_TAG;
    [gameBarView addSubview:gameBarZoomItem];
    
    gameBarHelpItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [gameBarHelpItem setFrame:CGRectMake(webbarItemFrameWidth*4,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [gameBarHelpItem setImage:[UIImage imageNamed:@"help.png"] forState:UIControlStateNormal];
    [gameBarHelpItem setImage:[UIImage imageNamed:@"help_touch.png"] forState:UIControlStateHighlighted];
    [gameBarHelpItem addTarget:self action:@selector(onGameBarHelp) forControlEvents:UIControlEventTouchUpInside];
    gameBarHelpItem.tag = FIFTH_ITEM_TAG;
    [gameBarView addSubview:gameBarHelpItem];
    
    gameBarCloseItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [gameBarCloseItem setFrame:CGRectMake(viewWidth-webbarItemFrameWidth,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [gameBarCloseItem setImage:[UIImage imageNamed:@"close.png"] forState:UIControlStateNormal];
    [gameBarCloseItem setImage:[UIImage imageNamed:@"close_touch.png"] forState:UIControlStateHighlighted];
    [gameBarCloseItem addTarget:self action:@selector(onGameBarClose) forControlEvents:UIControlEventTouchUpInside];
    gameBarCloseItem.tag = LAST_ITEM_TAG;
    [gameBarView addSubview:gameBarCloseItem];
    return;

}

- (void) initvideoBarView
{
    CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    CGFloat viewWidth = bounds.size.width;
//    CGFloat webbarItemFrameWidth = viewWidth / 6;
    
    videoBarView = [[HDToolBarView alloc] initWithFrame:CGRectMake(0,0,viewWidth,BWWEBBARVIEWHEIGHT)];
    [videoBarView setDelegate:self];
    [videoBarView setBackgroundColor:[UIColor clearColor]];
    videoBarBackgroundImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"toolbarBackground.png"]];
    [videoBarBackgroundImageView setContentMode:UIViewContentModeScaleToFill];
    [videoBarBackgroundImageView setFrame:CGRectMake(0,0,viewWidth,BWWEBBARVIEWHEIGHT)];
    videoBarBackgroundImageView.tag = BAR_BACKGROUND_TAG;
    [videoBarView addSubview:videoBarBackgroundImageView];
    
    videoBarModeItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [videoBarModeItem setFrame:CGRectMake(0,0,BWWEBBARITEMWIDTH,BWWEBBARITEMHEIGHT)];
    [videoBarModeItem setImage:[UIImage imageNamed:@"mode.png"] forState:UIControlStateNormal];
    [videoBarModeItem setImage:[UIImage imageNamed:@"mode_touch.png"] forState:UIControlStateHighlighted];
    [videoBarModeItem addTarget:self action:@selector(onVideoBarMode) forControlEvents:UIControlEventTouchUpInside];
    videoBarModeItem.tag = FIRST_ITEM_TAG;
    [videoBarView addSubview:videoBarModeItem];
    
    videoBarSettingItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [videoBarSettingItem setFrame:CGRectMake(BWWEBBARITEMWIDTH,0,BWWEBBARITEMWIDTH,BWWEBBARITEMHEIGHT)];
    [videoBarSettingItem setImage:[UIImage imageNamed:@"setting.png"] forState:UIControlStateNormal];
    [videoBarSettingItem setImage:[UIImage imageNamed:@"setting_touch.png"] forState:UIControlStateHighlighted];
    [videoBarSettingItem addTarget:self action:@selector(onVideoBarSetting) forControlEvents:UIControlEventTouchUpInside];
    videoBarSettingItem.tag = SECOND_ITEM_TAG;
    [videoBarView addSubview:videoBarSettingItem];
    
    videoBarSaveItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [videoBarSaveItem setFrame:CGRectMake(BWWEBBARITEMWIDTH*2,0,BWWEBBARITEMWIDTH,BWWEBBARITEMHEIGHT)];
    [videoBarSaveItem setImage:[UIImage imageNamed:@"save.png"] forState:UIControlStateNormal];
    [videoBarSaveItem setImage:[UIImage imageNamed:@"save_touch.png"] forState:UIControlStateHighlighted];
    [videoBarSaveItem setImage:[UIImage imageNamed:@"save_untouch.png"] forState:UIControlStateDisabled];
    [videoBarSaveItem addTarget:self action:@selector(onVideoBarSave) forControlEvents:UIControlEventTouchUpInside];
    videoBarSaveItem.tag = THIRD_ITEM_TAG;
    [videoBarSaveItem setEnabled:NO];
    [videoBarView addSubview:videoBarSaveItem];
    
    videoBarZoomItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [videoBarZoomItem setFrame:CGRectMake(BWWEBBARITEMWIDTH*3,0,BWWEBBARITEMWIDTH,BWWEBBARITEMHEIGHT)];
    [videoBarZoomItem setImage:[UIImage imageNamed:@"zoomin.png"] forState:UIControlStateNormal];
    [videoBarZoomItem setImage:[UIImage imageNamed:@"zoomin_touch.png"] forState:UIControlStateHighlighted];
    [videoBarZoomItem addTarget:self action:@selector(onVideoBarZoom) forControlEvents:UIControlEventTouchUpInside];
    videoBarZoomItem.tag = FORTH_ITEM_TAG;
    [videoBarView addSubview:videoBarZoomItem];
    
    videoBarCloseItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [videoBarCloseItem setFrame:CGRectMake(viewWidth-BWWEBBARITEMWIDTH,0,BWWEBBARITEMWIDTH,BWWEBBARITEMHEIGHT)];
    [videoBarCloseItem setImage:[UIImage imageNamed:@"close.png"] forState:UIControlStateNormal];
    [videoBarCloseItem setImage:[UIImage imageNamed:@"close_touch.png"] forState:UIControlStateHighlighted];
    [videoBarCloseItem addTarget:self action:@selector(onVideoBarClose) forControlEvents:UIControlEventTouchUpInside];
    videoBarCloseItem.tag = LAST_ITEM_TAG;
    [videoBarView addSubview:videoBarCloseItem];
    return;
}
/*
-(void)initnetBarView
{
    CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    CGFloat viewWidth = bounds.size.width;
    CGFloat webbarItemFrameWidth = viewWidth / 6;
    
    netBarView = [[HDToolBarView alloc] initWithFrame:CGRectMake(0,0,viewWidth,BWWEBBARVIEWHEIGHT)];
    [netBarView setDelegate:self];
    [netBarView setBackgroundColor:[UIColor clearColor]];
    netBarBackgroundImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"toolbarBackground.png"]];
    [netBarBackgroundImageView setContentMode:UIViewContentModeScaleToFill];
    [netBarBackgroundImageView setFrame:CGRectMake(0,0,viewWidth,BWWEBBARVIEWHEIGHT)];
    netBarBackgroundImageView.tag = BAR_BACKGROUND_TAG;
    [netBarView addSubview:netBarBackgroundImageView];
    
    netBarModeItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [netBarModeItem setFrame:CGRectMake(0,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [netBarModeItem setImage:[UIImage imageNamed:@"mode.png"] forState:UIControlStateNormal];
    [netBarModeItem setImage:[UIImage imageNamed:@"mode_touch.png"] forState:UIControlStateHighlighted];
    [netBarModeItem addTarget:self action:@selector(onNetBarMode) forControlEvents:UIControlEventTouchUpInside];
    netBarModeItem.tag = FIRST_ITEM_TAG;
    [netBarView addSubview:netBarModeItem];
    
    netBarSettingItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [netBarSettingItem setFrame:CGRectMake(webbarItemFrameWidth,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [netBarSettingItem setImage:[UIImage imageNamed:@"setting.png"] forState:UIControlStateNormal];
    [netBarSettingItem setImage:[UIImage imageNamed:@"setting_touch.png"] forState:UIControlStateHighlighted];
    [netBarSettingItem addTarget:self action:@selector(onNetBarSetting) forControlEvents:UIControlEventTouchUpInside];
    netBarSettingItem.tag = SECOND_ITEM_TAG;
    [netBarView addSubview:netBarSettingItem];
    
    netBarZoomItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [netBarZoomItem setFrame:CGRectMake(webbarItemFrameWidth*2,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [netBarZoomItem setImage:[UIImage imageNamed:@"zoomin.png"] forState:UIControlStateNormal];
    [netBarZoomItem setImage:[UIImage imageNamed:@"zoomin_touch.png"] forState:UIControlStateHighlighted];
    [netBarZoomItem addTarget:self action:@selector(onNetBarZoom) forControlEvents:UIControlEventTouchUpInside];
    netBarZoomItem.tag = THIRD_ITEM_TAG;
    [netBarView addSubview:netBarZoomItem];
    
    netBarPayItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [netBarPayItem setFrame:CGRectMake(webbarItemFrameWidth*3,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [netBarPayItem setImage:[UIImage imageNamed:@"pay.png"] forState:UIControlStateNormal];
    [netBarPayItem setImage:[UIImage imageNamed:@"pay_touch.png"] forState:UIControlStateHighlighted];
    [netBarPayItem addTarget:self action:@selector(onNetBarPay) forControlEvents:UIControlEventTouchUpInside];
    [netBarPayItem setEnabled:NO];
    netBarPayItem.tag = FORTH_ITEM_TAG;
    [netBarView addSubview:netBarPayItem];
    
    netBarRefreshItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [netBarRefreshItem setFrame:CGRectMake(webbarItemFrameWidth*4,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [netBarRefreshItem setImage:[UIImage imageNamed:@"refresh.png"] forState:UIControlStateNormal];
    [netBarRefreshItem setImage:[UIImage imageNamed:@"refresh_touch.png"] forState:UIControlStateHighlighted];
    [netBarRefreshItem addTarget:self action:@selector(onNetBarRefresh) forControlEvents:UIControlEventTouchUpInside];
    netBarRefreshItem.tag = FIFTH_ITEM_TAG;
    [netBarView addSubview:netBarRefreshItem];
    
    netBarCloseItem = [UIButton buttonWithType:UIButtonTypeCustom];
    [netBarCloseItem setFrame:CGRectMake(viewWidth-webbarItemFrameWidth,0,webbarItemFrameWidth,BWWEBBARITEMHEIGHT)];
    [netBarCloseItem setImage:[UIImage imageNamed:@"close.png"] forState:UIControlStateNormal];
    [netBarCloseItem setImage:[UIImage imageNamed:@"close_touch.png"] forState:UIControlStateHighlighted];
    [netBarCloseItem addTarget:self action:@selector(onNetBarClose) forControlEvents:UIControlEventTouchUpInside];
    netBarCloseItem.tag = LAST_ITEM_TAG;
    [netBarView addSubview:netBarCloseItem];
    return;
    
}
*/
- (void)initBar
{
    [self initwebBarView];
    [self initwebMoreBarView];
    [self initgameBarView];
    [self initvideoBarView ];
//    [self initnetBarView ];
}

- (void)showBar:(BOOL)aShowEnable
{
#if BWSHOWBARMODE
    if (!showBarModeEnable)
    {
        BWWARNING(@"当前模式不允许显示工具栏");
        return;
    }
#endif
    
    if (currentBarShowStatus==aShowEnable)
    {
        BWWARNING(@"工具栏已经显示/隐藏，请勿重复设置");
        return;
    }
    else

        currentBarShowStatus = aShowEnable;


    if (currentBarShowStatus)
    {
        
        if (currentBarView&&(currentBarView.superview==nil))
        {
            CGRect bounds = self.view.bounds;
            CGFloat viewHeight = bounds.size.height;
            CGFloat viewWidth = bounds.size.width;
            CGRect barFrame = [currentBarView bounds];
            CGRect clientFrame = [clientView bounds];
            barFrame.origin.y = viewHeight - barFrame.size.height;
            barFrame.size.width = viewWidth;
            clientFrame.size.width = viewWidth;
            //clientFrame.size.height = viewHeight+0.01;
            clientFrame.size.height = viewHeight - barFrame.size.height + 2; // HDToolBarView顶端有两个像素的进度条
            [currentBarView setFrame:barFrame];
            [clientView setFrame:clientFrame];
            
            // 以下对 net game vedio bar之外适配
            UIView * backGroundView = [currentBarView viewWithTag:BAR_BACKGROUND_TAG];
            if (backGroundView)
            {
                CGRect backGroundFrame = backGroundView.frame;
                backGroundFrame.size.width = viewWidth;
                [backGroundView setFrame:backGroundFrame];
                CGFloat itemViewWidth = 0;
                if(g_deviceData.m_screenWidth > 320)
                {
                    itemViewWidth = 64;
                }
                else
                {
                    itemViewWidth = g_deviceData.m_screenWidth/6;
                }
                for (int i = 1; i < 6; i++)
                {
                    UIView * itemView = [currentBarView viewWithTag:i];
                    if (itemView)
                    {
                        CGRect itemViewFrame = itemView.frame;
                        itemViewFrame.origin.x = itemViewWidth * (i - 1);
                        itemViewFrame.size.width = itemViewWidth;
                        [itemView setFrame:itemViewFrame];
                    }
                }
                UIView * lastView = [currentBarView viewWithTag:LAST_ITEM_TAG];
                CGRect itemViewFrame = lastView.frame;
                itemViewFrame.origin.x = viewWidth - itemViewWidth;
                itemViewFrame.size.width = itemViewWidth;
                [lastView setFrame:itemViewFrame];
            }
            else  // 如果是web bar 
            {
                if ((g_deviceData.m_screenWidth > g_deviceData.m_screenHeight)&&(g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait))
                {
                    return; // web bar 在横屏模式下不显示
                }
            }
            // 适配结束
            
            [self.view addSubview:currentBarView];
            
        }
        else 
            ;//BWWARNING(@"工具栏已经添加到主视图中，请勿重复添加");
    }
    else 
    {
        if (currentBarView&&(currentBarView.superview!=nil))
        {
            CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
            CGFloat viewHeight = bounds.size.height;
            CGRect clientFrame = [clientView bounds];
            clientFrame.size.height = viewHeight;
            clientFrame.size.width = bounds.size.width;
            [clientView setFrame:clientFrame];
            
            [currentBarView removeFromSuperview];
        }
        else 
            ;//BWWARNING(@"工具栏已经从主视图中移除，请勿重复移除");
    }
    switch (currentBarMode) {
        case EBARMODE_GAME:
        case EBARMODE_VIDEO:
        case EBARMODE_NET:
            currentBarView.hidden = YES;
            break;
        default:
            break;
    }
}

- (void)changeBarMode:(EBARMODE)aMode
{
    
    BOOL reAddBarView = NO;
    
    if (currentBarMode==aMode)
    {
        //BWWARNING(@"工具栏当前已是需要的模式，请勿在相同模式上切换");
        return;
    }
    else
    {
        currentBarMode = aMode;
        if (currentBarShowStatus)
        {
            [self showBar:NO];
            reAddBarView = YES;
        }
    }
    
    switch (currentBarMode)
    {
        case EBARMODE_WEB:
        {
            [webBarHomeItem setEnabled:YES];
#ifdef ADDLOCAL
            [webBarLocalItem setEnabled:YES];
#endif
            [webBarBackItem setEnabled:NO];
            [webBarRefreshItem setEnabled:YES];
            [webBarMoreItem setEnabled:YES];
            [webBarRefreshItem setImage:[UIImage imageNamed:@"refresh.png"] forState:UIControlStateNormal];
            [webBarRefreshItem setImage:[UIImage imageNamed:@"refresh_touch.png"] forState:UIControlStateHighlighted];
            [webBarRefreshItem removeTarget:self action:@selector(onWebBarStop) forControlEvents:UIControlEventTouchUpInside];
            [webBarRefreshItem addTarget:self action:@selector(onWebBarRefresh) forControlEvents:UIControlEventTouchUpInside];
            void* mobileView = [self.clientView mobileView];
            if (isClientWebViewShow)
            {
                [webBarBackItem setEnabled:[clientWebView canGoBack]];
            }
            else if (isWebBViewMode && (!isClientWebViewShow))
            {
                [webBarBackItem setEnabled:YES];
            }
            else
            {
                if (g_canMobileViewBack(mobileView))
                    [webBarBackItem setEnabled:YES];
                else
                    [webBarBackItem setEnabled:NO];
            }
            [webBarBackItem setNeedsDisplay];
            currentBarView = webBarView;
        }
            break;
        case EBARMODE_LOADING:
        {
            [webBarHomeItem setEnabled:NO];
            //[webBarLocalItem setEnabled:NO];
            [webBarBackItem setEnabled:YES];
            //[webBarRefreshItem setEnabled:NO];
            [webBarMoreItem setEnabled:NO];
            [webBarRefreshItem setImage:[UIImage imageNamed:@"close.png"] forState:UIControlStateNormal];
            [webBarRefreshItem setImage:[UIImage imageNamed:@"close_touch.png"] forState:UIControlStateHighlighted];
            [webBarRefreshItem removeTarget:self action:@selector(onWebBarRefresh) forControlEvents:UIControlEventTouchUpInside];
            [webBarRefreshItem addTarget:self action:@selector(onWebBarStop) forControlEvents:UIControlEventTouchUpInside];
            
            [webBarRefreshItem setNeedsDisplay];
            currentBarView = webBarView;
        }
            break;
        case EBARMODE_GAME:
        {
            currentBarView = gameBarView;
        }
            break;
        case EBARMODE_VIDEO:
        {
            currentBarView = (UIView*)videoBarView;
        }
            break;
        case EBARMODE_NET:
        {
            currentBarView = nil;//netBarView;
        }
            break;
        default:
            break;
    }
    
    if (reAddBarView)
        [self showBar:YES];
}

- (void)showMoreBar:(BOOL)aShowEnable
{
    if (currentMoreBarShowStatus==aShowEnable)
    {
        //BWWARNING(@"更多工具栏已经显示/隐藏，请勿重复设置");
        return;
    }
    else
        currentMoreBarShowStatus = aShowEnable;
/*
    if (currentMoreBarShowStatus)
    {
        if (webMoreBarView&&(webMoreBarView.superview==nil))
        {
            // 计算初始添加位置
            CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
            CGFloat viewHeight = bounds.size.height;
            CGRect webMoreBarOldFrame = [webMoreBarView bounds];
            webMoreBarOldFrame.origin.y = viewHeight;
            [webMoreBarView setFrame:webMoreBarOldFrame];
            [self.view insertSubview:webMoreBarView belowSubview:currentBarView];
            
            // 计算最终移动位置
            CGRect webMoreBarNewFrame = webMoreBarOldFrame;
            CGRect barBounds = [currentBarView bounds];
            webMoreBarNewFrame.origin.y = viewHeight - barBounds.size.height -webMoreBarNewFrame.size.height + 2; // HDToolBarView顶端有两个像素的进度条
            
            // 移动
            [UIView beginAnimations:@"showMoreBar" context:nil];
            [webMoreBarView setFrame:webMoreBarNewFrame];
            [UIView commitAnimations];
        }
        else
            BWWARNING(@"更多工具栏已经添加到主视图中，请勿重复添加");
    }
    else
    {
        if (webMoreBarView&&(webMoreBarView.superview!=nil))
        {
            // 计算最终移动位置
            CGRect webMoreBarOldFrame = [webMoreBarView frame];
            CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
            CGFloat viewHeight = bounds.size.height;
            CGRect webMoreBarNewFrame = webMoreBarOldFrame;
            webMoreBarNewFrame.origin.y = viewHeight;
            
            // 移动
            [UIView beginAnimations:@"showMoreBar" context:nil];
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(moreBarDidHide)];
            [webMoreBarView setFrame:webMoreBarNewFrame];
            [UIView commitAnimations];
        }
    }*/
}

- (void)moreBarDidHide
{
    if (webMoreBarView&&(webMoreBarView.superview!=nil))
        [webMoreBarView removeFromSuperview];
}

- (void)initWidget
{
    widgetView = [[HDWidgetView alloc] initWithType:HDWidgetViewTypeAuto];
    [widgetView setDelegate:self];
    [widgetView setFrame:CGRectMake(0,10,BWSMALLWIDGETWIDTH,BWSMALLWIDGETHEIGHT)];
    [widgetView setMoveEnabled:YES];
    [widgetView setViewController:self];
    [widgetView setDelegate:self];
    
    CGRect smallWidgetFrame = CGRectMake(0,0,BWSMALLWIDGETITEMWIDTH,BWSMALLWIDGETITEMHEIGHT);
    moreWidget = [[HDWidget widget] retain];
    [moreWidget setFrame:smallWidgetFrame];
    [moreWidget setMoveEnabled:YES];
    [moreWidget setImage:[UIImage imageNamed:@"morePlay.png"] forState:UIControlStateNormal];
    [moreWidget setImage:[UIImage imageNamed:@"morePlay_touch.png"] forState:UIControlStateHighlighted];
    [moreWidget addTarget:self action:@selector(onWidgetMore) forControlEvents:UIControlEventTouchUpInside];
    
    zoomWidget = [[HDWidget widget] retain];
    [zoomWidget setFrame:smallWidgetFrame];
    [zoomWidget setMoveEnabled:YES];
    [zoomWidget setImage:[UIImage imageNamed:@"zoominPlay.png"] forState:UIControlStateNormal];
    [zoomWidget setImage:[UIImage imageNamed:@"zoominPlay_touch.png"] forState:UIControlStateHighlighted];
    [zoomWidget addTarget:self action:@selector(onWidgetZoom) forControlEvents:UIControlEventTouchUpInside];
    [widgetView addWidget:moreWidget row:0 column:0];
    [widgetView addWidget:zoomWidget row:0 column:1];
    
    CGSize loadingSize = [[UIImage imageNamed:@"1.png"] size];
    CGRect loadingRect = CGRectMake(56,0,loadingSize.width,loadingSize.height);
    NSArray* loadingImageArray = [[NSArray alloc] initWithObjects:[UIImage imageNamed:@"1.png"],[UIImage imageNamed:@"2.png"],[UIImage imageNamed:@"3.png"],[UIImage imageNamed:@"4.png"],[UIImage imageNamed:@"5.png"],[UIImage imageNamed:@"6.png"],[UIImage imageNamed:@"7.png"],[UIImage imageNamed:@"8.png"],[UIImage imageNamed:@"9.png"],nil];
    loadingView = [[UIImageView alloc] initWithFrame:loadingRect];
    [loadingView setAnimationImages:loadingImageArray];
    [loadingView setAnimationDuration:1.8];
    [loadingImageArray release];
    [loadingView startAnimating];
    [widgetView addSubview:loadingView];
    [loadingView setHidden:YES];
    
    timeView = [[HDTimeLabel alloc] initWithFrame:CGRectMake(5,0,30,15)];
    [timeView setTextColor:[UIColor whiteColor]];
    [widgetView addSubview:timeView];
    
    moreWidgetView = [[HDWidgetView alloc] initWithType:HDWidgetViewTypeDefault];
    [moreWidgetView setFrame:CGRectMake(0,0,BWBIGWIDGETWIDTH,BWBIGWIDGETHEIGHT)];
    CGRect bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    [moreWidgetView setCenter:CGPointMake(bounds.size.width/2,bounds.size.height/2)];
    settingWidgetView = [[HDWidgetView alloc] initWithType:HDWidgetViewTypeDefault];
    [settingWidgetView setFrame:CGRectMake(0,0,BWBIGWIDGETWIDTH,BWBIGWIDGETHEIGHT)];
    [settingWidgetView setCenter:CGPointMake(bounds.size.width/2,bounds.size.height/2)];
    
    moreSaveWidget = [[HDWidget widget] retain];
    [moreSaveWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreSaveWidget setImage:[UIImage imageNamed:@"savePlay.png"] forState:UIControlStateNormal];
    [moreSaveWidget setImage:[UIImage imageNamed:@"savePlay_touch.png"] forState:UIControlStateHighlighted];
    [moreSaveWidget setImage:[UIImage imageNamed:@"savePlay_untouch.png"] forState:UIControlStateDisabled];
    [moreSaveWidget setTitle:@"保存" forState:UIControlStateNormal];
    [moreSaveWidget setTitle:@"保存" forState:UIControlStateHighlighted];
    [moreSaveWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreSaveWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreSaveWidget addTarget:self action:@selector(onWidgetSave) forControlEvents:UIControlEventTouchUpInside];
    [moreSaveWidget setEnabled:NO];
    
    moreSettingWidget = [[HDWidget widget] retain];
    [moreSettingWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreSettingWidget setImage:[UIImage imageNamed:@"settingPlay.png"] forState:UIControlStateNormal];
    [moreSettingWidget setImage:[UIImage imageNamed:@"settingPlay_touch.png"] forState:UIControlStateHighlighted];
    [moreSettingWidget setTitle:@"设置" forState:UIControlStateNormal];
    [moreSettingWidget setTitle:@"设置" forState:UIControlStateHighlighted];
    [moreSettingWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreSettingWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreSettingWidget addTarget:self action:@selector(onWidgetSetting) forControlEvents:UIControlEventTouchUpInside];
    
    moreHelpWidget = [[HDWidget widget] retain];
    [moreHelpWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreHelpWidget setImage:[UIImage imageNamed:@"helpPlay.png"] forState:UIControlStateNormal];
    [moreHelpWidget setImage:[UIImage imageNamed:@"helpPlay_touch.png"] forState:UIControlStateHighlighted];
    [moreHelpWidget setTitle:@"帮助" forState:UIControlStateNormal];
    [moreHelpWidget setTitle:@"帮助" forState:UIControlStateHighlighted];
    [moreHelpWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreHelpWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreHelpWidget addTarget:self action:@selector(onWidgetHelp) forControlEvents:UIControlEventTouchUpInside];
    
    morePayWidget = [[HDWidget widget] retain];
    [morePayWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [morePayWidget setImage:[UIImage imageNamed:@"payPlay.png"] forState:UIControlStateNormal];
    [morePayWidget setImage:[UIImage imageNamed:@"payPlay_touch.png"] forState:UIControlStateHighlighted];
    [morePayWidget setTitle:@"充值" forState:UIControlStateNormal];
    [morePayWidget setTitle:@"充值" forState:UIControlStateHighlighted];
    [morePayWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [morePayWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [morePayWidget addTarget:self action:@selector(onWidgetPay) forControlEvents:UIControlEventTouchUpInside];
    [morePayWidget setEnabled:NO];
    
    moreReloadWidget = [[HDWidget widget] retain];
    [moreReloadWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreReloadWidget setImage:[UIImage imageNamed:@"refreshPlay.png"] forState:UIControlStateNormal];
    [moreReloadWidget setImage:[UIImage imageNamed:@"refreshPlay_touch.png"] forState:UIControlStateHighlighted];
    [moreReloadWidget setTitle:@"重新加载" forState:UIControlStateNormal];
    [moreReloadWidget setTitle:@"重新加载" forState:UIControlStateHighlighted];
    [moreReloadWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreReloadWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreReloadWidget addTarget:self action:@selector(onWidgetRefresh) forControlEvents:UIControlEventTouchUpInside];
    
    morePointWidget = [[HDWidget widget] retain];
    [morePointWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [morePointWidget setTitle:@"鼠标指示" forState:UIControlStateNormal];
    [morePointWidget setTitle:@"鼠标指示" forState:UIControlStateHighlighted];
    [morePointWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [morePointWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [morePointWidget addTarget:self action:@selector(onWidgetPoint) forControlEvents:UIControlEventTouchUpInside];
    
    moreButtonWidget = [[HDWidget widget] retain];
    [moreButtonWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreButtonWidget setTitle:@"虚拟按键" forState:UIControlStateNormal];
    [moreButtonWidget setTitle:@"虚拟按键" forState:UIControlStateHighlighted];
    [moreButtonWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreButtonWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreButtonWidget addTarget:self action:@selector(onWidgetButton) forControlEvents:UIControlEventTouchUpInside];
    
    moreModeWidget = [[HDWidget widget] retain];
    [moreModeWidget setFrame:CGRectMake(0, 0, BWBIGWIDGETITEMWIDTH, BWBIGWIDGETITEMHEIGHT)];
    [moreModeWidget setTitle:@"菜单切换" forState:UIControlStateNormal];
    [moreModeWidget setTitle:@"菜单切换" forState:UIControlStateHighlighted];
    [moreModeWidget setImage:[UIImage imageNamed:@"modePlay.png"] forState:UIControlStateNormal];
    [moreModeWidget setImage:[UIImage imageNamed:@"modePlay_touch.png"] forState:UIControlStateHighlighted];
    [moreModeWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreModeWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreModeWidget addTarget:self action:@selector(onWidgetMode) forControlEvents:UIControlEventTouchUpInside];
    
    moreBackWidget = [[HDWidget widget] retain];
    [moreBackWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreBackWidget setImage:[UIImage imageNamed:@"backPlay.png"] forState:UIControlStateNormal];
    [moreBackWidget setImage:[UIImage imageNamed:@"backPlay_touch.png"] forState:UIControlStateHighlighted];
    [moreBackWidget addTarget:self action:@selector(onWidgetBack) forControlEvents:UIControlEventTouchUpInside];
    
    moreCloseWidget = [[HDWidget widget] retain];
    [moreCloseWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreCloseWidget setImage:[UIImage imageNamed:@"exitPlay.png"] forState:UIControlStateNormal];
    [moreCloseWidget setImage:[UIImage imageNamed:@"exitPlay_touch.png"] forState:UIControlStateHighlighted];
    [moreCloseWidget setTitle:@"退出播放" forState:UIControlStateNormal];
    [moreCloseWidget setTitle:@"退出播放" forState:UIControlStateHighlighted];
    [moreCloseWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreCloseWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreCloseWidget addTarget:self action:@selector(onWidgetClose) forControlEvents:UIControlEventTouchUpInside];
    
#if SDKNEEDUSERCENTER
    moreUserCenterWidget = [[HDWidget widget] retain];
    [moreUserCenterWidget setFrame:CGRectMake(0,0,BWBIGWIDGETITEMWIDTH,BWBIGWIDGETITEMHEIGHT)];
    [moreUserCenterWidget setImage:[UIImage imageNamed:@"userCenterWidget.png"] forState:UIControlStateNormal];
    [moreUserCenterWidget setImage:[UIImage imageNamed:@"userCenterWidget_touch.png"] forState:UIControlStateHighlighted];
    [moreUserCenterWidget setTitle:@"用户中心" forState:UIControlStateNormal];
    [moreUserCenterWidget setTitle:@"用户中心" forState:UIControlStateHighlighted];
    [moreUserCenterWidget setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [moreUserCenterWidget setTitleColor:[UIColor colorWithRed:239.0/255.0 green:169.0/255.0 blue:48.0/255.0 alpha:1.0] forState:UIControlStateHighlighted];
    [moreUserCenterWidget addTarget:self action:@selector(onWebMoreBarSDKCenter) forControlEvents:UIControlEventTouchUpInside];
#endif
    tmpWidgetView = [[HDWidgetView alloc] initWithType:HDWidgetViewTypeDefault];
}

- (void)showWidget:(BOOL)aShowEnable
{
    if (currentWidgetShowStatus==aShowEnable)
    {
        //BWWARNING(@"widget已经显示/隐藏，请勿重复设置");
        return;
    }
    else
        currentWidgetShowStatus = aShowEnable;
    
    if (currentWidgetShowStatus)
    {
        currentMode = EMODE_WIDGET;
        if (currentWidgetView&&(currentWidgetView.superview==nil))
            [self.view addSubview:currentWidgetView];
        else
            ;//BWWARNING(@"widget已经添加到主视图中，请勿重复添加");
    }
    else
    {
        //self.currentMode = EMODE_BAR;
        if (currentWidgetView&&(currentWidgetView.superview!=nil))
            [currentWidgetView removeFromSuperview];
        else
            ;//BWWARNING(@"widget已经从主视图中移除，请勿重复移除");
    }
}

- (void)loadWidgetPosition
{}

- (void)saveWidgetPosition
{}

- (void)changeWidgetMode:(EWIDGETMODE)aMode
{
    BOOL reAddWidgetView = NO;
    
    if (currentWidgetMode==aMode)
    {
        //BWWARNING(@"widget当前已经是需要的模式，请勿在相同的模式上切换");
        return;
    }
    else
    {
        currentWidgetMode = aMode;
        if (currentWidgetShowStatus)
        {
            CGRect oldFrame = [currentWidgetView frame];
            [tmpWidgetView setFrame:oldFrame];
            [self showWidget:NO];
            reAddWidgetView = YES;
        }
    }
    
    
    
    
    switch (currentWidgetMode) 
    {
        case EWIDGETMODE_SMALL:
        //{
        //    currentWidgetView = widgetView;
        //}
        //    break;
        case EWIDGETMODE_GAME:
        {
            [moreWidgetView removeAllWidgets];
            [moreWidgetView addWidget:moreSaveWidget row:0 column:1];
            [moreWidgetView addWidget:moreSettingWidget row:1 column:0];
            [moreWidgetView addWidget:moreBackWidget row:1 column:1];
            [moreWidgetView addWidget:moreHelpWidget row:1 column:2];
            [moreWidgetView addWidget:moreCloseWidget row:2 column:1];
            currentWidgetView = moreWidgetView;
        }
            break;
        case EWIDGETMODE_VIDEO:
        {
            [moreWidgetView removeAllWidgets];
            [moreWidgetView addWidget:moreSaveWidget row:0 column:1];
            [moreWidgetView addWidget:moreSettingWidget row:1 column:0];
            [moreWidgetView addWidget:moreBackWidget row:1 column:1];
            [moreWidgetView addWidget:moreCloseWidget row:2 column:1];
            currentWidgetView = moreWidgetView;
        }
            break;
        case EWIDGETMODE_NET:
        {
            [moreWidgetView removeAllWidgets];
            [moreWidgetView addWidget:morePayWidget row:0 column:1];
            [moreWidgetView addWidget:moreSettingWidget row:1 column:0];
            [moreWidgetView addWidget:moreBackWidget row:1 column:1];
            [moreWidgetView addWidget:moreReloadWidget row:1 column:2];
            [moreWidgetView addWidget:moreCloseWidget row:2 column:1];
            currentWidgetView = moreWidgetView;
        }
            break;
        case EWIDGETMODE_SETTING:
        {
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            
            [currentWidgetView moveWidget:moreSettingWidget row:1 column:1];
            moreSettingWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            [currentWidgetView moveWidget:moreCloseWidget row:1 column:1];
            moreCloseWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            
            moreBackWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            
            switch (g_flashContentMode([clientView mobileView]))
            {
                case 0:
                {
                    if (g_isFlashGame([clientView mobileView]))
                    {
                        [currentWidgetView moveWidget:moreHelpWidget row:1 column:1];
                        moreHelpWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
                        [currentWidgetView moveWidget:moreSaveWidget row:1 column:1];
                        moreSaveWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
                    }
                    else
                    {
                        [currentWidgetView moveWidget:moreSaveWidget row:1 column:1];
                        moreSaveWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
                    }
                }
                    break;
                case 1:
                {
                    [currentWidgetView moveWidget:moreReloadWidget row:1 column:1];
                    moreReloadWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
                    [currentWidgetView moveWidget:morePayWidget row:1 column:1];
                    morePayWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
                }
                    break;
            }
            
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(removeMainWidget)];
            [UIView commitAnimations];
            
            [currentWidgetView addWidgets:moreModeWidget row:1 column:1];
            moreModeWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            
            [self switchWidgetPoint:[clientView mouseShowEnable]];
            [currentWidgetView addWidgets:morePointWidget row:1 column:1];
            morePointWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #if SDKNEEDUSERCENTER
            [currentWidgetView addWidgets:moreUserCenterWidget row:1 column:1];
            moreUserCenterWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #endif
            
            void* mobileView = [clientView mobileView];
            if (g_hasFlashButton(mobileView)) {
                [self switchWidgetButton:g_isFlashButtonEnable(mobileView)];
                [currentWidgetView addWidgets:moreButtonWidget row:1 column:1];
                moreButtonWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            }
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            moreModeWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            morePointWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreButtonWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreBackWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            #if SDKNEEDUSERCENTER
            moreUserCenterWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            #endif
            [currentWidgetView moveWidget:moreModeWidget row:1 column:2];
            [currentWidgetView moveWidget:morePointWidget row:0 column:1];
            #if SDKNEEDUSERCENTER
            [currentWidgetView moveWidget:moreUserCenterWidget row:1 column:0];
            #endif
            if (g_hasFlashButton(mobileView)) {
                [self switchWidgetButton:g_isFlashButtonEnable(mobileView)];
                [currentWidgetView moveWidget:moreButtonWidget row:2 column:1];
            }
            
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(setSettingWidgetMode)];
            [UIView commitAnimations];
        }
            break;

    }
    
    if (reAddWidgetView)
    {
        CGRect newFrame = [currentWidgetView frame];
        [self.view addSubview:tmpWidgetView];
        [UIView beginAnimations:nil context:nil];
        [UIView setAnimationDuration:0.2];
        [tmpWidgetView setFrame:newFrame];
        [UIView setAnimationDelegate:self];
        [UIView setAnimationDidStopSelector:@selector(tmpAnimationOver)];
        [UIView commitAnimations];
    }
}

- (void)removeMainWidget
{
    [moreSettingWidget removeFromSuperview];
    [moreCloseWidget removeFromSuperview];
    [moreReloadWidget removeFromSuperview];
    [morePayWidget removeFromSuperview];
    [moreSaveWidget removeFromSuperview];
    [moreHelpWidget removeFromSuperview];
}

- (void)removeSettingWidget
{
    [moreButtonWidget removeFromSuperview];
    [morePointWidget removeFromSuperview];
    [moreModeWidget removeFromSuperview];
    #if SDKNEEDUSERCENTER
    [moreUserCenterWidget removeFromSuperview];
    #endif
}

- (void)setSettingWidgetMode
{
    currentWidgetMode = EWIDGETMODE_SETTING;
}

- (void)changeSettingWidgetMode:(EWIDGETMODE)aMode
{
    currentWidgetMode = aMode;
    
    switch (currentWidgetMode)
    {
        case EWIDGETMODE_SMALL:
        {
            moreSettingWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreCloseWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            
            switch (g_flashContentMode([clientView mobileView]))
            {
                case 0:
                {
                    if (g_isFlashGame([clientView mobileView]))
                    {
                        moreHelpWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
                        moreSaveWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
                    }
                    else
                    {
                        moreSaveWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
                    }
                }
                    break;
                case 1:
                {
                    morePayWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
                    moreReloadWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
                }
                    break;
            }
            currentWidgetView = widgetView;
        }
            break;
        case EWIDGETMODE_GAME:
        {
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            
            [currentWidgetView moveWidget:moreModeWidget row:1 column:1];
            moreModeWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            [currentWidgetView moveWidget:morePointWidget row:1 column:1];
            morePointWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #if SDKNEEDUSERCENTER
            [currentWidgetView moveWidget:moreUserCenterWidget row:1 column:1];
            moreUserCenterWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #endif
            moreBackWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            
            void* mobileView = [clientView mobileView];
            if (g_hasFlashButton(mobileView)) {
                [currentWidgetView moveWidget:moreButtonWidget row:1 column:1];
                moreButtonWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            }
            
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(removeSettingWidget)];
            [UIView commitAnimations];
            
            [currentWidgetView addWidgets:moreSaveWidget row:1 column:1];
            [currentWidgetView addWidgets:moreSettingWidget row:1 column:1];
            [currentWidgetView addWidgets:moreHelpWidget row:1 column:1];
            [currentWidgetView addWidgets:moreCloseWidget row:1 column:1];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            moreSaveWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreSettingWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreHelpWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreCloseWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreBackWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            [currentWidgetView moveWidget:moreSaveWidget row:0 column:1];
            [currentWidgetView moveWidget:moreSettingWidget row:1 column:0];
            [currentWidgetView moveWidget:moreHelpWidget row:1 column:2];
            [currentWidgetView moveWidget:moreCloseWidget row:2 column:1];
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(setGameWidgetMode)];
            [UIView commitAnimations];
        }
            break;
        case EWIDGETMODE_VIDEO:
        {
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            
            [currentWidgetView moveWidget:moreModeWidget row:1 column:1];
            moreModeWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            [currentWidgetView moveWidget:morePointWidget row:1 column:1];
            morePointWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #if SDKNEEDUSERCENTER
            [currentWidgetView moveWidget:moreUserCenterWidget row:1 column:1];
            moreUserCenterWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #endif
            moreBackWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(removeSettingWidget)];
            [UIView commitAnimations];
            
            [currentWidgetView addWidgets:moreSaveWidget row:1 column:1];
            [currentWidgetView addWidgets:moreSettingWidget row:1 column:1];
            [currentWidgetView addWidgets:moreCloseWidget row:1 column:1];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            moreSaveWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreSettingWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreCloseWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreBackWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            [currentWidgetView moveWidget:moreSaveWidget row:0 column:1];
            [currentWidgetView moveWidget:moreSettingWidget row:1 column:0];
            [currentWidgetView moveWidget:moreCloseWidget row:2 column:1];
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(setVideoWidgetMode)];
            [UIView commitAnimations];
        }
            break;
        case EWIDGETMODE_NET:
        {
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            
            [currentWidgetView moveWidget:moreModeWidget row:1 column:1];
            moreModeWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            [currentWidgetView moveWidget:morePointWidget row:1 column:1];
            morePointWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #if SDKNEEDUSERCENTER
            [currentWidgetView moveWidget:moreUserCenterWidget row:1 column:1];
            moreUserCenterWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            #endif
            moreBackWidget.transform = CGAffineTransformMakeScale(0.01, 0.01);
            
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(removeSettingWidget)];
            [UIView commitAnimations];
            
            [currentWidgetView addWidgets:morePayWidget row:1 column:1];
            [currentWidgetView addWidgets:moreSettingWidget row:1 column:1];
            [currentWidgetView addWidgets:moreReloadWidget row:1 column:1];
            [currentWidgetView addWidgets:moreCloseWidget row:1 column:1];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.2];
            morePayWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreSettingWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreReloadWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreCloseWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            moreBackWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            [currentWidgetView moveWidget:morePayWidget row:0 column:1];
            [currentWidgetView moveWidget:moreSettingWidget row:1 column:0];
            [currentWidgetView moveWidget:moreReloadWidget row:1 column:2];
            [currentWidgetView moveWidget:moreCloseWidget row:2 column:1];
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(setNetWidgetMode)];
            [UIView commitAnimations];
        }
            break;
        case EWIDGETMODE_SETTING:
        {
            [settingWidgetView removeAllWidgets];
            moreModeWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            [settingWidgetView addWidget:moreModeWidget row:1 column:2];
            [self switchWidgetPoint:[clientView mouseShowEnable]];
            morePointWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            [settingWidgetView addWidget:morePointWidget row:0 column:1];
            #if SDKNEEDUSERCENTER
            moreUserCenterWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
            [settingWidgetView addWidget:moreUserCenterWidget row:1 column:0];
            #endif
            void* mobileView = [clientView mobileView];
            if (g_hasFlashButton(mobileView))
            {
                [self switchWidgetButton:g_isFlashButtonEnable(mobileView)];
                moreButtonWidget.transform = CGAffineTransformMakeScale(1.0, 1.0);
                [settingWidgetView addWidget:moreButtonWidget row:1 column:0];
            }
            [settingWidgetView addWidget:moreBackWidget row:1 column:1];
            currentWidgetView = settingWidgetView;
        }
    }
}

- (void)setGameWidgetMode
{
    currentWidgetMode = EWIDGETMODE_GAME;
}

- (void)setVideoWidgetMode
{
    currentWidgetMode = EWIDGETMODE_VIDEO;
}

- (void)setNetWidgetMode
{
    currentWidgetMode = EWIDGETMODE_NET;
}



- (void)changeToRotation:(ROTATE_METHOD)rotateMethod
{
    std::swap(g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    CGRect bounds = [self.view bounds];
	float width = bounds.size.width;
	float height = bounds.size.height;
    std::swap(width, height);
    
    self.view.bounds = CGRectMake(0, 0, width, height);
    UIApplication *app = [UIApplication sharedApplication];
    CGAffineTransform at;
    if([[[UIDevice currentDevice] systemVersion] hasPrefix:@"8"])
    {
        if (_currentInterfaceOrientation == UIInterfaceOrientationPortrait)
        {
            _currentInterfaceOrientation = UIInterfaceOrientationLandscapeRight;
            _naviBarInterfaceOrientation = UIInterfaceOrientationLandscapeRight;
        }
        else
        {
            _currentInterfaceOrientation = UIInterfaceOrientationPortrait;
            _naviBarInterfaceOrientation = UIInterfaceOrientationPortrait;
        }
        [UIApplication sharedApplication].keyWindow.rootViewController = nil;
        [UIApplication sharedApplication].keyWindow.rootViewController = self.navigationController;
    }
    else
    {
        if (_currentInterfaceOrientation == _naviBarInterfaceOrientation)
        {
            if (_currentInterfaceOrientation == UIInterfaceOrientationPortrait)
            {
                at = CGAffineTransformMakeRotation(M_PI/2);
                [self.view setTransform:at];
                _currentInterfaceOrientation = UIInterfaceOrientationLandscapeRight;
                //isAutoOrientation = NO;
                [app setStatusBarOrientation: UIInterfaceOrientationLandscapeRight];

            }
            else
            {
                //isAutoOrientation = NO;
                at = CGAffineTransformMakeRotation(-M_PI/2);
                [self.view setTransform:at];
                _currentInterfaceOrientation = UIInterfaceOrientationPortrait;
                //isAutoOrientation = NO;
                [app setStatusBarOrientation: UIInterfaceOrientationPortrait];
            }
        }
        else
        {
            if (_currentInterfaceOrientation == UIInterfaceOrientationPortrait)
            {
                at = CGAffineTransformMakeRotation(0);
                [self.view setTransform:at];
                //isAutoOrientation = NO;
                _currentInterfaceOrientation = UIInterfaceOrientationLandscapeRight;
                //isAutoOrientation = YES;
                [app setStatusBarOrientation: UIInterfaceOrientationLandscapeRight];
            }
            else
            {
                at = CGAffineTransformMakeRotation(0);
                [self.view setTransform:at];
                _currentInterfaceOrientation = UIInterfaceOrientationPortrait;
                //isAutoOrientation = NO;
                [app setStatusBarOrientation: UIInterfaceOrientationPortrait];
            }
        }
    }
    

    [self.clientView setFrame:CGRectMake(0, 0, width, height)];
    [self.clientView layoutSubviews];
    
    
    [self showBar:NO];
    [self showBar:YES];
    [self.glView setFrame:CGRectMake(0, 0, width, height) rotation:true];
//    [self.clientView moveMobileView:CGRectMake(0, 0, width, height)];
    
    // 转屏后widget居中显示
    [moreWidgetView setCenter:CGPointMake(width/2, height/2)];
    [settingWidgetView setCenter:CGPointMake(width/2, height/2)];
    // 转屏后widget居中显示 end
}

#if BWPUSHMODE
- (BOOL)isResignActive
{
    return isResignActive;
}
- (void)setResignActive:(BOOL)resignActive
{
    isResignActive = resignActive;
    g_flashPause(clientView.mobileView, resignActive);
}
#endif

- (void)start
{
    [clientView start];
}

- (void)stop
{
    [clientView stop];
}

- (void)onWebBarHome
{
    if (isWebBViewMode)
    {
        if (!isClientWebViewShow)
        {
            void* mobileView = [self.clientView mobileView];
            if (!g_isFlashStatus(mobileView))
                g_mobileStop(mobileView);
            else
                g_flashExit(mobileView);
        }
        
        if(clientWebView)
        {
            [self.view addSubview: clientWebView];
            isClientWebViewShow = YES;
        }
        [clientWebView onHomeButton];
    }
    else
    {
        void* mobileView = [self.clientView mobileView];
        g_mobileViewConnect(mobileView,HomeAddress_iPhone);
    }
    if(currentMoreBarShowStatus)
        [self showMoreBar:false];
}

- (void)onWebBarLocal
{
    if (!localViewController)
        localViewController = [[web_localViewController_ios alloc] init];
    if (localViewController)
    {
        [localViewController setMobileView:[clientView mobileView]];
        [localViewController initData];
        UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
        UIViewController *rootViewController = [window rootViewController];
        rootViewController.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentPopViewController:localViewController animated:YES completion:nil];
        _naviBarInterfaceOrientation = _currentInterfaceOrientation;
    }

    if(currentMoreBarShowStatus)
        [self showMoreBar:false];
}

- (void)onWebBarBack
{
    if (isWebBViewMode && isClientWebViewShow)
    {
        [clientWebView onBackButton];
        if ([clientWebView canGoBack])
            [webBarBackItem setEnabled:YES];
        else
            [webBarBackItem setEnabled:NO];
    }
    else if (isWebBViewMode && (!isClientWebViewShow))
    {
        [self.view addSubview:clientWebView];
        isClientWebViewShow = YES;
        if ([clientWebView canGoBack])
            [webBarBackItem setEnabled:YES];
        else
            [webBarBackItem setEnabled:NO];
        void* mobileView = [self.clientView mobileView];
        g_mobileBack(mobileView);
    }
    else
    {
        void* mobileView = [self.clientView mobileView];
        g_mobileBack(mobileView);
        if (g_canMobileViewBack(mobileView))
            [webBarBackItem setEnabled:YES];
        else
            [webBarBackItem setEnabled:NO];
    }
    if(currentMoreBarShowStatus)
        [self showMoreBar:false];
}

- (void)onWebBarStop
{
    if (isClientWebViewShow)
    {
        [clientWebView stopLoading];
    }
    else
    {
        void* mobileView = [self.clientView mobileView];
        if (!g_isFlashStatus(mobileView))
            g_mobileStop(mobileView);
        else 
            g_flashExit(mobileView);
    }
    if(currentMoreBarShowStatus)
        [self showMoreBar:false];
}

- (void)onWebBarRefresh
{
    if (isClientWebViewShow)
        [clientWebView onRefreshButton];
    else
    {
        void* mobileView = [self.clientView mobileView];
        g_mobileRefresh(mobileView);
    }
    if(currentMoreBarShowStatus)
        [self showMoreBar:false];
}

- (void)onWebBarMore
{
    [self showMoreBar:!currentMoreBarShowStatus];
}

- (void)onWebMoreBarPoint
{
    [self.clientView setMouseShowEnable:![self.clientView mouseShowEnable]];
    clientWebView.mouseShowEnable = !clientWebView.mouseShowEnable;
    [self switchWebMoreBarPoint:[self.clientView mouseShowEnable]];
}

- (void)onWebMoreBarFeedback
{
#if BWTHIRDPARTSDKISON
    [[ShareSDKInstance sharedSDK] showFeedBackView];
#else
    void* mobileView = [self.clientView mobileView];
    g_mobileViewConnect(mobileView, FeedBack_iPhone);
#endif  
    [self hideMoreBar];
}

- (void)onWebMoreBarCleanCache
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"是否清理缓存？" delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alertView setTag:EALERTVIEWMODE_CLEANCACHE];
    [alertView show];
    [alertView release];
    
    [self hideMoreBar];
}

- (void)onWebMoreBarHelp
{
    webHelpAboutViewController_ios* helpViewController = [[[webHelpAboutViewController_ios alloc] initWithStyle: WEB_PAGE_TYPE_HELP] autorelease];
    if (g_deviceData.m_isPad)
    {
        UIPopoverController* popoverController = [[UIPopoverController alloc] initWithContentViewController:helpViewController];
        [popoverController setPopoverContentSize:helpViewController.view.bounds.size];
        [popoverController setDelegate:self];
        CGRect popoverFrame = [self.view convertRect:[webMoreBarAboutItem frame] fromView:webMoreBarView];
        [popoverController presentPopoverFromRect:popoverFrame inView:self.view permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
    }
    else
    {
        UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
        UIViewController *rootViewController = [window rootViewController];
        rootViewController.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentPopViewController:helpViewController animated:YES completion:nil];
        _naviBarInterfaceOrientation = _currentInterfaceOrientation;
    }
}

- (void)onWebMoreBarAbout
{
    webHelpAboutViewController_ios* aboutViewController = [[[webHelpAboutViewController_ios alloc] initWithStyle: WEB_PAGE_TYPE_ABOUT] autorelease];
    if (g_deviceData.m_isPad)
    {
        UIPopoverController* popoverController = [[UIPopoverController alloc] initWithContentViewController:aboutViewController];
        [popoverController setPopoverContentSize:aboutViewController.view.bounds.size];
        [popoverController setDelegate:self];
        CGRect popoverFrame = [self.view convertRect:[webMoreBarAboutItem frame] fromView:webBarView];
        [popoverController presentPopoverFromRect:popoverFrame inView:self.view permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
    }
    else
    {
        UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
        UIViewController *rootViewController = [window rootViewController];
        rootViewController.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentPopViewController:aboutViewController animated:YES completion:nil];
        _naviBarInterfaceOrientation = _currentInterfaceOrientation;
    }
}

- (void)switchWebMoreBarPoint:(BOOL)aShowEnable
{
    if (aShowEnable)
    {
        [webMoreBarPointItem setImage:[UIImage imageNamed:@"pointOn.png"] forState:UIControlStateNormal];
        [webMoreBarPointItem setImage:[UIImage imageNamed:@"pointOn_touch.png"] forState:UIControlStateHighlighted];
    }
    else 
    {
        [webMoreBarPointItem setImage:[UIImage imageNamed:@"pointOff.png"] forState:UIControlStateNormal];
        [webMoreBarPointItem setImage:[UIImage imageNamed:@"pointOff_touch.png"] forState:UIControlStateHighlighted];
    }
}

- (void)onGameBarMode
{
    currentMode = EMODE_WIDGET;
    [self changeWidgetMode:EWIDGETMODE_SMALL];
    [self showBar:NO];
    [self showWidget:YES];
}

- (void)onGameBarSetting
{
    [self changeSettingWidgetMode:EWIDGETMODE_SETTING];
    [self showWidget:YES];
    [self showBar:NO];
}

- (void)onGameBarSave
{
    if (!isSaving)
    {
        g_flashSave([clientView mobileView]);
        isSaving = YES;
    }
}

- (void)onGameBarZoom
{
    return;
    g_flashPointZoomIn([clientView mobileView]);
}

- (void)onGameBarHelp
{
    [self showBar:NO];
    g_flashSwitchHelp([clientView mobileView]);
}

- (void)onGameBarClose
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"确认退出？" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alertView setTag:EALERTVIEWMODE_EXITFLASH];
    [alertView show];
    [alertView release];
}

- (void)onVideoBarMode
{
    currentMode = EMODE_WIDGET;
    [self changeWidgetMode:EWIDGETMODE_SMALL];
    [self showBar:NO];
    [self showWidget:YES];
}

- (void)onVideoBarSetting
{
    [self changeSettingWidgetMode:EWIDGETMODE_SETTING];
    [self showWidget:YES];
    [self showBar:NO];
}

- (void)onVideoBarSave
{
    if (!isSaving)
    {
        g_flashSave([clientView mobileView]);
        isSaving = YES;
    }
    
}

- (void)onVideoBarZoom
{
    return;
    g_flashPointZoomIn([clientView mobileView]);
}

- (void)onVideoBarClose
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"确认退出？" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alertView setTag:EALERTVIEWMODE_EXITFLASH];
    [alertView show];
    [alertView release];
}

- (void)onNetBarMode
{
    currentMode = EMODE_WIDGET;
    [self changeWidgetMode:EWIDGETMODE_SMALL];
    [self showBar:NO];
    [self showWidget:YES];
}

- (void)onNetBarSetting
{
    [self changeSettingWidgetMode:EWIDGETMODE_SETTING];
    [self showWidget:YES];
    [self showBar:NO];
}

- (void)onNetBarZoom
{
    g_flashPointZoomIn([clientView mobileView]);
}

- (void)onNetBarRefresh
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"确认刷新？" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alertView setTag:EALERTVIEWMODE_EXITFLASH];
    [alertView show];
    [alertView release];
}

- (void)onNetBarPay
{
    if ([[self.payUrl substringToIndex:7] isEqualToString:@"webv://"])
    {
        NSMutableString * mutablestring = [[NSMutableString alloc]initWithString:@"http://"];
        [mutablestring appendString:[self.payUrl substringFromIndex:7]];
        [self openWebViewWithUrl:mutablestring withIsWebB:NO];
        [mutablestring release];
    }
    else
    {
        void* mobileView = [self.clientView mobileView];
        g_flashPay(mobileView);
    }
    //test used
    //测试内购的界面
    //[self openWebViewWithUrl:@"http://bwtest.jqbar.com/newphone/appleiap/PhoneAppleCharge.aspx?userid=2323117&serverid=405&appcode=BWNBCS_2_9_47" withIsWebB:NO];
    //[self openWebViewWithUrl:@"http://512*512/www.baidu.com"];
    //[self presentOfferWall:@"http://BW_DOMOB_OFFERWALL"];
    //[self presentOfferWall:@"http://BW_LiMei_OFFERWALL"];
    //[self presentOfferWall:@"http://BW_YouMi_OFFERWALL"];
    //[self presentOfferWall:@"http://BW_Mobisage_OFFERWALL"];
}

- (void)onNetBarClose
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"确认退出？" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alertView setTag:EALERTVIEWMODE_EXITFLASH];
    [alertView show];
    [alertView release];
}

- (void)onWidgetMore
{
    switch (g_flashContentMode([clientView mobileView])) 
    {
        case 0:
        {
            if (g_isFlashGame([clientView mobileView]))
                [self changeWidgetMode:EWIDGETMODE_GAME];
            else
                [self changeWidgetMode:EWIDGETMODE_VIDEO];
        }
            break;
        case 1:
        {
            [self changeWidgetMode:EWIDGETMODE_NET];
        }
            break;
    }
}

- (void)onWidgetZoom
{
    if((xgg_contentMode([clientView mobileView])==2)||(xgg_contentMode([clientView mobileView])==3))
        return;
    g_flashPointZoomIn([clientView mobileView]);
}

- (void)onWidgetSave
{
    [self changeWidgetMode:EWIDGETMODE_SMALL];
    if (!isSaving)
    {
        g_flashSave([clientView mobileView]);
        isSaving = YES;
    }
}

- (void)onWidgetSetting
{
    [self changeWidgetMode:EWIDGETMODE_SETTING];
}

- (void)onWidgetHelp
{
    [self showWidget:NO];
    [self changeWidgetMode:EWIDGETMODE_SMALL];
    g_flashSwitchHelp([clientView mobileView]);
}

- (void)onWidgetBack
{
    if (currentWidgetMode==EWIDGETMODE_SETTING)
    {
        if (currentMode==EMODE_BAR)
        {
            [self showBar:YES];
            [self showWidget:NO];
        }
        else if (currentMode==EMODE_WIDGET)
        {
            switch (g_flashContentMode([clientView mobileView])) 
            {
                case 0:
                {
                    if (g_isFlashGame([clientView mobileView]))
                        [self changeSettingWidgetMode:EWIDGETMODE_GAME];
                    else
                        [self changeSettingWidgetMode:EWIDGETMODE_VIDEO];
                }
                    break;
                case 1:
                {
                    [self changeSettingWidgetMode:EWIDGETMODE_NET];
                }
                    break;
            }
        }
    }
    else if (currentWidgetMode==EWIDGETMODE_GAME)
        [self changeWidgetMode:EWIDGETMODE_SMALL];
    else if (currentWidgetMode==EWIDGETMODE_VIDEO)
        [self changeWidgetMode:EWIDGETMODE_SMALL];
    else if (currentWidgetMode==EWIDGETMODE_NET)
        [self changeWidgetMode:EWIDGETMODE_SMALL];
}

- (void)onWidgetPay
{
    if ([[self.payUrl substringToIndex:7] isEqualToString:@"webv://"])
    {
        NSMutableString * mutablestring = [[NSMutableString alloc]initWithString:@"http://"];
        [mutablestring appendString:[self.payUrl substringFromIndex:7]];
        [self openWebViewWithUrl:mutablestring withIsWebB:NO];
        [mutablestring release];
    }
    else
    {
        g_flashPay([clientView mobileView]);
    }
}

- (void)onWidgetRefresh
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"确认刷新？" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alertView setTag:EALERTVIEWMODE_REFRESH];
    [alertView show];
    [alertView release];
}

- (void)onWidgetPoint
{
    [self.clientView setMouseShowEnable:![clientView mouseShowEnable]];
    clientWebView.mouseShowEnable = !clientWebView.mouseShowEnable;
    [self switchWidgetPoint:[clientView mouseShowEnable]];
}

- (void)onWidgetButton
{
    void* mobileView = [clientView mobileView];
    g_setFlashButtonEnable(mobileView,!g_isFlashButtonEnable(mobileView));
    [self switchWidgetButton:g_isFlashButtonEnable(mobileView)];
}

- (void)onWidgetMode
{
    switch (currentMode)
    {
        case EMODE_BAR:
        {
            currentMode = EMODE_WIDGET;
            [self changeWidgetMode:EWIDGETMODE_SMALL];
            [self showBar:NO];
            [self showWidget:YES];
        }
            break;
        case EMODE_WIDGET:
        {
            currentMode = EMODE_BAR;
            [self showBar:YES];
            [self showWidget:NO];
            [self changeSettingWidgetMode:EWIDGETMODE_SMALL];
        }
            break;
    }
}

- (void)onWidgetClose
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"确认退出？" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alertView setTag:EALERTVIEWMODE_EXITFLASH];
    [alertView show];
    [alertView release];
}

- (void)switchWidgetPoint:(BOOL)aShowEnable
{
    if (aShowEnable)
    {
        [morePointWidget setImage:[UIImage imageNamed:@"pointPlayOn.png"] forState:UIControlStateNormal];
        [morePointWidget setImage:[UIImage imageNamed:@"pointPlayOn_touch.png"] forState:UIControlStateHighlighted];
    }
    else 
    {
        [morePointWidget setImage:[UIImage imageNamed:@"pointPlayOff.png"] forState:UIControlStateNormal];
        [morePointWidget setImage:[UIImage imageNamed:@"pointPlayOff_touch.png"] forState:UIControlStateHighlighted];
    }
//    if (aShowEnable)
//    {
//        [morePointWidget setImage:[UIImage imageNamed:@"buttonPlayOn.png"] forState:UIControlStateNormal];
//        [morePointWidget setImage:[UIImage imageNamed:@"buttonPlayOn_touch.png"] forState:UIControlStateHighlighted];
//    }
//    else
//    {
//        [morePointWidget setImage:[UIImage imageNamed:@"buttonPlayOff.png"] forState:UIControlStateNormal];
//        [morePointWidget setImage:[UIImage imageNamed:@"buttonPlayOff_touch.png"] forState:UIControlStateHighlighted];
//    }
}

- (void)switchWidgetButton:(BOOL)aShowEnable
{
    if (aShowEnable)
    {
        [moreButtonWidget setImage:[UIImage imageNamed:@"buttonPlayOn.png"] forState:UIControlStateNormal];
        [moreButtonWidget setImage:[UIImage imageNamed:@"buttonPlayOn_touch.png"] forState:UIControlStateHighlighted];
    }
    else 
    {
        [moreButtonWidget setImage:[UIImage imageNamed:@"buttonPlayOff.png"] forState:UIControlStateNormal];
        [moreButtonWidget setImage:[UIImage imageNamed:@"buttonPlayOff_touch.png"] forState:UIControlStateHighlighted]; 
    }
}

- (void)tmpAnimationOver
{
    [tmpWidgetView removeFromSuperview];
    [self showWidget:YES];
}

- (void)waitLoading
{
    [self showBar:YES];
}
#if BWONSALEMODE
- (void)handleOnSaleModeTimer
{
    onSaleModeTimer = nil;
    if (!onSaleModeEnable)
    {
        showBarModeEnable = NO;
        [self playLocalFile];
    }
    else
    {
        if (showBoutiqueGame && ([self isFileExist:@"game.dat"]))
        {
            showBarModeEnable = NO;
            [self playBoutiqueGame];
        }
        else
        {
            
            if (g_deviceData.m_isPad)
            {
                g_deviceData.mbOnSale = true;
                [clientView SetNormalScale];
            }
            g_deviceData.m_isWebGame = true;
            g_deviceData.m_isDomView = true;
            g_deviceData.m_isDomExtView = false;

        }
    }
    [self showBar:YES];
}
#endif


- (void)playBoutiqueGame
{
     g_mobileViewConnect(clientView.mobileView, "C:/game.dat");
}


- (void)playLocalFile
{
	[self showADStart];
    g_mobileViewConnect(clientView.mobileView, "C:/game.dat");
}
- (void)layoutAnimated:(BOOL)animated
{
    // As of iOS 6.0, the banner will automatically resize itself based on its width.
    // To support iOS 5.0 however, we continue to set the currentContentSizeIdentifier appropriately.
    CGRect contentFrame = self.view.bounds;
    if (contentFrame.size.width < contentFrame.size.height) {
        _bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
    } else {
        _bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierLandscape;
    }
    
    CGRect bannerFrame = _bannerView.frame;
    if (_bannerView.bannerLoaded) {
        contentFrame.size.height -= _bannerView.frame.size.height;
        bannerFrame.origin.y = contentFrame.size.height;
    } else {
        bannerFrame.origin.y = contentFrame.size.height;
    }
    
    [UIView animateWithDuration:animated ? 0.25 : 0.0 animations:^{
        _bannerView.frame = bannerFrame;
    }];
}

- (void)bannerViewDidLoadAd:(ADBannerView *)banner
{
    [self layoutAnimated:YES];
    [NSTimer scheduledTimerWithTimeInterval:60 target:self selector:@selector(hideBannerView) userInfo:nil repeats:NO];
}
-(void)hideBannerView
{
    CGRect contentFrame = self.view.bounds;
    if (contentFrame.size.width < contentFrame.size.height) {
        _bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
    } else {
        _bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierLandscape;
    }
    
    CGRect bannerFrame = _bannerView.frame;
    bannerFrame.origin.y = contentFrame.size.height;
    
    [UIView animateWithDuration:0.25 animations:^{
        _bannerView.frame = bannerFrame;
    }];
}
- (void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    [self layoutAnimated:YES];
}


#if BWIAPMODE
- (void)showCurrentStep
{
    [self.view showHUDWithText:@"服务器处理中" Type:ShowLoading WithCenter:CGPointMake(self.view.bounds.size.width/2, self.view.bounds.size.height/2) Enabled:YES];
}

- (void)hideCurrentStep
{
    [self.view showHUDWithText:@"服务器处理中" Type:ShowDismiss WithCenter:CGPointMake(self.view.bounds.size.width/2, self.view.bounds.size.height/2) Enabled:YES];
}

//- (void)setCurrentStepTitle:(NSString*)title
//{
//    if (IAPCurrentStepLabel)
//    {
//        [IAPCurrentStepLabel setText:title];
//    }
//}

- (void)saveIAPData
{
    NSDictionary* BWDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSMutableDictionary* dictionary = [NSMutableDictionary dictionaryWithDictionary:BWDictionary];
    
    [dictionary setValue:[NSNumber numberWithInt:IAPStepID] forKey:BWIAPKEYSTEPID];
    [dictionary setValue:self.IAPProductID forKey:BWIAPKEYPRODUCTID];
    [dictionary setValue:self.IAPOrderID forKey:BWIAPKEYORDERID];
    [dictionary setValue:[NSNumber numberWithInt:IAPCodeID] forKey:BWIAPKEYCODEID];
    [dictionary setValue:self.IAPDestription forKey:BWIAPKEYDESCRIPTION];
    [dictionary setValue:self.IAPChargeData forKey:BWIAPKEYCHARGEDATA];
    [dictionary setValue:self.IAPChargeUrlData forKey:BWIAPKEYCHARGEURLDATA];
    
    [[NSUserDefaults standardUserDefaults] setObject:dictionary forKey:BWUSERDEFAULTKEY];
    [[NSUserDefaults standardUserDefaults] synchronize];
    
}

- (void)loadIAPData
{
    NSDictionary* BWDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSMutableDictionary* dictionary = [NSMutableDictionary dictionaryWithDictionary:BWDictionary];
    
    
    IAPStepID = [[dictionary valueForKey:BWIAPKEYSTEPID] intValue];
    if ([dictionary valueForKey:BWIAPKEYPRODUCTID])
        self.IAPProductID = [[[NSString alloc] initWithString:[dictionary valueForKey:BWIAPKEYPRODUCTID]]autorelease];
    if ([dictionary valueForKey:BWIAPKEYORDERID])
        self.IAPOrderID = [[[NSString alloc] initWithString:[dictionary valueForKey:BWIAPKEYORDERID]]autorelease];
    IAPCodeID = [[dictionary valueForKey:BWIAPKEYCODEID] intValue];
    if ([dictionary valueForKey:BWIAPKEYDESCRIPTION])
        self.IAPDestription = [[[NSString alloc] initWithString:[dictionary valueForKey:BWIAPKEYDESCRIPTION]]autorelease];
    if ([dictionary valueForKey:BWIAPKEYCHARGEDATA])
        self.IAPChargeData = [[[NSString alloc] initWithString:[dictionary valueForKey:BWIAPKEYCHARGEDATA]]autorelease];
    if ([dictionary valueForKey:BWIAPKEYCHARGEURLDATA])
        self.IAPChargeUrlData = [[[NSString alloc] initWithString:[dictionary valueForKey:BWIAPKEYCHARGEURLDATA]]autorelease];
}

- (void)checkStart
{
    //内购
    IAPStepID = BWIAPSTATUSCHECKSTART;
    //[self saveIAPData];
    APPLECANCEL(productRequest);
    NSArray* productIDArray = [[NSArray alloc] initWithObjects:self.IAPProductID, nil];
    NSSet* productIDSet = [NSSet setWithArray:productIDArray];
    productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIDSet];
    [productRequest setDelegate:self];
    APPLERELEASE(productIDArray);
    [productRequest start];
    IAPTestNum = 0;
}

- (void)startFeedBack
{
    if (self.IAPFeedBackTimer)
        BWINVALIDATE(self.IAPFeedBackTimer);
   
    self.IAPFeedBackTimer = [NSTimer scheduledTimerWithTimeInterval:60 target:self selector:@selector(feedBack) userInfo:nil repeats:YES];
    [self hideCurrentStep];
    [self feedBack];
}

- (void)feedBack
{
    IAPStepID = BWIAPSTATUSFEEDBACKSTART;
    [self saveIAPData];
    g_iosPidPost([clientView mobileView], [self.IAPOrderID cStringUsingEncoding:NSUTF8StringEncoding],
                 [[NSString stringWithFormat:@"%d", IAPCodeID] cStringUsingEncoding:NSUTF8StringEncoding],
                 [self.IAPDestription cStringUsingEncoding:NSUTF8StringEncoding],
                 [self.IAPChargeData cStringUsingEncoding:NSUTF8StringEncoding],
                 [self.IAPChargeUrlData cStringUsingEncoding:NSUTF8StringEncoding]);
//    self.IAPDestription = @"none";
//    //    self.IAPfeedBackString = [NSString stringWithFormat:@"&price=%@&transaction_id=%@&%@", self.IAPPrice, self.IAPTransactionIdentifier,self.IAPFeedBackParameter];
//    self.IAPfeedBackString = [NSString stringWithFormat:@"orderid=%@&status=%d&description=%@&price=%@&transaction_id=%@&%@&chargedata=%@", self.IAPOrderID, IAPCodeID,self.IAPDestription,self.IAPPrice,self.IAPTransactionIdentifier, self.IAPFeedBackParameter,self.IAPChargeData];
//    NSData *bodyData = [[self.IAPfeedBackString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]dataUsingEncoding:NSUTF8StringEncoding];//把bodyString转换为NSData数据
//    NSURL *serverUrl = [NSURL URLWithString:@"http://bwtest.jqbar.com/AppleIAP/chargeUrl.aspx"];//获取到服务器的url地址
//    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:serverUrl
//                                                           cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
//                                                       timeoutInterval:10];//请求这个地址， timeoutInterval:10 设置为10s超时：请求时间超过10s会被认为连接不上，连接超时
//    
//    [request setHTTPMethod:@"POST"];//POST请求
//    [request setHTTPBody:bodyData];//body 数据
//    [request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"content-type"];//请求头
//    
//    dispatch_async(dispatch_get_global_queue(0, 0), ^{
//        // something
//        NSData *returnData = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
//        NSString *str = [[NSString alloc]initWithData:returnData encoding:NSUTF8StringEncoding];
//        if (![str isEqualToString:[NSString stringWithFormat:@"iapstatus=%@", self.IAPTransactionIdentifier]])
//        {
//            IAPStepID = BWIAPSTATUSFEEDBACKFINISH;
//            [self.IAPFeedBackTimer invalidate];
//            self.IAPFeedBackTimer = nil;
//            [self saveIAPData];
            IAPStepID = BWIAPSTATUSREADY;
//        }
//    });
}

//- (void)finishFeedBack
//{
//    if (IAPFeedBackTimer)
//        BWINVALIDATE(IAPFeedBackTimer);
//}
#endif

#pragma mark - UIAlertView Delegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    switch ([alertView tag])
    {
        case EALERTVIEWMODE_CLEANCACHE:
        {
            if (buttonIndex==[alertView firstOtherButtonIndex])
            {
                int num = g_mobileViewCleanCache([clientView mobileView]);
                NSString* msg = [[NSString alloc] initWithFormat:@"共清理%d个文件", num];
                
                HDTipsView* tipsView = [[HDTipsView alloc] initWithText:msg];
                [tipsView setAutoHide:YES];
                [tipsView showInView:self.view];
                [tipsView release];
                [msg release];
            }
        }
            break;
#if BWNEWAPPMODE
        case EALERTVIEWMODE_UPDATE:
        {
            if (buttonIndex==[alertView firstOtherButtonIndex])
            {
                UIApplication* app = [UIApplication sharedApplication];
                [app openURL:[NSURL URLWithString:newAppUrl]];
                exit(0);
            }
        }
            break;
#endif
        case EALERTVIEWMODE_OPENSAFARI:
        {
            if (buttonIndex==[alertView firstOtherButtonIndex])
            {
                [[UIApplication sharedApplication] openURL:launchUrl];
            }
        }
            break;
        case EALERTVIEWMODE_REFRESH:
        {
            if (buttonIndex==[alertView firstOtherButtonIndex])
            {
 //               self.isNetRefreshing = TRUE;
                g_flashRestart([clientView mobileView]);
            }
        }
            break;
#if BWTHIRDPARTSDKISON
        case EALERTVIEWMODE_THIRDPARTSDK:
        {
            if (buttonIndex==[alertView firstOtherButtonIndex])
                [[ShareSDKInstance  sharedSDK] login];
            
        }
            break;
#endif
        default:
            break;
    }
}
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    switch ([alertView tag])
    {
        case EALERTVIEWMODE_EXITFLASH:
        {
            if (buttonIndex==[alertView firstOtherButtonIndex])
            {
                g_flashExit([clientView mobileView]);
            }
        }
            break;
        default:
            break;
    }
}

#pragma mark - UIPopoverController Delegate 

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{
    [popoverController release];
}

#pragma mark - HDToolBarViewDelegate

- (void)moveupFromBottom
{
    currentMode = EMODE_WIDGET;
    [self showBar:NO];
    [self showWidget:YES];
}

#pragma mark - HDWidgetViewDelegate

- (void)moveDownToBottom
{
    currentMode = EMODE_BAR;
    [self showBar:YES];
    [self showWidget:NO];
}

#if BWIAPMODE
#pragma mark - SKProductRequestDelegate
-(void)requestProductData
{
    return;
}
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    [self hideCurrentStep];
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"获取商品信息失败" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
    [alertView show];
    [alertView release];
    IAPStepID = BWIAPSTATUSREADY;
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    [self hideCurrentStep];
    NSArray* validProducts = response.products;
    for(SKProduct *product in validProducts){
        BWLOG(@"product info-->");
        BWLOG(@"SKProduct 描述信息%@", [product description]);
        BWLOG(@"产品标题 %@" , product.localizedTitle);
        BWLOG(@"产品描述信息: %@" , product.localizedDescription);
        BWLOG(@"价格: %@" , product.price);
        BWLOG(@"Product id: %@" , product.productIdentifier);
    }
    if (IAPStepID!=BWIAPSTATUSCHECKSTART)
        return;
    
    if (validProducts&&validProducts.count)
    {
        IAPTestNum = 0;
        SKPayment* payMent = [SKPayment paymentWithProduct:[validProducts objectAtIndex:0]];
        if (![[payMent productIdentifier] isEqualToString:self.IAPProductID])
        {
            UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"获取商品信息失败" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
            [alertView release];
            IAPStepID = BWIAPSTATUSREADY;
            return;
        }
        
        SKPaymentQueue* payQueue = [SKPaymentQueue defaultQueue];
        [payQueue addTransactionObserver:self];
        
        if (![SKPaymentQueue canMakePayments])
        {
            UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"设备不支持内购" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
            [alertView release];
            IAPCodeID = BWIAPERRORUNSUPPORTED;
            self.IAPDestription = [[NSString alloc] initWithString:@"Unsupported IAP"];
            IAPStepID = BWIAPSTATUSREADY;
        }
        else
        {
            IAPStepID = BWIAPSTATUSCHARGESTART;
            //[self saveIAPData];
            //[self showCurrentStep];
            self.IAPPrice = [NSString stringWithFormat:@"%@",((SKProduct *)[[response products] objectAtIndex:0]).price];
            [payQueue addPayment:payMent];
        }
    }
    else
    {
        //NSMutableString  msg = NULL;
        NSArray* invalid = [response invalidProductIdentifiers];
        if (invalid&&invalid.count) {
            BWLOG(@"invalid::::%@",invalid);
            if(IAPTestNum < 10){
                IAPStepID = BWIAPSTATUSCHECKSTART;
                APPLECANCEL(productRequest);
                NSArray* productIDArray = [[NSArray alloc] initWithObjects:self.IAPProductID, nil];
                NSSet* productIDSet = [NSSet setWithArray:productIDArray];
                productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIDSet];
                [productRequest setDelegate:self];
                APPLERELEASE(productIDArray);
                [productRequest start];
                IAPTestNum++;
                NSLog(@"IAPTestNum = %d",IAPTestNum);
                return;
                }
        }
        UIAlertView* alertView = nil;
        if ([BaiWanViewController_ios isJailBroken])
        {
            alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"越狱ios设备不支持内购" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        }
        else
        {
            alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"获取商品信息失败,请重试！" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        }
        [alertView show];
        [alertView release];
        IAPStepID = BWIAPSTATUSCHECKFAILED;
        IAPStepID = BWIAPSTATUSREADY;
    }
}

#pragma mark - SKPaymentTransactionObserver
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction* transaction in transactions)
    {
        switch ([transaction transactionState]) 
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self completeTransaction:transaction];
                break;
                
            default:
                break;
        }
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{    
    IAPStepID = BWIAPSTATUSCHARGEFINISH;
    IAPCodeID = [[transaction error] code];
    self.IAPTransactionIdentifier = transaction.transactionIdentifier;
    if ([[transaction error] localizedDescription])
        self.IAPDestription = [[NSString alloc] initWithString:[[transaction error] localizedDescription]];
    if ([GTMBase64 stringByEncodingData:[transaction transactionReceipt]])
        self.IAPChargeData = [[NSString alloc] initWithString:[GTMBase64 stringByEncodingData:[transaction transactionReceipt]]];
    [self saveIAPData];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//    [self feedBack];
    [self startFeedBack];
    IAPStepID = BWIAPSTATUSREADY;
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    IAPStepID = BWIAPSTATUSCHARGEFINISH;
    IAPCodeID = [[transaction error] code];
    if ([[transaction error] localizedDescription])
        self.IAPDestription = [[NSString alloc] initWithString:[[transaction error] localizedDescription]];
    if ([GTMBase64 stringByEncodingData:[transaction transactionReceipt]])
        self.IAPChargeData = [[NSString alloc] initWithString:[GTMBase64 stringByEncodingData:[transaction transactionReceipt]]];
    [self saveIAPData];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//    [self feedBack];
    [self startFeedBack];
    IAPStepID = BWIAPSTATUSREADY;
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    IAPStepID = BWIAPSTATUSCHARGEFAILED;
    IAPCodeID = [[transaction error] code];
    //NSLog(@"code id:%d", IAPCodeID);
    if ([[transaction error] localizedDescription])
        self.IAPDestription = [[NSString alloc] initWithString:[[transaction error] localizedDescription]];
//    if ([GTMBase64 stringByEncodingData:[transaction transactionReceipt]])
//        self.IAPChargeData = [[NSString alloc] initWithString:[GTMBase64 stringByEncodingData:[transaction transactionReceipt]]];
//    [self saveIAPData];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//    [self feedBack];
//    [self startFeedBack];
    if (IAPCodeID == SKErrorPaymentCancelled)
    {
        IAPStepID = BWIAPSTATUSREADY;
        return;
    }
    //NSLog(@"code id:%d", IAPCodeID);
//    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"购买失败! 请重试" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
//    [alertView show];
//    [alertView release];
    NSString* msg = [[NSString alloc] initWithFormat:@"购买失败! 请重试"];
    HDTipsView* tipsView = [[HDTipsView alloc] initWithText:msg];
    [tipsView setAutoHide:YES];
    [tipsView showInView: payViewControllor.view];
    [tipsView release];
    [msg release];
    IAPStepID = BWIAPSTATUSREADY;
}
#endif
//更改工具条
- (void) xgg_changeBar
{
    if (currentMode==EMODE_BAR)
    {
        switch (xgg_contentMode([clientView mobileView]))
        {
            case 0:
                break;
            case 1:
                [self changeBarMode:EBARMODE_NET];
                break;
            case 2:
                [self changeBarMode:EBARMODE_GAME];
                break;
            case 3:
                [self changeBarMode:EBARMODE_VIDEO];
                break;
            default:
                break;
        }
    }
    else if (currentMode==EMODE_WIDGET)
    {
        [self showBar:NO];
        [self changeWidgetMode:EWIDGETMODE_SMALL];
        [self showWidget:YES];
    }
}

- (void)NoticeShowHideMenu:(bool)bShow
{
    if (bShow )
    {
        [self onWidgetMore];
    }
    else
    {
        [self onWidgetBack];
    }
}

-(void)noticeThirdPartPid:(unsigned int)value
{
    char* pidChar = (char*)value;
    if (!pidChar)
        return;
    NSString* pidString = [[NSString alloc] initWithCString:pidChar encoding:NSUTF8StringEncoding];
    NSArray* pidArray = [pidString componentsSeparatedByString:@"|"];
    if ([[pidArray lastObject] isEqualToString:@"ZFBSDKPAY"])
    {
        if ([pidArray count]!=7)
        {
            [pidString release];
            return;
        }
        
        NSString *appScheme = @"AlixPay2088701209894192";
        AlixPayOrder *order = [[AlixPayOrder alloc] init];
        order.partner = [pidArray objectAtIndex:4];
        order.seller = [pidArray objectAtIndex:5];
        order.tradeNO = [pidArray objectAtIndex:0]; //订单ID（由商家自行制定）
        order.productName = [pidArray objectAtIndex:1]; //商品标题
        order.productDescription = [pidArray objectAtIndex:1]; //商品描述
        order.amount = [NSString stringWithFormat:@"%.2f",[[pidArray objectAtIndex:2] doubleValue]]; //商品价格
        order.notifyURL =  [pidArray objectAtIndex:3]; //回调URL
        NSString* orderInfo = [order description];
        id<DataSigner> signer;
        signer = CreateRSADataSigner(PartnerPrivKey);
        NSString *signedStr = [signer signString:orderInfo];
        
        NSString *orderString = [NSString stringWithFormat:@"%@&sign=\"%@\"&sign_type=\"%@\"",
                                 orderInfo, signedStr, @"RSA"];
        [AlixLibService payOrder:orderString AndScheme:appScheme seletor:@selector(paymentResult:) target:self];
        AlixViewController *vc = [[AlixViewController alloc] init];
        UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
        UIViewController *rootViewController = [window rootViewController];
        rootViewController.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentPopViewController:vc animated:YES completion:^{}];
        [vc release];
    }
    else if ([[pidArray lastObject] isEqualToString:@"IAPSDKPAY"])
    {
        if ([pidArray count]!=4)
        {
            [pidString release];
            UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"网络连接错误，请重试！" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
            [alertView release];
            return;
        }
        [self noticeIosPid:pidString];
    }
    else
    {
#if BWTHIRDPARTSDKISON
        [[ShareSDKInstance sharedSDK] pay:value];
#endif
    }
}
//#ifdef BWALIPAYSDKMODE
//wap回调函数
-(void)paymentResult:(NSString *)resultd
{
    //结果处理
    AlixPayResult* result = [[AlixPayResult alloc] initWithString:resultd];
    
	if (result)
    {
		
		if (result.statusCode == 9000)
        {
			/*
			 *用公钥验证签名 严格验证请使用result.resultString与result.signString验签
			 */
            
            //交易成功
            NSString* key = AlipayPubKey;//签约帐户后获取到的支付宝公钥
			id<DataVerifier> verifier;
            verifier = CreateRSADataVerifier(key);
            
			if ([verifier verifyString:result.resultString withSign:result.signString])
            {
                //验证签名成功，交易结果无篡改
			}
        }
        else
        {
            //交易失败
        }
    }
    else
    {
        //失败
    }
    
}
//#endif
# pragma mark - Third Part SDK Call Back Delegate Method
-(void) logoff
{
    currentMode = EMODE_WIDGET;
    g_flashExit([clientView mobileView]);
}

-(void) loginSuccess:(NSString *)productURL
{
    g_mobileViewConnect(clientView.mobileView, [productURL cStringUsingEncoding:NSUTF8StringEncoding]);
}

-(void)setCurrentBarIsValid:(BOOL)isValid
{
    self.clientView.userInteractionEnabled = isValid;
    webBarView.userInteractionEnabled = isValid;
    webMoreBarView.userInteractionEnabled = isValid;
}
#ifdef BWWXSDKMODE

-(void)g_noticeWeiXinSendSession:(unsigned int)value
{

    if (!g_WeiXinShareSessionSavedData)
        return;
    XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
    if (value == XWM_WXSENDTEXT)
    {
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
//        req.text = [NSString stringWithUTF8String:(char *)g_WeiXinShareSessionSavedData->parameter1];
        req.text = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
        //        char *send = (char *)malloc(6);
        //        *send = 230;
        //        *(send + 1) = 155;
        //        *(send + 2) = 185;
        //        *(send + 3) = 230;
        //        *(send + 4) = 147;
        //        *(send + 5) = 141;
        //        req.text = [NSString stringWithUTF8String:send];
        req.bText = YES;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
    }
    else if (value == XWM_WXSENDPHOTO)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        // make data provider with data.
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter1, g_WeiXinShareSessionSavedData->parameter1Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter1Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter1Width, g_WeiXinShareSessionSavedData->parameter1Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        [message setThumbImage:trumbImag];
        
        WXImageObject *ext = [WXImageObject object];
        ext.imageData = UIImagePNGRepresentation(sendImag);
        message.mediaObject = ext;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter1);
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
    }
    else if (value == XWM_WXSENDLINK)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        message.title =
        [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len-1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
        message.description = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter2 length:g_WeiXinShareSessionSavedData->parameter2Len-1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter2 = NULL;
        
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter3, g_WeiXinShareSessionSavedData->parameter3Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter3Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter3Width, g_WeiXinShareSessionSavedData->parameter3Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        if (g_WeiXinShareSessionSavedData->parameter3Len < 32000)
            [message setThumbImage:sendImag];
        else
            [message setThumbImage:trumbImag];
        
        
        WXWebpageObject *ext = [WXWebpageObject object];
        ext.webpageUrl = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter4 length:g_WeiXinShareSessionSavedData->parameter4Len-1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter4 = NULL;
        message.mediaObject = ext;
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter3);
        g_WeiXinShareSessionSavedData->parameter3 = NULL;
    }
    else if (value == XWM_WXSENDMUSIC)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        message.title = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
        message.description = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter2 length:g_WeiXinShareSessionSavedData->parameter2Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter2 = NULL;
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter3 length:g_WeiXinShareSessionSavedData->parameter3Len freeWhenDone:YES]];
        //g_WeiXinShareSessionSavedData->parameter3 = NULL;
        //[message setThumbImage:sendImag];
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter3, g_WeiXinShareSessionSavedData->parameter3Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter3Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter3Width, g_WeiXinShareSessionSavedData->parameter3Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        if (g_WeiXinShareSessionSavedData->parameter3Len < 32000)
            [message setThumbImage:sendImag];
        else
            [message setThumbImage:trumbImag];
        
        WXMusicObject *ext = [WXMusicObject object];
        ext.musicUrl = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter4 length:g_WeiXinShareSessionSavedData->parameter4Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter4 = NULL;
        ext.musicDataUrl = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter5 length:g_WeiXinShareSessionSavedData->parameter5Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter5 = NULL;
        
        message.mediaObject = ext;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter3);
        g_WeiXinShareSessionSavedData->parameter3 = NULL;
    }
    else if (value == XWM_WXSENDVEDIO)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        message.title = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
        message.description = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter2 length:g_WeiXinShareSessionSavedData->parameter2Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter2 = NULL;
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter3 length:g_WeiXinShareSessionSavedData->parameter3Len freeWhenDone:YES]];
        //g_WeiXinShareSessionSavedData->parameter3 = NULL;
        //[message setThumbImage:sendImag];
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter3, g_WeiXinShareSessionSavedData->parameter3Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter3Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter3Width, g_WeiXinShareSessionSavedData->parameter3Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        if (g_WeiXinShareSessionSavedData->parameter3Len < 32000)
            [message setThumbImage:sendImag];
        else
            [message setThumbImage:trumbImag];
        
        WXVideoObject *ext = [WXVideoObject object];
        ext.videoUrl = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter4 length:g_WeiXinShareSessionSavedData->parameter4Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter4 = NULL;
        
        message.mediaObject = ext;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter3);
        g_WeiXinShareSessionSavedData->parameter3 = NULL;
    }
    else if (value == XWM_WXSENDAPPMESSAGE)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        message.title = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
        message.description = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter2 length:g_WeiXinShareSessionSavedData->parameter2Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter2 = NULL;
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter3 length:g_WeiXinShareSessionSavedData->parameter3Len freeWhenDone:YES]];
        //g_WeiXinShareSessionSavedData->parameter3 = NULL;
        //[message setThumbImage:sendImag];
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter3, g_WeiXinShareSessionSavedData->parameter3Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter3Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter3Width, g_WeiXinShareSessionSavedData->parameter3Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        if (g_WeiXinShareSessionSavedData->parameter3Len < 32000)
            [message setThumbImage:sendImag];
        else
            [message setThumbImage:trumbImag];
        
        WXAppExtendObject *ext = [WXAppExtendObject object];
        ext.extInfo = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter4 length:g_WeiXinShareSessionSavedData->parameter4Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter4 = NULL;
        ext.url = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter5 length:g_WeiXinShareSessionSavedData->parameter5Len -1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter5 = NULL;
        
        ext.fileData = [NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter6 length:g_WeiXinShareSessionSavedData->parameter6Len freeWhenDone:YES];
        g_WeiXinShareSessionSavedData->parameter6 = NULL;
        message.mediaObject = ext;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter3);
        g_WeiXinShareSessionSavedData->parameter3 = NULL;
    }
    else if (value == XWM_WXSENDNOTGIFCONTENT)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        //[message setThumbImage:[UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]]];
        //g_WeiXinShareSessionSavedData->parameter1 = NULL;
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter1, g_WeiXinShareSessionSavedData->parameter1Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter1Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter1Width, g_WeiXinShareSessionSavedData->parameter1Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        if (g_WeiXinShareSessionSavedData->parameter1Len < 32000)
            [message setThumbImage:sendImag];
        else
            [message setThumbImage:trumbImag];
        
        WXEmoticonObject *ext = [WXEmoticonObject object];
        ext.emoticonData = [NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter2 length:g_WeiXinShareSessionSavedData->parameter2Len freeWhenDone:YES];
        g_WeiXinShareSessionSavedData->parameter2 = NULL;
        message.mediaObject = ext;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter1);
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
    }
    else if (value == XWM_WXSENDGIFCONTENT)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        //[message setThumbImage:[UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]]];
        //g_WeiXinShareSessionSavedData->parameter1 = NULL;
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter1, g_WeiXinShareSessionSavedData->parameter1Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter1Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter1Width, g_WeiXinShareSessionSavedData->parameter1Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        if (g_WeiXinShareSessionSavedData->parameter1Len < 32000)
            [message setThumbImage:sendImag];
        else
            [message setThumbImage:trumbImag];
        
        WXEmoticonObject *ext = [WXEmoticonObject object];
        ext.emoticonData = [NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter2 length:g_WeiXinShareSessionSavedData->parameter2Len freeWhenDone:YES];
        g_WeiXinShareSessionSavedData->parameter2 = NULL;
        message.mediaObject = ext;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter1);
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
    }
    else if (value == XWM_WXSENDFILECONTENT)
    {
        WXMediaMessage *message = [WXMediaMessage message];
        message.title = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter1 = NULL;
        message.description = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter2 length:g_WeiXinShareSessionSavedData->parameter2Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter2 = NULL;
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter3 length:g_WeiXinShareSessionSavedData->parameter3Len freeWhenDone:YES]];
        //g_WeiXinShareSessionSavedData->parameter3 = NULL;
        //[message setThumbImage:sendImag];
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, g_WeiXinShareSessionSavedData->parameter3, g_WeiXinShareSessionSavedData->parameter3Len, NULL);
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * g_WeiXinShareSessionSavedData->parameter3Width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(g_WeiXinShareSessionSavedData->parameter3Width, g_WeiXinShareSessionSavedData->parameter3Height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *sendImag = [UIImage imageWithCGImage:imageRef];
        //UIImage * sendImag = [UIImage imageWithData:[NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter1 length:g_WeiXinShareSessionSavedData->parameter1Len freeWhenDone:YES]];
        CGSize picSize = CGSizeMake(200, 200);
        UIImage * trumbImag = [UIImage thumbnailWithImageWithoutScale:sendImag size:picSize];
        if (g_WeiXinShareSessionSavedData->parameter3Len < 32000)
            [message setThumbImage:sendImag];
        else
            [message setThumbImage:trumbImag];
        
        WXFileObject *ext = [WXFileObject object];
        ext.fileExtension = [[[NSString alloc]initWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter5 length:g_WeiXinShareSessionSavedData->parameter5Len - 1 encoding:NSUTF8StringEncoding  freeWhenDone:YES]autorelease];
        g_WeiXinShareSessionSavedData->parameter5 = NULL;
        ext.fileData = [NSData dataWithBytesNoCopy:g_WeiXinShareSessionSavedData->parameter4 length:g_WeiXinShareSessionSavedData->parameter4Len freeWhenDone:YES];
        g_WeiXinShareSessionSavedData->parameter4 = NULL;
        message.mediaObject = ext;
        
        SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
        req.bText = NO;
        req.message = message;
        req.scene = g_WeiXinSendType;
        
        [WXApi sendReq:req];
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(imageRef);
        free(g_WeiXinShareSessionSavedData->parameter3);
        g_WeiXinShareSessionSavedData->parameter3 = NULL;
    }
    lock.UnLock();
}

-(void) onResp:(BaseResp*)resp
{
    if(g_WeiXinShareSessionSavedData)
        delete g_WeiXinShareSessionSavedData;
    g_WeiXinShareSessionSavedData = NULL;
}
#endif
-(void)presentOfferWall:(NSString *)url
{
    NSArray * arr = [url componentsSeparatedByString:@"="];
    NSString *userID = nil;
    if ([arr count]>1)
    {
        userID = [arr objectAtIndex:1];
    }
    if ([url hasPrefix:@"http://BW_DOMOB_OFFERWALL"])
    {
        #ifdef BWDMSDKMODE
        self.domobInstance = [[DomobSDKInstance alloc]init];
        [self.domobInstance initOfferWallWithDelegate:self];
        UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
        UIViewController *rootViewController = [window rootViewController];
        rootViewController.modalPresentationStyle = UIModalPresentationFullScreen;
        [self.domobInstance showOfferWallWithAppID:nil andAppSec:nil andUserID:userID];
        self.view.transform = CGAffineTransformMakeRotation(degreeToRadians(0));
        _naviBarInterfaceOrientation = _currentInterfaceOrientation;
        #endif
    }
    else if([url hasPrefix:@"http://BW_LiMei_OFFERWALL"])
    {
        #ifdef BWLMSDKMODE
        self.liMeiInstance = [[LiMeiSDKInstance alloc]init];
        [self.liMeiInstance initOfferWallWithDelegate:self];
        [self.liMeiInstance showOfferWallWithAppID:nil andAppSec:nil andUserID:userID];
        #endif
    }
    else if([url hasPrefix:@"http://BW_YouMi_OFFERWALL"])
    {
        #ifdef BWYMSDKMODE
        self.youMiInstance = [[YMSDKInstance alloc] init];
        [self.youMiInstance showOfferWallWithAppID:nil andAppSec:nil andUserID:userID];
        #endif
    }
    else if([url hasPrefix:@"http://BW_Mobisage_OFFERWALL"])
    {
        #ifdef BWMBSDKMODE
        self.mobisageInstance =[[MobisageSDKInstance alloc]init];
        [self.mobisageInstance initOfferWallWithDelegate:self];
        UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
        UIViewController *rootViewController = [window rootViewController];
        rootViewController.modalPresentationStyle = UIModalPresentationFullScreen;
        if(_naviBarInterfaceOrientation != _currentInterfaceOrientation)
        {
            self.view.hidden = YES;
            [NSTimer scheduledTimerWithTimeInterval:2 target:self selector:@selector(delayMethodAfterPresentedController) userInfo:nil repeats:NO];
        }
        [self.mobisageInstance showOfferWallWithAppID:nil andAppSec:nil andUserID:userID];
        _naviBarInterfaceOrientation = _currentInterfaceOrientation;
        #endif
    }
}

-(void)delayMethodAfterPresentedController
{
    self.view.hidden = NO;
}

-(void)presentNetGameOfferWall
{
    void* mobileView = [self.clientView mobileView];
    g_offerWallFlashPay(mobileView);
}
- (BOOL)prefersStatusBarHidden
{
    return YES;
}
-(void)noticeClientPayUrl:(unsigned int)url
{
    self.payUrl = [NSString stringWithUTF8String:(char *)url];
}
-(void)openWebViewWithUrl:(NSString *)url withIsWebB:(BOOL)isWebB
{
    if (isWebB)
    {
        isWebBViewMode = YES;
        isClientWebViewShow = YES;
        if (!clientWebView)
        {
            clientWebView = [[ClientWebView alloc] initwithUrl:url andFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight - BWWEBBARVIEWHEIGHT + 2)];
        }
        else
            [clientWebView resetHomeUrl:url];
        [clientView addSubview:clientWebView];
    }
    else
    {
        if (!isPayWebViewPresented)
        {
            if(payViewControllor)
                [payViewControllor release];
            payViewControllor = [[PayWebViewController alloc] initwithUrl:url];
            isPayWebViewPresented = YES;
            UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
            UIViewController *rootViewController = [window rootViewController];
            if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
            {
                payViewControllor.modalPresentationStyle = UIModalPresentationOverFullScreen;
                [rootViewController presentViewController:payViewControllor animated:YES completion:nil];
            }
            else
            {
                if(_naviBarInterfaceOrientation == _currentInterfaceOrientation)
                {
                    rootViewController.modalPresentationStyle = UIModalPresentationCurrentContext;
                }
                else
                {
                    rootViewController.modalPresentationStyle = UIModalPresentationFullScreen;
                    self.view.transform = CGAffineTransformMakeRotation(degreeToRadians(0));
                }
                
                [self presentPopViewController:payViewControllor animated:YES completion:nil];
                _naviBarInterfaceOrientation = _currentInterfaceOrientation;
            }
            //[payViewControllor release];
        }
    }
    if(currentMoreBarShowStatus)
        [self showMoreBar:false];

}
-(void)startFlash
{
    isPayWebViewPresented = NO;
    //g_flashPause([clientView mobileView], NO);
}

-(void)setGobackItem:(BOOL)isCanGoback
{
    [webBarBackItem setEnabled:isCanGoback];
}
-(void)clientViewConnectUrl:(NSString *)url
{
    self.mobileViewConnectAddress = url;
    g_mobileViewConnect(clientView.mobileView, [url UTF8String]);
}

- (BOOL) isFileExist:(NSString *)fileName
{
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* resPath = [bundle resourcePath];
    NSString* filePath;
    filePath = [resPath stringByAppendingFormat:@"/%@",fileName];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL result = [fileManager fileExistsAtPath:filePath];
    return result;
}

-(void)changeToOriginalView
{
    if(isWebBViewMode && clientWebView.superview)
        [clientWebView removeFromSuperview];
    isClientWebViewShow = NO;
}

@end


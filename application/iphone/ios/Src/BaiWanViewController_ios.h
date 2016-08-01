//
//  BaiWanViewController_iPhone.h
//  BaiWan
//
//  Created by handong on 11-5-11.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>
#import "HDWidgetView.h"
#import "HDToolBarView.h"
#import "ThirdPartSDKProtocol.h"
#if BWMOBISAGE
#import "MobiSageSDK.h"
#endif
#ifdef BWWXSDKMODE
#import "XWindow.h"
#import "WXApi.h"
#endif
#ifdef BWYMSDKMODE
#import "YouMiSDKInstance.h"
#endif
#ifdef BWMBSDKMODE
#import "MobisageSDKInstance.h"
#endif
#ifdef BWDMSDKMODE
#import "DomobSDKInstance.h"
#endif
#ifdef BWLMSDKMODE
#import "LiMeiSDKInstance.h"
#endif
// 银联SDK
#import "UPPayPluginDelegate.h"
#import "UPPayPlugin.h"
// 未出售状态添加广告使用
#import "DMAdViewController.h"
#import <iAd/iAd.h>
#import "OpenGLView.h"

@class ClientView_ios;
@class ClientWebView;
@class web_localViewController_ios;
@class web_optionViewController_ios;
@class PayWebViewController;
@class flash_game_optionViewController_ios;
@class inputView_ios;
@class HDToolBarView;
@class HDToolBarItem;
@class HDWidget;
@class HDWidgetView;
@class HDTimeLabel;
@class EAGLView;


typedef enum
{
    TO_ORG = 1,
    TO_90,
    TO_90_ORG
}ROTATE_METHOD;
// 工具栏模式
typedef enum
{
    EBARMODE_NONE   =   0x60,       // 初始化模式
    EBARMODE_WEB,                   // 浏览器工具栏
    EBARMODE_LOADING,               // 加载工具栏
    EBARMODE_GAME,                  // 单机游戏工具栏
    EBARMODE_VIDEO,                 // 单机短片工具栏
    EBARMODE_NET,                   // 网页游戏工具栏
}EBARMODE;

// 悬浮widget模式
typedef enum
{
    EWIDGETMODE_NONE    =   0x60,       // 初始化模式
    EWIDGETMODE_SMALL,                  // 小widget模式
    EWIDGETMODE_GAME,                   // 单机游戏工具栏
    EWIDGETMODE_VIDEO,                  // 单机短片工具栏
    EWIDGETMODE_NET,                    // 网页游戏工具栏
    EWIDGETMODE_SETTING,                // 设置模式
}EWIDGETMODE;

// 模式
typedef enum
{
    EMODE_BAR   =   0x60,               // 工具栏
    EMODE_WIDGET,                       // widget
}EMODE;

// 对话框模式
typedef enum 
{
    EALERTVIEWMODE_CLEANCACHE   =   0x60,   // 清理缓存
    EALERTVIEWMODE_UPDATE,                  // 升级提示
    EALERTVIEWMODE_REFRESH,                 // 刷新flash
    EALERTVIEWMODE_EXITFLASH,               // 退出flash
    EALERTVIEWMODE_OPENSAFARI,              // 调用safari
    EALERTVIEWMODE_THIRDPARTSDK,
}EALERTVIEWMODE;

enum
{
    EShow,              //软键盘显示
    EHide,              //软键盘隐藏
    ESplite             //软键盘拆分
};

@interface BaiWanViewController_ios : UIViewController < UIAlertViewDelegate ,
                                                            UIPopoverControllerDelegate ,
                                                            SKProductsRequestDelegate ,
                                                            SKPaymentTransactionObserver,
                                                            HDWidgetViewDelegate,
                                                            HDToolBarViewDelegate,
                                            #ifdef BWWXSDKMODE
                                              WXApiDelegate,
                                            #endif
                                    ThirdPartSDKDelegateProtocol,
                                    UPPayPluginDelegate,
                                    ADBannerViewDelegate,OpenGLViewDelegate>
{
    
    //UIButton *pBtton;
    //NSTimer* pTimer;
    
    // clientView
#if BWPUSHMODE
    BOOL isResignActive;
#endif
    ClientView_ios* clientView;
    
     OpenGLView* _glView;

    
    BOOL isAutoOrientation;
    // 是否初始化
    BOOL isInited;
    // 是否正在保存
    BOOL isSaving;
    // 是否开subview
    bool m_bIsSubview;
    // 是否开启了webView模式
    BOOL isWebBViewMode;
    // 现在是否显示webView;
    BOOL isClientWebViewShow;
    
    // 当前工具栏
    UIView* currentBarView;
    EBARMODE currentBarMode;
    EMODE currentMode;
    BOOL currentBarShowStatus;
    BOOL currentMoreBarShowStatus;
    
    // 浏览器工具栏
    HDToolBarView* webBarView;
    HDToolBarItem* webBarHomeItem;
    HDToolBarItem* webBarLocalItem;
    HDToolBarItem* webBarBackItem;
    HDToolBarItem* webBarRefreshItem;
    HDToolBarItem* webBarMoreItem;
     NSTimer *timer;
    // 浏览器工具栏更多
    HDToolBarView* webMoreBarView;
    HDToolBarItem* webMoreBarPointItem;
//    HDToolBarItem* webMoreBarFeedbackItem;
    HDToolBarItem* webMoreBarCleanCacheItem;
    HDToolBarItem* webMoreBarHelpItem;
    HDToolBarItem* webMoreBarAboutItem;
#if BWTHIRDPARTSDKISON
    HDToolBarItem* webMoreBarSDKCenterItem;
#endif
    
    // 单机游戏工具栏
    HDToolBarView* gameBarView;
    UIImageView* gameBarBackgroundImageView;
    UIButton* gameBarModeItem;
    UIButton* gameBarSettingItem;
    UIButton* gameBarSaveItem;
    UIButton* gameBarZoomItem;
    UIButton* gameBarHelpItem;
    UIButton* gameBarCloseItem;
    
    // 短片工具栏
    HDToolBarView* videoBarView;
    UIImageView* videoBarBackgroundImageView;
    UIButton* videoBarModeItem;
    UIButton* videoBarSettingItem;
    UIButton* videoBarSaveItem;
    UIButton* videoBarZoomItem;
    UIButton* videoBarCloseItem;
    
    // 网页游戏工具栏
/*    HDToolBarView* netBarView;
    UIImageView* netBarBackgroundImageView;
    UIButton* netBarModeItem;
    UIButton* netBarSettingItem;
    UIButton* netBarZoomItem;
    UIButton* netBarPayItem;
    UIButton* netBarRefreshItem;
    UIButton* netBarCloseItem;
 */
    // 当前widget
    HDWidgetView* currentWidgetView;
    EWIDGETMODE currentWidgetMode;
    BOOL currentWidgetShowStatus;
    
    // widget
    HDWidgetView* widgetView;
    UIImageView* loadingView;
    HDTimeLabel* timeView;
    HDWidgetView* tmpWidgetView;
    HDWidget* moreWidget;
    HDWidget* zoomWidget;
    
    HDWidgetView* moreWidgetView;
    HDWidget* moreSaveWidget;
    HDWidget* moreSettingWidget;
    HDWidget* moreHelpWidget;
    HDWidget* morePayWidget;
    HDWidget* moreReloadWidget;
    HDWidget* moreBackWidget;
    HDWidget* moreCloseWidget;
    
    HDWidgetView* settingWidgetView;
    HDWidget* morePointWidget;
    HDWidget* moreButtonWidget;
    HDWidget* moreModeWidget;
#if SDKNEEDUSERCENTER
    HDWidget* moreUserCenterWidget;
#endif
    
    // 本地viewcontroller
    web_localViewController_ios* localViewController;
    // 输入viewcontroller
    inputView_ios* inputView;
    ClientWebView * clientWebView;
    UIScrollView *iScrollView;
    // The url for launch
    NSURL* launchUrl;
    // 默认本地swf文件路径
    NSString* defaultSwfPath;
    
#if BWSHOWBARMODE
    BOOL showBarModeEnable;
#endif
#if BWONSALEMODE
    BOOL onSaleModeEnable;
    BOOL showADModeEnable;
    BOOL showBoutiqueGame;
    NSTimer* onSaleModeTimer;
#endif
#if BWTHIRDPARTSDKISON
    BOOL outLoginModeEnable;
    BOOL viewIsAppeared;
#endif
#if BWNEWAPPMODE
    int newAppStatus;
    NSString* newAppUrl;
#endif
#if BWPUSHMODE
    BOOL isPushUrlOpen;
#endif
    
#if BWIAPMODE
    int         IAPStepID;
    int         IAPCodeID;
    SKProductsRequest* productRequest;
    int         IAPTestNum;//内购测试次数
#endif
//    UIView*     IAPCurrentStepView;
//    UILabel*    IAPCurrentStepLabel;
    
#if BWMOBISAGE
    MobiSageAdBanner*   banner;    // mobisage 广告
    UIButton*           btn;       // 广告关闭按钮
#endif
    BOOL isPayWebViewPresented;
    PayWebViewController * payViewControllor;
    ADBannerView *_bannerView;
}
@property   BOOL isStypeValid;
#ifdef BWSHOWBARMODE
@property (readonly) BOOL showBarModeEnable;
#endif//BWSHOWBARMODE

@property (readonly) EMODE currentMode;
@property (readonly) bool m_bIsSubview;

@property (nonatomic, readonly) UIInterfaceOrientation currentInterfaceOrientation;
@property UIInterfaceOrientation naviBarInterfaceOrientation;
@property (nonatomic, assign)  ClientView_ios* clientView;
@property (nonatomic, assign) inputView_ios* inputView;
@property (strong, retain)  OpenGLView *glView;

@property (nonatomic, copy) NSURL* launchUrl;
@property BOOL isNetRefreshing; // 是否正在刷新页面
#if BWPUSHMODE
@property BOOL isPushUrlOpen;
#endif

#if BWMOBISAGE
@property (readonly) MobiSageAdBanner* banner;
@property (readonly) UIButton* btn;
#endif
@property(copy)NSString *payUrl;
@property (copy) NSString *payUrlWithNum;
@property (copy) NSString * mobileViewConnectAddress;
@property (nonatomic, retain)DMAdViewController *adViewController;

#ifdef BWYMSDKMODE
@property (nonatomic, retain)YMSDKInstance *youMiInstance;
#endif
#ifdef BWMBSDKMODE
@property (nonatomic, retain)MobisageSDKInstance *mobisageInstance;
#endif
#ifdef BWDMSDKMODE
@property (nonatomic, retain)DomobSDKInstance *domobInstance;
#endif
#ifdef BWLMSDKMODE
@property (nonatomic, retain)LiMeiSDKInstance *liMeiInstance;
#endif

#if BWIAPMODE
@property (nonatomic, retain)NSString*   IAPProductID;
@property (nonatomic, retain)NSString*   IAPPrice;
@property (nonatomic, retain)NSString*   IAPOrderID;
@property (nonatomic, retain)NSString*   IAPDestription;
@property (nonatomic, retain)NSString*   IAPChargeData;
@property (nonatomic, retain)NSString*   IAPChargeUrlData;
@property (nonatomic, retain)NSTimer*    IAPFeedBackTimer;
@property (nonatomic, retain)NSString*   IAPFeedBackParameter;
@property (nonatomic, retain)NSString *  IAPTransactionIdentifier;
@property (nonatomic, retain)NSString *  IAPfeedBackString;
#endif

+ (BOOL)isJailBroken;
- (void)setKeyBoardFrame:(CGRect)frame withDuration:(NSTimeInterval)duration isKeyBoard:(int)aKeyBoard;
- (void)setNoneTouch:(bool) isTouch;
- (void)hideMoreBar;
- (void)startEdit;
- (void)finishEdit;
- (void)needScrollV:(bool) isNeed;

// 显示/隐藏工具栏
- (void)showBar:(BOOL)aShowEnable;
// 显示/隐藏widget
- (void)showWidget:(BOOL)aShowEnable;

- (void)completeTransaction:(SKPaymentTransaction *)transaction;
- (void)restoreTransaction:(SKPaymentTransaction *)transaction;
- (void)failedTransaction:(SKPaymentTransaction *)transaction;
- (void)changeToRotation:(ROTATE_METHOD)rotateMethod;

#if BWIAPMODE
- (IBAction)onCurrentStepCancel:(id)sender;
#endif

#if BWPUSHMODE
- (BOOL)isResignActive;
- (void)setResignActive:(BOOL)resignActive;
#endif

//更改工具条
- (void) xgg_changeBar;

// 内核通知客户端
- (void)noticeFlashStart;
- (void)noticeFlashFinish;
- (void)noticePageLoadingStart;
- (void)noticePageLoadingPercent:(int)percent;
- (void)noticePageLoadingFinish;
- (void)noticeFlashSavingPercent:(int)percent;
- (void)noticeSubViewOpen;
- (void)noticeSubViewClose;
- (void)noticeZoominEnable;
- (void)noticeZoomoutEnable;
- (void)noticeShowBarAndOnSale:(unsigned int)value;
- (void)noticeNewAppStatus:(unsigned int)value;
- (void)noticeNewAppUrl:(unsigned int)value;
- (void)noticeOutLogin:(unsigned int)value;
- (void)noticeIosPid:(NSString *)value;
- (void)noticeStopIosPid:(unsigned int)value;
- (void)noticeThirdPartPid:(unsigned int)value;
- (void)noticeUpdateStart:(int)nMax;
- (void)noticeUpdateSet:(int)nProgValue;
- (void)noticeUpdateEnd;
//可以保存回调
- (void)noticeCallBackFlashCanSave;
//可以支付
- (void)noticeCallBackFlashCanPay;

- (void)NoticeShowHideMenu:(bool)bShow;

#ifdef BWWXSDKMODE
-(void)g_noticeWeiXinSendSession:(unsigned int)value;
#endif
-(void)presentOfferWall:(NSString *)url;
-(void)openWebViewWithUrl:(NSString *)url withIsWebB:(BOOL)isWebB;

-(void)noticeClientPayUrl:(unsigned int)url;
-(void)startFlash;
-(void)setGobackItem:(BOOL)isCanGoback;
-(void)clientViewConnectUrl:(NSString *)url;
-(void)changeToOriginalView;
@end


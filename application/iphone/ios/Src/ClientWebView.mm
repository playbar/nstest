//
//  ClientWebView.m
//  BaiWan
//
//  Created by xiepf on 14-5-6.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import "ClientWebView.h"
#import <objc/runtime.h>
#import "PayWebViewController.h"
#import "UIViewController+popController.h"
#import "GGBase.h"
#import "BaiWanViewController_ios.h"
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
//#endif
#define WEBVIEW_INIIAL_PROGRESS 10


extern iOSDeviceRelateData g_deviceData;
extern BaiWanViewController_ios* g_explorerController_ios;

@interface ClientWebView ()
@property (assign, nonatomic) UIActivityIndicatorView *activityView;
@end
@implementation ClientWebView

-(void)onGes:(UITapGestureRecognizer *)sender
{
    [g_explorerController_ios hideMoreBar];
    if (self.mouseShowEnable)
    {
        if (aTimer)
        {
            [aTimer invalidate];
            aTimer = nil;
        }
        
        aTimer = [NSTimer scheduledTimerWithTimeInterval:0.3
                                                  target:self
                                                selector:@selector(aimageView_hide:)
                                                userInfo:nil
                                                 repeats:NO];
        CGPoint touchPoint = [gesT locationInView:self];
        [aImageView setCenter:touchPoint];
        [aImageView setHidden:NO];
    }
}
-(id)initwithUrl:(NSString *)url andFrame:(CGRect)frame
{
    id returned = [super initWithFrame:frame];
    if (returned)
    {
        errorPageDidShow = FALSE;
        errorPageBeginShow = FALSE;
        self.homeUrl = url;
        width = frame.size.width;
        height = frame.size.height;
        CGRect boundFrame;
        if ((!g_deviceData.mbOnSale)&&(g_deviceData.m_scale>1.1))
        {
            boundFrame = CGRectMake(0, 0, width * g_deviceData.m_scale, height * g_deviceData.m_scale);
            self.frame = boundFrame;
            
            self.layer.anchorPoint = CGPointMake(0.5, 0.5);
            CGAffineTransform at1 = CGAffineTransformMakeTranslation(-width, -height);
            CGAffineTransform at2 = CGAffineTransformMakeScale(1/g_deviceData.m_scale, 1/g_deviceData.m_scale);
            CGAffineTransform  at3;
            if(g_deviceData.m_scale > 2.1)
            {
//                CGAffineTransform at1 = CGAffineTransformMakeTranslation(-width, -height);
//                CGAffineTransform at2 = CGAffineTransformMakeScale(1/g_deviceData.m_scale, 1/g_deviceData.m_scale);
                at3 = CGAffineTransformConcat(at2,at1);
            }else
            {
//                CGAffineTransform at1 = CGAffineTransformMakeTranslation(-width, -height);
//                
//                CGAffineTransform at2 = CGAffineTransformMakeScale(0.5, 0.5);
                at3 = CGAffineTransformConcat(at1,at2);
            }
            [self setTransform:at3];
            
        }
        else
        {
            boundFrame = CGRectMake(0, 0, width , height);
            self.frame = boundFrame;
        }
        
        self.scrollView.showsHorizontalScrollIndicator = NO;
        self.scrollView.showsVerticalScrollIndicator = NO;
        NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.homeUrl]];
        self.delegate =self;
        [self loadRequest:request];
        
        _activityView = [[UIActivityIndicatorView alloc]initWithFrame:CGRectMake( (boundFrame.size.width  - 37)/2, (boundFrame.size.height  - 37)/2, 37, 37)];
        _activityView.color = [UIColor blackColor];
        [self addSubview:_activityView];
        _activityView.hidesWhenStopped = YES;
        [_activityView startAnimating];
        gesT = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(onGes:)];
        gesT.delegate = self;
        gesT.cancelsTouchesInView = NO;
        [self addGestureRecognizer:gesT];
        
        
        aImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"touch.png"]];
        [self addSubview:aImageView];
        [aImageView setHidden:YES];
        self.mouseShowEnable = YES;
        NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
        NSNumber* pointNumber = (NSNumber*)[bwDictionary objectForKey:BWPOINTKEY];
        if (pointNumber) {
            self.mouseShowEnable = [pointNumber boolValue];
        }

    }
    return returned;
}

-(void)resetHomeUrl:(NSString *)url
{
    self.homeUrl =  url;
    [self stopLoading];
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.homeUrl]];
    self.delegate =self;
    [self loadRequest:request];
    [_activityView startAnimating];
    [g_explorerController_ios setGobackItem:NO];
}

-(void)onHomeButton
{
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.homeUrl]];
    [self loadRequest:request];
}

-(void)onBackButton
{
    [self goBack];
}

-(void)onRefreshButton
{
    [self reload];
}
-(void)onCloseButton
{
    [self stopLoading];
    self.delegate = nil;
    [self removeFromSuperview];
}

-(void)dealloc
{
    [_activityView release];
    [aImageView release];
    [gesT release];
    [super dealloc];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    // js
    NSString * encodedString = (NSString*)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault,
                                                                                                  (CFStringRef)request.URL.absoluteString,
                                                                                                  CFSTR(""),
                                                                                                  kCFStringEncodingUTF8);
    BWLOG(@"-->%@",encodedString);
//    NSLog(@"***%@",start);
    NSRange range1 = [encodedString rangeOfString:@"/BaiWanIOSopenAlipay|"];
    //NSRange range2 = [encodedString rangeOfString:@"/BaiWanIOScloseWebView"];
    NSRange range3 = [encodedString rangeOfString:@"/BaiWanIOSUPPay|"];
    NSRange range4 = [encodedString rangeOfString:@"/BWWEBGAME|"];
    if(range1.location != NSNotFound)
    {
        NSArray* pidArray = [encodedString componentsSeparatedByString:@"|"];
        if ([pidArray count]!=7)
        {
            return NO;
        }
        
        NSString *appScheme = @"AlixPay2088701209894192";
        AlixPayOrder *order = [[AlixPayOrder alloc] init];
        order.partner = [pidArray objectAtIndex:5];
        order.seller = [pidArray objectAtIndex:6];
        order.tradeNO = [pidArray objectAtIndex:1]; //订单ID（由商家自行制定）
        order.productName = [pidArray objectAtIndex:2]; //商品标题
        order.productDescription = [pidArray objectAtIndex:2]; //商品描述
        order.amount = [NSString stringWithFormat:@"%.2f",[[pidArray objectAtIndex:3] doubleValue]]; //商品价格
        order.notifyURL =  [pidArray objectAtIndex:4]; //回调URL
        NSString* orderInfo = [order description];
        id<DataSigner> signer;
        signer = CreateRSADataSigner(PartnerPrivKey);
        NSString *signedStr = [signer signString:orderInfo];
        
        NSString *orderString = [NSString stringWithFormat:@"%@&sign=\"%@\"&sign_type=\"%@\"",
                                 orderInfo, signedStr, @"RSA"];
        //NSLog(@"-->%@",orderString);
        [AlixLibService payOrder:orderString AndScheme:appScheme seletor:@selector(paymentResult:) target:self];
        AlixViewController *vc = [[AlixViewController alloc] init];
        [g_explorerController_ios presentViewController:vc animated:YES completion:^{
            
        }];
        [vc release];
        return NO;
    }
//    if(range2.location != NSNotFound)
//    {
//        [self onCloseButton];
//        return NO;
//    }
    if(range3.location != NSNotFound)
    {
        NSArray* pidArray = [encodedString componentsSeparatedByString:@"|"];
        NSString *tn = [pidArray lastObject];
        
        [UPPayPlugin startPay:tn mode:@"00" viewController:g_explorerController_ios delegate:self];
        return NO;
    }
    if (range4.location != NSNotFound)
    {
        NSArray* pidArray = [encodedString componentsSeparatedByString:@"|"];
        NSString *url = [pidArray lastObject];
        [g_explorerController_ios clientViewConnectUrl:url];
        return NO;
    }
    
    if ([[encodedString substringToIndex:7] isEqualToString:@"webv://"])
    {
        NSMutableString * mutablestring = [[NSMutableString alloc]initWithString:@"http://"];
        [mutablestring appendString:[encodedString substringFromIndex:7]];
        [g_explorerController_ios openWebViewWithUrl:mutablestring withIsWebB:NO];
        [mutablestring release];
        return NO;
    }
    
    if ([encodedString hasSuffix:@"|IAPSDKPAY"])
    {
        NSRange range4 = [encodedString rangeOfString:@"|"];
        NSString *value = [[NSString alloc] initWithString:[encodedString substringFromIndex:range4.location+1]];
        [g_explorerController_ios noticeIosPid:value];
        return NO;
    }
    progress = 0;
    totalRequestCount = totalLoadedCount = 0;
    [g_explorerController_ios noticePageLoadingStart];
    
    return YES;
}
- (void)UPPayPluginResult:(NSString *)result
{
    //NSString* msg = [NSString stringWithFormat:kResult, result];
    //[self showAlertMessage:msg];
}
- (void)webViewDidStartLoad:(UIWebView *)webView
{
    totalRequestCount ++;
    if (progress < WEBVIEW_INIIAL_PROGRESS)
    {
        progress = WEBVIEW_INIIAL_PROGRESS;
        [g_explorerController_ios noticePageLoadingPercent:progress];
    }
    [_activityView startAnimating];
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    if (errorPageBeginShow)
        errorPageDidShow = TRUE;
    else
        errorPageDidShow = FALSE;
    // 禁用用户选择
    [self stringByEvaluatingJavaScriptFromString:@"document.documentElement.style.webkitUserSelect='none';"];
    // 禁用长按弹出框
    [self stringByEvaluatingJavaScriptFromString:@"document.documentElement.style.webkitTouchCallout='none';"];

    [_activityView stopAnimating];
    [g_explorerController_ios setGobackItem:[self canGoBack]];
    totalLoadedCount ++;
    if (totalRequestCount <= totalLoadedCount)
    {
        [g_explorerController_ios noticePageLoadingPercent:100];
        [self performSelector:@selector(HideProgressBar) withObject:nil afterDelay:0.5];
    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    [_activityView stopAnimating];
    totalLoadedCount ++;
    if (totalRequestCount <= totalLoadedCount)
    {
        [g_explorerController_ios noticePageLoadingPercent:100];
        [self performSelector:@selector(HideProgressBar) withObject:nil afterDelay:0.5];
    }
    if (([error code] != NSURLErrorCancelled)&&(!errorPageDidShow))
    {
        NSBundle* bundle = [NSBundle mainBundle];
        NSString* resPath = [bundle resourcePath];
        NSString* filePath;
        filePath = [resPath stringByAppendingFormat:@"%@",@"/error.html"];
        [self loadHTMLString:[NSString stringWithContentsOfFile:filePath encoding:NSUnicodeStringEncoding error:nil] baseURL:[bundle bundleURL]];
        errorPageBeginShow = YES;
    }
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    if (self.mouseShowEnable)
    {
        if (aTimer)
        {
            [aTimer invalidate];
            aTimer = nil;
        }
        
        aTimer = [NSTimer scheduledTimerWithTimeInterval:0.3
                                                  target:self
                                                selector:@selector(aimageView_hide:)
                                                userInfo:nil
                                                 repeats:NO];
        CGPoint touchPoint = [touch locationInView:self];
        [aImageView setCenter:touchPoint];
        [aImageView setHidden:NO];
    }
    [g_explorerController_ios hideMoreBar];
    return YES;
}


- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return YES;
}

- (void)aimageView_hide:(NSTimer *) timer
{
    [aImageView setHidden:YES];
    aTimer = nil;
}

-(void)HideProgressBar
{
    [g_explorerController_ios noticePageLoadingFinish];
}
@end

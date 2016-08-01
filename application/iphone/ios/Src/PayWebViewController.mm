//
//  PayWebViewController.m
//  BaiWan
//
//  Created by imac1 on 14-1-22.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//
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
extern iOSDeviceRelateData g_deviceData;
extern BaiWanViewController_ios* g_explorerController_ios;

@interface PayWebViewController ()
@property (assign, nonatomic) UIActivityIndicatorView *activityView;
//@property (assign, nonatomic) UILabel *errorLabel;
@property (assign, nonatomic) UINavigationItem *barItem;
@end

@implementation PayWebViewController

-(id)initwithUrl:(NSString *)url
{
    self = [super init];
    if (self)
    {
        //url = @"http://512*384/hidewebbar/m.jqbar.com/testweb/test/testlist.aspx";
        isBarHide = NO;
        timerForErrorShow = nil;
        isOpaque = NO;
        //NSRange range = [url rangeOfString:@""];
        NSRange range = [url rangeOfString:@"hidewebbar"];
        if (range.location != NSNotFound)
        {
            isBarHide = YES;
            NSString *stringAfterRange = [url substringFromIndex:range.location];
            NSRange tail = [stringAfterRange rangeOfString:@"/"];
            NSRange pre;
            pre.location = 0;
            pre.length = range.location;
            url = [NSString stringWithFormat:@"%@%@",[url substringWithRange:pre],[stringAfterRange substringFromIndex:tail.location + 1]];
        }
        
        NSString * stringAferhttp = [url substringFromIndex:7];
        NSArray* pidArray = [stringAferhttp componentsSeparatedByString:@"/"];
        range = [[pidArray objectAtIndex:0] rangeOfString:@"*"];
        int lenthToSkip = [[pidArray objectAtIndex:0] length];
        if (range.location == NSNotFound)
        {
            self.payUrl = url;
            width = g_deviceData.m_screenWidth;
            height = g_deviceData.m_screenHeight;
        }
        else
        {
            isOpaque = YES;
            NSArray* pidArraySize = [[pidArray objectAtIndex:0] componentsSeparatedByString:@"*"];
            //pidArraySize = [@"512*" componentsSeparatedByString:@"*"];
            float widthTemp = [[pidArraySize objectAtIndex:0]floatValue];
            float heighTemp = [[pidArraySize objectAtIndex:1]floatValue];
            
            if ([[pidArraySize objectAtIndex:0] isEqualToString:@""] ||
                (widthTemp > g_deviceData.m_screenWidth))
            {
                width = g_deviceData.m_screenWidth;
                
            }
            else
            {
                width = widthTemp;
            }
            
            if ([[pidArraySize objectAtIndex:1]isEqualToString:@""] ||
                (heighTemp > g_deviceData.m_screenHeight))
            {
                height = g_deviceData.m_screenHeight;
            }
            else
            {
                height = [[pidArraySize objectAtIndex:1]floatValue];
            }
            
            NSMutableString * mutablestring = [[NSMutableString alloc]initWithString:@"http://"];
            [mutablestring appendString:[url substringFromIndex:lenthToSkip + 8]];
            self.payUrl = [mutablestring substringFromIndex:0];
            [mutablestring release];
        }
    }
    //self.payUrl = @"http://m.jqbar.com/testweb/test/testlist.aspx";
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor clearColor];
    float orgLeft = (g_deviceData.m_screenWidth - width)/2;
    float orgTop = (g_deviceData.m_screenHeight - height)/2;
    CGRect naviBarBoundIphone;
    naviBarBoundIphone.origin.x = orgLeft;
    naviBarBoundIphone.origin.y = orgTop;
    naviBarBoundIphone.size.width = width;
    naviBarBoundIphone.size.height = 44;
    if (!isBarHide)
    {
        UINavigationBar *navBar = [[UINavigationBar alloc]initWithFrame:naviBarBoundIphone];
        
        _barItem = [[UINavigationItem alloc]initWithTitle:nil];
        
        UIBarButtonItem * barButtonItem = [[UIBarButtonItem alloc]initWithTitle:@"返回" style:UIBarButtonItemStyleDone target:self action:@selector(onCloseButton)];
        [_barItem setLeftBarButtonItem:barButtonItem];
        [barButtonItem release];
        UIBarButtonItem * barButtonItemEdit = [[UIBarButtonItem alloc]initWithTitle:@"刷新" style:UIBarButtonItemStyleDone target:self action:@selector(onRefreshButton)];
        [_barItem setRightBarButtonItem:barButtonItemEdit];
        [barButtonItemEdit release];
        
        [navBar setItems:[NSArray arrayWithObject:_barItem]];
        //[barItem release];
        
        [self.view addSubview:navBar];
        [navBar release];
    }
    
    if ((!g_deviceData.mbOnSale)&&(g_deviceData.m_scale>1.1))
    {
        if (isBarHide)
        {
            payWebView = [[UIWebView alloc]initWithFrame:CGRectMake(orgLeft, orgTop, width * 2 , height*2)];
            payWebView.layer.anchorPoint = CGPointMake(0.5, 0.5);
            CGAffineTransform at1 = CGAffineTransformMakeTranslation(-width, -height);
            CGAffineTransform at2 = CGAffineTransformMakeScale(0.5, 0.5);
            CGAffineTransform  at3 = CGAffineTransformConcat(at1,at2);
            [payWebView setTransform:at3];
        }
        else
        {
            payWebView = [[UIWebView alloc]initWithFrame:CGRectMake(orgLeft, orgTop + 44, width * 2 , (height - 44)*2)];
            payWebView.layer.anchorPoint = CGPointMake(0.5, 0.5);
            CGAffineTransform at1 = CGAffineTransformMakeTranslation(-width, -(height - 44));
            CGAffineTransform at2 = CGAffineTransformMakeScale(0.5, 0.5);
            CGAffineTransform  at3 = CGAffineTransformConcat(at1,at2);
            [payWebView setTransform:at3];
        }
    }
    else
    {
        if (isBarHide)
            payWebView = [[UIWebView alloc]initWithFrame:CGRectMake(orgLeft, orgTop, width, height)];
        else
            payWebView = [[UIWebView alloc]initWithFrame:CGRectMake(orgLeft, orgTop + 44, width , height - 44)];
    }
    if(isOpaque)
    {
        payWebView.backgroundColor = [UIColor clearColor];
        payWebView.opaque = NO;
    }
    [self.view addSubview:payWebView];
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.payUrl]];
    payWebView.delegate =self;
    [payWebView loadRequest:request];
    
    _activityView = [[UIActivityIndicatorView alloc]initWithFrame:CGRectMake(orgLeft + (width  - 37)/2, orgTop + (height  - 37)/2, 37, 37)];
    _activityView.color = [UIColor blackColor];
    [self.view addSubview:_activityView];
    _activityView.hidesWhenStopped = YES;
    [_activityView startAnimating];
    
//    _errorLabel = [[UILabel alloc]initWithFrame:CGRectMake(orgLeft + (width  - 130)/2, orgTop + (height  - 21)/2, 130, 21)];
//    _errorLabel.text = @"网络连接失败";
//    _errorLabel.backgroundColor = [UIColor clearColor];
//    _errorLabel.textColor = [UIColor darkGrayColor];
//    _errorLabel.font = [UIFont systemFontOfSize:20];
//    _errorLabel.hidden = YES;
//    [self.view addSubview:_errorLabel];
}

-(void)onRefreshButton
{
    [payWebView reload];
}
-(void)onCloseButton
{
//    if (timerForErrorShow)
//    {
//        [timerForErrorShow invalidate];
//        [timerForErrorShow release];
//        timerForErrorShow = nil;
//    }
    
    [payWebView stopLoading];
    payWebView.delegate = nil;
    [self dismissPopViewControllerAnimated:YES completion:^(void){
        [g_explorerController_ios startFlash];
            }];;
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
-(void)dealloc
{
    [payWebView release];
    [_barItem release];
    [_activityView release];
    [super dealloc];
}
- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    if (g_explorerController_ios.currentInterfaceOrientation == UIInterfaceOrientationPortrait)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    else
    {
        return UIInterfaceOrientationMaskLandscapeRight;
    }
}
-(UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    if (g_explorerController_ios.currentInterfaceOrientation == UIInterfaceOrientationPortrait)
    {
        return UIInterfaceOrientationPortrait;
    }
    else
    {
        return UIInterfaceOrientationLandscapeRight;
    }
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (g_explorerController_ios.currentInterfaceOrientation == UIInterfaceOrientationPortrait)
    {
        return interfaceOrientation == UIInterfaceOrientationPortrait;
    }
    else
    {
        return interfaceOrientation == UIInterfaceOrientationLandscapeRight;
    }
}
-(BOOL)prefersStatusBarHidden
{
    return YES;
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    // js
    NSString * encodedString = (NSString*)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault,
                                                                                                  (CFStringRef)request.URL.absoluteString,
                                                                                                  CFSTR(""),
                                                                                                  kCFStringEncodingUTF8);
    BWLOG(@"-->%@",encodedString);
    NSRange range1 = [encodedString rangeOfString:@"/BaiWanIOSopenAlipay|"];
    NSRange range2 = [encodedString rangeOfString:@"/BaiWanIOScloseWebView"];
    NSRange range3 = [encodedString rangeOfString:@"/BaiWanIOSUPPay|"];
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
        [self presentViewController:vc animated:YES completion:^{
            
        }];
        [vc release];
        return NO;
    }
    if(range2.location != NSNotFound)
    {
        [self onCloseButton];
        return NO;
    }
    if(range3.location != NSNotFound)
    {
        NSArray* pidArray = [encodedString componentsSeparatedByString:@"|"];
        NSString *tn = [pidArray lastObject];

        [UPPayPlugin startPay:tn mode:@"00" viewController:self delegate:self];
        return NO;
    }
    if ([encodedString hasSuffix:@"|IAPSDKPAY"])
    {
        NSRange range4 = [encodedString rangeOfString:@"|"];
        NSString *value = [[NSString alloc] initWithString:[encodedString substringFromIndex:range4.location+1]];
        [g_explorerController_ios noticeIosPid:value];
        return NO;
    }
    return YES;
}
- (void)UPPayPluginResult:(NSString *)result
{
    //NSString* msg = [NSString stringWithFormat:kResult, result];
    //[self showAlertMessage:msg];
}
- (void)webViewDidStartLoad:(UIWebView *)webView
{
    //_errorLabel.hidden = YES;
    [_activityView startAnimating];
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    //_errorLabel.hidden = YES;
    [_activityView stopAnimating];
    _barItem.title = [webView stringByEvaluatingJavaScriptFromString:@"document.title"];
}
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    //if (_errorLabel.hidden)
    //{
     //   _errorLabel.hidden = NO;
     //   timerForErrorShow = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(hideErrorLabel) userInfo:nil repeats:NO];
    //    [timerForErrorShow retain];
    //}
    [_activityView stopAnimating];
    
}


//-(void)hideErrorLabel
//{
//    _errorLabel.hidden = YES;
//    if (timerForErrorShow)
//    {
//        [timerForErrorShow invalidate];
//        [timerForErrorShow release];
//        timerForErrorShow = nil;
//    }
//}
@end

//
//  ViewController.m
//  InterstitialAdView
//
//  Created by domob on 14-2-8.
//  Copyright (c) 2014年 domob. All rights reserved.
//

#import "DMAdViewController.h"
#import "IPhoneDeviceData.h"
#import "BaiWanViewController_ios.h"

extern iOSDeviceRelateData g_deviceData;
extern BaiWanViewController_ios* g_explorerController_ios;
@interface DMAdViewController ()<UIWebViewDelegate>
{
    BOOL loadSuceed;
}
@property (nonatomic, retain) UIWebView *myWeb;
@property (nonatomic, retain) UIButton *myBtn;
//@property (nonatomic, retain) NSURLConnection *urlConnection;
//@property (nonatomic, retain) NSMutableData *responseData;

@end

@implementation DMAdViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (id)init
{
    if (self = [super init]) {
        //设定frame与color
        loadSuceed = NO;
        CGRect rect = [UIScreen mainScreen].bounds;
        self.view.frame = rect;
        self.view.backgroundColor = [UIColor colorWithRed:255.0/255 green:255.0/255 blue:255.0/255 alpha:0.5];
        
        CGRect webRect = CGRectZero;
        CGRect btnRect = CGRectZero;
        // 判定 ipad or iphone 然后赋值
//#warning 请根据自己的横竖屏视图自定义该值
        if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
        {
            webRect = CGRectMake((g_deviceData.m_screenWidth - 302)/2, (g_deviceData.m_screenHeight - 302)/2, 302, 302);
            btnRect = CGRectMake((g_deviceData.m_screenWidth - 302)/2 + 280 , (g_deviceData.m_screenHeight - 302)/2 - 14, 36, 36);
            
        }
        else
        { //ipad
            webRect = CGRectMake((g_deviceData.m_screenWidth - 500)/2, (g_deviceData.m_screenHeight - 500)/2, 500, 500);
            btnRect = CGRectMake((g_deviceData.m_screenWidth - 500)/2 + 500 -18, (g_deviceData.m_screenHeight - 500)/2 - 20, 36, 36);
        }
        _myWeb = [[UIWebView alloc] initWithFrame:webRect];
        _myWeb.scalesPageToFit = YES;
        _myWeb.delegate = self;
        _myWeb.scrollView.scrollEnabled = NO;
        //阴影
        UIBezierPath *shadowPath = [UIBezierPath bezierPathWithRect:webRect];
        _myWeb.layer.masksToBounds = NO;
        _myWeb.layer.shadowColor = [UIColor blackColor].CGColor;
        _myWeb.layer.shadowOffset = CGSizeMake(0,0);
        _myWeb.layer.shadowOpacity = 0.9f;
        _myWeb.layer.shadowRadius = 10;
        [shadowPath moveToPoint:webRect.origin];
        
        _myBtn = [[UIButton alloc] initWithFrame:btnRect];
        [self.myBtn setImage:[UIImage imageNamed:@"AdCloseBtn"] forState:UIControlStateNormal];
        [self.myBtn setImage:[UIImage imageNamed:@"AdCloseBtn"] forState:UIControlStateSelected];
        
        [self.myBtn addTarget:self
                       action:@selector(closeAdAction)
             forControlEvents:UIControlEventTouchUpInside];
        
        [self.view addSubview:self.myWeb];
        [self.view addSubview:self.myBtn];
    }
    return self;
}

-(void)delayLoading
{
    timer = [NSTimer scheduledTimerWithTimeInterval:5
                                             target:self
                                           selector:@selector(loadRequest)
                                           userInfo:nil
                                            repeats:YES];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark --
#pragma mark NSURLConnectionMethod
- (void)loadRequest
{
    [_myWeb stopLoading];
    if (loadSuceed)
    {
        [timer invalidate];
        return;
    }
//    NSString *string = [NSString stringWithFormat:@"http://r.domob.cn/iOS/promotion.html?bid=%@&width=%d&height=%d",[self getBundleIdentifier],(int)self.myWeb.frame.size.width,(int)self.myWeb.frame.size.height];
//    //string = @"http://r.domob.cn/iOS/promotion.html?bid=cn.domob.InterstitialAdView&width=300&height=250";
//    NSURL *url = [NSURL URLWithString:string];
//    
//    NSMutableURLRequest *theRequest = [NSMutableURLRequest requestWithURL:url
//                                                              cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
//                                                          timeoutInterval:3.0];
//    
//    [theRequest addValue: @"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
//    
//	[theRequest setHTTPMethod:@"GET"];
//    
//    _urlConnection = [NSURLConnection connectionWithRequest:theRequest delegate:self];
    NSURL *url = [NSURL URLWithString:@"http://m.jqbar.com/IPAD/AD/IPadAd.aspx"];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [_myWeb loadRequest:request];
    BWLOG(@"loadStarted");
}

#pragma mark --
#pragma mark NSURLConnectionDelegate
//- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
//{
//    // reinit
//	_responseData = [[NSMutableData alloc] init];
//}
//
//- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
//{
//    [_responseData appendData:data];
//}
//
//- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
//{
//    //请求失败 不作操作
//}
//
//- (void)connectionDidFinishLoading:(NSURLConnection *)connection
//{
//    
//    // 成功接收内容，并且Http Status 为 200
//    if (_responseData && [_responseData length] > 0)
//    {
//#warning 此处使用的系统方法解析json 仅支持iOS5.0 及其以上  如果支持更低的版本 请使用第三方JSON库替换此方法
//        //解析json数据，使用系统方法
//        NSDictionary* resultDict = [NSJSONSerialization JSONObjectWithData:_responseData
//                                                                   options:NSJSONReadingMutableLeaves
//                                                                     error:nil];
//        
//        //此处进行数据解析与wenView加载 同时启动计时器
//        if ([resultDict[@"promotion"] boolValue]) {
//            
//            NSURL *url = [NSURL URLWithString:resultDict[@"url"]];
//            NSURLRequest *request = [NSURLRequest requestWithURL:url];
//            [_myWeb loadRequest:request];
//        }
//    }
//    else
//    {
//        [self connection:connection didFailWithError:nil];
//    }
//}
//



#pragma mark --
#pragma mark methods
//获取 bundle id
- (NSString *)getBundleIdentifier
{
    // get bundle config file path
    NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"Info" ofType:@"plist"];
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithContentsOfFile:bundlePath];
    NSString *pbidentifier=[dict valueForKey:@"CFBundleIdentifier"];
    //return @"cn.domob.InterstitialAdView";
    return pbidentifier;
}


//关闭当前视图
- (void)closeAdAction
{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 5.0) {
        [self dismissViewControllerAnimated:YES completion:nil];
    }else{
        [self dismissModalViewControllerAnimated:YES];
    }
   
}
//加载广告
- (void)loadAdView
{
    [self performSelector:@selector(delayLoading) withObject:nil afterDelay:8];
}


#pragma mark --
#pragma mark UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{
    NSURL *url = request.URL;
    
    if (navigationType == UIWebViewNavigationTypeLinkClicked)
    {
        if ([[UIApplication sharedApplication] canOpenURL:url])
        {
            // Open safari
            [[UIApplication sharedApplication] openURL:url];
        }
        return NO;
    }
    
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    BWLOG(@"loadSuceed");
    loadSuceed = YES;
    UIWindow *window = [[[UIApplication sharedApplication] delegate] window];
    UIViewController *rootViewController = [window rootViewController];
    //UIViewController *rootViewController = g_explorerController_ios;
    
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
    {
        self.modalPresentationStyle = UIModalPresentationOverFullScreen;
        [rootViewController presentViewController:self animated:YES completion:nil];
    }
    else if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 5.0)
    {
        rootViewController.modalPresentationStyle = UIModalPresentationCurrentContext;
        [rootViewController presentViewController:self animated:YES completion:nil];
    }
    else
    {
        [rootViewController presentModalViewController:self animated:YES];
        //[rootViewController pushViewController:self animated:YES];
    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    BWLOG(@"loadFailed");
    if (!loadSuceed)
    {
        
    }
}


- (void)dealloc
{
//    self.responseData = nil;
//    self.urlConnection = nil;
    self.myWeb.delegate = nil;
    self.myWeb = nil;
    self.myBtn = nil;
    
    [super dealloc];
}
-(BOOL)prefersStatusBarHidden
{
    return YES;
}
- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    else
    {
        return UIInterfaceOrientationMaskLandscapeRight;
    }
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        return interfaceOrientation == UIInterfaceOrientationPortrait;
    }
    else
    {
        return interfaceOrientation == UIInterfaceOrientationLandscapeRight;
    }
}
@end

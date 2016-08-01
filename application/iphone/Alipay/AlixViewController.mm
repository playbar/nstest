//
//  AlixViewController.m
//  AlipaySdkDemo
//
//  Created by sky on 12/30/13.
//  Copyright (c) 2013 Alipay. All rights reserved.
//

#import "AlixViewController.h"
#import "AlixPaylib+extra.h"
#import "GGBase.h"
#import "BaiWanViewController_ios.h"
#import "UIViewController+popController.h"

extern iOSDeviceRelateData g_deviceData;
extern BaiWanViewController_ios* g_explorerController_ios;
@interface AlixViewController ()


@property (nonatomic, retain) NSURLRequest *request;
@property (assign, nonatomic) UIWebView *webView;

@property (assign, nonatomic) UIImageView *backgroundImageView;
@property (assign, nonatomic) UIButton *backBtn;
@property (assign, nonatomic) UIImageView *titleImageView;
@property (assign, nonatomic) UIButton *refreshBtn;

@property (nonatomic) BOOL backHome;
@property (assign, nonatomic) UIActivityIndicatorView *activityView;
//@property (assign, nonatomic) UILabel *errorLabel;
@property (nonatomic) BOOL isSuccess;

@end

@implementation AlixViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    _backHome = NO;
    self.view.backgroundColor = [UIColor whiteColor];
    _backgroundImageView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"Alipaytitle.png"]];
    _backgroundImageView.frame = CGRectMake(0, 0, g_deviceData.m_screenWidth, 44);
    [self.view addSubview:_backgroundImageView];
    [_backgroundImageView release];
    _titleImageView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"Alipaylogo.png"]];
    _titleImageView.frame = CGRectMake((g_deviceData.m_screenWidth - 76)/2, 9, 76, 25);
    [self.view addSubview:_titleImageView];
    [_titleImageView release];
    _backBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    _backBtn.frame = CGRectMake(20, 6, 51, 31);
    _backBtn.titleLabel.text = @"返回";
    _backBtn.titleLabel.textColor = [UIColor whiteColor];
    [_backBtn setTitle:@"返回" forState:UIControlStateNormal];
    [_backBtn setBackgroundImage:[UIImage imageNamed:@"Alipaynavbar_back.png"] forState:UIControlStateNormal];
    [_backBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_backBtn addTarget:self action:@selector(backCallback:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_backBtn];
    
    _refreshBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    _refreshBtn.frame = CGRectMake(g_deviceData.m_screenWidth - 51 -20, 6, 51, 31);
    _refreshBtn.titleLabel.text = @"刷新";
    _refreshBtn.titleLabel.textColor = [UIColor whiteColor];
    [_refreshBtn setBackgroundImage:[UIImage imageNamed:@"Alipaynavbar.png"] forState:UIControlStateNormal];
    [_refreshBtn setTitle:@"刷新" forState:UIControlStateNormal];
    [_refreshBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_refreshBtn addTarget:self action:@selector(refreshCallback:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_refreshBtn];
    _webView = [[UIWebView alloc]initWithFrame:CGRectMake(0, 44, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight - 44)];
    _webView.delegate = self;
    [self.view addSubview:_webView];
    _activityView = [[UIActivityIndicatorView alloc]initWithFrame:CGRectMake((g_deviceData.m_screenWidth  - 37)/2, (g_deviceData.m_screenHeight  - 37)/2, 37, 37)];
    [self.view addSubview:_activityView];
    _activityView.color = [UIColor blackColor];
    _activityView.hidesWhenStopped = YES;
    [_activityView startAnimating];
//    _errorLabel = [[UILabel alloc]initWithFrame:CGRectMake((g_deviceData.m_screenWidth  - 130)/2, (g_deviceData.m_screenHeight  - 21)/2, 130, 21)];
//    _errorLabel.text = @"网络连接失败";
//    _errorLabel.backgroundColor = [UIColor clearColor];
//    _errorLabel.textColor = [UIColor darkGrayColor];
//    _errorLabel.font = [UIFont systemFontOfSize:20];
//    _errorLabel.hidden = YES;
//    [self.view addSubview:_errorLabel];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notifitionRefreshAlixWebView:) name:kNotifitionRefreshAlixWebView object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notifitionErrorLoading:) name:kNotifitionErrorLoading object:nil];
    
    
}

- (void)dealloc
{
    [_webView release];
    [_activityView release];
    [super dealloc];
    
}

- (void)notifitionRefreshAlixWebView:(NSNotification *)notification
{
    
//    UIView *view = [[AlixPaylib shareLib] webViewController].view;
//    [self.view addSubview:view];
    
    [self reloadWebView];
}

- (void)notifitionErrorLoading:(NSNotification *)notification
{
//    _errorLabel.hidden = NO;
    [_activityView stopAnimating];
}

- (void)reloadWebView
{
    NSString *urlStr = [[AlixPaylib shareLib] openUrl];
    NSURL *url = [NSURL URLWithString:urlStr];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    //        _request = request;
    [_webView loadRequest:request];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    

    [_activityView startAnimating];
    
    NSString *urlStr = [[request URL] absoluteString];
    
   // NSLog(@"\n\n\nurl:\n%@", urlStr);
    
    NSRange foundObj=[urlStr rangeOfString:@"product_sign.htm?" options:NSCaseInsensitiveSearch];
    if (foundObj.length > 0)
    {
        _backHome = YES;
    }else if([urlStr rangeOfString:@"https://wapcashier.alipay.com/cashier/asyn_payment_result.htm?" options:NSCaseInsensitiveSearch].length > 0)
    {
        //NSLog(@"支付成功");
        
    }

    _request = request;

    
    
    
    
    
    return YES;
}
- (void)webViewDidStartLoad:(UIWebView *)webView
{
    
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
//    NSDictionary *dict = [_request allHTTPHeaderFields];
//    
//    NSString *str = [[NSString alloc] initWithData:[_request HTTPBody] encoding:NSUTF8StringEncoding];
//    
//    NSLog(@"\n\n\nHTTPBody:\n%@", str);
    
//    _errorLabel.hidden = YES;
    [_activityView stopAnimating];
    
}


- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    [_activityView stopAnimating];
    
//    _errorLabel.hidden = NO;
}

- (void)backCallback:(UIButton *)sender
{
    
//    if (_activityView.isAnimating)
//    {
//        return;
//    }
    [_activityView stopAnimating];
    if (_backHome)
    {
        _backHome = NO;
        [self reloadWebView];
    }else
    {
        [[NSNotificationCenter defaultCenter] removeObserver:self];
        [_webView stopLoading];
        _webView.delegate = nil;

        [self dismissViewControllerAnimated:YES completion:^{
            
        }];
    }
}
- (void)refreshCallback:(id)sender
{
    if (_activityView.isAnimating)
    {
        return;
    }
    [_webView reload];

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
@end

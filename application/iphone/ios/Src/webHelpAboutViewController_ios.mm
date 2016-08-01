//
//  webHelpAboutViewController.m
//  BaiWan
//
//  Created by imac1 on 13-10-23.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//

#import "webHelpAboutViewController_ios.h"
#import "BaiWanViewController_ios.h"
#import "ClientView_ios.h"
#include "GlobalPortingFunction.h"
#include "XString.h"
#include "GGBase.h"


extern iOSDeviceRelateData g_deviceData;
extern BaiWanViewController_ios* g_explorerController_ios;

@interface webHelpAboutViewController_ios ()

@end

@implementation webHelpAboutViewController_ios

-(id)initWithStyle:(WEB_PAGE_TYPE)pagetype
{
    
    m_pageType = pagetype;
    return [super init];
}

-(void)dealloc
{
    [baiwanInfoView release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    if (g_deviceData.m_isPad)
    {
        self.view.bounds = CGRectMake(0, 0, 320, 480);
    }
    else
    {
        self.view.bounds = CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight);
    }
    CGRect webViewBound;
    if (g_deviceData.m_isPad)
    {
        webViewBound.origin.x = 0;
        webViewBound.origin.y = 0;
        webViewBound.size.width = 320;
        webViewBound.size.height = 480;
    }
    else
    {
        webViewBound.origin.x = 0;
        webViewBound.origin.y = 44;
        webViewBound.size.width = g_deviceData.m_screenWidth;
        webViewBound.size.height = g_deviceData.m_screenHeight - 44;
    }
    
    if (m_pageType == WEB_PAGE_TYPE_HELP)
    {
        NSBundle* bundle = [NSBundle mainBundle];
        NSString* resPath = [bundle resourcePath];
        NSString* filePath;
        filePath = [resPath stringByAppendingFormat:@"%@",@"/help.html"];
        UIWebView * webView = [[[UIWebView alloc] initWithFrame:webViewBound]autorelease];
        [webView setBackgroundColor:[UIColor blackColor]];
        [webView setOpaque:NO];
        webView.alpha = 1;
        webView.scrollView.showsVerticalScrollIndicator = NO;
        [webView setUserInteractionEnabled:YES];
        [webView loadHTMLString:[NSString stringWithContentsOfFile:filePath encoding:NSUnicodeStringEncoding error:nil] baseURL:[bundle bundleURL]];
        
        [webView setDataDetectorTypes:UIDataDetectorTypePhoneNumber|UIDataDetectorTypeAddress];
        [self.view addSubview:webView];
    }
    else if (m_pageType == WEB_PAGE_TYPE_ABOUT)
    {
        UIImage *backGroundImag = [UIImage imageNamed:@"aboutus_back.png"];
        UIImageView *backgroundView = [[[UIImageView alloc]initWithImage:backGroundImag]autorelease];
        UIImage *logoImag = [UIImage imageNamed:@"aboutus_logo.png"];
        UIImageView *backgroundLogoView = [[[UIImageView alloc]initWithImage:logoImag]autorelease];
        if (g_deviceData.m_isPad || (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait))
        {
            backgroundView.frame = self.view.bounds;
        }
        else
        {
            backgroundView.frame = webViewBound;
        }
        CGRect backgroundFrame = self.view.bounds;
        [self.view addSubview:backgroundView];
        NSString *appCurVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
        
        UILabel * labelView1;
        UILabel * labelView2;
        UILabel * labelView3;
        UILabel * labelView4;
        int logoHeightLeft;
        
        if (g_deviceData.m_isPad || (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait))
        {
            labelView1 = [[[UILabel alloc]initWithFrame:CGRectMake(30, backgroundFrame.size.height - 172, backgroundFrame.size.width, 80)]autorelease];
            labelView2 = [[[UILabel alloc]initWithFrame:CGRectMake(100, backgroundFrame.size.height - 172, backgroundFrame.size.width, 80)]autorelease];
            labelView3 = [[[UILabel alloc]initWithFrame:CGRectMake(30, backgroundFrame.size.height - 172 + 72, backgroundFrame.size.width, 80)]autorelease];
            labelView4 = [[[UILabel alloc]initWithFrame:CGRectMake(100, backgroundFrame.size.height - 172 + 72, backgroundFrame.size.width, 80)]autorelease];
            logoHeightLeft = backgroundFrame.size.height - 172;
        }
        else
        {
            labelView1 = [[[UILabel alloc]initWithFrame:CGRectMake(20, backgroundFrame.size.height-100, webViewBound.size.width, 80)]autorelease];
            labelView2 = [[[UILabel alloc]initWithFrame:CGRectMake(90, backgroundFrame.size.height-100, webViewBound.size.width, 80)]autorelease];
            labelView3 = [[[UILabel alloc]initWithFrame:CGRectMake(webViewBound.size.width - 190, backgroundFrame.size.height-100, webViewBound.size.width, 80)]autorelease];
            labelView4 = [[[UILabel alloc]initWithFrame:CGRectMake(webViewBound.size.width - 120, backgroundFrame.size.height-100, webViewBound.size.width, 80)]autorelease];
            logoHeightLeft = backgroundFrame.size.height - 100;
        }
        
        logoHeightLeft = logoHeightLeft;
        backgroundLogoView.frame =  CGRectMake((backgroundFrame.size.width-logoHeightLeft*585/720)/2, (backgroundFrame.size.height-logoHeightLeft)/2, logoHeightLeft*585/720, logoHeightLeft);
        [self.view addSubview:backgroundLogoView];
        labelView1.text = @"软件版本：\n软件版权：\n百玩官网：\n官方微博：";
        labelView1.backgroundColor = [UIColor clearColor];
        labelView1.lineBreakMode = NSLineBreakByWordWrapping;
        labelView1.numberOfLines = 4;
        labelView1.font = [UIFont fontWithName:@"Helvetica-Bold" size:14];
        
        labelView2.text = [NSString stringWithFormat: @"%@ for IOS\n北京集趣信息技术有限责任公司\nhttp://www.playbyone.com\nhttp://weibo.com/playbyone", appCurVersion];
        labelView2.backgroundColor = [UIColor clearColor];
        labelView2.font = [UIFont fontWithName:@"Helvetica" size:14];
        labelView2.lineBreakMode = NSLineBreakByWordWrapping;
        labelView2.numberOfLines = 4;

        labelView3.text = @"客服热线：\n客服QQ：\n企业邮箱：\n企业电话：";
        labelView3.backgroundColor = [UIColor clearColor];
        labelView3.font = [UIFont fontWithName:@"Helvetica-Bold" size:14];
        labelView3.lineBreakMode = NSLineBreakByWordWrapping;
        labelView3.numberOfLines = 4;

        labelView4.text = @"010-88570330\n800090428\nhi@jqbar.com\n010-88570486";
        labelView4.backgroundColor = [UIColor clearColor];
        labelView4.font = [UIFont fontWithName:@"Helvetica" size:14];
        labelView4.lineBreakMode = NSLineBreakByWordWrapping;
        labelView4.numberOfLines = 4;

        [self.view addSubview:labelView1];
        [self.view addSubview:labelView2];
        [self.view addSubview:labelView3];
        [self.view addSubview:labelView4];
        UILongPressGestureRecognizer *longPressGR = [[[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(showBaiWanInfo)] autorelease];
        [longPressGR setNumberOfTouchesRequired:2];
        [self.view addGestureRecognizer:longPressGR];
        baiwanInfoView = [[UITextView alloc] initWithFrame:webViewBound];
        [baiwanInfoView setEditable:NO];
        [baiwanInfoView setFont:[UIFont systemFontOfSize:20]];
    }

    
    
    if (!g_deviceData.m_isPad)
    {
        CGRect naviBarBoundIphone;
        naviBarBoundIphone.origin.x = 0;
        naviBarBoundIphone.origin.y = 0;
        naviBarBoundIphone.size.width = g_deviceData.m_screenWidth;
        naviBarBoundIphone.size.height = 44;
        UINavigationBar *navBar = [[UINavigationBar alloc]initWithFrame:naviBarBoundIphone];
        UINavigationItem *barItem;
        if (m_pageType == WEB_PAGE_TYPE_ABOUT)
        {
            barItem = [[UINavigationItem alloc]initWithTitle:@"关于"];
        }
        else if (m_pageType == WEB_PAGE_TYPE_HELP)
        {
            barItem = [[UINavigationItem alloc]initWithTitle:@"帮助"];
        }
        
        UIBarButtonItem * barButtonItem = [[UIBarButtonItem alloc]initWithTitle:@"完成" style:UIBarButtonItemStyleDone target:self action:@selector(onDone)];
        [barItem setRightBarButtonItem:barButtonItem];
        [barButtonItem release];
        [navBar setItems:[NSArray arrayWithObject:barItem]];
        [barItem release];
        [self.view addSubview:navBar];
        [navBar release];
    }
}
- (void)viewWillAppear:(BOOL)animated
{
    if (baiwanInfoView.superview)
    {
        [baiwanInfoView removeFromSuperview];
        baiwanInfoViewIsShow = NO;
    }
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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
        return interfaceOrientation==UIInterfaceOrientationPortrait;
    }
    else
    {
        return interfaceOrientation==UIInterfaceOrientationLandscapeRight;
    }
}

- (void)onDone
{
    [self dismissModalViewControllerAnimated:YES];
}
- (void)showBaiWanInfo
{
    if (baiwanInfoViewIsShow)
        return;
    
    // 添加显示信息
    NSMutableString* message = [[NSMutableString alloc] init];
    void* mobileView = g_explorerController_ios.clientView.mobileView;
    [message appendFormat:@"%@\n%s\n",@"UA",g_getAppUA(mobileView)];
    const XString8Array* headers = (const XString8Array*)g_getAppHeaders(mobileView);
    for (int i=0;i<headers->GetSize();i++)
        [message appendFormat:@"%s\n",headers->operator[](i).GetData()];
    
    [baiwanInfoView setText:message];
    [message release];
    [self.view addSubview:baiwanInfoView];
    baiwanInfoViewIsShow = YES;
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}
@end

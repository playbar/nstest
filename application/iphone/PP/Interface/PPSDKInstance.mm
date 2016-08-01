//
//  SDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BWPPSDKMODE
#import "PPSDKInstance.h"



@implementation PPSDKInstance
@synthesize delegate;

-(void)dealloc
{
    [tapGestureRec release];
    [super dealloc];
}
-(void)initSDKWithDelegate:(id)delegate
{
    /**
     *必须写在程序window初始化之后。详情请commad + 鼠标左键 点击查看接口注释
     *初始化应用的AppId和AppKey。从开发者中心游戏列表获取（https://pay.25pp.com）
     *设置是否打印日志在控制台,[发布时请务必改为NO]
     *设置充值页面初始化金额,[必须为大于等于1的整数类型]
     *设置游戏客户端与游戏服务端链接方式是否为长连接【如果游戏服务端能主动与游戏客户端交互。例如发放道具则为长连接。此处设置影响充值并兑换的方式】
     *用户注销后是否自动push出登陆界面
     *是否开放充值页面【操作在按钮被弹窗】
     *若关闭充值响应的提示语
     *初始化SDK界面代码
     */
    
    
    [[PPAppPlatformKit sharedInstance] setAppId:1663 AppKey:@"b5806152ef20d5a108649d00ca7e6237"];
    [[PPAppPlatformKit sharedInstance] setIsNSlogData:NO]; // 发布时改为NO，打印调试信息
    [[PPAppPlatformKit sharedInstance] setRechargeAmount:10];
    [[PPAppPlatformKit sharedInstance] setIsLongComet:YES];
    [[PPAppPlatformKit sharedInstance] setIsLogOutPushLoginView:YES];
    [[PPAppPlatformKit sharedInstance] setIsOpenRecharge:YES]; //是否开启充值
    [[PPAppPlatformKit sharedInstance] setCloseRechargeAlertMessage:@"关闭充值提示语"];
    [[PPAppPlatformKit sharedInstance] setDelegate:self];
    
    
    
    
    self.delegate = delegate;
    isUIInitial = NO;
    tapGestureRec = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapDetected) ];
    [[self.delegate view] addGestureRecognizer:tapGestureRec];
    [self.delegate setCurrentBarIsValid:NO];
}

-(void)login
{
    BWLOG(@"PP Userid is %lld", [[PPAppPlatformKit sharedInstance] currentUserId]);
    if (!isUIInitial)
    {
        [PPUIKit sharedInstance];
        [PPUIKit setIsDeviceOrientationLandscapeLeft:YES];
        [PPUIKit setIsDeviceOrientationLandscapeRight:YES];
        [PPUIKit setIsDeviceOrientationPortrait:NO];
        [PPUIKit setIsDeviceOrientationPortraitUpsideDown:NO];
        isUIInitial = YES;
    }
    if ([[PPAppPlatformKit sharedInstance]currentUserId]) // 如果已经是登录状态 不在出现登录view
        return;
    
    [[PPAppPlatformKit sharedInstance] showLogin];
}
-(void)pay:(unsigned int)value
{
    // 拆分数据
    char* pidChar = (char*)value;
    if (!pidChar)
        return;
//    char a[20] = "1|1|1|1";
//    printf("%s\n",(char*)a);
//    printf("%s",(char*)value);
    NSString* pidString = [[NSString alloc] initWithCString:pidChar encoding:NSUTF8StringEncoding];
    NSArray* pidArray = [pidString componentsSeparatedByString:@"|"];
    if ([pidArray count]!=4)
    {
        [pidString release];
        return;
    }
    // 拆分完毕（商品价格｜商品订单号｜商品名称｜角色ID｜开发者后台配置的分区ID）
    // 开始购买
    
    double price = [[pidArray objectAtIndex:0] doubleValue];
    NSString *billNO = [pidArray objectAtIndex:1];
    NSString *billTitle = [pidArray objectAtIndex:2];
    NSString *roleID = [pidArray objectAtIndex:3];
//    int zoneID = [[pidArray objectAtIndex:4]integerValue];
    
    [[PPAppPlatformKit sharedInstance] exchangeGoods:price BillNo:billNO BillTitle:billTitle RoleId:roleID ZoneId:0];
}
-(void)showCenter
{
    [[PPAppPlatformKit sharedInstance] showCenter];
}

-(BOOL)handleAlixPayResult:(NSURL *)url
{
    [[PPAppPlatformKit sharedInstance] alixPayResult:url];
	return YES;
}
-(void)showFeedBackView
{

    UIView * feedbackView = [[UIView alloc]initWithFrame:CGRectMake(272, 203, 480, 331)];
    UIImage *imagBackground = [UIImage imageNamed:@"ppFeedBackBackground.png"];
    UIImage *imagButton = [UIImage imageNamed:@"ppFeedBackButton.png"];
    UIImageView *imagView = [[UIImageView alloc]initWithImage:imagBackground];
    imagView.frame = CGRectMake(0, 0, 480, 331);
    UIButton *btnConfirm = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    btnConfirm.frame = CGRectMake(204, 293, 73, 27);
    btnConfirm.titleLabel.text = @"确定";
    [btnConfirm setBackgroundImage:imagButton forState:UIControlStateNormal];
    [btnConfirm setTitle:@"确定" forState:UIControlStateNormal];
    [btnConfirm addTarget:self action:@selector(feedBackButtonPressed) forControlEvents:UIControlEventTouchUpInside];
    feedbackView.tag = 80;
    [feedbackView addSubview:imagView];
    [imagView release]; 
    [feedbackView addSubview:btnConfirm];

    // 动画
    feedbackView.alpha = 0;
    CGContextRef context = UIGraphicsGetCurrentContext();
    [UIView beginAnimations:@"fadeIn" context:context];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDuration:0.25];
    // 动画end
    feedbackView.alpha = 1;
    [[self.delegate view]addSubview:feedbackView];
    [feedbackView release];
    // 动画
    [UIView commitAnimations];
    // 动画end
}
-(void)feedBackButtonPressed
{
    // 动画
//    [[self.delegate view] viewWithTag:80].alpha = 1;
    CGContextRef context = UIGraphicsGetCurrentContext();
    [UIView beginAnimations:@"fadeIn" context:context];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
    [UIView setAnimationDuration:0.25];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(animationDidStop)];
    [[self.delegate view] viewWithTag:80].alpha = 0;
    [UIView commitAnimations];
    // 动画end
    
}
-(void)animationDidStop
{
    [[[self.delegate view] viewWithTag:80]removeFromSuperview];
}
#pragma mark    ---------------SDK CALLBACK---------------


//登录成功回调
-(void)ppLoginCallBack:(NSString *)paramPPToKenKey
{
    
    
    BWLOG(@"登录成功的回调是，touken是%@", paramPPToKenKey);
    //    MSG_GAME_SERVER mgs = {};
    //    mgs.len =  41;
    //    mgs.commmand = 0xAA000022;
    //    memcpy(mgs.token_key, [paramPPToKenKey UTF8String], 33);
    //    //下面请注意，登录验证分两种情况e
    //    //1. 如果您没有业务服务器则在此处直接跳入游戏界面
    //    //2. 如果您有游戏服务器，请在这里验证登录信息，然后跳转到游戏界面
    //    //下面代码为内部服务器测试代码
    //    int fd = socket( AF_INET , SOCK_STREAM , 0 ) ;
    //    if(fd == -1)
    //        printf("socket err : %m\n"),exit(1);
    //    struct sockaddr_in addr;
    //    addr.sin_family = AF_INET;
    //    addr.sin_port = htons([GAMESERVER_PORT_TEST intValue]);
    //    addr.sin_addr.s_addr = inet_addr(GAMESERVER_IP_TEST);
    //
    //    int r = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    //    if(r == -1)
    //        printf("connect err : %m\n"),exit(-1);
    //    send(fd, &mgs, sizeof(MSG_GAME_SERVER), 0);
    //    MSG_GAME_SERVER_RESPONSE mgsr;
    //    recv(fd, &mgsr, 12, 0);
    //    NSLog(@"%02X",mgsr.status);
    //    if(mgsr.status == 0){
    //        //跳入游戏界面
    //
    //    }
    
    
    
    [[PPAppPlatformKit sharedInstance] getUserInfoSecurity]; // 登录成功后必须调用
     NSMutableString* productURL = [[NSMutableString alloc] init];
    [productURL appendFormat:@"http://bwtest.jqbar.com/product/?outsid_pp=%@",paramPPToKenKey];
    [self.delegate loginSuccess:productURL];
    [productURL release]; 
    [[self.delegate view] removeGestureRecognizer:tapGestureRec];
    [self.delegate setCurrentBarIsValid:YES];
}

//关闭客户端页面回调方法
-(void)ppClosePageViewCallBack:(PPPageCode)paramPPPageCode{
    //可根据关闭的VIEW页面做你需要的业务处理
    BWLOG(@"当前关闭的VIEW页面回调是%d", paramPPPageCode);
    if(![[PPAppPlatformKit sharedInstance]currentUserId])
    {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"抱 歉" message:@"登录PP帐户才可继续游戏" delegate:self cancelButtonTitle:nil otherButtonTitles:@"确定",nil];
        [alert show];
        [alert release];
        
    }
}



//关闭WEB页面回调方法
- (void)ppCloseWebViewCallBack:(PPWebViewCode)paramPPWebViewCode{
    //可根据关闭的WEB页面做你需要的业务处理
    BWLOG(@"当前关闭的WEB页面回调是%d", paramPPWebViewCode);
}

//注销回调方法
- (void)ppLogOffCallBack
{
    [self.delegate logoff];
    [[self.delegate view] addGestureRecognizer:tapGestureRec];
    [self.delegate setCurrentBarIsValid:NO];
    BWLOG(@"注销的回调");
}

//兑换回调接口【只有兑换会执行此回调】
- (void)ppPayResultCallBack:(PPPayResultCode)paramPPPayResultCode{
    BWLOG(@"兑换回调返回编码%d",paramPPPayResultCode);
    //回调购买成功。其余都是失败
    if(paramPPPayResultCode == PPPayResultCodeSucceed)
    {
        //购买成功发放道具
        
    }else
    {
        
    }
}

-(void)ppVerifyingUpdatePassCallBack{
    BWLOG(@"验证游戏版本完毕回调");
    [[PPAppPlatformKit sharedInstance] showLogin];
}

#pragma mark      ---------------------ios supportedInterfaceOrientations -------------
//iOS 6.0旋屏开关
- (BOOL)shouldAutorotate
{
    return YES;
}


//iOS 6.0旋屏支持方向
- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskLandscape;
}


//iOS 6.0以下旋屏
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    if (UIInterfaceOrientationIsLandscape(interfaceOrientation))
    {
        return YES;
    }
    return NO;
}
#pragma mark      --------------------- alert view delegate -------------

//- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
//{
////    if (buttonIndex ==[alertView firstOtherButtonIndex])
//        [self.delegate loginViewIsClosedWithOutLogin];
//}
-(void)tapDetected
{
    [self login];
}
@end
#endif
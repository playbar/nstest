//
//  SDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BWUCSDKMODE
#import "UCSDKInstance.h"

//#if BWUCSDKMODE
//#define UCGAMESDK_FRAMEWORK

//#ifdef UCGAMESDK_FRAMEWORK
//#import <UCGameSdk/UCGameSdk.h>
//#import <UCGameSdk/UCGameSdkConst.h>
//#else
//#import "UCGameSdk.h"
//#import "UCGameSdkConst.h"
//#endif

//#import <UCGameSdk/UCGameSdk.h>
//
//#endif

#if BWUCSDKMODE
    #if BWUCSDKMODE
    // OK
    #else
    #error BWUCSDKMODE is OFF
    #endif
#endif

@implementation UCSDKInstance
@synthesize delegate;
-(void)initSDKWithDelegate:(id)delegate
{
    [[NSNotificationCenter defaultCenter]removeObserver:self];
    
    // 添加Observer来监听SDK初始化事件，以便在SDK初始化完成时进行后续处理
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(onSdkInitFin:) name:UCG_SDK_MSG_SDK_INIT_FIN object:nil];
    
    // 设置游戏相关参数
    UCGameSdk *sdk = [UCGameSdk defaultSDK];
    
    sdk.cpId = BWUCSDKCPID;                 // 设置游戏合作商编号
    sdk.gameId = BWUCSDKGAMEID;             // 设置游戏编号，该编号在游戏接入时由uc分配
    sdk.serverId = BWUCSDKSERVERID;         // 设置游戏服务器ID，该编号由所在的游戏分区决定
    sdk.isDebug = NO;                       // 设置联测模式或正是生产模式：YES 联测模式，SDK会自动链接到SDK平台的联测环境；NO 正是生产模式，SDK会自动链接到SDK平台的正是生产环境
    
    sdk.logLevel = UCLOG_LEVEL_ERR;     // 设置日志级别：UCGLOG_LEVEL_ERR:错误级
    //            UCGLOG_LEVEL_WARNING:警告级，包含错误级
    //            UCGLOG_LEVEL_DEBUG:调试级，包含以上两级
    
    [sdk initSDK];
    self.delegate = delegate;
}
-(void)login
{
    [[UCGameSdk defaultSDK] login];
    [[UCGameSdk defaultSDK] sid];
}
-(void)pay:(unsigned int)value
{
    // 拆分数据
    char* pidChar = (char*)value;
    if (!pidChar)
        return;
    NSString* pidString = [[NSString alloc] initWithCString:pidChar encoding:NSUTF8StringEncoding];
    NSArray* pidArray = [pidString componentsSeparatedByString:@"|"];
    if ([pidArray count]!=2)
    {
        [pidString release];
        return;
    }
    
    BWLOG(@"pidString = %@",[pidArray objectAtIndex:0]);
    
    // 开始购买
    // 设置当前serverId为需要充值的serverId。
    [UCGameSdk defaultSDK].serverId = BWUCSDKSERVERID;
    // 调用充值方法
    NSDictionary *paymenInfo = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES],@"allowContinuousPay",[NSString localizedStringWithFormat:[pidArray objectAtIndex:0]],@"customInfo",nil];
    [[UCGameSdk defaultSDK] payWithPaymentInfo: paymenInfo];

}
- (void)onSdkInitFin:(NSNotification *)notification
{
    // 读取初始化结果数据
    NSDictionary *res = notification.userInfo;
    NSNumber *code = (NSNumber *)[res objectForKey:@"code"];
    
    // 判断收到的code的值，确定SDK是否成功初始化
    if ([code intValue] == 1) {
        // code为1时表示SDK初始化失败，为0表示SDK初始化成功
        // 读取初始化错误信息
        NSString *errorInformation = [NSString stringWithFormat:@"SDK init result: code=%@, msg=%@",code,[res objectForKey:@"msg"]];
        return;
    }
    
    UCGameSdk *sdk = [UCGameSdk defaultSDK];
    
    
    // 添加Observer以监听SDK登录消息
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onLoginFin:) name:UCG_SDK_MSG_LOGIN_FIN object:nil];
    
    // 添加Observer以监听SDK未登录退出登录消息
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onSDKWithoutLoginExit:) name:UCG_SDK_MSG_EXIT_WITHOUT_LOGIN object:nil];
}

- (void)onLoginFin:(NSNotification*)notification
{
    // 读取登录结果数据
    NSDictionary* res = notification.userInfo;
    NSNumber* code = (NSNumber*)[res objectForKey:@"code"];
    
    // 判断收到的code值，确定SDK是否成功登录
    if ([code intValue] == 0) {
        // code为0时表示登录成功，为1表示登录失败
        
        //        [[UCGameSdk defaultSDK] sid];
        
        UCGameSdk* sdk = [UCGameSdk defaultSDK];
        
        sdk.cpId = BWUCSDKCPID;
        sdk.gameId = BWUCSDKGAMEID;
        sdk.serverId = BWUCSDKSERVERID;
        
        NSMutableString* productURL = [[NSMutableString alloc] init];
        [productURL appendFormat:@"http://m.jqbar.com/product/?outuid_UC=%@&cpid_uc=%d&gameid_uc=%d&serverid_uc=%d",[sdk sid],[sdk cpId],[sdk gameId],[sdk serverId]];
        
        BWLOG(@"sid = %@",[sdk sid]);
        [self.delegate loginSuccess:productURL];

//        g_mobileViewConnect(clientView.mobileView, [productURL cStringUsingEncoding:NSUTF8StringEncoding]);
        
//        isLogin = YES;
        
        // 添加Observer以监听SDK充值请求消息
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onPayFin:) name:UCG_SDK_MSG_PAY_FIN object:nil];
    }else {
        // 读取登录失败错误信息
        NSString *errorInformation = [NSString stringWithFormat:@"SDK login failed: code=%@, msg=%@",code,[res objectForKey:@"msg"]];
        return;
    }
}

- (void)onSDKWithoutLoginExit:(NSNotification*)notification
{
    // 调用此函数即代表用户在未登录的情况下退出了登录界面；
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"登录UC帐户才可继续游戏" delegate:self.delegate cancelButtonTitle:nil otherButtonTitles:@"确定",nil];
    [alert setTag:EALERTVIEWMODE_THIRDPARTSDK];
    [alert show];
    [alert release];
}

- (void)onPayFin:(NSNotification *)notification
{
    //    NSDictionary *res = notification.userInfo;
    //    NSNumber *code = (NSNumber*)[res objectForKey:@"code"];
    //
    //    // 判断收到的code值，确定SDK是否成功充值
    //    if ([code intValue] == 0) {
    //        // code为0时表示充值成功，为1表示充值失败
    //
    //        //读取充值结果
    //        NSDictionary *data = [res objectForKey:@"data"];
    //        NSString *orderId = [data objectForKey:@"orderId"];
    //        double payAmount = [[data objectForKey:@"amount"]doubleValue];
    //        NSString *payWayName = [data objectForKey:@"payWayName"];
    //
    //        NSString *payInformation = [NSString stringWithFormat:@"充值成功，订单号为：%@，金额：%d，充值方式：%@", orderId, payAmount, payWayName];
    //    }else {
    //        // 读取充值错误信息
    //        NSString *errorInformation = [NSString stringWithFormat:@"SDK pay failed:code=%@, msg=%@", code, [res objectForKey:@"msg"]];
    //        return;
    //    }
}
@end
#endif


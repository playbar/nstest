//
//  SDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BW91SDKMODE
#import "_91SDKInstance.h"
#import "GlobalPortingFunction.h"
#import <NdComPlatform/NDComPlatform.h>
#import <NdComPlatform/NdComPlatformAPIResponse.h>
#import <NdComPlatform/NdCPNotifications.h>
//#endif
// 定义91的id和key
//#if BW91SDKMODE
#define BW91SDKID       102324
#define BW91SDKKEY      @"9d65abe08158006a8b046eec269d81b500b2f15918d626d5"



@implementation _91SDKInstance
@synthesize delegate;
-(void)initSDKWithDelegate:(id)delegate
{
    [[NdComPlatform defaultPlatform] setAppId:BW91SDKID];
    [[NdComPlatform defaultPlatform] setAppKey:BW91SDKKEY];
    self.delegate = delegate;
}
-(void)login
{
    if ([[NdComPlatform defaultPlatform] isLogined])
        return;
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(loginOK91SDK:)
                                                 name:(NSString *)kNdCPLoginNotification
                                               object:nil];
    [[NdComPlatform defaultPlatform] NdLogout:1];
    [[NdComPlatform defaultPlatform] NdLogin:0];
    [[NdComPlatform defaultPlatform] NdSetDebugMode:0];
}
-(void)pay:(unsigned int)value
{
    // 拆分数据
    char* pidChar = (char*)value;
    if (!pidChar)
        return;
    NSString* pidString = [[NSString alloc] initWithCString:pidChar encoding:NSUTF8StringEncoding];
    NSArray* pidArray = [pidString componentsSeparatedByString:@"|"];
    if ([pidArray count]!=4)
    {
        [pidString release];
        return;
    }
    // 拆分完毕（产品ID｜订单ID｜产品名称｜产品价格）
    // 开始购买
    NdBuyInfo* buyInfo = [[NdBuyInfo new] autorelease];
    buyInfo.cooOrderSerial = [pidArray objectAtIndex:1];
    buyInfo.productId = [pidArray objectAtIndex:0];
    buyInfo.productName = [pidArray objectAtIndex:2];
    buyInfo.productPrice = [[pidArray objectAtIndex:3] floatValue];
    buyInfo.productOrignalPrice = [[pidArray objectAtIndex:3] floatValue];
    buyInfo.productCount = 1;
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(NdUiPayAsynResult:) name:kNdCPBuyResultNotification object:nil];
    
    int res = [[NdComPlatform defaultPlatform] NdUniPayAsyn:buyInfo];
    if (res<0)
    {
        // 参数不正确，直接取消交易
        [[NSNotificationCenter defaultCenter] removeObserver:self name:kNdCPBuyResultNotification object:nil];
    }
    
}

#pragma mark - ---- 91 SDK delegate ------------------
- (void)loginOK91SDK:(NSNotification*)notification
{
    NSDictionary *dict = [notification userInfo];
    BOOL success = [[dict objectForKey:@"result"] boolValue];
    int error = [[dict objectForKey:@"error"] intValue];
    if (error == ND_COM_PLATFORM_ERROR_USER_CANCEL)
    {
        //用户取消登录和和相应处理
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"错误" message:@"登录91帐户才可继续游戏" delegate:self.delegate cancelButtonTitle:nil otherButtonTitles:@"确定",nil];
        [alert setTag:EALERTVIEWMODE_THIRDPARTSDK];
        [alert show];
        [alert release];
    }
    if([[NdComPlatform defaultPlatform] isLogined] && success)
    {
        //用户登录成功后相应操作
        // 移出监听
        [[NSNotificationCenter defaultCenter] removeObserver:self name:(NSString*)kNdCPLoginNotification object:nil];
        // 提示登录成功
        NdMyUserInfo *info = [[NdComPlatform defaultPlatform] NdGetMyInfo];
        if (g_enc91UID([self.delegate clientView].mobileView, [info.baseInfo.uin cStringUsingEncoding:NSUTF8StringEncoding]))
        {
            NSString* strContext = [[NSString alloc] initWithCString:g_getEnc91UID([self.delegate clientView].mobileView) encoding:NSUTF8StringEncoding];
            NSArray* strContextArray = [strContext componentsSeparatedByString:@"|"];
            NSMutableString* productURL = [[NSMutableString alloc] init];
            [productURL appendFormat:@"http://m.jqbar.com/product/?outuid=%@&t=%@&m=%@",[strContextArray objectAtIndex:0],[strContextArray objectAtIndex:1],[strContextArray objectAtIndex:2]];
            [self.delegate loginSuccess:productURL];
//            g_mobileViewConnect(clientView.mobileView, [productURL cStringUsingEncoding:NSUTF8StringEncoding]);
        }
    }
    else if (error != ND_COM_PLATFORM_ERROR_USER_CANCEL)
    {
        //用户登录失败提示和处理
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"错误" message:@"登录失败，请重新尝试" delegate:self.delegate cancelButtonTitle:nil otherButtonTitles:@"确定",nil];
        [alert setTag:EALERTVIEWMODE_THIRDPARTSDK];
        [alert show];
        [alert release];
    }
}

- (void)NdUiPayAsynResult:(NSNotification*)notify
{
    
}

@end
#endif

//
//  SDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWYMSDKMODE
#import "YouMiSDKInstance.h"



@implementation YMSDKInstance

-(void)dealloc
{
    [super dealloc];
}
-(void)initOfferWallWithDelegate:(id)dele
{
    self.delegate = dele;
}
-(void)showOfferWallWithAppID:(NSString *)appID andAppSec:(NSString *)appSec andUserID:(NSString *)userID
{
    //[YouMiConfig setUserID:userID];
    // 替换下面的appID和appSecret为你的appid和appSecret
    [YouMiConfig launchWithAppID:@"6191437ca20f2a14" appSecret:@"45e2b6f1f2a6ef2b"];
    [YouMiConfig setUseInAppStore:YES];
    // 开启积分管理[本例子使用自动管理];
    //[YouMiPointsManager enable];
    // 开启积分墙
    [YouMiWall enable];
    [YouMiConfig setFullScreenWindow:[[[UIApplication sharedApplication] delegate] window]];
    [YouMiWall showOffers:YES didShowBlock:^{} didDismissBlock:^{}];
}
-(void)checkPointWithUserID:(NSString *)userID
{
    
}
@end
//#endif
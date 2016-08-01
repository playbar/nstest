//
//  SDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWLMSDKMODE
#import "LiMeiSDKInstance.h"



@implementation LiMeiSDKInstance

-(void)dealloc
{
    [adView_AdWall release];
    [super dealloc];
}
-(void)initOfferWallWithDelegate:(id)dele
{
    self.delegate = dele;
}
-(void)showOfferWallWithAppID:(NSString *)appID andAppSec:(NSString *)appSec andUserID:(NSString *)userID
{
    adView_AdWall=[[immobView alloc] initWithAdUnitID:@"bf878b7813d5095cc561980089d92163"];
    adView_AdWall.delegate=self;
    [adView_AdWall immobViewRequest];
}
-(void)checkPointWithUserID:(NSString *)userID
{
    
}

-(UIViewController *)immobViewController
{
    return [[[[UIApplication sharedApplication] delegate] window] rootViewController];
}
-(void)immobViewDidReceiveAd:(immobView *)immobView
{
    [[self.delegate view] addSubview:adView_AdWall];
    [adView_AdWall immobViewDisplay];
}
@end
//#endif
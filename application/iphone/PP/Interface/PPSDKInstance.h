//
//  SDKInstance.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BWPPSDKMODE
#import <Foundation/Foundation.h>
#import "ThirdPartSDKProtocol.h"
#import <PPAppPlatformKit/PPAppPlatformKit.h>

@interface PPSDKInstance : NSObject<ThirdPartSDKProtocol, PPAppPlatformKitDelegate>//, UIAlertViewDelegate>
{
    UITapGestureRecognizer *tapGestureRec;
    BOOL isUIInitial;
}
@property (assign) id delegate;
-(BOOL)handleAlixPayResult:(NSURL *)url;
-(void)feedBackButtonPressed;
@end
#endif
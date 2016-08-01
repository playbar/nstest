//
//  SDKInstance.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWLMSDKMODE
#import <Foundation/Foundation.h>
#import "ThirdPartSDKProtocol.h"
#import <immobSDK/immobView.h>

@interface LiMeiSDKInstance : NSObject<ThirdPartOfferWallSDKProtocol,immobViewDelegate>
{
    immobView *adView_AdWall;
}
@property (assign) id delegate;
@end
//#endif
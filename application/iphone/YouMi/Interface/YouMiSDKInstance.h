//
//  SDKInstance.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWYMSDKMODE
#import <Foundation/Foundation.h>
#import "ThirdPartSDKProtocol.h"
#import "YouMiConfig.h"
#import "YouMiPointsManager.h"
#import "YouMiWall.h"
#import "YouMiWallAppModel.h"

@interface YMSDKInstance : NSObject<ThirdPartOfferWallSDKProtocol>
{
}
@property (assign) id delegate;
@end
//#endif
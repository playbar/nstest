//
//  SDKInstance.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWMBSDKMODE
#import <Foundation/Foundation.h>
#import "ThirdPartSDKProtocol.h"
#import "MobiSageOfferWallViewController.h"

@interface  MobisageSDKInstance : NSObject<ThirdPartOfferWallSDKProtocol, MobiSageOfferWallDelegate>
{
}
@property (assign) id delegate;
@end
//#endif
//
//  SDKInstance.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWDMSDKMODE
#import <Foundation/Foundation.h>
#import "ThirdPartSDKProtocol.h"
#import "DMOfferWallViewController.h"
#import "DMOfferWallManager.h"

@interface DomobSDKInstance : NSObject<ThirdPartOfferWallSDKProtocol,
                                        DMOfferWallDelegate,
                                        DMOfferWallManagerDelegate>
{
    DMOfferWallManager *_offerWallManager;
    DMOfferWallViewController *_offerWallController;
}
@property (assign) id delegate;
@end
//#endif
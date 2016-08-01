//
//  SDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWDMSDKMODE
#import "DomobSDKInstance.h"



@implementation DomobSDKInstance

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
    _offerWallManager = [[DMOfferWallManager alloc] initWithPublishId:@"96ZJ1NrQzeg1TwTAJy" userId:userID];
    _offerWallManager.delegate = self;
    _offerWallController = [[DMOfferWallViewController alloc] initWithPublisherID:@"96ZJ1NrQzeg1TwTAJy" andUserID:userID];
    // !!!:重要：如果需要禁用应用内下载，请将此值设置为YES。
    _offerWallController.disableStoreKit = NO;
    _offerWallController.delegate = self;
    [_offerWallManager requestOnlinePointCheck];

    [_offerWallController presentOfferWall];
}
-(void)checkPointWithUserID:(NSString *)userID
{
    
}
// 积分查询成功之后，回调该接口，获取总积分和总已消费积分。
- (void)offerWallDidFinishCheckPointWithTotalPoint:(NSInteger)totalPoint
                             andTotalConsumedPoint:(NSInteger)consumed {
    BWLOG(@"offerWallDidFinishCheckPoint:%d", totalPoint);
    BWLOG(@"offerWallDidFinishCheckConsumedPoint:%d", consumed);
}
@end
//#endif
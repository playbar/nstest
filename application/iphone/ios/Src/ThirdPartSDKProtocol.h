//
//  TheThirdPartSDKProtocol.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol ThirdPartSDKProtocol <NSObject>
@required
-(void)initSDKWithDelegate:(id)delegate;
-(void)login;
-(void)pay:(unsigned int)value;
-(void)showCenter;
-(void)showFeedBackView;
@end


@protocol ThirdPartSDKDelegateProtocol <NSObject>

@required
-(void)logoff;
-(void)loginSuccess:(NSString *)loginToken;
-(void)loginViewIsClosedWithOutLogin;
-(void)setCurrentBarIsValid:(BOOL)isValid;
@end

@protocol ThirdPartOfferWallSDKProtocol <NSObject>
@required
-(void)initOfferWallWithDelegate:(id)dele; // 查询积分必须设置代理
-(void)showOfferWallWithAppID:(NSString *)appID andAppSec:(NSString *)appSec andUserID:(NSString *)userID;
-(void)checkPointWithUserID:(NSString *)userID;
@end

@protocol ThirdPartOfferWallSDKDelegateProtocol <NSObject>
@required
-(void)getOfferWallPoint:(float)point;
@end
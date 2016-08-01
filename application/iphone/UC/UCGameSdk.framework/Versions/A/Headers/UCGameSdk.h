//
//  UCGameSdk.h
//  
//  本文件主要包含两部分声明：
//  1 sdk入口api的定义 UCGameSdk
//  2 部分API回调的协议 UCGameSdkUIProtocol
//
//  Created by Zhiyong Lin on 12年2月29日.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "UCGameSdkConst.h"
#import "UCGameSdkResponse.h"

@protocol UCGameSdkProtocol

@optional
/**
 @brief getUCZoneFriendList的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。
 @param pageIndex  页索引, 索引从1开始
 @param pageSize 每页好友个数
 @param friendList 应用好友信息列表
 */
- (void)getUCZoneFriendListDidFinish:(int)error pageIndex: (int) pageIndex pageSize: (int)pageSize friendList:(UCFriendList *)friendList;

/**
 @brief 游戏官方帐号登录校验，在游戏方支持官方帐号登录，即UCGameSdk类中的allowGameUserLogin=YES时，必须实现此函数
 @param gameUser 游戏官方帐号
 @param gamePassword  游戏官方帐号密码
 @return 如果校验通过，返回的字典中包括处理结果标识 resultCode，处理结果描述 resultMsg，如果校验通过的话，还会有会话标识 sid；
         其中resultCode=0表示校验通过，此时需要将SDK Server返回的sid一并返回给SDK；
         其他非0情况则代表校验不通过，而resultMsg 则是结果的字符描述，在校验不通过的情况下，会将resultMsg显示到对话框中提示用户，请确保此resultMsg友好，并能正确的提示用户.
 */
- (NSDictionary *)verifyGameUser:(NSString *)gameUser gamePassword: (NSString *)gamePassword;

/**
 @brief 获取当前ucid是否是会员vip
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。
 @param status 会员状态，当error值为0时，通过此值可以知道当前ucid是否会员。0，不是会员；1，是会员
 */
- (void)isVipDidFinish:(int)error isVip:(int)status;

/**
 @brief 获取当前用户的特权信息
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。
 @param vipInfo 会员特权信息
 */
- (void)getVipInfoDidFinish:(int) error vipInfo:(VipInfo *)vipInfo;

@end


@interface UCGameSdk : NSObject

/*
 cp在UC标识的ID，由UC分配 
 */
@property(assign, nonatomic) int cpId;  

/*
 游戏在UC标识的ID，由UC分配
 */
@property(assign, nonatomic) int gameId;

/* 
 游戏的区服标识，如需改变此值，只需直接设置即可起作用，不必再次初始化 
 */
@property(assign, nonatomic) int serverId;

/* 
 游戏的渠道标识
 */
@property(retain, readonly, nonatomic) NSString *channelId;

/* 
 是否允许官方帐号登录，如置成YES，请务必实现 UCGameSdkProtocol 协议中的- (NSDictionary *)verifyGameUser:gamePassword:方法 
 */
@property(assign, nonatomic) BOOL allowGameUserLogin;

/*
 游戏官方帐号名称，如果设置此参数，在SDK的登录界面中，将使用此参数显示，如可设置此参数为“三国号”，如果未设置此参数，则默认使用“游戏官方帐号”显示 
 */
@property(retain, nonatomic) NSString *gameUserName; 

/* 
 是否为调试模式，如果置为YES，代表是调试模式，将连接SDK Server的测试环境进行调试；置为NO，表示正式运营模式，将连接SDK Server的正式环境；请确保此参数的设置与游戏服务器连接的SDK Server的环境保持一致 
 */
@property(assign, nonatomic) BOOL isDebug;

/* 
 控制sdk自身日志的输出，范围从大到小：UCLOG_LEVEL_DEBUG > UCLOG_LEVEL_WARNING > UCLOG_LEVEL_ERR，具体的取值说明可参见 UCGameSdkConst.h 中的 UCLogLevel 类型
 */
@property(assign, nonatomic) UCLogLevel logLevel;

/*
 exInfo 个人中心消息Info类，现其中的属性包括:
 cpServiceContact:NSString cp服务客服电话，使用“\n”进行还行，可传类似的文字：三国争霸\n客服电话：020-1234567
 */
@property(retain, nonatomic) ExInfo *exInfo;

/**
 @brief 获取UCGameSdk的实例对象
 */
+ (UCGameSdk *)defaultSDK;

/**
 *@brief 初始化sdk.
 */
- (void) initSDK;

/**
 *@brief 获取游戏参数.
 */
- (NSDictionary *) getGameParams;

/**
 *@brief 获取当前会话的session id.
 */
- (NSString *) sid;

#pragma mark - ui

/**
 *@brief 进入登录界面，默认不支持官方帐号登录
 */
- (void) login;

/**
 *@brief 进入登录界面，如果支持官方帐号登陆，请调用此登录方法，官方帐号的校验会通过调用delegate中的- verifyGameUser：gamePassword:方法进行校验，请确保此方法正确的实现
 */
- (void) loginWithDelegate:(id<UCGameSdkProtocol>)delegate;

/**
 @brief 进入支付页面，并带上所需的参数
 @param paymentInfo 可定制性字典，现支持的参数包括:
        allowContinuousPay:BOOL 是否支持连续充值功能，默认为YES，即支持连续充值
        customInfo:NSString 游戏合作商自定义参数
        roleId:NSString 游戏角色id
        roleName:NSString 游戏角色名
        grade:NSString 角色等级
 @result 暂无，支付成功后，订单信息将通过NSNotification进行通知；
        而在用户退出支付界面时，也会发起NSNotification通知，具体请参考文档
 */
- (void) payWithPaymentInfo:(NSDictionary *) paymentInfo;


/**
 @brief 进入个人管理界面
 @result 暂无，在用户退出个人管理界面时，会发起NSNotification通知，具体请参考文档
 */
- (void)enterUserCenter;


#pragma mark - sns api
/**
 @brief 获取UC小乐园好友
 @param pageIndex  页索引, 索引从1开始
 @param pageSize 每页好友个数
 @param type  好友关系 目前仅有1 表示为熟人
 @param delegate 回调对象，回调接口参见 UCGameSdkProtocol
 @result 暂固定返回0，方便以后进行扩展
 */
- (int)getUCZoneFriendList:(int)pageIndex pageSize:(int)pageSize type:(int)type delegate:(id<UCGameSdkProtocol>)delegate;

#pragma - vip api

/**
 @brief 获取该帐号是否为uc会员
 @param delegate 回调对象，回调接口参见 UCGameSdkProtocol
 @result 暂固定返回0，方便以后进行扩展
 */
- (int) isVipWithDelegate:(id<UCGameSdkProtocol>)delegate;

/**
 @brief 获取该帐号的uc会员信息
 @param delegate 回调对象，回调接口参见 UCGameSdkProtocol
 @result 暂固定返回0，方便以后进行扩展
 */
- (int) getVipInfoWithDelegate:(id<UCGameSdkProtocol>)delegate;
@end





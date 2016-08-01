//
//  UCGameSdkAPIResponse.h
//  UCSdk_iOS
//
// 本文件包含了"UCGameSdk.h"的api回调协议UCGameSdkUIProtocol里
// 数据类的声明。
//
//  Created by 陈振华 on 12-3-8.
//  Modified by liangyc on 12-5-21
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 @brief 好友信息
 */
@interface UCFriendInfo : NSObject 
{
    int ucid;
}

/*
 ucid:UC帐号标识
 */
@property (nonatomic, assign) int ucid;	


@end

/**
 @brief 查找到的好友信息列表
 @note  entityList存放的是UCFriendInfo类型对象
 */

@interface UCFriendList : NSObject {
    int	totalCount;
    NSArray* entityList;
}

/*
 用户好友总数
 */
@property (nonatomic, assign) int totalCount;	

/*
 当次获取的用户某页好友列表 
 */
@property (nonatomic, retain) NSArray* entityList;  

@end

/**
 @brief 特权信息
 每个PrivilegeInfo包括了以下的属性:
 (1)	enjoy: 1代表能能享受；0代表不能享受
 (2)	pId: 特权ID
 */
@interface PrivilegeInfo : NSObject {
    int enjoy;
    NSString* pId;
}
/*
 enjoy: 1代表能能享受；0代表不能享受
 */
@property (nonatomic, assign) int enjoy;

/*
 pId: 特权ID
 */
@property (nonatomic, retain) NSString* pId;

@end


/**
 @brief 会员特定信息
 */
@interface VipInfo : NSObject

/*
 会员的状态标识：0 表示非会员、1 表示有效会员、2 表示过期会员
 */
@property (nonatomic, assign) int status;

/*
 会员等级，0表示非会员，1表示一级会员，2表示2级，目前只有一级会员
 */
@property (nonatomic, assign) int grade;

/*
 会员身份有效期开始时间格式为：yyyy-MM-dd HH:mm:ss，如果该用户为非会员，此字段为nil
 */
@property (nonatomic, retain) NSString* validFrom;

/*
 会员身份有效期结束时间格式为：yyyy-MM-dd HH:mm:ss，如果该用户为非会员，此字段为nil
 */
@property (nonatomic, retain) NSString* validTo;

/*
 是一个PrivilegeInfo类型的数组
 */
@property (nonatomic, retain) NSArray* privilegeList; 

@end


/**
 @brief 个人中心信息
 */
@interface ExInfo : NSObject

/*
 设置游戏合作商客服联系方式，用于在意见反馈界面中显示，如需换行使用“\n”分隔。如：“三国时代\n客服电话:020-12345678”
 */
@property (nonatomic, retain) NSString *cpServiceContact;

@end




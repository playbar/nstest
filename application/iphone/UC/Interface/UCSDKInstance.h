//
//  SDKInstance.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BWUCSDKMODE
#import <Foundation/Foundation.h>
#import "ThirdPartSDKProtocol.h"
// 定义UC的cpId,gameId,serverId
#define BWUCSDKCPID      760        // 设置游戏合作商编号
#define BWUCSDKGAMEID    104749     // 设置游戏编号，该编号在游戏接入时由uc分配
#define BWUCSDKSERVERID  1189       // 设置游戏服务器ID，该编号由所在的游戏分区决定

@interface UCSDKInstance : NSObject<ThirdPartSDKProtocol>
{
    id delegate;
}
@property id delegate;
@end
#endif
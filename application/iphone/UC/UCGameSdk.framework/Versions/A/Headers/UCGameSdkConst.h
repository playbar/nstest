//
//  UCGameSdkConsts.h
//  UCSdk_iOS
//
//  Created by Zhiyong Lin on 12年3月12日.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef UCSdk_iOS_UCGameSdkConsts_h
#define UCSdk_iOS_UCGameSdkConsts_h

/*
 SDK 初始化 NSNotification name，在 SDK 初始化完成后进行通知 
 */
#define UCG_SDK_MSG_SDK_INIT_FIN @"ucg_sdk_msg_init_fin"  

/*
 用户登陆 NSNotification name，在用户登陆或者用户完成注册点击“马上加入”后进行通知
 */
#define UCG_SDK_MSG_LOGIN_FIN @"ucg_sdk_msg_login_fin"  

/*
 用户支付 NSNotification name，在用户完成充值后进行通知 
 */
#define UCG_SDK_MSG_PAY_FIN @"ucg_sdk_msg_pay_fin"

/*
 SDK 未登录退出 NSNotification name，在SDK退出后进行通知
 */
#define UCG_SDK_MSG_EXIT_WITHOUT_LOGIN @"ucg_sdk_msg_exit_without_login"

/*
 SDK 支付界面退出 NSNotification name，在游戏调用SDK支付界面，退出支付界面后进行通知
 */
#define UCG_SDK_MSG_PAY_EXIT @"ucg_sdk_msg_pay_exit"

/*
 SDK 个人中心界面退出 NSNotification name，在游戏调用SDK个人中界面，退出个人中心界面后进行通知
 */
#define UCG_SDK_MSG_USER_CENTER_EXIT @"ucg_sdk_msg_user_center_exit"

/* 
控制sdk自身日志的输出，范围从大到小：UCLOG_LEVEL_DEBUG > UCLOG_LEVEL_WARNING > UCLOG_LEVEL_ERR
UCLOG_LEVEL_DEBUG：debug日志，warning日志，error日志都会输出
UCLOG_LEVEL_WARNING：忽略debug日志，只输出warning及error日志
UCLOG_LEVEL_ERR：忽略debug及warning日志，只输出error日志
*/
typedef enum {
    UCLOG_LEVEL_ERR = 0,
    UCLOG_LEVEL_WARNING = 1,
    UCLOG_LEVEL_DEBUG = 2
} UCLogLevel;

#endif

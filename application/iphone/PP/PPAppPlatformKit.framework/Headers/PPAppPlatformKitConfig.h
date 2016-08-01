//
//  PPAppPlatformKitConfig.h
//  PPAppPlatformKit
//
//  Created by 张熙文 on 1/11/13.
//  Copyright (c) 2013 张熙文. All rights reserved.
//

#import <Foundation/Foundation.h>



typedef struct MsgGameServerResponse
{
    uint32_t len;
    uint32_t command;
    uint32_t status;                //0为成功，其他为失败
}MSG_GAME_SERVER_RESPONSE;


typedef struct MsgGameServer{
    uint32_t		len;
    uint32_t		commmand;           //[0xAA000021]
    char            token_key[33];
}MSG_GAME_SERVER;


#pragma mark ------------------------ SDK发送给游戏客户端通知 ------------------------------





/**
 * @brief PPWeb页面关闭回调参数
 */
typedef enum{
    /**
     * 返回充值页面
     */
	PPWebViewCodeRecharge = 1,
    /**
     * 返回充值记录页面
     */
    PPWebViewCodeRechargeRecord = 2,
    /**
     * 返回兑换记录页面
     */
    PPWebViewCodeExpendRecord = 3,
    /**
     * 返回帮助页面
     */
    PPWebViewCodeHelp = 4,
    /**
     * 返回充值并且兑换页面
     */
    PPWebViewCodeRechargeAndExchange = 5,
    /**
     * 返回密保页面
     */
    PPWebViewCodeUserInfoSecurity = 6,
    /**
     * 返回论坛
     */
    PPWebViewCodeBBS = 7,
    /**
     * 返回忘记密码
     */
    PPWebViewCodeForgetPassWord = 8
}PPWebViewCode;


/**
 * @brief 错误范围，用来标识错误是接口返回的还是SDK返回的。
 */
typedef enum{
    /**
     * 关闭接口为登录页面
     */
	PPLoginViewPageCode	= 1,
    /**
     * 关闭接口为注册
     */
    PPRegisterViewPageCode = 2,
    /**
     * 关闭接口为PP中心
     */
    PPCenterViewPageCode = 3,
    /**
     * 关闭接口为修改密码
     */
    PPUpdatePwdViewPageCode = 4,
    /**
     * 关闭接口为安全提示
     */
    PPAlertSecurityViewPageCode = 5,
}PPPageCode;


/**
 * @brief 错误范围，返回兑换结果。
 */
typedef enum{
    /**
     * 购买成功
     */
	PPPayResultCodeSucceed	= 0,
    /**
     * 禁止访问
     */
    PPPayResultCodeForbidden = 1,
    /**
     * 该用户不存在
     */
    PPPayResultCodeUserNotExist = 2,
    /**
     * 必选参数丢失
     */
    PPPayResultCodeParamLost = 3,
    /**
     * PP币余额不足
     */
    PPPayResultCodeNotSufficientFunds = 4,
    /**
     * 该游戏数据不存在
     */
    PPPayResultCodeGameDataNotExist = 5,
    /**
     * 开发者数据不存在
     */
    PPPayResultCodeDeveloperNotExist = 6,
    /**
     * 该区数据不存在
     */
    PPPayResultCodeZoneNotExist = 7,
    /**
     * 系统错误
     */
    PPPayResultCodeSystemError = 8,
    /**
     * 购买失败
     */
    PPPayResultCodeFail = 9,
    /**
     * 与开发商服务器通信失败，如果长时间未收到商品请联系PP客服：电话：020-38276673　 QQ：800055602
     */
    PPPayResultCodeCommunicationFail = 10,
    /**
     * 开发商服务器未成功处理该订单，如果长时间未收到商品请联系PP客服：电话：020-38276673　 QQ：800055602
     */
    PPPayResultCodeUntreatedBillNo = 11,
    /**
     * 非法访问，可能用户已经下线
     */
    PPPayResultCodeUserOffLine = 88,
}PPPayResultCode;


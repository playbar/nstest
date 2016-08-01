/*
 *  NdCPNotifications.h
 *  NdComPlatform
 *
 *  Created by Sie Kensou on 10-8-12.
 *  Copyright 2010 NetDragon WebSoft Inc.. All rights reserved.
 *
 */

#ifndef _NdCPNotifications_H_
#define _NdCPNotifications_H_

extern NSString * const kNdCPLoginNotification;					/**< 登录完成的通知*/
extern NSString * const kNdCPBuyResultNotification;				/**< 购买结果的通知,在购买结束时会发送该通知。 */
extern NSString * const kNdCPNewMessageNotification;			/**< 定时发送的收到新消息的条数*/
extern NSString * const kNdCPAsynPaySMSSentNotification;		/**< 异步购买选择短信充值，在短信发送成功时会发送该通知 */
extern NSString * const kNdCPLeavePlatformNotification;			/**< 离开平台界面时，会发送该通知 */
#endif // _NdCPNotifications_H_

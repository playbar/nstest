//
//  NdComPlatform.h
//  NdComPlatform_SNS
//
//  Created by Sie Kensou on 10-9-15.
//  Copyright 2010 NetDragon WebSoft Inc.. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NdComPlatformError.h"
#import "NdComPlatformAPIResponse.h"
#import "NdComPlatformAPIResponse+ThirdPlatform.h"
#import "NdComPlatformAPIResponse+LeaderBoard.h"

@class NdNewMsgMng;

@interface NdComPlatform : NSObject {
	NSDictionary		*m_stepDict;
	id					m_updateDelegate;
	NdMyUserInfo		*m_myUserInfo;
	id					m_hasBindPhoneNumberDelegate;
	UIInterfaceOrientation	m_orientation;
	BOOL				m_shouldAutoRotate;
	BOOL				m_showLoadingWhenAutoLogin;
	BOOL				m_isLogining;
	BOOL				m_isPayDebugMode;
	BOOL				m_isUpdateDebugMode;
	NdNewMsgMng*		m_newMsgMng;
	UIViewController	*m_internalViewController;
}

@property (nonatomic, assign) BOOL showLoadingWhenAutoLogin;				/**< 如果自动登录的时候不希望出现loading界面，请将该参数设置为NO */
@property (nonatomic, readonly, assign) UIInterfaceOrientation orientation;

/**
 @brief 获取NdComPlatform的实例对象
 */
+ (NdComPlatform *)defaultPlatform;

/**
 @brief 获取NdComPlatform的版本号
 */
+ (NSString *)version;

/**
 @brief 获取屏幕截图
 */
+ (UIImage *)NdGetScreenShot;

/**
 @brief 获取某视图的局域截图
 @param view 要截图的UIView
 @param captureRect 要截取的区域
 */
+ (UIImage *)NdGetViewCapture:(UIView *)view captureRect:(CGRect)captureRect;

/**
 @brief 设定平台为横屏或者竖屏
 */
- (void)NdSetScreenOrientation:(UIInterfaceOrientation)orientation;

/**
 @brief 设置是否自动旋转。
 @note 
 1:iPad默认开启自动旋转，iPhone默认关闭自动旋转
 2设置NO后，使用NdSetScreenOrientation设置的方向
 3设置Yes后，iPad支持4个方向切换自适应旋转；iPhone不支持横竖屏切换自适应旋转，仅支持横屏自适应旋转或者竖屏自适应旋转。
 */
- (void)NdSetAutoRotation:(BOOL)isAutoRotate;

/**
 @brief 设定调试模式
 @param nFlag 预留，默认为0
 */
- (void)NdSetDebugMode:(int)nFlag;

/**
 @brief 设置应用Id
 @param appId 应用程序id，需要向用户中心申请，合法的id大于0
 */
- (BOOL)setAppId:(int)appId;

/**
 @brief 获取appId，需要预先设置
 */
- (int)appId;

/**
 @brief 设置应用密钥
 @param appKey 第三方应用程序密钥，appKey未系统分配给第三方的应用程序密钥，第三方需要向平台提供方申请，并设置到平台上
 */
- (BOOL)setAppKey:(NSString *)appKey;

/**
 @brief 获取应用软件名称，需要登录后才能获取
 */
- (NSString *)appName;

/**
 @brief 判断是否已经登录平台
 */
- (BOOL)isLogined;

/**
 @brief 是否配置了自动登录
 */
- (BOOL)isAutoLogin;

/**
 @brief 登录平台,进入登录或者注册界面入口
 @param nFlag 标识（按位标识）预留，默认为0
 @result 错误码
 */
- (int)NdLogin:(int) nFlag;

/**
 @brief 注销
 @param nFlag 标识（按位标识）0,表示注销；1，表示注销，并清除自动登录
 @result 错误码 
 */
- (int)NdLogout:(int) nFlag;

/**
 @brief 获取本次登录的sessionId，需要登录后才能获取
 */
- (NSString *)sessionId;

/**
 @brief 获取登录后的Uin
 */
- (NSString *)loginUin;

/**
 @brief 获取登录后的昵称
 */
- (NSString *)nickName;

#pragma mark -
/**
 @brief 进入平台中心
 @param nFlag 预留, 默认为0
 */
- (void)NdEnterPlatform:(int) nFlag;

/**
 @brief 进入好友中心
 @param nFlag  预留，默认为0。
 */
- (void)NdEnterFriendCenter:(int) nFlag;

/**
 @brief 进入游戏大厅
 @param nFlag 预留，默认为0。
 */
- (void)NdEnterAppCenter:(int) nFlag;

/**
 @brief 进入指定AppId的应用主页
 @param nFlag 预留，默认为0。
 @param appId 指定应用或游戏的appid，
 @note 传入的appid小于等于0时直接进入游戏大厅
 */
- (void)NdEnterAppCenter:(int) nFlag appId:(int)appId;

/**
 @brief 进入设置界面
 @param nFlag 预留，默认为0。
 */
- (void)NdEnterUserSetting:(int) nFlag;

/**
 @brief 进入指定用户的空间
 @param uin 用户uin
 @result 错误码
 */
- (int)NdEnterUserSpace:(NSString*)uin;

/**
 @brief 进入看看谁在玩
 @param nFlag 预留，目前传0即可
 @result 错误码
 */
- (int)NdEnterAppUserCenter:(int)nFlag;

/**
 @brief 邀请好友
 @param strInviteContent 邀请内容
 @result 错误码
 */
- (int)NdInviteFriend:(NSString*)strInviteContent;

/**
 @brief 用户反馈
 @result 错误码
 */
- (int)NdUserFeedBack;

/**
 @brief 进入应用论坛
 @param nFlag 预留，目前传0即可
 @result 错误码
 */
- (int)NdEnterAppBBS:(int)nFlag;

/**
 @brief 进入充值界面
 @param nFlag	预留，目前传0即可
 @param content 预留，目前传空即可
 @result 错误码
 */
- (int)NdEnterRecharge:(int)nFlag content:(NSString *)content;

/**
 @brief 进入虚拟币充值界面
 @param cooOrderSerial 合作商订单号，必须保证唯一，双方对账的唯一标记, 用GUID生成，32位
 @param needPayCoins 需支付虚拟币个数，开发者不关注可以传零
 @param payDescription 支付描述，发送支付通知时，原样返回给开发者，默认为空串
 @result 错误码
 */
- (int)NdUniPayForCoin:(NSString*)cooOrderSerial needPayCoins:(int)needPayCoins payDescription:(NSString*)payDescription;

/**
 @brief 向通用业务服务器发起支付请求,必须登录后才能使用
 @param buyInfo 购买信息
 @result 若未登录调用则返回错误码，否则返回0
 */
- (int)NdUniPay:(NdBuyInfo *)buyInfo;

/**
 @brief 如果余额足够的情况下，进行购买；余额不足的情况下，先充值，然后再购买
 @param buyInfo	购买信息
 @result 若未登录调用则返回错误码，否则返回0
 @note 
 1:开发者根据自己的需要和自己的业务服务器确认支付结果
 2:若余额充足，购买完成后会通知购买结果。
 3:若余额不足，选择短信充值并发送短信成功，会通知短信发送成功
 */
- (int)NdUniPayAsyn:(NdBuyInfo *)buyInfo;

#pragma mark -
/**
 @brief 获取当前应用的用户
 @param pagination 分页信息
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetAppUserList:(NdPagination *)pagination delegate:(id)delegate;

/**
 @brief 获取当前应用的我的好友
 @param pagination 分页信息
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetAppMyFriendList:(NdPagination *)pagination delegate:(id)delegate;

/**
 @brief 获取我的好友
 @param pagination 分页信息
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetMyFriendList:(NdPagination *)pagination delegate:(id)delegate;

/**
 @brief 给好友发送信息
 @param uin 要发送的好友uin
 @param msgContent 要发送的消息内容
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdSendFriendMsg:(NSString *)uin msgContent:(NSString *)msgContent delegate:(id)delegate;

/**
 @brief 检查自身版本更新
 @param nFlag 标识（按位标识）默认为0，检测更新
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdAppVersionUpdate:(int)nFlag delegate:(id)delegate;

/**
 @brief 获取好友在通讯录中的名字
 @param uin 好友uin
 @result 好友联系人名称，如果联系人中没有查找到，则返回空
 */
- (NSString*)NdGetContactName:(NSString*)uin;

/**
 @brief 获取新消息数和新系统消息数
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 @note 该接口从网络获取数据，返回结果会回调给delegate；同时，如果消息数目有变化，也会发kNdCPNewMessageNotification消息通知。
 */
- (int)NdGetNewMsgAndNewSysMsgCount:(id)delegate;

/**
 @brief 获取新系统消息数
 @result 新系统消息数
 @note 平台会定期刷新该数据。如果有变更，会发kNdCPNewMessageNotification消息通知。如果要立即更新，调用NdGetNewMsgAndNewSysMsgCount接口。
 */
- (int)NdGetNewMsgCountSys;

/**
 @brief 获取新消息数（来自好友的）
 @result 新消息数
 @note 平台会定期刷新该数据。如果有变更，会发kNdCPNewMessageNotification消息通知。如果要立即更新，调用NdGetNewMsgAndNewSysMsgCount接口。
 */
- (int)NdGetNewMsgCount;

/**
 @brief 获取登录账户的信息
 @result 当前登录账户的信息，包含uin，昵称等。
 @note 该接口在登录后立即返回
 */
- (NdMyUserInfo *)NdGetMyInfo;

/**
 @brief 获取登录账户的详细信息
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetMyInfoDetail:(id)delegate;

/**
 @brief 获取用户的详细信息
 @param uin		 用户uin，不能为空
 @param flag	 标志位，按位与组合。1＝基本信息，2＝积分，4＝心情。
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetUserInfoDetail:(NSString*)uin  flag:(int)flag  delegate:(id)delegate;

/**
 @brief 查询是否已经绑定手机号码
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdHasBindPhoneNumber:(id)delegate;

/**
 @brief 使用动态模版发送动态
 @param templateId 模版ID
 @param paramList 参数列表，键值对。
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdSendTemplateActivity:(int)templateId paramList:(NSDictionary*)dicParamList delegate:(id)delegate;

/**
 @brief 设置本地通知
 @param fireDate notification的倒计时时间，单位秒
 @param alertBody notification的消息内容
 @result 设置结果
 */
- (BOOL)NdSetLocalNotification:(NSTimeInterval)fireDate alertBody:(NSString*)alertBody;

#pragma mark -
#pragma mark Icon （用户图标，成就排行榜图标，虚拟商品图标）

/**
 @brief 获取默认图标
 @param nType 1＝默认用户头像，2＝默认应用图标，3＝默认商品图标
 @result 返回的头像
 */
- (UIImage *)NdGetDefaultPhoto:(int)nType;

/**
 @brief 获取好友头像
 @param uin			要获取的头像的uin
 @param checkSum	预期的头像checkSum, 如果为空，则优先使用本地缓存文件；如果与缓存checkSum不一致，则网络获取新头像。
 @param delegate	回调对象，回调接口参见 NdComPlatformUIProtocol
 @note 回调函数为：	- (void)getPortraitDidFinish:(int)error uin:(NSString *)uin portrait:(UIImage *)portrait checkSum:(NSString *)checkSum;
 @result 错误码
 */
- (int)NdGetPortrait:(NSString *)uin checkSum:(NSString *)checkSum delegate:(id)delegate;

/**
 @brief 获取好友头像
 @param uin			要获取的头像的uin
 @param imageType	图标大小类型
 @param checkSum	预期的头像checkSum, 如果为空，则优先使用本地缓存文件；如果与缓存checkSum不一致，则网络获取新头像。
 @param delegate	回调对象，回调接口参见 NdComPlatformUIProtocol
 @note 回调函数为：	- (void)getPortraitDidFinish:(int)error uin:(NSString *)uin portrait:(UIImage *)portrait checkSum:(NSString *)checkSum;
 @result 错误码
 */
- (int)NdGetPortraitEx:(NSString *)uin imageType:(ND_PHOTO_SIZE_TYPE)imgType checkSum:(NSString *)checkSum delegate:(id)delegate;

/**
 @brief 获取好友头像图片文件
 @param uin			要获取的头像的uin
 @param imageType	图标大小类型
 @param checkSum	预期的头像checkSum, 如果为空，则优先使用本地缓存文件；如果与缓存checkSum不一致，则网络获取新头像。
 @param delegate	回调对象，回调接口参见 NdComPlatformUIProtocol
 @note 回调函数为：	- (void)getPortraitPathDidFinish:(int)error uin:(NSString *)uin portraitPath:(NSString *)portraitPath checkSum:(NSString *)checkSum;
 @result 错误码
 */
- (int)NdGetPortraitPath:(NSString *)uin imageType:(ND_PHOTO_SIZE_TYPE)imgType checkSum:(NSString *)checkSum delegate:(id)delegate;


typedef	enum  _ND_SNS_BOARD_TYPE {
	ND_SNS_BOARD_LEADERBOARD = 0,		/**< 排行榜图标		*/
	ND_SNS_BOARD_ACHIEVEMENT = 1,		/**< 成就榜图标		*/
	ND_SNS_BOARD_VIRTUALGOODS= 8,		/**< 虚拟商品图标		*/
}	ND_SNS_BOARD_TYPE;

/**
 @brief下载榜图标
 @param strId		图标对应boardType的id（如排行榜id，成就榜id，虚拟商品id）
 @param boardType	图标种类
 @param photoType	图标大小类型
 @param checkSum	预期的图标checkSum, 如果为空，则优先使用本地缓存文件；如果与缓存checkSum不一致，则网络获取新图标。
 @param delegate	回调对象，回调接口参见 NdComPlatformUIProtocol
 @note 回调函数为：	- (void)getBoardIconDidFinish:(int)error strId:(NSString*)strId boardType:(ND_SNS_BOARD_TYPE)boardType photoType:(ND_PHOTO_SIZE_TYPE)photoType checksum:(NSString*)checksum image:(UIImage*)img;
 @result 错误码
 */
- (int)NdGetBoardIcon:(NSString*)strId boardType:(ND_SNS_BOARD_TYPE)boardType 
		  photoType:(ND_PHOTO_SIZE_TYPE)photoType  checksum:(NSString*)checksum  delegate:(id)delegate;

#pragma mark -
#pragma mark Achievement, LeaderBoard

/**
 @brief 提交排行榜分数
 @param nLeaderBoardId	排行榜Id
 @param nCurrentScore	当前提交的分数值
 @param displayText		替换排行名次显示的文本，可以传空
 @param delegate		回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdSubmitScore:(int)nLeaderBoardId  score:(int)nCurrentScore  displayText:(NSString*)displayText delegate:(id)delegate;

/**
 @brief 解锁某个成就
 @param nAchievementId	成就榜Id
 @param nCurrentValue	当前成就进度值
 @param displayText		替换进度值显示的文本，可以传空
 @param delegate		回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdUnLockAchievement:(int)nAchievementId value:(int)nCurrentValue displayText:(NSString*)displayText delegate:(id)delegate;

/**
 @brief 打开排行榜界面
 @param nLeaderBoardId 排行榜ID, 默认为0
 @param nFlag 标记位，默认为0
 @result 错误码
 */
- (int)NdOpenLeaderBoard:(int)nLeaderBoardId  flag:(int)nFlag;

/**
 @brief 打开成就榜界面
 @param nFlag 标记位，默认为0
 @result 错误码
 */
- (int)NdOpenAchievement:(int)nFlag;

/**
 @brief 获取当前应用所有排行榜榜单列表
 @param delegate 回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetLeaderBoard:(id)delegate;

/**
 @brief 获取某个排行榜中的用户排行信息列表
 @param uin				用户uin，会附加返回该用户的排行信息
 @param leaderBoardId	排行榜id
 @param userType		参与排行的用户类型：0＝所有用户；1＝我的好友
 @param subType			参与排行的时间范围：0＝所有时间；1＝日排行，当天有提交数据的用户；2＝周排行，最近七天有提交数据的用户；3＝月排行，最近一个月有提交数据的用户
 @param pagination		分页信息，pageIndex从1开始
 @param delegate		回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetUserLeaderBoardInfoList:(NSString*)uin leaderBoardId:(int)leaderBoardId  
						   type:(int)userType  subType:(int)subType 
						 pagination:(NdPagination*)pagination  delegate:(id)delegate;

/**
 @brief 获取当前应用的指定用户的成就列表
 @param uin				用户uin
 @param pagination		分页信息，pageIndex从1开始
 @param delegate		回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdGetAchievementInfoList:(NSString*)uin pagination:(NdPagination*)pagination  delegate:(id)delegate;


#pragma mark -
/**
 @brief 判断支付是成功
 @param strCooOrderSerial	支付订单号
 @param delegate		回调对象，回调接口参见 NdComPlatformUIProtocol
 @result 错误码
 */
- (int)NdCheckPaySuccess:(NSString*)strCooOrderSerial delegate:(id)delegate;

/**
 @brief 进入分享到第三方平台的界面，如果没有绑定指定的平台，则会跳到绑定界面。
 @param strContent	预分享的内容（最大140个字符），第三方平台可能会对重复内容进行屏蔽处理（如新浪微博禁止发重复内容）
 @param imageInfo	预分享的图片，可以是当前屏幕，指定的UIImage 或者 图片名称（支持全路径，或者只有文件名。如果为文件名，load from main bundle）
 @result 错误码
 */
- (int)NdShareToThirdPlatform:(NSString*)strContent  imageInfo:(NdImageInfo*)imageInfo;

/**
 @brief 上传渠道id，需要先设置appId，无须登录
 @param delegate 回调对象
 @result 错误码
 */
- (int)NdUploadChannelId:(id)delegate;

@end



#pragma mark -
@protocol NdComPlatformUIProtocol

/**
 @brief NdSendFriendMsg的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 @param msgId 发送成功后返回的消息id
 */
- (void)sendFriendMsgDidFinish:(int)error msgId:(NSString *)msgId;

/**
 @brief NdGetPortrait 和 NdGetPortraitEx的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param uin 获取头像对应用户的uin
 @param portrait 获取到的头像
 @param checkSum 获取到的头像的checkSum
 */
- (void)getPortraitDidFinish:(int)error uin:(NSString *)uin portrait:(UIImage *)portrait checkSum:(NSString *)checkSum;

/**
 @brief NdGetPortraitPath的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param uin 获取头像对应用户的uin
 @param portraitPath 获取到的头像文件
 @param checkSum 获取到的头像的checkSum
 */
- (void)getPortraitPathDidFinish:(int)error uin:(NSString *)uin portraitPath:(NSString *)portraitPath checkSum:(NSString *)checkSum;

/**
 @brief NdGetBoardIcon的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param strId		图标对应boardType的id（如排行榜id，成就榜id，虚拟商品id）
 @param boardType	图标种类
 @param photoType	图标大小类型
 @param checksum	图标checksum
 @param img			图标
 */
- (void)getBoardIconDidFinish:(int)error strId:(NSString*)strId boardType:(ND_SNS_BOARD_TYPE)boardType photoType:(ND_PHOTO_SIZE_TYPE)photoType checksum:(NSString*)checksum image:(UIImage*)img;

typedef	enum  _ND_APP_UPDATE_RESULT {
	ND_APP_UPDATE_NO_NEW_VERSION = 0,				/**< 没有新版本 */
	ND_APP_UPDATE_FORCE_UPDATE_CANCEL_BY_USER = 2,	/**< 用户取消下载强制更新 */
	ND_APP_UPDATE_NORMAL_UPDATE_CANCEL_BY_USER = 3,	/**< 用户取消下载普通更新 */
	ND_APP_UPDATE_NEW_VERSION_DOWNLOAD_FAIL = 4,	/**< 下载新版本失败 */
	ND_APP_UPDATE_CHECK_NEW_VERSION_FAIL = 7,		/**< 检测新版本失败 */
}	ND_APP_UPDATE_RESULT;
/**
 @brief NdAppVersionUpdate的回调函数
 @param updateResult 检测更新结果
 */
- (void)appVersionUpdateDidFinish:(ND_APP_UPDATE_RESULT)updateResult;

/**
 @brief NdGetNewMsgAndNewSysMsgCount的回调函数
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 @param newMsgCount 新消息个数
 @param newSysMsgCount 新系统消息个数
 */
- (void)getNewMsgAndNewSysMsgCountDidFinish:(int)error newMsgCount:(int)newMsgCount newSysMsgCount:(int)newSysMsgCount;

/**
 @brief NdGetAppUserList的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 @param resultList 应用用户信息列表
 */
- (void)getAppUserListDidFinish:(int)error resultList:(NdStrangerUserInfoList *)userInfoList;

/**
 @brief NdGetAppMyFriendList的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 @param resultList 应用好友信息列表
 */
- (void)getAppMyFriendListDidFinish:(int)error resultList:(NdFriendUserInfoList *)userInfoList;

/**
 @brief NdGetMyFriendList的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 @param resultList 搜索结果
 */
- (void)searchMyFriendDidFinish:(int)error resultList:(NdFriendUserInfoList *)friendUserInfoList;

/**
 @brief NdGetMyInfoDetail 和 NdGetUserInfoDetail 的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param userInfo 获取到的用户信息
 */
- (void)getUserInfoDidFinish:(int)error userInfo:(NdUserInfo *)userInfo;

/**
 @brief NdHasBindPhoneNumber的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 @param bHasBind 是否已经绑定手机号码。
 */
- (void)NdHasBindPhoneNumberDidFinish:(int)error  hasBind:(BOOL)bHasBind;

/**
 @brief NdSendTemplateActivity的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件  
 */
- (void)sendTemplateActivityDidFinish:(int)error;

/**
 @brief NdSubmitScore的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param userRank  返回所提交的数据在排行榜中的排名。
 @param img 图标
 */
- (void)submitScoreDidFinish:(int)error;

/**
 @brief NdUnLockAchievement的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 */
- (void)unlockAchievementDidFinish:(int)error;

/**
 @brief NdGetLeaderBoard的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param leaderboardInfoList  返回所有的排行榜榜单列表，leaderboardInfoList.records存放NdLeaderBoardInfo*
 */
- (void)getAllLeaderBoardsDidFinish:(int)error  result:(NdBasePageList*)leaderboardInfoList;

/**
 @brief NdGetUserLeaderBoardInfoList的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param result 返回分页的排行榜的用户数据。
 */
- (void)getLeaderBoardWithBoardIdDidFinish:(int)error type:(NSUInteger)userType	 subType:(NSUInteger)subType  
									result:(NdUserInfoOfLeaderBoardList*)userInfoOfLeaderBoardList;

/**
 @brief NdGetAchievementInfoList的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件
 @param achievementList 返回分页的成就列表信息，achievementList.records存放NdAchievementInfo*
 */
- (void)getAllAchievementsDidFinish:(int)error result:(NdBasePageList*)achievementList;


/**
 @brief NdCheckPaySuccess回调
 @param error  错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 @param bSuccess 返回支付是否成功
 */
- (void)checkPaySuccessDidFinish:(int)error cooOrderSerial:(NSString*)cooOrderSerial bSuccess:(BOOL)bSuccess;

/**
 @brief NdUploadChannelId的回调
 @param error 错误码，如果error为0，则代表API执行成功，否则失败。错误码涵义请查看NdComPlatformError文件 
 */
- (void)uploadChannelIdDidFinish:(int)error;


@end


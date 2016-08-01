//
//  NdComPlatformAPIResponse.h
//  NdComPlatform_SNS
//
//  Created by Sie Kensou on 10-10-8.
//  Copyright 2010 NetDragon WebSoft Inc.. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum _ND_PHOTO_SIZE  {
	ND_PHOTO_SIZE_TINY = 0,		/**< 16 * 16像素		*/
	ND_PHOTO_SIZE_SMALL,		/**< 48 * 48像素		*/
	ND_PHOTO_SIZE_MIDDLE,		/**< 120*120像素		*/
	ND_PHOTO_SIZE_BIG,			/**< 200*200像素		*/
}   ND_PHOTO_SIZE_TYPE;

/**
 @brief 分页信息
 */
@interface NdPagination : NSObject
{
	int pageIndex;
	int pageSize;
}

@property (nonatomic, assign) int pageIndex;		/**< 要获取的第几页记录，从1开始*/
@property (nonatomic, assign) int pageSize;			/**< 每页记录的个数（5的倍数），最大为50*/

@end

/**
 @brief 按页获取信息列表
 */
@interface NdBasePageList : NSObject
{
	NdPagination	*pagination;
	int				totalCount;
	NSArray*		records;
}

@property (nonatomic, retain) NdPagination *pagination;		/**< 分页结构体 */
@property (nonatomic, assign) int			totalCount;		/**< 总个数 */
@property (nonatomic, retain) NSArray*		records;		/**< 根据具体接口存放对应的数据 */

@end


#pragma mark -
#pragma mark  ------------ Buy, Recharge, Records ------------

/**
 @brief 91豆支付信息
 @note 购买价格保留2位小数
 */
@interface NdBuyInfo : NSObject
{
	NSString *cooOrderSerial;
	NSString *productId;
	NSString *productName;
	float productPrice;			
	float productOrignalPrice;	
	unsigned int productCount;			
	NSString *payDescription;			
}

@property (nonatomic, retain) NSString *cooOrderSerial;				/**< 合作商的订单号，必须保证唯一，双方对账的唯一标记（用GUID生成，32位）*/
@property (nonatomic, retain) NSString *productId;					/**< 商品Id */
@property (nonatomic, retain) NSString *productName;				/**< 商品名字 */
@property (nonatomic, assign) float productPrice;					/**< 商品价格，两位小数 */
@property (nonatomic, assign) float productOrignalPrice;			/**< 商品原始价格，保留两位小数 */
@property (nonatomic, assign) unsigned int productCount;			/**< 购买商品个数 */
@property (nonatomic, retain) NSString *payDescription;				/**< 购买描述，可选，没有为空 */

- (BOOL)isValidBuyInfo;						/**<  判断支付信息是否有效 */
- (BOOL)isCostGreaterThanThreshold;			/**<  返回（总价>100W || 单价> 100W || 数量 > 100W*100） */
- (BOOL)isCostGreaterThanValue:(float)fValue;/**<  返回（productPrice * productCount > fValue) */

@end




/**
 @brief 购买记录
 */
@interface NdPayRecord : NSObject
{
	NSString	*cooOrderSerial;
	NSString	*buyTime;
	NSString	*productName;
	unsigned int	productCount;
	float		pay91Bean;
	//NSString	*status;
	NSString	*appName;
}

@property (nonatomic, retain) NSString *cooOrderSerial;		/**< 购买订单号 */
@property (nonatomic, retain) NSString *buyTime;			/**< 购买时间（yyyy-MM-dd HH:mm:ss) */
@property (nonatomic, retain) NSString *productName;		/**< 商品名称 */
@property (nonatomic, assign) unsigned int productCount;	/**< 购买商品个数 */
@property (nonatomic, assign) float		pay91Bean;			/**< 购买所消费91豆,保留2位小数 */
//@property (nonatomic, retain) NSString *status;			/**< 发货状态 */
@property (nonatomic, retain) NSString *appName;			/**< 应用软件名称 */

@end




/**
 @brief 代付请求结果信息
 */
@interface NdAgencyPayRequestResult : NSObject
{
	NSString *cooOrderSerial;
	NSString *requestUin;
	NSString *orderSerial;
	BOOL	success;
}

@property(nonatomic,retain)	NSString *cooOrderSerial;		/**< 合作商的订单号 */
@property(nonatomic,retain) NSString *requestUin;			/**< 代付人的uin */
@property(nonatomic,retain) NSString *orderSerial;			/**< 通用平台代付订单号 */
@property(nonatomic,assign) BOOL	 success;				/**< 请求是否发送成功 */

@end




/**
 @brief 代付订单信息
 */
@interface NdAgencyPayOrderStatus : NSObject
{
	NSString *requestUin;
	NSString *requestNickName;
	NSString *appId;
	NSString *appName;
	NSString *goodsId;
	NSString *goodsName;
	NSString *goodsPrice;
	NSString *goodsOriginalPrice;
	int		 goodsCount;
	NSString *createTime;
	NSString *orderStatus;
}

@property(nonatomic,retain) NSString *requestUin;			/**< 发起请求人的uin */
@property(nonatomic,retain) NSString *requestNickName;		/**< 发起请求人的昵称 */
@property(nonatomic,retain) NSString *appId;				/**< 应用id */
@property(nonatomic,retain) NSString *appName;				/**< 应用名称 */
@property(nonatomic,retain) NSString *goodsId;				/**< 商品id */
@property(nonatomic,retain) NSString *goodsName;			/**< 商品名称 */
@property(nonatomic,retain) NSString *goodsPrice;			/**< 商品价格（单位元，保留小数点两位） */
@property(nonatomic,retain) NSString *goodsOriginalPrice;	/**< 商品原价（单位元，保留小数点两位） */
@property(nonatomic,assign) int		 goodsCount;			/**< 购买商品个数 */
@property(nonatomic,retain) NSString *createTime;			/**< 订单生成时间 */
@property(nonatomic,retain) NSString *orderStatus;			/**< 订单状态 */

@end


/**
 @brief 充值记录类
 */
@interface NdRechargingRecord : NSObject 
{
	NSString			*cooOrderSerial;
	NSString			*rechargingTime;
	NSString			*rechargingType;
	float				recharging91Bean;
}

@property (nonatomic, retain) NSString	*cooOrderSerial;			/**< 充值订单号（默认为空）*/
@property (nonatomic, retain) NSString	*rechargingTime;			/**< 充值时间（yyyy－MM－dd HH：mm：ss）*/
@property (nonatomic, retain) NSString	*rechargingType;			/**< 充值渠道 */
@property (nonatomic, assign) float		recharging91Bean;			/**< 充值91豆,保留2位小数*/

@end

#pragma mark -
#pragma mark  ------------ User Info ------------

/**
 @brief 用户详细信息
 */
@interface NdUserInfo : NSObject<NSCoding> 
{
	NSString	*uin;
	NSString	*nickName;
	int			bornYear;
	int			bornMonth;
	int			bornDay;
	int			sex;
	NSString	*province;
	NSString	*city;
	NSString	*trueName;
	NSString	*point;
	NSString	*emotion;
	NSString	*checkSum;
}

- (void)copyDataFromUserInfo:(NdUserInfo*)info;		/**< 浅复制 */

@property (nonatomic, retain) NSString *uin;		/**< 用户uin */
@property (nonatomic, retain) NSString *nickName;	/**< 昵称（1－20个字符，不可为空）*/
@property (nonatomic, assign) int bornYear;			/**< 出生年份，未知为空 */
@property (nonatomic, assign) int bornMonth;		/**< 出生月份，未知为空 */
@property (nonatomic, assign) int bornDay;			/**< 出生日，未知为空 */
@property (nonatomic, assign) int sex;				/**< 0＝未设置，1＝男，2＝女 */
@property (nonatomic, retain) NSString *province;	/**< 省份，未知为空 */
@property (nonatomic, retain) NSString *city;		/**< 城市，未知未空 */
@property (nonatomic, retain) NSString *trueName;	/**< 真实姓名（2－4个汉字），未知为空 */
@property (nonatomic, retain) NSString *point;		/**< 积分 */
@property (nonatomic, retain) NSString *emotion;	/**< 心情 */
@property (nonatomic, retain) NSString *checkSum;	/**< 好友头像的Md5值 */

@end




/**
 @brief 我的基础信息
 */
@interface NdMyBaseInfo : NSObject 
{
	NSString *uin;
	NSString *nickName;
	NSString *checkSum;
}

@property (nonatomic, retain) NSString *uin;			/**< 自己的uin */
@property (nonatomic, retain) NSString *nickName;		/**< 自己的昵称 */
@property (nonatomic, retain) NSString *checkSum;		/**< 自己的头像的checkSum */

@end




/**
 @brief 我的用户信息
 */
@interface NdMyUserInfo : NSObject 
{
	NdMyBaseInfo *baseInfo;
}

@property (nonatomic, retain) NdMyBaseInfo *baseInfo;	/**< 基础信息 */

@end





/**
 @brief 好友详细信息 
 */
@interface NdFriendRemarkUserInfo : NdUserInfo 
{
	NSString*	friendRemark;
	NSString*	updateTime;	
	NSString*	strTip_aux;	
}

@property (nonatomic, retain) NSString*	friendRemark;	/**< 好友备注 */
@property (nonatomic, retain) NSString*	updateTime;		/**< 好友最后更新标志 */
@property (nonatomic, retain) NSString*	strTip_aux;		/**< 搜索辅助标志 */

- (void)updateFriendContactName;						/**< 通用平台内部使用 */
- (void)updateRemarkToPinyinInitial;					/**< 通用平台内部使用 */
- (NSString*)friendName;								/**< 优先返回备注，如果没有备注，返回昵称 */

@end



/**
 @brief 用户基础信息
 */
@interface NdBaseUserInfo : NSObject 
{
	NSString *uin;
	NSString *nickName;
	NSString *checkSum;
	BOOL	  bMyFriend;
}

- (void)replaceNickNameWithFriendRemark;				/**< 如果是我的好友，把昵称替换为备注（有备注的情况） */

@property (nonatomic, retain) NSString *uin;			/**< 好友的uin */
@property (nonatomic, retain) NSString *nickName;		/**< 好友的昵称 */
@property (nonatomic, retain) NSString *checkSum;		/**< 好友头像的Md5值 */
@property (nonatomic, assign) BOOL		bMyFriend;		/**< 该用户是否是我的好友 */

@end




/**
 @brief 陌生人信息
 */
@interface NdStrangerUserInfo : NSObject 
{
	NdBaseUserInfo	*baseUserInfo;
	NSString		*province;
	NSString		*city;
	int				sex;
	int				age;
	int				onlineStatus;
}

@property (nonatomic, retain) NdBaseUserInfo *baseUserInfo;	/**< 基础信息 */
@property (nonatomic, retain) NSString *province;			/**< 省份 */
@property (nonatomic, retain) NSString *city;				/**< 城市 */
@property (nonatomic, assign) int sex;						/**< 0＝未设置，1＝男，2＝女 */
@property (nonatomic, assign) int age;						/**< 年龄 */
@property (nonatomic, assign) int onlineStatus;				/**< 在线状态，0＝未知，1＝在线，2＝离线 */

@end




/**
 @brief 陌生人信息列表
 @note  records 存放的是NdStrangerUserInfo类型对象
 */
@interface NdStrangerUserInfoList : NdBasePageList {
}

+ (NSArray *)createUserListArray:(NSString *)users;			/**< 通用平台内部使用 */
- (void)replaceNickNameWithFriendRemark;					/**< 把列表中的好友昵称换成好友备注 */

@end




/**
 @brief 好友信息
 */
@interface NdFriendUserInfo : NSObject 
{
	NdBaseUserInfo	*baseUserInfo;
	NSString		*point;
	NSString		*emotion;
	int				onlineStatus;
}

@property (nonatomic, retain) NdBaseUserInfo *baseUserInfo;		/**< 基础信息 */
@property (nonatomic, retain) NSString		 *point;			/**< 积分 */
@property (nonatomic, retain) NSString		 *emotion;			/**< 心情 */
@property (nonatomic, assign) int			 onlineStatus;		/**< 在线状态：0＝未知，1＝在线，2＝离线 */

@end




/**
 @brief 查找到的好友信息列表
 @note  records 存放的是NdFriendUserInfo类型对象
 */
@interface NdFriendUserInfoList : NdBasePageList {
}

+ (NSArray *)createFriendInfo:(NSString *)friends;		/**< 通用平台内部使用 */
- (void)replaceNickNameWithFriendRemark;				/**< 把列表中的好友昵称换成好友备注 */

@end




#pragma mark -
#pragma mark  ------------ Permission ------------

/**
 @brief 添加好友权限定义值
 */
typedef enum _ND_FRIEND_AUTHORIZE_TYPE 
{
	ND_FRIEND_AUTHORIZE_TYPE_READ = -1,					/**< 读取 */
	ND_FRIEND_AUTHORIZE_TYPE_NEED_AUTHORIZE,			/**< 需要验证才能添加 */
	ND_FRIEND_AUTHORIZE_TYPE_EVERYONE_CAN_ADD,			/**< 允许任何人添加 */
	ND_FRIEND_AUTHORIZE_TYPE_NO_ONE_CAN_ADD,			/**< 不允许任何人添加 */
} ND_FRIEND_AUTHORIZE_TYPE;

/**
 @brief 是否启用支付密码权限定义值
 */
typedef enum _ND_PAY_AUTHORIZE_TYPE
{
	ND_PAY_AUTHORIZE_TYPE_READ = -1,					/**< 读取*/
	ND_PAY_AUTHORIZE_TYPE_CLOSE,						/**< 关闭 */
	ND_PAY_AUTHORIZE_TYPE_OPEN,							/**< 启用 */
}ND_PAY_AUTHORIZE_TYPE;

/**
 @brief 是否已经设置帐号登录密码权限定义值
 */
typedef enum _ND_ACCOUNTS_AUTHORIZE_TYPE
{
	ND_ACCOUNTS_AUTHORIZE_TYPE_READ = -1,					/**< 读取*/
	ND_ACCOUNTS_AUTHORIZE_TYPE_CLOSE,						/**< 未设置 */
	ND_ACCOUNTS_AUTHORIZE_TYPE_OPEN,						/**< 已设置 */
}ND_ACCOUNTS_AUTHORIZE_TYPE;


/**
 @brief 用户的添加好友权限信息
 */
@interface NdAddFriendPermission : NSObject 
{
	ND_FRIEND_AUTHORIZE_TYPE		canAddFriend;
	NSString*						uin;
}

@property (nonatomic, assign) ND_FRIEND_AUTHORIZE_TYPE canAddFriend;			/**< uin对应的权限 */	
@property (nonatomic, retain) NSString*		uin;								/**< 用户uin, 为空时代表自己 */	

@end

/**
 @brief 是否启用支付密码权限信息
 */
@interface NdPayPasswordPermission : NSObject 
{
	ND_PAY_AUTHORIZE_TYPE			canPayPassword;
	NSString*						uin;
}

@property (nonatomic, assign) ND_PAY_AUTHORIZE_TYPE canPayPassword;				/**< uin对应的权限 */	
@property (nonatomic, retain) NSString*		uin;								/**< 用户uin, 为空时代表自己 */	

@end

/**
 @brief 是否已经设置帐号登录密码权限信息
 */
@interface NdPasswordPermission : NSObject 
{
	ND_ACCOUNTS_AUTHORIZE_TYPE		canPassword;
	NSString*						uin;
}

@property (nonatomic, assign) ND_ACCOUNTS_AUTHORIZE_TYPE canPassword;			/**< uin对应的权限 */	
@property (nonatomic, retain) NSString*		uin;								/**< 用户uin, 为空时代表自己 */	

@end


/**
 @brief 用户的权限信息
 */
@interface NdPermission : NSObject
{
	NdAddFriendPermission*	addFriendPermission;	
	NdPayPasswordPermission* payPasswordPermission;
	NdPasswordPermission* passwordPermission;
}

@property (nonatomic, retain) NdAddFriendPermission*	addFriendPermission;	/**< 添加好友权限 */
@property (nonatomic, retain) NdPayPasswordPermission*	payPasswordPermission;	/**< 是否启用支付密码权限 */ 
@property (nonatomic, retain) NdPasswordPermission*	passwordPermission;			/**< 设置帐号登录密码权限 */

@end



#pragma mark -
#pragma mark  ------------ App Info ------------

/**
 @brief 应用程序基础信息
 */
@interface NdBaseAppInfo : NSObject 
{
	int			appId;
	NSString	*appName;
	NSString	*checkSum;
}

@property (nonatomic, assign) int		appId;			/**< 应用程序Id */
@property (nonatomic, retain) NSString  *appName;		/**< 应用程序名称 */
@property (nonatomic, retain) NSString  *checkSum;		/**< 应用程序Icon的Md5值 */

@end




/**
 @brief 好友应用信息
 */
@interface NdFriendAppInfo : NSObject 
{
	NdBaseAppInfo	*baseAppInfo;
	int				grade;
	NSString		*description;
	NSString		*opinion;
}

@property (nonatomic, retain) NdBaseAppInfo *baseAppInfo;		/**< 应用程序基础信息 */
@property (nonatomic, assign) int grade;						/**< 应用评级 */
@property (nonatomic, retain) NSString *description;			/**< 应用简介 */
@property (nonatomic, retain) NSString *opinion;				/**< 好友对应用评价 */

@end




/**
 @brief 最新应用总数信息
 */
@interface NdProductNumInfo : NSObject
{
    NSString *currentTime;
    int      newProductNum;
}

@property (nonatomic, retain) NSString *currentTime;
@property (nonatomic, assign) int newProductNum;

@end




/**
 @brief 获取应用分享信息及其在资源中心的url地址
 */
@interface NdSharedMessageInfo : NSObject 
{
	NSString*	strAppInfo;
	NSString*	strAppUrl;
	NSString*	strTime;
}

@property (nonatomic, retain) NSString*	strAppInfo;	/**< 分享的信息 */
@property (nonatomic, retain) NSString*	strAppUrl;	/**< 应用在资源中心的页面地址 */
@property (nonatomic, retain) NSString*	strTime;	/**< 当前时间 */

@end



#pragma mark -
#pragma mark  ------------ Others ------------


/**
 @brief 版本升级信息
 */
@interface NdAppUpdateInfo : NSObject
{
	int updateType;
	NSString *newVersion;
	unsigned long softSize;
	NSString *updateUrl;
	NSString *resourceId;
}

@property (nonatomic,assign) int updateType;		/**< 更新类型，0＝版本一致，无须更新，1＝需要强制更新，2＝不需要强制更新 */
@property (nonatomic,retain) NSString *newVersion;	/**< 新版本号名称 */
@property (nonatomic,assign) unsigned long softSize;/**< 软件大小 */
@property (nonatomic,retain) NSString *updateUrl;	/**< 新版本下载url */
@property (nonatomic,retain) NSString *resourceId;	/**< 软件资源id */

@end


/**
 @brief 高级查找参数
 */
@interface NdAdvanceSearch : NSObject 
{
	int ageBegin;
	int ageEnd;
	int sex;
	NSString *province;
	NSString *city;
}

@property (nonatomic, assign) int ageBegin;			/**< 若值为－1，则不使用该搜索条件 */
@property (nonatomic, assign) int ageEnd;			/**< 若值为－1，则不使用该搜索条件 */
@property (nonatomic, assign) int sex;				/**< 若值为－1，则不使用该搜索条件 , 0=未设置，1＝男，2＝女*/
@property (nonatomic, retain) NSString *province;	/**< 若值为nil，则不使用该搜索条件 */
@property (nonatomic, retain) NSString *city;		/**< 若值为nil，则不使用该搜索条件 */

@end


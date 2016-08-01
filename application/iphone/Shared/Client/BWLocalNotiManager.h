//
//  BWLocalNotiManager.h
//  BaiWan
//
//  Created by xiepf on 14-9-15.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BWLocalNotiManager : NSObject
+(id)sharedInstance;
-(void)startLoadingNotificationMessage;
@property(retain)NSMutableData* requestData;
@property(retain)NSURLConnection* requestConnection;
@end

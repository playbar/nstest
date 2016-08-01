//
//  Login.h
//  MSCDemo
//
//  Created by hchuang on 13-7-31.
//  Copyright (c) 2013年 iflytek. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <iflyMSC/IFlySpeechUser.h>

@interface IFLYTEKLoginModule : NSObject
+(id)sharedIFLYTEKLoginModule;
+(BOOL) isLogin;
-(void) Login;
//-(void) initPopView:(UIView*) parentView;
@end

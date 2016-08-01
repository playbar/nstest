//
//  Login.m
//  MSCDemo
//
//  Created by hchuang on 13-7-31.
//  Copyright (c) 2013年 iflytek. All rights reserved.
//

#import "IFLYTEKLoginModule.h"
#import "IFLYTEKDefinition.h"
#import "iflyMSC/iflySetting.h"
#import "PopupView.h"


static IFLYTEKLoginModule * ShareIFLYTEKLogin = nil;
@implementation IFLYTEKLoginModule
{
    //PopupView           * _popUpView;
    //UIView              * _parentView;
    IFlySpeechUser      * _loginUser;
}

+(id)sharedIFLYTEKLoginModule
{
    if (!ShareIFLYTEKLogin)
    {
        ShareIFLYTEKLogin = [[IFLYTEKLoginModule alloc]init];
        [IFlySetting setLogFile:LVL_ALL];
        [IFlySetting showLogcat:NO];
    }
    return ShareIFLYTEKLogin;
}

-(void) initPopView:(UIView*) parentView
{
    //_popUpView = [[PopupView alloc]initWithFrame:CGRectMake(100, 300, 0, 0)];
}

+(BOOL) isLogin
{
    return [IFlySpeechUser isLogin];
}

-(void) Login
{
    if (![IFlySpeechUser isLogin])
    {
        //[_popUpView setText: @"正在登陆..."];
        //[_parentView addSubview:_popUpView];
        
        // 需要先登陆
        _loginUser = [[IFlySpeechUser alloc] init];
        
        // user 和 pwd 都传入nil时表示是匿名登陆
        NSString *loginString = [[NSString alloc] initWithFormat:@"appid=%@",APPID];
        [_loginUser login:nil pwd:nil param:loginString];
        [loginString release];
    }
//    else {
//        [_popUpView setText: @"已登陆"];
//        [_parentView addSubview:_popUpView];
//    }
}


-(void)dealloc
{
//    [_popUpView release];
//    _popUpView = nil;
    [_loginUser release];
    _loginUser = nil;
    [super dealloc];
}
@end

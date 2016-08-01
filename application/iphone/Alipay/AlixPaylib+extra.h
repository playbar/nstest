//
//  AlixPaylib+extra.h
//  AlipaySdkDemo
//
//  Created by sky on 12/26/13.
//  Copyright (c) 2013 Alipay. All rights reserved.
//



#import <Foundation/Foundation.h>

#define kNotifitionRefreshAlixWebView @"kNotifitionRefreshAlixWebView"
#define kNotifitionErrorLoading @"kNotifitionErrorLoading"

@interface AlixPaylib : NSObject

+ (id)shareLib;

- (id)payOrder:(NSString*)order AndScheme:(NSString*)scheme  seletor:(SEL)seletor target:(id)target;

- (void)setOrder:(NSString *)order;

- (void)setScheme:(NSString *)scheme;

- (void)setIsFullScreen:(BOOL)b;

- (NSString *)openUrl;
- (NSObject *)seletor;

- (UIViewController *) webViewController;
- (void)setWebViewController:(UIViewController *)vc;
- (void)showNetworkError:(NSObject *)obj;


//打开web支付界面
- (void)openWebCashierWithUrl;

//- (void)showWebView:(UIView *)view WithAnimation:(NSObject  *)obj;

//打开网页支付界面
- (void)openWebView;


//处理选择事件
//- (void)alertShow:(AlixButtonInfo *)view;

@end


@interface AlixPaylib (extra)

@end

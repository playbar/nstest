//
//  AlixPaylib+extra.m
//  AlipaySdkDemo
//
//  Created by sky on 12/26/13.
//  Copyright (c) 2013 Alipay. All rights reserved.
//

#import "AlixPaylib+extra.h"

@implementation AlixPaylib (extra)


//- (void)downloadApp
//{
//    [[NSNotificationCenter defaultCenter] postNotificationName:kNotifitionRefreshAlixWebView object:nil];
//}

//////选择是否需要去下载支付宝客服端app
- (void)downloadApp
{

    
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotifitionRefreshAlixWebView object:nil];

}

- (void)openApp:(NSString *)name
{
       [[NSNotificationCenter defaultCenter] postNotificationName:kNotifitionRefreshAlixWebView object:nil];
//        [self openWebView];
}

- (void)showNetworkError:(NSObject *)obj
{
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotifitionErrorLoading object:nil];
}

- (void)openWebCashierWithUrl
{
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotifitionRefreshAlixWebView object:nil];
}



@end

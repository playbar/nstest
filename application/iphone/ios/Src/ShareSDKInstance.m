//
//  ShareSDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BWTHIRDPARTSDKISON
#import "ShareSDKInstance.h"
#if BW91SDKMODE
#import "_91SDKInstance.h"
#elif BWUCSDKMODE
#import "UCSDKInstance.h"
#elif BWPPSDKMODE
#import "PPSDKInstance.h"
#endif

static id ShareSDKAll = nil;
@implementation ShareSDKInstance
+(id)sharedSDK
{

        if (!ShareSDKAll)
        {
        #if BW91SDKMODE
            ShareSDKAll = [[_91SDKInstance alloc]init];
        #elif BWUCSDKMODE
            ShareSDKAll = [[UCSDKInstance alloc]init];
        #elif BWPPSDKMODE
            ShareSDKAll = [[PPSDKInstance alloc] init];
        #endif

        }


    return ShareSDKAll;

}

@end
#endif
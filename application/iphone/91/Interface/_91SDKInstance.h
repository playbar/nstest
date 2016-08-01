//
//  SDKInstance.h
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BW91SDKMODE

#import <Foundation/Foundation.h>
#import "ThirdPartSDKProtocol.h"

@interface _91SDKInstance : NSObject<ThirdPartSDKProtocol>
{
    id delegate;
}
@property (assign)id delegate;
- (void)loginOK91SDK:(NSNotification*)notification;
- (void)NdUiPayAsynResult:(NSNotification*)notify;
@end

#endif

//
//  PayWebViewController.h
//  BaiWan
//
//  Created by imac1 on 14-1-22.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>
// 银联SDK
#import "UPPayPluginDelegate.h"
#import "UPPayPlugin.h"
#import "UIViewController+ext.h"

@interface PayWebViewController : UIViewController<UIWebViewDelegate,UPPayPluginDelegate>
{
    UIWebView * payWebView;
    float width;
    float height;
    bool isBarHide;
    bool isOpaque;
    NSTimer * timerForErrorShow;
}
@property (retain) NSString *payUrl;

-(id)initwithUrl:(NSString *)url;
@end

//
//  ClientWebView.h
//  BaiWan
//
//  Created by xiepf on 14-5-6.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>
// 银联SDK
#import "UPPayPluginDelegate.h"
#import "UPPayPlugin.h"
#import "UIViewController+ext.h"

@interface ClientWebView : UIWebView<UIWebViewDelegate, UPPayPluginDelegate, UIGestureRecognizerDelegate>
{
    float width;
    float height;
    UIImageView * aImageView;
    NSTimer * aTimer;
    UIPanGestureRecognizer * gesT;
    float progress;
    int totalRequestCount;
    int totalLoadedCount;
    BOOL errorPageDidShow;
    BOOL errorPageBeginShow;
}
@property BOOL mouseShowEnable;
@property (copy) NSString *homeUrl;
-(id)initwithUrl:(NSString *)url andFrame:(CGRect)frame;
-(void)resetHomeUrl:(NSString *)url;
-(void)onRefreshButton;
-(void)onHomeButton;
-(void)onBackButton;
@end

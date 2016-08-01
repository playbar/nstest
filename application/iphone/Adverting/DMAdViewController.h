//
//  ViewController.h
//  InterstitialAdView
//
//  Created by domob on 14-2-8.
//  Copyright (c) 2014年 domob. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DMAdViewController : UIViewController
{
    NSTimer *timer;
}
//提供给开发者调用请求广告
- (void)loadAdView;

@end

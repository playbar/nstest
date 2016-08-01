//
//  AppDelegate_iPhone.h
//  BaiWan
//
//  Created by handong on 11-5-9.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BaiWanViewController_ios;
@class UINavigationControllerLandscape;
#if BWLOADINGMODE
@class HDLaunchViewController;
#endif

@interface AppDelegate_ios : UIResponder <UIApplicationDelegate>
{
    UIWindow *window;
    BaiWanViewController_ios* mainViewController;
    
#if BWLOADINGMODE
    HDLaunchViewController* launchViewController;
//    UINavigationController * navigationController;
#endif
    
#if BWPUSHMODE
    // push里带的url和类型
    // 类型1：用内部浏览器打开
    // 类型2：用safari打开
    // 类型3：用appstore打开
	NSString* customUrlString;
	int customUrlType;
	
    // 程序是否切换到后台
    // 在收到push时根据此变量来实现不同表现
	BOOL isResignActive;
#endif
}

@property (nonatomic, retain) UIWindow* window;
@property (nonatomic, retain) BaiWanViewController_ios* mainViewController;
@property (nonatomic, retain)UINavigationControllerLandscape * navigationController;
@end

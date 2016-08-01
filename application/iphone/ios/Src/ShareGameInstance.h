//
//  ShareGameInstance.h
//  BaiWan
//
//  Created by imac1 on 13-12-24.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BWPLAYNOTFLASHGAME
#import <Foundation/Foundation.h>
#import "unity_static.h"
@protocol ThirdPartGameProtocol <NSObject>
@required
-(void)initInMainFunction:(int)argc andArgv:(char *[])argv;;
- (BOOL)applicationDidFinishLaunchingWithOptions:(id)launchOptions;
-(void)applicationWillResignActive:(UIApplication *)application;
-(void)applicationDidBecomeActive:(UIApplication *)application;
-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application;
-(void)applicationDidEnterBackground:(UIApplication*)application;
-(void)applicationWillEnterForeground:(UIApplication*)application;
-(void)applicationWillTerminate:(UIApplication *)application;
-(BOOL)startGame;

@optional
-(void)applicationSignificantTimeChange:(UIApplication *)application;
-(void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary*)userInfo;
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification;
- (void)applicationDidRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken;
- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error;
- (BOOL)application:(UIApplication*)application openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication annotation:(id)annotation;
-(void)releaseApplication;
@end

@interface ShareGameInstance : NSObject<ThirdPartGameProtocol>
+(id)sharedGame;
@end
#endif

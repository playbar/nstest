//
//  ShareGameInstance.m
//  BaiWan
//
//  Created by imac1 on 13-12-24.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
#if BWPLAYNOTFLASHGAME
#import "ShareGameInstance.h"
static id BWShareGameInstance = nil;

@implementation ShareGameInstance
+(id)sharedGame
{
    
    if (!BWShareGameInstance)
    {
        BWShareGameInstance = [[ShareGameInstance alloc]init];
    }
    return BWShareGameInstance;
}

-(void)initInMainFunction:(int)argc andArgv:(char *[])argv;
{
   [[unity_static shareUnity]initInMain:argc andArgv:argv];
}
- (BOOL)applicationDidFinishLaunchingWithOptions:(id)launchOptions
{
    return [[unity_static shareUnity] applicationDidFinishLaunchingWithOptions:launchOptions];
}
-(void)applicationWillResignActive:(UIApplication *)application
{
    [[unity_static shareUnity]applicationWillResignActive:application];
}
-(void)applicationDidBecomeActive:(UIApplication *)application
{
    [[unity_static shareUnity]applicationDidBecomeActive:application];
}
-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    [[unity_static shareUnity] applicationDidReceiveMemoryWarning:application];
}
-(void)applicationDidEnterBackground:(UIApplication*)application
{
    [[unity_static shareUnity]applicationDidEnterBackground:application];
}
-(void)applicationWillEnterForeground:(UIApplication*)application
{
    [[unity_static shareUnity]applicationWillEnterForeground:application];
}
-(void)applicationWillTerminate:(UIApplication *)application
{
    [[unity_static shareUnity]applicationWillTerminate:application];
}
-(BOOL)startGame
{
    return [[unity_static shareUnity] startGame];
}

-(void)applicationSignificantTimeChange:(UIApplication *)application
{
   // [[unity_static shareUnity] applicationSignificantTimeChange:application];
}
//3D function
-(void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary*)userInfo
{
    //[[unity_static shareUnity] applicationDidReceiveLocalNotification:notification];
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    [[unity_static shareUnity]application:application didReceiveLocalNotification:notification];
}
- (void)applicationDidRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
    //[[unity_static shareUnity]applicationDidRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
}
- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
    [[unity_static shareUnity]application:application didFailToRegisterForRemoteNotificationsWithError:error];
}

- (BOOL)application:(UIApplication*)application openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication annotation:(id)annotation
{
    return TRUE;
}
-(void)releaseApplication
{
    
}
@end
#endif
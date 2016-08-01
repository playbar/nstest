//
//  AppDelegate_iPhone.m
//  BaiWan
//
//  Created by handong on 11-5-9.
//  Copyright 2011 JQBar. All rights reserved.
//

#import "AppDelegate_ios.h"
#import "UINavigationControllerLandscape.h"
#import "BaiWanViewController_ios.h"
#import "ClientView_ios.h"
#import "UIDeviceAdvance.h"
#import "GlobalPortingFunction.h"
#if BWLOADINGMODE
#import "HDLaunchView.h"
#import "HDLaunchViewController.h"
#endif
#if BWTHIRDPARTSDKISON
#import "ShareSDKInstance.h"
#endif
#if BWMOBISAGE
#import "MobiSageSDK.h"
#endif
#import "GGBase.h"
#if BWPLAYNOTFLASHGAME
#import "ShareGameInstance.h"
#endif

#ifdef BWWXSDKMODE
extern BOOL g_WeiXinAppIsInstalled;
#endif

BaiWanViewController_ios* g_explorerController_ios = nil;
extern iOSDeviceRelateData g_deviceData;

@interface AppDelegate_ios (PrivateMethod)

#if BWLOADINGMODE
- (void)launchViewWillHide;
- (void)launchViewDidHide;
#endif

#if BWPUSHMODE
// 打开push的链接
- (void)openCustomURL;
#endif

@end

@implementation AppDelegate_ios
@synthesize window;
@synthesize mainViewController;
@synthesize navigationController;

#pragma mark - Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions 
{    
    BWLOG(@"DeviceName:%@", [[UIDevice currentDevice] name]);
    BWLOG(@"SystemName:%@", [[UIDevice currentDevice] systemName]);
    BWLOG(@"SystenVersion:%@", [[UIDevice currentDevice] systemVersion]);
    BWLOG(@"DeviceModel:%@", [[UIDevice currentDevice] model]);
    BWLOG(@"DeviceMachine:%@", [[UIDevice currentDevice] machine]);
    BWLOG(@"ShowBarMode:%@", BWSHOWBARMODE?@"ON":@"OFF");
    BWLOG(@"OnSaleMode:%@", BWONSALEMODE?@"ON":@"OFF");
    BWLOG(@"PushMode:%@", BWPUSHMODE?@"ON":@"OFF");
    BWLOG(@"IAPMode:%@", BWIAPMODE?@"ON":@"OFF");
    BWLOG(@"NewAppMode:%@", BWNEWAPPMODE?@"ON":@"OFF");
    BWLOG(@"LoadingMode:%@", BWLOADINGMODE?@"ON":@"OFF");
    BWLOG(@"MemoryShowMode:%@", BWMEMORYSHOWMODE?@"ON":@"OFF");
    BWLOG(@"Options:%@", [launchOptions description]);
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    mainViewController = [[BaiWanViewController_ios alloc]init];
    g_explorerController_ios = mainViewController;
    BWLOG(@"screen bound is %f, %f, %f, %f",[[UIScreen mainScreen]bounds].origin.x, [[UIScreen mainScreen]bounds].origin.y, [[UIScreen mainScreen]bounds].size.width, [[UIScreen mainScreen]bounds].size.height);
    [self showViewController];
#if BWPUSHMODE
	// 清空iconbadgenumber并注册push
	[[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
//    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
//    {
//        UIUserNotificationSettings *notiSetting = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge|UIUserNotificationTypeSound|UIUserNotificationTypeAlert categories:nil];
//        [[UIApplication sharedApplication]registerUserNotificationSettings:notiSetting];
//        [[UIApplication sharedApplication] registerForRemoteNotifications];
//    }
//    else
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge|UIRemoteNotificationTypeSound|UIRemoteNotificationTypeAlert];
    
    
    
	// 处理launchOptions
	BWRELEASE(customUrlString);
	customUrlType = 0;
	
	if (launchOptions)
	{
		NSDictionary* baiwan_iphone = [launchOptions objectForKey:@"UIApplicationLaunchOptionsRemoteNotificationKey"];
        if (baiwan_iphone)
        {
            NSDictionary* bw = [baiwan_iphone objectForKey:@"bw"];
            if (bw)
            {
                id value = [bw valueForKey:@"url"];
                if (value)
                    customUrlString = [[NSString alloc] initWithString:value];
                customUrlType = [[bw valueForKey:@"urltype"] intValue];
                [self openCustomURL];
            }
        }
	}
#endif
	
    return YES;	
}

- (void)applicationWillResignActive:(UIApplication *)application 
{
#if BWPUSHMODE
	isResignActive = YES;
    [mainViewController setResignActive:YES];
#endif
#if BWPLAYNOTFLASHGAME
    [[ShareGameInstance sharedGame]applicationWillResignActive:application];
#endif
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
#if BWPUSHMODE
    // 清空iconbadgenumber
	if ([[UIApplication sharedApplication] applicationIconBadgeNumber]!=0)
		[[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
	
	isResignActive = NO;
    [mainViewController setResignActive:NO];
#endif
#if BWPLAYNOTFLASHGAME
    [[ShareGameInstance sharedGame]applicationDidBecomeActive:application];
#endif
}

#pragma mark - AppDelegate_iPhone PrivateMethod
-(void)showViewController
{
    mainViewController.isStypeValid = NO; // 2s后方向不起作用
    if(!g_deviceData.m_isPad &&[[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
    {
        self.window = [[UIWindow alloc] initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
    }
    else
        self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];//CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
    
    mainViewController.wantsFullScreenLayout = YES;

    
    self.navigationController = [[UINavigationControllerLandscape alloc]initWithRootViewController:mainViewController];
    self.navigationController.navigationBarHidden = YES;
    [self.window setRootViewController:self.navigationController];
    
#if BWLOADINGMODE
    launchViewController = [[HDLaunchViewController alloc] init];
    CGRect rect = [[UIScreen mainScreen] bounds];
    CGFloat width, height;
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        width = rect.size.width < rect.size.height ? rect.size.width : rect.size.height;
        height = rect.size.width > rect.size.height ? rect.size.width : rect.size.height;
        [[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortrait];
    }
    else
    {
        width = rect.size.width > rect.size.height ? rect.size.width : rect.size.height;
        height = rect.size.width < rect.size.height ? rect.size.width : rect.size.height;
        [[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight];
    }
    rect.size.width = width;
    rect.size.height = height;
    mainViewController.view.frame = rect;
    HDLaunchView * launchView = [[HDLaunchView alloc] initWithFrame:rect];
    [launchViewController.view addSubview:launchView];
    [launchView release];
    [self.navigationController pushViewController:launchViewController animated:NO];
    [launchViewController release];
    [NSTimer scheduledTimerWithTimeInterval:5 target:self selector:@selector(launchViewWillHide:) userInfo:nil repeats:NO];
#endif
    [self.window makeKeyAndVisible];
    
#if BWTHIRDPARTSDKISON
    [[ShareSDKInstance sharedSDK] initSDKWithDelegate:mainViewController]; //should behind
#endif
#ifdef BWWXSDKMODE
    [WXApi registerApp:@"wx28e19f6077bba742"];
    g_WeiXinAppIsInstalled = [WXApi isWXAppInstalled];
#endif
#if BWPLAYNOTFLASHGAME
	[[ShareGameInstance sharedGame]applicationDidFinishLaunchingWithOptions:launchOptions];
#endif

}

#if BWLOADINGMODE
- (void)launchViewWillHide:(NSTimer*)sender
{
    [UIView beginAnimations:@"hideLaunchView" context:nil];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(launchViewDidHide)];
    [UIView commitAnimations];
    
    // 读取存储的mode
#if BWPLAYNOTFLASHGAME
    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSNumber* modeNumber = (NSNumber*)[bwDictionary objectForKey:BWMODEKEY];
    
#if BWONSALEMODE

    // 读取存储的onSaleMode
    NSNumber* onSaleModeNumber = (NSNumber*)[bwDictionary objectForKey:BWONSALEKEY];
    BOOL onSaleModeEnable = NO;
    if (onSaleModeNumber)
        onSaleModeEnable = [onSaleModeNumber boolValue];
    if(onSaleModeEnable)
        return;
    else
    {
        NSDictionary *launchOptions = sender.userInfo;
        [[ShareGameInstance sharedGame]startGame];
    }

#endif
#endif

}

- (void)launchViewDidHide
{
    [self.navigationController popToRootViewControllerAnimated:NO];
}
#endif

#if BWPUSHMODE
- (void)openCustomURL
{
	if (customUrlString&&(customUrlType!=0))
	{
		switch (customUrlType) 
		{
			case 1:
			{
				void* mobileView = [[mainViewController clientView] mobileView];
				g_mobileViewConnect(mobileView, [customUrlString cStringUsingEncoding:NSUTF8StringEncoding]);
                mainViewController.isPushUrlOpen = YES;
			}
				break;
			case 2:
			case 3:
			{
                if (customUrlString)
                {
                    NSURL* customUrl = [NSURL URLWithString:customUrlString];
                    if ([[UIApplication sharedApplication] canOpenURL:customUrl])
                        [[UIApplication sharedApplication] openURL:customUrl];
                }
			}
				break;
            default:
                break;
		}
	}
}
#endif

#pragma mark - Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
#if BWPLAYNOTFLASHGAME
   [[ShareGameInstance sharedGame]applicationDidReceiveMemoryWarning:application];
#endif
    
}

- (void)dealloc 
{

#if BWPUSHMODE
	BWRELEASE(customUrlString);
#endif
#if BWPLAYNOTFLASHGAME
    [[ShareGameInstance sharedGame]releaseApplication];
#endif
    self.navigationController = nil;
    [mainViewController release];
    [window release];
    [super dealloc];
}

#pragma mark - RemoteNotification

#if BWPUSHMODE
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    BWLOG(@"userinfo:%@", [userInfo description]);
	BWRELEASE(customUrlString);
	customUrlType = 0;
    
	if ([[UIApplication sharedApplication] applicationIconBadgeNumber]!=0)
        [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
	
	if (userInfo!=NULL)
	{
		NSDictionary* aps = [userInfo objectForKey:@"aps"];
		NSDictionary* bw = [userInfo objectForKey:@"bw"];
		if (aps)
		{
			NSString* alert = [aps valueForKey:@"alert"];
			UIAlertView* alertView;
			if (bw)
			{
                customUrlString = [[NSString alloc] initWithString:[bw valueForKey:@"url"]];
//				customUrlString = [bw valueForKey:@"url"];
				customUrlType = [[bw valueForKey:@"urltype"] intValue];
				alertView = [[UIAlertView alloc] initWithTitle:nil message:alert delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
			}
			else 
				alertView = [[UIAlertView alloc] initWithTitle:nil message:alert delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
			
			if (isResignActive)
				[self openCustomURL];
			else 
			{
				[alertView show];
				[alertView release];
			}
		}
	}
#if BWPLAYNOTFLASHGAME
    [[ShareGameInstance sharedGame]application:application didReceiveRemoteNotification:userInfo];
#endif
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSString* token = [[deviceToken stringWithHexData] retain];
    [NSTimer scheduledTimerWithTimeInterval:15 target:self selector:@selector(saveToken:) userInfo:token repeats:NO];
    BWLOG(@"DeviceToken:%@",token);
#if BWPLAYNOTFLASHGAME
    [[ShareGameInstance sharedGame]application:application didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
#endif
}
-(void)saveToken:(NSTimer*)timer
{
    NSString *token = (NSString *)[timer userInfo];
	void* mobileView = [[mainViewController clientView] mobileView];
    
	g_saveDeviceToken(mobileView, (const char*)[token cStringUsingEncoding:NSUTF8StringEncoding]);
    [token release];

}
#endif

#pragma mark - UIAlertView Delegate

#if BWPUSHMODE
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex==[alertView firstOtherButtonIndex])
	{
		[[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
		[self openCustomURL];
	}
}
#endif

//支付宝回调

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
#if BWTHIRDPARTSDKISON
	[[ShareSDKInstance sharedSDK] handleAlixPayResult:url];
#endif
#ifdef BWWXSDKMODE
        [WXApi handleOpenURL:url delegate:g_explorerController_ios];
#endif
#if BWPLAYNOTFLASHGAME
	[[ShareGameInstance sharedGame] application:application handleOpenURL:url];
#endif
	return YES;
}
#if BWPLAYNOTFLASHGAME
-(void)applicationDidEnterBackground:(UIApplication*)application
{
    return [[ShareGameInstance sharedGame] applicationDidEnterBackground:application];
}

-(void)applicationWillEnterForeground:(UIApplication*)application
{
    return [[ShareGameInstance sharedGame] applicationWillEnterForeground:application];
}
-(void)applicationWillTerminate:(UIApplication *)application
{
    [[ShareGameInstance sharedGame] applicationWillTerminate:application];
}
-(void)applicationSignificantTimeChange:(UIApplication *)application
{
    [[ShareGameInstance sharedGame] applicationSignificantTimeChange:application];
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    return [[ShareGameInstance sharedGame] application:application didReceiveLocalNotification:notification];
}
- (void)applicationDidRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
    return [[ShareGameInstance sharedGame] applicationDidRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
}
- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
    return [[ShareGameInstance sharedGame] application:application didFailToRegisterForRemoteNotificationsWithError:error];
}
#endif
- (BOOL)application:(UIApplication*)application openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication annotation:(id)annotation
{
#if BWPLAYNOTFLASHGAME
    [[ShareGameInstance sharedGame] application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
#endif
#ifdef BWWXSDKMODE
    BOOL isSuc = [WXApi handleOpenURL:url delegate:g_explorerController_ios];
    BWLOG(@"url %@ isSuc %d",url,isSuc == YES ? 1 : 0);
    return  isSuc;
#endif
    return YES;
}
@end

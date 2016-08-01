//
//  HDLaunchView.m
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-15.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import "HDLaunchView.h"
#import "IPhoneDeviceData.h"
#import "BWLoadingView.h"

extern iOSDeviceRelateData g_deviceData;
@interface HDLaunchView (Private)

- (void)initSelf;

@end

@implementation HDLaunchView

- (id)initWithFrame:(CGRect)frame
{
    loadingImageTimer = nil;
    loadingImageNum = 0;
    self = [super initWithFrame:frame];
    if (self) {
        [self initSelf];
    }
    return self;
}

- (void)initSelf
{
    BOOL onsaleEnable = false;
    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSNumber* onSaleModeNumber = (NSNumber*)[bwDictionary objectForKey:BWONSALEKEY];
    if (onSaleModeNumber)
        onsaleEnable = [onSaleModeNumber boolValue];
    
    BOOL onWebGameEnable = YES;
    NSNumber* webGameModeNumber = (NSNumber*)[bwDictionary objectForKey:BWWEBGAMEKEY];
    if (webGameModeNumber)
    {
        //printf("onWebGame\n");
        onWebGameEnable = [webGameModeNumber boolValue];
    }
    
#ifdef __ZBShowLoading

#else/*__ZBShowLoading*/
    CGSize imageSize = [[UIImage imageNamed:@"1.png"] size];
#endif/*__ZBShowLoading*/
    CGFloat logoHeight;
    if (g_deviceData.m_isPad)
    {
        logoHeight = 110;
    }
    else
    {
        logoHeight = 55;
    }
    
    if (onsaleEnable)
    {
        if (onWebGameEnable || (![self isFileExist:@"jingpinloading.png"]))
        {
            //printf("WebGame\n");
            [self setImage:[UIImage imageNamed:@"loading_onsaleBackground.png"]];
            UIImageView * logoImagView;
            if (g_deviceData.m_isPad)
            {
                logoHeight = 110;
                logoImagView = [[UIImageView alloc]initWithFrame:CGRectMake((g_deviceData.m_screenWidth - 480)/2, (g_deviceData.m_screenHeight - 800)/2, 480, 800)];
            }
            else
            {
                logoHeight = 55;
                logoImagView = [[UIImageView alloc]initWithFrame:CGRectMake((g_deviceData.m_screenWidth - 240)/2, (g_deviceData.m_screenHeight - 400)/2, 240, 400)];
            }
            [logoImagView setImage:[UIImage imageNamed:@"loading_onsaleBaiWanLogo.png"]];
            [self addSubview:logoImagView];
            [logoImagView release];
            BWLoadingView *view = [[BWLoadingView alloc]init];
            view.viewOrentation = g_deviceData.m_deviceOrientation;
            view.scaleFactor = 1.0f;
            [self addSubview:view];
            [view initView];
        }
        else
        {
            [self setImage:[UIImage imageNamed:@"loading1.png"]];
            loadingImageTimer = [NSTimer scheduledTimerWithTimeInterval:1.5 target:self selector:@selector(setImage)  userInfo:nil repeats:YES];
//           [self setImage:[UIImage imageNamed:@"jingpinloading.png"]];
//            BWLoadingView *view = [[BWLoadingView alloc]init];
//            view.viewOrentation = g_deviceData.m_deviceOrientation;
//            view.scaleFactor = 1.0f;
//            [self addSubview:view];
//            [view initView];
        }
    }
    else
//#ifdef DJJDP
    {
        [self setImage:[UIImage imageNamed:@"loading1.png"]];
        loadingImageTimer = [NSTimer scheduledTimerWithTimeInterval:1.5 target:self selector:@selector(setImage)  userInfo:nil repeats:YES];
    }
//#else
//        [self setImage:[UIImage imageNamed:@"loading.png"]];
//#endif
   
#ifdef __ZBShowLoading

#else/*__ZBShowLoading*/
    loadingImageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, imageSize.width, imageSize.height)];
    
    NSArray* imageArray = [[NSArray alloc] initWithObjects:[UIImage imageNamed:@"1.png"],
                           [UIImage imageNamed:@"2.png"],
                           [UIImage imageNamed:@"3.png"],
                           [UIImage imageNamed:@"4.png"],
                           [UIImage imageNamed:@"5.png"],
                           [UIImage imageNamed:@"6.png"],
                           [UIImage imageNamed:@"7.png"],
                           [UIImage imageNamed:@"8.png"],
                           [UIImage imageNamed:@"9.png"],
                           nil];
    [loadingImageView setAnimationImages:imageArray];
    [loadingImageView setAnimationDuration:1.8];
    CGFloat x = self.bounds.size.width/2;
    CGFloat y = self.bounds.size.height*0.5+logoHeight;
    [loadingImageView setCenter:CGPointMake(x, y)];
//    [self addSubview:loadingImageView];
    [loadingImageView startAnimating];
    [loadingImageView release];
    [imageArray release];
#endif/*__ZBShowLoading*/
}
-(void) setImage
{
    switch (loadingImageNum) {
//        case 0:
//            loadingImageNum ++;
//            [self setImage:[UIImage imageNamed:@"loading1.png"]];
//            break;
        case 0:
            loadingImageNum ++;
            [self setImage:[UIImage imageNamed:@"loading2.png"]];
            break;
        case 1:
            loadingImageNum = 0;
            if (loadingImageTimer) {
                [loadingImageTimer invalidate];
                loadingImageTimer = nil;
            }
            [self setImage:[UIImage imageNamed:@"loading3.png"]];
            break;
    }
}
- (void)dealloc
{
    [super dealloc];
}

- (BOOL) isFileExist:(NSString *)fileName
{
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* resPath = [bundle resourcePath];
    NSString* filePath;
    filePath = [resPath stringByAppendingFormat:@"/%@",fileName];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL result = [fileManager fileExistsAtPath:filePath];
    return result;
}
@end

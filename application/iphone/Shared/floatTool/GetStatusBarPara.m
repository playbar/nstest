//
//  GetStatusBar.m
//  ExpandingBarExample
//
//  Created by xiepf on 14-7-24.
//  Copyright (c) 2014年 Topic Design. All rights reserved.
//

#import "GetStatusBarPara.h"

CGFloat UIInterfaceOrientationAngleOfOrientation(UIInterfaceOrientation orientation)
{
    CGFloat angle;
    
    switch (orientation)
    {
        case UIInterfaceOrientationPortraitUpsideDown:
            angle = M_PI;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            angle = -M_PI_2;
            break;
        case UIInterfaceOrientationLandscapeRight:
            angle = M_PI_2;
            break;
        default:
            angle = 0.0;
            break;
    }
    
    return angle;
}

@implementation GetStatusBarPara

+ (CGFloat)getStatusBarHeight
{
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if(UIInterfaceOrientationIsLandscape(orientation))
    {
        return [UIApplication sharedApplication].statusBarFrame.size.width;
    }
    else
    {
        return [UIApplication sharedApplication].statusBarFrame.size.height;
    }
}

+(CGRect)gainWindowBoundsWithoutStatusBar
{
    UIInterfaceOrientation statusBarOrientation = [UIApplication sharedApplication].statusBarOrientation;
    CGFloat statusBarHeight = [[self class] getStatusBarHeight];
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    CGRect validWindowBounds;
    validWindowBounds.origin.x = 0;
    validWindowBounds.origin.y = statusBarHeight;
    
    if (UIInterfaceOrientationIsLandscape(statusBarOrientation))
    {
        validWindowBounds.size.width = screenBounds.size.height;
        validWindowBounds.size.height = screenBounds.size.width - statusBarHeight;
    }
    else
    {
        validWindowBounds.size.width = screenBounds.size.width;
        validWindowBounds.size.height = screenBounds.size.height - statusBarHeight;
    }
    return validWindowBounds;
}

@end

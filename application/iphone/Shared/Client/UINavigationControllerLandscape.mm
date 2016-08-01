//
//  UINavigationControllerLandscape.m
//  BaiWan
//
//  Created by imac1 on 13-10-12.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//

#import "UINavigationControllerLandscape.h"
#import "GGBase.h"

extern iOSDeviceRelateData g_deviceData;

@interface UINavigationControllerLandscape ()

@end

@implementation UINavigationControllerLandscape


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return [[self.viewControllers lastObject] shouldAutorotateToInterfaceOrientation:interfaceOrientation];
}

- (BOOL)shouldAutorotate
{
    return [[self.viewControllers lastObject] shouldAutorotate];
}

- (NSUInteger)supportedInterfaceOrientations
{
    return [[self.viewControllers lastObject] supportedInterfaceOrientations];
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}
@end

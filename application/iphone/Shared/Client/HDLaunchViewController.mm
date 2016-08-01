//
//  HDLaunchViewController.m
//  BaiWan
//
//  Created by imac1 on 13-9-11.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//

#import "HDLaunchViewController.h"
#import "HDLaunchView.h"
#import "GGBase.h"

extern iOSDeviceRelateData g_deviceData;

@interface HDLaunchViewController ()

@end

@implementation HDLaunchViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        return interfaceOrientation == UIInterfaceOrientationPortrait;
    }
    else
    {
        return interfaceOrientation == UIInterfaceOrientationLandscapeRight;
    }
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    else
    {
        return UIInterfaceOrientationMaskLandscapeRight;
    }
}
- (BOOL)prefersStatusBarHidden
{
    return YES;
}
@end

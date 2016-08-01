//
//  historyViewController_iPhone.m
//  BaiWan
//
//  Created by zhengbin on 12-6-4.
//  Copyright (c) 2012年 JQBar. All rights reserved.
//

#import "historyViewController_ios.h"

@implementation historyViewController_ios

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view = [[[UITableView alloc]initWithFrame:CGRectMake(0, 0, 22, 22)]autorelease];
    // Do any additional setup after loading the view from its nib.
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end

//
//  UIViewController+popController.m
//  BaiWan
//
//  Created by imac1 on 14-2-14.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import "UIViewController+popController.h"
#define degreeToRadians(x) (3.1415926 * (x) / 180.0 )

@implementation UIViewController (popController)
-(void)presentPopViewController:(UIViewController *)presentedController animated:(BOOL)flag completion:(void (^)(void))completion
{
    
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 5.0) {
        [self presentViewController:presentedController animated:flag completion:^{
            [self calledMethodAfterPresentViewController];
        }];
        
    }else{
        [self presentModalViewController:presentedController animated:flag];
    }
    
}
-(void)calledMethodAfterPresentViewController
{
    self.view.transform = CGAffineTransformMakeRotation(degreeToRadians(0));
}
-(void)dismissPopViewControllerAnimated:(BOOL)flag completion:(void (^)(void))completion
{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 5.0)
    {
        [self dismissViewControllerAnimated:flag completion:completion];
    }else
    {
        [self dismissModalViewControllerAnimated:flag];
    }
}
@end

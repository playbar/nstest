//
//  UIViewController+popController.h
//  BaiWan
//
//  Created by imac1 on 14-2-14.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIViewController (popController)
-(void)presentPopViewController:(UIViewController *)presentedController animated:(BOOL)flag completion:(void (^)(void))completion;
-(void)calledMethodAfterPresentViewController;
-(void)dismissPopViewControllerAnimated:(BOOL)flag completion:(void (^)(void))completion;

@end

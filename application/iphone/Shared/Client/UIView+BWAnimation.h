//
//  UIView+BWAnimation.h
//  TestAnimation
//
//  Created by taomy on 14/11/18.
//  Copyright (c) 2014年 Liuyu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIView (BWAnimation)
-(void)beginAnimateViewWithPath:(NSArray *)point isAlpha:(BOOL)alphaFlag isScale:(BOOL)scaleFlag animateTime:(float)aniTime;
@end

//
//  UIView+BWAnimation.m
//  TestAnimation
//
//  Created by taomy on 14/11/18.
//  Copyright (c) 2014年 Liuyu. All rights reserved.
//

#import "UIView+BWAnimation.h"

@implementation UIView (BWAnimation)
-(void)beginAnimateViewWithPath:(NSArray *)point isAlpha:(BOOL)alphaFlag isScale:(BOOL)scaleFlag animateTime:(float)aniTime
{
    int pointNum = [point count];
    if (pointNum<2)
        return;
    
    NSMutableArray *animationOptions = [NSMutableArray array];
    //位移动画开始
    CAKeyframeAnimation *positionAnimation = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    [positionAnimation setDuration:aniTime];
    CGMutablePathRef path = CGPathCreateMutable();
    CGPathMoveToPoint(path, NULL, [[point objectAtIndex:0]floatValue], [[point objectAtIndex:1]floatValue]);
    for (int i = 0; i<(pointNum-2)/6; i++)
    {
        CGPathAddCurveToPoint(path, NULL, [[point objectAtIndex:2+i*6]floatValue], [[point objectAtIndex:2+i*6 + 1]floatValue], [[point objectAtIndex:2+i*6 + 2]floatValue], [[point objectAtIndex:2+i*6 + 3]floatValue], [[point objectAtIndex:2+i*6 + 4]floatValue], [[point objectAtIndex:2+i*6 + 5]floatValue]);
    }
    [positionAnimation setPath: path];
    CGPathRelease(path);
    [animationOptions addObject:positionAnimation];
    //位移动画结束
    
    //缩放动画开始
    if (scaleFlag)
    {
        CAKeyframeAnimation *scaleAnimation = [CAKeyframeAnimation animationWithKeyPath:@"transform.scale"];
        [scaleAnimation setValues:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f],[NSNumber numberWithFloat:1.0f], nil]];
        [scaleAnimation setDuration:aniTime];
        [scaleAnimation setKeyTimes:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f], [NSNumber numberWithFloat:1.0f], nil]];
        [animationOptions addObject:scaleAnimation];
    }
    //缩放动画结束
    
    //透明度动画开始
    if (alphaFlag)
    {
        CAKeyframeAnimation *alphAnimation = [CAKeyframeAnimation animationWithKeyPath:@"opacity"];
        [alphAnimation setValues:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f],[NSNumber numberWithFloat:1.0f], nil]];
        [alphAnimation setDuration:aniTime];
        [alphAnimation setKeyTimes:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f], [NSNumber numberWithFloat:1.0f], nil]];
        [animationOptions addObject:alphAnimation];
    }
    //透明度动画结束
    
    CAAnimationGroup *animationGroup = [CAAnimationGroup animation];
    [animationGroup setAnimations: animationOptions];
    [animationGroup setDuration:aniTime];
    [animationGroup setFillMode: kCAFillModeForwards];
    [animationGroup setTimingFunction: [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut]];
    
    [self.layer addAnimation:animationGroup forKey:@"bwanimate"];
    self.center = CGPointMake([[point objectAtIndex:pointNum-2]floatValue], [[point objectAtIndex:pointNum-1]floatValue]);
}
@end

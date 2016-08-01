//
//  UIView+BWactivity.h
//  BaiWanSDK_IOS
//
//  Created by imac7 on 14-8-19.
//  Copyright (c) 2014年 BaiWan. All rights reserved.
//

#import <UIKit/UIKit.h>
#pragma mark - 提示框类型
typedef NS_ENUM(NSUInteger, showHUDType) {
    ShowLoading = 0,//加载中
    ShowPhotoYes,//对号
    ShowPhotoNo,//叉子
    ShowDismiss,//删除
};

typedef void (^AnimationBlock)(UIView *animationView);
@interface UIView (BWactivity)
-(void)setIndicatorViewCenter:(CGPoint)center;
-(void)showHUDWithText:(NSString *)text Type:(showHUDType)type WithCenter:(CGPoint) center Enabled:(BOOL)enabled;
-(void)showHUDWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime;
@end

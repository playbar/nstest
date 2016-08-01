//
//  UIView+BWactivity.m
//  BaiWanSDK_IOS
//
//  Created by imac7 on 14-8-19.
//  Copyright (c) 2014年 BaiWan. All rights reserved.
//

#import "UIView+BWactivity.h"

#import "BWYzdHUDBackgroundView.h"
#import "BWYzdHUDImageView.h"
#import "BWYzdHUDIndicator.h"
#import "BWYzdHUDLabel.h"

#define YzdHUDBounds CGRectMake(0, 0, 100, 100)
#define YzdHUDCenter CGPointMake(self.bounds.size.width/2, self.bounds.size.height/2)
#define YzdHUDBackgroundAlpha 1
#define YzdHUDComeTime 0.15
#define YzdHUDStayTime 1
#define YzdHUDGoTime 0.15
#define YzdHUDFont 17
@implementation UIView (BWactivity)

-(void)setIndicatorViewCenter:(CGPoint)center
{
    [BWYzdHUDBackgroundView shareHUDView].center = center;
    [BWYzdHUDLabel shareHUDView].center = CGPointMake(center.x, center.y + YzdHUDBounds.size.height/3.5);
    [BWYzdHUDImageView shareHUDView].center = CGPointMake(center.x, center.y - YzdHUDBounds.size.height/6);
    [BWYzdHUDIndicator shareHUDView].center = CGPointMake(center.x, center.y - YzdHUDBounds.size.height/6);
}

-(void)showHUDWithText:(NSString *)text Type:(showHUDType)type WithCenter:(CGPoint) center Enabled:(BOOL)enabled{
   
   
    [self showHUDWithText:text Type:type Enabled:(BOOL)enabled Bounds:YzdHUDBounds Center:center BackgroundAlpha:YzdHUDBackgroundAlpha ComeTime:YzdHUDComeTime StayTime:YzdHUDStayTime GoTime:YzdHUDGoTime];
   
   
}
 static BOOL isShow = YES;
-(void)showHUDWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime{
    if (isShow) {
        isShow = NO;
        [self addSubview:[BWYzdHUDBackgroundView shareHUDView]];
        [self addSubview:[BWYzdHUDImageView shareHUDView]];
        [self addSubview:[BWYzdHUDLabel shareHUDView]];
        [self addSubview:[BWYzdHUDIndicator shareHUDView]];
    
        [BWYzdHUDBackgroundView shareHUDView].center = center;
        [BWYzdHUDLabel shareHUDView].center = CGPointMake(center.x, center.y + bounds.size.height/3.5);
        [BWYzdHUDImageView shareHUDView].center = CGPointMake(center.x, center.y - bounds.size.height/6);
        [BWYzdHUDIndicator shareHUDView].center = CGPointMake(center.x, center.y - bounds.size.height/6);
        [self goTimeBounds:bounds];
    }
    
    [BWYzdHUDLabel shareHUDView].bounds = CGRectMake(0, 0, bounds.size.width, bounds.size.height/2 - 10);
    if ([self textLength:text] * YzdHUDFont + 10 >= bounds.size.width) {
        [BWYzdHUDLabel shareHUDView].font = [UIFont systemFontOfSize:YzdHUDFont - 2];
    }
    
    self.userInteractionEnabled = enabled;
    
    switch (type) {
        case ShowLoading:
            [self showLoadingWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime];
            break;
        case ShowPhotoYes:
            [self showPhotoYesWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime];
            break;
        case ShowPhotoNo:
            [self showPhotoNoWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime];
            break;
        case ShowDismiss:
            [self showDismissWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime];
            break;
            
        default:
            break;
    }
    
}

-(void)showLoadingWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime{
    if ([BWYzdHUDBackgroundView shareHUDView].alpha != 0) {
        return;
    }
    
    [BWYzdHUDLabel shareHUDView].text = text;
    [[BWYzdHUDIndicator shareHUDView] stopAnimating];
    [BWYzdHUDImageView shareHUDView].alpha = 0;
    
    [UIView animateWithDuration:comeTime animations:^{
        [self comeTimeBounds:bounds];
        [self comeTimeAlpha:backgroundAlpha withImage:NO];
        [[BWYzdHUDIndicator shareHUDView] startAnimating];
    } completion:^(BOOL finished) {
        
    }];
}

-(void)showPhotoYesWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime{
    if ([[BWYzdHUDIndicator shareHUDView] isAnimating]) {
        [[BWYzdHUDIndicator shareHUDView] stopAnimating];
        
        [BWYzdHUDImageView shareHUDView].bounds =
        CGRectMake(0, 0, (bounds.size.width/2.5 - 5) * 2, (bounds.size.height/2.5 - 5) * 2);
    }else{
        if ([BWYzdHUDBackgroundView shareHUDView].alpha != 0) {
            return;
        }
        [self goTimeBounds:bounds];
        [self goTimeInit];
    }
    
    [BWYzdHUDLabel shareHUDView].text = text;
    NSBundle *bundle = [NSBundle bundleWithURL:[[NSBundle mainBundle] URLForResource:@"BaiWanPlatformKit" withExtension:@"bundle"]];
    
    [BWYzdHUDImageView shareHUDView].image =[UIImage imageWithContentsOfFile:[bundle pathForResource:@"HUD_YES" ofType:@"png"]];
    [UIView animateWithDuration:comeTime animations:^{
        [self comeTimeBounds:bounds];
        [self comeTimeAlpha:backgroundAlpha withImage:YES];
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:stayTime animations:^{
            [self stayTimeAlpha:backgroundAlpha];
        } completion:^(BOOL finished) {
            [UIView animateWithDuration:goTime animations:^{
                [self goTimeBounds:bounds];
                [self goTimeInit];;
            } completion:^(BOOL finished) {
                isShow=YES;
                //Nothing
            }];
        }];
    }];
}

-(void)showPhotoNoWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime{
    if ([[BWYzdHUDIndicator shareHUDView] isAnimating]) {
        [[BWYzdHUDIndicator shareHUDView] stopAnimating];
        
        [BWYzdHUDImageView shareHUDView].bounds =
        CGRectMake(0, 0, (bounds.size.width/2.5 - 5) * 2, (bounds.size.height/2.5 - 5) * 2);
    }else{
        if ([BWYzdHUDBackgroundView shareHUDView].alpha != 0) {
            return;
        }
        [self goTimeBounds:bounds];
        [self goTimeInit];
    }
    
    [BWYzdHUDLabel shareHUDView].text = text;
    NSBundle *bundle = [NSBundle bundleWithURL:[[NSBundle mainBundle] URLForResource:@"BaiWanPlatformKit" withExtension:@"bundle"]];
    
    [BWYzdHUDImageView shareHUDView].image =[UIImage imageWithContentsOfFile:[bundle pathForResource:@"HUD_NO" ofType:@"png"]];
  
    [UIView animateWithDuration:comeTime animations:^{
        [self comeTimeBounds:bounds];
        [self comeTimeAlpha:backgroundAlpha withImage:YES];
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:stayTime animations:^{
            [self stayTimeAlpha:backgroundAlpha];
        } completion:^(BOOL finished) {
            [UIView animateWithDuration:goTime animations:^{
                [self goTimeBounds:bounds];
                [self goTimeInit];;
            } completion:^(BOOL finished) {
                isShow=YES;
                //Nothing
            }];
        }];
    }];
}

-(void)showDismissWithText:(NSString *)text Type:(showHUDType)type Enabled:(BOOL)enabled Bounds:(CGRect)bounds Center:(CGPoint)center BackgroundAlpha:(CGFloat)backgroundAlpha ComeTime:(CGFloat)comeTime StayTime:(CGFloat)stayTime GoTime:(CGFloat)goTime{
    if ([[BWYzdHUDIndicator shareHUDView] isAnimating]) {
        [[BWYzdHUDIndicator shareHUDView] stopAnimating];
    }
    
    [BWYzdHUDLabel shareHUDView].text = nil;
    [BWYzdHUDImageView shareHUDView].image = nil;
    [UIView animateWithDuration:goTime animations:^{
        [BWYzdHUDImageView shareHUDView].bounds =
        CGRectMake(0, 0, (bounds.size.width/2.5 - 5) * 2, (bounds.size.height/2.5 - 5) * 2);
        [self goTimeBounds:bounds];
        [self goTimeInit];
    } completion:^(BOOL finished) {
        isShow=YES;
        //Nothing
    }];
}

#pragma mark 状态
-(void)goTimeBounds:(CGRect)bounds{
    [BWYzdHUDBackgroundView shareHUDView].bounds =
    CGRectMake(0, 0, bounds.size.width * 1.5, bounds.size.height * 1.5);
    [BWYzdHUDImageView shareHUDView].bounds =
    CGRectMake(0, 0, (bounds.size.width/2.5 - 5) * 2, (bounds.size.height/2.5 - 5) * 2);
}

-(void)goTimeInit{
    [BWYzdHUDBackgroundView shareHUDView].alpha = 0;
    [BWYzdHUDImageView shareHUDView].alpha = 0;
    [BWYzdHUDLabel shareHUDView].alpha = 0;
    [[BWYzdHUDIndicator shareHUDView] stopAnimating];
}

-(void)stayTimeAlpha:(CGFloat)alpha{
    [BWYzdHUDBackgroundView shareHUDView].alpha = alpha - 0.01;
}

-(void)comeTimeBounds:(CGRect)bounds{
    [BWYzdHUDBackgroundView shareHUDView].bounds =
    CGRectMake(0, 0, bounds.size.width, bounds.size.height);
    [BWYzdHUDImageView shareHUDView].bounds =
    CGRectMake(0, 0, bounds.size.width/2.5 - 5, bounds.size.height/2.5 - 5);
}

-(void)comeTimeAlpha:(CGFloat)alpha withImage:(BOOL)isImage{
    [BWYzdHUDBackgroundView shareHUDView].alpha = alpha;
    [BWYzdHUDLabel shareHUDView].alpha = 1;
    if (isImage) {
        [BWYzdHUDImageView shareHUDView].alpha = 1;
    }
}

#pragma mark - 计算字符串长度
- (int)textLength:(NSString *)text{
    float number = 0.0;
    for (int index = 0; index < [text length]; index++)
    {
        NSString *character = [text substringWithRange:NSMakeRange(index, 1)];
        
        if ([character lengthOfBytesUsingEncoding:NSUTF8StringEncoding] == 3)
        {
            number++;
        }
        else
        {
            number = number + 0.5;
        }
    }
    return ceil(number);
}
@end

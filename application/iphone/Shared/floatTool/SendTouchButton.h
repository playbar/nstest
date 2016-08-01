//
//  SendTouchButton.h
//  ExpandingBarExample
//
//  Created by xiepf on 14-7-23.
//  Copyright (c) 2014年 Topic Design. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol SendTouchButtonDelegate <NSObject>

-(void)BarAlphaChangeIncrease:(BOOL)isIncrese;
-(void)SenTouchButtonTouchBegain;
@end


@interface SendTouchButton : UIButton
{
    NSTimer *timer;
}
@property(nonatomic,assign)id<SendTouchButtonDelegate>delegate;
@property(nonatomic,assign)BOOL buttonIsShow;
@end

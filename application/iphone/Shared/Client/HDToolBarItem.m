//
//  HDToolBarItem.m
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-6.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import "HDToolBarItem.h"

@interface HDToolBarItem()

- (void)initSelf;

@end

@implementation HDToolBarItem

- (id)init
{
    self = [super init];
    if (self)
        [self initSelf];
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
        [self initSelf];
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self)
        [self initSelf];
    return self;
}

- (void)initSelf
{
    [self setContentHorizontalAlignment:UIControlContentHorizontalAlignmentCenter];
    [self setContentVerticalAlignment:UIControlContentVerticalAlignmentCenter];
}

- (void)setTitle:(NSString *)title forState:(UIControlState)state
{
    UIEdgeInsets imageEdgeInsets = [self imageEdgeInsets];
    [super setTitle:title forState:state];
    [self setImageEdgeInsets:imageEdgeInsets];
}

@end

@interface HDTimeLabel (PrivateMethod) 

- (void)initSelf;
// 刷新时间
- (void)updateTime;

@end

@implementation HDTimeLabel
@synthesize showSecond;
@synthesize isFlash;
    
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        [self initSelf];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self)
    {
        [self initSelf];
    }
    return self;
}

- (void)initSelf
{
    [self setTextColor:[UIColor whiteColor]];
    [self setBackgroundColor:[UIColor clearColor]];
    [self setTextAlignment:UITextAlignmentCenter];
    [self setBaselineAdjustment:UIBaselineAdjustmentAlignCenters];
    [self setAdjustsFontSizeToFitWidth:YES];
    updateTimer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(updateTime) userInfo:nil repeats:YES];
}

- (void)updateTime
{
    // 获取时间
    NSDate*date = [NSDate date];
    NSCalendar*calendar = [NSCalendar currentCalendar];
    NSDateComponents*comps;
    comps =[calendar components:(NSHourCalendarUnit | NSMinuteCalendarUnit |NSSecondCalendarUnit) fromDate:date];
    
    NSInteger hour = [comps hour];
    NSInteger minute = [comps minute];
    NSInteger second = [comps second];
    
    static BOOL showFlash;
    NSString* timeString;
    if (!showSecond)
    {
        timeString = [NSString stringWithFormat:@"%.2d:%.2d", hour,minute];
        [self setText:timeString];
    }
    else
    {
        if (!isFlash)
        {
            timeString = [NSString stringWithFormat:@"%.2d:%.2d:%.2d",hour, minute, second];
            [self setText:timeString];
        }
        else
        {
            timeString = showFlash?[NSString stringWithFormat:@"%.2d:%.2d:%.2d",hour,minute,second]:[NSString stringWithFormat:@"%.2d:%.2d %.2d",hour,minute,second];
            [self setText:timeString];
            showFlash = !showFlash;
        }
    }
}

- (void)dealloc
{
    [updateTimer invalidate];
    updateTimer = nil;
    
    [super dealloc];
}

@end

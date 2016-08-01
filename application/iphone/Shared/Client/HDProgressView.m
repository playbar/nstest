//
//  HDProgressView.m
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-6.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import "HDProgressView.h"

@interface HDProgressView ()

- (void)initSelf;

@end

@implementation HDProgressView

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
    progress = 0.0f;
    [self setBackgroundColor:[UIColor clearColor]];
}

- (void)setProgress:(int)aProgress
{
    progress = aProgress;
    if (progress>100)
        progress = 100;
    if (progress<0)
        progress = 0;
    [self setNeedsDisplay];
}

- (void)setProgressTint:(UIColor*)color
{
    if (color)
    {
        if (progressTint)
        {
            [progressTint release];
            progressTint = nil;
        }
        
        progressTint = [color retain];
    }
}

- (void)setTrackTint:(UIColor*)color
{
    if (color)
    {
        [self setBackgroundColor:color];
    }
}

- (float)progress
{
    return progress;
}

- (UIColor*)progressTint
{
    return progressTint;
}

- (UIColor*)trackTint
{
    return [self backgroundColor];
}

- (void)drawRect:(CGRect)rect
{
    // calc progress frame
    CGRect progressFrame = CGRectMake(0, 0, [self bounds].size.width*progress/100, [self bounds].size.height);
    
    CGContextRef gc = UIGraphicsGetCurrentContext();
    CGContextSaveGState(gc);
    
    const CGFloat* color = CGColorGetComponents([progressTint CGColor]);
    if (color)
        CGContextSetFillColor(gc, color);
    CGContextFillRect(gc, progressFrame);
    
    CGContextRestoreGState(gc);
}

- (void)dealloc
{
    if (progressTint)
        [progressTint release];
    [super dealloc];
}

@end

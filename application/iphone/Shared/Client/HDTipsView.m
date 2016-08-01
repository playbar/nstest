//
//  HDTipsView.m
//  BaiWan
//
//  Created by 红桃 XII on 11-12-13.
//  Copyright (c) 2011年 JQBar. All rights reserved.
//

#import "HDTipsView.h"

@interface HDTipsView (Private)

- (void)initSelf;
- (void)didShowView;
- (void)didHideView;

@end

@implementation HDTipsView

- (id)init
{
    self = [super init];
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

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
        [self initSelf];
    return self;
}

- (id)initWithText:(NSString*)text
{
    self = [super init];
    if (self)
    {
        [self initSelf];
        [self setText:text];
    }
    return self;
}

- (void)initSelf
{
    [self setBackgroundColor:[UIColor clearColor]];
    [self setTextAlignment:UITextAlignmentCenter];
    [self setNumberOfLines:3];
    [self setTextColor:[UIColor whiteColor]];
    autoHide = YES;
}

- (void)drawRect:(CGRect)rect
{
    // 灰色背景
    CGContextRef gc = UIGraphicsGetCurrentContext();
    CGContextSaveGState(gc);
    CGRect bound = [self bounds];
    float ovalWidth = 5;
    float ovalHeight = 5;
    CGContextTranslateCTM(gc, CGRectGetMinX(bound), CGRectGetMinY(bound));
    CGContextScaleCTM(gc, ovalWidth ,ovalHeight);
    float fw = CGRectGetWidth(bound)/ovalWidth;
    float fh = CGRectGetHeight(bound)/ovalHeight;
    CGContextMoveToPoint(gc,fw,fh/2);
    CGContextAddArcToPoint(gc, fw, fh, fw/2, fh, 1);
    CGContextAddArcToPoint(gc, 0, fh, 0, fh/2, 1);
    CGContextAddArcToPoint(gc, 0, 0, fw/2, 0, 1);
    CGContextAddArcToPoint(gc, fw, 0, fw, fh/2, 1);
    CGContextClosePath(gc);
    CGFloat black[4]={0.5f,0.5f,0.5f,0.75f};
    CGContextSetFillColor(gc, black);
    CGContextFillPath(gc);
    CGContextRestoreGState(gc);
    
    CGContextSaveGState(gc);
    bound.origin.x += 2;
    bound.origin.y += 2;
    bound.size.width -= 4;
    bound.size.height -= 4;
    float ovalWidth1 = 3;
    float ovalHeight1 = 3;
    CGContextTranslateCTM(gc, CGRectGetMinX(bound), CGRectGetMinY(bound));
    CGContextScaleCTM(gc, ovalWidth1 ,ovalHeight1);
    fw = CGRectGetWidth(bound)/ovalWidth1;
    fh = CGRectGetHeight(bound)/ovalHeight1;
    CGContextMoveToPoint(gc,fw,fh/2);
    CGContextAddArcToPoint(gc, fw, fh, fw/2, fh, 1);
    CGContextAddArcToPoint(gc, 0, fh, 0, fh/2, 1);
    CGContextAddArcToPoint(gc, 0, 0, fw/2, 0, 1);
    CGContextAddArcToPoint(gc, fw, 0, fw, fh/2, 1);
    CGContextClosePath(gc);
    CGFloat black1[4]={0.0f,0.0f,0.0f,0.75f};
    CGContextSetFillColor(gc, black1);
    CGContextFillPath(gc);
    
    CGContextRestoreGState(gc);
    
    [super drawRect:rect];
}

- (void)showInView:(UIView*)aView
{
    if (self.superview)
        return;
    
    CGRect bounds = [aView bounds];
    CGRect frame = CGRectZero;
    CGSize fontSize = [self.text sizeWithFont:[self font]];
    int lineNumber = (fontSize.width/200)+1;
    if (fontSize.height!=0)
        frame.size = CGSizeMake(200, (fontSize.height+2)*lineNumber);
    CGPoint point = CGPointMake(bounds.size.width/2, bounds.size.height*0.45);
    [self setFrame:frame];
    [self setCenter:point];
    [self setAlpha:0];
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDelay:0.5];
    [UIView setAnimationDuration:0.5];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(didShowView)];
    [aView addSubview:self];
    [self setAlpha:1];
    [UIView commitAnimations];
}

- (void)didShowView
{
    if (autoHide)
        [self hideView];
}

- (void)hideView
{
    if (!self.superview)
        return;
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(didHideView)];
    [UIView setAnimationDuration:0.5];
    if (autoHide)
    {
        [UIView setAnimationDelay:1];
    }
    [self setAlpha:0];
    [UIView commitAnimations];
}

- (void)didHideView
{
    [self removeFromSuperview];
}

- (void)setAutoHide:(BOOL)hide
{
    autoHide = hide;
}

- (BOOL)isAutoHide
{
    return autoHide;
}

@end

//
//  HDWidget.m
//  BaiWan
//
//  Created by 红桃 XII on 11-11-30.
//  Copyright (c) 2011年 JQBar. All rights reserved.
//

#import "HDWidget.h"
#import "HDWidgetView.h"

@interface HDWidget (Private)

- (UIEdgeInsets)rectToEdge:(CGRect)rect;
- (CGRect)edgeToRect:(UIEdgeInsets)edge;

- (void)initSelf;

@end

@implementation HDWidget

#pragma mark - UIControl

+ (id)widget
{
    HDWidget* widget = [HDWidget buttonWithType:UIButtonTypeCustom];
    [widget initSelf];
    return widget;
}

- (void)initSelf
{
    [self setContentHorizontalAlignment:UIControlContentHorizontalAlignmentCenter];
    [self setContentVerticalAlignment:UIControlContentVerticalAlignmentCenter];
}

- (void)dealloc
{
    [super dealloc];
}

- (void)setFrame:(CGRect)frame
{
    [super setFrame:frame];
    
    [self setTitleEdgeInsets:UIEdgeInsetsMake(0.0, -self.imageView.image.size.width, self.titleLabel.frame.size.height-self.imageView.frame.size.height+10, 0.0)];
    UIEdgeInsets imageEdge = [self imageEdgeInsets];
    imageEdge.right = 0;
    [self setImageEdgeInsets:imageEdge];
    [[self titleLabel] setFont:[UIFont systemFontOfSize:13]];
    [[self titleLabel] setTextAlignment:UITextAlignmentCenter];
    [[self titleLabel] setBaselineAdjustment:UIBaselineAdjustmentAlignCenters];
    [[self imageView] setBounds:frame];
    [[self imageView] setContentMode:UIViewContentModeCenter];
}

- (void)setBounds:(CGRect)bounds
{
    [super setBounds:bounds];
    
    [self setTitleEdgeInsets:UIEdgeInsetsMake(0.0, -self.imageView.image.size.width, self.titleLabel.frame.size.height-self.imageView.frame.size.height, 0.0)];
}

- (void)setMoveEnabled:(BOOL)enabled
{
    isMoveEnabled = enabled;
}

- (BOOL)moveEnabled
{
    return isMoveEnabled;
}

#pragma mark -
- (UIEdgeInsets)rectToEdge:(CGRect)rect
{
    return UIEdgeInsetsMake(rect.origin.y, rect.origin.x, 
                            rect.size.height+rect.origin.y, 
                            rect.size.width+rect.origin.x);
}

- (CGRect)edgeToRect:(UIEdgeInsets)edge;
{
    return CGRectMake(edge.left, edge.top, 
                      edge.right-edge.left, 
                      edge.bottom-edge.top);
}

#pragma mark - touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    
    if (isMoveEnabled)
    {
        if (self.superview)
            [self.superview touchesBegan:touches withEvent:event];
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    if (isMoveEnabled)
    {
        if (self.superview)
            [self.superview touchesMoved:touches withEvent:event];
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (isMoveEnabled)
    {
        if ([(HDWidgetView*)self.superview startMoved])
        {
            if (self.superview)
                [self.superview touchesEnded:touches withEvent:event];
        
            [self touchesCancelled:touches withEvent:event];
            return;
        }
    }
    [super touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesCancelled:touches withEvent:event];
    
    if (isMoveEnabled)
    {
        if (self.superview)
            [self.superview touchesCancelled:touches withEvent:event];
    }
}

@end

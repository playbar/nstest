//
//  HDToolBarView.m
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-6.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import "HDToolBarView.h"
#import "HDProgressView.h"

@interface HDToolBarView()

- (void)initSelf;

@end

@implementation HDToolBarView
@synthesize delegate;

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
    {
        [self initSelf];
        [self setBackgroundImage:[UIImage imageNamed:@"toolbarBackground.png"]];
    }
    return self;
}

- (void)initSelf
{
    [self setBackgroundColor:[UIColor clearColor]];
    backgroundView = [[UIImageView alloc] init];
    percentView = [[HDProgressView alloc] init];
    CGRect totalFrame = [self frame];
    CGRect percentFrame = CGRectMake(0, 0, totalFrame.size.width, 2);
    CGRect backgroundFrame = CGRectMake(0, 2, totalFrame.size.width, totalFrame.size.height-2);
    [backgroundView setFrame:backgroundFrame];
    [percentView setFrame:percentFrame];
    [self addSubview:percentView];
    [self insertSubview:backgroundView atIndex:0];
    [percentView setHidden:YES];
    privatePoint = CGPointZero;
}

- (void)setItemsCount:(int)count
{
    itemsCount = count;
}

- (int)itemsCount
{
    return itemsCount;
}

- (void)setBackgroundImage:(UIImage*)image
{
    if (image&&backgroundView)
    {
        [backgroundView setImage:image];
    }
}

- (UIImage*)backgroundImage
{
    if (backgroundView)
        return [backgroundView image];
    else
        return nil;
}

- (void)addItem:(HDToolBarItem*)item
{
    if (item)
    {
        for (int i=0; i<itemsCount; i++)
        {
            if (!toolBarItems[i])
            {
                toolBarItems[i] = item;
                CGRect frame = [item frame];
                CGPoint centerPoint = CGPointMake((self.bounds.size.width/(itemsCount+1))*(i+1), self.bounds.size.height/2+2);
                [item setFrame:frame];
                [item setCenter:centerPoint];
                [self addSubview:(UIView*)item];
                break;
            }
        }
    }
}

-(void) setFrame:(CGRect)frame
{
    [super setFrame:frame];
    for (int i=0; i<itemsCount; i++)
    {
 //       if (!toolBarItems[i])
 //       {
 //           toolBarItems[i] = item;
 //           CGRect frame = [item frame];
            CGPoint centerPoint = CGPointMake((self.bounds.size.width/(itemsCount+1))*(i+1), self.bounds.size.height/2+2);
 //           [item setFrame:frame];
            [toolBarItems[i] setCenter:centerPoint];
 //           [self addSubview:(UIView*)item];
 //           break;
 //       }
    }
}
- (void)addItems:(NSArray*)item
{
    for (HDToolBarItem* toolBarItem in item)
    {
        for (int i=0; i<itemsCount; i++)
        {
            if (!toolBarItems[i])
            {
                toolBarItems[i] = toolBarItem;
                CGFloat width = [self bounds].size.width/itemsCount;
                CGRect frame = [toolBarItem frame];
                frame.origin.y = 2;
                frame.origin.x = width*i;
                [toolBarItem setFrame:frame];
                [self addSubview:(UIView*)toolBarItem];
                break;
            }
        }
    }
}

- (void)replaceItem:(HDToolBarItem*)item atIndex:(int)index
{
    if (item&&index<itemsCount)
    {
        if (toolBarItems[index])
        {
            [toolBarItems[index] removeFromSuperView];
            toolBarItems[index] = nil;
        }
        toolBarItems[index] = item;
        CGFloat width = [self bounds].size.width/itemsCount;
        CGRect frame = [item frame];
        frame.origin.y = 2;
        frame.origin.x = width*index;
        [item setFrame:frame];
        [self addSubview:(UIView*)item];
    }
}

- (void)removeItems
{
    for (int i=0;i<itemsCount;i++)
    {
        if (toolBarItems[i])
        {
            [toolBarItems[i] removeFromSuperview];
            toolBarItems[i] = nil;
        }
    }
}

- (void)showPercent:(BOOL)show
{
    [percentView setHidden:!show];
}

- (void)setPercent:(float)aPercent
{
    if (percentView)
    {
        [percentView setProgress:aPercent];
    }
//    if(aPercent >= 100)
//    {
//        [self showPercent:NO];
//    }
}

- (void)setPercentTint:(UIColor*)color
{
    if (percentView)
        [percentView setProgressTint:color];
}

- (void)setTrackTint:(UIColor*)color
{
    if (percentView)
        [percentView setTrackTint:color];
}

- (float)precent
{
    if (percentView)
        return [percentView progress];
    else
        return 0.0f;
}

- (UIColor*)precentTint
{
    if (percentView)
        return [percentView progressTint];
    else
        return nil;
}

- (UIColor*)trackTint
{
    if (percentView)
        return [percentView trackTint];
    else
        return nil;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [touches anyObject];
    if (touch)
    {
        CGPoint superEndPoint = [touch locationInView:self.superview];
        // 如果做的是一个向上移动的动作
        if (superEndPoint.y < privatePoint.y)
        {
            if (!CGRectContainsPoint(self.frame, superEndPoint))
                [delegate moveupFromBottom];
        }
        
        privatePoint = superEndPoint;
    }
}

@end

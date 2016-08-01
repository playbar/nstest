//
//  HDWidgetView.m
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-5.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import "HDWidgetView.h"
#import "HDWidget.h"
#import "BaiWanViewController_ios.h"
#import "IPhoneDeviceData.h"

extern iOSDeviceRelateData g_deviceData;

@interface HDWidgetView()

- (void)initSelf:(HDWidgetViewType)aType;
- (void)calcBound;

@end

@implementation HDWidgetView
@synthesize viewController;
@synthesize delegate;

- (id)init
{
    self = [super init];
    if (self) {
        [self initSelf:HDWidgetViewTypeDefault];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self)
    {
        [self initSelf:HDWidgetViewTypeDefault];
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self initSelf:HDWidgetViewTypeDefault];
    }
    return self;
}

- (id)initWithType:(HDWidgetViewType)aType
{
    self = [super init];
    if (self)
    {
        [self initSelf:aType];
    }
    return self;
}

- (void)initSelf:(HDWidgetViewType)aType
{
    type = aType;
    currentBottomRightRow = -1;
    currentBottomRightColumn = -1;
    [self setBackgroundColor:[UIColor clearColor]];
    privatePoint = CGPointZero;
}

- (void)drawRect:(CGRect)rect
{
    // drawBackground
    CGContextRef gc = UIGraphicsGetCurrentContext();
    CGContextSaveGState(gc);
    CGRect bound = [self bounds];
    float ovalWidth = 10;
    float ovalHeight = 10;
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
    CGFloat black[4]={0.0f,0.0f,0.0f,0.7f};
    CGContextSetFillColor(gc, black);
    CGContextFillPath(gc);
    CGContextRestoreGState(gc);
}

- (void)addWidget:(HDWidget*)widget row:(int)row column:(int)column
{
    if (column<HDWidgetViewMaxColumns &&
        row<HDWidgetViewMaxRows &&
        widget)
    {
        if (type==HDWidgetViewTypeDefault)
        {
            HDWidget* subWidget = subWidgets[row][column];
            if (subWidget)
            {
                [subWidget removeFromSuperview];
                subWidget = nil;
            }
            subWidgets[row][column] = widget;
            int subWidgetWidth = [self bounds].size.width/HDWidgetViewMaxColumns;
            int subWidgetHeight = [self bounds].size.height/HDWidgetViewMaxRows;
            [self addSubview:widget];
            [widget setFrame:CGRectMake(subWidgetWidth*column, 
                                        subWidgetHeight*row, 
                                        subWidgetWidth, 
                                        subWidgetWidth)];
        }
        else if (type==HDWidgetViewTypeAuto)
        {
            HDWidget* subWidget = subWidgets[row][column];
            if (subWidget)
            {
                [subWidget removeFromSuperview];
                subWidget = nil;
            }
            subWidgets[row][column] = widget;
            [self addSubview:widget];
            if (currentBottomRightRow<row || currentBottomRightColumn<column)
            {
                currentBottomRightRow = row;
                currentBottomRightColumn = column;
                [self calcBound];
            }
        }
    }
}

- (void)calcBound
{
    int height=0;
    int width=0;
    for (int i=0; i<=currentBottomRightRow; i++)
    {
        int maxHeight = 0;
        for (int j=0; j<=currentBottomRightColumn; j++)
        {
            HDWidget* widget = subWidgets[i][j];
            if (widget)
                maxHeight = MAX(maxHeight, [widget bounds].size.height);
        }
        height += maxHeight;
    }
    for (int j=0; j<=currentBottomRightColumn; j++)
    {
        int maxWidth = 0;
        for (int i=0; i<=currentBottomRightRow; i++)
        {
            HDWidget* widget = subWidgets[i][j];
            if (widget)
                maxWidth = MAX(maxWidth, [widget bounds].size.width);
        }
        width += maxWidth;
    }
    
    self.bounds = CGRectMake(0, 0, width, height);
    int subWidgetWidth = width/(currentBottomRightColumn+1);
    int subWidgetHeight = height/(currentBottomRightRow+1);
    for (int i=0; i<=currentBottomRightRow; i++)
    {
        for (int j=0; j<=currentBottomRightColumn; j++)
        {
            HDWidget* widget = subWidgets[i][j];
            if (widget)
                [widget setFrame:CGRectMake(subWidgetWidth*j, subWidgetHeight*i, subWidgetWidth, subWidgetHeight)];
        }
    }
}

- (void)removeWidgetOfRow:(int)row column:(int)column
{
    if (type==HDWidgetViewTypeDefault)
    {
    if (column<HDWidgetViewMaxColumns &&
        row<HDWidgetViewMaxRows)
    {
        HDWidget* subWidget = subWidgets[row][column];
        if (subWidget)
        {
            [subWidget removeFromSuperview];
            subWidget = nil;
        }
    }
    }
    else if (type==HDWidgetViewTypeAuto)
    {
        
    }
}

- (void)removeAllWidgets
{
    for (int i=0; i<HDWidgetViewMaxRows; i++)
    {
        for (int j=0;j<HDWidgetViewMaxColumns; j++)
        {
            HDWidget* subWidget = subWidgets[i][j];
            if (subWidget)
            {
                [subWidget removeFromSuperview];
                subWidget = nil;
            }
        }
    }
}

- (void)setMoveEnabled:(BOOL)enabled
{
    isMoveEnabled = enabled;
}

- (BOOL)moveEnabled
{
    return isMoveEnabled;
}

- (BOOL)startMoved
{
    return isStartMoved;
}

- (void)moveWidget:(HDWidget *)widget row:(int)row column:(int)column
{
    if (column<HDWidgetViewMaxColumns &&
        row<HDWidgetViewMaxRows &&
        widget)
    {
        int subWidgetWidth = [self bounds].size.width/HDWidgetViewMaxColumns;
        int subWidgetHeight = [self bounds].size.height/HDWidgetViewMaxRows;
        
        subWidgets[row][column] = widget;
        [widget setFrame:CGRectMake(subWidgetWidth*column, subWidgetHeight*row, subWidgetWidth, subWidgetWidth)];
    }
}

- (void)addWidgets:(HDWidget*)widget row:(int)row column:(int)column
{
    if (column<HDWidgetViewMaxColumns &&
        row<HDWidgetViewMaxRows &&
        widget)
    {
        if (type==HDWidgetViewTypeDefault)
        {
            HDWidget* subWidget = subWidgets[row][column];
            subWidgets[row][column] = widget;
            int subWidgetWidth = [self bounds].size.width/HDWidgetViewMaxColumns;
            int subWidgetHeight = [self bounds].size.height/HDWidgetViewMaxRows;
            [self addSubview:widget];
            [widget setFrame:CGRectMake(subWidgetWidth*column,
                                        subWidgetHeight*row,
                                        subWidgetWidth,
                                        subWidgetWidth)];
        }
        else if (type==HDWidgetViewTypeAuto)
        {
            HDWidget* subWidget = subWidgets[row][column];
            subWidgets[row][column] = widget;
            [self addSubview:widget];
            if (currentBottomRightRow<row || currentBottomRightColumn<column)
            {
                currentBottomRightRow = row;
                currentBottomRightColumn = column;
                [self calcBound];
            }
        }
    }
}

#pragma mark - Touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
//    [super touchesBegan:touches withEvent:event];
    if (isMoveEnabled)
    {
        UITouch* touch = [touches anyObject];
        if (touch)
        {
            startPoint = [touch locationInView:self];
        }
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (isMoveEnabled)
    {
        UITouch* touch = [touches anyObject];
        if (touch)
        {
            CGPoint movePoint = [touch locationInView:self];
            if ((ABS(movePoint.x-startPoint.x)>20)||(movePoint.y-startPoint.y)>20)
            {
                isStartMoved = YES;
            }
        
            if (isStartMoved)
            {
                CGRect newFrame = [self frame];
                newFrame.origin.x += movePoint.x-startPoint.x;
                newFrame.origin.y += movePoint.y-startPoint.y;
                
                CGRect superFrame = [[self.viewController clientView] bounds];
                if (newFrame.origin.x <0)
                    newFrame.origin.x = 0;
                if (newFrame.origin.x + newFrame.size.width > superFrame.size.width)
                    newFrame.origin.x = superFrame.size.width-newFrame.size.width;
                if (newFrame.origin.y<0)
                    newFrame.origin.y = 0;
                if (newFrame.origin.y + newFrame.size.height > superFrame.size.height)
                    newFrame.origin.y = superFrame.size.height-newFrame.size.height;
                
                [self setFrame:newFrame];
                
                CGPoint superEndPoint = [touch locationInView:self.superview];
//#ifdef __IPHONE__
//                if (superEndPoint.y > privatePoint.y)
//                {
//                    CGRect buttomRect = CGRectMake(0,g_deviceData.m_screenHeight-30,g_deviceData.m_screenWidth,30);
//                    if (CGRectIntersectsRect(buttomRect, self.frame))
//                    {
//                        if ([delegate respondsToSelector:@selector(moveDownToBottom)])
//                            [delegate moveDownToBottom];
//                    }
//                }
//#elif defined(__IPAD__)
                if (superEndPoint.y > privatePoint.y)
                {
                    CGRect buttomRect = CGRectMake(0,g_deviceData.m_screenHeight-30,g_deviceData.m_screenWidth,30);
                    if (CGRectIntersectsRect(buttomRect, self.frame))
                    {
                        if ([delegate respondsToSelector:@selector(moveDownToBottom)])
                            [delegate moveDownToBottom];
                    }
                }
//#endif
                privatePoint = superEndPoint;
            }
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (isMoveEnabled)
    {
        isStartMoved = NO;
        UITouch* touch = [touches anyObject];
        if (touch)
        {
//            CGPoint superEndPoint = [touch locationInView:self.superview];
//            if (CGRectContainsPoint(CGRectMake(0, 276, 480, 44), superEndPoint))
//                [delegate endToBottom];
        }
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
//    [super touchesCancelled:touches withEvent:event];
    if (isMoveEnabled)
    {
        isStartMoved = NO;
    }
}

@end

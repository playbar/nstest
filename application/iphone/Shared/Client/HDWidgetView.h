//
//  HDWidgetView.h
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-5.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HDWidget;
//@class BaiWanViewController_iPhone;
@class BaiWanViewController_ios;
@protocol HDWidgetViewDelegate;

#define HDWidgetViewMaxColumns 3
#define HDWidgetViewMaxRows 3

typedef enum
{
    HDWidgetViewTypeDefault,    // 默认模式下，暂时以3x3来存放widget
    HDWidgetViewTypeAuto        // 自动模式下，会自动计算存放widget需要的bound
}HDWidgetViewType;

@interface HDWidgetView : UIView
{
    HDWidget* subWidgets[HDWidgetViewMaxRows][HDWidgetViewMaxColumns];
    int currentBottomRightColumn;
    int currentBottomRightRow;
    
    HDWidgetViewType type;
    
    CGPoint startPoint;
    BOOL isStartMoved;
    BOOL isMoveEnabled;
    // 前一个点，用于moveDownToBottom
    CGPoint privatePoint;
//#ifdef __IPHONE__
//    BaiWanViewController_iPhone* viewController;
//#else
    BaiWanViewController_ios* viewController;
//#endif
    id<HDWidgetViewDelegate> delegate;
}

//#ifdef __IPHONE__
//@property (nonatomic, retain) IBOutlet BaiWanViewController_iPhone* viewController;
//#else
@property (nonatomic, retain) BaiWanViewController_ios* viewController;
//#endif
@property (nonatomic, retain) id<HDWidgetViewDelegate> delegate;

- (id)initWithType:(HDWidgetViewType)aType;
- (void)addWidget:(HDWidget*)widget row:(int)row column:(int)column;
- (void)removeWidgetOfRow:(int)row column:(int)column;
- (void)removeAllWidgets;
- (void)setMoveEnabled:(BOOL)enabled;
- (BOOL)moveEnabled;
- (BOOL)startMoved;

// 重新设置widget位置
- (void)moveWidget:(HDWidget*)widget row:(int)row column:(int)column;
// 同一个位置添加多个widget
- (void)addWidgets:(HDWidget*)widget row:(int)row column:(int)column;    

@end

@protocol HDWidgetViewDelegate <NSObject>

- (void)moveDownToBottom;

@end

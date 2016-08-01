//
//  HDToolBarView.h
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-6.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HDProgressView;
@class HDToolBarItem;
@protocol HDToolBarViewDelegate;

@interface HDToolBarView : UIView
{
    UIImageView* backgroundView;
    HDProgressView* percentView;
    HDToolBarItem* toolBarItems[6];
    int itemsCount;
    
    id<HDToolBarViewDelegate> delegate;
    // 纪录前一个点，用户moveupFromBottom
    CGPoint privatePoint;
}

@property (nonatomic, retain) id<HDToolBarViewDelegate> delegate;

- (void)setItemsCount:(int)count;
- (int)itemsCount;
- (void)setBackgroundImage:(UIImage*)image;
- (UIImage*)backgroundImage;
- (void)addItem:(HDToolBarItem*)item;
- (void)addItems:(NSArray*)item;
- (void)replaceItem:(HDToolBarItem*)item atIndex:(int)index;
- (void)removeItems;

- (void)showPercent:(BOOL)show;
- (void)setPercent:(float)aProgress;
- (void)setPercentTint:(UIColor*)color;
- (void)setTrackTint:(UIColor*)color;

- (float)progress;
- (UIColor*)progressTint;
- (UIColor*)trackTint;


@end

@protocol HDToolBarViewDelegate <NSObject>

- (void)moveupFromBottom;

@end

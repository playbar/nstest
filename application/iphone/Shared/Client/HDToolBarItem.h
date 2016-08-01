//
//  HDToolBarItem.h
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-6.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "HDWidget.h"

@interface HDToolBarItem : HDWidget

@end

@interface HDTimeLabel : UILabel 
{
    // 刷新的timer
    NSTimer* updateTimer;
    // 是否显示秒
    BOOL showSecond;
    // 是否闪动
    BOOL isFlash;
}

@property (nonatomic) BOOL showSecond;
@property (nonatomic) BOOL isFlash;

@end
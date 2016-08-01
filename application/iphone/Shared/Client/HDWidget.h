//
//  HDWidget.h
//  BaiWan
//
//  Created by 红桃 XII on 11-11-30.
//  Copyright (c) 2011年 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HDWidget : UIButton
{
    BOOL isMoveEnabled;
}
// 获取type类型的widget
+ (id)widget;
- (void)setMoveEnabled:(BOOL)enabled;
- (BOOL)moveEnabled;

@end

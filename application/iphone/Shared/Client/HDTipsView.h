//
//  HDTipsView.h
//  BaiWan
//
//  Created by 红桃 XII on 11-12-13.
//  Copyright (c) 2011年 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HDTipsView : UILabel
{
    BOOL autoHide;
}


- (id)initWithText:(NSString*)text;
- (void)showInView:(UIView*)aView;
- (void)hideView;
- (void)setAutoHide:(BOOL)hide;
- (BOOL)isAutoHide;

@end

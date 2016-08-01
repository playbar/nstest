//
//  HDProgressView.h
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-6.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HDProgressView : UIView
{
    int progress;
    UIColor* progressTint;
}

- (void)setProgress:(int)aProgress;
- (void)setProgressTint:(UIColor*)color;
- (void)setTrackTint:(UIColor*)color;

- (float)progress;
- (UIColor*)progressTint;
- (UIColor*)trackTint;

@end

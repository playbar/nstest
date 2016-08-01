//
//  GetStatusBar.h
//  ExpandingBarExample
//
//  Created by xiepf on 14-7-24.
//  Copyright (c) 2014年 Topic Design. All rights reserved.
//

#import <Foundation/Foundation.h>

CGFloat UIInterfaceOrientationAngleOfOrientation(UIInterfaceOrientation orientation);

@interface GetStatusBarPara : NSObject
+ (CGFloat)getStatusBarHeight;
+(CGRect)gainWindowBoundsWithoutStatusBar;
@end

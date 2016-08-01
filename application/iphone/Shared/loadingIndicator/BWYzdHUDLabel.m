//
//  YzdHUDLabel.m
//  YzdHUD
//
//  Created by ShineYang on 13-12-6.
//  Copyright (c) 2013年 YangZhiDa. All rights reserved.
//

#import "BWYzdHUDLabel.h"

static BWYzdHUDLabel *_shareHUDView = nil;
@implementation BWYzdHUDLabel

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

+(BWYzdHUDLabel *)shareHUDView{
    if (!_shareHUDView) {
        _shareHUDView = [[BWYzdHUDLabel alloc] init];
        _shareHUDView.numberOfLines = 0;
        _shareHUDView.alpha = 0;
        _shareHUDView.textAlignment = NSTextAlignmentCenter;
        _shareHUDView.backgroundColor = [UIColor clearColor];
        _shareHUDView.textColor = [UIColor whiteColor];

    }
    return _shareHUDView;
}
@end

//
//  YzdHUDImageView.m
//  YzdHUD
//
//  Created by ShineYang on 13-12-6.
//  Copyright (c) 2013年 YangZhiDa. All rights reserved.
//

#import "BWYzdHUDImageView.h"

static BWYzdHUDImageView *_shareHUDView = nil;
@implementation BWYzdHUDImageView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

+(BWYzdHUDImageView *)shareHUDView{
    if (!_shareHUDView) {
        _shareHUDView = [[BWYzdHUDImageView alloc] init];
        _shareHUDView.alpha = 0;
    }
    return _shareHUDView;
}


@end

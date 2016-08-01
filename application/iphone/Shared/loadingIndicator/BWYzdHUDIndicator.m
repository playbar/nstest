//
//  YzdHUDIndicator.m
//  YzdHUD
//
//  Created by ShineYang on 13-12-6.
//  Copyright (c) 2013年 YangZhiDa. All rights reserved.
//

#import "BWYzdHUDIndicator.h"

static BWYzdHUDIndicator *_shareHUDView = nil;
@implementation BWYzdHUDIndicator

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

+(BWYzdHUDIndicator *)shareHUDView{
    if (!_shareHUDView) {
        _shareHUDView = [[BWYzdHUDIndicator alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    }
    return _shareHUDView;
}

@end

//
//  UIImage+thumbnailProcessMethod.h
//  BaiWan
//
//  Created by imac1 on 14-1-15.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (thumbnailProcessMethod)
+ (UIImage *)thumbnailWithImageWithoutScale:(UIImage *)image size:(CGSize)asize;
@end

//
//  LoadingView.h
//  TestAnimation
//
//  Created by taomy on 14/11/18.
//  Copyright (c) 2014年 Liuyu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface BWLoadingView : UIView
{
    UIImageView *backGround;
    float screenWidth;
    float screenHeight;
    float xToMove;
    float yToMove;
}
@property float scaleFactor;
@property UIInterfaceOrientation viewOrentation;
-(void)initView;
@end

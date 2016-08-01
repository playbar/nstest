//
//  HDLaunchView.h
//  WidgetDemo
//
//  Created by 红桃 XII on 11-12-15.
//  Copyright (c) 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
//#define DJJDP //大将军单品

@interface HDLaunchView : UIImageView
{
    UIImageView* loadingImageView;
    NSTimer*     loadingImageTimer;
    int          loadingImageNum;
}

@end

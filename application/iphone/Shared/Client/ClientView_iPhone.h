//
//  ClientView_iPhone.h
//  BaiWan
//
//  Created by handong on 11-6-9.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BaiWanViewController_iPhone;
@class AccelerometerFilter;

// 多点触控序列的MaxSize
const int MultiTouchMaxSize_iPhone = 2;

@interface ClientView_iPhone : UIView <UIAccelerometerDelegate> 
{
    void* m_mobileView;
    void* m_graphics;
    
    NSTimer* m_timer;
    NSTimer* m_idleTimer;

    bool m_inited;
    bool m_paused;
    bool mbFlash;
    
    BaiWanViewController_iPhone* m_viewController;

    AccelerometerFilter* filter;
	
	// multitouch array
	NSMutableArray* multiTouchArray;
	UITouch* invalidTouch;
	UIImageView* aImageView;
    NSTimer* aTimer;
    CGPoint startPoint;
    
    BOOL mouseShowEnable;
    BOOL startScroll;
    BOOL onsaleEnable;
}

@property (nonatomic) CGPoint lastPoint;
@property (nonatomic) BOOL mouseShowEnable;

- (void)start;
- (void)stop;
- (void)noticeFlashStart;
- (void)noticeFlashFinish;

- (void)setViewController:(BaiWanViewController_iPhone*)viewController;
- (BaiWanViewController_iPhone*)viewController;

- (void*)graphics;
- (void*)mobileView;
- (void*)subMobileView;

- (void)clearAll;
- (void)drawInContext:(CGContextRef)context;

- (void)cleanupMultiTouchArray;

- (void)aimageView_hide:(NSTimer *)timer;

@end

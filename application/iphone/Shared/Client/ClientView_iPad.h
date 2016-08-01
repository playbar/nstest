//
//  ClientView_iPad.h
//  BaiWan
//
//  Created by handong on 11-6-10.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BaiWanViewController_iPad;
@class AccelerometerFilter;

// 多点触控序列的MaxSize
const int MultiTouchMaxSize_iPad = 2;

@interface ClientView_iPad : UIView <UIAccelerometerDelegate> 
{
    void* m_mobileView;
    void* m_graphics;
    
    NSTimer* m_timer;
    NSTimer* m_idleTimer;
    
    bool m_inited;
    bool m_paused;
    bool mbFlash;
    
    BaiWanViewController_iPad* m_viewController;

    AccelerometerFilter* filter;
	
	// multitouch array
	NSMutableArray* multiTouchArray;
	UITouch* invalidTouch;
	UIImageView* aImageView;
    NSTimer* aTimer;
    CGPoint lastPoint;
    
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

- (void)setViewController:(BaiWanViewController_iPad*)viewController;
- (BaiWanViewController_iPad*)viewController;

- (void*)graphics;
- (void*)mobileView;
- (void*)subMobileView;

- (void)clearAll;
- (void)drawInContext:(CGContextRef)context;

- (void)cleanupMultiTouchArray;

- (void)aimageView_hide:(NSTimer *) timer;

@end

//
//  ClientView_iPad.m
//  BaiWan
//
//  Created by handong on 11-6-10.
//  Copyright 2011 JQBar. All rights reserved.
//

#import "ClientView_iPad.h"
#import "AccelerometerFilter.h"
#import "BaiWanViewController_iPad.h"
#import "UIDeviceAdvance.h"
//#ifdef __HDRetinaText
#import <QuartzCore/QuartzCore.h>
//#endif

#include "GlobalPortingFunction.h"
#include "GGBase.h"
#include "KeyBoardMiddleLayer.h"

extern iOSDeviceRelateData g_deviceData;

#define kUpdateFrequency 60

@interface ClientView_iPad ()

- (void)initMobileView;

- (void)onTimer;
- (void)onIdleTimer;
- (void)startTimer;
- (void)stopTimer;

- (void)changeFilter:(Class)filterClass;

// Event Handling
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
@end

@implementation ClientView_iPad
@synthesize lastPoint;
@synthesize mouseShowEnable;

XString8 g_documentDirPath_iPad;
XString8 g_bundlePath_iPad;

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    [self initMobileView];
    [self layoutSubviews];
    return self;
}

- (void)initMobileView
{
    m_paused = true;
    mbFlash = false;
    m_mobileView = NULL;
	multiTouchArray = [[NSMutableArray alloc] init];
	invalidTouch = [[UITouch alloc] init];
    if (self!=nil) 
	{
        m_inited = false;
        self.backgroundColor = [UIColor clearColor];
        self.opaque = YES;
        self.clearsContextBeforeDrawing = YES;
        self.multipleTouchEnabled = YES;
        self.userInteractionEnabled = YES;
    }
    [self changeFilter:[LowpassFilter class]];
    
    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:1.0/kUpdateFrequency];
    [[UIAccelerometer sharedAccelerometer] setDelegate:self];
	aImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"touch.png"]];
	[m_viewController.clientView addSubview:aImageView];
	[aImageView setHidden:YES];
    
    // 读取存储的鼠标指示状态
    mouseShowEnable = YES;
    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSNumber* pointNumber = (NSNumber*)[bwDictionary objectForKey:BWPOINTKEY];
    if (pointNumber)
        mouseShowEnable = [pointNumber boolValue];

    //onsaleEnable = true;
    NSNumber* onSaleModeNumber = (NSNumber*)[bwDictionary objectForKey:BWONSALEKEY];
    if (onSaleModeNumber)
    {
#ifdef __USE_RETINA_SCREEN__
        onsaleEnable = NO;
#else
        onsaleEnable = [onSaleModeNumber boolValue];
#endif
        g_deviceData.mbOnSale = onsaleEnable;
    }
    else
    {
#ifdef __USE_RETINA_SCREEN__
        onsaleEnable = NO;
#else
        onsaleEnable = g_deviceData.mbOnSale;
#endif
    }
}

- (void)layoutSubviews
{
    if (!m_inited)
    {
        m_inited = true;
        XInitGGBase();
        CGRect viewBounds = [self bounds];
        m_graphics = g_newGraphics(2048, 1536);
        char buffer[512];
        memset(buffer, 0, 512);
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentationDirectory, NSUserDomainMask, YES);
        if ([paths count]>0)
        {
            NSString* documentsDirectory = [paths objectAtIndex:0];
            [documentsDirectory getCString:buffer maxLength:512 encoding:NSUTF8StringEncoding];
        }
        g_documentDirPath_iPad = buffer;
        
        NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
        [bundlePath getCString:buffer maxLength:512 encoding:NSUTF8StringEncoding];
        g_bundlePath_iPad = buffer;
        
        m_mobileView = g_newMobileView(viewBounds.origin.x, viewBounds.origin.y, viewBounds.size.width, viewBounds.size.height, 
                                       m_graphics, self, XResource::LoadWString(XIDS_TITLE), "");
        g_setMAC(m_mobileView,[[[UIDevice currentDevice] MACAddress] cStringUsingEncoding:NSUTF8StringEncoding]);

        [self start];
        return;
    }
    
    XRect xrect;
    CGRect viewBounds = [self bounds];
    xrect = XRect(viewBounds.origin.x, viewBounds.origin.y, viewBounds.origin.x+viewBounds.size.width, viewBounds.origin.y+viewBounds.size.height);
   
    if (!g_deviceData.mbOnSale && g_deviceData.m_scale > 1.1 )
    {
        [[self layer] setContentsScale:g_deviceData.m_scale];
          xrect += xrect;
    }
    g_viewMoveWindow(m_mobileView, &xrect);
    [[self layer] setNeedsDisplay];
}

- (void)start
{
    m_paused = false;
    [self startTimer];
}

- (void)stop
{
    m_paused = true;
    [self stopTimer];
}

- (void)noticeFlashStart
{
    mbFlash = true;
    XRect xrect;
    int l,t,w,h;
    CGRect viewBounds = [self bounds];
    xrect = XRect(viewBounds.origin.x, viewBounds.origin.y, viewBounds.origin.x+viewBounds.size.width, viewBounds.origin.y+viewBounds.size.height);
    xrect += xrect;
    g_viewMoveWindow(m_mobileView, &xrect);
}

- (void)noticeFlashFinish
{
    mbFlash = false;
    XRect xrect;
    int l,t,w,h;
    CGRect viewBounds = [self bounds];
    xrect = XRect(viewBounds.origin.x, viewBounds.origin.y, viewBounds.origin.x+viewBounds.size.width, viewBounds.origin.y+viewBounds.size.height);
    
    g_viewMoveWindow(m_mobileView, &xrect);
}

- (void)startTimer
{
    [self stopTimer];
    m_timer = [NSTimer scheduledTimerWithTimeInterval:0.04
                                               target:self 
                                             selector:@selector(onTimer)
                                             userInfo:nil
                                              repeats:YES];
//    g_invokeOnTimer(m_mobileView);
//    if ([m_viewController currentInterfaceOrientation]==UIInterfaceOrientationLandscapeRight)
//        g_sendAccelerometer(m_mobileView, -filter.y, filter.x, filter.z);
//    else if ([m_viewController currentInterfaceOrientation]==UIInterfaceOrientationLandscapeLeft)
//        g_sendAccelerometer(m_mobileView, filter.y, filter.x, filter.z);
    
    m_idleTimer = [NSTimer scheduledTimerWithTimeInterval:0.01 
                                                   target:self 
                                                 selector:@selector(onIdleTimer) 
                                                 userInfo:nil 
                                                  repeats:YES];
}

- (void)stopTimer
{
    [m_timer invalidate];
    m_timer = nil;
    [m_idleTimer invalidate];
    m_idleTimer = nil;
}

- (void)onTimer
{
    g_invokeOnTimer(m_mobileView);
    if ([m_viewController currentInterfaceOrientation]==UIInterfaceOrientationLandscapeRight)
        g_sendAccelerometer(m_mobileView, -filter.y, filter.x, filter.z);
    else if ([m_viewController currentInterfaceOrientation]==UIInterfaceOrientationLandscapeLeft)
        g_sendAccelerometer(m_mobileView, filter.y, filter.x, filter.z);
}

- (void)onIdleTimer
{
    g_sendOnIdle(m_mobileView);
}

- (void)setFrame:(CGRect)frame
{
    [super setFrame:frame];
}

- (void)setViewController:(BaiWanViewController_iPad*)viewController
{
    m_viewController = viewController;
}

- (BaiWanViewController_iPad*)viewController
{
    return m_viewController;
}

- (void*)graphics
{
    return m_graphics;
}

- (void*)mobileView
{
    return m_mobileView;
}

- (void)setMouseShowEnable:(BOOL)enable
{
    mouseShowEnable = enable;
    // 存储鼠标指示状态
    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
    NSMutableDictionary* bwMutableDictionary = [NSMutableDictionary dictionaryWithDictionary:bwDictionary];
    [bwMutableDictionary setObject:[NSNumber numberWithBool:mouseShowEnable] forKey:BWPOINTKEY];
    [[NSUserDefaults standardUserDefaults] setObject:bwMutableDictionary forKey:BWUSERDEFAULTKEY];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)clearAll
{
    [self stopTimer];
    if (m_mobileView)
    {
        g_deleteMobileView(m_mobileView);
        m_mobileView = NULL;
    }
}

- (void)drawInContext:(CGContextRef)context
{
    CGRect viewBounds = [self bounds];
    g_drawToContext(context, viewBounds.size.width, viewBounds.size.height, m_mobileView, 1);
}

/*
 虚拟键盘唤出后不会补发touchBegin对应的touchend或touchcancel
 所以需要手动清空当前的点击队列
 2011/09/08
 */
- (void)cleanupMultiTouchArray
{
	// 补发touchEnd
	for (int pos=0; pos<[multiTouchArray count]; pos++)
	{
		UITouch* touch = [multiTouchArray objectAtIndex:pos];
		if (touch!=invalidTouch)
		{
			CGPoint touchPoint = [touch locationInView:self];
            if (mbFlash && !onsaleEnable && g_deviceData.m_scale > 1.1)
            {
                g_sendEndMouse(m_mobileView, touchPoint.x * g_deviceData.m_scale, touchPoint.y * g_deviceData.m_scale, pos);
            }
            else
            {
                g_sendEndMouse(m_mobileView, touchPoint.x, touchPoint.y, pos);
            }
		}
	}

	[multiTouchArray removeAllObjects];
}

#ifdef __AddDrawRect // zhengjl 2012.11.13
-(void)drawRect:(CGRect)rect
{
    // UIView uses the existence of -drawRect: to determine if should allow its CALayer
    // to be invalidated, which would then lead to the layer creating a backing store and
    // -drawLayer:inContext: being called.
    // By implementing an empty -drawRect: method, we allow UIKit to continue to implement
    // this logic, while doing our real drawing work inside of -drawLayer:inContext:
}
#endif // __AddDrawRect

-(void)drawLayer:(CALayer*)layer inContext:(CGContextRef)context
{
	UIGraphicsPushContext(context);
	[self drawInContext:context];
	UIGraphicsPopContext();
}

- (void)dealloc 
{
    [self clearAll];
	[invalidTouch release];
	invalidTouch = nil;
	[multiTouchArray release];
	multiTouchArray = nil;
	[aImageView release];
	aImageView = nil;
    XReleaseGGBase();
    [super dealloc];
}

- (void) SetNormalScale
{
    [[self layer] setContentsScale:1];
    XRect xrect;
    CGRect viewBounds = [self bounds];
    xrect = XRect(viewBounds.origin.x, viewBounds.origin.y, viewBounds.origin.x+viewBounds.size.width, viewBounds.origin.y+viewBounds.size.height);
    g_viewMoveWindow(m_mobileView, &xrect);
    //g_MVInvalidate( m_mobileView );
}

#pragma mark -
#pragma mark Event Handling
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [m_viewController hideMoreBar];

	NSArray* eventTouches = [touches allObjects];
	for (UITouch* touch in eventTouches)
	{
		if ([multiTouchArray containsObject:invalidTouch])
		{
            int index = [multiTouchArray indexOfObject:invalidTouch];
			if (index!=NSNotFound) {
                [multiTouchArray replaceObjectAtIndex:[multiTouchArray indexOfObject:invalidTouch] withObject:touch];
            }
			CGPoint touchPoint = [touch locationInView:self];
			int touchPos = [multiTouchArray indexOfObject:touch];
            if (mouseShowEnable)
            {
#if BWONSALEMODE
                if (!onsaleEnable)
                {
                    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
                    NSNumber* onSaleModeNumber = (NSNumber*)[bwDictionary objectForKey:BWONSALEKEY];
                    if (onSaleModeNumber)
#ifdef __USE_RETINA_SCREEN__
                        onsaleEnable = NO;
#else
                        onsaleEnable = [onSaleModeNumber boolValue];
#endif
                    g_deviceData.mbOnSale = onsaleEnable;
                }
                if (onsaleEnable)
                {
                    [aImageView setHidden:NO];
                    [aImageView setCenter:touchPoint];
                }
#else
                [aImageView setHidden:NO];
                [aImageView setCenter:touchPoint];
#endif
            }
            if (mbFlash && !onsaleEnable && g_deviceData.m_scale > 1.1)
            {
                g_sendBeginMouse(m_mobileView, touchPoint.x * g_deviceData.m_scale, touchPoint.y * g_deviceData.m_scale, touchPos);
            }
            else
            {
                g_sendBeginMouse(m_mobileView, touchPoint.x, touchPoint.y, touchPos);
            }
		}
		else if ([multiTouchArray count]<MultiTouchMaxSize_iPad)
		{
			[multiTouchArray addObject:touch];
			CGPoint touchPoint = [touch locationInView:self];
            int touchPos = [multiTouchArray indexOfObject:touch];
            if (mouseShowEnable)
            {
#if BWONSALEMODE
                if (!onsaleEnable)
                {
                    NSDictionary* bwDictionary = [[NSUserDefaults standardUserDefaults] objectForKey:BWUSERDEFAULTKEY];
                    NSNumber* onSaleModeNumber = (NSNumber*)[bwDictionary objectForKey:BWONSALEKEY];
                    if (onSaleModeNumber)
#ifdef __USE_RETINA_SCREEN__
                        onsaleEnable = NO;
#else
                        onsaleEnable = [onSaleModeNumber boolValue];
#endif
                     g_deviceData.mbOnSale = onsaleEnable;
                    if (onsaleEnable )
                    {
                        [[self layer] setContentsScale:1];
                        XRect xrect;
                        CGRect viewBounds = [self bounds];
                        xrect = XRect(viewBounds.origin.x, viewBounds.origin.y, viewBounds.origin.x+viewBounds.size.width, viewBounds.origin.y+viewBounds.size.height);
                        g_viewMoveWindow(m_mobileView, &xrect);
                    }
                }
                if (onsaleEnable)
                {
                    [aImageView setHidden:NO];
                    [aImageView setCenter:touchPoint];
                }
#else
                [aImageView setHidden:NO];
                [aImageView setCenter:touchPoint];
#endif
            }
            [m_viewController.clientView addSubview:aImageView];
            if (mbFlash && !onsaleEnable && g_deviceData.m_scale > 1.1)
            {
                g_sendBeginMouse(m_mobileView, touchPoint.x * g_deviceData.m_scale, touchPoint.y * g_deviceData.m_scale, touchPos);
            }
            else
            {
                g_sendBeginMouse(m_mobileView, touchPoint.x, touchPoint.y, touchPos);
            }
		}
		else
		{
			break;
		}
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSArray* eventTouches = [touches allObjects];
	for (UITouch* touch in eventTouches)
	{
		if ([multiTouchArray containsObject:touch])
		{
			CGPoint touchPoint = [touch locationInView:self];
            int touchPos = [multiTouchArray indexOfObject:touch];
            [aImageView setCenter:touchPoint];
            if (mbFlash && !onsaleEnable && g_deviceData.m_scale > 1.1)
            {
                g_sendMoveMouse(m_mobileView, touchPoint.x * g_deviceData.m_scale, touchPoint.y * g_deviceData.m_scale, touchPos);
            }
            else
            {
                g_sendMoveMouse(m_mobileView, touchPoint.x, touchPoint.y, touchPos);
            }
		}
		else 
			continue;
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (mouseShowEnable)
    {
    if (aTimer) 
    {
        [aTimer invalidate];
        aTimer = nil;
    }

        aTimer = [NSTimer scheduledTimerWithTimeInterval:0.3
                                                  target:self 
                                                selector:@selector(aimageView_hide:)
                                                userInfo:nil
                                                 repeats:NO];
    }
	NSArray* eventTouches = [touches allObjects];
	for (UITouch* touch in eventTouches)
	{
		if ([multiTouchArray containsObject:touch])
		{
			CGPoint touchPoint = [touch locationInView:self];
            lastPoint = touchPoint;
			int touchPos = [multiTouchArray indexOfObject:touch];
            if (mbFlash && !onsaleEnable && g_deviceData.m_scale > 1.1)
            {
                g_sendEndMouse(m_mobileView, touchPoint.x * g_deviceData.m_scale, touchPoint.y * g_deviceData.m_scale, touchPos);
            }
            else
            {
                g_sendEndMouse(m_mobileView, touchPoint.x, touchPoint.y, touchPos);
            }
			//g_sendEndMouse(m_mobileView, touchPoint.x, touchPoint.y, touchPos);
			int index = [multiTouchArray indexOfObject:touch];
			if (index!=NSNotFound)
            {
                [multiTouchArray replaceObjectAtIndex:[multiTouchArray indexOfObject:touch] withObject:invalidTouch];
            }
            else
            {
            
            }
        }
		else 
			continue;
	}
}

- (void)aimageView_hide:(NSTimer *) timer
{
    [aImageView setHidden:YES];
    aTimer = nil;
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSArray* eventTouches = [touches allObjects];
	for (UITouch* touch in eventTouches)
	{
		if ([multiTouchArray containsObject:touch])
		{
			CGPoint touchPoint = [touch locationInView:self];
			int touchPos = [multiTouchArray indexOfObject:touch];
			g_sendCancelMouse(m_mobileView, touchPoint.x, touchPoint.y, touchPos);
            int index = [multiTouchArray indexOfObject:touch];
			if (index!=NSNotFound) {
                [multiTouchArray replaceObjectAtIndex:[multiTouchArray indexOfObject:touch] withObject:invalidTouch];
            }else{
            
            }
		}
		else 
			continue;
	}
}

#pragma mark -
#pragma mark Acceleration
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
    [filter addAcceleration:acceleration];
}
- (void)changeFilter:(Class)filterClass
{
    if (filterClass!=[filter class]) 
    {
        [filter release];
        filter = [[filterClass alloc] initWithSampleRate:kUpdateFrequency cutoffFrequency:5.0];
        filter.adaptive = NO;
    }
}
#pragma mark -


@end

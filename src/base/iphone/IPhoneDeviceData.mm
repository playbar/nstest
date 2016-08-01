/*
 *  IPhoneDeviceData.cpp
 *  BaiWan
 *
 *  Created by lining on 10-11-3.
 *  Modified by handong on 11-05-10.
 *  Copyright 2010 JQBar. All rights reserved.
 *
 */

#include "IPhoneDeviceData.h"
#include <string.h>
#include "string_comp.h"
#import "UIDeviceAdvance.h"

NSString* KiPhoneSimulator = @"x86_64";
NSString* KiPhone = @"iPhone1,1";
NSString* KiPhone3G = @"iPhone1,2";
NSString* KiPhone3GS = @"iPhone2,1";
NSString* KiPhone4 = @"iPhone3,1";
NSString* KiPhone4VZ = @"iPhone3,3";
NSString* KiPodTouch = @"iPod1,1";
NSString* KiPodTouch2 = @"iPod2,1";
NSString* KiPodTouch3 = @"iPod3,1";
NSString* KiPodTouch4 = @"iPod4,1";
NSString* KiPad = @"iPad1,1";
NSString* KiPad2WIFI = @"iPad2,1";
NSString* KiPad2GSM = @"iPad2,2";
NSString* KiPad2CDMA = @"iPad2,3";
NSString* KAppleTV2G = @"AppleTV2,1";

#define isRetina ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 960), [[UIScreen mainScreen] currentMode].size) : NO)	

iOSDeviceRelateData::iOSDeviceRelateData()
{
    
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
    NSString* deviceMachine = [[[UIDevice currentDevice] machine] retain];
    if ([deviceMachine isEqualToString:KiPhoneSimulator]) 
        m_type = iPhoneSimulator;
    else if ([deviceMachine isEqualToString:KiPhone])
        m_type = iPhone;
    else if ([deviceMachine isEqualToString:KiPhone3G])
        m_type = iPhone3G;
    else if ([deviceMachine isEqualToString:KiPhone3GS])
        m_type = iPhone3GS;
    else if ([deviceMachine isEqualToString:KiPhone4])
        m_type = iPhone4;
    else if ([deviceMachine isEqualToString:KiPhone4VZ])
        m_type = iPhone4VZ;
    else if ([deviceMachine isEqualToString:KiPodTouch])
        m_type = iPodTouch;
    else if ([deviceMachine isEqualToString:KiPodTouch2])
        m_type = iPodTouch2;
    else if ([deviceMachine isEqualToString:KiPodTouch3])
        m_type = iPodTouch3;
    else if ([deviceMachine isEqualToString:KiPodTouch4])
        m_type = iPodTouch4;
    else if ([deviceMachine isEqualToString:KiPad])
        m_type = iPad;
    else if ([deviceMachine isEqualToString:KiPad2WIFI])
        m_type = iPad2WIFI;
    else if ([deviceMachine isEqualToString:KiPad2GSM])
        m_type = iPad2GSM;
    else if ([deviceMachine isEqualToString:KiPad2CDMA])
        m_type = iPad2CDMA;
    else if ([deviceMachine isEqualToString:KAppleTV2G])
        m_type = AppleTV2G;
    else 
        m_type = iPhone3GS;
        
    [deviceMachine release];
	
	switch (m_type) {
        case iPhoneSimulator:
            m_fontMultiplier = 1;
            
            if (UI_USER_INTERFACE_IDIOM()!=UIUserInterfaceIdiomPad)
			{
                m_isPad = false;
				m_defaultFontSize = 4;
			}
            else 
			{
                m_isPad = true;
				m_defaultFontSize = 8;
			}
            break;
        case iPhone:
        case iPhone3G:
        case iPhone3GS:
        case iPhone4:
        case iPhone4VZ:
        case iPodTouch:
        case iPodTouch2:
        case iPodTouch3:
        case iPodTouch4:
            m_fontMultiplier = 1;
            m_defaultFontSize = 4;
            m_isPad = false;
            break;
        case iPad:
        case iPad2WIFI:
        case iPad2GSM:
        case iPad2CDMA:
        case AppleTV2G:
            m_fontMultiplier = 1;
			m_defaultFontSize = 8;
            m_isPad = true;
            break;

		default:
            APPLEASSERT(false);
			break;
	}
	
	m_configName = "config";
	m_supportMultiTask = false;
    
    mbOnSale = false;
	m_isWebGame = false;
    m_isDomView = false;
    m_isDomExtView = false;
	m_scale = [[UIScreen mainScreen] scale];
    
#if __IPHONE_OS_VERSION_MIN_REQUIRED > 40000
    UIDevice* device = [UIDevice currentDevice];
	if ([device respondsToSelector:@selector(isMultitaskingSupported)])
	{
		m_supportMultiTask = true;
	}
#endif

    NSLog(@"_____Device is %d", [[UIDevice currentDevice] userInterfaceIdiom]);
    //IPHONE和IPAD设备方向支持
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        m_isPad = YES;
    }
    else
    {
        m_isPad = NO;
    }
    
    if (m_isPad)
    {
#if IPAD_LANDSCAPE_IPHONE_LANDSCAPE || IPAD_LANDSCAPE_IPHONE_PORTRAIT
        m_deviceOrientation = UIInterfaceOrientationLandscapeRight;
#elif IPAD_PORTRAIT_IPHONE_PORTRAIT || IPAD_PORTRAIT_IPHONE_LANDSCAPE
        m_deviceOrientation =  UIInterfaceOrientationPortrait;
#endif
    }
    else
    {
#if IPAD_LANDSCAPE_IPHONE_LANDSCAPE || IPAD_PORTRAIT_IPHONE_LANDSCAPE
        m_deviceOrientation = UIInterfaceOrientationLandscapeRight;
#elif IPAD_PORTRAIT_IPHONE_PORTRAIT || IPAD_LANDSCAPE_IPHONE_PORTRAIT
        m_deviceOrientation =  UIInterfaceOrientationPortrait;
#endif
    }
//    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
//        m_deviceOrientation = UIInterfaceOrientationLandscapeRight;
	[pool release];
}

iOSDeviceRelateData::~iOSDeviceRelateData()
{
	
}
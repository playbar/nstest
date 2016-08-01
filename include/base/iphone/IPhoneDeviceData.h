/*
 *  IPhoneDeviceData.h
 *  jqbar
 *
 *  Created by lining on 10-11-3.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _IPHONE_DEVICE_DATA_H_
#define _IPHONE_DEVICE_DATA_H_

typedef enum iOSDeviceType
{
	iPhoneSimulator,
	iPhone,
	iPhone3G,
	iPhone3GS,
    iPhone4,
    iPhone4VZ,
	iPodTouch,
    iPodTouch2,
    iPodTouch3,
    iPodTouch4,
    iPad,
    iPad2WIFI,
    iPad2GSM,
    iPad2CDMA,
    AppleTV2G
}iOSDeviceType;

class iOSDeviceRelateData
{
public:
	iOSDeviceRelateData();
	~iOSDeviceRelateData();
	
	iOSDeviceType m_type;
	const char* m_configName;
	float m_fontMultiplier;
	int m_defaultFontSize;
	bool m_supportMultiTask;
    bool m_isPad;
    bool mbOnSale; // 是否高清
    bool m_isWebGame;
    bool m_isDomView;
    bool m_isDomExtView;
	float m_scale;
    float m_screenWidth;
    float m_screenHeight;
    int m_deviceOrientation;
};


#endif
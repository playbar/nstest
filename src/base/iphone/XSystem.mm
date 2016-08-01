// XTime.cpp: implementation of the XTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XSystem.h"
#import "UIDeviceAdvance.h"
#import "IPhoneDeviceData.h"

enum
{
	XPROTO_HTTP,
	XPROTO_HTTPS,
	XPROTO_WEBB,
	XPROTO_WEBV,
	XPROTO_FTP,
	XPROTO_SOCKET,
	XPROTO_APP,
	XPROTO_EXT,
	XPROTO_WIN,
	XPROTO_FILE,
	XPROTO_RES,
	XPROTO_SYSTEM
};

//#ifdef __IPHONE__
//#include "BaiWanViewController_iPhone.h"
//extern BaiWanViewController_iPhone* g_explorerController_iPhone;
//#elif defined(__IPAD__)
#include "BaiWanViewController_ios.h"
extern BaiWanViewController_ios* g_explorerController_ios;
extern iOSDeviceRelateData g_deviceData;
//#endif


XString16 XSystem::m_strURL;
XString16 XSystem::m_strPostData;
XString16 XSystem::m_strHeaders;
XString8 XSystem::m_strURL8;
XString8 XSystem::m_strPostData8;
XString8 XSystem::m_strHeaders8;

XU32 XSystem::_nTotalFrames = 0;
XU32 XSystem::_nTotalFrameMemory = 0;
XU32 XSystem::_nTotalTextures = 0;
XU32 XSystem::_nTotalTextureMemory = 0;
XU32 XSystem::_nTotalImages = 0;
XU32 XSystem::_nTotalImageMemory = 0;
XU32 XSystem::_nTotalBuffers = 0;
XU32 XSystem::_nTotalBufferMemory = 0;

bool  XSystem::SetClipText(XString16&text)
{
    return false;
}
bool  XSystem::GetClipText(XString16&text)
{
	return false;
}
bool  XSystem::Execl(const char* strCmd,const char*args)
{
	return false;
}
bool  XSystem::GetClientDesc(XString8&text)
{
    NSMutableString* textString = [[[NSMutableString alloc] init] autorelease];
    NSString* deviceMode = [[UIDevice currentDevice] machine];
    if (!deviceMode)
        return XFALSE;
    else
        [textString appendString:[deviceMode copy]];
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    int screenWidth = screenBounds.size.width>screenBounds.size.height?screenBounds.size.width:screenBounds.size.height;
    int screenHeight = screenBounds.size.width>screenBounds.size.height?screenBounds.size.height:screenBounds.size.width;
    if ((!g_deviceData.m_isPad)&&(g_deviceData.m_scale > 1.1))
    {
        screenWidth *=2;
        screenHeight *=2;
    }
    [textString appendString:@"/"];
    [textString appendFormat:@"%dX%d",screenWidth,screenHeight];
    
    unsigned totalMemory = XSystem::GetTotalMemory();
    [textString appendString:@"/"];
    [textString appendFormat:@"%u", (totalMemory/1024)/1024];
	text.SetString([textString cStringUsingEncoding:NSUTF8StringEncoding], [textString lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
	return XTRUE;
}
bool  XSystem::LaunchHTML(const char*url)
{
	NSString* urlStr = [[NSString alloc] initWithCString:url encoding:NSUTF8StringEncoding];
	NSURL* purl = [NSURL URLWithString:urlStr];
	[urlStr release];
	if (![[UIApplication sharedApplication] canOpenURL:purl])
		return XFALSE;
	else
	{
//#ifdef __IPHONE__
//        [g_explorerController_iPhone setLaunchUrl:purl];
//        UIAlertView* alertView = [[UIAlertView alloc]
//								  initWithTitle:@"确认弹出Safari？" message:nil delegate:g_explorerController_iPhone cancelButtonTitle:@"取消" otherButtonTitles:@"确认", nil];
//        [alertView setTag:EALERTVIEWMODE_OPENSAFARI];
//		[alertView show];
//		[alertView release];
//#elif defined (__IPAD__)
        [g_explorerController_ios setLaunchUrl:purl];
		UIAlertView* alertView = [[UIAlertView alloc]
								  initWithTitle:@"确认弹出Safari？" message:nil delegate:g_explorerController_ios cancelButtonTitle:@"取消" otherButtonTitles:@"确认", nil];
        [alertView setTag:EALERTVIEWMODE_OPENSAFARI];
		[alertView show];
		[alertView release];
//#endif
        return XTRUE;
	}
}

bool  XSystem::LaunchApplication(const char*url)
{
	NSString* urlStr = [[NSString alloc] initWithCString:url encoding:NSUTF8StringEncoding];
	NSURL* purl = [NSURL URLWithString:urlStr];
	[urlStr release];
	if (![[UIApplication sharedApplication] canOpenURL:purl])
		return XFALSE;
	else
	{
//#ifdef __IPHONE__
//        [g_explorerController_iPhone setLaunchUrl:purl];
//        UIAlertView* alertView = [[UIAlertView alloc]
//								  initWithTitle:@"确认弹出Safari？" message:nil delegate:g_explorerController_iPhone cancelButtonTitle:@"取消" otherButtonTitles:@"确认", nil];
//        [alertView setTag:EALERTVIEWMODE_OPENSAFARI];
//		[alertView show];
//		[alertView release];
//#elif defined (__IPAD__)
        [g_explorerController_ios setLaunchUrl:purl];
		UIAlertView* alertView = [[UIAlertView alloc]
								  initWithTitle:@"确认弹出Safari？" message:nil delegate:g_explorerController_ios cancelButtonTitle:@"取消" otherButtonTitles:@"确认", nil];
        [alertView setTag:EALERTVIEWMODE_OPENSAFARI];
		[alertView show];
		[alertView release];
//#endif
        return XTRUE;
	}
}
bool XSystem::LaunchContext(const char*url,int nType)
{
    //LOGWHEREMSG( url );
    return XSystem::OpenWindow(url, nType);
}

bool XSystem::OpenWebView(const char*url,int nType)
{
//    if (nType == XPROTO_WEBB)
//        [g_explorerController_ios openWebViewWithUrl:[NSString stringWithUTF8String:url] withIsWebB:YES];
//    else
//        [g_explorerController_ios openWebViewWithUrl:[NSString stringWithUTF8String:url] withIsWebB:NO];
    
    return true;
}

bool XSystem::OpenWindow(const char *url, int nArg)
{

    // open ext://
    NSString* urlStr = [[NSString alloc] initWithCString:url encoding:NSUTF8StringEncoding];
    if ([urlStr hasPrefix:@"http://BW_DOMOB_OFFERWALL"] || [urlStr hasPrefix:@"http://BW_LiMei_OFFERWALL"] || [urlStr hasPrefix:@"http://BW_YouMi_OFFERWALL"] || [urlStr hasPrefix:@"http://BW_Mobisage_OFFERWALL"])
    {
        
        [g_explorerController_ios presentOfferWall: urlStr];
        [urlStr release];
        return true;
    }
#ifdef BWLMSDKMODE
    if ([urlStr hasPrefix:@"http://BW_LIMEI_OFFERWALL"])
    {
        
        [g_explorerController_ios presentLMOfferWall: urlStr];
        [urlStr release];
        return true;
    }
#endif
	NSURL* purl = [NSURL URLWithString:urlStr];
	[urlStr release];
    
//#ifdef __IPHONE__
//#elif defined (__IPAD__)
//#if BWUCSDKMODE
//    [g_explorerController_ios onUCUserCenter:urlStr];
//#endif
//#endif
    
	if (![[UIApplication sharedApplication] canOpenURL:purl])
		return XFALSE;
	else
	{
//#ifdef __IPHONE__
//        [g_explorerController_iPhone setLaunchUrl:purl];
//        UIAlertView* alertView = [[UIAlertView alloc]
//								  initWithTitle:@"确认弹出Safari？" message:nil delegate:g_explorerController_iPhone cancelButtonTitle:@"取消" otherButtonTitles:@"确认", nil];
//        [alertView setTag:EALERTVIEWMODE_OPENSAFARI];
//		[alertView show];
//		[alertView release];
//#elif defined (__IPAD__)
        [g_explorerController_ios setLaunchUrl:purl];
		UIAlertView* alertView = [[UIAlertView alloc]
								  initWithTitle:@"确认弹出Safari？" message:nil delegate:g_explorerController_ios cancelButtonTitle:@"取消" otherButtonTitles:@"确认", nil];
        [alertView setTag:EALERTVIEWMODE_OPENSAFARI];
		[alertView show];
		[alertView release];
//#endif
        return XTRUE;
	}
}

XU32  XSystem::GetTotalMemory()
{
	return (XU32)[[UIDevice currentDevice] totalMemory];
}
XU32  XSystem::GetFreeMemory()
{
	return (XU32)[[UIDevice currentDevice] freeMemory];
}
XU32  XSystem::GetUsedMemory()
{
	return (XU32)[[UIDevice currentDevice] usedMemory];
}


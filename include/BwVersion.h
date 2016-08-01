////////////////////////////////////////////
// BwVersion.h
// 请在本文件中记录所有用到的版本号
////////////////////////////////////////////

#ifndef __BW_VERSION__ 
#define __BW_VERSION__

// 使用示例
#if (__CORE_VERSION__ >= 0x02070100)
#else
#endif

// 版本检查
#pragma message("----------------------------------------------------")

#define _TO_STRING(x) #x
#define VERSION_TO_STRING(x) _TO_STRING(x)

// 内核检查
#ifndef __CORE_VERSION__
#error __CORE_VERSION__ is not defined !
#else	// __CORE_VERSION__
#pragma message( "内核版本\t: " VERSION_TO_STRING(__CORE_VERSION__) )
#endif	// __CORE_VERSION__

// 客户端检查
#ifdef __ANDROID__
#ifndef __ANDROID_VERSION__
#error __ANDROID_VERSION__ is not defined !
#else	// __CLIENT_VERSION__
#pragma message( "安卓版本\t: " VERSION_TO_STRING(__ANDROID_VERSION__) )
#endif	// __CLIENT_VERSION__
#endif

#ifdef __APPLE__
#ifndef __IOS_VERSION__
#error __IOS_VERSION__ is not defined !
#else	// __CLIENT_VERSION__
#pragma message( "苹果版本\t: " VERSION_TO_STRING(__IOS_VERSION__) )
#endif	// __CLIENT_VERSION__
#endif

#pragma message("----------------------------------------------------")

#endif	// __BW_VERSION__

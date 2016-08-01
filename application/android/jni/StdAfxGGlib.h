
#ifndef __STDAFXGGLIB_H__
#define __STDAFXGGLIB_H__

#define FPM_DEFAULT

//#include <jni.h>
#include <android/log.h>
//#include <stdlib.h>

#ifdef WIN32
#define STDCALL  _stdcall
#elif defined( __ANDROID__ )
#define STDCALL __attribute__ ((visibility("default")))
#else
#define STDCALL
#endif

#define __CORE_VERSION__ 0x02080000
#define __ANDROID_TEST__

#ifdef __ANDROID__
#define __FILENAME__ (strrchr(__FILE__,'/')+1)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, __FILENAME__,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,__FILENAME__,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,__FILENAME__,__VA_ARGS__)
#define LOGWHERE()         LOGE("L: %4d \t F: %s  ",      __LINE__, __FUNCTION__ )
#define LOGWHEREMSG( msg ) LOGE("L: %4d \t F: %s  M: %s", __LINE__, __FUNCTION__, msg )
#define LOGWHEREVAL( val ) LOGE("L: %4d \t F: %s  V: %d", __LINE__, __FUNCTION__, val )
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
//#define LOGWHERE() LOGE("%s, %s, %d", __FILE__, __FUNCTION__, __LINE__ )
//#define LOGWHEREMSG( msg ) LOGE("%s, %s, %d, %s", __FILE__, __FUNCTION__, __LINE__, msg )
//#define LOGWHEREVAL( val ) LOGE("%s, %s, %d, %d", __FILE__, __FUNCTION__, __LINE__, val )
#else
#define LOGI(...)
#define LOGD(...)
#define LOGE(...)
#define LOGWHERE()
#define LOGWHEREMSG( msg )
#define LOGWHEREVAL( val )
#endif

#define _ARM_ 1

#endif

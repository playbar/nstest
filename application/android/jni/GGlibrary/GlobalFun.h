
#ifndef __GLOBALFUN_H__
#define __GLOBALFUN_H__

#include <wchar.h>
#include "GGType.h"
#include "jni.h"

#define SK_BUILD_FOR_ANDROID_NDK 1

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

JNIEnv* GetJNIEnv();

//int32_t sk_atomic_inc(int32_t* addr);

//int32_t sk_atomic_dec(int32_t* addr);

//int GetScreenDepth();
//
//int GetScreenWidth();
//
//int GetScreenHeigth();
//
//bool GetScreenWH(int &w, int &h );

/////////////////////////////////////////////

void GFunCallTextEdit( jstring instr, int ilength, int style );

void GFunChangeUserStatus( int userSta );

void GFunOpenViewUrl(jstring strurl );

void GFunOpenWebViewUrl(jstring strurl, int proto );
// zhengjl 2012.03
void GFunPaySDK(jstring strurl);
void GFunOutLogin(int b);

void GFunUpgradeApp( jstring strUrl, int status );

char * GFunGetClientDesc( );

char* GFunGetCpu();

char* GFunGetRam();

char* GFunGetDpi();

int GFunGetClientVersion( );

int GFunGetBrowerHeight();

void GFunSetFont( int nSize );

void GFunDrawString( char* str );

JNIEnv* getThreadJNIEnv();

jstring c2js( const char* pat);
char* js2c( jstring jstr);
jstring w2js( wchar_t* str);
jstring xw2js( XWCHAR* str);
wchar_t* js2w( jstring str);
XWCHAR* js2xw( jstring str);
jbyteArray c2jbarray( const char* pat,int len);
jstring c2js_len( const char* pat,int len);
bool TwoLineIsIntersect(float x0, float y0, float x1, float y1,
		float x2, float y2, float x3, float y3, float &InterX, float &InterY);

#ifdef __cplusplus
}
#endif //__cplusplus

extern int gWidth;
extern int gHeight;
extern int giVersion; //???信�?
extern int giBrowerHeight;  //�???��?�?

extern unsigned char * gTxtBmpData;
extern int gTxtWidth;
extern int gTxtHeight;

#endif

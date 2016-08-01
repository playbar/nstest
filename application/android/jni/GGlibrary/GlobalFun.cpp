

#include "android_runtime/AndroidRuntime.h"
using namespace android;

#include "string_comp.h"
#include "GlobalFun.h"
#include "XBWSendMessageContent.h"
extern BWSendMessageContentClass *g_WeiXinShareSessionSavedData;
int gWidth = 0;
int gHeight = 0;

int giVersion = 0; //???信�?
int giBrowerHeight = 0;  //�???��?�?

unsigned char * gTxtBmpData = NULL;
int gTxtWidth = 0;
int gTxtHeight = 0;

JNIEnv* GetJNIEnv()
{
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	return env;
}

//void GetWindow(ANativeWindow *window,EGLDisplay display){
//
//}
//
//void SetWindow(ANativeWindow *window,EGLDisplay display){
//
//}

////SkMutex gAtomicMutex;
//int32_t sk_atomic_inc(int32_t* addr)
//{
//    int32_t value = *addr;
//    *addr = value + 1;
//    return value;
//}
//
//int32_t sk_atomic_dec(int32_t* addr)
//{
//     int32_t value = *addr;
//    *addr = value - 1;
//    return value;
//}
//
//int GetScreenDepth()
//{
//    android::DisplayInfo info;
//    android::SurfaceComposerClient::getDisplayInfo(android::DisplayID(0), &info);
//    return info.pixelFormatInfo.bitsPerPixel;
//	return 0;
//}
//
//int GetcreenDepthPerComponent()
//{
//    android::DisplayInfo info;
//    android::SurfaceComposerClient::getDisplayInfo(android::DisplayID(0), &info);
//    return info.pixelFormatInfo.bitsPerPixel;
//	return 0;
//}
//
//#ifdef ANDROID_ORIENTATION_SUPPORT
//int Screen::orientation() const
//{
//    android::DisplayInfo info;
//    android::SurfaceComposerClient::getDisplayInfo(
//            android::DisplayID(0), &info);
//    // getDisplayInfo returns an enum describing the orientation. Map the enum
//    // to the values described here
//    // (http://developer.apple.com/documentation/AppleApplications/Reference/SafariWebContent/HandlingEvents/chapter_8_section_6.html)
//    switch (info.orientation) {
//        case android::ISurfaceComposer::eOrientationDefault:
//            return 0;
//        case android::ISurfaceComposer::eOrientation90:
//            return 90;
//        case android::ISurfaceComposer::eOrientation180:
//            return 180;
//        case android::ISurfaceComposer::eOrientation270:
//            return -90;
//        default:
//            LOGE("Bad orientation returned from getDisplayIndo %d",
//                    info.orientation);
//            return 0;
//    }
//}
//#endif
//
//
//
//int GetScreenWidth()
//{
//    android::DisplayInfo info;
//    android::SurfaceComposerClient::getDisplayInfo(android::DisplayID(0), &info);
//    return  info.w;
//	return 0;
//}
//
//int GetScreenHeigth()
//{
//    android::DisplayInfo info;
//    android::SurfaceComposerClient::getDisplayInfo(android::DisplayID(0), &info);
//    return  info.h;
//	return 0;
//}
//
//bool GetScreenWH(int &w, int &h )
//{
//	 android::DisplayInfo info;
//	 android::SurfaceComposerClient::getDisplayInfo(android::DisplayID(0), &info);
//	 w = info.w;
//	 h = info.h;
//	 return true;
//}

char * GFunGetClientDesc( )
{
	//XMobileMain* mobileMain = GetMobileMain(env, g_mmobject );
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	jclass build_class = env->FindClass( "android/os/Build");
	jfieldID model_id = env->GetStaticFieldID( build_class, "MODEL", "Ljava/lang/String;");
	jstring model_obj = (jstring)env->GetStaticObjectField( build_class, model_id);
	char *pDes = js2c( model_obj );
	return pDes;
}

int GFunGetClientVersion( )
{
	return giVersion;
}

int GFunGetBrowerHeight()
{
	return giBrowerHeight;
}


//int GFunGetClientVersion( )
//{
//	static int iver = 0;
//	if( iver == 0 )
//	{
//		JNIEnv* env = AndroidRuntime::getJNIEnv();
//		jclass build_class = env->FindClass( "android/os/Build/VERSION");
//		jfieldID model_id = env->GetStaticFieldID( build_class, "RELEASE", "Ljava/lang/String;");
//		jstring model_obj = (jstring)env->GetStaticObjectField( build_class, model_id);
//		char *pDes = js2c( model_obj );
//		iver = atoi( pDes );
//	}
//	LOGWHEREVAL( iver );
//	return iver;
//}

/*******************************************************************
 *char*----->jstring
 *******************************************************************/
jstring c2js( const char* pat)
{
	if( pat == NULL )
		return NULL;
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
}
jstring c2js_len( const char* pat,int len)
{
	if( pat == NULL )
		return NULL;
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(len);
	env->SetByteArrayRegion(bytes, 0, len, (jbyte*) pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
}
jbyteArray c2jbarray( const char* pat,int len)
{
	if( pat == NULL )
			return NULL;
		JNIEnv* env = AndroidRuntime::getJNIEnv();
		if( env == NULL )
			return NULL;
		jbyteArray bytes = env->NewByteArray(len);
		env->SetByteArrayRegion(bytes, 0, len, (jbyte*) pat);
		return bytes;
}
/******************************************************************
 *jstring ----->char*
 ********************************************************************/
char* js2c( jstring jstr)
{
	if( jstr == NULL )
		return NULL;
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}
/*****************************************************************
 *wchar_t* ----->jstring
 *******************************************************************/
jstring w2js( wchar_t* str)
{
	if( str == NULL )
		return NULL;
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	size_t len = wcslen(str);
	jchar* str2 = (jchar*) malloc(sizeof(jchar) * (len + 1));
	int i;
	for (i = 0; i < len; i++)
		str2[i] = str[i];
	str2[len] = 0;
	jstring js = env->NewString(str2, len);
	free(str2);
	return js;
}

/*****************************************************************
 *XWCHAR* ----->jstring
 *******************************************************************/
jstring xw2js( XWCHAR* str)
{
	if( str == NULL )
		return NULL;
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	size_t len = gg_wcslen(str);
	jchar* str2 = (jchar*) malloc(sizeof(jchar) * (len + 1));
	int i = 0;
	for (i = 0; i < len; i++)
		str2[i] = str[i];
	str2[len] = 0;
	jstring js = env->NewString(str2, len);
	free(str2);
	return js;
}

/******************************************************************
 *jstring----->wchar_t*
 ********************************************************************/
wchar_t* js2w( jstring str)
{
	if( str == NULL )
		return NULL;
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	int len = env->GetStringLength(str);
	wchar_t *w_buffer = new wchar_t[len];
	memset(w_buffer, 0, len + 1);
	w_buffer[len] = '\0';
	wcsncpy(w_buffer, (wchar_t *) env->GetStringChars(str, 0), len);
	env->ReleaseStringChars(str, (const unsigned short *) w_buffer);
	return w_buffer;
}

/******************************************************************
 *jstring----->XWCHAR*
 ********************************************************************/
XWCHAR* js2xw( jstring str)
{
	if( str == NULL )
		return NULL;
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	if( env == NULL )
		return NULL;
	int len = env->GetStringLength(str);

	XWCHAR *w_buffer = new XWCHAR[len + 1];
	memset(w_buffer, 0, (len+1) * 2 );

	const jchar* tmp = env->GetStringChars(str, false);
	memcpy(w_buffer, tmp, len * 2 );

	env->ReleaseStringChars(str, tmp);

	return w_buffer;
}

bool TwoLineIsIntersect(float x0, float y0, float x1, float y1,
		float x2, float y2, float x3, float y3, float &InterX, float &InterY)
{
	//两条线段是否相交X0X1 AND X1X2
   float x, y;
   float Minx01 = XMIN(x0, x1);
   float Miny01 = XMIN(y0, y1);
   float Minx23 = XMIN(x2, x3);
   float Miny23 = XMIN(y2, y3);
   float Maxx01 = XMAX(x0, x1);
   float Maxy01 = XMAX(y0, y1);
   float Maxx23 = XMAX(x2, x3);
   float Maxy23 = XMAX(y2, y3);

   if(x1!=x0 && x2!=x3)
   {
       float k1 = (y1-y0)/(x1-x0);
       float k2 = (y3-y2)/(x3-x2);
       float Den = (y1-y0)*(x3-x2) - (y3-y2)*(x1-x0);
       if(k1==k2)
       {
    	   //平行不相交
          float d1 = abs(y0*(x1-x0)-x0*(y1-y0)-y2*(x3-x2)+x2*(y3-y2)); //距离公式d = abs(c1-c2) / sqrt(a*a+b*b)
          if(d1==0)
          {
        	  //直线重合
             if((x2>Minx01 && x2<Maxy01 && y2>Miny01 && y2<Maxy01) || (x3>Minx01 && x3<Maxy01 && y3>Miny01 && y3<Maxy01)
             || (x0>Minx23 && x0<Maxy23 && y0>Miny23 && y0<Maxy23) || (x1>Minx23 && x1<Maxy23 && y1>Miny23 && y1<Maxy23))
             {
            	 //实际碰撞问题线段重合认为相交了
                return true;
             }
             else
             {
                return false;
             }
          }
          else
          {
             return false;
          }
       }
       x = ((y2-y0)*(x1-x0)*(x3-x2)+(y1-y0)*(x3-x2)*x0-(y3-y2)*(x1-x0)*x2)/Den;
       y = ((y1-y0)*(x-x0))/(x1-x0) + y0;

       if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 &&
    		   Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
       {
          InterX = x;
          InterY = y;
          return true;
       }
   }
   else if(x1==x0 && x2!=x3)
   {
       x = x0;
       y = ((y3-y2)*(x0-x2))/(x3-x2) + y2;
       if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 &&
    		   Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
       {
          InterX = x;
          InterY = y;
          return true;
       }
   }
   else if(x1!=x0 && x2==x3)
   {
       x = x2;
       y = ((y1-y0)*(x2-x0))/(x1-x0) + y0;
       if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 &&
    		   Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
       {
          InterX = x;
          InterY = y;
          return true;
       }
   }
   return false;
}

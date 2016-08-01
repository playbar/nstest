#include "jni.h"
#include "nativehelper/JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
using namespace android;

#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h>
#include <EGL/egl.h>

#include "Define.h"
#include "GlobalFun.h"
#include "GlesUtil.h"
#include "XFontDraw.h"
#include "com_jqbar_MobileView.h"
#include "com_jqbar_MobileMain.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "android/bitmap.h"

static JavaVM* gvm = NULL;
static int gneedDetach = 0;

static stmmField mmfields;
static jobject gmmObject = NULL;
//ANativeWindow *mainwindow = 0;
//EGLDisplay maindisplay;
//////////////////////////////////////////////////////////////////////////////////

JNIEnv* getThreadJNIEnv()
{
	JNIEnv *env = NULL;
	jint result = -1;
	 if (gvm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
	 {
		 int status = gvm->AttachCurrentThread( &env, 0 );
		 if( status < 0 )
		 {
			 LOGWHERE();
			 return NULL;
		 }
		 gneedDetach = 1;
	 }
	 return env;
}


char* GFunGetCpu()
{
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	jstring  p = (jstring)env->GetObjectField( gmmObject, mmfields.XMobileMainCpuID );
	//LOGWHEREVAL(js2c(p));
	char * pch = js2c( p );
	LOGWHEREVAL( pch );
	return  pch;
}

char* GFunGetRam()
{
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	jstring  p = (jstring)env->GetObjectField( gmmObject, mmfields.XMobileMainRamID );
	char * pch = js2c( p );
	LOGWHEREVAL( pch );
	return  pch;
}
char* GFunGetDpi(){
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	jstring  p = (jstring)env->GetObjectField( gmmObject, mmfields.XMobileMainDpiID );
	char * pch = js2c( p );
	LOGWHEREVAL( pch );
	return  pch;
}
void GFunOpenViewUrl(jstring strurl )
{
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	env->CallVoidMethod(gmmObject, mmfields.OpenViewUrlID, strurl );
	return;
}

void GFunOpenWebViewUrl(jstring strurl, int proto )
{
	LOGE("%s, %d-------->,%s", __FUNCTION__,__LINE__,strurl);
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	env->CallVoidMethod(gmmObject, mmfields.OpenWebViewUrlID, strurl, proto );
	return;
}
void GFunUpgradeApp( jstring strUrl, int status )
{
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	env->CallVoidMethod(gmmObject, mmfields.UpgradeAppID, strUrl, status );
	return;
}

// zhengjl 2012.03
void GFunPaySDK(jstring strurl )
{
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	env->CallVoidMethod(gmmObject, mmfields.PaySDKID, strurl );
	return;
}
void GFunOutLogin(int b)
{
	JNIEnv* env = AndroidRuntime::getJNIEnv();
	env->CallVoidMethod(gmmObject, mmfields.OutLoginID, b );
	//XMobileMain* mobileMain = GetMobileMain(env, g_mmobject );
	//mobileMain->OutLogin(b);
	return;
}

void GFunSetFont( int nSize )
{
	JNIEnv* env = getThreadJNIEnv();
	env->CallVoidMethod( gmmObject, mmfields.JavaSetFontID, nSize );
	if( gneedDetach )
	{
		gvm->DetachCurrentThread();
		gneedDetach = 0;
	}
	return;
}

void GFunDrawString( char* str )
{
	LOGWHEREMSG( str );
	JNIEnv * env = getThreadJNIEnv();

	if( str == NULL )
		return;
	if( env == NULL )
		return;
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(str));
	env->SetByteArrayRegion(bytes, 0, strlen(str), (jbyte*) str );
	jstring encoding = env->NewStringUTF("utf-8");
	jstring jstr = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

	env->CallVoidMethod( gmmObject, mmfields.JavaDrawStringID, jstr );
	if( gneedDetach )
	{
		gvm->DetachCurrentThread();
		gneedDetach = 0;
	}
	return;
}

void MobileMain_SetTxtBmpData( JNIEnv *env, jobject thiz, jobject bitmap, int width, int height )
{
	AndroidBitmapInfo infocolor;
	int ret = 0;
	void *pixelscolor;
	if ((ret = AndroidBitmap_getInfo(env, bitmap, &infocolor)) < 0)
	{
		LOGWHEREMSG( "error bmp getinfo" );
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixelscolor)) < 0)
	{
		LOGWHEREMSG("error lockpixels" );
	}
	//LOGE("AndoridBitmap_getInfo::infocolor-width:%d, infocolor-height:%d, infocolor-format:%d", infocolor.width, infocolor.height,infocolor.format);
	int size = width * height;

	gTxtWidth = width;
	gTxtHeight = height;
	if( gTxtBmpData != NULL )
	{
		delete []gTxtBmpData;
		gTxtBmpData = NULL;
	}
	gTxtBmpData = new unsigned char[ size ];
	//env->GetByteArrayRegion( )

	memcpy( gTxtBmpData, pixelscolor, size );


	//XMobileView* mobileView = getMobileView(env, thiz);
	//mobileView->SetBmpData((unsigned char *) pixelscolor);

	AndroidBitmap_unlockPixels(env, bitmap);
	return;
}

static void MobileMain_SetTxtBitData( JNIEnv *env, jobject thiz, jbyteArray pixels, int width, int height )
{
	int size = width * height;

	gTxtWidth = width;
	gTxtHeight = height;
	if( gTxtBmpData != NULL )
	{
		delete []gTxtBmpData;
		gTxtBmpData = NULL;
	}
	gTxtBmpData = new unsigned char[ size ];

	env->GetByteArrayRegion( pixels, 0, size, (jbyte*)gTxtBmpData );

	return;

}

static void MobileMain_NativeInit(JNIEnv * env, jobject thiz,
		int width, int height, int iversion, int browerHeight )
{
	//LOGWHERE();

	giVersion = iversion;
	giBrowerHeight = browerHeight;
//	LOGWHEREVAL(gCpu);
//	LOGWHEREVAL(gRam);
//	gCpu=js2c( cpu );
//	LOGWHEREVAL(gCpu);
//	gRam=js2c( ram );
//	LOGWHEREVAL(gRam);
	gmmObject = env->NewGlobalRef( thiz );

	/////////////////////////////////////////////////////////////////////////
	mmfields.XMoblieMainClass = env->FindClass( MOBILEMAIN );
	if( mmfields.XMoblieMainClass == NULL )
	{
		LOGWHERE();
		return;
	}

	mmfields.XMobileMainID = env->GetFieldID(mmfields.XMoblieMainClass, "mMobileMainID", "I");
	if (mmfields.XMobileMainID == NULL)
	{
		LOGWHERE();
		return;
	}
	mmfields.XMobileMainCpuID = env->GetFieldID(mmfields.XMoblieMainClass, "mCpu", "Ljava/lang/String;");
	if (mmfields.XMobileMainCpuID == NULL)
	{
		LOGWHERE();
		return;
	}
	mmfields.XMobileMainRamID = env->GetFieldID(mmfields.XMoblieMainClass, "mRam", "Ljava/lang/String;");
	if (mmfields.XMobileMainRamID == NULL)
	{
		LOGWHERE();
		return;
	}
	mmfields.XMobileMainDpiID = env->GetFieldID(mmfields.XMoblieMainClass, "mDpi", "Ljava/lang/String;");
	if (mmfields.XMobileMainDpiID == NULL)
	{
		LOGWHERE();
		return;
	}


	mmfields.OpenViewUrlID = env->GetMethodID( mmfields.XMoblieMainClass,
				"openViewUrl", "(Ljava/lang/String;)V");
	if( mmfields.OpenViewUrlID == NULL )
	{
		LOGWHERE();
		return;
	}

	mmfields.OpenWebViewUrlID = env->GetMethodID( mmfields.XMoblieMainClass,
				"openWebViewUrl", "(Ljava/lang/String;I)V");
	if( mmfields.OpenWebViewUrlID == NULL )
	{
		LOGWHERE();
		return;
	}
	mmfields.UpgradeAppID = env->GetMethodID( mmfields.XMoblieMainClass,
				"upgradeApp", "(Ljava/lang/String;I)V");
	if( mmfields.UpgradeAppID == NULL )
	{
		LOGWHERE();
		return;
	}

	mmfields.PaySDKID = env->GetMethodID( mmfields.XMoblieMainClass,
				"paySDK", "(Ljava/lang/String;)V");
	if( mmfields.PaySDKID == NULL )
	{
		LOGWHERE();
		return;
	}
	// zhengjl 2012.03	
	mmfields.OutLoginID = env->GetMethodID( mmfields.XMoblieMainClass,
				"setOutLogin", "(I)V");
	if( mmfields.OutLoginID == NULL )
	{
		LOGWHERE();
		return;
	}

	mmfields.JavaSetFontID = env->GetMethodID( mmfields.XMoblieMainClass, "setFont", "(I)V");
	if( mmfields.JavaSetFontID == NULL )
	{
		LOGWHERE();
		return;
	}

	mmfields.JavaDrawStringID = env->GetMethodID( mmfields.XMoblieMainClass, "drawString",  "(Ljava/lang/String;)V" );
	if( mmfields.JavaDrawStringID == NULL )
	{
		LOGWHERE();
		return;
	}

	//LOGWHERE();
	return;
}

static void MobileMain_NativeFinalize(JNIEnv * env, jobject thiz )
{
	if( gmmObject != NULL )
	{
		env->DeleteGlobalRef(gmmObject);
	}
	return;
}

//"fonts.fnt"
static void CopyEachFontFile( AAssetManager* mgr, char *pFileName )
{
//	 char szFontName[256] = {0};
//	 sprintf( szFontName, "%s/%s", WORKPATH, pFileName );
//	 if( access(szFontName, F_OK ) == -1  )
//	 {
//		 AAsset* asset = AAssetManager_open(mgr, pFileName, AASSET_MODE_UNKNOWN);
//		 if( asset == NULL )
//		 {
//			 LOGWHERE();
//			 return;
//		 }
//		 int ilen = AAsset_getLength( asset );
//		 //LOGWHEREVAL( ilen );
//		 char *pBuffer = new char[ilen];
//		 AAsset_read( asset, pBuffer, ilen );
//		 AAsset_close( asset );
//		 FILE *pf = fopen( szFontName, "w+" );
//		 if( pf != NULL )
//		 {
//			 fwrite( pBuffer, ilen, 1, pf );
//			 fclose( pf );
//		 }
//		 delete []pBuffer;
//		 pBuffer = NULL;
//		 //LOGWHEREMSG( szFontName );
//	 }
}


//static void MobileMain_NativeSetSurface(JNIEnv* jenv, jobject obj, jobject surface,jint width,jint height){
//	if (surface != 0) {
//		gWidth = width;
//		gHeight = height;
//		mainwindow = ANativeWindow_fromSurface(jenv, surface);
//	        LOGE("Got mainwindow %p", mainwindow);
//	        maindisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
//	        LOGE("Got maindisplay %p", maindisplay);
//	        SetWindow(mainwindow,maindisplay);
//	    } else {
//	        LOGE("Releasing mainwindow");
//	        ANativeWindow_release(mainwindow);
//	    }
//
//}

static void MobileMain_CopyFontFile(JNIEnv * env, jobject thiz, jobject assertMg )
{
//	 AAssetManager* mgr = AAssetManager_fromJava(env, assertMg);
//	 CopyEachFontFile( mgr, "fonts.fnt" );
//	 CopyEachFontFile( mgr, "hz8.dot" );
//	 CopyEachFontFile( mgr, "hz12.dot" );
//	 CopyEachFontFile( mgr, "hz16.dot" );
	 return;
}

static JNINativeMethod gMobileMainMethods[] =
{
    {"nativeInit",   	"(IIII)V",              				(void*)MobileMain_NativeInit},
    {"nativeFinalize",	"()V",									(void*)MobileMain_NativeFinalize },
    {"copyFontFile",	"(Landroid/content/res/AssetManager;)V",(void*)MobileMain_CopyFontFile },
    {"setTxtBmpData",	"(Landroid/graphics/Bitmap;II)V",		(void*)MobileMain_SetTxtBmpData },
    {"setTxtBitData", 	"([BII)V",								(void*)MobileMain_SetTxtBitData },
};

int register_MobileMain( JNIEnv *env )
{
	 return AndroidRuntime::registerNativeMethods(env,
			 MOBILEMAIN, gMobileMainMethods, NELEM(gMobileMainMethods));
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	gvm = vm;
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed\n");
        goto bail;
    }

    assert(env != NULL);
    if( register_MobileMain( env ) < 0 )
    {
    	LOGE("ERROR: MobileMain native registration failed\n");
    	goto bail;
    }

    if (register_MobileView(env) < 0)
    {
        LOGE("ERROR: MobileView native registration failed\n");
        goto bail;
    }

//    if (register_MobileViewGLES(env) < 0)
//    {
//        LOGE("ERROR: MobileViewGLES native registration failed\n");
//        goto bail;
//    }

//    if (register_MobileViewSurface(env) < 0)
//    {
//        LOGE("ERROR: MobileViewSurface native registration failed\n");
//        goto bail;
//    }

    result = JNI_VERSION_1_4;

bail:
    return result;
}




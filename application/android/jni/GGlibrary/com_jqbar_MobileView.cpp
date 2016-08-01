#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "nativehelper/JNIHelp.h"
#include "jni.h"
#include "pthread.h"
#include "android_runtime/AndroidRuntime.h"
#include<ctime>
using namespace std;
using namespace android;

#include "GlobalFun.h"
#include "Define.h"
#include "MobileView.h"
#include "XMainWnd.h"
#include "GlesUtil.h"
#include "XAcc.h"
#include "com_jqbar_MobileView.h"
//#include "GraphicsJNI.h"

#include "android/bitmap.h"

#if 0
#define LOGWHEREMV()  LOGWHERE()
#else
#define LOGWHEREMV()
#endif

stMVfields gMVField;

static XMobileView* getMobileView(JNIEnv * env, jobject thiz) {
	LOGWHEREMV();
	//Mutex::Autolock l(sLock );
	XMobileView * p = (XMobileView *) env->GetIntField(thiz,gMVField.mobileViewID);
	return p;
}

static XMobileView* setMobileView(JNIEnv * env, jobject thiz, XMobileView* view)
{
	LOGWHEREMV();
	//Mutex::Autolock l(sLock );
	XMobileView* old = (XMobileView *) env->GetIntField(thiz, gMVField.mobileViewID);
	env->SetIntField(thiz, gMVField.mobileViewID, (int) view);
	return old;

}

static void MobileView_cppSurfaceCreate( JNIEnv *env, jobject thiz, jobject surface )
{
	gANativeWindow = ANativeWindow_fromSurface(env, surface);
	GlesInitDisplay();
	MakeCurrent( true );
	CreateGlesEnv();
	MakeCurrent( false );
	return;

}

static void MobileView_cppSurfaceChange(JNIEnv *env, jobject thiz, jint width, jint height)
{
	gWidth = width;
	gHeight = height;
	MakeCurrent( true );
	GlesEnvChanged(width, height);
	MakeCurrent( false );
	//解决退出游戏后，转屏，游戏残缺问题，刷新
	if(bwMode==0){
	XMobileView* mobileView = getMobileView(env, thiz);
	mobileView->Refresh();
	}
	return;
}

static void MobileView_TermRenderGLES(JNIEnv *env, jobject thiz) {
	GlesTermDisplay();
}

static void MobileView_NativesetWinWH(JNIEnv *env, jobject thiz,int screenWidth,int screenHeight,int mode) {
	if(mode==-1){
	RasterGL::sharedRasterGL()->SetWinSize( screenWidth, screenHeight);
	}
	gWidth = screenWidth;
	gHeight = screenHeight;
	bwMode=mode;
}

static void MobileView_NativeInit(JNIEnv * env, jobject thiz) {
	LOGWHEREMV();
	jclass mvclass = env->FindClass( MOBILEVIEW);
	if (mvclass == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.mobileViewClass = mvclass;
	gMVField.mobileViewID = env->GetFieldID(gMVField.mobileViewClass,
			"mMobileViewID", "I");
	if (gMVField.mobileViewID == NULL) {
		LOGWHERE();
		return;
	}

//	gMVField.mSurfaceID = env->GetFieldID(gMVField.mobileViewClass, "mSurface", "Landroid/view/Surface;");
//	if (gMVField.mSurfaceID == NULL)
//	{
//		LOGWHERE();
//		return;
//	}
	gMVField.changeScreenLID = env->GetMethodID(gMVField.mobileViewClass,
			"changeScreenLandscape", "(I)V");
	if (gMVField.changeScreenLID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.changeScreenPID = env->GetMethodID(gMVField.mobileViewClass,
			"changeScreenPortrait", "(I)V");
	if (gMVField.changeScreenPID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.changeScreenDir = env->GetMethodID(gMVField.mobileViewClass,
			"changeScreenDir", "(I)V");
	if (gMVField.changeScreenDir == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.refresh = env->GetMethodID(gMVField.mobileViewClass,
			"refresh", "()V");
	if (gMVField.refresh == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.postInvalidateID = env->GetMethodID(gMVField.mobileViewClass,
			"postInvalidate", "()V");
	if (gMVField.postInvalidateID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.postInvalidateRectID = env->GetMethodID(gMVField.mobileViewClass,
			"postInvalidate", "(IIII)V");
	if (gMVField.postInvalidateID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.FlashSaveFinishID = env->GetMethodID(gMVField.mobileViewClass,
			"flashSaveFinish", "()V");
	if (gMVField.FlashSaveFinishID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.StartFlashViewID = env->GetMethodID(gMVField.mobileViewClass,
			"startFlashView", "(I)V");
	if (gMVField.StartFlashViewID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.CloseFlashViewID = env->GetMethodID(gMVField.mobileViewClass,
			"closeFlashView", "()V");
	if (gMVField.CloseFlashViewID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.MakeErrorPageID = env->GetMethodID(gMVField.mobileViewClass,
			"MakeErrorPage", "()V");
	if (gMVField.MakeErrorPageID == NULL) {
		LOGWHERE();
		return;
	}
	///////////////////////
	gMVField.LoadPageStartID = env->GetMethodID(gMVField.mobileViewClass,
			"loadPageStart", "()V");
	if (gMVField.LoadPageStartID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.LoadPagePercentID = env->GetMethodID(gMVField.mobileViewClass,
			"loadPagePercent", "(I)V");
	if (gMVField.LoadPagePercentID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.LoadPageEndID = env->GetMethodID(gMVField.mobileViewClass,
			"loadPageEnd", "()V");
	if (gMVField.LoadPageEndID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.ShowBarsaleID = env->GetMethodID(gMVField.mobileViewClass,
			"showBarandsale", "(I)V");
	if (gMVField.ShowBarsaleID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.FlashLoadingEndID = env->GetMethodID(gMVField.mobileViewClass,
			"flashLoadingEnd", "()V");
	if (gMVField.FlashLoadingEndID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.CloseSubViewID = env->GetMethodID(gMVField.mobileViewClass,
			"closeSubView", "()V");
	if (gMVField.CloseSubViewID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.SetModeID = env->GetMethodID(gMVField.mobileViewClass, "SetMode",
			"(I)V");
	if (gMVField.SetModeID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.WeiXinInStalledID = env->GetMethodID(gMVField.mobileViewClass,
			"WeiXinInStalled", "()Z");
	if (gMVField.WeiXinInStalledID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendTextID = env->GetMethodID(gMVField.mobileViewClass,
			"WXSendText", "(Ljava/lang/String;I)V");
	if (gMVField.WXSendTextID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendPhotoID = env->GetMethodID(gMVField.mobileViewClass,
			"WXSendPhoto", "([BII)V");
	if (gMVField.WXSendPhotoID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendLinkID = env->GetMethodID(gMVField.mobileViewClass,
			"WXSendLink",
			"(Ljava/lang/String;Ljava/lang/String;[BLjava/lang/String;I)V");
	if (gMVField.WXSendLinkID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendMusicID =
			env->GetMethodID(gMVField.mobileViewClass, "WXSendMusic",
					"(Ljava/lang/String;Ljava/lang/String;[BLjava/lang/String;Ljava/lang/String;I)V");
	if (gMVField.WXSendMusicID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendVideoID = env->GetMethodID(gMVField.mobileViewClass,
			"WXSendVideo",
			"(Ljava/lang/String;Ljava/lang/String;[BLjava/lang/String;I)V");
	if (gMVField.WXSendVideoID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendAppMessgID =
			env->GetMethodID(gMVField.mobileViewClass, "WXSendAppMessage",
					"(Ljava/lang/String;Ljava/lang/String;[BLjava/lang/String;Ljava/lang/String;[BI)V");
	if (gMVField.WXSendAppMessgID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendNoGifID = env->GetMethodID(gMVField.mobileViewClass,
			"WXSendNoGif", "([B[BI)V");
	if (gMVField.WXSendNoGifID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendGifID = env->GetMethodID(gMVField.mobileViewClass,
			"WXSendGif", "([B[BI)V");
	if (gMVField.WXSendGifID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.WXSendFileID = env->GetMethodID(gMVField.mobileViewClass,
			"WXSendFile",
			"(Ljava/lang/String;Ljava/lang/String;[B[BLjava/lang/String;I)V");
	if (gMVField.WXSendFileID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.BWPayUrlID = env->GetMethodID(gMVField.mobileViewClass,
			"LoadPayUrl", "(Ljava/lang/String;)V");
	if (gMVField.BWPayUrlID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.ShowOfferWallID = env->GetMethodID(gMVField.mobileViewClass,
			"showDomobOfferWall", "(Ljava/lang/String;)V");
	if (gMVField.ShowOfferWallID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.NOTICEPAYURLID = env->GetMethodID(gMVField.mobileViewClass,
			"NoticePayUrl", "(Ljava/lang/String;)V");
	if (gMVField.NOTICEPAYURLID == NULL) {
		LOGWHERE();
		return;
	}
	gMVField.OpenWebUrlPayID = env->GetMethodID(gMVField.mobileViewClass,
			"openWebViewUrl", "(Ljava/lang/String;)V");
	if (gMVField.OpenWebUrlPayID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.showImgProgressID = env->GetMethodID(gMVField.mobileViewClass,
			"showImgProgress", "()V");
	if (gMVField.showImgProgressID == NULL) {
		LOGWHERE();
		return;
	}

	gMVField.CallTextEditID = env->GetMethodID(gMVField.mobileViewClass,
			"CallTextEdit", "(Ljava/lang/String;II)V");
	if (gMVField.CallTextEditID == NULL)

	{
		LOGWHERE();
		return;
	}

	////
	XMobileView* mobileView = new XMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	XMobileView* old = setMobileView(env, thiz, mobileView);
	if (old != NULL) {
		delete old;
	}

	return;

}

static void MobileView_CreateView(JNIEnv *env, jobject thiz, int width,
		int height, jstring strcid, jstring strUA, jstring strMac) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	char *chcid = js2c(strcid);
	char *chUA = js2c(strUA);
	char *chMac = js2c(strMac);
	mobileView->CreateView(width, height, chcid, chUA, chMac);
	free(chcid);
	free(chUA);
	free(chMac);
	return;
}

static void MobileView_SetBmpData(JNIEnv *env, jobject thiz,
		jobject bitmap) {
	AndroidBitmapInfo infocolor;
	int ret = 0;
	void *pixelscolor;
	if ((ret = AndroidBitmap_getInfo(env, bitmap, &infocolor)) < 0) {
		LOGWHEREMSG( "error bmp getinfo" );
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixelscolor)) < 0) {
		LOGWHEREMSG("error lockpixels" );
	}

	XMobileView* mobileView = getMobileView(env, thiz);
	mobileView->SetBmpData((unsigned char *) pixelscolor);

	AndroidBitmap_unlockPixels(env, bitmap);

//	jbyte *outjbyte = 0;
//	jbyteArray outbytearray = 0;
//	unsigned char *out = NULL;
//
//	jclass bytebufferclass = env->GetObjectClass( outbuf );
//	jmethodID arraymethodid = env->GetMethodID( bytebufferclass, "array", "()[B" );
//	outbytearray = ( jbyteArray)env->CallObjectMethod( outbuf, arraymethodid );
//	outjbyte = env->GetByteArrayElements( outbytearray, 0 );
//
//	out = ( unsigned char *)outjbyte;
//	XMobileView* mobileView = getMobileView(env, thiz );
//	mobileView->GetOutBuffer( out );

	return;
}

//static void com_jqbar_MobileView_OnPaint( JNIEnv *env, jobject thiz, jobject canvas )
//{
//	LOGWHEREMV();
//	SkCanvas* canv = GraphicsJNI::getNativeCanvas(env, canvas );
//	if ( canv == NULL )
//	{
//		LOGWHERE();
//		return;
//	}
//	XMobileView* mobileView = getMobileView(env, thiz );
//	if( mobileView == NULL )
//	{
//		LOGWHERE();
//		return;
//	}
//	mobileView->OnPaint(canv);
//
//	return;
//}

static void MobileView_SetScreenWH(JNIEnv *env, jobject thiz, int width,
		int height) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SetScreenWH(width, height);
	return;
}

static void MobileView_freeParameter(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->freeParameter();
	return;
}
static void MobileView_NativeFinalize(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();

	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	delete mobileView;

	return;
}

void MobileView_PostInvalidate(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	env->CallVoidMethod(thiz, gMVField.postInvalidateID);
}

static void MobileView_OnIdle(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->setEnv(env, thiz);
	mobileView->OnIdle();

	return;
}

static void MobileView_TimeTickCpp(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->TimeTick();
	return;
}

static void MobileView_SetTopPos(JNIEnv *env, jobject thiz, int pos) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SetTopPos(pos);
	return;
}
static void MobileView_SetBottomPos(JNIEnv *env, jobject thiz, int pos) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SetBottomPos(pos);
	return;
}
static void MobileView_onTouchDown(JNIEnv *env, jobject thiz, int x, int y,
		int pos) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	XPoint pt(x, y);
	if (pos == 0) {
		XPoint pt2(x, y);
		mobileView->MouseMove(pt2, pos);
	}
	// time_t first_time;
	// first_time = time(NULL);
	//	LOGE("%s, %s %d, %d, %d", __FILE__, "first_time", __LINE__, 1,first_time);
	mobileView->MouseDown(pt, pos);
	// time_t second_time;
	// second_time = time(NULL);
	//LOGE("%s, %s %d, %d, %d", __FILE__, "second_time", __LINE__,1, second_time);
	return;
}

static void MobileView_onTouchMove(JNIEnv *env, jobject thiz, int x, int y,
		int pos) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	XPoint pt(x, y);
	//time_t first_time_move;
	// first_time_move = time(NULL);
	//	LOGE("%s, %s %d, %d, %d", __FILE__, "first_time_move", __LINE__, 2,first_time_move);
	mobileView->MouseMove(pt, pos);
	// time_t second_time_move;
	// second_time_move = time(NULL);
	//	LOGE("%s, %s %d, %d, %d", __FILE__, "second_time_move", __LINE__, 2,second_time_move);
	//LOGE("%s, %s %d, %d, %d", __FILE__, __FUNCTION__, __LINE__, x, y );
	return;
}

static void MobileView_onTouchUp(JNIEnv *env, jobject thiz, int x, int y,
		int pos) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}

	XPoint pt(x, y);
	mobileView->MouseUp(pt, pos);
	//LOGE("%s, %s %d, %d, %d", __FILE__, __FUNCTION__, __LINE__, x, y );
	return;
}

static jboolean MobileView_onKeyDown(JNIEnv *env, jobject thiz, jint code,
		jobject keyEvent) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	int icode = (int) code;
	XKEYMSG msg = { icode, icode, 0, 0, 0, 0 };
	mobileView->KeyDown(msg);
	//mobileView->Char( icode );
	return true;
}

static jboolean MobileView_onKeyUp(JNIEnv *env, jobject thiz, jint code,
		jobject keyEvent) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	int icode = (int) code;
	XKEYMSG msg = { icode, icode, 0, 0, 0, 0 };
	mobileView->KeyUp(msg);
	return true;
}

static void MobileView_SetXEdit(JNIEnv *env, jobject thiz, jstring str) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}

	mobileView->XEdit_SetText(str);
	return;
}

static jboolean MobileView_OnDomQuit(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	mobileView->OnDomQuit();
	return true;
}

static jboolean MobileView_OnDomStop(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	mobileView->OnDomStop();
	return true;
}

static jboolean MobileView_OnDomHome(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->OnDomHome();
}

static jboolean MobileView_OnDomBack(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	mobileView->OnDomBack();
	return true;
}

static jboolean MobileView_OnDomForward(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	mobileView->OnDomForward();
	return true;
}

static jboolean MobileView_OnDomAddbook(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	mobileView->OnDomAddbook();
	return true;
}

static jboolean MobileView_OnDomUpdate(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	mobileView->OnDomUpdate();
	return true;
}

static int MobileView_clearCache(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}

	return mobileView->ClearCache();
}

static jboolean MobileView_CanFlashSave(JNIEnv *env, jobject thiz)
{
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}
	return mobileView->CanFlashSave();
}

static void MobileView_FlashSave(JNIEnv *env, jobject thiz)
{
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashSave();
	return;
}

static void MobileView_FlashSaveCancel(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashSaveCancel();
	return;
}

static void MobileView_Exit_setShader(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}

	mobileView->Exit_setShader();
	return;
}
static jboolean MobileView_ExitFlash(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	LOGWHERE();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}

	return mobileView->ExitFlash();
}

static void MobileView_ConnectUrl(JNIEnv *env, jobject thiz, jstring str) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	char *churl = js2c(str);
	mobileView->ConnectUrl(churl);
	free(churl);
	return;
}

static void MobileView_SetDeviceId(JNIEnv *env, jobject thiz, jstring str) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	char *chDevice = js2c(str);
	mobileView->SetDeviceID(chDevice);
	free(chDevice);
	return;
}

static void MobileView_UpScore(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->UpScore();
	return;
}

static jboolean MobileView_IsUpScore(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->IsUpScore();
}

static void MobileView_FlashPause(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashPause();
	return;
}

static void MobileView_FlashResume(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashResume();
	return;
}

static void MobileView_FlashRestart(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashRestart();
	return;
}

static jboolean MobileView_CanBack(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->CanBack();

}

static jboolean MobileView_CanForward(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->CanForward();
}

static jboolean MobileView_IsCurrentHomePage(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->IsCurrentHomePage();
}

static jint MobileView_FlashContentMode(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}
	return mobileView->FlashContentMode();
}

static void MobileView_FlashZoomInOrOut(JNIEnv *env, jobject thiz, jint inOut) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashZoomInOrOut(inOut);
	return;
}

static void MobileView_FlashZoomInOut(JNIEnv *env, jobject thiz, jint data) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashZoomInOut(data);
	return;
}

static jint MobileView_FlashLockMode(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}
	return mobileView->FlashLockMode();
}

static void MobileView_SetFlashLockMode(JNIEnv *env, jobject thiz,
		jboolean bLock) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SetFlashLockMode(bLock);
	return;
}

static void MobileView_FlashLockZoomInOut(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashLockZoomInOut();
	return;
}

static void MobileView_FlashUnLockZoomInOut(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashUnLockZoomInOut();
	return;
}

static void MobileView_SetFlashZoomMode(JNIEnv *env, jobject thiz, jint mode) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SetFlashZoomMode(mode);
	return;
}

static void MobileView_FlashPointZoom(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->FlashPointZoom();
	return;
}

static int MobileView_GetZoomPecent(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}
	return mobileView->GetZoomPecent();
}

static void MobileView_SwitchHelp(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SwitchHelp();
	return;
}

static void MobileView_SwitchAcc(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SwitchAcc();
	return;
}

static void MobileView_SwitchButton(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SwitchButton();
	return;
}

static void MobileView_SenseAcc(JNIEnv *env, jobject thiz, float x, float y,
		float z) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	//LOGE("%s, %d, %f, %f, %f", __FILE__, __LINE__, x, y, z );
	mobileView->ACC(x, y, z);
	XAcc::SetAcc(x / 10, y / 10, z / 10);
	return;

}

static jboolean MobileView_HasFlashACC(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->HasFlashACC();
}

static jboolean MobileView_IsFlashAccEnable(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->IsFlashAccEnable();
}

static jboolean MoblieView_HasFlashButton(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->HasFlashButton();
}

static void MobileView_CheckUpgrade(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	return mobileView->CheckUpgrade();
}

static void MobileViwe_ClosePopView(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	return mobileView->ClosePopView();
}

static void MoblieView_OpenPayWindow(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	return mobileView->OpenPayWindow();
}

static jboolean MobileView_CanPayWindow(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->CanPayWindow();
}

static jboolean MobileView_IsPause(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return false;
	}
	return mobileView->IsPause();
}

static void MobileView_SetSilence(JNIEnv *env, jobject thiz,
		jboolean bsilence) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SetSilence(bsilence);
	return;
}

static void MobileView_OpenFeedbackView(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->OpenFeedbackView();
	return;
}

static void MobileView_SetViewRegion(JNIEnv *env, jobject thiz, int left,
		int top, int right, int bottom) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return;
	}
	mobileView->SetViewRegion(left, top, right, bottom);
	return;
}
// zhengjl 2012.03
static jstring MobileMain_encUIDStr(JNIEnv *env, jobject thiz, jstring str) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}

	char *churl = js2c(str);
	char *ret = mobileView->encUIDStr(churl);
	free(churl);

	jstring jstr = c2js(ret);
	return jstr;
}

static jstring MobileMain_getAppHeaders(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}
	char* result = mobileView->getAppHeaders();
	jstring jstr = c2js(result);
	return jstr;
}

static jstring MobileMain_getAppUA(JNIEnv *env, jobject thiz) {
	LOGWHEREMV();
	XMobileView* mobileView = getMobileView(env, thiz);
	if (mobileView == NULL) {
		LOGWHERE();
		return 0;
	}
	const char *ret = mobileView->getAppUA();
	jstring jstr = c2js(ret);
	return jstr;
}

static JNINativeMethod gMobileViewMethods[] =
{
		{ "cppSurfaceCreate","(Landroid/view/Surface;)V",									(void*) MobileView_cppSurfaceCreate },
		{ "cppSurfaceChange", "(II)V", 														(void*) MobileView_cppSurfaceChange },
		{ "termRenderGLES", "()V", 															(void*) MobileView_TermRenderGLES },
		{ "nativesetWinWH", "(III)V", 															(void*) MobileView_NativesetWinWH },
		{ "CreateView",		"(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",	(void*) MobileView_CreateView },
		{ "setBmpData",		"(Landroid/graphics/Bitmap;)V",									(void*) MobileView_SetBmpData },
		{ "nativeInit",		"()V", 															(void*) MobileView_NativeInit },
		{ "setScreenWH",	"(II)V", 														(void*) MobileView_SetScreenWH },
		{ "freeParameter",	"()V", 															(void*) MobileView_freeParameter },
		{ "nativeFinalize",	"()V", 															(void*) MobileView_NativeFinalize },
		{ "OnIdle", 		"()V",															(void*) MobileView_OnIdle },
		{ "timeTickCpp", 	"()V",															(void*) MobileView_TimeTickCpp },
		{ "setTopPos", 		"(I)V",															(void*) MobileView_SetTopPos },
		{ "setBottomPos", 	"(I)V",															(void*) MobileView_SetBottomPos },
		{ "onKeyDownCpp", 	"(ILandroid/view/KeyEvent;)Z",									(jboolean*) MobileView_onKeyDown },
		{ "onKeyUpCpp",		"(ILandroid/view/KeyEvent;)Z",									(jboolean *) MobileView_onKeyUp },
		{ "onTouchDownCpp",	"(III)V", 														(void*) MobileView_onTouchDown },
		{ "onTouchMoveCpp", "(III)V",														(void*) MobileView_onTouchMove },
		{ "onTouchUpCpp",	"(III)V", 														(void*) MobileView_onTouchUp },
		{ "SetXEdit",		"(Ljava/lang/String;)V", 										(void*) MobileView_SetXEdit },
		{ "onDomQuit", 		"()Z", 															(jboolean*) MobileView_OnDomQuit },
		{ "onDomStop", 		"()Z", 															(jboolean*) MobileView_OnDomStop },
		{ "onDomHome", 		"()Z", 															(jboolean*) MobileView_OnDomHome },
		{ "onDomBack", 		"()Z", 															(jboolean*) MobileView_OnDomBack },
		{ "onDomForward", 	"()Z", 															(jboolean*) MobileView_OnDomForward },
		{ "onDomAddbook", 	"()Z", 															(jboolean*) MobileView_OnDomAddbook },
		{ "onDomUpdate", 	"()Z", 															(jboolean*) MobileView_OnDomUpdate },
		{"clearCache", 		"()I", 															(jint*) MobileView_clearCache },
		{ "canFlashSave", 	"()Z", 															(jboolean*) MobileView_CanFlashSave },
		{ "flashSave", 		"()V", 															(void*) MobileView_FlashSave },
		{"flashSaveCancel", "()V",															(void*) MobileView_FlashSaveCancel },
		{ "exitFlash",		"()Z", 															(void*) MobileView_ExitFlash },
		{"exit_setShader", 	"()V",															(void*) MobileView_Exit_setShader },
		{ "connectUrl", 	"(Ljava/lang/String;)V",										(void*) MobileView_ConnectUrl },
		{ "setDeviceId", 	"(Ljava/lang/String;)V",										(void*) MobileView_SetDeviceId },
		{ "upScore", 		"()V", 															(void*) MobileView_UpScore },
		{ "isUpScore",		"()Z", 															(void*) MobileView_IsUpScore },
		{ "flashPause",		"()V", 															(void*) MobileView_FlashPause },
		{ "flashResume",	"()V", 															(void*) MobileView_FlashResume },
		{ "flashRestart", 	"()V", 															(void*) MobileView_FlashRestart },
		{"canBack", 		"()Z",	 														(void*) MobileView_CanBack },
		{"canForward", 		"()Z", 															(void*) MobileView_CanForward },
		{"isCurrentHomePage", "()Z",														(void*) MobileView_IsCurrentHomePage },
		{ "flashContentMode", "()I", 														(void*) MobileView_FlashContentMode },
		{ "flashZoomInOrOut", "(I)V", 														(void*) MobileView_FlashZoomInOrOut },
		{ "flashZoomInOut", "(I)V", 														(void*) MobileView_FlashZoomInOut },
		{ "flashLockMode", 	"()I", 															(void*) MobileView_FlashLockMode },
		{"setFlashLockMode", "(Z)V",														(void*) MobileView_SetFlashLockMode },
		{"flashLockZoomInOut", "()V",														(void*) MobileView_FlashLockZoomInOut },
		{"flashUnLockZoomInOut", "()V",														(void*) MobileView_FlashUnLockZoomInOut },
		{"setFlashZoomMode", "(I)V",														(void*) MobileView_SetFlashZoomMode },
		{"flashPointZoom", 	"()V",															(void*) MobileView_FlashPointZoom },
		{ "getZoomPecent",	"()I", 															(void*) MobileView_GetZoomPecent },
		{ "switchHelp", 	"()V", 															(void*) MobileView_SwitchHelp },
		{"switchAcc", 		"()V", 															(void*) MobileView_SwitchAcc },
		{ "switchButton", 	"()V", 															(void*) MobileView_SwitchButton },
		{"senseAcc", 		"(FFF)V", 														(void*) MobileView_SenseAcc },
		{"hasFlashACC", 	"()Z", 															(void*) MobileView_HasFlashACC },
		{ "isFlashAccEnable", "()Z", 														(void*) MobileView_IsFlashAccEnable },
		{ "hasFlashButton", "()Z", 															(void*) MoblieView_HasFlashButton },
		{ "checkUpgrade", 	"()V", 															(void*) MobileView_CheckUpgrade },
		{ "closePopView", 	"()V", 															(void*) MobileViwe_ClosePopView },
		{ "openPayWindow", "()V", 															(void*) MoblieView_OpenPayWindow },
		{ "canPayWindow", 	"()Z", 															(void*) MobileView_CanPayWindow },
		{"isPause", 		"()Z", 															(void*) MobileView_IsPause },
		{"setSilence", 		"(Z)V", 														(void*) MobileView_SetSilence },
		{"openFeedbackView", "()V",															(void*) MobileView_OpenFeedbackView },
		{"setViewRegion", 	"(IIII)V",														(void*) MobileView_SetViewRegion },
		{ "encUIDStr",		"(Ljava/lang/String;)Ljava/lang/String;",						(jstring*) MobileMain_encUIDStr },	// zhengjl 2012.03
		{ "getAppUA", 		"()Ljava/lang/String;",											(jstring*) MobileMain_getAppUA },
		{ "getAppHeaders",	"()Ljava/lang/String;",											(jstring*) MobileMain_getAppHeaders },
};

int register_MobileView(JNIEnv *env) {
	return AndroidRuntime::registerNativeMethods(env,
	MOBILEVIEW, gMobileViewMethods, NELEM(gMobileViewMethods));
}



#include "GlobalFun.h"
#include "XEdit.h"
#include "MobileMain.h"

XMobileMain::XMobileMain()
{

}

XMobileMain::XMobileMain(JNIEnv *env, jobject obj)
{
	mEnv = env;
	mmmobject = obj;
}

XMobileMain::~XMobileMain()
{

}

void XMobileMain::SetEnv(JNIEnv* env, jobject thiz)
{
	mEnv = env;
	mmmobject = thiz;
}

void XMobileMain::SetEnv( JNIEnv * env )
{
	mEnv = env;
}

void XMobileMain::SetObj( jobject thiz )
{
	mmmobject = thiz;
}

jobject XMobileMain::GetObj()
{
	return mmmobject;
}

void XMobileMain::SetCallTextEditID(jmethodID callTE_ID)
{
	mCallTextEditID = callTE_ID;
}

void XMobileMain::SetStartFlashViewID( jmethodID startFlashView_ID )
{
	mStartFlashViewID = startFlashView_ID;
}

void XMobileMain::SetOpenViewUrlID( jmethodID openViewUrlID )
{
	mOpenViewUrlID = openViewUrlID;
}

void XMobileMain::SetUpgradeAppID( jmethodID upgradeAppID )
{
	mUpgradeAppID = upgradeAppID;
}

void XMobileMain::CallTextEdit( jstring inStr,  int ilength, int style)
{
	JNIEnv* env = GetJNIEnv();
	env->CallVoidMethod(mmmobject, mCallTextEditID, inStr, ilength, style ); //
	return;
}

void XMobileMain::StartFlashView( int userSta )
{
	JNIEnv* env = GetJNIEnv();
	env->CallVoidMethod(mmmobject, mStartFlashViewID, userSta ); //
	return;
}

void XMobileMain::OpenViewUrl( jstring strUrl )
{
	JNIEnv* env = GetJNIEnv();
	env->CallVoidMethod(mmmobject, mOpenViewUrlID, strUrl );
	return;
}

void XMobileMain::UpgradeApp( jstring strUrl, int status )
{
	JNIEnv* env = GetJNIEnv();
	env->CallVoidMethod(mmmobject, mUpgradeAppID, strUrl, status );
	return;
}

void XMobileMain::XEdit_SetText( jstring str )
{
	if( mXEdit == NULL )
		return;
	XWCHAR *xwstr = js2xw( str );
	((XEdit*)mXEdit)->SetText( xwstr );
	delete []xwstr;
	return;
}


// zhengjl 2012.03
void XMobileMain::SetPaySDKID( jmethodID paySDKID )
{
	mPaySDKID = paySDKID;
}
void XMobileMain::SetOutLoginID( jmethodID outLoginID )
{
	mOutLoginID = outLoginID;
}
void XMobileMain::PaySDK( jstring strUrl )
{
	JNIEnv* env = GetJNIEnv();
	env->CallVoidMethod(mmmobject, mPaySDKID, strUrl );
	return;
}
void XMobileMain::OutLogin(int b)
{
	JNIEnv* env = GetJNIEnv();
	env->CallVoidMethod(mmmobject, mOutLoginID, b );
}


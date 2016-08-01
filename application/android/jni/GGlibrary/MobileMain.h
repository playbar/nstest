
#ifndef __MOBILEMAIN_H__
#define __MOBILEMAIN_H__

#include "Define.h"
#include "utils/RefBase.h"
using namespace android;

class XMobileMain
{
public:
	XMobileMain();
	XMobileMain(JNIEnv *env, jobject obj);
	~XMobileMain();

public:
	void SetXEdit( void *xedit ){ mXEdit = xedit; }
	void XEdit_SetText( jstring str );

private:
	void *mXEdit;

public:
	void SetEnv(JNIEnv* env, jobject thiz);
	void SetEnv( JNIEnv * env );
	void SetObj( jobject thiz );
	jobject GetObj();
	void SetCallTextEditID(jmethodID callTE_ID);
	void SetStartFlashViewID( jmethodID startFlashView_ID );
	void SetOpenViewUrlID( jmethodID openViewUrlID );
	void SetUpgradeAppID( jmethodID upgradeAppID );
	void SetPaySDKID( jmethodID paySDKID );
	void SetOutLoginID( jmethodID outLoginID );	// zhengjl 2012.03

	void CallTextEdit(jstring inStr, int ilength, int style );
	void StartFlashView( int userSta );
	void OpenViewUrl( jstring strUrl );
	void UpgradeApp( jstring strUrl, int status );
	void PaySDK( jstring strUrl );
	void OutLogin(int b);	// zhengjl 2012.03


private:
	JNIEnv* mEnv;
	jobject mmmobject;
	jmethodID mCallTextEditID;
	jmethodID mStartFlashViewID;
	jmethodID mOpenViewUrlID;
	jmethodID mUpgradeAppID;
	jmethodID mPaySDKID;
	jmethodID mOutLoginID;	// zhengjl 2012.03
};


#endif // __MOBILEMAIN_H__

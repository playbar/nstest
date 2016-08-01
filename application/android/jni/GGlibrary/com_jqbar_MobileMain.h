#ifndef __JQBAR_MOBILEMAIN_H__
#define __JQBAR_MOBILEMAIN_H__

//MobileMain.java
struct stmmField
{
	jclass 		XMoblieMainClass;
	jfieldID    XMobileMainID;
	jfieldID    XMobileMainCpuID;
	jfieldID    XMobileMainRamID;
	jfieldID    XMobileMainDpiID;
	jmethodID   OpenViewUrlID;
	jmethodID	UpgradeAppID;
	jmethodID   PaySDKID;
	jmethodID   OutLoginID;	// zhengjl 2012.03
	jmethodID   OpenWebViewUrlID;
	jmethodID   JavaSetFontID;
	jmethodID	JavaDrawStringID;
};

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/////////////////////////////////////////////////////////////////

int register_MobileMain( JNIEnv *env );


#ifdef __cplusplus
}
#endif //__cplusplus


#endif // __GG_INTERFACE__

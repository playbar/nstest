
#ifndef __JQBAR_MOBILEVIEW_H__
#define __JQBAR_MOBILEVIEW_H__

//MobileView.java
struct stMVfields
{
	jclass mobileViewClass;
	jfieldID mobileViewID;
	//jfieldID mSurfaceID;
	jmethodID postInvalidateID;
	jmethodID postInvalidateRectID;
	jmethodID FlashSaveFinishID;
	jmethodID StartFlashViewID;
	jmethodID CloseFlashViewID;
	jmethodID LoadPageStartID;
	jmethodID LoadPagePercentID;
	jmethodID LoadPageEndID;
	jmethodID ShowBarsaleID;
	jmethodID FlashLoadingEndID;
	jmethodID CloseSubViewID;
	jmethodID SetModeID;
	jmethodID showImgProgressID;
	jmethodID CallTextEditID;
	jmethodID changeScreenLID;
	jmethodID changeScreenPID;
	jmethodID changeScreenDir;
	jmethodID WeiXinInStalledID;
	jmethodID WXSendTextID;
	jmethodID WXSendPhotoID;
	jmethodID WXSendLinkID;
	jmethodID WXSendMusicID;
	jmethodID WXSendVideoID;
	jmethodID WXSendAppMessgID;
	jmethodID WXSendNoGifID;
	jmethodID WXSendGifID;
	jmethodID WXSendFileID;
	jmethodID BWPayUrlID;
	jmethodID ShowOfferWallID;
	jmethodID NOTICEPAYURLID;
	jmethodID OpenWebUrlPayID;
	jmethodID MakeErrorPageID;
	jmethodID refresh;
};

extern stMVfields gMVField;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int register_MobileView( JNIEnv *env );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // end of __JQBAR_MOBILEVIEW_H__

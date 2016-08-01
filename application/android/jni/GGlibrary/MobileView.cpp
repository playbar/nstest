// MobileView.cpp: implementation of the XMobileView class.
//
//////////////////////////////////////////////////////////////////////

//#include <vector>

#include "GGDom.h"
#include "XMainWnd.h"
#include "GlobalFun.h"
#include "MultiStringUtil.h"
#include "XFlashView.h"
#include "XDomViewExt.h"
#include "XDialogClear.h"
#include "XDialogSave.h"
#include "ShellCore.h"
#include "com_jqbar_MobileView.h"
#include "com_jqbar_MobileMain.h"
#include "MobileView.h"
#include "GlesUtil.h"
#include <unistd.h>
#include "XBWSendMessageContent.h"

extern BWSendMessageContentClass *g_WeiXinShareSessionSavedData;
extern BWSendMessageContentClass *g_BaiWanPayUrlSavedData;
extern bool g_WeiXinAppIsInstalled;
extern int g_WeiXinSendType;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const static int USER_NORMAL = XCMD_USER;
const static int FLASH_TYPE_GAME = XCMD_USER + 114;
const static int FLASH_TYPE_MEDIA = XCMD_USER + 115;
const static int FLASH_TYPE_WEBGAME = XCMD_USER + 116;
const static int USER_TYPE_NONE = XCMD_USER + 117;

//XImage gImage;

XMobileView::XMobileView() {
	mbMouseDown = false;
	mbSlip = false;
	//mbLoadingPage = false;
	mUserStatus = USER_NORMAL;
	miMode = 0;
	m_nCurType = 0;
	m_bEnuHand = XFALSE;
	mpSlipBmpData = NULL;
	mTopPos = 0;
	mBottomPos = 0;
	//m_strWorkPath = WORKPATH;
	XInitGGDom();
}

XMobileView::XMobileView(JNIEnv* env, jobject mvobj) {
	mbMouseDown = false;
	mbSlip = false;
	changel = 0;
	changep = 1;
	//mbLoadingPage = false;
	mUserStatus = USER_NORMAL;
	m_pTopmost = XNULL;
	miMode = 0;
	mEnv = env;
	mmVobject = mvobj;
	m_nCurType = 0;
	m_bEnuHand = XFALSE;
	mpSlipBmpData = NULL;
	//m_strWorkPath = WORKPATH;
	mAppStatus = 0;
	mTopPos = 0;
	mBottomPos = 0;
	XInitGGDom();
	g_WeiXinAppIsInstalled = mEnv->CallBooleanMethod(mmVobject,
			gMVField.WeiXinInStalledID);

}

XMobileView::~XMobileView() {
	XReleaseGGDom();
	//JNIEnv* env = GetJNIEnv();
	//mSlipBmp = NULL;
	if (mpSlipBmpData != NULL) {
		delete[] mpSlipBmpData;
		mpSlipBmpData = NULL;
	}
}

void XMobileView::Release() {
	m_pTopmost = XNULL;
	XReleaseGGDom();
}

void XMobileView::CreateView(int width, int height, char *pcid, char *pUA, char *pMac)
{
	//int *pi = NULL;
	//*pi = 10;
	//	XPCWSTR pName = utf8ToUnicode( "/sdcard/test.jpg" );
	//	gImage.Create( pName );
	//	delete []pName;

	mScreenWidth = width;
	mScreenHight = height;
	mBottomPos = height - GFunGetBrowerHeight();
	//m_clientApply.m_info.strUserAgent = "AndroidPad;10.0.035;Android2.3;BWTEST/2.3.1";
	XString16 strTitle = XResource::LoadWString(XIDS_TITLE);
	XRect rt(0, 0, mScreenWidth, mScreenHight);
	m_draw.Create(mScreenWidth, mScreenHight, 32);
	XPCWSTR pwch = strTitle.GetData();
	XPCTSTR pch = WORKPATH; //m_strWorkPath.GetData();
	Create(rt, &m_draw, pwch, pch);
	XDomView* view = GetActiveView();
	if (view == NULL) {
		LOGWHERE();
		return;
	}
	//	if( m_clientApply.IsEmptyCID())
	//	{
	//		m_clientApply.SetCID( pcid );
	//	}
	m_clientApply.SetCID(pcid);
	m_clientApply.m_info.strMAC = pMac;
	LOGWHEREMSG(m_clientApply.m_info.strMAC.GetData());
	if (pUA != NULL) {
		XString8 strTmp = m_clientApply.m_info.strUserAgent;
		int ipos = strTmp.ReverseFind(';');
		XString8 strLeft;
		strTmp.Left(strLeft, ipos + 1);
		ipos = strTmp.ReverseFind('/');
		XString8 strRight;
		strTmp.Right(strRight, strTmp.GetLength() - ipos);

		m_clientApply.m_info.strUserAgent = strLeft;
		m_clientApply.m_info.strUserAgent += pUA;
		m_clientApply.m_info.strUserAgent += strRight;
	}
	LOGWHEREMSG(m_clientApply.m_info.strUserAgent.GetData());

	miDatalen = mScreenWidth * mScreenHight * 4;

	//http://m.jqbar.com/testweb/test/testlist.aspx
	//req.URL.SetURL("http://m.jqbar.com/product/");
	//req.URL.SetURL("D:/sdcard/byone/Local/tante.swf");
	//req.URL.SetURL("http://m.jqbar.com/as/as3test.aspx");

}

void XMobileView::SetBmpData(unsigned char *out) {
//	LOGWHEREVAL( miDatalen );
	memmove(out, m_draw.GetXImage()->GetImgData(), miDatalen);
//	LOGWHERE();
}

void XMobileView::SetScreenWH(int width, int height) {
	mScreenWidth = width;
	mScreenHight = height;
	miDatalen = mScreenWidth * mScreenHight * 4;
	LOGWHEREVAL(miDatalen);

	m_draw.Create(mScreenWidth, mScreenHight, 32);
	XRect rect(0, 0, mScreenWidth, mScreenHight);
	MoveWindow(rect);
	return;
}

void XMobileView::freeParameter() {
	if (g_WeiXinShareSessionSavedData->parameter1 != NULL)
		free(g_WeiXinShareSessionSavedData->parameter1);
	g_WeiXinShareSessionSavedData->parameter1 = NULL;

	if (g_WeiXinShareSessionSavedData->parameter2 != NULL)
		free(g_WeiXinShareSessionSavedData->parameter2);
	g_WeiXinShareSessionSavedData->parameter2 = NULL;

	if (g_WeiXinShareSessionSavedData->parameter3 != NULL)
		free(g_WeiXinShareSessionSavedData->parameter3);
	g_WeiXinShareSessionSavedData->parameter3 = NULL;

	if (g_WeiXinShareSessionSavedData->parameter4 != NULL)
		free(g_WeiXinShareSessionSavedData->parameter4);
	g_WeiXinShareSessionSavedData->parameter4 = NULL;

	if (g_WeiXinShareSessionSavedData->parameter5 != NULL)
		free(g_WeiXinShareSessionSavedData->parameter5);
	g_WeiXinShareSessionSavedData->parameter5 = NULL;

	if (g_WeiXinShareSessionSavedData->parameter6 != NULL)
		free(g_WeiXinShareSessionSavedData->parameter6);
	g_WeiXinShareSessionSavedData->parameter6 = NULL;
}

void XMobileView::setEnv(JNIEnv* env, jobject thiz) {
	mEnv = env;
	mmVobject = thiz;
}

XBOOL XMobileView::OnDomQuit() {
	PostMsg(XWM_COMMAND, XCMD_DOM_QUIT, 100, XMS_TOPMOST);
	return true;
}

XBOOL XMobileView::OnDomStop() {
	PostMsg(XWM_COMMAND, XCMD_DOM_STOP, 100, XMS_TOPMOST);
	return true;
}

XBOOL XMobileView::OnDomHome() {
	PostMsg(XWM_COMMAND, XCMD_DOM_HOME, 100, XMS_TOPMOST);
	return true;
}

XBOOL XMobileView::OnDomBack() {
	//LOGWHERE();
	//PostMsg(XWM_COMMAND, XCMD_DOM_BACK, 100, XMS_TOPMOST);

	//m_drawSlip.Create( mScreenWidth * 3 + 40, mScreenHight, 32 );
	//XPoint dst( mScreenWidth + 20, 0 );
	//XRect rst( 0, 0, mScreenWidth, mScreenHight );
	//m_drawSlip.Copy( m_draw, dst, rst );

	//mSlipBmp->setPixels( m_draw.GetXImage()->GetImgData());

	XDomView* view = GetActiveView();
	view->Back();

	mbBack = true;
	mbSlip = true;
	mbMouseDown = false;
	mLeftLen = mScreenWidth;
	mISlipLen = 21;
	Paint(m_draw);
	//XPoint dst1( 0, 0 );
	//XRect rst1( 0, 0, mScreenWidth, mScreenHight );
	//m_drawSlip.Copy( m_draw, dst1, rst1 );

	return true;
}
XBOOL XMobileView::OnDomForward() {
	//LOGWHERE();
	//PostMsg(XWM_COMMAND, XCMD_VIEW_FORWARD, 100, XMS_TOPMOST);
	XDomView* view = GetActiveView();
	view->Forward();
	return true;
}
XBOOL XMobileView::OnDomAddbook() {
	PostMsg(XWM_COMMAND, XCMD_DOM_ADDBOOK, 100, XMS_TOPMOST);
	return true;
}
XBOOL XMobileView::OnDomUpdate() {
	PostMsg(XWM_COMMAND, XCMD_DOM_UPDATE, 100, XMS_TOPMOST);
	return true;
}

XBOOL XMobileView::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch (nOpera)
	{
	case ANDROID_START_FLASH:
	{
//		LOGE( "startFlash" );
		if (mpSlipBmpData != NULL)
		{
			delete[] mpSlipBmpData;
			mpSlipBmpData = NULL;
		}
		//StartFlash();
		mUserStatus = FLASH_TYPE_GAME;
		mEnv->CallVoidMethod(mmVobject, gMVField.StartFlashViewID, mUserStatus);
		break;
	}
	case ANDROID_FLASH_LOADING_END: // flash加载完成
	{
		//LOGWHEREMSG( "flash loading end" );
		mEnv->CallVoidMethod(mmVobject, gMVField.FlashLoadingEndID);
		break;
	}
	case ANDROID_FLASH_CLOSESUBVIEW: {
		//LOGWHERE();
		mEnv->CallVoidMethod(mmVobject, gMVField.CloseSubViewID);
		break;
	}
	case ANDROID_START_LOAD_PAGE: {
		//	LOGWHEREMSG("start load page");
		if (mUserStatus == USER_NORMAL) {
			mEnv->CallVoidMethod(mmVobject, gMVField.LoadPageStartID);
		}
		break;
	}
	case ANDROID_END_LOAD_PAGE: {
		//	LOGWHEREMSG("end load page");
		if (mUserStatus == USER_NORMAL) {
			mEnv->CallVoidMethod(mmVobject, gMVField.LoadPageEndID);
			PostMsg(XWM_REPAINT, 0, (XU32) this, XMS_THIS);
		}
		break;
	}
	case ANDROID_LOAD_PROCESS: {
		//LOGWHEREVAL( (int)pData1 );
		if (mUserStatus == USER_NORMAL) {
			mEnv->CallVoidMethod(mmVobject, gMVField.LoadPagePercentID,
					(int) pData1);
		}
		break;
	}
	case ANDROID_LOAD_PAGE_NOT_EMPTY:
		LOGWHERE();
		break;
	case ANDROID_TOP_SCORE_AVAILABLE:
		LOGWHERE();
		break;
	case ANDROID_FLASH_SAVE_PERCENT: {
		int ipro = pData1;
		if (ipro > 100) {
			mEnv->CallVoidMethod(mmVobject, gMVField.FlashSaveFinishID);
			FlashResume();
		}
		break;
	}
	case XWM_SHOWBARANDONSALE: {
		XU32 idata = pData1;
		//LOGWHEREVAL( idata );
//		if( idata &0x1F != 0 )
		{
			mEnv->CallVoidMethod(mmVobject, gMVField.ShowBarsaleID, idata);
		}
		break;
	}
	//微信
	case XWM_WXSENDTEXT: {
		jstring jstrurl = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter1);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendTextID, jstrurl,
				g_WeiXinSendType);
		break;
	}
	case XWM_WXSENDPHOTO: {
		jbyteArray jbarray = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter1,
				g_WeiXinShareSessionSavedData->parameter1Len);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendPhotoID, jbarray,
				g_WeiXinShareSessionSavedData->parameter1Len, g_WeiXinSendType);
		break;
	}
	case XWM_WXSENDLINK: {
		jbyteArray jbarray = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter3,
				g_WeiXinShareSessionSavedData->parameter3Len);
		jstring jstitle = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter1);
		jstring jsdescription = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter2);
		jstring jswebrurl = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter4);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendLinkID, jstitle,
				jsdescription, jbarray, jswebrurl, g_WeiXinSendType);
		break;
	}
	case XWM_WXSENDMUSIC: {
		jbyteArray jbarray = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter3,
				g_WeiXinShareSessionSavedData->parameter3Len);
		jstring jstitle = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter1);
		jstring jsdescription = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter2);
		jstring musicurl = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter4);
		jstring musidatacurl = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter5);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendMusicID, jstitle,
				jsdescription, jbarray, musicurl, musidatacurl,
				g_WeiXinSendType);

		break;
	}
	case XWM_WXSENDVEDIO: {
		jbyteArray jbarray = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter3,
				g_WeiXinShareSessionSavedData->parameter3Len);
		jstring jstitle = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter1);
		jstring jsdescription = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter2);
		jstring videourl = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter4);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendVideoID, jstitle,
				jsdescription, jbarray, videourl, g_WeiXinSendType);

		break;
	}
	case XWM_WXSENDAPPMESSAGE: {
		jbyteArray jbarray1 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter3,
				g_WeiXinShareSessionSavedData->parameter3Len);
		jstring jstitle = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter1);
		jstring jsdescription = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter2);
		jstring extInfo = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter4);
		jstring url = c2js((char*) g_WeiXinShareSessionSavedData->parameter5);
		jbyteArray jbarray2 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter6,
				g_WeiXinShareSessionSavedData->parameter6Len);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendAppMessgID, jstitle,
				jsdescription, jbarray1, extInfo, url, jbarray2,
				g_WeiXinSendType);

		break;
	}
	case XWM_WXSENDNOTGIFCONTENT: {
		jbyteArray jbarray1 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter1,
				g_WeiXinShareSessionSavedData->parameter1Len);
		jbyteArray jbarray2 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter2,
				g_WeiXinShareSessionSavedData->parameter2Len);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendNoGifID, jbarray1,
				jbarray2, g_WeiXinSendType);
		break;
	}
	case XWM_WXSENDGIFCONTENT: {
		jbyteArray jbarray1 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter1,
				g_WeiXinShareSessionSavedData->parameter1Len);
		jbyteArray jbarray2 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter2,
				g_WeiXinShareSessionSavedData->parameter2Len);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendGifID, jbarray1,
				jbarray2, g_WeiXinSendType);
		break;
	}
	case XWM_WXSENDFILECONTENT: {
		LOGE("%s, %d--------> ", __FUNCTION__, __LINE__);

		jbyteArray jbarray1 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter3,
				g_WeiXinShareSessionSavedData->parameter3Len);
		jstring jstitle = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter1);
		jstring jsdescription = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter2);
		jbyteArray jbarray2 = c2jbarray(
				(char*) g_WeiXinShareSessionSavedData->parameter4,
				g_WeiXinShareSessionSavedData->parameter4Len);
		jstring fileExt = c2js(
				(char*) g_WeiXinShareSessionSavedData->parameter5);
		mEnv->CallVoidMethod(mmVobject, gMVField.WXSendFileID, jstitle,
				jsdescription, jbarray1, jbarray2, fileExt, g_WeiXinSendType);
		break;
	}
		//payUrl
	case XWM_BWPAYURL: {
		StUTF8String str((Stringp) pData1);
		jstring jstrurl = c2js(str.c_str());
		mEnv->CallVoidMethod(mmVobject, gMVField.BWPayUrlID, jstrurl);
		break;
	}
		//Domob_offerwall
	case XWM_YMSHOWOFFERWALL: {
		mEnv->CallVoidMethod(mmVobject, gMVField.ShowOfferWallID, NULL);
		break;
	}
	case ANDROID_NOTICE_PAYURL: {
		jstring jstrurl = c2js((char*) pData2);
		mEnv->CallVoidMethod(mmVobject, gMVField.NOTICEPAYURLID, jstrurl);
		break;
	}
	case ANDROID_SHOWWEBVIEW: {
		jstring jstrurl = c2js_len((char*) g_BaiWanPayUrlSavedData->parameter1,
				g_BaiWanPayUrlSavedData->parameter1Len);
		mEnv->CallVoidMethod(mmVobject, gMVField.OpenWebUrlPayID, jstrurl);

		break;
	}
	case XWM_APP_STATUS: //升级状态
	{
		//LOGWHERE();
		mAppStatus = (int) pData1 - 48;
		break;
	}
	case XWM_NEW_APP: //升级URL
	{
		//LOGWHERE();
		XString8 *pstr8 = (XString8*) pData1;
		char * pchurl = pstr8->GetData();
		jstring jstrurl = c2js(pchurl);
		GFunUpgradeApp(jstrurl, mAppStatus);
		break;
	}
	case ANDROID_UPDATE_CLICKOPT:
		//LOGWHERE();
		mEnv->CallVoidMethod(mmVobject, gMVField.showImgProgressID);
		break;
	case XWM_PAYSDK_PID: {
		jstring jstrurl = c2js((char*) pData1);
		GFunPaySDK(jstrurl);
	}
		break;
	case XWM_OUTLOGIN: {
		GFunOutLogin(pData1 - 48);
	}
		break;

	case XWM_STARTEDIT: {
		LOGWHERE();
		int nMaxLength = pData1 >> 16;
		XU8 nStyle = pData1 & 0xff;
		XWCHAR *str = (XWCHAR *) pData2;
		jstring jstr = xw2js(str);
		mEnv->CallVoidMethod(mmVobject, gMVField.CallTextEditID, jstr,
				nMaxLength, nStyle);
		break;
	}
	case XCMD_FLASHMODECMD:
	{
		int mode = (int) pData1;
		mEnv->CallVoidMethod(mmVobject, gMVField.changeScreenDir, mode);
		break;
	}
	case XWM_REPAINT:
	{
		//m_buffClip = m_clip;
		XRect clip(m_clip);
		bool b = Paint(m_draw);
		if (b)
		{
			if (gANativeWindow != NULL&&windowViewInitGL_success==false)
			{
				MakeCurrent( true );
				LoadImgDataTex(gWidth, gHeight,
						m_draw.GetXImage()->GetImgData());
				GlesPaint();
				FlushBuffer();
				MakeCurrent( false );
			}

		}
		else
		{
			InvalidateRect(clip);
		}
		break;
	}
	case XWM_REFRESH://退出游戏后重新刷新界面
			mEnv->CallVoidMethod(mmVobject, gMVField.refresh);
			break;
	case XWM_UNBINGMAKECURRENT:
		    MakeCurrent( false );
			break;
	case XWM_SETSHADER:
		LOGWHERE();
		if (eglMakeCurrent(gDisplay, gSurface, gSurface, gContext)
							== EGL_FALSE) {
						LOGE("Unable to eglMakeCurrent");
					}
					glViewport(0, 0, gWidth, gHeight);
					// Use the program object
					glUseProgram ( gProObject );

					if (eglMakeCurrent(gDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
							EGL_NO_CONTEXT) == EGL_FALSE) {
						LOGWHEREMSG("Release MakeCurrent Error");
					} LOGWHERE();

		break;
	
	case XWM_MAKEERRORPAGE:
	{
		mEnv->CallVoidMethod(mmVobject, gMVField.MakeErrorPageID);
		break;
	}
	case XWM_QUITAPP:
		//m_pWnd->PostMsg(WM_COMMAND,ID_APP_EXIT,0);
		break;

	}
	return XMainWnd::Handle(nOpera, pData1, pData2);
}

void XMobileView::StartFlash()
{
	mUserStatus = USER_NORMAL;
	int re = FlashContentMode();
//	if (re == XCNT_MODE_LOCAL) {
	//mUserStatus = FLASH_TYPE_GAME;
	bool b = IsFlashGame();
	if (b)
	{
		mUserStatus = FLASH_TYPE_GAME;
	} else {
		mUserStatus = FLASH_TYPE_MEDIA;
	}

	//JNIEnv* env = GetJNIEnv();
	LOGE("%s, %d-------->,%d ", __FUNCTION__, __LINE__, mUserStatus);

	mUserStatus = FLASH_TYPE_GAME;
	mEnv->CallVoidMethod(mmVobject, gMVField.StartFlashViewID, mUserStatus);

	return;
}

void XMobileView::OnFocus() {
	XWindow::OnFocus();
}

XBOOL XMobileView::Create(XRect &rect, XGraphics*pdraw, XPCWSTR strTitle,
		XPCTSTR strPath) {
	XString8 str;
	XRect rt(rect.left, rect.top, rect.right, rect.bottom);
	return XMainWnd::Create(rect, strTitle, pdraw, strPath);
}

void XMobileView::PeekMessage() {
	return;
}

void XMobileView::SetCursor(XU8 nType) {
	if (m_nCurType == nType)
		return;

}

void XMobileView::SetConfig(BROWNSINFO &info) {
	XU32 i;

	m_clientApply.ReleaseClient();

	m_bEnuHand = info.bEnuHand;
	m_clientApply.m_info.uPort = info.nPort;
	m_clientApply.m_info.nProtocol = info.nProtcol;
	m_clientApply.m_info.nRetries = info.nRetries;
	m_clientApply.m_info.nTimeout = info.nTimeout;
	m_clientApply.m_info.strAccepts.RemoveAll();
	for (i = 0; i < (XU32) info.strAccepts.GetSize(); i++) {
		m_clientApply.m_info.strAccepts.Add(info.strAccepts[i]);
	}
	m_clientApply.m_info.strGateway = info.strGameway;
	m_clientApply.m_info.strProfile = info.strProfile;
	m_clientApply.m_info.strUserAgent = info.strUserAgent;
	m_clientApply.m_info.nLanguages.Add(XEnumLan::CHINESE);
	m_clientApply.m_info.nLanguages.Add(XEnumLan::ENGLISH);
	XU32 nColors[XCW_BUTTON + 1];
	for (i = 0; i < XCW_BUTTON + 1; i++)
		nColors[i] = TRUECOLOR(info.nColor[i]);
	XWindow::SetColors(nColors);
}

void XMobileView::GetConfig(BROWNSINFO &info) {
	XU32 i;
	info.nPort = m_clientApply.m_info.uPort;
	info.nProtcol = m_clientApply.m_info.nProtocol;
	info.nRetries = m_clientApply.m_info.nRetries;
	info.nTimeout = m_clientApply.m_info.nTimeout;
	info.strAccepts.RemoveAll();
	info.bEnuHand = m_bEnuHand;
	for (i = 0; i < m_clientApply.m_info.strAccepts.GetSize(); i++) {
		info.strAccepts.Add((XPCTSTR) m_clientApply.m_info.strAccepts[i]);
	}
	info.strGameway = m_clientApply.m_info.strGateway;
	info.strProfile = m_clientApply.m_info.strProfile;
	info.strUserAgent = m_clientApply.m_info.strUserAgent;
	m_clientApply.m_info.nLanguages.Add(XEnumLan::CHINESE);
	m_clientApply.m_info.nLanguages.Add(XEnumLan::ENGLISH);
	for (i = 0; i <= XCW_BUTTON; i++)
		info.nColor[i] = TRUECOLOR(XWindow::GetSysColor(i));
}

void XMobileView::ViewSource() {

}

void XMobileView::ViewURL() {
	//	printf("viewURL? NO IMPLEMENTATION!\n");

}

void XMobileView::ViewTree() {
	//	printf("No view tree\n");
}

void XMobileView::Layerout() {
	GetActiveView()->Layerout();
}

void XMobileView::ViewTextVars() {
	XDomView*pView = GetActiveView();
	if (pView == XNULL)
		return;
	XSWFPlayer*player = pView->GetPlayer();
	if (player == XNULL)
		return;
	XSObjectArray list;
	if (!pView->GetTextEdits(list))
		return;
	XString8 strTxt = "名字\t路径\t内容\t对象\t\r\n";

	int i, nc = list.GetSize();
	for (i = 0; i < nc; i++) {
		SObject*pObj = list[i];
		if (!pObj->character || pObj->character->type != editTextChar)
			continue;
		EditText*p = pObj->editText;
		if (!p->m_variable.IsNull()) //continue;
			strTxt += p->m_variable.strTxt;
		strTxt += "\t";
		XXVar var;
		//char*strPath=
		player->GetTargetPath(pObj->thread->rootObject, var);
		if (var.IsString()) //strPath)
		{
			XString8 strT = var.strTxt; //strPath;
			strT.ConvertToGB();
			strTxt += strT;
		}

		strTxt += "\t";
		//		FreeStr(strPath);
		//char*str=p->GetBuffer();
		XXVar str;
		p->GetBuffer(str);
		//if(str)
		{
			XString8 strT = str.strTxt;
			strT.ConvertToGB();
			strTxt += strT;
		}
		strTxt += "\t";
		if (!pObj->name.IsNull()) {
			XString8 strT = pObj->name.strTxt;
			strT.ConvertToGB();
			strTxt += strT;
		}
		//		FreeStr(str);
		strTxt += "\r\n";
	}

}

void XMobileView::ThumbPoint(XPoint&pt) {
	if (!m_bEnuHand)
		return;
	int rx = rand() % 20;
	int ry = rand() % 20;
	pt.x += rx - 10;
	pt.y += rx - 10;
}

void XMobileView::QuitView() {
	// TODO: 释放资源
	PostMsg(XWM_QUITAPP, 0, (XU32) this, XMS_TOPMOST);
}

void XMobileView::XEdit_SetText(jstring str) {
	XWCHAR *xwstr = js2xw(str);
	PostMsg(XWM_FINISHEDEDIT, (XU32) xwstr, 0, XMS_NORMAL);
	return;
}

MARKLIST* XMobileView::GetHistory() {
	return &m_clientApply.m_lNearest;
}

MARKLIST* XMobileView::GetConfigBookmark() {
	return &(m_clientApply.m_marks[0]->lMarks);
}

MARKLIST* XMobileView::GetCustomBookmark() {
	return &(m_clientApply.m_marks[1]->lMarks);
}

void XMobileView::UpdateCustomBookmark(int* ar, int arSize) {
	MARKLIST l;
	MARKLIST& old_l = m_clientApply.m_marks[1]->lMarks;
	old_l = l;
}

bool XMobileView::ConnectUrl(const char* url) {
	XDomView* view = GetActiveView();
	CONNECTREQ req;
	req.bForward = true;
	req.nMethod = XEnumMethod::GET;
	req.URL.SetURL(url);
	LOGWHEREMSG( url );
	return view->Connect(req, true);
}

bool XMobileView::SetDeviceID(const char *chDevice) {
	XDomView* view = GetActiveView();
	XClientApply* apply = view->m_pClientApply;
	XString8 strDevice = chDevice;
	apply->SetToken(strDevice);
	return true;
}

bool XMobileView::CanBack() const {
	XDomView* view = m_views[0];
	if (!view->m_bBackList)
		return false;
	if (view->m_pFlashView)
		return false;
	return view->m_reqList.GetSize() > 0;
}

bool XMobileView::CanForward() const {
	XDomView* view = m_views[0];
	if (!view->m_bBackList)
		return false;
	if (view->m_pFlashView)
		return false;
	return view->m_forList.GetSize() > 0;
}

bool XMobileView::IsFlashStatus() const {
	XDomView* view = m_views[0];
	return view->m_pFlashView != 0;
}

void XMobileView::DomBack() {
	XDomView* view = GetActiveView();
	view->Back();
}

void XMobileView::DomForward() {
	XDomView* view = GetActiveView();
	view->Forward();
}

void XMobileView::AddCurrentBookmark() {
	XDomView* view = GetActiveView();
	// add later.
	XPCWSTR strTitle = GetTitle(this);
	if (strTitle) {
		view->m_pClientApply->AddMarker(XNULL, strTitle,
				view->m_req.URL.m_strURL, 0, XFALSE);
	}
}

int XMobileView::ClearCache() {
	XCatch c;
	c.Create(m_clientApply.m_strWorkPath);
	bool b = c.Clear(true);
	return b;
}

void XMobileView::CleanHistory() {
	XDomView* view = GetActiveView();
	XClientApply* apply = view->m_pClientApply;
	apply->RemoveAllClick();
}

void XMobileView::SaveConfig() {
	XDomView* view = GetActiveView();
	XClientApply* apply = view->m_pClientApply;
	if (apply->m_marks.GetSize() > 0)
		apply->SaveConfig();
}

bool XMobileView::IsFlashGame() {
//	XDomView* domView = GetActiveView();
//	if (!domView->m_ptLock.Lock(XLONGTIME))
//		return false;
//	bool bre = false;
//	XFlashView* flashView = domView->m_pFlashView;
//	if (flashView == NULL) {
//		LOGWHERE();
//		domView->m_ptLock.UnLock();
//		return false;
//	}
//
//	XSWFPlayer* player = flashView->GetPlayer();
//	XConfig* config = player->GetConfig();
//	bre = config->m_nContentType == XConfig::GAME;
//	domView->m_ptLock.UnLock();
	return true;

}

bool XMobileView::HasFlashACC() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	bre = config->m_ACCMaps.GetSize() > 0;
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::HasFlashButton() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}

	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	bre = config->m_buttonMaps.GetSize() > 0;
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::CanFlashSave() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	bre = flashView->m_flashSaveReady;
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::CanFlashSize() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	bre = flashView->IsEnableZoom();
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::IsUpScore() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	bre = flashView->m_topScoreAvailable;
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::IsFlashPause() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	bre = flashView->m_bPause;
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::IsFlashAccEnable() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	bre = config->m_bACCMap;
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::IsFlashKeyBoardEnable() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	bool bre = false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	bre = config->m_bButtons;
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::IsCurrentHomePage() {
	XDomView* domView = GetActiveView();

	if (domView->m_bHome == XTRUE) {
		return true;
	} else {
		return false;
	}

}

void XMobileView::SaveConfirmOK() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}
	if (flashView->m_pSave) {
		flashView->PostMsg(XWM_COMMAND, XIDOK, (XU32) this, XMS_THIS);
	}
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::FlashSave() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_SAVE, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
}

void XMobileView::FlashSaveCancel() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}
	if (flashView->m_pSave) {
		flashView->m_pSave->PostMsg(XWM_COMMAND, XIDCANCEL, (XU32) this,
				XMS_THIS);
	}
	//flashView->PostMsg(XWM_COMMAND, XIDSTOP, 0, XMS_THIS);
	FlashResume();
	domView->m_ptLock.UnLock();
	return;
}
void XMobileView::CancelFlashSave() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	if (flashView->m_pSave) {
		flashView->m_pSave->PostMsg(XWM_COMMAND, XIDCANCEL, (XU32) this,
				XMS_THIS);
	}
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::FlashChangeSize() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMMODE, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::UpScore() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		return;
	}
	flashView->PushScore();
	domView->m_ptLock.UnLock();
}

float XMobileView::GetCurrentScore() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return 0;
	float fre = 0;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return 0;
	}
	fre = flashView->getCurrentScore();
	domView->m_ptLock.UnLock();
	return fre;
}

void XMobileView::SwitchPause() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	flashView->m_bPause = !flashView->m_bPause;
	flashView->AddCommand(
			flashView->m_bPause ? XFlashView::XPAUSE : XFlashView::XRESUME, 0,
			0);

	flashView->Invalidate();
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::FlashPause() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	if (flashView->m_bPause) {
		domView->m_ptLock.UnLock();
		return;
	}
	SwitchPause();
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::SwitchHelp() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	flashView->Pause();
	domView->m_ptLock.UnLock();
}

void XMobileView::FlashResume() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	if (!flashView->m_bPause) {
		domView->m_ptLock.UnLock();
		return;
	}
	SwitchPause();
	domView->m_ptLock.UnLock();
}

void XMobileView::FlashRestart() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

//	if (flashView) {
//		flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_RESTART, 0, XMS_THIS);
//	}
	domView->m_ptLock.UnLock();
}

void XMobileView::SwitchAcc() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	config->m_bACCMap = !config->m_bACCMap;
	domView->m_ptLock.UnLock();
}

void XMobileView::SwitchButton() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* config = player->GetConfig();
	config->m_bButtons = !config->m_bButtons;

	flashView->Invalidate();
	domView->m_ptLock.UnLock();
}

void XMobileView::Refresh() {
	PostMsg(XWM_REFRESH, 0, (XU32) this, XMS_THIS);
}
void XMobileView::Exit_setShader() {
	PostMsg(XWM_SETSHADER, 0, (XU32) this, XMS_TOPMOST);

}
bool XMobileView::ExitFlash() {
	SetTopPos(0);
	mUserStatus = USER_NORMAL;
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}

	domView->AddCommand(XWM_COMMAND, XCMD_DOM_VIEW, 0);

	domView->m_ptLock.UnLock();
	//	if( !domView->m_bBackList || domView->m_reqList.GetSize() == 0 )
	//	{
	//		CONNECTREQ req;
	//		req.bForward=true;
	//		req.nMethod=XEnumMethod::GET;
	//		req.URL.SetURL("http://m.jqbar.com/product/");
	//		domView->Connect(req, true);
	//		domView->m_req.nMethod=HOMEPAGE;
	//		return;
	//	}
	return true;
}

const char* XMobileView::GetFlashLocalPath() {
	XDomView* domView = GetActiveView();

	return domView->m_pClientApply->m_strLocalPath.GetData();
}

int XMobileView::FlashContentMode() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return -1;
	int ire = -1;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return -1;
	}
	//ire = flashView->m_nCntMode;
	domView->m_ptLock.UnLock();
	return 1;
}

void XMobileView::FlashZoomInOrOut(int inOut) {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	if (flashView) {
		switch(inOut){
		case 0://缩小
			flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMIN, 0,
							XMS_THIS);
			break;
		case 1://放大
			flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMOUT, 0,
							XMS_THIS);
			break;
		case 2://start drag
			flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_STARTDRAG, 0,
							XMS_THIS);
			break;
		case 3://draging
			flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_DRAGING, 0,
							XMS_THIS);
			break;
		case 4://end drag
			flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ENDDRAG, 0,
							XMS_THIS);

			break;
		}

	}
	domView->m_ptLock.UnLock();
}

void XMobileView::FlashZoomInOut(int zoom) {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

//	if (flashView) {
//		flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMINOUT, zoom + 100,
//				XMS_THIS);
//	}
	domView->m_ptLock.UnLock();
}

int XMobileView::FlashLockMode() {
	bool bre = false;
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return 0;
	}
	bre = flashView->m_player->m_nLockMode == XSWFPlayer::LK_SELECT;
	domView->m_ptLock.UnLock();
	return bre;

}

void XMobileView::SetFlashLockMode(bool bLock) {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}
	if (!bLock)
		flashView->m_player->m_nLockMode = XSWFPlayer::LK_NONE;
	else
		flashView->m_player->m_nLockMode = XSWFPlayer::LK_SELECT;
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::FlashLockZoomInOut() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

//	if (flashView)
//		flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMINOUT, XTOOLBAR_MV_DOWN,
//				XMS_THIS);
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::FlashUnLockZoomInOut() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

//	if (flashView)
//		flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMINOUT, XTOOLBAR_MV_UP,
//				XMS_THIS);
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::SetFlashZoomMode(int mode) {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}
	XSWFPlayer* player = flashView->GetPlayer();
	XConfig* pConfig = player->GetConfig();
	pConfig->m_nZoomMode = mode;
	flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_ZOOMMODE, 0, XMS_THIS);
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::FlashPointZoom() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

//	if (flashView) {
//		flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_POINTZOOM, 0, XMS_THIS);
//	}

	domView->m_ptLock.UnLock();
	return;
}

int XMobileView::GetZoomPecent() {
	int ipecent = 0;
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return ipecent;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return ipecent;
	}
	if (flashView)
		ipecent = flashView->GetZoomPecent();
	domView->m_ptLock.UnLock();
	return ipecent;
}

void XMobileView::CheckUpgrade() {
	//	XClientApply clientApp;
	//	clientApp.Create();
	//	XGraphics graphics;
	//	graphics.Create( mScreenWidth, mScreenHight, 32 );
	//	XDomView view(&graphics, &clientApp );
	XDomView* domView = GetActiveView();
	CONNECTREQ req;
	req.bForward = true;
	req.nMethod = XEnumMethod::GET;
	req.URL.SetURL("http://10.0.0.1:8081");
	domView->Connect(req, true);
	return;
}

void XMobileView::ClosePopView() {
	XDomView* domView = GetActiveView();
	if (domView != NULL) {
		PostMsg(XWM_COMMAND, XCMD_CLOSE_VIEWEX, (XU32) this, XMS_PARENT);
	}
	return;
}

void XMobileView::OpenPayWindow() {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}

	if (flashView) {
		flashView->PostMsg(XWM_COMMAND, XCMD_FLASH_PAY, 0, XMS_THIS);
	}

	domView->m_ptLock.UnLock();
	return;
}

bool XMobileView::CanPayWindow() {
	bool bre = false;
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
		//LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	bre = flashView->CanPay();
	domView->m_ptLock.UnLock();
	return bre;
}

bool XMobileView::IsPause() {
	bool bre = false;
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return false;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
//		LOGWHERE();
		domView->m_ptLock.UnLock();
		return false;
	}
	bre = flashView->IsPause();
	domView->m_ptLock.UnLock();
	return bre;
}

void XMobileView::SetSilence(bool bsilence) {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	XFlashView* flashView = domView->m_pFlashView;
	if (flashView == NULL) {
//		LOGWHERE();
		domView->m_ptLock.UnLock();
		return;
	}
	flashView->GetPlayer()->theSoundMix.SetSilence(bsilence);
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::OpenFeedbackView() {
	XDomView* view = GetActiveView();
	if (view == NULL) {
		LOGWHERE();
		return;
	}
	CONNECTREQ req;
	req.bForward = true;
	req.nMethod = XEnumMethod::GET;
	req.URL.SetURL("http://m.jqbar.com/feedback/");
	view->Connect(req, true);
}

void XMobileView::SetViewRegion(int left, int top, int right, int bottom) {
	XDomView* domView = GetActiveView();
	if (!domView->m_ptLock.Lock(XLONGTIME))
		return;
	domView->SetViewRegion(left, top, right, bottom);
	domView->m_ptLock.UnLock();
	return;
}

void XMobileView::MouseDown(XPoint&pt, int iPos) {
	if (mbSlip) {
		return;
	}
	mbMouseDown = true;
	if (mUserStatus == USER_NORMAL) {
		//LOGWHERE();
		//m_drawSlip.Create( mScreenWidth * 3 + 40, mScreenHight, 32 );
		mbSlip = false;
		mbForward = false;
		mbBack = false;
		mISlipLen = 0;
		mLeftLen = 0;

		mDownPt.x = pt.x;
		mDownPt.y = pt.y;

		XDomView* view = GetActiveView();
		if (!view->CanMoveScreen()) {
			//LOGWHERE();
			int ilen = mScreenWidth * mScreenHight * 4;
			if (mpSlipBmpData == NULL)
				mpSlipBmpData = new unsigned char[ilen];
			memcpy(mpSlipBmpData, m_draw.GetXImage()->GetImgData(), ilen);
			//mSlipBmp->setPixels( mpSlipBmpData);
		}

		//		XPoint dst( mScreenWidth + 20, 0 );
		//		XRect rst( 0, 0, mScreenWidth, mScreenHight );
		//		m_drawSlip.Copy( m_draw, dst, rst );

	}
	XMainWnd::MouseDown(pt, iPos);
//	FlashZoomInOrOut(2);
	return;
}

void XMobileView::MouseMove(XPoint&pt, int iPos)
{
	if (mbSlip && !mbMouseDown)
		return;
	mISlipLen = pt.x - mDownPt.x;
	int idy = pt.y - mDownPt.y;
	int dxdy = abs(mISlipLen) - abs(idy);
	//LOGE("%s, %d, dx:%d, dy:%d, dxdy:%d", __FILE__, __LINE__, mISlipLen, idy, dxdy );
	if (mUserStatus == USER_NORMAL && mbMouseDown && !mbSlip && dxdy > 0) {
		XDomView* view = GetActiveView();
		if (view->CanMoveScreen()) {
			//LOGWHERE();
			mbSlip = false;
		} else {
			if (mISlipLen > 20) {
				mbSlip = true;
				//LOGWHERE();
				if (CanBack()) {
					DomBack();
					mbBack = true;
					Paint(m_draw);
					//mMainBmp->setPixels( m_draw.GetXImage()->GetImgData());
					//XPoint dst( 0, 0 );
					//XRect rst( 0, 0, mScreenWidth, mScreenHight );
					//m_drawSlip.Copy( m_draw, dst, rst );
				}
				view->OffXChangeVal();
			} else if (mISlipLen < -20) {
				mbSlip = true;
				//LOGWHERE();
				if (CanForward()) {
					OnDomForward();
					mbForward = true;
					Paint(m_draw);
					//mMainBmp->setPixels( m_draw.GetXImage()->GetImgData());
					//XPoint dst( mScreenWidth * 2 + 40, 0 );
					//XRect rst( 0, 0, mScreenWidth, mScreenHight );
					//m_drawSlip.Copy( m_draw, dst, rst );
				}
				view->OffXChangeVal();
			} // end of mISlipLen < -20
		}
	}
	if (!mbSlip)
		XMainWnd::MouseMove(pt, iPos);
//	FlashZoomInOrOut(3);
	return;
}

void XMobileView::MouseUp(XPoint&pt, int iPos) {
	mbMouseDown = false;
	if (mbSlip) {
		XMainWnd::MouseUp(mDownPt, iPos);
	} else {
		XMainWnd::MouseUp(pt, iPos);
	}

	if (mUserStatus == USER_NORMAL && mbSlip) {
		if (abs(mISlipLen) <= 20 && (mbBack || mbForward)) {
			if (mbBack)
				OnDomForward();
			if (mbForward)
				DomBack();
			mbBack = false;
			mbForward = false;
			mbSlip = false;
			mISlipLen = 0;
			mLeftLen = 0;
			//delete []mpSlipBmpData;
			//mpSlipBmpData = NULL;
		} else if (!mbForward && !mbBack) {
			mbSlip = false;
			mISlipLen = 0;
			mLeftLen = 0;
			mEnv->CallVoidMethod(mmVobject, gMVField.postInvalidateID); // 调用java刷新函数
			//delete []mpSlipBmpData;
			//mpSlipBmpData = NULL;
		} else if (mISlipLen > 20) {
			mLeftLen = mScreenWidth - mISlipLen;
		} else if (mISlipLen < -20) {
			mLeftLen = mScreenWidth + mISlipLen;
		}
	}
//	FlashZoomInOrOut(4);
	return;
}

void XMobileView::TimeTick() {
	static int icount = 6;
	if (mbSlip && !mbMouseDown && (mbBack || mbForward)) {
		int slippara[] = { 1, 1, 2, 3, 5, 8 };
		if (mISlipLen > 20) {
			int ilen = slippara[icount - 1] * mLeftLen / 20;
			mISlipLen += ilen;
			mEnv->CallVoidMethod(mmVobject, gMVField.postInvalidateID); // 调用java刷新函数
			//LOGWHERE();
		} else if (mISlipLen < -20) {
			int ilen = slippara[icount - 1] * mLeftLen / 20;
			mISlipLen -= ilen;
			mEnv->CallVoidMethod(mmVobject, gMVField.postInvalidateID); // 调用java刷新函数
			//LOGWHERE();
		}
		//LOGWHEREVAL( mISlipLen );
		if (icount-- <= 1) {
			icount = 6;
			mbSlip = false;
			mbBack = false;
			mbForward = false;
			mISlipLen = 0;
			mLeftLen = 0;
			//delete []mpSlipBmpData;
			//mpSlipBmpData = NULL;
		}
	}
	//LOGWHERE();
	XMainWnd::TimeTick();
	return;
}

void XMobileView::SetTopPos(int top) {
	mTopPos = top;
	XRect rect(0, mTopPos, mScreenWidth, mScreenHight);
	MoveWindow(rect);
}
void XMobileView::SetBottomPos(int bottom) {
	mBottomPos = bottom;
	XRect rect(0, 0, mScreenWidth, bottom);
	MoveWindow(rect);
}

// zhengjl 2012.03
char* XMobileView::encUIDStr(char *str) {
	XDomView* view = GetActiveView();
	XClientApply* apply = view->m_pClientApply;

	apply->Enc91UID(strUID);
	return strUID.GetData();
}
char* XMobileView::getAppUA() {
	XDomView* domView = GetActiveView();
	XClientApply* apply = domView->m_pClientApply;
	return apply->m_info.strUserAgent.GetData();

}
char* XMobileView::getAppHeaders() {
	XDomView* domView = GetActiveView();
	XClientApply* apply = domView->m_pClientApply;
	//XString8Array  headerArray=apply->m_info.strHeaders;
	//	 char* headers;
	//	 XString8 header;
	//	 XString8 strTmp;
	header.Empty();
	int ilen = apply->m_info.strHeaders.GetSize();
	LOGWHEREVAL(ilen);
	int m = 0;
	for (int i = 0; i < ilen; i++) {
		//		LOGWHEREMSG(apply->m_info.strHeaders[i].GetData());
		//		strTmp = apply->m_info.strHeaders[i].GetData();
		//		header += strTmp;
		//		header += '@';
		m += 1;
		header += apply->m_info.strHeaders[i].GetData();
		if (m == 1) {
			header += '#';
		} else if (m == 2) {
			header += '$';
			m = 0;
		}
		//		LOGWHEREMSG(header.GetData());
	}
	//	 LOGWHERE();
	//	 return header.GetData();
	return header.GetData();
}

// MobileView.h: interface for the CMobileView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MOBILEVIEW_H__
#define __MOBILEVIEW_H__

#include "GGDom.h"
#include "XDialog.h"
#include "Define.h"
//#include "SkCanvas.h"

class XMobileView : public XMainWnd
{
public:
	XMobileView();
	XMobileView(JNIEnv* env, jobject mvobj );
	virtual ~XMobileView();
	void Release();

public:
//	void OnPaint( void * canvas );
	void CreateView(int width, int height, char *pcid, char *pUA, char *pMac );
	void SetBmpData( unsigned char *out );
	void SetScreenWH( int width, int height );
	void freeParameter();
	void ThumbPoint(XPoint&pt);

	void Layerout();
	void ViewTree();
	void ViewURL();
	void ViewSource();
	void ViewTextVars();

	void GetConfig(BROWNSINFO&info);
	void SetConfig(BROWNSINFO&info);
	XBOOL Create(XRect&rect,XGraphics*pdraw,  XPCWSTR strTitle,XPCTSTR strPath);
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);

	void OnFocus();

	void setEnv(JNIEnv* env, jobject thiz);
	void XEdit_SetText( jstring str );

public:
	XBOOL OnDomQuit();
	XBOOL OnDomStop();
	XBOOL OnDomHome();//返回主页
	XBOOL OnDomBack();
	XBOOL OnDomForward();
	XBOOL OnDomAddbook();
	XBOOL OnDomUpdate();

public:
	MARKLIST* GetHistory();
	MARKLIST* GetConfigBookmark();
	MARKLIST* GetCustomBookmark();
	void UpdateCustomBookmark(int* ar, int arSize);
	bool ConnectUrl(const char* url);
	bool SetDeviceID( const char *chDevice );

	void QuitView();

    // 是否是FLASH状态
	bool IsFlashStatus() const;
    // 是否可以后退
	bool CanBack() const;
    // 是否可以前进
	bool CanForward() const;
    // 后退
	void DomBack();
    // 前进
	void DomForward();
    // 将当前页面添加到书签中
	void AddCurrentBookmark();
	// 获取当前分数
	float GetCurrentScore();

	int ClearCache();
	void CleanHistory();

	// 是否为游戏，用来判断是否需要显示播放控制条
	bool IsFlashGame();
    // 是否支持重力感应
	bool HasFlashACC();
    // 是否支持虚拟按键
	bool HasFlashButton();
    // 是否可以保存
	bool CanFlashSave();
    // 是否可以缩放
	bool CanFlashSize();
    // 是否可以传分
	bool IsUpScore();
    // 是否处于暂停状态
	bool IsFlashPause();
    // 重力感应是否打开
	bool IsFlashAccEnable();
    // 虚拟按键是否打开
	bool IsFlashKeyBoardEnable();
	// 当前页面是否是本地主页
	bool IsCurrentHomePage();

	void SaveConfirmOK();
    // 取消Flash保存
	void CancelFlashSave();
    // Flash保存
	void FlashSave();

	//取消保存flash
	void FlashSaveCancel();
    // Flash缩放
	void FlashChangeSize();
    // 上传积分
	void UpScore();
    // 暂停/继续
	void SwitchPause();
    // 重力感应 开/关
	void SwitchAcc();
	//重力感应接口
	//void SenseAcc();
    // 虚拟按键 开/关
	void SwitchButton();

	void Refresh();

	void Exit_setShader();

	bool ExitFlash();
	// 帮助 开/关
	void SwitchHelp();
	// Flash暂停
	void FlashPause();
    // Flash继续
	void FlashResume();

	//falsh 刷新
	void FlashRestart();

	void SaveConfig();

	const char* GetFlashLocalPath();

/////////////////////////////////////////////////
	int FlashContentMode();	// 当前flash是短片，游戏，还是网页游戏
	void FlashZoomInOrOut(int inOut);
	void FlashZoomInOut(int data);
	int FlashLockMode();
	void SetFlashLockMode(bool bLock);
	void FlashLockZoomInOut();
	void FlashUnLockZoomInOut();
	/*
	 0-
	 1-
	 2-FullScreen
	 */
	void SetFlashZoomMode(int mode);

	void FlashPointZoom();  //点击放大

	int GetZoomPecent();

	void CheckUpgrade( );  //检测升级

	void ClosePopView(); //关闭弹出视图

	void OpenPayWindow();//打开充值页面

	bool CanPayWindow();//是否能够打开充值页面

	bool IsPause();

	void SetSilence( bool bsilence );//设置是否静音，true为静音，false 为正常

	void OpenFeedbackView();

	void SetViewRegion(int left, int top, int right, int bottom );


	void MouseDown(XPoint&pt,int iPos);
	void MouseMove(XPoint&pt,int iPos);
	void MouseUp(XPoint&pt,int iPos);

	void TimeTick();

	void SetTopPos( int top );
	void SetBottomPos(int bottom);

	char *encUIDStr(char *str);	// zhengjl 2012.03
	//liupan 2012_05_16
	 char* getAppUA();
	 char* getAppHeaders();
private:
	void StartFlash();

protected:
	void SetCursor(XU8 nType);
	void PeekMessage();

	XBOOL m_bEnuHand;
	XU8 m_nCurType;
    XString8 strUID;	// zhengjl 2012.03

private:
//	SkBitmap *mMainBmp;
//	SkBitmap *mSlipBmp;
	unsigned char *mpSlipBmpData;
	int miDatalen;
private:
	XGraphics m_draw;
	volatile int miFlashExit;
	int changel;
	int changep;
	int mUserStatus;  //用户状态
	bool mbMouseDown;
	XPoint mDownPt;
	volatile bool mbSlip;//是否开始划屏
	bool mbForward;
	bool mbBack;
	int mISlipLen;//划动距离
	int mLeftLen; //剩下距离
	//bool mbLoadingPage;

	int mScreenWidth;
	int mScreenHight;

	int mTopPos;
	int mBottomPos;

	int miMode;

	int mAppStatus; //升级状态

	//XRect m_buffClip; //有效区域
	XString8 header;//liupan
private:
	//Java 相关函数，回调java函数
	JNIEnv* mEnv;
	jobject mmVobject;

};

#endif // !defined(AFX_MOBILEVIEW_H__A311BB1E_86B4_4D22_96D4_FBEEF651235D__INCLUDED_)

// MobileView.h: interface for the CMobileView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOBILEVIEW_H__A311BB1E_86B4_4D22_96D4_FBEEF651235D__INCLUDED_)
#define AFX_MOBILEVIEW_H__A311BB1E_86B4_4D22_96D4_FBEEF651235D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TRUECOLOR(CC) ((CC&0xff00ff00)|((CC>>16)&0xff)|((CC&0xff)<<16))

#include <vector>
#include "GGBase.h"
#include "XMainWnd.h"

class XDomViewExt;

typedef struct _BROWNSINFO
{
	XString8		strUserAgent; //User Argent
	XString8		strProfile;	  //Profile
	std::vector<XString8> strAccepts;
//	XStringArray	strAccepts;	  //Accepts
	XString8		strHome;	  // ◊“≥µƒ¡¨Ω”
	XString8		strHost;
	int			nProtcol;	  //¡¨Ω”–≠“È
	int			nRetries;	  //÷ÿ≥¢ ‘µƒ¥Œ ˝
	int			nPort;			   //∂Àø⁄
	XString8		strGameway;		   //Õ¯πÿ
	int			nTimeout;		   //≥¨ ± ±º‰
	unsigned int		nColor[6+1];//‰Ø¿¿∆˜—’…´
	////////////////////////////////////////////
	XString8		strMobile;
	XString8		strSelect;
//	XRect		sysRect[RC_WELL+1];
	int			nMode;
}BROWNSINFO,*PBROWNSINFO;

enum
{
    EHtml,
    EWebGame,
    ESingleGame,
    EVideo
};

class CMobileView : public XMainWnd  
{
public:
	void Layerout();
	void ViewTree();
	void ViewURL();
	void ViewSource();
	void GetConfig(BROWNSINFO&info);
	void SetConfig(BROWNSINFO&info);
	XBOOL Create(XRect&rect,XGraphics*p,void*pWnd,XPCWSTR strTitle,XPCTSTR strPath);
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	CMobileView();
	virtual ~CMobileView();
	
	
	MARKLIST* getHistory();
	MARKLIST* getConfigBookmark();
	MARKLIST* getCustomBookmark();
	void updateCustomBookmark(int* ar, int arSize);
	
	bool isEmpty() const;
	bool connectUrl(const char* url, bool isRes = false);
	
	void QuitView() {}
	
    // 获得页面内容模式
    int xgg_contentMode();
    // 获得flash页面当前显示尺寸
    bool xgg_getShowRect(int &left,
                         int &top,
                         int &width,
                         int &height);
    // 是否是FLASH状态
	bool isFlashStatus() const;
    // 是否可以后退
	bool canBack() const;
    // 是否可以前进
	bool canForward() const;
    // 后退
	void back();
    // 前进
	void forward();
    // 将当前页面添加到书签中
	void addCurrentBookmark();
	// 获取当前分数
	float getCurrentScore();
	
	int clearCache();
	void cleanHistory();
	
	void handleAppleMessage(XU32 pData1, XU32 pData2);
	// 是否为游戏，用来判断是否需要显示播放控制条
	bool isFlashGame();
//    // 是否支持重力感应
//	bool hasFlashACC();
//    // 是否支持虚拟按键
//	bool hasFlashButton();
    // 是否可以保存
	bool canFlashSave();
    // 是否可以缩放
	bool canFlashSize();
    // 是否可以传分
	bool isFlashScoreAvailable();
    // 是否处于暂停状态
	bool isFlashPause();
//    // 重力感应是否打开
//	bool isFlashAccEnable();
//    // 虚拟按键是否打开
//	bool isFlashKeyBoardEnable();
	// 当前页面是否是本地主页
	bool isCurrentHomePage();
	
	void saveConfirmOK();
    // 取消Flash保存
	void cancelFlashSave();
    // Flash保存
	void flashSave();
    // Flash缩放
	void flashChangeSize();
    // 上传积分
	void topScore();
    // 暂停/继续
	void switchPause();
//    // 重力感应 开/关
//	void switchAcc();
//    // 虚拟按键 开/关
//	void switchButton();
	void exitFlash();
	// 帮助 开/关
	void switchHelp();
	// Flash暂停
	void flashPause();
    // Flash继续
	void flashResume();
	
	void saveConfig();
	
	const char* getFlashLocalPath();
//	const char* getFlashSharedPath();
	
	int flashContentMode();	// 当前flash是短片，游戏，还是网页游戏
	void flashZoomInOut(int data);
	int flashLockMode();
	void setFlashLockMode(bool bLock);
	
	void flashLockZoomInOut();
	
	void flashUnLockZoomInOut();
	void flashRestart();
    static void finishFeedback();
    void closeSubView();
	void SetSilence(bool bsilence);
	/*
	 0-
	 1-
	 2-FullScreen
	 */
	void setFlashZoomMode(int mode);
	
	void saveDeviceToken(const char* deviceToken);
	
	void flashPointZoomIn();
	bool flashPay();
    bool OfferWallflashPay();
	bool canFlashPay();
	
	void repaint();
    void iosPidPost(const char* oriderid, const char* status, const char* description, const char* chargedata, const char* chargeUrl);
    bool enc91UID(const char* strCnt);
    const char* getEnc91UID();
    void setMAC(const char* strMAC);
    void setEip( const char *strEip );
    void setIDFV( const char *strIdfv );
    void setSysVer( const char *strSysVer );
    // 虚拟按键
    bool hasFlashButton();
    bool isFlashButtonEnable();
    void setFlashButtonEnable(bool aEnable);
    
    // 重力感应
    bool hasFlashAcc();
    bool isFlashAccEnable();
    void setFlashAccEnable(bool aEnable);
    
    // 获取应用程序信息
    // 获取ua
    const char* getAppUA();
    const char* getAppPushToken();
    // 获取headers
    const XString8Array* getAppHeaders();
	
    static CMobileView* s_getView();
protected:
	XBOOL OnPaint(XGraphics&g);
	XListBox m_list;

	XU8 m_nCurType;
	void SetCursor(XU8 nType);
	void PeekMessage();

	void* m_pWnd;

	// temp
	void* m_saveController;
	
	
	bool m_empty;
    
    XDomView* sendDomView;
    
    XString8 strUID;
};

#endif // !defined(AFX_MOBILEVIEW_H__A311BB1E_86B4_4D22_96D4_FBEEF651235D__INCLUDED_)

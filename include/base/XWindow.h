// XWindow.h: interface for the XWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_H__58D08909_D65A_4D5F_939D_71D8420B3146__INCLUDED_)
#define AFX_XWINDOW_H__58D08909_D65A_4D5F_939D_71D8420B3146__INCLUDED_

#include <stdlib.h>
#include "XData.h"	// Added by ClassView
#include "XMutex.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XGraphics.h"

//#ifdef __ANDROID__
//#else
//#define _NOCOMPOSERPAINT_
//#endif
#ifdef _NOCOMPOSERPAINT_
	#define MAX_MESSAGES 128*3
	//#define MAX_MESSAGES 64*3
#else
	#define MAX_MESSAGES 64*3
#endif

enum 
{
	  XWM_CLOSE,   //窗口关闭
	  XWM_CREATE,  //窗口创建
	  XWM_FOCUS,   //窗口得到焦点
	  XWM_BLUR,	   //窗口失去焦点
	  XWM_PAINT,   //绘制窗口
	  XWM_MOVE,	   //移动窗口
	  XWM_KEYDOWN, //按键
	  XWM_KEYUP,   //松开
	  XWM_CHAR,	   //字符输入
	  XWM_CLICK,   //鼠标点击
	  XWM_DBLCLICK,//鼠标双击
	  XWM_MOUSEDOWN,//按下
	  XWM_MOUSEUP,  //松开
	  XWM_MOUSEMOVE,//鼠标移动
	  XWM_MOUSEENTER,//进入窗口
	  XWM_MOUSEOUT,  //出窗口
	  XWM_REPAINT,    //重新绘制
	  //XWM_SETACTIVE,
	  XWM_SETPECENT,  //设置进度
	  XWM_SETTITLE,	  //设置标题
	  XWM_SETSTATUS,  //设置状态
	  XWM_COMMAND,	  //命令
	  XWM_DESTORY,	  //注销窗口
	  XWM_QUITAPP,	  //退出程序
	  XWM_ENDDIALOG,  //结束一个对话框
	  XWM_SOCKET,	  //通讯命令
	  XWM_RESULT,	  //结果
	  XWM_VSCROLL,	  //垂直滚动条命令
	  XWM_HSCROLL,	  //水平滚动条命令
      XWM_SETSHADER,  //退出游戏时，设置着色器
      XWM_REFRESH,//退出游戏后重新刷新界面
      XWM_UNBINGMAKECURRENT,//开启游戏时，通知上个线程解绑
	  XWM_STARTEDIT,
	  XWM_FINISHEDEDIT,
	  XWM_CHANGEEDIT,
	  XWM_LOWNETSPEED,
	  XWM_SHOWBARANDONSALE,
	  XWM_APP_STATUS,
	  XWM_NEW_APP,
	  XWM_OUTLOGIN,
	  XWM_PAYSDK_PID,
	  XWM_XSHOWHIDEMENU = 288,
      XWM_WXSENDTEXT,
	  XWM_WXSENDPHOTO,
	  XWM_WXSENDLINK,
	  XWM_WXSENDMUSIC,
	  XWM_WXSENDVEDIO,
	  XWM_WXSENDAPPMESSAGE,
	  XWM_WXSENDNOTGIFCONTENT,
	  XWM_WXSENDGIFCONTENT,
	  XWM_WXSENDFILECONTENT,
	  XWM_BWPAYURL,
	  XWM_YMSHOWOFFERWALL,
	  XWM_MAKEERRORPAGE,
	  XWM_UPDATEMSG
	};

#define XCMD_TABBAR 100
#define XCMD_VIEW	101
#define XCMD_QUIT 102
#define XCMD_ABOUT 103
#define XCMD_HELP  104
#define XCMD_VSCROLL 105
#define XCMD_HSCROLL 106
#define XCMD_TABLE	107
#define XCMD_CLOSE	108
#define XCMD_FLASH	109
#define XCMD_EDIT	200
#define XCMD_EDIT_CHANGE 201
#define XCMD_LIST	300
#define XCMD_LIST_CHANGE 302
#define XCMD_RADIOBOX  400
#define XCMD_CHECKBOX 500
#define XSO_CONNECT 0
#define XCMD_UPDATE_FLASHBAR 100
#define XCMD_UPDATE_WEBVIEW	 101
#define XCMD_UPDATE_DATASTART 102
#define XCMD_UPDATE_DATASET	103
#define XCMD_UPDATE_DATAEND 104
#define XCMD_USER  1000
//窗口状态字定义
enum {XES_VISIABLE=0x1,XES_FOCUS=0x2,XES_MOUSEON=0x4,XES_POPUP=0x8,XES_SETFOCUS=0x80,XES_TOPMOST=0x40,XES_DIALOG=0x20};
//窗口颜色索引定义
enum {XCW_BACK=0,//背景颜色
	  XCW_TEXT=1,//普通文字
	  XCW_LINK=2,//连接文字
	  XCW_SELECTBACK=3,//选中区域的背景
	  XCW_SELECTTEXT=4,//选中区域的文字
	  XCW_ACTIVE=5,//活动文字颜色
	  XCW_BUTTON=6};//按钮颜色

#define XW_WAITTIME 10
#define XW_REPTIME  0

#ifdef __ANDROID__
// 系统图标索引
#define XMAX_ICONS 8
enum
{
	XICON_LOADING,//图片载入中（动画）
	XICON_FAILED,//载入失败
	XICON_BLUEBLOCK,
	XICON_GREENBLOCK,
	XICON_BUTTON,
	XICON_GAMEHELP,
	XICON_NETTIP,
	XICON_PAGELOADING
//	XICON_LOADING,//图片载入中（动画）
//	XICON_FAILED,//载入失败
//	XICON_CONNECT,//连接
//	XICON_HOME,//主页
//	XICON_ANI, //数据动画
//	XICON_MAIN,
//	XICON_GREENBLOCK,
//	XICON_BLUEBLOCK,
//	XICON_GRAYBLOCK,
//	XICON_BUTTON,
};//主程序图标
#else
// 系统图标索引
#define XMAX_ICONS 13
enum
{
	XICON_LOADING,//图片载入中（动画）
	XICON_FAILED,//载入失败
	XICON_CONNECT,//连接
	XICON_HOME,//主页
	XICON_ANI, //数据动画
	XICON_MAIN,
	XICON_GREENBLOCK,
	XICON_BLUEBLOCK,
	XICON_GRAYBLOCK,
	XICON_BUTTON,
	XICON_GAMEHELP,
	XICON_NETTIP,
	XICON_PAGELOADING,
};//主程序图标
#endif
enum {XDW_LEFT,XDW_TOP,XDW_RIGHT,XDW_BOTTOM};
//消息传送类型
enum {XMS_NORMAL,//普通
	  XMS_PARENT,//传送给父窗口
	  XMS_THIS,//只传送给当前窗口及其子窗口
	  XMS_TOPMOST};	//传送到顶窗口
struct XKEYMSG
{
	int nKeyCode;
	int nCharCode;
	int nLocation;
	int bShiftKey;
	int bCtrlKey;
	int bAltKey;
};
struct XPOINTMSG
{
	enum {XMG_UP=0,XMG_DOWN=1,XMG_MOVE=2,XMG_ISDOWN=0x80};
	int nStatus[5];
	int nPosX[5];
	int nPosY[5];
};

////////////////////////////////////////////////////////////////////////////
// 所有窗口基础类，该类封装鼠标，键盘，菜单命令，消息处理，屏幕绘制等功能
class  _XEXT_CLASS XWindow
{
public:
	XWindow* GetPopupWindow();
	static XWindow*m_pMainWnd;
	static void SetMainWnd(XWindow*pWnd)
	{
		m_pMainWnd=pWnd;
	}
	static void Trace(const XU16*msg)
	{
		if(m_pMainWnd)
			m_pMainWnd->TraceText(msg);
	}
	virtual void TraceText(const XU16*msg){}
	// 获取窗口状态，
	XU8 GetStyle(){return m_nStyle;}
	// 设置窗口状态
	void SetStyle(XU8 s){m_nStyle=s;}
	// 检查窗口是否可用（Create后未被Destory）
	XBOOL IsValid(){return !m_rect.IsRectEmpty();}
	// 检查窗口内容是否需要重新绘制
	XBOOL IsUpdate(){return !m_clip.IsRectEmpty();}
public:
	// 获取系统图标图片， 索引由XICON_定义
	static XImage* GetIcon(XU8 id)
	{
		return id<XMAX_ICONS?&m_icons[id]:XNULL;
	}
	// 获取系统颜色值，索引由XCW_定义
	static XU32 GetSysColor(XU32 id)
	{
		if(id>XCW_BUTTON) return 0;
		return m_nColors[id];
	}
	// 设置全部系统颜色，初始化时调用
	static void SetColors(XU32 nColor[XCW_BUTTON+1])
	{
		memcpy(m_nColors,nColor,sizeof(m_nColors));
	}
	// 获取窗口工作区域（左上角为（0，0）的矩形区域）
	void GetClientRect(XRect&rect)
	{
		rect=m_rect;
		rect-=m_rect.TopLeft();
	}
	XU32 GetID()
	{	return m_nCmd;	}
	// 获取窗口区域（相对父窗口的位置及大小）
	XRect& GetWindowRect(){return m_rect;}
	// 获取需要重新绘制的窗口区域
	XRect& GetClipRect(){return m_clip;}
	// 系统空闲是调用，该虚函数可以用于后台处理或其它需要
	// 不定期触发的事件的触发，
	virtual void OnIdle();
	// 发送一个注销窗口命令到父窗口
	void PostDestory()
	{
		//Close();
		if(m_pParent!=XNULL)
			m_pParent->DestoryChild(this);
		
	}
	// 发送一个退出命令到系统
	void PostQuitMessage()
	{  PostMsg(XWM_QUITAPP,0,(XU32)this,XMS_TOPMOST); }
	//////////////////////////////////////////////////////////////////////
	// 发送一条命令到命令队列
	// nOpera:命令字，决定命令类型
	// pData1:第一个参数，具体数据类型由发送方和处理方约定
	// pData2:第二个参数，具体数据类型由发送方和处理方约定
    // es:命令传送的方式(XMS_定义)
	XBOOL PostMsg(XU32 nOpera,XU32 pData1,XU32 pData2,XU8 es=XMS_NORMAL);
	////////////////////////////////////////////////////////////////////////
	// 失效窗口内一个矩形区域（该区域需要重新绘制）
	void InvalidateRect(const XRect&rect);
	// 失效全部窗口区域
	void Invalidate();
	// 关闭窗口
	void Close();
	////////////////////////////////////////////////////////////////////////
	// 创建一个矩形窗口
	// rect:相对父窗口的矩形区域
	// nStyle:窗口形态，由XES_定义
	// pParent:父窗口指针
	// nID:该窗口的唯一编号
	XBOOL Create(const XRect&rect,XU8 nStyle,XWindow*pParent,XU32 nID=0);
	// 绘制窗口
	XBOOL Paint(XGraphics&g);
	
	XWindow();
	virtual ~XWindow();
	////////////////////////////////////////////////////////////////////
	// 处理发送到该窗口的消息
	virtual XBOOL Handle(XU32 nOperator,XU32 nParam1,XU32 nParam2);
	// 获取该窗口的父窗口指针
	XWindow* GetParent(){return m_pParent;}
	// 将该窗口设置为输入焦点或非输入焦点
	void SetFocus(XBOOL bFocus);
	// 定时功能调用，没一个创建注册的窗口，该函数会被系统以10毫秒频率调用
	void TimeTick();
	/////////////////////////////////////////////////////////////////////
	// 输入指针（鼠标）移动
	void MouseMove(XPoint&pt,int iPos);
	//////////////////////////////////////////////
	// 当该窗口有字符输入时候调用
	XBOOL Char(XWCHAR c);
	// 按键时调用，流程和OnChar相同
	XBOOL KeyDown(XKEYMSG&m);
	// 按键放开时调用
	XBOOL KeyUp(XKEYMSG&m);	
	// 设置窗口位置和大小
	void MoveWindow(const XRect&rect);
	// 指针点击（按下松开一次）
	void Click(XPoint&pt,int iPos);
	// 指针双击（连续按下松开两次）
	void DblClick(XPoint&pt,int iPos);
	// 指针松开
	void MouseUp(XPoint&pt,int iPos);
	// 指针按下
	void MouseDown(XPoint&pt,int iPos);
	// 检测窗口是否可见
	XBOOL IsVisiable(){return m_nStyle&XES_VISIABLE;}
	// 检测窗口是否为输入焦点
	XBOOL IsFocus(){return m_nStyle&XES_FOCUS;}
	// 是否为弹出窗口
	virtual XBOOL IsPopup(){return m_nStyle&XES_POPUP;}
	// 显示或隐藏窗口
	// bShow:XTRUE 显示窗口
	//       XFALSE 隐藏窗口
	void  ShowWindow(XBOOL bShow)
	{		
		if(bShow) m_nStyle|=XES_VISIABLE;
		else      m_nStyle&=~XES_VISIABLE;
	}
	//  转换窗口坐标为屏幕坐标
	void ClientToScreen(XPoint&pt);
	// 转换屏幕坐标为窗口坐标
	void ScreenToClient(XPoint&pt);
	// 改变活动窗口
	void ChangeActive(XBOOL bNext=XTRUE);
	
protected:
	// 格式化剪裁区域
	void FormatClip(XRect&rect,XPoint&pt);
	// 获得正确鼠标事件处理窗口
	XWindow* MouseEvent(XPoint&pt);
	virtual void OnTimer(){}
	virtual void OnMove(XRect&){}
	virtual void OnClose(){m_rect.SetRectEmpty();}
	virtual void OnCreate(){}
	virtual XBOOL OnPaint(XGraphics&){return XTRUE;}
	virtual void OnFocus()
	{
		m_nStyle|=XES_SETFOCUS;
	}
	virtual void OnBlur()
	{
		m_nStyle&=~XES_SETFOCUS;
	}
	virtual void OnACC(int x,int y,int z){}
	virtual XBOOL OnKeyDown(XKEYMSG&m){return XFALSE;}
	virtual XBOOL OnKeyUp(XKEYMSG&m){return XFALSE;}
	virtual XBOOL OnChar(XWCHAR){return XFALSE;}
	virtual void OnClick(XPoint&pt,int iPos);
	virtual void OnDblClick(XPoint&,int iPos){}
	virtual void OnMouseDown(XPoint&,int iPos){}
	virtual void OnMouseUp(XPoint&,int iPos){}
	
	virtual void OnMouseMove(XPoint&,int iPos)
	{	SetCursor(0);	}
	virtual void OnMouseEnter(){}
	virtual void OnMouseOut(){}
 	//virtual void GetOffset(XPoint&pt){pt.x=0;pt.y=0;}
	virtual XBOOL OnCommand(XU32,XWindow*){return XFALSE;}
	virtual void PeekMessage()
	{
		
		if(m_pParent!=XNULL) 
			m_pParent->PeekMessage();
		OnIdle();
	}
	XRect m_rect,m_clip;
	XWindow*m_pParent,*m_pActive;
	XU8   m_nStyle,m_bFocus;
	XU32  m_nCmd;
	XVector<XWindow*> m_childs;
	XU32  m_messages[MAX_MESSAGES];
	volatile int	  m_nReadID,m_nSetID;
	//int	  m_nRead
	//XU32Array		  m_messages;
	XMutex            m_lock;
	virtual XBOOL DestoryChild(XWindow*pWnd);
	void SetActive(XWindow*pActive);
	static XWindow*   m_pCapture;
	static XWindow*	  m_pTopmost;
	static void SetCapture(XWindow*pWnd) {m_pCapture=pWnd;}
	static XWindow* GetCapture(){return m_pCapture;}
	static XU32		  m_nColors[XCW_BUTTON+1];
	static XImage	  m_icons[XMAX_ICONS];
	static XPCWSTR    m_strAppTitle;
public:
	void ACC(int x,int y,int z);
	static void DrawButton(XGraphics&g,const XRect&r,XU32 c,XU8 b);
	void ClearMessage();
	virtual void SetCursor(XU8){}
	static XU8		  m_nTextHeight;	
	static XU8		  m_nTextSize;
	void AddChild(XWindow*pWnd);
};

#define RSHIFTF(FV,BS) {XU32&vi=*((XU32*)&FV);\
						XU8   pow=(XU8)((vi>>23)-BS);\
						vi=(vi&0x807fffff)|(pow<<23);}
#define LSHIFTF(FV,BS) {XU32&vi=*((XU32*)&FV);\
						XU8   pow=(XU8)((vi>>23)+BS);\
						vi=(vi&0x807fffff)|(pow<<23);}
class XGlobal
{
public:
	static int GetScreenPalette(_XCOLOR colors[]);
	static int ScreenBitDepth();
	inline static XU32 Random(XU32 nMax)
	{
		if(nMax<=0) return 0;
		XU32 v=(XU32)::rand();
		v=v%nMax;
		return v;
	}
	inline static int RandInt()
	{
		return ::rand();
	}

	inline static float Random()
	{
	   float v=(float)rand();
	   v=v/RAND_MAX;
	   return v;
	}	
	inline static void Seed(int v)
	{
		srand(v);
	}
	inline static void Memset(void*pDst,int v,int nSize)
	{	memset(pDst,v,nSize);	}
	inline static void Memcpy(void*pDst,void*pSrt,int nSize)
	{	memcpy(pDst,pSrt,nSize);}
	inline static int Memcmp(void*pDst,void*pSrt,int nSize)
	{
		return memcmp(pDst,pSrt,nSize);
	}
	inline static void Memmove(void*pDst,void*pSrt,int nSize)
	{
		memmove(pDst,pSrt,nSize);
	}

};

#endif // !defined(AFX_XWINDOW_H__58D08909_D65A_4D5F_939D_71D8420B3146__INCLUDED_)

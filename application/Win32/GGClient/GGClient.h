// GGClient.h : main header file for the GGCLIENT application
//

#if !defined(AFX_GGCLIENT_H__73C6C050_34FE_44D4_BA22_B1AEDB3B8ECB__INCLUDED_)
#define AFX_GGCLIENT_H__73C6C050_34FE_44D4_BA22_B1AEDB3B8ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGGClientApp:
// See GGClient.cpp for the implementation of this class
//

/*enum {KEY_USERAGENT=0,KEY_PROFILE=1,KEY_ACCEPTS=2,
	  KEY_HOME=3,KEY_PROTOCOL=4,KEY_RETRIES=5,KEYIMEOUT=6,
	  KEY_PORT=7,KEY_GATEWAY=8,KEY_COLORS=9,KEY_MOBILE=10,KEY_RECT=11,KEY_HOST=12};
enum {COLOR_BACK=0,COLOREXT=1,COLOR_LINK=2,COLOR_SELECTBACK=3,COLOR_SELECTTEXT=4,COLOR_ACTIVE=5,COLOR_BUTTON=6};
const LPCTSTR _strProtocol[]={("HTTP"),("UDP"),("AUTO")};*/
/*const LPCTSTR _strKeys[]={("UserAgent"),("Profile"),("Accepts"),
				  ("Home"),("Protocol"),("Retries"),("Timeout"),
				  ("Port"),("Gateway"),("Colors"),("Mobile"),("Rect"),("Host")};*/


enum 
{
	RC_SYSTEM=0,
	RC_WINDOW=1,
	RC_CALL=2,
	RC_CLOSE=3,
	RC_LEFTKEY=4,
	RC_RIGHTKEY=5,
	RC_MENU=6,
	RC_CLEAR=7,
	XRC_LEFT=8,
	XRC_RIGHT=9,
	XRC_UP=10,
	XRC_DOWN=11,
	XRC_ENTER=12,
	RC_NUM0=13,
	RC_NUM1=14,
	RC_NUM2=15,
	RC_NUM3=16,
	RC_NUM4=17,RC_NUM5=18,RC_NUM6=19,RC_NUM7=20,
	RC_NUM8=21,RC_NUM9=22,RC_PLUS=23,RC_WELL=24,
	SET_MOBILE=25,SET_MODE=26,SET_SELECT=27,ENUHAND=28,ENUACC=29,FONTSIZE=30
};

#define TRUECOLOR(CC) ((CC&0xff00ff00)|((CC>>16)&0xff)|((CC&0xff)<<16))

typedef struct _BROWNSINFO
{
   CString		strUserAgent; //User Argent
   CString		strProfile;	  //Profile
   CStringArray	strAccepts;	  //Accepts
   CString		strHome;	  //首页的连接
   CString		strHost;
   CString		strMobileType;
   CString		strChannelID;
   CString		strClientID;
   BYTE			nProtcol;	  //连接协议
   BYTE			nRetries;	  //重尝试的次数
   WORD			nPort;			   //端口
   CString		strGameway;		   //网关
   BYTE			nTimeout;		   //超时时间
   COLORREF		nColor[XCW_BUTTON+1];//浏览器颜色
   ////////////////////////////////////////////
   XString8		strMobile;
   XString8		strSelect;
   CRect		sysRect[RC_WELL+1];
   BYTE			nMode;
   BYTE		    nFontSize;
   XBOOL		bEnuHand,bEnuACC;
}BROWNSINFO,*PBROWNSINFO;



class CGGClientApp : public CWinApp
{
public:
	CGGClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGGClientApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGGClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GGCLIENT_H__73C6C050_34FE_44D4_BA22_B1AEDB3B8ECB__INCLUDED_)

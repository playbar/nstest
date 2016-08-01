#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "GGType.h"
#include "XWindow.h"

#define TRUECOLOR(CC) ((CC&0xff00ff00)|((CC>>16)&0xff)|((CC&0xff)<<16))

#define GIF _T("gif")

#define TRUECOLOR(CC) ((CC&0xff00ff00)|((CC>>16)&0xff)|((CC&0xff)<<16))

typedef unsigned long  COLORREF;

//#define NELEM(x) (sizeof(x)/sizeof(*(x)))

#define MAX_PATH          260

enum
{
	RC_SYSTEM=0,RC_WINDOW=1,RC_CALL=2,RC_CLOSE=3,
	RC_LEFTKEY=4,RC_RIGHTKEY=5,RC_MENU=6,RC_CLEAR=7,
	RC_LEFT=8,RC_RIGHT=9,RC_UP=10,RC_DOWN=11,RC_ENTER=12,
	RC_NUM0=13,RC_NUM1=14,RC_NUM2=15,RC_NUM3=16,
	RC_NUM4=17,RC_NUM5=18,RC_NUM6=19,RC_NUM7=20,
	RC_NUM8=21,RC_NUM9=22,RC_PLUS=23,RC_WELL=24,
	SET_MOBILE=25,SET_MODE=26,SET_SELECT=27,
	ENUHAND=28,ENUACC=29,FONTSIZE=30
};

typedef struct _BROWNSINFO
{
   XString16		strUserAgent; //User Argent
   XString16		strProfile;	  //Profile
   XString16Array	strAccepts;	  //Accepts
   XString16		strHome;	  //首页的连接
   XString16		strHost;
   XU8			nProtcol;	  //连接协议
   XU8			nRetries;	  //重尝试的次数
   XU16			nPort;			   //端口
   XString16		strGameway;		   //网关
   XU8			nTimeout;		   //超时时间
   COLORREF		nColor[XCW_BUTTON+1];//浏览器颜色
   ////////////////////////////////////////////
   XString8		strMobile;
   XString8		strSelect;
   XRect		sysRect[RC_WELL+1];
   XU8			nMode;
   XU8		    nFontSize;
   XBOOL		bEnuHand,bEnuACC;
}BROWNSINFO, *PBROWNSINFO;

#define WORKPATH  "/sdcard/byone"

#define MOBILEMAIN 			"com/jqbar/MobileMain"
#define MOBILEVIEW 			"com/jqbar/MobileView"
#define MOBILEVIEWGLES 		"com/jqbar/MobileViewGLES"
#define MOBILEVIEWSURFACE 	"com/jqbar/MobileViewSurface"

//#define BWANDROIDUA "IOSiPad;02071001;IOS3.2;BWNBCS/2.9.20"
#define BWANDROIDUA "Android;03000000;Android2.2+;BWNBCSAND/3.0.00"
#define BWLIBLOW    "/data/data/com.jqbar/lib/libbw22.so"
#define BWLIBHEIGHT "/data/data/com.jqbar/lib/libbw23.so"

#endif

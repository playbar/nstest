// XToolBar.h: interface for the XToolBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTOOLBAR_H__2C5DAC40_0220_4D91_868A_DFC352B67141__INCLUDED_)
#define AFX_XTOOLBAR_H__2C5DAC40_0220_4D91_868A_DFC352B67141__INCLUDED_

#include "XData.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define XCMD_HIDE_TOOLBAR XCMD_USER+6000
#define XCMD_MOVE_TOOLBAR XCMD_USER+6001
#define XCMD_BACK_TOOLBAR XCMD_USER+6002

enum {XTOOLBAR_MV_MOVE,XTOOLBAR_MV_DOWN,XTOOLBAR_MV_UP};

#ifdef __ANDROID__
#else

#if defined(__TOUCHPAD__)||defined(__POINT__)

#define MAX_ITEMS 32
enum {TICON_MOVE,TICON_HIDE,
	  TICON_SAVE,TICON_ZOOM,
	  TICON_BOOKMARK,TICON_PAUSE,
	  TICON_CLOSE,TICON_ENUKEY,
	  TICON_ENUACC,TICON_FORWARD,
	  TICON_BACK,TICON_UPDATE,TICON_HOME,TICON_EXIT,TICON_SCORE,TICON_ZOOMIN,TICON_ZOOMOUT};

enum {XTS_NORMAL,XTS_DISABLE,XTS_ACTIVE,XTS_ACTIVE1,XTS_ACTIVE2};
enum {XTP_AUTO,XTP_LEFT,XTP_RIGHT};

#define TICON_SLIDER 0xfffffff0

class XToolBar:public XWindow  
{
public:
	
	struct XTOOLITEM
	{
		int iImage;
		int iStatus;
		int iSlider;
		int iPos;
		XU32 iCommand;
		XBOOL bChecked;
		XBOOL bHide;

	};
	XTOOLITEM m_items[MAX_ITEMS];
	int m_nItemCount,m_nItemLeft,m_nItemRight,m_nPageCount;
	int m_nOffset,m_nItemWidth,m_nShowBegin,m_nOffsetEnd;
public:
	XBOOL SetItemStatus(XU32 iCommand,int nStatus);
	XBOOL SetItemSlider(XU32 iCommand,int nSlider);
	
	XBOOL Create(XWindow*pParent,int y,int w,XBOOL bHide,XU32 nBackID);	
	XBOOL OnPaint(XGraphics&g);
	XToolBar();
	virtual ~XToolBar();
	
	/*void GetRect(XRECT&rect)
	{
		rect.left=0;
		rect.right=m_nWidth;
		rect.top=m_nOffsetY;
		rect.bottom=rect.top+m_nHeight;
	}*/
public:
	int m_nMvStatus;
	XBOOL SetItemPos(XU32 nCommand,int iPos);
	XBOOL CheckItem(XU32 nCommand,XBOOL bCheck);
	void OnMouseDown(XPoint&pt,int iPos);
	void OnMouseUp(XPoint&pt,int iPos);
	
	void OnMouseMove(XPoint&pt,int iPos);
	void OnMove(XRect&rect);
	XBOOL RemoveItem(XU32 iCommand);
	void RemoveAllItems();
	XBOOL AddItem(XU32 iCommand,int iImage,int iStatus=XTS_NORMAL,int iPos=XTP_AUTO,XBOOL bCheck=XFALSE);
	XBOOL FindItem(XU32 iCommand)
	{
		for(int i=0;i<m_nItemCount;i++)
		{
			if(m_items[i].iCommand==iCommand) return XTRUE;
		}
		return XFALSE;
	}

	void SetImage(XPCTSTR iconNormal,XPCTSTR iconGray,XPCTSTR iconActive,XPCTSTR iconCheck);
	XBOOL UpdateToolBar();
protected:
	void OnIdle();
	int GetActiveID(int x,int&subItem);
	int m_nActiveID,m_nSubID,m_bDown,m_nStep,m_bIsDown,m_nDownID;
	XImage m_normal,m_gray,m_active,m_check,m_bar;
	XImage* m_pBack;
};

#endif
#endif // endif define ANDROID
#endif // !defined(AFX_XTOOLBAR_H__2C5DAC40_0220_4D91_868A_DFC352B67141__INCLUDED_)

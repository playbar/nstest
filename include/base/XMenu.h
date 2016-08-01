// XMenu.h: interface for the XMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMENU_H__D3715F8F_174A_42CC_B5E2_CC55867A228F__INCLUDED_)
#define AFX_XMENU_H__D3715F8F_174A_42CC_B5E2_CC55867A228F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "GGType.h"	// Added by ClassView

enum {XMS_SEPARATOR=0x1,XMS_POPUP=0x2,XMS_CHECKED=0x4,XMS_GRAY=0x8};

typedef struct _XMENUDATA
{
   XU32		 nCommand;
   XU32		 nData;
   XPCWSTR   strTitle;
   XImage*	 pImage;
   XU8		 nIndex,nStyle;
}XMENUDATA,*XPMENUDATA;
typedef XVector<XPMENUDATA>MENUDATAS;

enum {XMPS_LEFT=0x1,XMPS_RIGHT=0x2,XMPS_VCENTER=0x3,XMPS_WLEFT=0x4,XMPS_WRIGHT=5,XMPS_WVCENTER=6,
	  XMPS_TOP=0x10,XMPS_BOTTOM=0x20,XMPS_HCENTER=0x30,XMPS_WTOP=0x40,XMPS_WBOTTOM=0x50,XMPS_WHCENTER=0x60};

#ifdef __ANDROID__
#else
class _XEXT_CLASS XMenu : public XWindow  
{
	//enum {XMS_LEFT=1,XMS_RIGHT=2,
		  
public:
	void AddSeparator();
	void AddMenu(XPMENUDATA pMenu)
	{	
		m_menus.Add(pMenu);
	}
	void RemoveMenus(){m_menus.RemoveAll();}
	XBOOL Create(XPoint&pt,XU8 nStyle,XWindow*pWnd,XMenu*pRoot=XNULL);
	XMenu();
	virtual ~XMenu();
	XU32  GetCount(){return m_menus.GetSize();}
protected:
//	XImage* m_pImage,*m_pSelectImage;
	void OnMouseUp(XPoint&pt,int iPos);
	XBOOL CloseSubMenu();
	XMenu* m_pSubMenu;
	XMenu* m_pRootMenu;
	void PopMenu();
	void MoveStep();
	void OnTimer();
	int HitTest(XPoint&pt);
	void Select(int d);
	void CloseMenu(XBOOL bClearAll);
	void OnMouseMove(XPoint&pt,int iPos);
	XBOOL OnKeyDown(XKEYMSG& c);
	void OnMouseDown(XPoint&pt,int iPos);
	XU8	 m_nSelect,m_nOffset;
	XBOOL OnPaint(XGraphics&g);
	MENUDATAS m_menus;
	XU8		  m_nCount,m_nImageWidth,m_nTick,m_nMove;
};
#endif

#endif // !defined(AFX_XMENU_H__D3715F8F_174A_42CC_B5E2_CC55867A228F__INCLUDED_)

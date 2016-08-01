// XTabControl.h: interface for the XTabControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTABCONTROL_H__D6B23ECF_516B_4E33_A7F8_84F52889F392__INCLUDED_)
#define AFX_XTABCONTROL_H__D6B23ECF_516B_4E33_A7F8_84F52889F392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "GGType.h"	// Added by ClassView
#include "XString.h"
#define XMAXTABS 8

class _XEXT_CLASS XTabControl : public XWindow  
{
public:
	struct TABLEINFO
	{
		XImage*   pImage;
		XString16 strTitle;
		XString16 strTitleLong;
		XU16      nWidth,nPosX;
		XU16	  nActive,nMinWidth;
		XU32	  nData;
	};
public:
	XPCWSTR GetTabTitle(XU8 nID);
	XBOOL Create(XRect&rect,XWindow*pWnd,XU32 nCmd=XCMD_TABLE);
	int  FindData(XU32 d)
	{
		for(XU32 i=0;i<m_tabs.GetSize();i++)
			if(m_tabs[i]->nData==d) return i;
		return -1;
	}
	XU32 GetTabData(XU8 nID);
	XBOOL ActiveStatus(XU8 nID);
	XBOOL SetTabTitle(XU8 nID,XPCWSTR strTitle);
	void SetActive(XU8 nID);
	XBOOL Remove(XU32 id);
	void  RemoveAll();
	XBOOL Insert(XPCWSTR strTitle,XImage*pImage,XU32 nData=0,XINT nPos=-1,
				 XBOOL bActive=XFALSE);

	XBOOL OnPaint(XGraphics&g);
	XTabControl();
	virtual ~XTabControl();
	XU8  GetActiveID(){return m_nActive;}
protected:
	void FormatTitle(XGraphics&g,XString16&strTitle);
	void OnTimer();
	XBOOL OnKeyDown(XKEYMSG&);
	void OnFocus();
	void OnBlur();
	void OnMouseDown(XPoint&pt,int iPos);
	void DrawTab(XGraphics&g,XU8 id,XU32 c,XU8 s);
	void SetOffset();
	void Layerout(XGraphics&g);
	XVector<TABLEINFO*> m_tabs;
	XU8					m_nActive,m_bLayerout,m_nOffset;
};

#endif // !defined(AFX_XTABCONTROL_H__D6B23ECF_516B_4E33_A7F8_84F52889F392__INCLUDED_)

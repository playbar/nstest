// XEdit.h: interface for the XEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XEDIT_H__F88954F5_FADE_4BC1_84BB_E2C9A27DD7EC__INCLUDED_)
#define AFX_XEDIT_H__F88954F5_FADE_4BC1_84BB_E2C9A27DD7EC__INCLUDED_

#include "XString.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "GGType.h"	// Added by ClassView

enum {XEDS_MULTILINE=0x80,XEDS_PASSWORD=0x40,XEDS_LCHAR=0x20,XEDS_UCHAR=0x10,
	  XEDS_NUMBER=0x08,XEDS_SIGN=0x04,XEDS_WCHAR=0x02,
	  XEDS_UPLIST=0x01,XEDS_ANY=0x3e};

class _XEXT_CLASS XEdit : public XWindow  
{

public:
	XBOOL IsPopup();
	void SetLikeText(XString8&strTxt);
	void SetUpList();
	void SetDownList();
	void GetClientRect(XRect&rect);
	int PositionToCur(XPoint&pt);
	XBOOL OnKeyDown(XKEYMSG& c);
	XBOOL OnPaint(XGraphics&g);
	XBOOL Create(XRect&rect,XWindow*pWnd,XGraphics*p,XU8 s=XEDS_ANY);
	XEdit();
	virtual ~XEdit();
	void SetEditStyle(XU8 s);
	XU8  GetEditStyle(){return m_nEditStyle;}
	void GetText(XString8&str){str=m_strText;}
	void SetText(XPCWSTR str);
	XPCWSTR GetText(){return m_strText;}
	void SetMaxLength(XU16 l){m_nMaxLength=l;}
	int  GetMaxLength(){return m_nMaxLength;}
protected:
	int GetCurLine();
	void InsertList();
	void GetListRect(XRect&rect);
	virtual void SetMatchList(XBOOL bEmpty=XFALSE);
	XString16Array m_list;
	void DrawLine(XGraphics&g,XPCWSTR strText,int y,int nOffset,int nLen);
	int PositionToCur(XPCWSTR pData,int l,int x);
	void SetOffset();
	void CurToPosition(XGraphics&g,XPoint&pt);
	
	XGraphics* m_pGraphics;
public:
	void OnDblClick(XPoint&pt,int iPos);
	void OnMouseUp(XPoint&pt,int iPos);
	void OnMouseDown(XPoint&pt,int iPos);
	void OnBlur();
	void OnFocus();
	void OnMouseMove(XPoint&pt,int iPos);
	XBOOL OnChar(XWCHAR c);
	
	void OnTimer();
//#ifdef _SYMBIAN
//	void OnGlobalEvent(XU32 data1,XU32 data2);
//#endif
protected:
	void PaintEdit(XGraphics&g);
	int  TickList(XU8&index);
	void PaintList(XGraphics&g);
	void PaintText(XGraphics&g);
	void PaintCursor(XGraphics&g);
	DOMPOSARRAY m_nFormats;
	XString16 m_strText;
#pragma pack(1)
	XU8 m_nEditStyle,m_nTicks,m_nMatchs,m_nMatchIndex,m_nMatchOffset,m_nMaxList;
	XU16 m_nCur,m_nSelectCount,m_nOffset,m_nDownCur;
	XU16 m_nMaxLength;
#pragma pack()
};

#endif // !defined(AFX_XEDIT_H__F88954F5_FADE_4BC1_84BB_E2C9A27DD7EC__INCLUDED_)

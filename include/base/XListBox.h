// XListBox.h: interface for the XListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLISTBOX_H__F322AD70_265C_4E69_81AB_E511E5BEA610__INCLUDED_)
#define AFX_XLISTBOX_H__F322AD70_265C_4E69_81AB_E511E5BEA610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XEdit.h"
#include "GGType.h"	// Added by ClassView

enum {XLS_LIST=0x01,XLS_DROPLIST=0x02,XLS_DROPDOWN=0x03};
      

class XListBox : public XEdit  
{
public:
	XBOOL IsPopup();
	void SetLines(XU8 nLines);
	XINT GetSelect(){return m_nListIndex;}
	void OnBlur();
	XBOOL Select(XU8 nIndex);
	void PaintDropList(XGraphics&g);
	void OnDblClick(XPoint&pt,int iPos);
	void OnMouseUp(XPoint&pt,int iPos);
	void OnMouseDown(XPoint&pt,int iPos);
	void OnTimer();
	void OnFocus();
	XBOOL OnChar(XWCHAR c);
	XBOOL OnKeyDown(XKEYMSG& c);
	void OnClick(XPoint&pt,int iPos);
	void OnMouseMove(XPoint&pt,int iPos);
	XBOOL AddString(XPCWSTR str);
	XBOOL Create(XRect&rect,XWindow*pWnd,XGraphics*g,XU8 nStyle=XLS_DROPLIST);
	XListBox();
	virtual ~XListBox();
	XU8 GetListStyle(){return m_nListStyle;}
	XU8 SetListStyle(XU8 s){XU8 os=m_nListStyle;m_nListStyle=s;return os;}
protected:
	
	void ClickDropList(XPoint&pt);
	void SetMatchList(XBOOL bEmpty=XFALSE);
	XU8  MatchList();
	XBOOL OnKeyDropList(XKEYMSG& c);
	void SetDropList(XBOOL bList);
	void OnMoveList(XPoint&pt);
	void PaintList(XGraphics&g);
	XBOOL OnPaint(XGraphics&g);
	XU8 m_nListStyle;
	XU8 m_nListIndex;
	XU8 m_bDropList;
};

#endif // !defined(AFX_XLISTBOX_H__F322AD70_265C_4E69_81AB_E511E5BEA610__INCLUDED_)

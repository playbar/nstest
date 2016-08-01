// XHomeView.h: interface for the XHomeView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHOMEVIEW_H__1BB0354B_6505_4DBF_A533_938C6E8593E1__INCLUDED_)
#define AFX_XHOMEVIEW_H__1BB0354B_6505_4DBF_A533_938C6E8593E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomView.h"
#include "XDom.h"	// Added by ClassView
class XDomInput;

class XHomeView : public XDomView  
{
#ifdef _SYMBIAN
	friend class CMenuItem;
#endif
public:
#ifdef __ANDROID__
#else
	void AppendMenu(XMenu&menu);
#endif
	XHomeView(XGraphics*pDraw,XClientApply*pa);
	virtual ~XHomeView();
	
protected:
	//XString8 m_strBootFile;
	XBOOL OnPaint(XGraphics&g);
	XINT CalcPecent()
	{
		if(m_pDom==&m_days) return 0;
		else if(m_pClient==XNULL) return 0;
		else if(m_pClient->Get_CallID()==(XU32)&m_days) return 0;
		return XDomView::CalcPecent();
	}
	XBOOL m_bLoadOK;
	XDom m_days;
	XBOOL HandleSocket(XU32 nCmd,XClient*pClient);
	XBOOL ProcSysURL(CONNECTREQ&req);
	
	void OnCreate();
	XU32 m_nSelectID;
//	XDomInput* m_pInput;
//	XString16 m_strInput;
	XEdit m_edit;
	void OnFocus();
	XBOOL OnCommand(XU32 nCmd,XWindow*pWnd);
	XBOOL Connect(CONNECTREQ&req,XBOOL bSet=XFALSE);
	void InitMarks(XString8&strHTML);
	void InitNearData(XString8&strHTML);
	void InitURLData(XString8&strHTML);
	void InitHotData(XString8&strHTML);
	void InitDayData(XString8&strHTML);
	void InitHomePage();
};

#endif // !defined(AFX_XHOMEVIEW_H__1BB0354B_6505_4DBF_A533_938C6E8593E1__INCLUDED_)

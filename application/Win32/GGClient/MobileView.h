// MobileView.h: interface for the CMobileView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOBILEVIEW_H__A311BB1E_86B4_4D22_96D4_FBEEF651235D__INCLUDED_)
#define AFX_MOBILEVIEW_H__A311BB1E_86B4_4D22_96D4_FBEEF651235D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TRUECOLOR(CC) ((CC&0xff00ff00)|((CC>>16)&0xff)|((CC&0xff)<<16))

class CMobileView : public XMainWnd  
{
public:
	XClientApply* GetClientApply(){return &m_clientApply;}
	DRAWCONTEXT* GetDraw(){return m_views[0]->GetDraw();}
	void ThumbPoint(XPoint&pt);
	void ViewTextVars();
	void Layerout();
	void ViewTree();
	void ViewURL();
	void ViewSource();
	void GetConfig(BROWNSINFO&info);
	void SetConfig(BROWNSINFO&info);
	XBOOL Create(CRect&rect,XGraphics*p,CWnd*pWnd,XPCWSTR strTitle,XPCTSTR strPath);
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	CMobileView();
	virtual void TraceText(const XU16*msg);
	virtual ~CMobileView();
	virtual void StartCall();
	virtual void ReleaseCall();
protected:
	void QuitView();
	XListBox m_list;
//	XEdit m_edit;
//	XGraphics m_draw;
	XBOOL m_bEnuHand;
	XU8 m_nCurType;
	void SetCursor(XU8 nType);
	void PeekMessage();
	CWnd*	   m_pWnd;
};

#endif // !defined(AFX_MOBILEVIEW_H__A311BB1E_86B4_4D22_96D4_FBEEF651235D__INCLUDED_)

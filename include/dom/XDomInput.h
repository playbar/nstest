// XDomInput.h: interface for the XDomInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMINPUT_H__B0194C3F_25DB_4D04_A941_22BEC7B324D9__INCLUDED_)
#define AFX_XDOMINPUT_H__B0194C3F_25DB_4D04_A941_22BEC7B324D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomInput : public XDomNode  
{
	XDEC_CREATE(XDomInput)
public:
	XU32 OnKeyDown(XWCHAR c,XEVENTDATA*pData);
	enum {TEXT,PASSWORD};
	enum {UPA_SIGN,LOA_SIGN,NUMBER,UPCHAR,LOCHAR,ANY};
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomInput(XU16 nID=XDOM_INPUT):XDomNode(nID)
	{
		m_pEdit=XNULL;
		m_bReturn=XFALSE;
//		m_edit.Create(XRect(0,0,100,20));
//		m_nType=ANY;
//		m_nCur=0;
//		m_nPosX=m_nPosY=0;
//		m_nOff=0;
//		m_nClick=0;
	}
	~XDomInput(){if(m_pEdit!=XNULL) delete m_pEdit;}
protected:
	XU8 m_bReturn;
	XString16 m_strTmp;
	XU32 ChangeMode(XEVENTDATA*pData);
	XEdit* m_pEdit;
	XU32 ReplaceString(XPCWSTR buf,XEVENTDATA*pData);
	XU32 InsertChar(XWCHAR c,XEVENTDATA*pData);
	XU32 SetRepaint(XEVENTDATA*pData,XRect&rect);
	XU32 OnLoad(XEVENTDATA*pData,XBOOL bFromEdit);
	XU32 OnTimer(XEVENTDATA*pData);
	void GetRoundRect(XRect&rect);
	XBOOL HitTest(XINT x,XINT y);
	XBOOL Paint(DRAWCONTEXT*pDraw);
	void GetSize(DRAWCONTEXT*pDraw,CELLDATA*pData,XSIZE&sz);
	void Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells);
public:
	void SetText(XPCTSTR strText);
//	XString16 m_strValue;
//	XU16 m_nPosX,m_nPosY,m_nWidth,m_nHeight;
//	XU16 m_nCur,m_nOff;
//	XU8 m_nType,m_nClick;
};

#endif // !defined(AFX_XDOMINPUT_H__B0194C3F_25DB_4D04_A941_22BEC7B324D9__INCLUDED_)

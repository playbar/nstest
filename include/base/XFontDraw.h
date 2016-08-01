// XFontDraw.h: interface for the XFontDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFONTDRAW_H__06F70F98_BFB5_411C_98E5_2A9D7B55D2FF__INCLUDED_)
#define AFX_XFONTDRAW_H__06F70F98_BFB5_411C_98E5_2A9D7B55D2FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GGType.h"	// Added by ClassView

struct _FONTINFO
{
	int fw;
	int w,h;
	XU8* pData;
	bool bAlloc;
};

class XFontDraw
{
public:
//#ifdef _WIN32
	void* m_pFont;
	int	  m_fontSize;
	int	  m_fontID;
//#endif
	XFontDraw();
	~XFontDraw();
	void SetFont(const char* fontName,int id,int nSize);
	bool Draw(XPCWSTR str,int len,_FONTINFO&info);
private:
	static XFontDraw* m_pInstance;
public:
	static void Release()
	{
		if(m_pInstance)
			delete m_pInstance;
		m_pInstance = NULL;
	}
	static XFontDraw* GetInst()
	{
		if(m_pInstance==NULL)
			m_pInstance=new XFontDraw();
		return m_pInstance;
	}
};

#endif // !defined(AFX_XFONTDRAW_H__06F70F98_BFB5_411C_98E5_2A9D7B55D2FF__INCLUDED_)


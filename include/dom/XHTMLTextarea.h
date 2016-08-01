// XHTMLTextarea.h: interface for the XHTMLTextarea class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLTEXTAREA_H__F7706BBD_F7D1_4D25_BFF7_7F35DC5C6D6E__INCLUDED_)
#define AFX_XHTMLTEXTAREA_H__F7706BBD_F7D1_4D25_BFF7_7F35DC5C6D6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomInput.h"

class XHTMLTextarea : public XDomInput  
{
	XDEC_CREATE(XHTMLTextarea)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLTextarea(XU16 id):XDomInput(id)
	{
		m_bReturn=XFALSE;
		m_bFocus=XFALSE;
	}
protected:
	XU8 m_bReturn;
	XU32 SetPosition(XEVENTDATA*pData);
	XBOOL Paint(DRAWCONTEXT*pDraw);
	XU8 m_bFocus;
	void ResetText();
	XU32 Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bCells);
};

#endif // !defined(AFX_XHTMLTEXTAREA_H__F7706BBD_F7D1_4D25_BFF7_7F35DC5C6D6E__INCLUDED_)

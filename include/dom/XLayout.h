// XLayout.h: interface for the XLayout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLAYOUT_H__691F149B_5D8D_433F_B048_2B7901FBC7A1__INCLUDED_)
#define AFX_XLAYOUT_H__691F149B_5D8D_433F_B048_2B7901FBC7A1__INCLUDED_

#include "XDomItem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class XLayout  
{
public:
	enum {XL_NONE,XL_NEWROW=1,XL_NEWFLOAT=2,XL_TEXTINDENT=4};
	XLayout(DRAWCONTEXT*pDraw,CELLDATA*pData,BORDERDATA*pMargin);
	virtual ~XLayout();

protected:
	DRAWCONTEXT* m_pDraw;
	CELLDATA*	 m_pData;
	BORDERDATA*	 m_pMargin;
};

#endif // !defined(AFX_XLAYOUT_H__691F149B_5D8D_433F_B048_2B7901FBC7A1__INCLUDED_)

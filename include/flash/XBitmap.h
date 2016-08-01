// XBitmap.h: interface for the XBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XBITMAP_H__1FCDBCB6_4D02_4695_92AB_0A99DDCA46FF__INCLUDED_)
#define AFX_XBITMAP_H__1FCDBCB6_4D02_4695_92AB_0A99DDCA46FF__INCLUDED_

#include "XDrawDib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XBitmap  
{
public:
	XBitmap* Clone();
	XDrawDib m_dib;
	XBitmap();
	virtual ~XBitmap();
	int Width(){return m_dib.Width();}
	int Height(){return m_dib.Height();}
};

#endif // !defined(AFX_XBITMAP_H__1FCDBCB6_4D02_4695_92AB_0A99DDCA46FF__INCLUDED_)

// XDomP.h: interface for the XDomP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMP_H__F7E60DA7_AAE2_45C2_BD01_FAAA53E3FD8F__INCLUDED_)
#define AFX_XDOMP_H__F7E60DA7_AAE2_45C2_BD01_FAAA53E3FD8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomP : public XDomNode  
{
	XDEC_CREATE(XDomP)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomP(XU16 ID=XDOM_P):XDomNode(ID)
	{
//		m_bReturn=XFALSE;
	}
	
protected:
//	XU8 m_bReturn;
	XBOOL Final();
	void LayerOutCells(DRAWCONTEXT *pDraw, CELLDATA *pData,XBOOL bCells);
};

#endif // !defined(AFX_XDOMP_H__F7E60DA7_AAE2_45C2_BD01_FAAA53E3FD8F__INCLUDED_)

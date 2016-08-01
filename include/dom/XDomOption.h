// XDomOption.h: interface for the XDomOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMOPTION_H__1DB9C499_86A6_45EB_8E68_FC5FD19EFBE5__INCLUDED_)
#define AFX_XDOMOPTION_H__1DB9C499_86A6_45EB_8E68_FC5FD19EFBE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"



class _XEXT_CLASS XDomOption : public XDomNode  
{
	XDEC_CREATE(XDomOption)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomOption(XU16 ID=XDOM_OPTION):XDomNode(ID)
	{
		m_nPosX=m_nPosY=0;
		m_bCheckBox=XFALSE;
	}
	void SetCheckBox(XU8 bCheck){m_bCheckBox=bCheck;}
protected:
	XU32 OnClick(XEVENTDATA*pData);
	XBOOL IsSelect(DRAWCONTEXT*pDraw);
	void Paint(DRAWCONTEXT*pDraw);
	DOMPOS m_nPosX,m_nPosY;
	XU8	 m_bCheckBox;
	void Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells);
};

#endif // !defined(AFX_XDOMOPTION_H__1DB9C499_86A6_45EB_8E68_FC5FD19EFBE5__INCLUDED_)

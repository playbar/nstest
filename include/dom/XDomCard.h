// XDomCard.h: interface for the XDomCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMCARD_H__A6AA4CB3_7762_4870_B141_9E91DFDDED42__INCLUDED_)
#define AFX_XDOMCARD_H__A6AA4CB3_7762_4870_B141_9E91DFDDED42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomCard : public XDomNode  
{
	XDEC_CREATE(XDomCard)
public:
	DOMPOS m_nWidth;
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomCard(XU16 ID=XDOM_CARD):XDomNode(ID)
	{
		m_nWidth=0;
	}
	XString16 m_strTitle;
protected:
	//XU16 m_nWidth;
	XU32 OnEvent(XEVENTDATA*pData,XU32 nEvent);
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData);
};

#endif // !defined(AFX_XDOMCARD_H__A6AA4CB3_7762_4870_B141_9E91DFDDED42__INCLUDED_)

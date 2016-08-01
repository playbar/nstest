// XDomFieldset.h: interface for the XDomFieldset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMFIELDSET_H__0D3EAA6E_409B_4E96_BC2A_42C80AFCA93F__INCLUDED_)
#define AFX_XDOMFIELDSET_H__0D3EAA6E_409B_4E96_BC2A_42C80AFCA93F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomFieldset : public XDomNode  
{
	XDEC_CREATE(XDomFieldset)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomFieldset(XU16 ID=XDOM_FIELDSET):XDomNode(ID)
	{
		m_nPosX=0;
		m_nPosY=0;
		m_nWidth=0;
		m_nHeight=0;
		m_nTextHeight=0;
		m_nTextWidth=0;
	}
protected:
	void Paint(DRAWCONTEXT*pDraw);
	void Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
	XString16 m_strTitle;
	DOMPOS	  m_nPosX,m_nPosY,m_nWidth,m_nHeight;
	DOMPOS	  m_nTextHeight,m_nTextWidth;
//	XString16 m_strTitle;
};

#endif // !defined(AFX_XDOMFIELDSET_H__0D3EAA6E_409B_4E96_BC2A_42C80AFCA93F__INCLUDED_)

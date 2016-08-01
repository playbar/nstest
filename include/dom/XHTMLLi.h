// XHTMLLi.h: interface for the XHTMLLi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLLI_H__8575CD0D_FDBF_4DD3_B8F6_24258882D66A__INCLUDED_)
#define AFX_XHTMLLI_H__8575CD0D_FDBF_4DD3_B8F6_24258882D66A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomBlock.h"

class XHTMLLi : public XDomBlock  
{
	XDEC_CREATE(XHTMLLi)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLLi(XU16 id):XDomBlock(id)
	{
		/*XVar var(XEAB::PADDING_LEFT,8);
		AddAttrib(var);
		m_nPosX=m_nPosY=0;*/
	}
	XU32 Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bCells);
	XU32 Paint(DRAWCONTEXT*pDraw);
protected:
	DOMPOS m_nPosX,m_nPosY,m_nWidth,m_nHeight;
};

class XHTMLUl : public XDomNode  
{
	XDEC_CREATE(XHTMLUl)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLUl(XU16 id):XDomNode(id){}
	
	//XU32 LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
};

class XHTMLOl : public XDomNode  
{
	XDEC_CREATE(XHTMLOl)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLOl(XU16 id):XDomNode(id){}
	
protected:
	XU8 m_bFinal;
};

#endif // !defined(AFX_XHTMLLI_H__8575CD0D_FDBF_4DD3_B8F6_24258882D66A__INCLUDED_)

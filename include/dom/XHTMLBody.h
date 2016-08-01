// XHTMLBody.h: interface for the XHTMLBody class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLBODY_H__A7B4EEDF_35FC_41FD_954B_283DD661D15C__INCLUDED_)
#define AFX_XHTMLBODY_H__A7B4EEDF_35FC_41FD_954B_283DD661D15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomCard.h"
#include "XDomBlock.h"

class _XEXT_CLASS XHTMLBody : public XDomCard  
{
	XDEC_CREATE(XHTMLBody)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	DOMPOS m_nWidth,m_nHeight,m_nPosX,m_nPosY;
	XHTMLBody(XU16 nID);
protected:
};

class _XEXT_CLASS XHTMLFont:public XDomBlock
{
	XDEC_CREATE(XHTMLFont)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLFont(XU16 nID):XDomBlock(nID){}
protected:
//	XU8 m_bReturn;
};

class _XEXT_CLASS XHTMLHR:public XDomLeaf
{
	XDEC_CREATE(XHTMLHR)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLHR(XU16 nID):XDomLeaf(nID)
	{
		m_nPosX=0;
		m_nPosY=0;
		m_nWidth=0;
		m_nHeight=0;
	}
protected:
	XU32 Paint(DRAWCONTEXT*pDraw);
	DOMPOS m_nPosX,m_nPosY,m_nWidth,m_nHeight;
	void GetSize(DRAWCONTEXT*pDraw,CELLDATA*pData,XSize&sz);
	XU32 Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bCells);
};

#include "XDomTable.h"

class _XEXT_CLASS XHTMLDiv:public XDomBlock
{
	XDEC_CREATE(XHTMLDiv)
public:
	//void CalcSize(DRAWCONTEXT*pDraw,CELLDATA*pData);
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLDiv(XU16 id):XDomBlock(id)
	{
		m_nDeltaX=0;
		m_bReturn=XFALSE;
	}
protected:
	XU8 m_bReturn;
	DOMPOS m_nDeltaX;
//	XU32 m_nWidth,m_nHeight;
};

class _XEXT_CLASS XHTMLNobr:public XDomNode
{
	XDEC_CREATE(XHTMLNobr)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLNobr(XU16 id):XDomNode(id){}
};
#endif // !defined(AFX_XHTMLBODY_H__A7B4EEDF_35FC_41FD_954B_283DD661D15C__INCLUDED_)

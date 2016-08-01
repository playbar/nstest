// XDomTable.h: interface for the XDomTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMTABLE_H__74814D46_42E4_4CE2_94E1_56D066E264A8__INCLUDED_)
#define AFX_XDOMTABLE_H__74814D46_42E4_4CE2_94E1_56D066E264A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomBlock.h"

class _XEXT_CLASS XDomTable : public XDomNode  
{
	XDEC_CREATE(XDomTable)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomTable(XU16 ID=XDOM_TABLE):XDomNode(ID)
	{
//		m_nPosX=m_nPosY=m_nWidth=m_nHeight=0;
		m_nWidth=0;
		m_nHeight=0;
		//m_nFixWidth=0;
		//m_nFixHeight=0;
		m_nMin=0;
		m_bFinal=0;
		m_bReturn=0;
//		m_bNoChild=1;
		//m_nWin=0;
		m_nPosX=m_nPosY=0;
	}
protected:
//	XU16Array m_rows,m_cols,m_fixCols;
	DOMPOSARRAY m_rows;
	XU8  m_bFinal,m_bReturn;//m_bNoChild;
	DOMPOS m_nWidth,m_nHeight,m_nMin,m_nPosX,m_nPosY;//,m_nFixHeight,/*m_nWin,*/m_nFixWidth;
	XU32 Paint(DRAWCONTEXT*pDraw);
	void MoreRowSpan(CELLDATA*pData);
	void LayeroutTable(DRAWCONTEXT*pDraw,CELLDATA&data);
	void LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
//	XString16 m_strTitle;
	
};

class _XEXT_CLASS XDomTD : public XDomBlock
{
	XDEC_CREATE(XDomTD)
public:
	XU32 Handle(XU32 nOpera,XU32 pData,XU32 pData2);
	XDomTD(XU16 ID=XDOM_TD):XDomBlock(ID)
	{
		//m_nFixWidth=0;
		m_nFixWidth=0;
		m_nFixHeight=0;
	}
protected:
	void SpanCol(CELLDATA*pData);
	XU32 LayeroutPre(DRAWCONTEXT*pDraw,CELLDATA*pData);
	XBOOL Paint(DRAWCONTEXT*pDraw,XU32 nData);
	void LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData,int span=0);
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData,int span=0);

#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	DOMPOS m_nFixHeight,m_nFixWidth;//,m_nFixWidth;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
#pragma pack(1)
	//XU16 //m_nPosX,m_nPosY,m_nWidth,m_nHeight;
	DOMPOS m_nFixHeight,m_nFixWidth;//,m_nFixWidth;
#pragma	pack()
#endif
};

class _XEXT_CLASS XDomTR : public XDomNode
{
	XDEC_CREATE(XDomTR)
public:
	XU32 Handle(XU32 nOpera,XU32 pData,XU32 pData2);
	XDomTR(XU16 ID=XDOM_TR):XDomNode(ID)
	{
	}
protected:
	XU32 LayeroutPre(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void RowSpan(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bAdd);
	void LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
};
#endif // !defined(AFX_XDOMTABLE_H__74814D46_42E4_4CE2_94E1_56D066E264A8__INCLUDED_)

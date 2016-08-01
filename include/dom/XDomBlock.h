// XDomBlock.h: interface for the XDomBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMBLOCK_H__07D6A2CB_FA21_41D3_92E6_D6805747018D__INCLUDED_)
#define AFX_XDOMBLOCK_H__07D6A2CB_FA21_41D3_92E6_D6805747018D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomAnchor.h"

class XDomBlock : public XDomA  
{
protected:
	XU32 LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bReturn);
	XU32 LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bReturn);
	XU8 InitSize(DRAWCONTEXT*pDraw,CELLDATA*pData,int nWidth,XU8 bInit);
	XU8 InitFixed(DRAWCONTEXT*pDraw,CELLDATA*pData,int ws);
	void SaveData(CELLDATA*pData);
protected:
#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	DOMPOS m_nWidth,m_nHeight,m_nPosX,m_nPosY;
	DOMPOS m_nMin;
	XU8  m_bFinal,m_nMaxed;
	XU8 m_bReturn;
	DOMPOS m_nDeltaX;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
	#pragma pack(1)
	DOMPOS m_nWidth,m_nHeight,m_nPosX,m_nPosY;
	DOMPOS m_nMin;
	XU8  m_bFinal,m_nMaxed;
	XU8 m_bReturn;
	DOMPOS m_nDeltaX;

	#pragma pack()
#endif

	DOMPOSARRAY m_rows,m_cols;

	XDomBlock(XU16 id):XDomA(id)
	{
		m_bFinal=XFALSE;
		m_nMaxed=0;
		m_nDeltaX=0;
		m_bReturn=XFALSE;
		m_nWidth=m_nHeight=0;
		m_nMin=0;
		//m_nFixWidth=m_nFixHeight=0;
		m_nPosX=m_nPosY=0;
	}
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
};

#endif // !defined(AFX_XDOMBLOCK_H__07D6A2CB_FA21_41D3_92E6_D6805747018D__INCLUDED_)

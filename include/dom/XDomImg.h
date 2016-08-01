// XDomImg.h: interface for the XDomImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMIMG_H__E97FFAF0_48A6_4CEC_876D_AE76D34B1F36__INCLUDED_)
#define AFX_XDOMIMG_H__E97FFAF0_48A6_4CEC_876D_AE76D34B1F36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomImg : public XDomLeaf  
{
	XDEC_CREATE(XDomImg)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomImg(XU16 ID=XDOM_IMG):XDomLeaf(ID)
	{
		m_nPosX=0;m_nPosY=0;
		m_nWidth=0;m_nHeight=0;
		m_pData=NULL;
		m_nIndex=0;
		m_bSelectAble=0;
		m_nTicks=0;
		m_nMode=4;
		m_bReturn=XFALSE;
		m_nDeltaX=0;
	}
	XBOOL SelectAble(DRAWCONTEXT*pDraw);
#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	XIMAGEDATA* m_pData;
	DOMPOS	    m_nPosX,m_nPosY,m_nWidth,m_nHeight,m_nDeltaX;
	XU8			m_nTicks,m_nIndex;
	XU8			m_nMode,m_bSelectAble,m_bReturn;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
	#pragma pack(1)
	XIMAGEDATA* m_pData;
	DOMPOS	    m_nPosX,m_nPosY,m_nWidth,m_nHeight,m_nDeltaX;
	XU8			m_nTicks,m_nIndex;
	XU8			m_nMode,m_bSelectAble,m_bReturn;
	#pragma pack()
#endif

protected:
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
	XU32 SetOffset(XEVENTDATA*pData);
	void GetRoundRect(XRect&rect);
	void GetString(XString16&str);
	void GetSize(DRAWCONTEXT*pDraw,XImage*pImage,XSIZE&sz,int&nMin);
	XImage* GetImage();
	XBOOL Paint(DRAWCONTEXT*pDraw,XDomLeaf*pItem);
	//void LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData);
};	  

//#pragma pack()

#endif // !defined(AFX_XDOMIMG_H__E97FFAF0_48A6_4CEC_876D_AE76D34B1F36__INCLUDED_)

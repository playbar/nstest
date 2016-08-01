// XDomText.h: interface for the XDomText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMTEXT_H__E28F4099_9E1A_4F6A_AD48_1BBAB2E261B5__INCLUDED_)
#define AFX_XDOMTEXT_H__E28F4099_9E1A_4F6A_AD48_1BBAB2E261B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomText : public XDomItem
{
	struct VDATA
	{
		XU16 iBegin,iCount;
		XString8 strKey;
	};
protected:
	XDEC_CREATE(XDomText)
public:
	XDomText(XU16 ID = XDOM_TEXT);
	~XDomText();

	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
protected:
	XU32 TranslateText(XEVENTDATA*pData);
	void MakeLineData(DRAWCONTEXT*pDraw,CELLDATA*pData,int mh,int mw,int sw);
	void GetRect(XRect&rect,XU32 nType);
	XBOOL HitTest(XINT x,XUINT y);
	XBOOL Paint(DRAWCONTEXT*p,XDomLeaf*pParent);
	void LayerOutCell(DRAWCONTEXT *pData1, CELLDATA *pData2);
	void LayerOutCells(DRAWCONTEXT *pData1, CELLDATA *pData2);
public:
	void SetText16(XPCWSTR str);
	void SetText(XPCTSTR strTxt);
	XString16 m_strTxt;
	//XWCHAR* m_strTxt;
	DOMPOSARRAY m_lineDatas;
	XU8		  m_nOffset[2];

#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	DOMPOS m_nHeight,m_nWin;
	VDATA* m_pVData;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
#pragma pack(1)
	DOMPOS m_nHeight,m_nWin;
	VDATA* m_pVData;
#pragma pack()
#endif
};

#endif // !defined(AFX_XDOMTEXT_H__E28F4099_9E1A_4F6A_AD48_1BBAB2E261B5__INCLUDED_)

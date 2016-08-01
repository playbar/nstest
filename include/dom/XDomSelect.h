// XDomSelect.h: interface for the XDomSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMSELECT_H__9D4C480F_2937_448D_8574_D7AF26F31DB6__INCLUDED_)
#define AFX_XDOMSELECT_H__9D4C480F_2937_448D_8574_D7AF26F31DB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomSelect : public XDomNode  
{
	XDEC_CREATE(XDomSelect)
public:
	XU32 Select(XPCTSTR strValue,XEVENTDATA*pData,XDomItem*pItem);
	XBOOL GetRoundRect(XRect&rect);
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomSelect(XU16 ID=XDOM_SELECT):XDomNode(ID)
	{
		m_pList=XNULL;
		m_bReturn=XFALSE;
		//m_bIndexSelect=XFALSE;
		//m_nPosX=m_nPosY=m_nWidth=m_nHeight=0;
		//m_nIndex=0;
		//m_pSelect=XNULL;
	}
	~XDomSelect()
	{
		if(m_pList!=XNULL)
			delete m_pList;
	}
protected:
	XU8 m_bReturn;
	XU32 Connect(XEVENTDATA*pData);
	void SetMultiMark();
	void IndexToSelect(XUINT strIndex,XString8&strSelect);
	XU32 SetRepaint(XEVENTDATA*pData,XRect&rect);
	void CreateList(DRAWCONTEXT*pDraw);
	XU8 IsListMode();
	XListBox* m_pList;
	XString8  m_strSelect;
//	XU8		  m_bIndexSelect;
	XBOOL SetVarible(XEVENTDATA*pData,XBOOL bFromList);
	int IndexOf(XPCTSTR strValue);
	XBOOL OnLoad(XEVENTDATA*pData);
	XBOOL Paint(DRAWCONTEXT*pDraw);
	void LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells);
};

#endif // !defined(AFX_XDOMSELECT_H__9D4C480F_2937_448D_8574_D7AF26F31DB6__INCLUDED_)

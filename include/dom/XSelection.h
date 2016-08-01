// XSelection.h: interface for the XSelection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSELECTION_H__1A76260E_57AC_49D5_813A_CE33EF32F61C__INCLUDED_)
#define AFX_XSELECTION_H__1A76260E_57AC_49D5_813A_CE33EF32F61C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XSelection:public XSort  
{
public:
	struct XSI
	{
		int nLength;
		int nDelta;
		XDomItem* pItem;
	};
public:
	XDomItem* GetSelect();
	XU32 Select(XDomItem*pItem);
	void Init(DRAWCONTEXT*pDraw,XDomItem*p,const XSIZE&sz,XU8 dir,XU8 steps);
	XSelection();
	virtual ~XSelection();

protected:
	XINT Delta(int s0,int s1,int d0,int d1);
	XU32 GetCount(){return m_list.GetSize();}
	void* GetData(XU32 id) 
	{return (void*)m_list[id]->nLength;}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareInt((XINT)iSrt,(XINT)iDst);
	}
	XRect m_select;
	XU8   m_nDirect,m_nSteps;
	XVector<XSI*> m_list;
	DRAWCONTEXT*m_pDraw;
};

#endif // !defined(AFX_XSELECTION_H__1A76260E_57AC_49D5_813A_CE33EF32F61C__INCLUDED_)

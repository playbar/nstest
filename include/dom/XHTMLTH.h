// XHTMLTH.h: interface for the XHTMLTH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLTH_H__25C8B68F_4ADF_4CE6_8DAA_4C78DF5BDCDD__INCLUDED_)
#define AFX_XHTMLTH_H__25C8B68F_4ADF_4CE6_8DAA_4C78DF5BDCDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomTable.h"

class XHTMLTH : public XDomTD  
{
	XDEC_CREATE(XHTMLTH)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLTH(XU16 id):XDomTD(id){}
protected:

};

class XHTMLCaption:public XDomTD
{
	XDEC_CREATE(XHTMLCaption)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLCaption(XU16 id):XDomTD(id){}
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData);

};



#endif // !defined(AFX_XHTMLTH_H__25C8B68F_4ADF_4CE6_8DAA_4C78DF5BDCDD__INCLUDED_)

// XHTMLA.h: interface for the XHTMLA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLA_H__E308C33B_1C60_4DC5_8ACC_3FBB91AFCB3E__INCLUDED_)
#define AFX_XHTMLA_H__E308C33B_1C60_4DC5_8ACC_3FBB91AFCB3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomAnchor.h"
#include "XDomBlock.h"

class XHTMLA : public XDomBlock
{
	XDEC_CREATE(XHTMLA)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XHTMLA(XU16 id):XDomBlock(id){};
protected:
	XBOOL IsInline();
	XU32 LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bCells);
};

#endif // !defined(AFX_XHTMLA_H__E308C33B_1C60_4DC5_8ACC_3FBB91AFCB3E__INCLUDED_)

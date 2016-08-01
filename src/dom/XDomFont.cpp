// XDomFont.cpp: implementation of the XDomFont class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XDomFont::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_PAINT:
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
		{
			if(!IsVisiable()) return 0;
			DRAWCONTEXT*p=(DRAWCONTEXT*)pData1;
			XU32 f=p->pDraw->GetFontStyle();
			XU32 fs=p->FONTSIZEALL;
			XU32 ft=p->FONTSTYLE;
			switch(m_nID)
			{
			case XDOM_I:p->SetItalic(XTRUE);break;
			case XDOM_B:
			case XDOM_EM:
			case XDOM_STRONG:
				 p->SetBold(XTRUE);break;
			case XDOM_U:p->SetUnderline(XTRUE);break;
			case XDOM_BIG:p->SetFontSize((XU8)XGraphics::XS_BIG);break;
			case XDOM_SMALL:p->SetFontSize((XU8)XGraphics::XS_SMALL);break;
			}
			XDomNode::Handle(nOpera,pData1,pData2);
			p->FONTSIZEALL=fs;
			p->FONTSTYLE=ft;
			p->pDraw->SetFontStyle(f);
		}
		return XTRUE;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

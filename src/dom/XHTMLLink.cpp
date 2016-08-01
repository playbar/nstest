// XHTMLLink.cpp: implementation of the XHTMLLink class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLLink.h"
#include "XDom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XHTMLLink::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_GET_STYLE:
		 return XDomLeaf::Handle(nOpera,pData1,pData2);
	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 return XDomLeaf::Handle(nOpera,pData1,pData2);
	case XDO_SET_FINAL:
		if(pData2)
		{
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			if(pDraw->nLayerMode==LA_SAMPLE) break;
			XPCTSTR strURL=FindAttrib(XEAB::HREF,(XPCTSTR)XNULL);
			if(strURL!=XNULL)
			{
			 XEVENTDATA*pData=(XEVENTDATA*)pData2;
		     m_pData=pData->pDom->m_imgList.SetImage(strURL,this,pDraw->pDom,XIT_DATA);
			}
		}
		return 0;
	case XDO_UPDATE_IMAGE:
		if(pData1&&m_pData!=XNULL)
		{
			if(m_pData->nType==XIT_DATA&&m_pData->data.GetSize()>0)
			{
				m_strTxt.SetString((XPCTSTR)m_pData->data.GetData(),m_pData->data.GetSize());
				XDom*pDom=(XDom*)pData1;
				StyleCSS(pDom);
				pDom->SetLayerout();
			}
		}
		break;
	}
	int nType=FindAttrib(XEAB::TYPE,0);
	if(nType==0) return 0;
	
	if(nType==XEIT::TEXT_CSS)
		return XHTMLStyle::Handle(nOpera,pData1,pData2);
	else
		return XHTMLScript::Handle(nOpera,pData1,pData2);	
}
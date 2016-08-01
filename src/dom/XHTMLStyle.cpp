// XHTMLStyle.cpp: implementation of the XHTMLStyle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLStyle.h"
#include "XCSS.h"
#include "XDom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XHTMLStyle::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_SET_FINAL:
		 return StyleCSS(((DRAWCONTEXT*)pData1)->pDom); 
	}
	return XHTMLScript::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLStyle::StyleCSS(XDom*pDom)
{
	if(pDom==XNULL||m_strTxt.IsEmpty())
	    return 0;
	XCSS*pCss=&pDom->m_css;
	//XURL*pURL=XNULL;
	XPCTSTR strURL=FindAttrib(XEAB::HREF,(XPCTSTR)XNULL);
	if(strURL!=XNULL)
	{
		XURL url=pDom->m_data.tempURL;
		url.SetURL(strURL);
		if(url.m_strPath!=pDom->m_data.tempURL.m_strPath)
		{
			pCss->AddCSS(&url,m_strTxt);
			m_strTxt.Empty();
			return 1;
		}
	}

	pCss->AddCSS(XNULL,m_strTxt);
	m_strTxt.Empty();
	return 1;
}

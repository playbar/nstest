// XDomGo.cpp: implementation of the XDomGo class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomGo.h"
#include "XEnumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomGo::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_CONNECT:
		 return Connect((XEVENTDATA*)pData1);
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 case XEAB::ACCEPT_CHARSET:
			  if(p->nType==VSTRING)
			  {
				  XString8 str(p->strData,STRING_ATTACH);
				  p->strData=XNULL;
				  p->nType=VINT;
				  *p=XEC().Index(str);
			  }
			  break;
		 case XEAB::METHOD:
			  if(p->nType==VSTRING)
			  {
				  XString8 str(p->strData,STRING_ATTACH);
				  p->strData=XNULL;
				  p->nType=VINT;
				  *p=XEM::Index(str);
			  }
			  break;
		 case XEAB::SENDREFERER:
			  p->ToLogic();
			  break;
		 }break;*/
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XDomGo::Connect(XEVENTDATA *pData)
{
	XU32 s=XRCONNECT;
 	if(FindAttrib(XEAB::SENDREFERER,XFALSE))
		pData->strRefer=pData->setURL.m_strURL;
	XString8 strURL=FindAttrib(XEAB::HREF,"");
	Translate(pData->strValues,strURL);
	pData->SetURL(strURL,FindAttrib(XEAB::METHOD,XEM::GET));
	s|=XDomNode::Handle(XDO_CONNECT,(XU32)pData,0);
	return s;
}

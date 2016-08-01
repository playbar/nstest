// XHTMLScript.cpp: implementation of the XHTMLScript class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLScript.h"
#include "XDom.h"
#include "XDomView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XHTMLNOScript::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
   switch(nOpera)
   {
   case XDO_PAINT:
   case XDO_LAYEROUT_CELLS:
   case XDO_LAYEROUT_CELL:
	    return 0;
   case XDO_GET_STYLE:
	    return DOM_SCRIPT|DOM_NODE;
	    //return XTRUE;
   }
   return XDomLeaf::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLScript::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
   switch(nOpera)
   {
//#ifdef _WINEMU
   case XDO_SET_FINAL:
		if(pData2)
		{
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			//if(pDraw->nLayerMode==LA_SAMPLE) break;
			XPCTSTR strURL=FindAttrib(XEAB::SRC,(XPCTSTR)XNULL);
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
#ifdef _WINEMU
				XDom*pDom=(XDom*)pData1;
				const char*strPath=pDom->GetDrawContext()->pView->GetWorkPath();
				const char*strURL=m_pData->strURL;
				const char* strStart=strrchr(strURL,'/');
				const char* strEnd=strrchr(strURL,'?');
				XString8 strFile=strPath;
				XString8 strTmp;
				if(strStart==NULL) strStart=strURL;
				strTmp.SetString(strStart,strEnd==NULL?0:strEnd-strStart);
				strFile+="\\Scripts\\";
				XFile::CreateFolder(strFile);
				strFile+=strTmp;
				XFile file;
				if(file.Open(strFile,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
				{
					file.Write(m_pData->data.GetData(),m_pData->data.GetSize());
					file.Close();
				}
#endif
				m_strTxt.SetString((XPCTSTR)m_pData->data.GetData(),m_pData->data.GetSize());				
			}
		}
		break;
//#endif
   case XDO_SET_DATA:
	    if(pData2>0)
		{
			m_strTxt.SetString((XPCTSTR)pData1,pData2);

		}
	    break;
   case XDO_GET_STYLE:
	    return DOM_SCRIPT|DOM_NODE;
	    //return XTRUE;
   }
   return XDomLeaf::Handle(nOpera,pData1,pData2);
}

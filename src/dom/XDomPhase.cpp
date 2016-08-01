// XDomPhase.cpp: implementation of the XDomPhase class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxggdom.h"
#include "XDomPhase.h"
#include "XEnumData.h"
#include "XDomAnchor.h"
#include "XDomAccess.h"
#include "XDomFont.h"
#include "XDomBR.h"
#include "XDomCard.h"
#include "XDomImg.h"
#include "XDomMeta.h"
#include "XDomP.h"
#include "XDomTable.h"
#include "XDomTemplate.h"
#include "XDomText.h"
#include "XDomDo.h"
#include "XDomFieldset.h"
#include "XDomPostfield.h"
#include "XDomGo.h"
#include "XDomRefresh.h"
#include "XDomPrev.h"
#include "XDomPre.h"
#include "XDomOnEvent.h"
#include "XDomInput.h"
#include "XDomOptGroup.h"
#include "XDomOption.h"
#include "XDomSelect.h"
#include "XDomSetvar.h"
#include "XDomTimer.h"
#include "XDomEcho.h"
#include "XDomView.h"
#include "XDom.h"
//---------------------------------------------------
#include "XHTMLBody.h"
#include "XHTMLScript.h"
#include "XHTMLSup.h"
#include "XHTMLInput.h"
#include "XHTMLForm.h"
#include "XHTMLButton.h"
#include "XHTMLTextarea.h"
#include "XHTMLStyle.h"
#include "XHTMLTH.h"
#include "XHTMLObject.h"
#include "XHTMLFrame.h"
#include "XHTMLLi.h"
#include "XHTMLLink.h"
#include "XHTMLMap.h"
#include "XHTMLA.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


XDomPhase::XDomPhase(XDom*pDom)
{
   m_pDom=pDom;
}

XDomPhase::~XDomPhase()
{
}

inline int XDomPhase::NeedConvert()
{
		switch(m_pDom->m_nCharset)
		{
			case XCHARSET_US_ASCII:
		    case XCHARSET_GBK:
		    case XCHARSET_GB2314:
		    case XCHARSET_GB2312:
				 return XTRUE;
		}
		return XFALSE;
}



static XPCTSTR _strEntry[]={"?xml","!doctype"};

static XU16 _nChildsOption[]={XDOM_TEXT,XDOM_IMG,0};
static XU16 _nChildsNTD[]={XDOM_TD,XDOM_TR,0};

XDomPhaseXML::_DOMENTRY XDomPhaseXML::_tagToken[]=
{
	{XUSE_CREATE(XDomEcho),"!--",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLA),"a",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"acc",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomAccess),"access",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"adset",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomAnchor),"anchor",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"applet",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLArea),"area",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"b",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"big",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLBody),"body",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomBR),"br",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLButton),"button",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLCaption),"caption",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomCard),"card",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLCenter),"center",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"code",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomCard),"defines",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomNode),"dfn",XTRUE,XNULL,XNULL,XFALSE},
	//file_info,operate_define,display_define,sound_define,network_define,txt_define
	{XUSE_CREATE(XDomNode),"display_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XHTMLDiv),"div",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomDo),"do",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"em",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLEmbed),"embed",XTRUE,XNULL,XNULL,XTRUE},
	{XUSE_CREATE(XDomNode),"endset",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFieldset),"fieldset",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"file_info",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XHTMLFont),"font",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLForm),"form",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFrame),"frame",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFrameset),"frameset",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomGo),"go",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFont),"h1",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFont),"h2",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFont),"h3",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFont),"h4",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFont),"h5",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLFont),"h6",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"head",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLHR),"hr",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"html",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"i",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLIFrame),"iframe",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomImg),"img",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLInput),"input",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLInput),"inputa",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"key_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomNode),"key_map",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomNode),"kbd",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLLi),"li",XTRUE,XNULL,XNULL,XFALSE},
	//{XUSE_CREATE(XHTMLLink),"link",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"link",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLMap),"map",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomMeta),"meta",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"mouse_normal_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomNode),"mouse_normal_map",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomNode),"mouse_table_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD	
	{XUSE_CREATE(XDomNode),"mouse_table_map",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomNode),"network_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XHTMLNobr),"nobr",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLNOFrames),"noframes",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomItem),"noop",XFALSE,XNULL,XNULL,XFALSE},
	//{XUSE_CREATE(XHTMLNOScript),"noscript",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"noscript",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLObject),"object",XTRUE,XNULL,XNULL,XTRUE},
	{XUSE_CREATE(XHTMLOl),"ol",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomOnEvent),"onevent",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"operate_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomOptGroup),"optgroup",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomOption),"option",XFALSE,_nChildsOption,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLSpan),"p",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomLeaf),"param",XFALSE,XNULL,XNULL,XTRUE},
	{XUSE_CREATE(XDomPostfield),"postfield",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomPre),"pre",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomPrev),"prev",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomRefresh),"refresh",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"s",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"samp",XTRUE,XNULL,XNULL,XFALSE},
//#if (__CORE_VERSION__>= 0x02077000)
	{XUSE_CREATE(XDomSBR),"sbr",XFALSE,XNULL,XNULL,XFALSE},
//#endif
	{XUSE_CREATE(XHTMLScript),"script",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomSelect),"select",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomSetvar),"setvar",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"showtag",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"small",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"sound_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XHTMLSpan),"span",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"strike",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomFont),"strong",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLStyle),"style",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLSup),"sub",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLSup),"sup",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomTable),"table",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomTD),"td",XTRUE,XNULL,_nChildsNTD,XFALSE},
	{XUSE_CREATE(XDomTemplate),"template",XTRUE,XNULL,XNULL,XFALSE},
	
	{XUSE_CREATE(XHTMLTextarea),"textarea",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"textformat",XFALSE,XNULL,XNULL,XFALSE},

	{XUSE_CREATE(XHTMLTH),"th",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomTimer),"timer",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLTitle),"title",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"touch_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomNode),"touch_map",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	{XUSE_CREATE(XDomTR),"tr",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"tt",XFALSE,XNULL,XNULL,XFALSE},
	//{XUSE_CREATE(XDomNode),"txt",XTRUE,XNULL,XNULL},//for config DTD
	{XUSE_CREATE(XHTMLScript),"txt_define",XTRUE,XNULL,XNULL,XFALSE},//for config DTD
	
	{XUSE_CREATE(XDomFont),"u",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XHTMLUl),"ul",XTRUE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"var",XFALSE,XNULL,XNULL,XFALSE},
	{XUSE_CREATE(XDomNode),"wml",XTRUE,XNULL,XNULL,XFALSE},
};

#define DOMCOUNT sizeof(XDomPhaseXML::_tagToken)/sizeof(XDomPhaseXML::_DOMENTRY)

class XSortEntry:public XSort
{
public:
	XU32 GetCount(){return DOMCOUNT;}
	void* GetData(XU32 nID)
	{
		return (void*)XDomPhaseXML::_tagToken[nID].strEntry;
	}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
};

XU8 XDomPhaseXML::StringParam(XU16 id)
{
	if(id>=sizeof(_tagToken)/sizeof(XDomPhaseXML::_DOMENTRY))
		return XTRUE;
	return _tagToken[id].bStringParam;
}

XU8 XDomPhaseXML::MustEnd(XU16 id)
{
	//if(pItem==XNULL) return XTRUE;
	//int id=pItem->GetID();
	if(id>=sizeof(_tagToken)/sizeof(XDomPhaseXML::_DOMENTRY))
		return XTRUE;
	return _tagToken[id].bMustEnd;
}


XPCTSTR XDomPhaseXML::GetToken(XU16 ID)
{
	if(ID>=sizeof(_tagToken)/sizeof(XDomPhaseXML::_DOMENTRY)) 
		return XNULL;
	return _tagToken[ID].strEntry;
}

XU16* XDomPhaseXML::GetTokenChilds(XU16 ID)
{
	if(ID>=sizeof(_tagToken)/sizeof(XDomPhaseXML::_DOMENTRY)) 
		return XNULL;
	return _tagToken[ID].nChilds;
}
XU16* XDomPhaseXML::GetTokenNChilds(XU16 ID)
{
	if(ID>=sizeof(_tagToken)/sizeof(XDomPhaseXML::_DOMENTRY)) 
		return XNULL;
	return _tagToken[ID].nNChilds;
}

XBOOL XDomPhaseXML::PhaseData(XTCHAR *pData, int nSize,bool bFlash,bool bCondenseWhite)
{
	//FILE * pfile = fopen("/sdcard/testtxt", "wr" );
	//fwrite( pData, nSize, 1, pfile );
	//fclose( pfile );

	if(m_bFirst&&nSize>2)
	{
		XU32 v=(((XU8)pData[0])<<16)|(((XU8)pData[1])<<8)|((XU8)pData[2]);
		switch(v)
		{
		case 0xefbbbf:
			 nSize-=3;
			 pData+=3;
			 m_pDom->m_nCharset=XCHARSET_UTF8;
			 break;
		}
		m_bFirst=XFALSE;
	}

	//XU8 nCharset=m_pDom->m_nCharset;
#if defined( _SYMBIAN) && defined(_DEBUG)
		int whilecount = 0;
#endif
	while(nSize>0)
	{
#if defined( _SYMBIAN) && defined(_DEBUG)
		whilecount++;
		SYMBIANDEB1(DefaultLog,"%d\r\n",whilecount);
		if(whilecount == 140)
			{
			int a;
			a=0;
			}
#endif
		/*switch(nCharset)
		{
			case XCHARSET_ISO_8859_1:
			case XCHARSET_US_ASCII:
			case XCHARSET_GBK:
			case XCHARSET_GB2314:
			case XCHARSET_GB2312:				 
				 str.ConvertFrom(pData,m_pDom->m_nCharset,nSize);
				 nCharset=XCHARSET_UTF8;
				 pData=str.GetData();
				 nSize=str.GetLength();
				 break;
		}*/
		int l=nSize;
		switch(m_nStatus)
		{
		/*case XPS_CHARSET:
			 switch(m_pDom->m_nCharset)
			 {
			    case XCHARSET_ISO_8859_1:
				case XCHARSET_US_ASCII:
				case XCHARSET_GBK:
				case XCHARSET_GB2314:
				case XCHARSET_GB2312:				 
					 str.ConvertFrom(pData,m_pDom->m_nCharset,nSize);
					 pData=str.GetData();
					 nSize=str.GetLength();
					 
					 break;
			 }
			 m_bStart=XTRUE;
			 SetStatus(XPS_TEXT);
			 l=0;
			 break;*/

		case XPS_SCRIPT:
			//LOGWHERE();
			l=PhaseScript(pData,nSize);
			break;
		case XPS_TEXT:
			//LOGWHERE();
			l=PhaseText(pData,nSize,bFlash, bCondenseWhite);
			break;
		case XPS_TAG:
			//LOGWHERE();
			l=PhaseTag(pData,nSize);
			break;
		case XPS_ATTNAME:
			//LOGWHERE();
			l=PhaseName(pData,nSize);
			break;
		case XPS_ATTVAL:
			//LOGWHERE();
			l=PhaseValue(pData,nSize);
			break;
		case XPS_STRING:
			//LOGWHERE();
			l=PhaseString(pData,nSize);
			break;
		case XPS_CHAR:
			//LOGWHERE();
			l=PhaseChar(pData,nSize);
			break;
		case XPS_ECHO:
			//LOGWHERE();
			l=PhaseEcho(pData,nSize);
			break;
		case XPS_DATA:
			//LOGWHERE();
			l=PhaseKeyData(pData,nSize,XFALSE);
			break;
		case XPS_SKIP:
			//LOGWHERE();
			l=PhaseKeyData(pData,nSize,XTRUE);
			break;

		}
		//if(l<=0) 
		//	return XFALSE;
		pData+=l;
		nSize-=l;
		//XPCTSTR pSub=strstr(pData,".sec1");
		//if(pSub!=XNULL&&pSub-pData<100)
		//	int a=0;
	}
	//if(m_nStatus==XPS_TEXT&&!m_strValue.IsEmpty())
	//	m_pDom->AddText(m_strValue);
	return XTRUE;
}

//#if (__CORE_VERSION__>=0x02073000)
XBOOL XDomPhaseXML::EndParse(XBOOL bFlash)
{
	if(m_nStatus==XPS_TEXT&&!m_strValue.IsEmpty())
	{
		m_pDom->AddText(m_strValue,bFlash);
		m_strValue.Empty();
		return XTRUE;
	}
	if(m_nStatus==XPS_CHAR&&!m_strChar.IsEmpty())
	{
		m_strValue="&";
		m_strValue+=m_strChar;
		m_pDom->AddText(m_strValue,bFlash);
	}
	return XFALSE;
}
//#else
//XBOOL XDomPhaseXML::EndParse()
//{
//	if(m_nStatus==XPS_TEXT&&!m_strValue.IsEmpty())
//	{
//		m_pDom->AddText(m_strValue);
//		m_strValue.Empty();
//		return XTRUE;
//	}
//#if (__CORE_VERSION__>=0x02070100)
//	if(m_nStatus==XPS_CHAR&&!m_strChar.IsEmpty())
//	{
//		m_strValue="&";
//		m_strValue+=m_strChar;
//		m_pDom->AddText(m_strValue);
//	}
//#endif
//	return XFALSE;
//
//}
//#endif
XDomPhaseXML::~XDomPhaseXML()
{
  
}

XDomPhaseXML::XDomPhaseXML(XDom *pDom):XDomPhase(pDom)
{
   SetStatus(XPS_TEXT);
   m_bURL=XFALSE;
//   m_bStart=XFALSE;
   m_bMustEnd=XFALSE;
   m_bFirst=XTRUE;
}

void XDomPhaseXML::SetStatus(XU8 nStatus)
{
	//m_bURL=XFALSE;

	switch(nStatus)
	{
	default:
//	case XPS_CHARSET:break;
	case XPS_TEXT:
		 /*if(!m_bStart&&m_pDom->IsStartItem())
		 {
			 nStatus=XPS_CHARSET;
			 break;
		 }
		 else*/ if(m_pDom->IsScriptItem()==XNULL)//m_strKey))
		 {
			m_strValue.Empty();
			m_strKey.Empty();
			m_strTxt.Empty();
			m_nSpaceCount=0;
			break;	 
		 }
	case XPS_SCRIPT:
		 nStatus=XPS_SCRIPT;
		 m_strValue.Empty();
		 m_strTxt.Empty();
		 m_nSpaceCount=0;
		 m_nKeyCount=0;
		 break;
	case XPS_ATTNAME:
		 m_nSpaceCount=0;
	case XPS_TAG:
		 m_strKey.Empty();
		 break;
	case XPS_SKIP:break;
	case XPS_DATA:
	case XPS_ATTVAL:
		 m_strValue.Empty();
		 m_strTxt.Empty();
		 break;
	case XPS_CHAR:
		 m_strChar.Empty();
	case XPS_STRING:	
		 m_stack.Push(m_nStatus);
		 break;
	}
	m_nLength=0;
	m_nStatus=nStatus;
}

XBOOL XDomPhaseXML::AddTag(XU8 bEnd)
{
	if(m_strKey.IsEmpty()) return XTRUE;
	
	m_strKey.MakeLower();
	/*char* c=m_strKey.GetData();
	while(*c)
	{
		if(*c<0)
			int v=0;
		c++;
	}*/

	if(m_strKey[0]==XDIV)
	{
		//m_pDom->FinishToken();
		SetStatus(XPS_SKIP);
		return m_pDom->Parent(IndexOf(m_strKey.GetData()+1));
	}
	//m_strKey.MakeLower();
	if(m_strKey.Compare(_strEntry[0],XTRUE)==0) //?xml
	   SetStatus(XPS_ATTNAME);
	else if(m_strKey.Compare(_strEntry[1],XTRUE)==0) //!doctype
	   SetStatus(XPS_SKIP);
	else
	{
		XDomItem*p=CreateDomItem();
		if(p==NULL)
		{
			SetStatus(XPS_SKIP);
			return XTRUE;
		}
		if(!m_pDom->m_pDoc)
		{
		switch(p->GetID())
			{
		case XDOM_WML:
		case XDOM_QUESTION:
		case XDOM_HTML:break;
		case XDOM_CARD:
			 m_pDom->AddTag(new XDomNode(XDOM_WML));
			 break;
		default:
				//if(!(p->GetStyle()&XDomItem::DOM_NODE))
				{
					m_pDom->AddTag(new XDomNode(XDOM_WML));
					m_pDom->AddTag(new XDomCard());
				}
			}
		}
		m_pDom->AddTag(p);		
		XU32 s=p->GetStyle();
		XU16 nTokenID=p->GetID();
		m_bMustEnd=(nTokenID<DOMCOUNT)?_tagToken[p->GetID()].bMustEnd:XTRUE;
		//else
		{
			if(nTokenID==XDOM_ECHO)
				SetStatus(XPS_ECHO);
			else if(s&XDomItem::DOM_DATA)
				SetStatus(XPS_DATA);
			else if(!bEnd)
				SetStatus(XPS_ATTNAME);
			if(bEnd&&!(s&XDomItem::DOM_NODE))
				m_pDom->Parent();
		}
	}
	if(m_pDom->IsScriptItem())
		m_bURL=XTRUE;
	else
		m_bURL=XFALSE;
	return XTRUE;
}

XDomItem* XDomPhaseXML::CreateDomItem()
{
	_CREATEDOM p=XNULL;
	XSortEntry e;
	int id=e.Index(m_strKey);
	if(id>=0)// return XNULL;
		p=_tagToken[id].Create;
	/*for(XU16 i=0;i<sizeof(_tagToken)/sizeof(_DOMENTRY);i++)
	{
	  if(m_strKey.Compare(_tagToken[i].strEntry,XTRUE)==0)
	  {
		  p=_tagToken[i].Create;
		  break;
	  }
	}*/
	if(p!=XNULL) 
		return (*p)(id);
	else if(m_strKey[0]==XQUEST)
		return XDomQuestion::CreateObject();
	else if(m_strKey[0]!=XEX)
	   return new XDomNode(id);
	return XNULL;
}

inline int XDomPhaseXML::PhaseText(XTCHAR *pData, int nSize,bool bFlash,bool bCondenseWhite)
{
	for(int i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		case XAND://'&'
			 SetStatus(XPS_CHAR);
			 return i+1;
		case XLE://'<':
			{
				if(TransCharset() )
				{
//#if (__CORE_VERSION__>=0x02073000)
					m_pDom->AddText(m_strTxt,bFlash);
//#else
//					m_pDom->AddText(m_strTxt);
//#endif
					//LOGE("%s, %s, %d, data:%s", __FILE__, __FUNCTION__, __LINE__, m_strTxt.GetData() );
				}
				else 
				{
//#if (__CORE_VERSION__>=0x02073000)
					m_pDom->AddText(m_strValue,bFlash);
//#else
//					m_pDom->AddText(m_strValue);
//#endif
					//LOGE("%s, %s, %d, data:%s", __FILE__, __FUNCTION__, __LINE__, m_strValue.GetData() );
				}

				SetStatus(XPS_TAG);
				return i+1;
			}
//#if (__CORE_VERSION__>=0x02081000)
		case XCR:
		case XCT:
			if (bCondenseWhite)
			{
				m_strValue+=XSPACE;
				break;
			}
			else
			{
				m_nSpaceCount=0;
				if((XU8)pData[i]>XSPACE) 
					m_strValue+=pData[i];
				else if(m_pDom->m_nFormat||bFlash)
					m_strValue+=pData[i];
				else if(bFlash)
					m_strValue+=pData[i];
				break;
			}

		case XCTAB:
			{
				if( bFlash )
				{
					if( m_strValue.GetLength() > 1 && m_strValue[i-1] == XSPACE )
					{
						break;
					}
					else
					{
						if (bCondenseWhite)
						{
							m_strValue+=XSPACE;
						}
						else
						{
							for (int i=0;i<8;i++)
							{
								m_strValue+=XSPACE;
							}
						}

						//m_strValue+=XSPACE;
					}
				}
				else
				{
					m_nSpaceCount=0;
					if(m_pDom->m_nFormat )
						m_strValue+=pData[i];
				}
			}
			break;
//#endif
		case XSPACE:
//#if (__CORE_VERSION__>=0x02081000)
			if (bCondenseWhite)
			{
				if( bFlash )
				{
					if(  m_nSpaceCount > 0 )
					{
						m_nSpaceCount++;
						break;
					}
					if(m_pDom->m_nFormat )
					{
						m_strValue+=pData[i];
						m_nSpaceCount++;
					}
				}
				m_nSpaceCount=1;
				m_strValue+=XSPACE;
			}
			/*if( bFlash )
			{
				if(  m_nSpaceCount > 0 )
				{
					 m_nSpaceCount++;
					 break;
				}
				if(m_pDom->m_nFormat )
				{
					m_strValue+=pData[i];
					m_nSpaceCount++;
				}
			}*/
			/*if(bCondenseWhite)
			{
				if (m_nSpaceCount>0)
				{
					m_nSpaceCount=0;
				}
				m_strValue+=XSPACE;
			}*/
			else
			{
				if(m_pDom->m_nFormat||bFlash)
				{
					m_strValue+=pData[i];
				}
				else if(m_nSpaceCount<1)
				{
					 m_strValue+=pData[i];
					 m_nSpaceCount++;
				}
			}
//#else
//			 if(m_strValue.IsEmpty()) break;
//			 if(m_pDom->m_nFormat||bFlash)
//				 m_strValue+=pData[i];
//			 else if(m_nSpaceCount<1)
//			 {
//				m_strValue+=pData[i];
//				m_nSpaceCount++;
//			 }
//#endif
			 break;
		/*case XCT:
		case XCR:
			 if(bFlash)
			 {
				 m_strValue+=pData[i];
				 break;
			 }*/
		default:
			 m_nSpaceCount=0;
			 if((XU8)pData[i]>XSPACE) 
				 m_strValue+=pData[i];
			 else if(m_pDom->m_nFormat||bFlash)
				 m_strValue+=pData[i];
			 else if(bFlash)
				 m_strValue+=pData[i];
			 break;
		}
	}
	return nSize;
}

inline int XDomPhaseXML::PhaseTag(XTCHAR *pData, int nSize)
{
	for(int i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		case XQUEST:
			 if(m_strKey.IsEmpty()) m_strKey+=pData[i];
			 else
			 {
				 AddTag(XFALSE);
				 m_pDom->FinishToken(XFALSE);
				 m_pDom->Parent();
				 SetStatus(XPS_SKIP);
				 return i+1;
			 }

		case XCR:
		case XCT:
		case XSPACE:// ' '表示一个输入签结束,后面包含属性
			 //SetStatus(XPS_ATTNAME);
			 AddTag(XFALSE);
			 return i+1;
		case XLA:// '>'
			 AddTag(XTRUE);
			 m_pDom->FinishToken(XFALSE);
			 //if(m_nStatus==XPS_TAG)
			 SetStatus(XPS_TEXT);
			 return (i+1);
		case XDIV:// '/'表示输入输入签结束,且该输入签和结束签
			 if(m_strKey.IsEmpty()) 
			 {
				 m_strKey+=pData[i];
				 break;
			 }
			 else
			 {
				 AddTag(XFALSE);
				 m_pDom->FinishToken(XFALSE);
				 m_pDom->Parent();
				 SetStatus(XPS_SKIP);
				 return i+1;
			 }
			 break;
		default:
			 m_strKey+=pData[i];
			 if(m_strKey.GetLength()==3&&
				m_strKey==_tagToken[XDOM_ECHO].strEntry)
			 {
				 AddTag(XFALSE);
				 SetStatus(XPS_ECHO);
				 return i+1;
			 }
			 break;
		}
	}
	return nSize;
}

inline int XDomPhaseXML::PhaseName(XTCHAR *pData, int nSize)
{
		for(int i=0;i<nSize;i++)
		{
			switch(pData[i])
			{
			default:
				if((XU8)pData[i]>XSPACE) 
				{
					if(m_nSpaceCount)
					{
						m_strKey.Empty();
						m_nSpaceCount=0;
					}
					m_strKey+=pData[i];
				}
				break;
			case XQUEST:
			case XDIV://'/':
				//TransCharset();
				if(!m_strKey.IsEmpty())
				{
					if(TransCharset())
						m_pDom->AddAttrib(m_strKey,m_strTxt);
					else
						m_pDom->AddAttrib(m_strKey,m_strValue); 
				}
				m_pDom->Parent();
				m_pDom->FinishToken(XFALSE);
				//m_pDom->FinishToken();
				SetStatus(XPS_SKIP);
				return i+1;
			//case XQUEST:
			case XCR:
			case XCT:
			case XSPACE:
				if(!m_nSpaceCount)
					m_nSpaceCount++;
				break;
			case XEQU: //'='
				SetStatus(XPS_ATTVAL);
				switch(XEAB().Index(m_strKey))
				{
				case XEAB::HREF:
				case XEAB::SRC:
					 m_bURL=XTRUE;
					 break;
				}
				return i+1;
			case XLA://'>'
				//TransCharset();
				if(!m_strKey.IsEmpty())
				{
					if(TransCharset())
						m_pDom->AddAttrib(m_strKey,m_strTxt);
					else
						m_pDom->AddAttrib(m_strKey,m_strValue); 
				}
				m_pDom->FinishToken(XTRUE);
				SetStatus(XPS_TEXT);
				return i+1;
			}
		}
		return nSize;

}

inline int XDomPhaseXML::PhaseValue(XTCHAR *pData, int nSize)
{
	for(int i=0;i<nSize;i++)
	{
			switch(pData[i])
			{
			default:
				 if((XU8)pData[i]>XSPACE) 
					m_strValue+=pData[i];
				 break;
			case XSQO:
			case XQO:
				 m_nStrChar=pData[i];
				 SetStatus(XPS_STRING);
				 return (i+1);
			case XQUEST:
			case XCR:
			case XCT:
			case XSPACE:
				 if(!m_strValue.IsEmpty())
				 {
				  if(TransCharset())
					m_pDom->AddAttrib(m_strKey,m_strTxt);
				  else
					m_pDom->AddAttrib(m_strKey,m_strValue);
				  SetStatus(XPS_ATTNAME);
				  return (i+1);
				 }break;
			case XDIV:
				 if(m_bURL||m_bMustEnd)
				 {
					 m_strValue+=pData[i];
					 break;
				 }
				 //if(m_pParent==m_pItem)
				 if(TransCharset())
					m_pDom->AddAttrib(m_strKey,m_strTxt);
				 else
					m_pDom->AddAttrib(m_strKey,m_strValue);
				 SetStatus(XPS_SKIP);
				 m_pDom->FinishToken(XFALSE);
				 //if(m_pDom->IsScriptItem())
				 m_pDom->Parent();
				 return (i+1);
			case XLA:
				 if(TransCharset())
				 	 m_pDom->AddAttrib(m_strKey,m_strTxt);
				 else
					 m_pDom->AddAttrib(m_strKey,m_strValue);
				 m_pDom->FinishToken(XTRUE);
				 SetStatus(XPS_TEXT);
				 return (i+1);
				 break;
			}
	}
	return nSize;

}

inline int XDomPhaseXML::PhaseString(XTCHAR *pData, int nSize)
{
	//LOGE("%s, %s, %d, -->size:%d", __FILE__, __FUNCTION__, __LINE__, nSize );
	for(int i=0;i<nSize;i++)
	{
	 switch(pData[i])
		{
		
		case XSQO:
		case XQO://'"'
			 if(m_nStrChar!=pData[i])
			 {
				 m_strValue+=pData[i];
				 break;
			 }
			 m_stack.Pop(m_nStatus);
			 if(m_nStatus==XPS_ATTVAL)
			 {
				 //if(m_strValue=="仿真鼠标")
				//	 int v=0;
				 bool b = TransCharset()!=0;
				 if(b)
					 m_pDom->AddAttrib(m_strKey,m_strTxt);
				 else
					m_pDom->AddAttrib(m_strKey,m_strValue);
				 SetStatus(XPS_ATTNAME);
			 }
			 return (i+1);
		case XAND://'&'
			 //if(nSize>100&&strcmp(pData,"&amp;my_sig_uId"
			 //if(m_strValue.Find("http://my.hf.fminutes.com")==0)
			//	 int v=0;
			 if(i+1<nSize&&pData[i+1]!=XQO&&pData[i+1]!=XSQO)
				SetStatus(XPS_CHAR);
			 return (i+1);
		default:
			 m_strValue+=pData[i];
			 break;
		}
	}
	return nSize;
}

inline int XDomPhaseXML::PhaseChar(XTCHAR *pData, int nSize)
{
	for(int i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		//case XLE:
		case ' ':break;
		case XLA:
			 //m_strChar+=pData[i];
			 //i--;
			 //goto Continue;
		case XQO:
		case XSP:
		case XSQO:
			 m_stack.Pop(m_nStatus);
			 m_strValue+='&';
			 m_strValue+=m_strChar;
			 return i;
			 //i--;
		case '<':
		case XAND:i--;
		case XCR:
		case XCT:
		case XSPLIT:
			// if(nSize>100&&strcmp(pData,"amp;my_sig_sId=1692042")==0)
			//	 int v=0;
			 m_stack.Pop(m_nStatus);
			 if(m_strChar.GetLength()>1)
			 {
				TransCharset();
				if(m_strChar[0]==XSIGN)
				{
					XU16 v=m_strChar.ToInt();
					//if(v==0x2022)
					//	v=0x2022;
					m_strValue.AddWChar(v);
				}
				else
				{
					XWCHAR ch=XEnumAlias().Char(m_strChar);
					if(ch!=(XWCHAR)-1)
						m_strValue.AddWChar(ch);//,m_pDom->m_nCharset);
					//	m_strValue+=(XTCHAR)ch;
					else
					{
						m_strValue+=(XTCHAR)'&';
					    m_strValue+=m_strChar;
						if(pData[i+1]==XSPLIT)
							m_strValue+=XSPLIT;
					}
				}
				TransCharset(XTRUE);
			 }
			 return i+1;
		default:
			m_strChar+=pData[i];
			break;
		}
	}
	return nSize;

}

inline int XDomPhaseXML::PhaseEcho(XTCHAR *pData, int nSize)
{
	for(int i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		case XLA://'>'
			 if(m_strValue.GetLength()>=2)
			 {
				int id=m_strValue.GetLength()-2;
				if(m_strValue[id]=='-'&&
				   m_strValue[id+1]=='-')
				{
				   if(TransCharset())
					  m_pDom->SetData(m_strTxt);
				   else
					  m_pDom->SetData(m_strValue);
				   SetStatus(XPS_TEXT);
				   m_pDom->Parent();
				   return i+1;
				}
			 }
		default:
	 		 m_strValue+=pData[i];
			 break;
		}
	}
	return nSize;

}

inline int XDomPhaseXML::PhaseKeyData(XTCHAR *pData, int nSize,XBOOL bSkip)
{
	for(int i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		case XLA://'>'
			 if(!bSkip)
			 {
				 if(TransCharset())
					m_pDom->SetData(m_strTxt); 
				 else 
					m_pDom->SetData(m_strValue);
			 }
			 m_pDom->FinishToken(XFALSE);
			 SetStatus(XPS_TEXT);
			 return i+1;
		case XAND:
			 if(!bSkip)
				 SetStatus(XPS_CHAR);
			 break;
		case XSQO:
		case XQO:
			 if(!bSkip)
			 {
				 SetStatus(XPS_STRING);
				 m_nStrChar=pData[i];
				 return i+1;
			 }
			 break;
		default:
			 if(!bSkip)
				 m_strValue+=pData[i];
			 break;
		}
	}
	return nSize;

}

inline int XDomPhaseXML::PhaseScript(XTCHAR *pData, int nSize)
{
	int i;
	for( i=0;i<nSize;i++)
	{
	   m_strValue+=pData[i];
	   switch(pData[i])
	   {
	    case '<':if(m_nSpaceCount==0)
				 {
					m_nLength=m_strValue.GetLength()-1;
					m_nSpaceCount=1;
				 }
			     else m_nSpaceCount=0;
				 break;
		case '/':if(m_nSpaceCount==1)
				 {
					 m_nSpaceCount=2;
					 m_nKeyCount=0;
				 }
				 else m_nSpaceCount=0;
				 break;
		case ' ':break;
		default:if(m_nSpaceCount==1)
				  m_nSpaceCount=0;
			    else if(m_nSpaceCount)
				{
				   if(m_nKeyCount<10)//m_strKey.GetLength())
					 m_nKeyCount++;
				   else m_nSpaceCount=0;
				}
				break;
		case '>':
			if(m_nSpaceCount==2)//&&m_nKeyCount>=m_strKey.GetLength())
			{
				XPCTSTR strKey=m_pDom->IsScriptItem();
				XString8 str;
				m_strValue.Mid(str,m_strValue.GetLength()-m_nKeyCount-1,m_nKeyCount);
				if(str.Compare(strKey,XTRUE)==0)
				{
					//m_strValue.SetSize(m_nLength+1);
					m_strValue.GetData()[m_nLength]=0;
					if(TransCharset())
						m_pDom->SetData(m_strTxt);
					else
						m_pDom->SetData(m_strValue);
					m_pDom->Parent();
					m_pDom->FinishToken(XFALSE);
					SetStatus(XPS_TEXT);
					return i+1;
				}
			}
			m_nSpaceCount=0;
			break;
	   }
	   /*if(pData[i]=='>')
	   {
		 int id=m_strValue.ReverseFind('<');
		 if(id>=0&&id+(int)m_strKey.GetLength()+2<i&&
			m_strValue[id+1]==)
		 {
		   XString8 str=m_strValue.Mid(id+1,i-id-1);
		   str.TermLeft();
		   str.TermRight();
		 }
	   }*/
	}
	return i;
}

XBOOL XDomPhaseXML::TransCharset(XU8 bSet)
{
	//if(m_strValue.IsEmpty()) return;
	//m_strValue.ConvertFrom(m_pDom->m_nCharset);
	if(!NeedConvert()) return XFALSE;
	if(!m_strValue.IsEmpty())
	{
		if(!bSet)
		{
			if(m_strTxt.IsEmpty())
			{
				m_strTxt.ConvertFrom(m_strValue,m_pDom->m_nCharset);
			}
			else
			{
				m_strValue.ConvertFrom(m_pDom->m_nCharset);		
				m_strTxt+=m_strValue;
			}
		}
		else 
		{
			m_strTxt+=m_strValue;
		}
		m_strValue.Empty();
	}
	return XTRUE;
}

XINT XDomPhaseXML::IndexOf(XPCTSTR strKey,XU8 nType)
{
 	XSortEntry e;
	return e.Index((void*)strKey,nType);
}


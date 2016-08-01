// XWAPText.cpp: implementation of the XWAPText class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XWAPText.h"
#include "XEnumData.h"
#include "XDomPhase.h"
#include "XDomItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XWAPText::XWAPText()
{
   m_nMode=XW_EMPTY;
}

XWAPText::~XWAPText()
{

}

void XWAPText::Final()
{
   while(m_tokens.GetSize()>0)
	   EndToken();
}

XBOOL XWAPText::Create(XBOOL bWml,XU8 nCharset)
{
	if(m_nMode!=XW_EMPTY) return XFALSE;
	XPCTSTR pStr=XEnumCharset().Key(nCharset);
	if(pStr!=0)
	{
		AddString("<?xml version=\"1.0\" encoding=\"");
		AddString(pStr);
		AddString("\"?>");
	}
	m_nMode=XW_TEXT;
    AddToken(bWml?XDOM_WML:XDOM_HTML);
	Finish();
	return XTRUE;
}

XBOOL XWAPText::AddToken(XU16 nID)
{
   if(m_nMode==XW_EMPTY) return XFALSE;
   if(m_nMode==XW_TOKEN)
   {
	   AddString(">");
	   m_nMode=XW_TEXT;
   }
   XPCTSTR pStr=XDomPhaseXML::GetToken(nID);
   if(pStr==XNULL) return XFALSE;
//#if (__CORE_VERSION__>=0x02077000)
   XString8 strTmp(pStr);
   strTmp.MakeUpper();
   pStr=strTmp;
//#endif
   AddString("<");
   AddString(pStr);
   m_nMode=XW_TOKEN;
   m_tokens.Push(nID);
   return XTRUE;
}

XBOOL XWAPText::EndToken()
{
   XU16 id;
   if(!m_tokens.Pop(id)) return XFALSE;
   XPCTSTR pStr=XDomPhaseXML::GetToken(id);
//#if (__CORE_VERSION__>=0x02077000)
   XString8 strTmp(pStr);
   strTmp.MakeUpper();
   pStr=strTmp;
//#endif
   switch(m_nMode)
   {
   case XW_EMPTY:return XFALSE;
   case XW_TOKEN:
		AddString("/>");break;
   case XW_TEXT:
	    AddString("</");
		if(pStr!=XNULL) AddString(pStr);
		AddString(">");
		break;
   }   
   m_nMode=XW_TEXT;
   return XTRUE;
}

XBOOL XWAPText::AddText(XPCTSTR strTxt)
{
   if(m_nMode==XW_EMPTY) return XFALSE;
   Finish();
   if(strTxt!=XNULL) AddString(strTxt);
   return XTRUE;
}

XBOOL XWAPText::AddAttrib(XU16 nID, XPCTSTR strValue)
{
   //if(m_nMode!=XW_TOKEN) return XFALSE;
   XPCTSTR pStr=XEAB()[nID];
   if(pStr==XNULL) return XFALSE;
//#if (__CORE_VERSION__>=0x02077000)
   XString8 strTmp(pStr);
   strTmp.MakeUpper();
   pStr=strTmp;
//#endif
   return AddAttrib(pStr,strValue);
}

XBOOL XWAPText::Finish()
{
	if(m_nMode!=XW_TOKEN) return XFALSE;
	AddString(">");
	m_nMode=XW_TEXT;
	return XTRUE;
}

XBOOL XWAPText::AddAttrib(XPCTSTR pStr, XPCTSTR strValue)
{
	if(m_nMode!=XW_TOKEN) return XFALSE;
    AddString(" ");
	AddString(pStr);
	AddString("=\"");
	AddString(strValue);
	AddString("\"");
	return XTRUE;
}

XBOOL XWAPText::AddAttrib(XU16 nID, XINT nValue)
{
	XString8 str;
	str.FromInt(nValue);
	return AddAttrib(nID,str);
}

void XWAPText::AddLink(XPCTSTR strURL, XPCTSTR strTitle, XPCTSTR strImg)
{
	AddToken(XDOM_A);
	AddAttrib(XEAB::HREF,strURL);
	AddToken(XDOM_IMG);
	AddAttrib(XEAB::SRC,strImg);
	AddAttrib(XEAB::ALIGN,XEnumAlign()[XEA::MIDDLE]);
	EndToken();
	AddText(strTitle);
	EndToken();
}


XBOOL XWAPText::AddLink(XPCTSTR strURL, XPCTSTR strTitle,XU32 nPicks)
{
	AddToken(XDOM_A);
	AddAttrib(XEAB::HREF,strURL);
	if(nPicks>0)
		AddAttrib(XEAB::NAME,nPicks);
	AddText(strTitle);
	EndToken();
	return XTRUE;
}

//DEL XBOOL XWAPText::AddLink(XPCTSTR strURL, XPCWSTR strTitle,XU32 nPicks)
//DEL {
//DEL 	AddToken(XDOM_A);
//DEL 	AddAttrib(XEAB::HREF,strURL);
//DEL 	XString8 str(strTitle);
//DEL 	AddText(str);
//DEL 	EndToken();
//DEL 	return XTRUE;
//DEL }

void XWAPText::AddImage(XPCTSTR strURL, XPCTSTR strTitle)
{
    AddToken(XDOM_IMG);
	AddAttrib(XEAB::SRC,strURL);
	if(strTitle!=XNULL)
		AddAttrib(XEAB::ALT,strTitle);
	EndToken();
}

//DEL void XWAPText::AddBack()
//DEL {
//DEL 	AddToken(XDOM_ANCHOR);
//DEL 
//DEL }

void XWAPText::AddAttrib(XU16 id, XTime &time)
{
	XString8 str;
	time.GetTime(str);
	AddAttrib(id,str);
}

void XWAPText::AddAttrib(XU16 id, XString8Array &list)
{
	XString8 str;
	for(XU32 i=0;i<list.GetSize();i++)
	{
		if(!str.IsEmpty()) str+=';';
		str+=list[i];
	}
	AddAttrib(id,str);
}


void XWAPText::AddBR()
{
	AddToken(XDOM_BR);
	EndToken();
}

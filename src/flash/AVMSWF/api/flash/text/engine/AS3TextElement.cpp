#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3TextElement.h"
#include "AS3TextLine.h"

namespace avmshell{
TextElementClass::TextElementClass(VTable* cvtable):ClassClosure(cvtable)//ContentElementClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextElementClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextElementObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

TextElementObject* TextElementClass::CreateObject(Stringp strTxt,TextElementObject*oldObj)
{
	TextElementObject*pObj=(TextElementObject*)createInstance(ivtable(),prototype);
	pObj->AS3_text_set(strTxt);
	pObj->AS3_eventMirror_set(oldObj->m_pMirrorEvent);
	pObj->AS3_elementFormat_set(oldObj->m_pElementFormat);
	pObj->AS3_textRotation_set(oldObj->m_strRotation);
	return pObj;
}

bool TextElementObject::CreateContext(TextLineObject*pLine,int nID)
{
	//int nOldWidth=
	if(m_pCreationInfo==NULL)
	{
		if(m_pElementFormat==NULL)
		{
			ElementFormatClass*pClass=((ShellToplevel*)toplevel())->getElementFormatClass();
			m_pElementFormat=pClass->CreateObject();
		}
		this->InitCreationData();
	}
	int nMaxWidth=pLine->m_nMaxWidth-pLine->m_nTextWidth;
	pLine->m_pElement=this;
	pLine->m_nElementID=nID;
	if(nMaxWidth<=0) return false;
	int nEndID=this->FindEndPos(nID,nMaxWidth);
	if(nEndID<-1)
		return false;
	
	pLine->m_nElementID=nEndID;
	pLine->m_pElement=this;

	StUTF16String s16(m_strText->substring(nID,nEndID));
//	StUTF8String ss(m_strText->substring(nID,nEndID));
	//if(strstr(ss.c_str(),"hjjj"))
//		int v=0;
	pLine->AppendText((XU16*)s16.c_str(),s16.length(),m_pCreationInfo->pFormat,nMaxWidth);

	if(nEndID>=m_pCreationInfo->m_nLength)
	{
		ContentElementObject*next=this->GetNextElement();
		if(next)
			return next->CreateContext(pLine,0);
		pLine->m_pElement=NULL;
		return true;
	}

	return true;
}

TextElementObject::TextElementObject(VTable *vtable, ScriptObject* proto, int capacity): ContentElementObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nContentType=TYPE_TEXT;
}
//////////////////////////////////////////////////////////
//Native Method start...
//AvmBox TextElementObject::AS3_text_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

void TextElementObject::AS3_text_set(Stringp text)
{
	//Add your act code here...
	m_strText=text;
	m_strRawText=text;

//	StUTF16String ss(m_strRawText);
//	if(ss.c_str()[0]==0x2029)
//		int vv=0;
}

//AS3 contructor function..
//AvmBox TextElementObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

void TextElementObject::CombineText(TextElementObject*pTxt)
{
	if(pTxt->m_strText==NULL) return;
	if(m_strText==NULL)
		m_strText=pTxt->m_strText;
	else
		m_strText=String::concatStrings(m_strText,pTxt->m_strText);
	m_strRawText=m_strText;
//	StUTF16String ss(m_strRawText);
//	if(ss.c_str()[0]==0x2029)
//		int vv=0;
}

TextElementObject* TextElementObject::Split(int charIndex)
{
	if(m_strText==NULL||charIndex<0||charIndex>m_strText->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(charIndex),INTString(m_strText?m_strText->get_length():0));
	ShellToplevel*top=(ShellToplevel*)toplevel();
	Stringp strNew=m_strText->substring(charIndex);
	TextElementClass*pClass=top->getTextElementClass();
	TextElementObject*pObj=pClass->CreateObject(strNew,this);
	m_strText=m_strText->substring(0,charIndex);
	m_strRawText=m_strText;
//	StUTF16String ss(m_strRawText);
//	if(ss.c_str()[0]==0x2029)
//		int vv=0;
	return pObj;
}

void TextElementObject::AS3_replaceText(int beginIndex, int endIndex, Stringp newText)
{
	//Add your act code here...
	if(newText==NULL)
		m_strText=NULL;
	else if(m_strText==NULL)
		m_strText=newText;
	else if(beginIndex>=m_strText->get_length())
		m_strText=String::concatStrings(m_strText,newText);
	else if(beginIndex<=endIndex&&beginIndex>=0&&endIndex<=m_strText->get_length())
	{
		Stringp strTxt=NULL;
		if(beginIndex>0)
		{
			strTxt=m_strText->substring(0,beginIndex);
			strTxt=String::concatStrings(strTxt,newText);
		}
		else strTxt=newText;
		if(endIndex<m_strText->get_length())
		{
			Stringp str=m_strText->substring(endIndex);
			strTxt=String::concatStrings(strTxt,str);
		}
		m_strText=strTxt;
			//strTxt=String::concatStrings(
		//Stringp strLeft=m_strText->substring(0,beginIndex);
	}
	else
		toplevel()->throwRangeError(kInvalidRangeError,INTString(beginIndex),INTString(endIndex));
	m_strRawText=m_strText;
//	StUTF16String ss(m_strRawText);
//	if(ss.c_str()[0]==0x2029)
//		int vv=0;
}

}
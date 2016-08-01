#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;

#include "AS3TextLine.h"
#include "AS3TextBlock.h"
#include "AS3TextRotation.h"
#include "AS3SpaceJustifier.h"
#include "AS3TextLineValidity.h"

namespace avmshell{
TextBlockClass::TextBlockClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextBlockClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextBlockObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextBlockObject::TextBlockObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_bApplyNonLinearFontScaling=false;
	m_fBaseLineFontSize=12;
	m_nBidiLevel=0;
	
	ShellToplevel*top=(ShellToplevel*)toplevel();
	SpaceJustifierClass*pClass=(SpaceJustifierClass*)top->getBuiltinExtensionClass(abcclass_flash_text_engine_SpaceJustifier);
	if(pClass)
		m_pTextJustifier=(TextJustifierObject*)pClass->CreateObject();
}
//////////////////////////////////////////////////////////
//Native Method start...
int TextBlockObject::TypeOfChar(const wchar c)
{
	if(c>='0'&&c<='9') return CHAR_NUM;
	else if(c>='a'&&c<='z') return CHAR_ALPHA;
	else if(c>='A'&&c<='Z') return CHAR_ALPHA;
	return CHAR_SIGN;
}
bool	TextBlockObject::AS3_applyNonLinearFontScaling_get()
{
	return m_bApplyNonLinearFontScaling!=0;
	//return false;
}

void	TextBlockObject::AS3_applyNonLinearFontScaling_set(bool value)
{
	m_bApplyNonLinearFontScaling=value;
}

FontDescriptionObject*	TextBlockObject::AS3_baselineFontDescription_get()
{
	return m_pFontDesc;
}

void	TextBlockObject::AS3_baselineFontDescription_set(FontDescriptionObject *pValue)
{
	m_pFontDesc=pValue;
}

double	TextBlockObject::AS3_baselineFontSize_get()
{
	return m_fBaseLineFontSize;
}

void	TextBlockObject::AS3_baselineFontSize_set(double value)
{
	m_fBaseLineFontSize=value;
}

Stringp	TextBlockObject::AS3_baselineZero_get()
{
	if(m_strBaselineZero==NULL)
	{
		TextBaselineClass*pClass=((ShellToplevel*)toplevel())->getTextBaselineClass();
	}
	return m_strBaselineZero;
}

void	TextBlockObject::AS3_baselineZero_set(Stringp value)
{
	m_strBaselineZero=value;
}

ContentElementObject*	TextBlockObject::AS3_content_get()
{
	return m_pContent;
}

void	TextBlockObject::AS3_content_set(ContentElementObject *pValue)
{
	if(m_pContent)
		m_pContent->m_pBlock=NULL;
	m_pContent=pValue;
	if(m_pContent)
		m_pContent->m_pBlock=this;
	m_pFirstInvalidLine=m_pFirstLine;
}

int		TextBlockObject::AS3_bidiLevel_get()
{
	return m_nBidiLevel;
}

void	TextBlockObject::AS3_bidiLevel_set(int value)
{
	m_nBidiLevel=value;
}

TextLineObject*	TextBlockObject::AS3_firstInvalidLine_get()
{
	return m_pFirstInvalidLine;
}

TextLineObject*	TextBlockObject::AS3_firstLine_get()
{
	return m_pFirstLine;
}

TextLineObject*	TextBlockObject::AS3_lastLine_get()
{
	return m_pLastLine;
	//return NULL;
}

TextJustifierObject*	TextBlockObject::AS3_getTextJustifier()
{
	return m_pTextJustifier;
}

void	TextBlockObject::AS3_setTextJustifier(TextJustifierObject *pValue)
{
	m_pTextJustifier=pValue;
}

Stringp	TextBlockObject::AS3_textLineCreationResult_get()
{
	return m_strResult;
}

Stringp	TextBlockObject::AS3_lineRotation_get()
{
	if(m_strLineRotation==NULL)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		TextRotationClass*pClass=(TextRotationClass*)top->getBuiltinExtensionClass(abcclass_flash_text_engine_TextRotation);
		if(pClass)
			m_strLineRotation=pClass->getROTATE_0();
	}
	return m_strLineRotation;
}

void	TextBlockObject::AS3_lineRotation_set(Stringp value)
{
	m_strLineRotation=value;
}

ObjectVectorObject*	TextBlockObject::AS3_getTabStops()
{
	return m_pTabStops;
}

void	TextBlockObject::AS3_setTabStops(ObjectVectorObject *pValue)
{
	m_pTabStops=pValue;
}

int		TextBlockObject::AS3_findNextAtomBoundary(int afterCharIndex)
{
	if(this->m_pContent==NULL)
		toplevel()->throwRangeError(kInvalidRangeError,INTString(afterCharIndex),INTString(0));
	Stringp strRaw=m_pContent->m_strRawText;
	if(afterCharIndex<0||afterCharIndex>=strRaw->get_length()-1)
		toplevel()->throwRangeError(kInvalidRangeError,INTString(afterCharIndex),INTString(0));
	return afterCharIndex+1;
	//return NULL;
}

int		TextBlockObject::AS3_findPreviousAtomBoundary(int beforeCharIndex)
{
	if(this->m_pContent==NULL)
		toplevel()->throwRangeError(kInvalidRangeError,INTString(beforeCharIndex),INTString(0));
	Stringp strRaw=m_pContent->m_strRawText;
	if(beforeCharIndex<1||beforeCharIndex>=strRaw->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(beforeCharIndex),INTString(0));
	return beforeCharIndex-1;
}



int		TextBlockObject::AS3_findNextWordBoundary(int afterCharIndex)
{
	if(this->m_pContent==NULL)
	toplevel()->throwRangeError(kInvalidRangeError,INTString(afterCharIndex),INTString(0));
	Stringp strRaw=m_pContent->m_strRawText;
	if(afterCharIndex<0||afterCharIndex>=strRaw->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(afterCharIndex),INTString(0));
	StUTF16String st(strRaw);
	int i;
	const wchar* chars=st.c_str();
	int nType=TypeOfChar(chars[afterCharIndex]);
	if(nType==CHAR_SIGN) return afterCharIndex+1;
	for(i=afterCharIndex+1;i<st.length();i++)
	{
		int nNewType=TypeOfChar(chars[i]);
		switch(nNewType)
		{
		case CHAR_ALPHA:
			 if(nType!=CHAR_ALPHA)
				 return i;
			 break;
		case CHAR_NUM:
			 if(nType!=CHAR_NUM)
				 return i;
			 break;
		default:
			 return i;
		}
		//if(chars[i]==65007) return i;
	}
	return st.length();
	//return 0;
}

int		TextBlockObject::AS3_findPreviousWordBoundary(int beforeCharIndex)
{
	//if(beforeCharIndex>2000)
	//	int v=0;
	if(this->m_pContent==NULL)
	toplevel()->throwRangeError(kInvalidRangeError,INTString(beforeCharIndex),INTString(0));
	Stringp strRaw=m_pContent->m_strRawText;
	if(beforeCharIndex<1||beforeCharIndex>=strRaw->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(beforeCharIndex),INTString(0));
	StUTF16String st(strRaw);
	int i;
	const wchar* chars=st.c_str();
	int nType=TypeOfChar(chars[beforeCharIndex]);
	if(nType==CHAR_SIGN) return beforeCharIndex-1;
	for(i=beforeCharIndex-1;i>=0;i--)
	{
		int nNewType=TypeOfChar(chars[i]);
		switch(nNewType)
		{
		case CHAR_ALPHA:
			 if(nType!=CHAR_ALPHA)
				 return i;
			 break;
		case CHAR_NUM:
			 if(nType!=CHAR_NUM)
				 return i;
			 break;
		default:
			 return i;
		}
		//if(chars[i]==65007) return i;
	}	
	return 0;
}

TextLineObject*	TextBlockObject::AS3_getTextLineAtCharIndex(int charIndex)
{
	TextLineObject*pLine=m_pFirstLine;
	while(pLine)
	{
		if(pLine->m_nStartChar<=charIndex&&
			pLine->m_nEndChar>charIndex)
			return pLine;
		pLine=pLine->m_pNext;
	}
	return NULL;
}

TextLineObject*	TextBlockObject::AS3_DoCreateTextLine(TextLineObject *pPreviousLine, double width, double lineOffset, bool fitSomething, TextLineObject *pReuseLine)
{
	if(!m_pContent) return NULL;
//	if(width>=800)
//		int v=0;
	ContentElementObject*pCnt=m_pContent;
	int nID=0,m_nStartChar=0;
	if(pPreviousLine)
	{
		pCnt=pPreviousLine->m_pElement;
		nID=pPreviousLine->m_nElementID;
		m_nStartChar=pPreviousLine->m_nEndChar;
	}
	if(!pCnt) return NULL;

	ShellToplevel*top=(ShellToplevel*)toplevel();
	TextLineClass*pClass=top->getTextLineClass();
	TextLineObject*pObj=pClass->CreateObject();
	pObj->m_nMaxWidth=width;
	pObj->m_pBlock=this;
	pObj->m_nStartChar=m_nStartChar;
	pObj->m_nEndChar=m_nStartChar;
	//pObj->m_pElement=pCnt;
	//pObj->m_nElementID=nID;
	pCnt->CreateContext(pObj,nID);
	pObj->SetBounds();

	//if(pObj->m_nEndChar-pObj->m_nStartChar<4)
	//	int v=0;

	if(m_pFirstLine==NULL)
	{
		m_pFirstLine=pObj;
		m_pLastLine=pObj;
	}
	else
	{
		pObj->m_pPrev=m_pLastLine;
		m_pLastLine->m_pNext=pObj;
		m_pLastLine=pObj;
	}

	return pObj;
}

void	TextBlockObject::AS3_releaseLineCreationData()
{
	if(m_pContent)
		m_pContent->ReleaseCreationData();
}

void	TextBlockObject::AS3_releaseLines(TextLineObject *pFirstLine, TextLineObject *pLastLine)
{
	if(pFirstLine==NULL||
	   pLastLine==NULL) return;

	TextLineObject*pLine=m_pFirstLine;
	bool bFirst=false,bLast=false;
	while(pLine)
	{
		if(pLine==pFirstLine)
			bFirst=true;
		/*else*/ if(pLine==pLastLine)
			bLast=true;
		TextLineObject*pNext=pLine->m_pNext;
		//pLine->m_pBlock=NULL;
		//pLine->m_pNext=NULL;
		pLine=pNext;
	}
	if(!bFirst||!bLast) return;
	if(m_pFirstLine==pFirstLine)
		m_pFirstLine=pLastLine->m_pNext;
	if(m_pLastLine==pLastLine)
		m_pLastLine=pFirstLine->m_pPrev;
	pLine=pFirstLine;
	TextLineValidityClass*pClass=((ShellToplevel*)toplevel())->getTextLineValidityClass();
	bool bClear=true;
	TextLineObject*pStart=pFirstLine->m_pPrev;
	TextLineObject*pEnd=pLastLine->m_pNext;
	while(pLine)
	{
		TextLineObject*pNext=pLine->m_pNext;
		if(bClear)
		{
			pLine->m_pBlock=NULL;
			pLine->m_pPrev=NULL;
			pLine->m_pNext=NULL;
		}
		pLine->m_strInvalid=pClass->getINVALID();
		if(pLine==pLastLine) 
		{
			bClear=false;
			break;
		}
		pLine=pNext;
	}
	if(pStart) pStart->m_pNext=pEnd;
	if(pEnd)   pEnd->m_pPrev=pStart;
	/*if(pFirstLine->m_pPrev)
	{
		TextLineObject*pStart=pFirstLine->m_pPrev;
		pStart->m_pNext=pLastLine->m_pNext;
	}*/
	//m_pFirstLine=NULL;
	//m_pLastLine=NULL;
	this->m_pFirstInvalidLine=NULL;
}

Stringp	TextBlockObject::AS3_dump()
{
	Stringp strTxt=core()->newConstantStringLatin1("<block>");

	TextLineObject*pLine=this->m_pFirstLine;
	while(pLine)
	{
		strTxt=String::concatStrings(strTxt,pLine->AS3_dump());
		pLine=pLine->AS3_nextLine_get();
	}

	strTxt=String::concatStrings(strTxt,core()->newConstantStringLatin1("</block>"));
	return strTxt;
	//return NULL;
}
}
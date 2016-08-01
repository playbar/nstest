#include "StdAfxflash.h"
#include "ShellCore.h"
#include "avm2.h"
#include "XXVar.h"
#include "splayer.h"

using namespace avmplus;
#include "AS3GroupElement.h"
#include "AS3TextBlock.h"
#include "AS3ContentElement.h"
#include "AS3TextRotation.h"

namespace avmshell{
//#ifdef DOTFONT
//inline int WidthToDot(int w,int&nWidth)
//{
//	const int nDotWidth[]=
//	{0,0,0,0,0,0,//<6
//	 0,0,0,12,	 //6,7,8,9
//	 12,12,12,12,//10,11,12,13,
//	 12,16,16,16,16};//14,15,16,17,18
//	//w=(w*scale)>>16;
//	nWidth=w;
//	if(w>18) return 0;
//	return nDotWidth[w];
//}
//
////inline FontCharInfo* GetFont(int w,XSWFPlayer*player,int&fontSize)
////{
////	switch(w)
////	{
////	//case 8:
////	//	fontSize=8;
////	//	return &player->dotFont8.font;break;
////	case 12:
////		fontSize=12;
////		return &player->dotFont12.font;break;
////	case 16:
////		fontSize=16;
////		return &player->dotFont16.font;break;
////	}
////	return NULL;
////}
//inline int charPixel(int nCode,FontCharInfo*pInfo)
//{
//	XU8* pData=(XU8*)(((XU16*)pInfo->infoData)+pInfo->nGlyphs);
//	return pData[nCode];
//	//return w;
//}
//#endif
ContentElementClass::ContentElementClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ContentElementClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ContentElementObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ContentElementObject::ContentElementObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nContentType=TYPE_NONE;
	m_nBeginIndex=-1;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	TextRotationClass*pClass=top->getTextRotationClass();
	m_strRotation=pClass->getROTATE_0();
	m_pCreationInfo=NULL;
}

void ContentElementObject::Release()
{
	if(IsRunning())
	{
		ReleaseCreationData();
	}
}

void ContentElementObject::ReleaseCreationData()
{
	if(m_pCreationInfo==NULL) return;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->display.FreeFormat(m_pCreationInfo->pFormat);
	XXVar::_FreeData(m_pCreationInfo->pCharsInfo);
	XXVar::_FreeData(m_pCreationInfo);
	//XXVa::_FreeData(m_pCreationInfo->pFormat);

		
}

int ContentElementObject::GetLineWidth(int nStart,int nEnd)
{
	if(m_pCreationInfo==NULL) return 0;
	int nWidth=0;
	for(int i=nStart;i<nEnd;i++)
	{
		nWidth+=m_pCreationInfo->pCharsInfo[i].charWidth;
	}
	return nWidth/20;
}

int ContentElementObject::FindEndPos(int nStart,int& nWidth)
{
	if(!m_pCreationInfo) return -1;
	int nTotal=0,nMax=nWidth*20;
	for(int i=nStart;i<m_pCreationInfo->m_nLength;i++)
	{
		nTotal+=m_pCreationInfo->pCharsInfo[i].charWidth;
		if(nTotal>nMax)
		{
			if(i>nStart) 
			{
				nWidth=nTotal/20;
				return i;
			}
			else if(i==nStart)
			{
				nWidth=nTotal/20;
				return i+1;
			}
			else
			{
				return -1;
			}
		}
	}
	if(nTotal<=nMax)
	{
		nWidth=nTotal/20;
		return m_pCreationInfo->m_nLength;
	}

	/*int nEnd=m_pCreationInfo->m_nLength,nMid=(nStart+nEnd)/2;
	if(nEnd==nStart) return -1;
	int nBegin=nStart;

	for(;;)
	{
		int w=GetLineWidth(nStart,nEnd);
		if(w==nWidth) return nEnd;
		else if(w>nWidth)
			nEnd=nMid;
		else
			nBegin=nMid;
		nMid=(nBegin+nEnd)/2;
		
	}*/

	/*int nEnd=m_pCreationInfo->m_nLength;
	int nOldW=0;
	for(;;)
	{
		int w=GetLineWidth(nStart,nEnd);
		if(w<=nWidth) 
		{
			if(nOldW=nWidth)
			{
				nWidth=w;
				return nEnd;
			}
			else
			{

			}
		}
		//nOldEnd=nEnd;
		//nOldW=w;
		//nEnd=nStart+(nEnd-nStart)/2;
		if(nEnd==nStart) return -1;
	}*/
	return -1;
}

void ContentElementObject::InitCreationData()
{
	if(m_pCreationInfo) return;
	if(m_strText==NULL||!m_pElementFormat) return;
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	if(!player->LoadBuiltInFonts())
		return;
	m_pCreationInfo=(_CONTENTINFO*)XXVar::_AllocData(sizeof(_CONTENTINFO));
	if(!m_pCreationInfo)
	{
		c->throwExtError(ExtErrorContext::kExtError_MemoryError,c->kSpaceString);
		return;
	}
	StUTF16String s16(m_strText);
	m_pCreationInfo->pCharsInfo=(_CHARINFO*)XXVar::_AllocData(s16.length()*sizeof(_CHARINFO));
	if(m_pCreationInfo->pCharsInfo==NULL)
	{
		XXVar::_FreeData(m_pCreationInfo);
		m_pCreationInfo=NULL;
		c->throwExtError(ExtErrorContext::kExtError_MemoryError,c->kSpaceString);
		return;
	}
	m_pCreationInfo->pFormat=player->display.CreateFormat();
	if(m_pCreationInfo->pFormat==NULL)
	{
		XXVar::_FreeData(m_pCreationInfo->pCharsInfo);
		XXVar::_FreeData(m_pCreationInfo);
		m_pCreationInfo=NULL;
		c->throwExtError(ExtErrorContext::kExtError_MemoryError,c->kSpaceString);
		return;
	}
	_XTEXTFORMAT*fmt=m_pCreationInfo->pFormat;
	fmt->align=stextAlignLeft;
	fmt->blockIndent=0;
	fmt->Bold=false;
	fmt->bullet=false;
	XU32 nAlpha=(int)(m_pElementFormat->m_alpha*255);
	fmt->color=(nAlpha<<24)|m_pElementFormat->m_nColor;
#ifdef _ARGB_PIXEL_
	
#else
	SWAPPIXELRB(fmt->color);
#endif
	fmt->indent=false;
	fmt->italic=false;
	fmt->kerning=0;//m_pElementFormat->m_n
	fmt->leading=0;
	fmt->leftMargin=0;
	fmt->rightMargin=0;
	fmt->dotFont=NULL;
	fmt->size=m_pElementFormat->m_nFontSize*20;
	fmt->underline=false;

	EditText::FontDesc fontDesc;
	XGlobal::Memset(&fontDesc,0,sizeof(fontDesc));


	U16 fontTag = 1;
	fontDesc.font=XNULL;

	fontDesc.font=&player->buildInFont;
	//if(!player->LoadFont((XU16*)s16.c_str()))
	//	fontDesc.font=XNULL;
		//player = player->splayer->builtInFontsPlayer;

	if ( fontTag )
	{
		// Find the font character
		SCharacter* font = fontDesc.font;

		char fontName[256]="";

		if ( font )
		{
			// Get the font name
			if ( font->font.deviceState==textDync)
			{
				fontDesc.boldFlag = 0;
				fontDesc.italicFlag = 0;
				fontDesc.japaneseFlag = 0;
			}

			fontDesc.codeOffset=0;
		}
	}

	// No font info in the text field tag, so we'll have to use defaults.
	strcpy(fontDesc.fontName, "Times New Roman");	
	int i,nc=s16.length();
	m_pCreationInfo->m_nLength=nc;
	XU16* codes=(XU16*)s16.c_str();
	_CHARINFO* charsInfo=m_pCreationInfo->pCharsInfo;
	XU8* advanceTable=(XU8*)fontDesc.font->font.GetCodes();//.GetAdvance();
	int fontSize=m_pCreationInfo->pFormat->size;
//#ifdef DOTFONT
//	int nFontWidth=0,nFontSize=0;
//	//int nPixel=0;
//	//if(player->bDotFontOK)
//	//	nPixel=WidthToDot(fontSize/20,nFontWidth);
//#endif
	for(i=0;i<nc;i++)
	{
		charsInfo[i].charCode=codes[i];
		charsInfo[i].charGraph=EditText::FindGlyph(codes[i],fontDesc);


		U16 code = charsInfo[i].charGraph;
		U16 advance;
		if ( code == 0xffff )//Invalid Code
		{
			advance = 0;
		}
		else
		{
			if(code<256)
			{
				advance = (U16)advanceTable[code*2] | ((U16)advanceTable[code*2+1]<<8);
				advance = advance * fontSize / 1024;
			}
			else 
				advance = fontSize;
		}
		charsInfo[i].charWidth=advance;

	}

}

ContentElementObject* ContentElementObject::GetNextElement()
{
		if(!m_pGroup) return NULL;
		return m_pGroup->GetNext(this);
}

bool ContentElementObject::CreateContext(TextLineObject*pLine,int nID)
{
	ContentElementObject*pNext=GetNextElement();
	if(!pNext) return false;
	return pNext->CreateContext(pLine,nID);
}

//////////////////////////////////////////////////////////
//Native Method start...
ElementFormatObject* ContentElementObject::AS3_elementFormat_get()
{
	//Add your act code here...
	return m_pElementFormat;//Modify this please!
}

void ContentElementObject::AS3_elementFormat_set(ElementFormatObject *pElementFormat)
{
	//Add your act code here...
	//if(m_pElementFormat)
	//	m_pElementFormat->AS3_locked_set(true))
	m_pElementFormat=pElementFormat;
}

EventDispatcherObject* ContentElementObject::AS3_eventMirror_get()
{
	//Add your act code here...
	return m_pMirrorEvent;//Modify this please!
}

void ContentElementObject::AS3_eventMirror_set(EventDispatcherObject *pEventMirror)
{
	//Add your act code here...
	m_pMirrorEvent=pEventMirror;
}

GroupElementObject* ContentElementObject::AS3_groupElement_get()
{
	//Add your act code here...
	return m_pGroup;//Modify this please!
}

Stringp ContentElementObject::AS3_rawText_get()
{
	//Add your act code here...
	return m_strRawText;//Modify this please!
}

Stringp ContentElementObject::AS3_text_get()
{
	//Add your act code here...
	return m_strText;//Modify this please!
}

TextBlockObject* ContentElementObject::AS3_textBlock_get()
{
	//Add your act code here...
	return m_pBlock;//Modify this please!
}

int ContentElementObject::AS3_textBlockBeginIndex_get()
{
	//Add your act code here...

	return m_nBeginIndex;//Modify this please!
}

Stringp ContentElementObject::AS3_textRotation_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void ContentElementObject::AS3_textRotation_set(Stringp textRotation)
{
	//Add your act code here...
	m_strRotation=textRotation;
}

//AvmBox ContentElementObject::AS3_userData_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox ContentElementObject::AS3_userData_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
////AS3 contructor function..
//AvmBox ContentElementObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}
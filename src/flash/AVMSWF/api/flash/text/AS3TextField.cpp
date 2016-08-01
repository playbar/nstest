#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "sobject.h"
#include "edittext.h"
#include "avm2.h"
#include "AS3TextField.h"
#include "AS3AntiAliasType.h"
#include "AS3TextFieldAutoSize.h"
#include "AS3TextFormat.h"
#include "AS3StyleSheet.h"
#include "AS3TextFieldType.h"
#include "AS3Rectangle.h"
#include "AS3TextLineMetrics.h"
#include "AS3ApplicationDomain.h"
#include "AS3Loader.h"
#include "splayer.h"
//#define _DEBUGBREAK_

namespace avmshell{
TextFieldClass::TextFieldClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextFieldObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextFieldClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextFieldObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}


TextFieldObject::TextFieldObject(VTable *vtable, ScriptObject* proto, int capacity): InteractiveObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
	//StUTF8String utf8("pixel");
	//char* s = "pixel";
	//strcpy(m_strFitType,s);
	//strcpy( (char*)m_strFitType->GetBuffer()->p8, "pixel" );
	//m_strFitType = utf8;

	//Add your act code here...

	m_strFitType = String::createLatin1(vtable->core(),"pixel", String::Length("pixel"), String::k8, true);;
 
	m_strRestrict=NULL;
	m_pSheet=NULL;
//	m_nWidth=0;
	m_nWidth0=0;
	m_nHeight0=0;
	m_nPosX=0;
	m_bPosX=false;
//#if (__CORE_VERSION__>=0x02079000)
//	m_bHtmlNewLine=false;
//#endif
}

TextFieldObject::~TextFieldObject()
{
}


void TextFieldObject::InitObject()
{
	if(m_pObject==NULL)
	{

		DisplayObjectObject::CreateNewObject(editTextChar);//CreateNewObject(
	}
}
//////////////////////////////////////////////////////////
//Native Method start...
inline EditText* TextFieldObject::GetEditObject()
{
	if(!m_pObject) return NULL;
	if(m_pObject->character->type!=editTextChar&&
	  m_pObject->character->type!=textLineChar) return NULL;
	return m_pObject->editText;
}

bool TextFieldObject::AS3_alwaysShowSelection_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
	{
		return pEdit->IsAlwaysShowSelection();
	}
	else return false;
	//return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_alwaysShowSelection_set(bool b)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
		pEdit->SetAlwaysShowSelection(b);
}

Stringp TextFieldObject::AS3_antiAliasType_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	
	if(pEdit)
	{
		AntiAliasTypeClass*pClass=((ShellToplevel*)toplevel())->getAntiAliasTypeClass();
		switch(pEdit->m_nAntiType)
		{
		case EditText::ANTITYPE_NORMAL:
			return pClass->getSlotNORMAL();
		case EditText::ANTITYPE_ADVANCED:
			return pClass->getSlotADVANCED();			 
		}
	}
	return NULL;
}

void TextFieldObject::AS3_antiAliasType_set(Stringp strType)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
	{
		AntiAliasTypeClass*pClass=((ShellToplevel*)toplevel())->getAntiAliasTypeClass();
		if(strType->equals(pClass->getSlotNORMAL()))
			pEdit->m_nAntiType=EditText::ANTITYPE_NORMAL;
		else if(strType->equals(pClass->getSlotADVANCED()))
			pEdit->m_nAntiType=EditText::ANTITYPE_ADVANCED;
	}
}

Stringp TextFieldObject::AS3_autoSize_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	EditText*pEdit=GetEditObject();
	
	if(pEdit)
	{
		TextFieldAutoSizeClass*pClass=((ShellToplevel*)toplevel())->getTextFieldAutoSizeClass();
		if(!pEdit->IsAutoSize()) return pClass->getSlotNONE();
		return pClass->ToString(pEdit->m_autoMode);//m_pFormat->align);		
	}
	return NULL;
}

void TextFieldObject::AS3_autoSize_set(Stringp a)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
	{
//		if(m_pObject->name.strTxt&&
//			strcmp(m_pObject->name.strTxt,"title_txt")==0)
//			int v=0;
		//if(pEdit->m_length==0)
		//	int v=0;
		//if(m_pObject->name.strTxt&&strcmp(m_pObject->name.strTxt,"_Info")==0)
		//	int v=0;
		TextFieldAutoSizeClass*pClass=((ShellToplevel*)toplevel())->getTextFieldAutoSizeClass();
		XBOOL bAutoSize=pEdit->IsAutoSize();
		if(!pEdit->m_length)
			SetWidth(0,bAutoSize);
//#if (__CORE_VERSION__>=0x02075000)
		int nID=(int)pClass->ToID(a);
		if(nID<0) 
//#if (__CORE_VERSION__>=0x02077000)
			nID=0;
//#else
//			return;			
//#endif
		pEdit->m_autoMode=nID;
//#else
//		pEdit->m_autoMode=(U8)pClass->ToID(a);
//#endif
		pEdit->AutoSize(pEdit->m_autoMode!=stextAlignJustify);
		if(/*pEdit->m_length&&*/(pEdit->IsAutoSize()||bAutoSize!=pEdit->IsAutoSize()))
		{
			m_pObject->Modify();
			m_pObject->FreeCacheBmp();
			if(pEdit->IsAutoSize())
			{
				CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth,m_nWidth0));
//#if (__CORE_VERSION__>=0x02077000)
//#elif (__CORE_VERSION__>=0x02075000)
//				if(!m_nWidth0)
//					m_nWidth0=100;
//#endif
			}
			else
			{
				CalcAutoSize(m_nWidth0,false);
				SRECT*rect=&m_pObject->editText->editRect;//character->bounds;
				if(RectIsEmpty(rect))
				{
					rect->xmin=0;
					rect->ymin=0;
					rect->xmax=0;
					rect->ymax=0;
				}
				if(m_nHeight0)
					rect->ymax=rect->ymin+m_nHeight0*20;
//#if (__CORE_VERSION__>=0x02076000)
				else
					rect->ymax=rect->ymin+100*20;
//#endif
				if(m_nWidth0>20)
					rect->xmax=rect->xmin+m_nWidth0*20;
//#if (__CORE_VERSION__>=0x02076000)
				else
					rect->xmax=rect->xmin+100*20;
//#endif
//				if(rect->ymax<0)
//					int vv=0;
/*#ifdef _WINEMU
				if(rect->xmax-rect->xmin<100)
					int vvv=0;
#endif*/
			}
			//CalcAutoSize(XMAX(m_nWidth,m_nWidth0));

		}
		//if(m_pObject->editText->editRect.xmin==2340)
		//	int vv=0;
	}

}

bool TextFieldObject::AS3_background_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
		return pEdit->IsBackground()!=0;
	return false;
}

void TextFieldObject::AS3_background_set(bool b)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
		pEdit->SetBackground(b);
}

uint32_t TextFieldObject::AS3_backgroundColor_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
//#if (__CORE_VERSION__>=0x02077000)
	if(pEdit)
		return pEdit->GetBackColor()&0x00ffffff;
//#else
//	if(pEdit)
//		return pEdit->GetBackColor();
//#endif
	return 0;
}

void TextFieldObject::AS3_backgroundColor_set(uint32_t c)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
//#if (__CORE_VERSION__>=0x02081000)
//#else 
//#if (__CORE_VERSION__>=0x02077000)
//		c|=0xff000000;
//#endif
//#endif
	if(pEdit)
		pEdit->SetBackColor(c);
}

bool TextFieldObject::AS3_border_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
		return pEdit->IsBorder()!=0;
	return false;
}

void TextFieldObject::AS3_border_set(bool b)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
		pEdit->SetBorder(b);
}

uint32_t TextFieldObject::AS3_borderColor_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
	{
		uint32_t color = pEdit->GetBorderColor();
		return color &0xFFFFFF;
		//return pEdit->GetBorderColor();
	}
	return 0;
}

void TextFieldObject::AS3_borderColor_set(uint32_t c)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
	{
		c = 0xFF000000 | c;
		pEdit->SetBorderColor(c);
	}
}

int TextFieldObject::AS3_bottomScrollV_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
	{
//#if (__CORE_VERSION__>=0x02079000)
		int iLine=pEdit->CalcVisibleLines(pEdit->m_vscroll)+pEdit->m_vscroll;//CalcVisibleLines(pEdit->m_);//CalcMaxVScroll();
		if(iLine<1) 
			iLine=1;
		LOGWHEREVAL( iLine );
		return iLine;
//#else
//		return pEdit->CalcVisibleLines(pEdit->m_vscroll)+pEdit->m_vscroll;//CalcVisibleLines(pEdit->m_);//CalcMaxVScroll();
//#endif
	}
	return 0;
}

int TextFieldObject::AS3_caretIndex_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
	{
		LOGWHEREVAL(pEdit->m_caretIndex);
		return pEdit->m_caretIndex;//m_selectionStart;
	}
	return 0;
}

bool TextFieldObject::AS3_condenseWhite_get()
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
		return pEdit->IsCondenseWhite();
	return false;
}

void TextFieldObject::AS3_condenseWhite_set(bool b)
{
	//Add your act code here...
	EditText*pEdit=GetEditObject();
	if(pEdit)
		pEdit->SetCondenseWhite(b);
}

TextFormatObject* TextFieldObject::AS3_defaultTextFormat_get()
{
	//Add your act code here...
	if(m_pObject)
	{
		TextFormatClass*pClass=((ShellToplevel*)toplevel())->getTextFormatClass();
		TextFormatObject*pFmt=(TextFormatObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		pFmt->SetFormat(m_pObject->editText->GetDefaultFormat());
		return pFmt;
	}
	else
		return NULL;
	//return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_defaultTextFormat_set(TextFormatObject*pFmt)
{
	//Add your act code here...
	//return;
	if(pFmt==XNULL) return;
	if(m_pObject)
	{
#ifdef _DEBUGBREAK_
		/*{
			XString8 s;
			s.ConvertFrom("��ӭ",XCHARSET_GB2312);
			XString8 ss(m_pObject->editText->m_buffer);
			if(ss.Find(s)==0)
				int v=0;
		}*/
#endif
		//m_pObject->FreeCacheBmp();
		_XTEXTFORMAT*fmt=m_pObject->editText->GetDefaultFormat();
		//May be crash
		pFmt->GetFormat(fmt);
		m_pObject->editText->SetFilterSize(fmt);
		if(m_pObject->editText->m_bURL)
			m_pObject->editText->SetURLFlag();
		else if(!fmt->url.IsNull())
			m_pObject->editText->m_bURL=true;
		m_pObject->editText->m_bModifyHTML=true;
		if(m_pObject->editText->m_length)
		{
			m_pObject->Modify();
			//m_pObject->FreeCacheBmp();
			if(m_pObject->editText->IsAutoSize())
				CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth,m_nWidth0));
			else
				//m_pObject->editText->UpdateIfNeed();
				CalcAutoSize(m_nWidth0,false);//XMAX(m_nWidth,m_nWidth0));
		}
		m_pObject->Modify();
		m_pObject->FreeCacheBmp();
	}
	//return kAvmThunkUndefined;//Modify this please!
}

bool TextFieldObject::AS3_displayAsPassword_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->IsPassword();
	return false;
	//return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_displayAsPassword_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->editText->SetPassword(b);
}

bool TextFieldObject::AS3_embedFonts_get()
{
	//Add your act code here...
	if(m_pObject)
//#if (__CORE_VERSION__>=0x02075000)
		return (m_pObject->editText->m_flags&seditTextFlagsHasFont)!=0;
//#else
//		return m_pObject->editText->IsUseFont()!=0;
//#endif
	return false;
}

void TextFieldObject::AS3_embedFonts_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
//#if (__CORE_VERSION__>=0x02075000)
	{
		if(b)
			m_pObject->editText->m_flags|=seditTextFlagsHasFont;
		else
			m_pObject->editText->m_flags&=~seditTextFlagsHasFont;

	}
//#else
//		m_pObject->editText->SetUseFont(b);
//#endif
}

Stringp TextFieldObject::AS3_gridFitType_get()
{
	return m_strFitType;
}

void TextFieldObject::AS3_gridFitType_set(Stringp p)
{
	//Add your act code here...
	m_strFitType=p;
}

Stringp TextFieldObject::AS3_htmlText_get()
{
	//Add your act code here...
	if(m_pObject)
	{
#ifdef _DEBUGBREAK_
		{
			XString8 s;
			s.ConvertFrom("��ӭ",XCHARSET_GB2312);
			XString8 ss(m_pObject->editText->m_buffer);
			if(ss.Find(s)==0)
				int v=0;
		}
#endif
//#ifdef _WINEMU
//		XString16 sss(m_pObject->editText->m_initialText.strTxt);
//		XSWFPlayer::m_pInstance->Trace(sss);
//#endif
		m_pObject->editText->UpdateHTMLText();
		XXVar&t=m_pObject->editText->m_initialText;
//#ifdef _WINEMU
//		XString16 ss(t.strTxt);
//		XSWFPlayer::m_pInstance->Trace(ss);
//#endif
		return core()->newStringUTF8(t.strTxt,t.GetLength());
	}
	return false;
	//return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_htmlText_set(Stringp str)
{
	//Add your act code here...
	//int len = str->length();
	//if(len==9&&!str->isEmpty())
	//	int a = 0;
	if(m_pObject)
	{
/*#ifdef _WINEMU
	StUTF8String ss(str);
	if(strstr(ss.c_str(),"37/")!=NULL)
			int v=0;	
#endif*/
		//if(str->length()>100)
		//{
		//	StUTF8String ss(str);
		//	//XString8 s8(ss.c_str(),ss.length());
		//	//s8.ConvertToGB();
		//	XFile file;
		//	file.Open("D:\\ht.txt",XFile::XWRITE|XFile::XCREATE|XFile::XBINARY);
		//	file.Write(ss.c_str(),ss.length());//s8.GetData(),s8.GetLength());
		//	file.Close();
		//}
		EditText*pEdit=m_pObject->editText;

//		if(pEdit->m_length>10)
//			int v=0;

		if(!pEdit->IsAutoSize()&&m_pObject->character->type==editTextEmptyChar)
		{
			if(pEdit->m_autoMode==stextAlignJustify)
				pEdit->m_autoMode=stextAlignLeft;//(U8)pClass->ToID(a);
			pEdit->AutoSize(XTRUE);//pEdit->m_autoMode!=stextAlignJustify);	
		}


		StUTF8String s(str);
		//if(strstr(s.c_str(),"+22")!=NULL)
		//	int v=0;
		pEdit->SetHTMLText(NULL,s.c_str());
		m_pObject->Modify();
		m_pObject->FreeCacheBmp();
/*#if (__CORE_VERSION__>=0x02079000)
		{
			int i=0,len=pEdit->m_length-1;
			for(i=0;i<len;i++)
			{
				if(pEdit->m_buffer[i]=='\r')
				{
					m_bHtmlNewLine=true;
					break;
				}
			}
		}
#endif*/
		//XString8 ss(m_pObject->editText->m_buffer);
		//if(strstr(ss.GetData(),"Lv160"))
		//	int v=0;		
	
		/*if(s.length()>200)
		{
			XFile file;
			file.Open("E:\\chat.txt",XFile::XBINARY|XFile::XWRITE|XFile::XCREATE);
			file.Write(s.c_str(),s.length());
			file.Close();
		}*/
		
		//if(pEdit->m_length>6&&pEdit->m_buffer[0]==24403&&pEdit->m_buffer[1]==21069&&pEdit->m_buffer[2]==21487)
		//	int v=0;
		if(pEdit->IsAutoSize())
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth0,m_nWidth));
		else
			CalcAutoSize(m_nWidth0,false);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

int TextFieldObject::AS3_length_get()
{
	//Add your act code here...
	if(m_pObject)
	{

/*#if (__CORE_VERSION__>=0x02079000)
		if(m_pObject->editText->IsHtml())
		{
			return m_pObject->editText->m_length+1;
		}
#endif*/
		return m_pObject->editText->m_length;
	}
	return 0;
}

int TextFieldObject::AS3_maxChars_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->m_maxLength;
	return 0;
}

void TextFieldObject::AS3_maxChars_set(int l)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->editText->m_maxLength=l;
}

int TextFieldObject::AS3_maxScrollH_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
		return m_pObject->editText->CalcMaxHScroll();
	return 0;
}

int TextFieldObject::AS3_maxScrollV_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->CalcMaxVScroll()+1;
	return 0;
}

bool TextFieldObject::AS3_mouseWheelEnabled_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsMouseWheel();
	return false;
}

void TextFieldObject::AS3_mouseWheelEnabled_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetMouseWheel(b);
}

bool TextFieldObject::AS3_multiline_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->IsMultiLine()!=0;
	return false;
}

void TextFieldObject::AS3_multiline_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
	{
		m_pObject->editText->SetMultiLine(b);
		if(m_pObject->editText->IsAutoSize())
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth0,m_nWidth));
		else
			m_pObject->editText->UpdateIfNeed();
	}
}

int TextFieldObject::AS3_numLines_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->m_numLines;
	return 0;
}

Stringp TextFieldObject::AS3_restrict_get()
{
	//Add your act code here...
	return m_strRestrict;
}

void TextFieldObject::AS3_restrict_set(Stringp s)
{
	//Add your act code here...
	m_strRestrict=s;
}

int TextFieldObject::AS3_scrollH_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->m_hscroll;
	return 0;
}

void TextFieldObject::AS3_scrollH_set(int v)
{
	//Add your act code here...
	if(m_pObject)
	{
		int max=m_pObject->editText->CalcMaxHScroll();
		if(v>max) v=max;
		if(v!=m_pObject->editText->m_hscroll)
		{
			OnEvent(XAS3_EVENT_scroll,false,this,true);
			m_pObject->editText->m_hscroll=v;
			m_pObject->Modify();
			m_pObject->FreeCacheBmp();
		}
	}
}

int TextFieldObject::AS3_scrollV_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->m_vscroll+1;
	return 0;
}

void TextFieldObject::AS3_scrollV_set(int v)
{
	//Add your act code here...
	if(m_pObject)
	{
		int max=m_pObject->editText->CalcMaxVScroll();
		if(v>0) v--;
		if(v>max) v=max;
		if(v!=m_pObject->editText->m_vscroll)
		{
			OnEvent(XAS3_EVENT_scroll,false,this,true);
			m_pObject->editText->m_vscroll=v;
			m_pObject->Modify();
			m_pObject->FreeCacheBmp();
		}
	}
}

bool TextFieldObject::AS3_selectable_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->IsSelectable();
	return false;
}

void TextFieldObject::AS3_selectable_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
	{
		//if(!(m_pObject->editText->m_flags & seditTextFlagsReadOnly)&&!b)
		//	int v=0;
		m_pObject->editText->SetSelectable(b);
	}

}

int TextFieldObject::AS3_selectionBeginIndex_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->m_selectionStart;
	return 0;
}

int TextFieldObject::AS3_selectionEndIndex_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->editText->m_selectionEnd;
	return 0;
}

double TextFieldObject::AS3_sharpness_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
		return m_pObject->editText->m_sharpness;
	return 0;
}

void TextFieldObject::AS3_sharpness_set(double v)
{
	//Add your act code here...
	if(MathUtils::isNaN(v))
		return;
	int sharpness = v ;
	if( sharpness > 400 )
		sharpness = 400;
	else if( sharpness < -400 )
		sharpness = -400;
	if(m_pObject)
		m_pObject->editText->m_sharpness = sharpness;
}

StyleSheetObject* TextFieldObject::AS3_styleSheet_get()
{
	//Add your act code here...
	return m_pSheet;
}

void TextFieldObject::AS3_styleSheet_set(StyleSheetObject*p)
{
	//Add your act code here...
	m_pSheet=p;
	if(m_pObject)
	{
		m_pObject->Modify();
		m_pObject->FreeCacheBmp();
	}
}


Stringp TextFieldObject::AS3_text_get()
{
	//Add your act code here...
	if(m_pObject)
	{
		/*XU16*pData=m_pObject->editText->m_buffer;
		int l	   m_pobject->editText->m_length;
		if(!l)
			return core()->kEmptyString;
		else
		{
			int nSize=l*2;
			XU16*pNew=(XU16*)_AllocData(nSize);
			XGlobal::Memcmp(pNew,pData,l*2);
			for(int i=0;i<l;i++)
			{
				if(pNew[i]==editTextNewHTMLLine)
					pNew[i]=editTextSoftNewline;

			}
			_FreeData(nSize);
		}*/
/*#if (__CORE_VERSION__>=0x02079000)
		if(true)//)
		{
			//Stringp str=core()->newStringUTF16(m_pObject->editText->m_buffer,m_pObject->editText->m_length);
			//str->AS3_
			int i,len=m_pObject->editText->m_length,iSet=0;
			XU16* buf=(XU16*)XXVar::_AllocData(m_pObject->editText->m_length*sizeof(XU16));
			for(i=0;i<len;i++)
			{
				if(m_pObject->editText->m_buffer[i]!='\r')
				{
					buf[iSet]=m_pObject->editText->m_buffer[i];
					iSet++;
				}
			}
			Stringp str=core()->newStringUTF16(buf,iSet);
			XXVar::_FreeData(buf);
			return str;
			//return str;
		}
		else
			return core()->newStringUTF16(m_pObject->editText->m_buffer,m_pObject->editText->m_length);
#else
#if (__CORE_VERSION__>=0x02079000)
		if(m_pObject->editText->IsHtml())
		{
			Stringp str=core()->newStringUTF16(m_pObject->editText->m_buffer,m_pObject->editText->m_length);
			str->concatStrings(str,core()->newConstantStringLatin1("\n"));
			return str;
		}
#endif*/
		return core()->newStringUTF16(m_pObject->editText->m_buffer,m_pObject->editText->m_length);
//#endif
	}
	return core()->kEmptyString;
	//return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_text_set(Stringp strTxt)
{

	//Add your act code here...
/*#ifdef _WINEMU
	StUTF8String s(strTxt);
	if(s.length()>2)
	{
		if(strstr(s.c_str(),"j0xffffff$")!=NULL)
			int v=0;
	}
#endif*/
	//StUTF8String s( strTxt );
	//LOGWHEREMSG( s.c_str() );
	/*if(m_pObject->editText->m_pFormat->underline){
	StUTF16String si(strTxt);
	XString16 sii;
	sii.FromInt(m_pObject->editText->m_pFormat->underline);
	XString16 s1("$$$$$$$  ");
	s1+=si.c_str();
	s1+=sii;
	int a = m_pSheet?1:0;
	XString16 s2;
	s2.FromInt(a);
	s1+=s2;
	m_pObject->display->player->Trace(s1);}*/
	if(m_pObject)
	{
		//if(strTxt->length()==86)
		//	int a = 0;
		//	m_pObject->editText->m_pFormat->blockIndent = 1200;
		if(m_pSheet)
		{
			AS3_htmlText_set(strTxt);
			return;
		}
		/*if(m_pObject->editText->m_length>2&&
			m_pObject->editText->m_buffer[0]==21333&&
			m_pObject->editText->m_buffer[1]==20154)
		{
			int v=0;
		}*/
		//Stringp strTxt=AvmThunkUnbox_AvmString(argv[argoff1]);
//		if(m_pObject->name.strTxt&&
//			strcmp(m_pObject->name.strTxt,"title_txt")==0)
//			int v=0;
		StUTF16String strw(strTxt);
		m_pObject->AS3SetText(strw.c_str(),strw.length());
		if(strw.length()==0)
			m_pObject->editText->ClearFormat();
//#if (__CORE_VERSION__>=0x02079000)
//		m_bHtmlNewLine=false;
//#endif
		//if(m_pObject->editText->m_length==4&&m_pObject->editText->m_buffer[0]==20998)
		//	int v=0;
		//if(m_pObject->editText->m_buffer[0]==28857)
		//	int v=0;
		//XString8 str(m_pObject->editText->m_buffer);
		//XString8 strTmp;
		//strTmp.ConvertFrom("����Ѽ�����Ҧ�",XCHARSET_GB2312);
		//if(str.Find(strTmp)>=0)
		//	int v=0;
//		XString8 ss(m_pObject->editText->m_buffer);
//		if(strstr(ss.GetData(),"Lv160"))
//			int v=0;

		if(m_pObject->editText->IsAutoSize())
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth0,m_nWidth));
		else
			CalcAutoSize(m_nWidth0,false);
	}
	
}

uint32_t TextFieldObject::AS3_textColor_get()
{
	//Add your act code here...
#ifndef _ARGB_PIXEL_
	if(m_pObject)
	{
		XU32 c=m_pObject->editText->m_pFormat->color&0xffffff;
		SWAPPIXELRB(c);
		return c;
	}
#else
	if(m_pObject)
		return m_pObject->editText->m_pFormat->color&0xffffff;
#endif
	return 0;
}

void TextFieldObject::AS3_textColor_set(uint32_t c)
{
	//Add your act code here...
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(c);
#endif
	if(m_pObject)
	{
		m_pObject->editText->SetTextColor(c|0xff000000);
		m_pObject->Modify();
		m_pObject->FreeCacheBmp();
	}
}

double TextFieldObject::AS3_textHeight_get()
{
	//Add your act code here...
	if(m_pObject)
	{
		if(m_pObject->editText->nTextHeight)
		{
			return (m_pObject->editText->nTextHeight/*-onePoint*4*/)/20;
		}
		else
		{
			double w,h;
			m_pObject->editText->GetTextSize(w,h);
			//h-=onePoint*4;
			return (int)h;
		}
	}
	return 0;
}

double TextFieldObject::AS3_textWidth_get()
{
	//Add your act code here...	
	if(m_pObject)
	{
		//if(m_pObject->editText->m_length==3)
		//	int v=0;
		double w=0,h=0;
//#if (__CORE_VERSION__>=0x02071000)
		bool bWordWrap=(m_pObject->editText->m_flags&seditTextFlagsWordWrap)&&m_pObject->editText->IsMultiLine();
		if(m_pObject->editText->nTextWidth&&!bWordWrap)
//#else
//		if(m_pObject->editText->nTextWidth)
//#endif
		{
			w=(m_pObject->editText->nTextWidth)/20;
		}
		else
		{
			//double w,h;
			m_pObject->editText->GetTextSize(w,h);
			//w-=onePoint*4;
			
			//return w;
		}
		if(w<0) w=0;
		//if(m_pObject->editText->m_length==2&&w<=8)
		//	int v=0;
		return (int)w;
	}
	return 0;
}

double TextFieldObject::AS3_thickness_get()
{
	//Add your act code here...	
	if(m_pObject)
		return m_pObject->editText->m_sharpness/100.0;
	return 0;
}

void TextFieldObject::AS3_thickness_set(double v)
{
	if(MathUtils::isNaN(v))
		return;
	if(m_pObject)
		m_pObject->editText->m_sharpness=v*100;
	//Add your act code here...

}

Stringp TextFieldObject::AS3_type_get()
{
	//Add your act code here...
	if(m_pObject)
	{
		TextFieldTypeClass*p=((ShellToplevel*)toplevel())->getTextFieldTypeClass();
//#if (__CORE_VERSION__>=0x02079000)
		if(m_pObject->editText->IsEditAble(false))
//#else
//		if(m_pObject->editText->IsEditAble())
//#endif
			return p->getSlotINPUT();
		else
			return p->getSlotDYNAMIC();
	}
	return NULL;//Modify this please!
}

void TextFieldObject::AS3_type_set(Stringp s)
{
	//Add your act code here...
	if(m_pObject)
	{
		TextFieldTypeClass*p=((ShellToplevel*)toplevel())->getTextFieldTypeClass();
		if(s->equals(p->getSlotINPUT()))
		{
			m_pObject->editText->m_flags&=~seditTextFlagsReadOnly;
			m_pObject->editText->m_flags&=~seditTextFlagsNoSelect;
		}
		else
		{
			m_pObject->editText->m_flags|=seditTextFlagsReadOnly;
			m_pObject->editText->m_flags|=seditTextFlagsNoSelect;
		}
	}

}

bool TextFieldObject::AS3_useRichTextClipboard_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void TextFieldObject::AS3_useRichTextClipboard_set(bool b)
{
	//Add your act code here...
}

bool TextFieldObject::AS3_wordWrap_get()
{
	//Add your act code here...
	if(m_pObject)
		return (m_pObject->editText->m_flags&seditTextFlagsWordWrap)!=0;
	return false;//Modify this please!
}

void TextFieldObject::AS3_wordWrap_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
	{
		//if(m_pObject->editText->m_length==10)
		//	int v=0;
		if(b)
			m_pObject->editText->m_flags|=seditTextFlagsWordWrap;
		else
			m_pObject->editText->m_flags&=~seditTextFlagsWordWrap;
		//m_pObject->Modify();
		if(m_pObject->editText->IsAutoSize())
		{
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth,m_nWidth0));
		}
		else
			m_pObject->editText->UpdateIfNeed();
		m_pObject->Modify();
		m_pObject->FreeCacheBmp();
	}
}

//AS3 contructor function..
void TextFieldObject::AS3_constructor()
{
	//Add your act code here...
	//StUTF8String s(this->vtable->traits->name());
	//if(strcmp(s.c_str(),"CustomTextfield")==0)
	//	int v=0;
	InteractiveObjectObject::AS3_constructor();
	if(m_pObject==NULL)
	{
		DisplayObjectObject::CreateNewObject(editTextChar);
		
		//this->AS3_multiline_set(false);
	}
	if(m_pObject&&m_pObject->editText&&!m_pObject->editText->m_bNoSize)
	{
		m_nWidth0=RectWidth(&m_pObject->editText->editRect)/20.0;
		m_nHeight0=RectHeight(&m_pObject->editText->editRect)/20.0;
	}
//#ifdef _WINEMU
//	if(m_nWidth0<=1)
//		int vvv=0;
//	if((int)m_nWidth0==282)
//		int vvv=0;
//#endif
	//return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_appendText(Stringp s)
{
	//Add your act code here...	
	if( s == NULL )
	{
		return;
	}
	if(m_pObject)
	{
#ifdef _DEBUGBREAK_
		{
			XString8 s;
			s.ConvertFrom("��ӭ",XCHARSET_GB2312);
			XString8 ss(m_pObject->editText->m_buffer);
			if(ss.Find(s)==0)
				int v=0;
		}
#endif

		StUTF16String t(s);
//#if (__CORE_VERSION__>=0x02073000)
		m_pObject->editText->AppendText((U16*)t.c_str(),t.length());
//#else
//		int id=m_pObject->editText->m_length-1;
//		m_pObject->editText->Select(id,id);
//		m_pObject->editText->InsertWideChars((U16*)t.c_str(),t.length());
//#endif
		m_pObject->Modify();
		if(m_pObject->editText->IsAutoSize())
		{
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth,m_nWidth0));
		}
		else
			m_pObject->editText->UpdateIfNeed();
		m_pObject->FreeCacheBmp();
	}
}

inline void TextFieldObject::CheckID(int i)
{
	if(!m_pObject)
		toplevel()->throwRangeError(kOutOfRangeError,INTString(i),core()->knull);
	if(i<0||i>=m_pObject->editText->m_length)
		toplevel()->throwRangeError(kOutOfRangeError,INTString(i),INTString(m_pObject->editText->m_length));
}
inline void TextFieldObject::CheckLine(int i)
{
	if(!m_pObject)
		toplevel()->throwRangeError(kOutOfRangeError,INTString(i),core()->knull);
	m_pObject->editText->UpdateIfNeed();
	if(i<0||i>=m_pObject->editText->m_numLines)
		toplevel()->throwRangeError(kOutOfRangeError,INTString(i),INTString(m_pObject->editText->m_numLines));
}

RectangleObject* TextFieldObject::AS3_getCharBoundaries(int i)
{
	//Add your act code here...
	SRECT rect;
	if(i<0) return NULL;
	if(m_pObject)
	{
		//CheckID(i);
		if(i>=m_pObject->editText->m_length)
			return NULL;
		m_pObject->editText->GetCharBoundaries(i,rect);
	}
	RectangleObject*pObj=(RectangleObject*)((ShellToplevel*)toplevel())->getRectangleClass()->
		CreateRectangle(rect.xmin+2,rect.ymin+2,rect.xmax-rect.xmin,rect.ymax-rect.ymin);
	
	return pObj;
}

int TextFieldObject::AS3_getCharIndexAtPoint(double x,double y)
{
	//Add your act code here...
	if(m_pObject)
	{
		int lx=x*20;
		int ly=y*20;
		//m_pObject->editText->TranslatePoint(lx,ly);
		return m_pObject->editText->GetCharIndextAtPoint(lx,ly);
	}
	return 0;
}

int TextFieldObject::AS3_getFirstCharInParagraph(int id)
{
	//Add your act code here...
	if(id<0) return -1;
	if(m_pObject)
	{
		//CheckID(id);
		if(id>=m_pObject->editText->m_length)
			return -1;
			//return NULL;
		return m_pObject->editText->GetFirstCharInParagraph(id);
	}
	return 0;
}

DisplayObjectObject* TextFieldObject::AS3_getImageReference(Stringp s)
{
	//Add your act code here...
	if(m_pObject)
	{
		StUTF8String str(s);
		const char*strClass =m_pObject->editText->GetImageRefrence((char*)str.c_str());
		//if(pObj) return (DisplayObjectObject*)pObj->pASObject;
		if(strClass)
		{
			ShellToplevel*top=(ShellToplevel*)toplevel();
			ApplicationDomainObject*pDomain=top->getApplicationDomainClass()->AS3_currentDomain_get();
			if(pDomain)
			{
				Stringp strName=core()->newStringUTF8(strClass);
				if(pDomain->AS3_hasDefinition(strName))
				{
					Atom a=pDomain->AS3_getDefinition(strName);
					if(!AvmCore::isNullOrUndefined(a))
					{
						ClassClosure*pClass=(ClassClosure*)AvmCore::atomToScriptObject(a);
						if(ShellCore::IsSuperClassOf(top->getDisplayObjectClass(),pClass))
						{
							a=pClass->construct(0,&a);
							if(!AvmCore::isNullOrUndefined(a))
							{
								ScriptObject*obj=AvmCore::atomToScriptObject(a);
								return (DisplayObjectObject*)obj;
							}
						}
							//return (DisplayObjectObject*)
					}
				}
			}
			else
			{
				LoaderClass*pClass=top->getLoaderClass();
				LoaderObject*obj=pClass->CreateLoader();
				//obj->AS3_load(
				return obj;
			}
		}
	}
	return NULL;
}

int TextFieldObject::AS3_getLineIndexAtPoint(double x,double y)
{
	//Add your act code here...
	if(m_pObject)
	{
		return m_pObject->editText->GetLineIndexAtPoint(x,y);
	}
	return 0;
}

int TextFieldObject::AS3_getLineIndexOfChar(int i)
{
	//Add your act code here...
	if(i<0) return -1;
	if(m_pObject)
	{
		//CheckID(i);
		if(i>=m_pObject->editText->m_length)
			return -1;
		return m_pObject->editText->GetLineIndexOfChar(i);
	}
	return 0;
}

int TextFieldObject::AS3_getLineLength(int i)
{
	//Add your act code here...
	//if(i<0) return -1;
	if(m_pObject)
	{
		CheckLine(i);
		//if(i>=m_pObject->editText->m_numLines)
		//	return -1;
		return m_pObject->editText->GetLineLength(i);
	}
	return 0;
}

TextLineMetricsObject* TextFieldObject::AS3_getLineMetrics(int i)
{
	//Add your act code here...
	CheckLine(i);
	if(m_pObject)
	{
		_XLINEINFO*fmt=m_pObject->editText->GetLineMetrics(i);
		TextLineMetricsClass*pClass=((ShellToplevel*)toplevel())->getTextLineMetricsClass();
		TextLineMetricsObject*pObject=(TextLineMetricsObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		//pObject->SetTe
		int basex=AS3_x_get();
		pObject->AS3_constructor(fmt->x/20+basex,fmt->nWidth/20,fmt->nHeight/20,fmt->nAscent/20,fmt->nDescent/20,fmt->nLeading/20);
		return pObject;
	}
	return NULL;
}

int TextFieldObject::AS3_getLineOffset(int i)
{
	//Add your act code here...
	CheckLine(i);
	if(m_pObject)
		return m_pObject->editText->GetLineOffset(i);
	return 0;
}

Stringp TextFieldObject::AS3_getLineText(int i)
{
	//Add your act code here...
	CheckLine(i);
	if(m_pObject)
	{
		U16* pText=m_pObject->editText->GetLineText(i);
		if(pText)
			return core()->newStringUTF16(pText,i);
	}
	return NULL;
}

int TextFieldObject::AS3_getParagraphLength(int id)
{
	//Add your act code here...
	//CheckID(id);
	if(id<0) return -1;
	if(m_pObject)
	{
		if(id>=m_pObject->editText->m_length)
			return -1;
		return m_pObject->editText->GetPararaphLength(id);
	}
	return 0;
}

TextFormatObject* TextFieldObject::AS3_getTextFormat(int b,int e)
{
	//Add your act code here...
	if(b<0) b=0;
	if(e<0) e=0;
	TextFormatClass*pClass=((ShellToplevel*)toplevel())->getTextFormatClass();
	TextFormatObject*pObj=(TextFormatObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
	if(m_pObject)
	{
		_XTEXTFORMAT*fmt=m_pObject->editText->GetTextFormat(b,e);
		if(fmt)
			pObj->SetFormat(fmt);
	}
	return pObj;
}

void TextFieldObject::AS3_replaceSelectedText(Stringp s)
{
	//Add your act code here...
	if(m_pObject)
	{
#ifdef _DEBUGBREAK_
		{
			XString8 s;
			s.ConvertFrom("��ӭ",XCHARSET_GB2312);
			XString8 ss(m_pObject->editText->m_buffer);
			if(ss.Find(s)==0)
				int v=0;
		}
#endif
		StUTF16String str(s);
		m_pObject->editText->replaceSelectText((U16*)str.c_str(),str.length());
		m_pObject->Modify();
		if(m_pObject->editText->IsAutoSize())
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth0,m_nWidth));
		else
			CalcAutoSize(m_nWidth0,false);
		m_pObject->FreeCacheBmp();
//#if (__CORE_VERSION__>=0x02079000)
//		m_bHtmlNewLine=false;
//#endif
	}
	
}

void TextFieldObject::AS3_replaceText(int b,int e,Stringp s)
{
	//Add your act code here...
	if(m_pObject)
	{
#ifdef _DEBUGBREAK_
		{
			XString8 s;
			s.ConvertFrom("��ӭ",XCHARSET_GB2312);
			XString8 ss(m_pObject->editText->m_buffer);
			if(ss.Find(s)==0)
				int v=0;
		}
#endif
		StUTF16String str(s);
//#if (__CORE_VERSION__>=0x02079000)
		m_pObject->editText->replaceWideText((U16*)str.c_str(),str.length(),b,e,true);
//#else
//		m_pObject->editText->replaceWideText((U16*)str.c_str(),str.length(),b,e);
//#endif
		m_pObject->Modify();
		if(m_pObject->editText->IsAutoSize())
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth,m_nWidth0));
		else
			CalcAutoSize(m_nWidth0,false);
		m_pObject->FreeCacheBmp();
//#if (__CORE_VERSION__>=0x02079000)
//		m_bHtmlNewLine=false;
//#endif
	}
}

void TextFieldObject::AS3_setSelection(int b,int e)
{
	//Add your act code here...
	if(m_pObject)
	{
		m_pObject->editText->setSelection(b,e);
	}
}

void TextFieldObject::AS3_setTextFormat(TextFormatObject*f,int b,int e)
{
	//Add your act code here...
	if(m_pObject)
	{
#ifdef _DEBUGBREAK_
		{
			XString8 s;
			s.ConvertFrom("��ӭ",XCHARSET_GB2312);
			XString8 ss(m_pObject->editText->m_buffer);
			if(ss.Find(s)==0)
				int v=0;
		}
#endif
		if(b<0) b=0;
		if(e<0) e=0;
		m_pObject->editText->SetFormat(f,b,e);
		m_pObject->Modify();
		if(m_pObject->editText->IsAutoSize())
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth,m_nWidth0));
		else
			CalcAutoSize(m_nWidth0,false);
		m_pObject->FreeCacheBmp();
	}
}


double TextFieldObject::AS3_width_get()
{
	//if(m_pObject->character->type == textLineChar)
	//return m_nWidth0;
	if(m_pObject&&m_pObject->editText)
	{
		SRECT*rect=&m_pObject->editText->editRect;//character->bounds;
		double width=RectWidth(rect)/20.0;
		double height=RectHeight(rect)/20.0;
		if(RectIsEmpty(rect))
		{
			height=0;
			width=0;
		}
		if(width>20)
			m_nWidth0=width;
		if(height>10)
			m_nHeight0=height;

//#ifdef _WINEMU
//	if(m_nWidth0<=1)
//		int vvv=0;
//	if((int)m_nWidth0==282)
//		int vvv=0;
//#endif

//#if (__CORE_VERSION__>=0x02071000)
		if(m_pObject->editText)
			m_pObject->editText->m_bNoSize=XFALSE;
//#endif
		//if(width>20)
		//	m_nWidth=width;
		//if(m_nWidth==5)
		//	int v=0;
//#if (__CORE_VERSION__>=0x02070200)
		//if(width>1000)
		//	int v=0;
		return DisplayObjectObject::AS3_width_get();
		//return width;
//#else
//		return width-4;
//#endif
	}
	return 0;
}

void TextFieldObject::SetWidth(double v,bool b)
{
	SRECT*rect=&m_pObject->editText->editRect;//character->bounds;
	//if(v==640)
	//	int vv=0;
	v=v*20;
	//if(v==80)
/*#ifdef _WINEMU
	if(v<100)
		int vvv=0;
#endif*/
	if(RectIsEmpty(rect))
	{
		rect->xmin=0;
		rect->ymin=0;
		rect->xmax=0;
		rect->ymax=0;
		rect->xmax=rect->xmin+v;
		//if(rect->xmax==80)
		//	int vv=0;
//#ifdef _WINEMU
//		if(rect->xmax-rect->xmin==20)
//					int vvv=0;
//#endif
		return;
	}
	if(!b)
	{
		rect->xmax=rect->xmin+v;
//#ifdef _WINEMU
//		if(rect->xmax-rect->xmin==20)
//					int vvv=0;
//#endif
		return;
	}
	//int old=rect->xmin;
	switch(m_pObject->editText->m_autoMode)
	{
	case stextAlignCenter:
		 {
			 int cx=(rect->xmin+rect->xmax)>>1;
			 //w=w>>1;
			 rect->xmin=cx-v/2;
			 rect->xmax=cx+v/2;
		 };
		 break;
	default:
	case stextAlignLeft:
		 rect->xmax=rect->xmin+v;
		 break;
	case stextAlignRight:
		 rect->xmin=rect->xmax-v;
	}
	//if(rect->xmin!=0)
	//{
	//	if(!m_bPosX)
	//	{
	//		m_bPosX = true;
	//		m_nPosX = m_pObject->GetPosX();
	//	}
	//	rect->xmax-=rect->xmin;
	//	m_pObject->SetPosX(m_nPosX+rect->xmin/20);
	//	//m_nDeltaX+=rect->xmin;
	//	rect->xmin = 0;
	//	//m_pObject->SetPosX(m_pObject->GetPosX()+(old-rect->xmin)/20);
	//}
	//if(m_pObject->editText->editRect.xmin==2340)
	//		int vv=0;
/*#ifdef _WINEMU
				if(rect->xmax-rect->xmin<100)
					int vvv=0;
#endif*/
	//if(rect->xmin>=900)
	//	int v=0;
}

void TextFieldObject::AS3_width_set(double v)
{
//	XString8 ss(m_pObject->editText->m_buffer);
//	if(strstr(ss.GetData(),"Lv160"))
//		int v=0;
//	if(v>1000)
//		int vv=0;
	if(v<0) return;
	//if(v>=319&&v<=320)
	//	int a = 0;
	if(MathUtils::isNaN(v))
		return;

//#if (__CORE_VERSION__>=0x02071000)
	if(m_pObject&&m_pObject->editText)
			m_pObject->editText->m_bNoSize=XFALSE;
	//if(v<1)
	//	v=1;
/*#else
	if(v<=0) return;
#endif*/
	//if(v<40)
	//	int vv=0;
/*#if (__CORE_VERSION__>=0x02070200)
#else
	v+=4;
#endif*/
	if(v<=0) return;
	if(v>20)
		m_nWidth0=v;
//#ifdef _WINEMU
//	if(m_nWidth0<=1)
//		int vvv=0;
//	if((int)m_nWidth0==282)
//		int vvv=0;
//#endif
	//if(m_nWidth==5)
	//	int v=0;
	//m_nWidth0=v;
	if(m_pObject)
	{
//#if (__CORE_VERSION__>=0x02071000)
		if(m_pObject->editText)
			m_pObject->editText->m_bNoSize=XFALSE;
//#endif
		SetWidth(v,false);
		if(v>20)
		{
			//m_nWidth0=m_nWidth;
			//m_nHeight0=
			m_pObject->editText->m_bNoSize=false;
			//if(m_nWidth==5)
			//	int v=0;
		}
		if(m_pObject->editText->m_length)
		{
			if(m_pObject->editText->IsAutoSize())
			{
				CalcAutoSize(0,true);
				m_nWidth0=RectWidth(&m_pObject->editText->editRect)/20;
			}
			else
				m_pObject->editText->UpdateIfNeed();
		}
		m_pObject->Modify();
		m_pObject->FreeCacheBmp();
	}
}
double TextFieldObject::AS3_height_get()
{
	if(m_pObject&&m_pObject->editText)
	{

		SRECT*rect=&m_pObject->editText->editRect;//character->bounds;
		double height=RectHeight(rect)/20.0;
		double width=RectWidth(rect)/20.0;
		if(RectIsEmpty(rect))
		{
			height=0;
			width=0;
		}
		//m_nWidth=RectWidth(rect)/20.0;
		//if(m_nWidth<20)
		//	m_nWidth=0;
		if(width>20)
			m_nWidth0=width;
		if(height>150)
			m_nHeight0=height;
//#ifdef _WINEMU
//	if(m_nWidth0<=1)
//		int vvv=0;
//	if((int)m_nWidth0==282)
//		int vvv=0;
//#endif
//#if (__CORE_VERSION__>=0x02071000)
		m_pObject->editText->m_bNoSize=XFALSE;
//#endif
		//if(m_nWidth==5)
		//	int v=0;
//#if (__CORE_VERSION__>=0x02070200)
		return DisplayObjectObject::AS3_height_get();
		//return height;
//#else
//		return height-4;
//#endif
	}
	return 0;
}
void TextFieldObject::AS3_height_set(double v)
{
	if(v<0)
		return;
	//if(v==MathUtils::kNaN)
	if(MathUtils::isNaN(v))
		return;
	//	int v=0;
	//if(v==20&&m_pObject->editText->m_length==0)
	//	int v=0;
/*#if (__CORE_VERSION__>=0x02070200)
#else
	v+=4;
#endif*/
	//if(v<=0) v++;////return;
	if(v>0)
		m_nHeight0=v;
	if(m_pObject)
	{
		//if(v>60)
		//	int cc=0;
		if(v>20)
			m_pObject->editText->m_bNoSize=false;
		if(m_pObject->editText->IsAutoSize())
		{
			//m_nWidth0=RectWidth(&m_pObject->editText->editRect)/20.0;
			CalcAutoSize(m_nWidth0,true);//XMAX(m_nWidth,m_nWidth0));
			m_nHeight0=RectHeight(&m_pObject->editText->editRect)/20.0;	
			//if(m_nWidth0<20)
			//	m_nWidth0=0;
//#ifdef _WINEMU
//	if(m_nWidth0<=1)
//		int vvv=0;
//if((int)m_nWidth0==282)
//		int vvv=0;
//#endif
//#if (__CORE_VERSION__>=0x02071000)
			m_pObject->editText->m_bNoSize=XFALSE;
//#endif
		}
		else
		{
			SRECT*rect=&m_pObject->editText->editRect;//character->bounds;
			if(RectIsEmpty(rect))
			{
				rect->xmin=0;
				rect->ymin=0;
				rect->xmax=0;
				rect->ymax=0;
			}
			rect->ymax=rect->ymin+v*20;

			//if(rect->ymax==460)
			//	int v=0;

//			if(rect->ymax<0)
//				int vv=0;

			m_pObject->Modify();
			m_pObject->FreeCacheBmp();
		}
	}
}


void TextFieldObject::CalcAutoSize(double wi,bool bAuto)
{
	double w,h;
	EditText*pEdit=m_pObject->editText;

	//#ifdef _WINEMU
	//
	//	XString8 strEdit(pEdit->m_buffer);
	//	XString8 strTmp("朵朵");
	//	//strTmp.ConvertFrom("关闭",XCHARSET_GB2312);
	//	if(strEdit.Find(strTmp)>=0)
	//		int v=0;
	//
	//#endif
	bool bWordWrap=(pEdit->m_flags&seditTextFlagsWordWrap)!=0&&!pEdit->m_bNoSize;

	int	nWidthOld = pEdit->editRect.xmax-pEdit->editRect.xmin;
//	if(onePoint*4 == nWidthOld)
	int nWidthNew = (pEdit->editRect.xmax+pEdit->editRect.xmin);
	if(wi&&bAuto)
	{
		SetWidth(wi,false);
	}
	

	int hs=pEdit->CalcMaxHScroll();
	int vs=pEdit->CalcMaxVScroll();

	pEdit->GetTextSize(w,h,false);

	if(hs!=pEdit->CalcMaxHScroll()||
		vs!=pEdit->CalcMaxVScroll())
	{
		OnEvent(XAS3_EVENT_scroll,false,this,true);
	}

	if(!bAuto)
		return;
	w+=onePoint*4;
	h+=onePoint*4;

	SRECT*rect=&m_pObject->editText->editRect;//character->bounds;
	if(!bWordWrap)
	{
		pEdit->m_bNoSize=false;
		{
			int old=rect->xmin;
			switch(m_pObject->editText->m_autoMode)
			{
			case stextAlignCenter:
//#if (__CORE_VERSION__>=0x02077000)
//				 //if(nWidthOld!=2000)
//#endif
				 {
					 //int cx=nWidthOld>>1;
					 //w=w>>1;
					 int cx=nWidthNew>>1;
					 rect->xmin=cx-w/2;//cx-w/2;
					 rect->xmax=cx+w/2;//cx+w/2;
//#if (__CORE_VERSION__>=0x02078000)
					 //int delta=cx-w/2;
//#else
//					 int delta=cx-w/2-rect->xmin;
//#endif
					 //if(delta)
					//	 int v=0;
					// if(m_pObject->GetPosX()+delta/20==-10)
					//	 int v=0;
					// m_pObject->SetPosX(m_pObject->GetPosX()+delta/20);
				 };
				 break;
			case stextAlignLeft:
				break;
			case stextAlignRight:
				{
					rect->xmin=rect->xmax-w;
				}
				break;
			}

			rect->xmax=rect->xmin+w;
		}

	}

	if(h<m_nHeight0*20)
		h=m_nHeight0*20;
	rect->ymax=rect->ymin+h;

	m_pObject->Modify();
	m_pObject->FreeCacheBmp();
	return;

}


double TextFieldObject::AS3_x_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		double m=m_pObject->editText->editRect.xmin/20;//character->bounds.xmin/20.0;

		return m_pObject->GetPosX()+m;
	}
	return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_x_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//v=0;
	//XString8 ss(m_pObject->editText->m_buffer);
	//if(strstr(ss.GetData(),"Lv160"))
	//	int v=0;

	//if ( v > 53.9 && v < 54.1 )
	//{
	//	int i = 0;
	//	v = 0;
	//}

	if(MathUtils::isNaN(v))
		return;
	if(m_pObject)
	{
		double m=m_pObject->editText->editRect.xmin/20;//character->bounds.xmin/20.0;
		//if((v-m)*20==4540)
		//	int vvv=0;
		//if(v-m==-10)
		//	int vvv=0;
		m_pObject->SetPosX(v-m);
	}
//return kAvmThunkUndefined;//Modify this please!
}

double TextFieldObject::AS3_y_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		double m=m_pObject->editText->editRect.ymin/20;//character->bounds.ymin/20.0;
		if(RectIsEmpty(&m_pObject->editText->editRect))
			m=0;
		return m_pObject->GetPosY()+m;
	}
	return kAvmThunkUndefined;//Modify this please!
}

void TextFieldObject::AS3_y_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(MathUtils::isNaN(v))
		return;
	if(m_pObject)
	{
		//if(m_pObject->editText->m_length==4)
		//	int v=0;
		double m=m_pObject->editText->editRect.ymin/20;//character->bounds.ymin/20.0;
		if(RectIsEmpty(&m_pObject->editText->editRect))
			m=0;
		//if(m<-100)
		//	int v=0;
//#if (__CORE_VERSION__>=0x02074000)
		m_pObject->SetPosY(v-m);
//#else
//		m_pObject->SetPosY(v+m);
//#endif
	}
	//return kAvmThunkUndefined;//Modify this please!
}

//new adds
bool TextFieldClass::AS3_isFontCompatible(Stringp pFontName, Stringp pFontStyle)
{
	return false;
}
//new adds end
//new adds
Stringp TextFieldObject::AS3_textInteractionMode_get()
{
	return NULL;
}
//new adds end

//void TextFieldObject::PrintfAddr(char* c)
//{
//	XString16 s(c);
//	XString16 si;
//	si.FromInt((int)this);
//	if(m_pObject)
//		m_pObject->display->player->Trace(s);
}
//}

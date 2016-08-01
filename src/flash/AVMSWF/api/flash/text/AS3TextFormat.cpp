#include "StdAfxflash.h"
#include "edittext.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3TextFormat.h"
#include "AS3TextFieldAutoSize.h"
namespace avmshell{
TextFormatClass::TextFormatClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextFormatObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextFormatClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextFormatObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextFormatObject::TextFormatObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_strURL=core()->kEmptyString;
}
//////////////////////////////////////////////////////////
//Native Method start...

void TextFormatObject::Release()
{

}


void TextFormatObject::SetObjFormat(ScriptObject*pFmt)
{
	AvmCore*c = core();
	Atom value;
	value=pFmt->getStringProperty(c->internStringLatin1("textAlign"));
	if(c->isNullOrUndefined(value))
		value=pFmt->getStringProperty(c->internStringLatin1("text-align"));
	if(!c->isNullOrUndefined(value))
	{
		WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.align,c->atomToString(value));
	}
	//value=pFmt->getStringProperty(c->internStringLatin1("blockIndent"));
	//if(!c->isNullOrUndefined(value))
	//{
	//	m_slots_TextFormatObject.blockIndent = value;
	//}
	//value=pFmt->getStringProperty(c->internStringLatin1("bold"));
	//if(!c->isNullOrUndefined(value))
	//{
	//	m_slots_TextFormatObject.bold = value;
	//}
	//value=pFmt->getStringProperty(c->internStringLatin1("bullet"));
	//if(!c->isNullOrUndefined(value))
	//{
	//	m_slots_TextFormatObject.bullet = value;
	//}
	value=pFmt->getStringProperty(c->internStringLatin1("fontFamily"));
	if(c->isNullOrUndefined(value))
		value=pFmt->getStringProperty(c->internStringLatin1("font-family"));
	if(!c->isNullOrUndefined(value))
	{
		WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.font,c->atomToString(value));
		//m_slots_TextFormatObject
	}
	value=pFmt->getStringProperty(c->internStringLatin1("textIndent"));
	if(c->isNullOrUndefined(value))
		value=pFmt->getStringProperty(c->internStringLatin1("text-indent"));
	if(!c->isNullOrUndefined(value))
	{
		m_slots_TextFormatObject.indent = value;
	}
	//value=pFmt->getStringProperty(c->internStringLatin1("italic"));
	//if(!c->isNullOrUndefined(value))
	//{
	//	m_slots_TextFormatObject.italic = value;
	//}
	value=pFmt->getStringProperty(c->internStringLatin1("kerning"));
	if(!c->isNullOrUndefined(value))
	{
		m_slots_TextFormatObject.kerning = value;
	}
	value=pFmt->getStringProperty(c->internStringLatin1("leading"));
	if(!c->isNullOrUndefined(value))
	{
		m_slots_TextFormatObject.leading = value;
	}
	value=pFmt->getStringProperty(c->internStringLatin1("marginLeft"));
	if(c->isNullOrUndefined(value))
		value=pFmt->getStringProperty(c->internStringLatin1("margin-left"));
	if(!c->isNullOrUndefined(value))
	{
		m_slots_TextFormatObject.leftMargin = value;
	}
	value=pFmt->getStringProperty(c->internStringLatin1("letterSpacing"));
	if(c->isNullOrUndefined(value))
		value=pFmt->getStringProperty(c->internStringLatin1("letter-spacing"));
	if(!c->isNullOrUndefined(value))
	{
		m_slots_TextFormatObject.letterSpacing = value;
	}
	value=pFmt->getStringProperty(c->internStringLatin1("marginRight"));
	if(c->isNullOrUndefined(value))
		value=pFmt->getStringProperty(c->internStringLatin1("margin-right"));
	if(!c->isNullOrUndefined(value))
	{
		m_slots_TextFormatObject.rightMargin = value;
	}
	value=pFmt->getStringProperty(c->internStringLatin1("fontSize"));
	if(c->isNullOrUndefined(value))
		value=pFmt->getStringProperty(c->internStringLatin1("font-size"));
	if(!c->isNullOrUndefined(value))
	{
		m_slots_TextFormatObject.size = value;
	}
	//value=pFmt->getStringProperty(c->internStringLatin1("tabStops"));
	//if(!c->isNullOrUndefined(value))
	//{
	//	WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.tabStops,c->atomToScriptObject(value));
	//}
	//value=pFmt->getStringProperty(c->internStringLatin1("target"));
	//if(!c->isNullOrUndefined(value))
	//{
	//	WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.target,c->atomToString(value));
	//}
	value=pFmt->getStringProperty(c->internStringLatin1("display"));
	if(!c->isNullOrUndefined(value))
	{
		WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.display,c->atomToString(value));
	}
	//value=pFmt->getStringProperty(c->internStringLatin1("underline"));
	//if(!c->isNullOrUndefined(value))
	//{
	//	m_slots_TextFormatObject.underline = value;
	//}
	value=pFmt->getStringProperty(c->internStringLatin1("color"));
	if(!c->isNullOrUndefined(value))
	{
		XU32 color=(XU32)(c->integer_u(value));
#ifndef _ARGB_PIXEL_
		SWAPPIXELRB(color);
#endif	
		m_slots_TextFormatObject.color=c->uintToAtom(color);
	}
	/*Namespacep publicNS=c->getPublicNamespace(0);
	Traits* traits=pFmt->vtable->traits;
	//while(traits)
	{
		int index = 0;
		TraitsBindingsp b=traits->getTraitsBindings();
		for(;;)
		{
			index=b->next(index);
			if(index<=0) break;
			Namespacep ns=b->nsAt(index);
			if(ns!=publicNS) continue;
			Stringp key=b->keyAt(index);				
			if(key)
			{
				Binding bd=b->valueAt(index);
				if(bd==BIND_NONE) continue;
				Atom value=0;
				switch (AvmCore::bindingKind(bd))
				{
					case BKIND_VAR:
						 value=pFmt->getSlotAtom(AvmCore::bindingToSlotId(bd),traits);						 
						 break;
					case BKIND_CONST:
						 break;
					case BKIND_GET:
						 break;
					case BKIND_GETSET:
						 //if(t!=(AMF_OUTNAME|AMF_OUTVALUE))
						 {
							//WriteUTF(str,false);								
							{
								int m = AvmCore::bindingToGetterId(bd);
								MethodEnv *f = pFmt->vtable->methods[m];//vtable->method[m];
									//env->methods[m];
								Atom atomv=pFmt->atom();
								value = f->coerceEnter(atomv);
							}
						 }
						 break;							
				}
				if(value)
				{
					if(key->equals(c->internStringLatin1("align")))
					{
						WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.align,c->atomToString(value));
					}
					else if(key->equals(c->internStringLatin1("blockIndent")))
					{
						m_slots_TextFormatObject.blockIndent = value;
					}
					else if(key->equals(c->internStringLatin1("bold")))
					{
						m_slots_TextFormatObject.bold = value;
					}
					else if(key->equals(c->internStringLatin1("bullet")))
					{
						m_slots_TextFormatObject.bullet = value;
					}
					else if(key->equals(c->internStringLatin1("font")))
					{
						WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.font,c->atomToString(value));
						//m_slots_TextFormatObject
					}
					else if(key->equals(c->internStringLatin1("indent")))
					{
						m_slots_TextFormatObject.indent = value;
					}
					else if(key->equals(c->internStringLatin1("italic")))
					{
						m_slots_TextFormatObject.italic = value;
					}
					else if(key->equals(c->internStringLatin1("kerning")))
					{
						m_slots_TextFormatObject.kerning = value;
					}
					else if(key->equals(c->internStringLatin1("leading")))
					{
						m_slots_TextFormatObject.leading = value;
					}
					else if(key->equals(c->internStringLatin1("leftMargin")))
					{
						m_slots_TextFormatObject.leftMargin = value;
					}
					else if(key->equals(c->internStringLatin1("letterSpacing")))
					{
						m_slots_TextFormatObject.letterSpacing = value;
					}
					else if(key->equals(c->internStringLatin1("rightMargin")))
					{
						m_slots_TextFormatObject.rightMargin = value;
					}
					else if(key->equals(c->internStringLatin1("size")))
					{
						m_slots_TextFormatObject.size = value;
					}
					else if(key->equals(c->internStringLatin1("tabStops")))
					{
						WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.tabStops,c->atomToScriptObject(value));
					}
					else if(key->equals(c->internStringLatin1("target")))
					{
						WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.target,c->atomToString(value));
					}
					else if(key->equals(c->internStringLatin1("url")))
					{
						WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.display,c->atomToString(value));
					}
					else if(key->equals(c->internStringLatin1("underline")))
					{
						m_slots_TextFormatObject.underline = value;
					}
				}
			}
		}
		
		//traits=traits->base;
	}*/

	//_XTEXTFORMAT*fmt=(_XTEXTFORMAT*)f;
	/*TextFieldAutoSizeClass*pClass=((ShellToplevel*)toplevel())->getTextFieldAutoSizeClass();
	//m_slots_TextFormatObject.align=pClass->ToString(fmt->align);
	AvmCore*c=core();
	Stringp key=c->internStringLatin1("align");
	Atom value;
	if(pFmt->hasStringProperty(key))
	{
		value = pFmt->getStringProperty(key);
		WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.align,c->atomToString(value));//fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL);
	}
	//m_slots_TextFormatObject.blockIndent
	key = c->internStringLatin1
	m_slots_TextFormatObject.blockIndent=c->intToAtom(fmt->blockIndent/20);
	m_slots_TextFormatObject.bold=fmt->Bold?trueAtom:falseAtom;
	m_slots_TextFormatObject.bullet=fmt->bullet?trueAtom:falseAtom;
	{

		XU32 color=fmt->color&0xffffff;
#ifndef _ARGB_PIXEL_
		SWAPPIXELRB(color);
#endif	
		m_slots_TextFormatObject.color=c->uintToAtom(color);
	}
	WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.font,fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL);
	//m_slots_TextFormatObject.font=fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL;
	m_slots_TextFormatObject.indent=c->intToAtom(fmt->indent/20);
	m_slots_TextFormatObject.italic=fmt->italic?trueAtom:falseAtom;
	m_slots_TextFormatObject.kerning=c->intToAtom(fmt->kerning/20);
	m_slots_TextFormatObject.leading=c->intToAtom(fmt->leading/20);
	m_slots_TextFormatObject.leftMargin=c->intToAtom(fmt->leftMargin/20);
	m_slots_TextFormatObject.letterSpacing=c->intToAtom(fmt->letterSpacing/20);
	m_slots_TextFormatObject.rightMargin=c->intToAtom(fmt->rightMargin/20);
	m_slots_TextFormatObject.size=c->intToAtom(fmt->size/20);
	m_slots_TextFormatObject.tabStops=NULL;
	//m_slots_TextFormatObject.target=fmt->target.GetLength()?c->newStringUTF8(fmt->target.strTxt):NULL;
	WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.target,fmt->target.GetLength()?c->newStringUTF8(fmt->target.strTxt):NULL);//fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL);
	if(fmt->url.GetLength())
		m_strURL=c->newStringUTF8(fmt->url.strTxt);//:NULL;
	m_slots_TextFormatObject.underline=fmt->underline?trueAtom:falseAtom;*/
}

void TextFormatObject::SetFormat(void*f)
{
	_XTEXTFORMAT*fmt=(_XTEXTFORMAT*)f;
	TextFieldAutoSizeClass*pClass=((ShellToplevel*)toplevel())->getTextFieldAutoSizeClass();
	//m_slots_TextFormatObject.align=pClass->ToString(fmt->align);
	WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.align,pClass->ToString(fmt->align));//fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL);
	//m_slots_TextFormatObject.blockIndent
	AvmCore*c=core();
	m_slots_TextFormatObject.blockIndent=c->intToAtom(fmt->blockIndent/20);
	m_slots_TextFormatObject.bold=fmt->Bold?trueAtom:falseAtom;
	m_slots_TextFormatObject.bullet=fmt->bullet?trueAtom:falseAtom;
	{

		XU32 color=fmt->color&0xffffff;
#ifndef _ARGB_PIXEL_
		SWAPPIXELRB(color);
#endif
		/*XU8* cc=(XU8*)&color;
		XU8 m=cc[0];
		cc[0]=cc[2];
		cc[2]=m;*/
		m_slots_TextFormatObject.color=c->uintToAtom(color);
	}
	WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.font,fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL);
	//m_slots_TextFormatObject.font=fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL;
	m_slots_TextFormatObject.indent=c->intToAtom(fmt->indent/20);
	m_slots_TextFormatObject.italic=fmt->italic?trueAtom:falseAtom;
	m_slots_TextFormatObject.kerning=c->intToAtom(fmt->kerning/20);
	m_slots_TextFormatObject.leading=c->intToAtom(fmt->leading/20);
	m_slots_TextFormatObject.leftMargin=c->intToAtom(fmt->leftMargin/20);
	m_slots_TextFormatObject.letterSpacing=c->intToAtom(fmt->letterSpacing/20);
	m_slots_TextFormatObject.rightMargin=c->intToAtom(fmt->rightMargin/20);
	m_slots_TextFormatObject.size=c->intToAtom(fmt->size/20);
	m_slots_TextFormatObject.tabStops=NULL;
	//m_slots_TextFormatObject.target=fmt->target.GetLength()?c->newStringUTF8(fmt->target.strTxt):NULL;
	WBRC(gc(),&m_slots_TextFormatObject,&m_slots_TextFormatObject.target,fmt->target.GetLength()?c->newStringUTF8(fmt->target.strTxt):NULL);//fmt->font.GetLength()?c->newStringUTF8(fmt->font.strTxt):NULL);
	if(fmt->url.GetLength())
		m_strURL=c->newStringUTF8(fmt->url.strTxt);//:NULL;
	m_slots_TextFormatObject.underline=fmt->underline?trueAtom:falseAtom;
}

void TextFormatObject::GetFormat(void*f)
{
	_XTEXTFORMAT*fmt=(_XTEXTFORMAT*)f;
	TextFieldAutoSizeClass*pClass=((ShellToplevel*)toplevel())->getTextFieldAutoSizeClass();
	if(m_slots_TextFormatObject.align)
		fmt->align=(U8)pClass->ToID(m_slots_TextFormatObject.align);
	AvmCore*c=core();
	if(!c->isNull(m_slots_TextFormatObject.blockIndent))
		fmt->blockIndent=(short)c->integer_i(m_slots_TextFormatObject.blockIndent)*20;
	if(!c->isNull(m_slots_TextFormatObject.bold))
		fmt->Bold=c->boolean(m_slots_TextFormatObject.bold)!=0;
	if(!c->isNull(m_slots_TextFormatObject.bullet))
		fmt->bullet=c->boolean(m_slots_TextFormatObject.bullet)!=0;
	if(!c->isNull(m_slots_TextFormatObject.color))
	{
		fmt->color=c->integer(m_slots_TextFormatObject.color)|0xff000000;
#ifndef _ARGB_PIXEL_
		SWAPPIXELRB(fmt->color);
#endif
		/*XU8* c=(XU8*)&fmt->color;
		XU8 m=c[0];
		c[0]=c[2];
		c[2]=m;*/
	}
	if(m_slots_TextFormatObject.font)
	{
		StUTF8String s(m_slots_TextFormatObject.font);
		fmt->font=s.c_str();
		fmt->font.ToString(XTRUE);
	}
	if(!c->isNull(m_slots_TextFormatObject.indent))
//#if (__CORE_VERSION__>=0x02071000)
		fmt->indent=(short)AvmCore::integer(m_slots_TextFormatObject.indent)*20;
//#else
//		fmt->indent=(short)c->integer_i(m_slots_TextFormatObject.indent)*20;
//#endif
	if(!c->isNull(m_slots_TextFormatObject.italic))
		fmt->italic=c->boolean(m_slots_TextFormatObject.italic)!=0;
	if(!c->isNull(m_slots_TextFormatObject.kerning))
		fmt->kerning=(short)c->integer(m_slots_TextFormatObject.kerning)*20;
	if(!c->isNull(m_slots_TextFormatObject.leading))
		fmt->leading=(short)c->integer(m_slots_TextFormatObject.leading)*20;
	if(!c->isNull(m_slots_TextFormatObject.leftMargin))
		fmt->leftMargin=(short)c->integer(m_slots_TextFormatObject.leftMargin)*20;
	if(!c->isNull(m_slots_TextFormatObject.letterSpacing))
		fmt->letterSpacing=(short)c->integer(m_slots_TextFormatObject.letterSpacing)*20;
	if(!c->isNull(m_slots_TextFormatObject.rightMargin))
		fmt->rightMargin=(short)c->integer(m_slots_TextFormatObject.rightMargin)*20;
	if(!c->isNull(m_slots_TextFormatObject.size))
	{
		int s=c->integer(m_slots_TextFormatObject.size);
		if(s)
		{
			fmt->size=((short)s)*20;
		}
		//if(fmt->size==0)
		//	int v=0;
	}
	if(!c->isNull(m_slots_TextFormatObject.underline))
		fmt->underline=c->boolean(m_slots_TextFormatObject.underline)!=0;
	if(m_slots_TextFormatObject.target)
	{
		StUTF8String s(m_slots_TextFormatObject.target);
		fmt->target=s.c_str();
		fmt->target.ToString(XTRUE);
	}
	if(m_strURL&&m_strURL!=core()->kEmptyString)//m_slots_TextFormatObject.url)
	{
		StUTF8String s(m_strURL);//m_slots_TextFormatObject.url);
		fmt->url=s.c_str();
		fmt->url.ToString(XTRUE);
	}
		//fmt->leftMargin=(short)c->integer_i(m_slots_TextFormatObject.leftMargin);
}

}

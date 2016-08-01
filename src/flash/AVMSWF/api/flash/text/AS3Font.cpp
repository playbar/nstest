#include "StdAfxflash.h"
//#if (__CORE_VERSION__>=0x02075000)
#include "ShellCore.h"
//#else
//#include "avmplus.h"
//#endif
using namespace avmplus;
#include "AS3Font.h"
//#if (__CORE_VERSION__>=0x02075000)
#include "AS3FontStyle.h"
#include "AS3FontType.h"
#include "splay.h"
//#endif
namespace avmshell{
FontClass::FontClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())FontObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* FontClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FontObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FontObject::FontObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
//#if (__CORE_VERSION__>=0x02075000)
	m_pChar=this->vtable->GetInitData();
//#endif
}
//////////////////////////////////////////////////////////
//Native Method start...
void FontClass::AS3_registerFont(ClassClosure*)
{
	//Add your act code here...
	
}

ArrayObject* FontClass::AS3_enumerateFonts(XBOOL b)
{
	return toplevel()->arrayClass->newArray(0);
}


bool FontObject::AS3_hasGlyphs(Stringp)
{
	//Add your act code here...
//#if (__CORE_VERSION__>=0x02075000)
	return true;
//#else
//	return false;
//#endif
}

//#if (__CORE_VERSION__>=0x02075000)
Stringp FontObject::AS3_fontName_get()
{
	if(m_pChar==NULL)
		return NULL;
	else
	{
		SCharacter*ch=(SCharacter*)m_pChar;
		if(ch&&ch->type==fontChar&&ch->font.fLen)
			return core()->newStringUTF8(ch->font.strFontName,ch->font.fLen);
	}
	return NULL;
}
Stringp FontObject::AS3_fontStyle_get()
{
	if(m_pChar==NULL)
		return NULL;
	else
	{
		SCharacter*ch=(SCharacter*)m_pChar;
		if(ch&&ch->type==fontChar)
		{
			AvmCore* c=core();
			FontStyleClass*pClass=((ShellToplevel*)toplevel())->getFontStyleClass();
			int flags=ch->font.flags;
			Stringp str=pClass->getSlotREGULAR();
			if(flags&tfontBold)
			{
				
				if(flags&tfontItalic)
				{
					str=str->concatStrings(str,pClass->getSlotBOLD_ITALIC());
				}
				else
					str=pClass->getSlotBOLD();
			}
			else if(flags&tfontItalic)
				str=pClass->getSlotITALIC();
			return str;
		}
	}
	return NULL;
}
Stringp FontObject::AS3_fontType_get()
{
	if(m_pChar==NULL)
		return NULL;
	else
	{
		SCharacter*ch=(SCharacter*)m_pChar;
		if(ch&&ch->type==fontChar)
		{
			FontTypeClass*pClass=((ShellToplevel*)toplevel())->getFontTypeClass();
			if(ch->font.deviceState==textDync)
				return pClass->getSlotDEVICE();
			else
				return pClass->getSlotEMBEDDED();
		}
	}
	return NULL;
}
//#endif

}
#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3FontDescription.h"
#include "AS3CFFHinting.h"
#include "AS3FontLookup.h"
#include "AS3FontPosture.h"

namespace avmshell{
FontDescriptionClass::FontDescriptionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FontDescriptionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FontDescriptionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FontDescriptionObject::FontDescriptionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShellToplevel*top=(ShellToplevel*)toplevel();
	CFFHintingClass*pClass=top->getCFFHintingClass();
	m_strCffHinting=pClass->getHORIZONTAL_STEM();
	FontLookupClass*pLookClass=top->getFontLookupClass();
	m_strFontLookup=pLookClass->getDEVICE();
	m_strFontName=core()->newConstantStringLatin1("_serif");
	FontPostureClass*pPosClass=top->getFontPostureClass();
	m_strFontPosture=pPosClass->getNORMAL();
	m_strFontWeight=m_strFontPosture;
	m_strRenderingMode=m_strFontPosture;
	m_bLocked=false;
}
//////////////////////////////////////////////////////////
//Native Method start...
bool FontDescriptionClass::AS3_isDeviceFontCompatible(Stringp fontName, Stringp fontWeight, Stringp fontPosture)
{
	//Add your act code here...
	return true;//Modify this please!
}

bool FontDescriptionClass::AS3_isFontCompatible(Stringp fontName, Stringp fontWeight, Stringp fontPosture)
{
	//Add your act code here...
	return true;//Modify this please!
}

Stringp FontDescriptionObject::AS3_cffHinting_get()
{
	//Add your act code here...
	return m_strCffHinting;//Modify this please!
}

void FontDescriptionObject::AS3_cffHinting_set(Stringp cffHinting)
{
	//Add your act code here...
	m_strCffHinting=cffHinting;
}

Stringp FontDescriptionObject::AS3_fontLookup_get()
{
	//Add your act code here...
	return m_strFontLookup;//Modify this please!
}

void FontDescriptionObject::AS3_fontLookup_set(Stringp fontLookup)
{
	//Add your act code here...
	m_strFontLookup=fontLookup;
}

Stringp FontDescriptionObject::AS3_fontName_get()
{
	//Add your act code here...
	return m_strFontName;//Modify this please!
}

void FontDescriptionObject::AS3_fontName_set(Stringp fontName)
{
	//Add your act code here...
	m_strFontName=fontName;
}

Stringp FontDescriptionObject::AS3_fontPosture_get()
{
	//Add your act code here...
	return m_strFontPosture;//Modify this please!
}

void FontDescriptionObject::AS3_fontPosture_set(Stringp fontPosture)
{
	//Add your act code here...
	m_strFontPosture=fontPosture;
}

Stringp FontDescriptionObject::AS3_fontWeight_get()
{
	//Add your act code here...
	return m_strFontWeight;//Modify this please!
}

void FontDescriptionObject::AS3_fontWeight_set(Stringp fontWeight)
{
	//Add your act code here...
	m_strFontWeight=fontWeight;
}

bool FontDescriptionObject::AS3_locked_get()
{
	//Add your act code here...
	return m_bLocked;//Modify this please!
}

void FontDescriptionObject::AS3_locked_set(bool locked)
{
	//Add your act code here...
	m_bLocked=locked;
}

Stringp FontDescriptionObject::AS3_renderingMode_get()
{
	//Add your act code here...
	return m_strRenderingMode;//Modify this please!
}

void FontDescriptionObject::AS3_renderingMode_set(Stringp renderingMode)
{
	//Add your act code here...
	m_strRenderingMode=renderingMode;
}

//AS3 contructor function..
//AvmBox FontDescriptionObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontDescriptionObject::AS3_clone(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}

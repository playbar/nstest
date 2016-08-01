#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3FontMetrics.h"
#include "AS3Rectangle.h"

namespace avmshell{
FontMetricsClass::FontMetricsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FontMetricsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FontMetricsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

FontMetricsObject* FontMetricsClass::CreateObject(int nFontSize)
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	RectangleClass*pClass=top->getRectangleClass();
	RectangleObject*pRect=pClass->CreateRectangle(0,-nFontSize*2/3.0,nFontSize,nFontSize);
	//AvmCore c=core();
	Atom atoms[]={atom(),pRect->atom(),
				  core()->doubleToAtom(-nFontSize/4.5),
				  core()->doubleToAtom(nFontSize/20.0),
				  core()->doubleToAtom(nFontSize/16.0),
				  core()->doubleToAtom(nFontSize/20.0),
				  core()->doubleToAtom(0.075*nFontSize/24),
				  core()->doubleToAtom(0.6),
				  core()->doubleToAtom(-nFontSize/40.0),
				  core()->doubleToAtom(0.6)};


	return (FontMetricsObject*)(this->construct(9,atoms)&~7);
}

void FontMetricsObject::SetFontSize(int nFontSize)
{
	RectangleObject*box=(RectangleObject*)m_slots_FontMetricsObject.emBox;
	if(box)
		box->constructor(0,-nFontSize*2/3,nFontSize,nFontSize);
	m_slots_FontMetricsObject.strikethroughOffset=-nFontSize/4.5;
	m_slots_FontMetricsObject.strikethroughThickness=nFontSize/20.0;
	m_slots_FontMetricsObject.underlineOffset=nFontSize/16.0;
	m_slots_FontMetricsObject.underlineThickness=nFontSize/20.0;
	m_slots_FontMetricsObject.subscriptOffset=0.075*nFontSize/24;
	//m_slots_FontMetricsObject.subscriptScale=0.
	m_slots_FontMetricsObject.superscriptOffset=-nFontSize/5;

}

FontMetricsObject::FontMetricsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AvmBox FontMetricsObject::AS3_emBox_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_emBox_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_strikethroughOffset_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_strikethroughOffset_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_strikethroughThickness_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_strikethroughThickness_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_subscriptOffset_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_subscriptOffset_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_subscriptScale_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_subscriptScale_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_superscriptOffset_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_superscriptOffset_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_superscriptScale_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_superscriptScale_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_underlineOffset_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_underlineOffset_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_underlineThickness_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox FontMetricsObject::AS3_underlineThickness_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
////AS3 contructor function..
//AvmBox FontMetricsObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}
#include "StdAfxflash.h"
#include "ShellCore.h"
#include "Raster_gl.h"
using namespace avmplus;
#include "AS3ColorTransform.h"
namespace avmshell{
ColorTransformClass::ColorTransformClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ColorTransformObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ColorTransformClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ColorTransformObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ColorTransformObject* ColorTransformClass::CreateColorTransform(class XCXForm&form)
{
	ColorTransformObject*pObj=(ColorTransformObject*)createInstance(ivtable(),prototype);
	pObj->SetCXForm(form);
	return pObj;
}

ColorTransformObject::ColorTransformObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nColor=0;
}


uint32 ColorTransformObject::AS3_color_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_nColor) return m_nColor;
	return m_slots_ColorTransformObject._getColor();
	//return kAvmThunkUndefined;//Modify this please!
}

void ColorTransformObject::AS3_color_set(uint32 c)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	//m_slots_ColorTransformObject._setColor(c);
	m_nColor=c|0xff000000;
#ifdef _ARGB_PIXEL_
		SWAPPIXELRB(m_nColor);
#endif
}


//AS3 contructor function..
void ColorTransformObject::AS3_constructor(double ra,double ga,double ba,double aa,double rb,double gb,double bb,double ab)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	m_slots_ColorTransformObject.aa=aa;
	m_slots_ColorTransformObject.ba=ba;
	m_slots_ColorTransformObject.ga=ga;
	m_slots_ColorTransformObject.ra=ra;
	m_slots_ColorTransformObject.rb=rb;
	m_slots_ColorTransformObject.gb=gb;
	m_slots_ColorTransformObject.bb=bb;
	m_slots_ColorTransformObject.ab=ab;
	//return kAvmThunkUndefined;//Modify this please!
}

void ColorTransformObject::AS3_concat(ColorTransformObject*pObject)
{
	//Add your act code here...
	m_slots_ColorTransformObject._concat(&pObject->m_slots_ColorTransformObject);
	//return kAvmThunkUndefined;//Modify this please!
}

Stringp ColorTransformObject::AS3_toString()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	ShellCore* c=(ShellCore*)core();
	Stringp str=ScriptObject::formatString(c->kSpString,c->kEqualString,c->kQuotString,true,true,true);
	str=str->concatStrings(c->newConstantStringLatin1("("),str);
	str=str->concatStrings(str,c->newConstantStringLatin1(")"));
	return str;
	//return kAvmThunkUndefined;//Modify this please!
}

void ColorTransformObject::SetCXForm(class XCXForm&form)
{
#ifdef _ARGB_PIXEL_
	m_slots_ColorTransformObject.aa=(double)form.aa/256;
	m_slots_ColorTransformObject.ba=(double)form.ra/256;
	m_slots_ColorTransformObject.ra=(double)form.ba/256;
	m_slots_ColorTransformObject.ga=(double)form.ga/256;
	m_slots_ColorTransformObject.ab=form.ab;
	m_slots_ColorTransformObject.bb=form.rb;
	m_slots_ColorTransformObject.rb=form.bb;
	m_slots_ColorTransformObject.gb=form.gb;
	if(form.nForeColor)
	{
		m_nColor=form.nForeColor;
		SWAPPIXELRB(m_nColor);
	}
#else
	m_slots_ColorTransformObject.aa=(double)form.aa/256;
	m_slots_ColorTransformObject.ba=(double)form.ba/256;
	m_slots_ColorTransformObject.ra=(double)form.ra/256;
	m_slots_ColorTransformObject.ga=(double)form.ga/256;
	m_slots_ColorTransformObject.ab=form.ab;
	m_slots_ColorTransformObject.bb=form.bb;
	m_slots_ColorTransformObject.rb=form.rb;
	m_slots_ColorTransformObject.gb=form.gb;
	if(form.nForeColor)
		m_nColor=form.nForeColor;
#endif
}
void ColorTransformObject::GetCXForm(class XCXForm&form)
{
#ifdef _ARGB_PIXEL_
	form.aa=(S16)(m_slots_ColorTransformObject.aa*255);
	form.ba=(S16)(m_slots_ColorTransformObject.ra*255);
	form.ra=(S16)(m_slots_ColorTransformObject.ba*255);
	form.ga=(S16)(m_slots_ColorTransformObject.ga*255);
	form.ab=(S16)m_slots_ColorTransformObject.ab;
	form.bb=(S16)m_slots_ColorTransformObject.rb;
	form.rb=(S16)m_slots_ColorTransformObject.bb;
	form.gb=(S16)m_slots_ColorTransformObject.gb;
	form.nForeColor=m_nColor;
	form.CalcFlags();
#else
	form.aa=(S16)(m_slots_ColorTransformObject.aa*255);
	form.ba=(S16)(m_slots_ColorTransformObject.ba*255);
	form.ra=(S16)(m_slots_ColorTransformObject.ra*255);
	form.ga=(S16)(m_slots_ColorTransformObject.ga*255);
	form.ab=(S16)m_slots_ColorTransformObject.ab;
	form.bb=(S16)m_slots_ColorTransformObject.bb;
	form.rb=(S16)m_slots_ColorTransformObject.rb;
	form.gb=(S16)m_slots_ColorTransformObject.gb;
	form.nForeColor=m_nColor;
	form.CalcFlags();
#endif
}

}

#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Rectangle.h"
namespace avmshell{
RectangleClass::RectangleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())RectangleObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};

RectangleObject* RectangleClass::CreateRectangle(double l,double t,double w,int h)
{
	/*struct
	{
		Atom atom;
		double l,t,w,h;
	}args={0,l,t,w,h};
	return construct(4,(Atom*)&args);	*/

	RectangleObject*pRect=(RectangleObject*)createInstance(ivtable(),prototype);
	
	pRect->constructor(l,t,w,h);
	return pRect;
}

ScriptObject* RectangleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) RectangleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
RectangleObject::RectangleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}

bool RectangleObject::AS3_containsRect(RectangleObject*pRect)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	return m_slots_RectangleObject._containsRect(&pRect->m_slots_RectangleObject);
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}


RectangleObject* RectangleObject::AS3_intersection(RectangleObject*pRect)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	double x,y,w,h;
	m_slots_RectangleObject._intersection(&pRect->m_slots_RectangleObject,x,y,w,h);
	//if(w||h)
	//{
	//	m_slots_RectangleObject._intersection(&pRect->m_slots_RectangleObject,x,y,w,h);
		//int v=0;
	//}
	return ((ShellToplevel*)toplevel())->getRectangleClass()->CreateRectangle(x,y,w,h);//->atom();
	//return kAvmThunkUndefined;//Modify this please!
}

bool RectangleObject::AS3_intersects(RectangleObject*pRect)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return m_slots_RectangleObject._intersects(&pRect->m_slots_RectangleObject);
	//return kAvmThunkUndefined;//Modify this please!
}


RectangleObject* RectangleObject::AS3_union(RectangleObject*pRect)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	double x,y,w,h;
	m_slots_RectangleObject._union(&pRect->m_slots_RectangleObject,x,y,w,h);
	return ((ShellToplevel*)toplevel())->getRectangleClass()->CreateRectangle(x,y,w,h);//->atom();
	//return kAvmThunkUndefined;//Modify this please!
}

Stringp RectangleObject::AS3_toString()
{
	ShellCore*c=(ShellCore*)core();
	Stringp str=formatString(c->kSpString,c->kEqualString,c->kQuotString,true,true,true);
	str=str->concatStrings(c->newConstantStringLatin1("("),str);
	str=str->concatStrings(str,c->newConstantStringLatin1(")"));
	return str;
}
 //new adds 11
 void RectangleObject::AS3_setTo(double xa, double ya, double widtha, double heighta)
 {
	m_slots_RectangleObject.left = xa;
	m_slots_RectangleObject.top  = ya;
	m_slots_RectangleObject.width = widtha;
	m_slots_RectangleObject.height = heighta;
	//LOGWHERE();
 }
 //new adds 11 end
}
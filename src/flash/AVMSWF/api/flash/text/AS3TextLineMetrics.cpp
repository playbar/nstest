#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextLineMetrics.h"
namespace avmshell{
TextLineMetricsClass::TextLineMetricsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextLineMetricsObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextLineMetricsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextLineMetricsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextLineMetricsObject::TextLineMetricsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
void TextLineMetricsObject::AS3_constructor(double x,double w,double h,double ascent,double descent,double leading)
{
	m_x=x;
	m_width=w;
	m_height=h;
	m_ascent=ascent;
	m_descent=descent;
	m_leading=leading;
}
}
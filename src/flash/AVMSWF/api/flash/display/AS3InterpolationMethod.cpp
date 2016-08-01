#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3InterpolationMethod.h"
namespace avmshell{
InterpolationMethodClass::InterpolationMethodClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())InterpolationMethodObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* InterpolationMethodClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InterpolationMethodObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InterpolationMethodObject::InterpolationMethodObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
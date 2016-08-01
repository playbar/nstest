#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DisplacementMapFilterMode.h"
namespace avmshell{
DisplacementMapFilterModeClass::DisplacementMapFilterModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())DisplacementMapFilterModeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DisplacementMapFilterModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DisplacementMapFilterModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DisplacementMapFilterModeObject::DisplacementMapFilterModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
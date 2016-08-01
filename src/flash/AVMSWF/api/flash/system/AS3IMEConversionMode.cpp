#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IMEConversionMode.h"
namespace avmshell{
IMEConversionModeClass::IMEConversionModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())IMEConversionModeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* IMEConversionModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IMEConversionModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IMEConversionModeObject::IMEConversionModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
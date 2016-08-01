#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3LineScaleMode.h"
namespace avmshell{
LineScaleModeClass::LineScaleModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())LineScaleModeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* LineScaleModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LineScaleModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
LineScaleModeObject::LineScaleModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
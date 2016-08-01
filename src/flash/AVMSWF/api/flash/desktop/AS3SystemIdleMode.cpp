#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SystemIdleMode.h"
namespace avmshell{
SystemIdleModeClass::SystemIdleModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SystemIdleModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SystemIdleModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SystemIdleModeObject::SystemIdleModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3HTMLSWFCapability.h"
namespace avmshell{
HTMLSWFCapabilityClass::HTMLSWFCapabilityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* HTMLSWFCapabilityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) HTMLSWFCapabilityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
HTMLSWFCapabilityObject::HTMLSWFCapabilityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
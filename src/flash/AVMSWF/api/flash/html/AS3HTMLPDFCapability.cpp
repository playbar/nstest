#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3HTMLPDFCapability.h"
namespace avmshell{
HTMLPDFCapabilityClass::HTMLPDFCapabilityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* HTMLPDFCapabilityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) HTMLPDFCapabilityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
HTMLPDFCapabilityObject::HTMLPDFCapabilityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
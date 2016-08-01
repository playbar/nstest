#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MemoryError.h"
namespace avmshell{
MemoryErrorClass::MemoryErrorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())MemoryErrorObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* MemoryErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MemoryErrorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MemoryErrorObject::MemoryErrorObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorObject(vtable, proto)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
AvmBox MemoryErrorObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}
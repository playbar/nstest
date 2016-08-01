#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ObjectEncoding.h"
namespace avmshell{
ObjectEncodingClass::ObjectEncodingClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ObjectEncodingObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ObjectEncodingClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ObjectEncodingObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ObjectEncodingObject::ObjectEncodingObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox ObjectEncodingObject::AS3_dynamicPropertyWriter_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ObjectEncodingObject::AS3_dynamicPropertyWriter_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}
#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ScriptTimeoutError.h"
namespace avmshell{
ScriptTimeoutErrorClass::ScriptTimeoutErrorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ScriptTimeoutErrorObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ScriptTimeoutErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ScriptTimeoutErrorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ScriptTimeoutErrorObject::ScriptTimeoutErrorObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorObject(vtable, proto)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
AvmBox ScriptTimeoutErrorObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}
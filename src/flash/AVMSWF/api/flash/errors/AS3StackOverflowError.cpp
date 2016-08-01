#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StackOverflowError.h"
namespace avmshell{
StackOverflowErrorClass::StackOverflowErrorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StackOverflowErrorObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StackOverflowErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StackOverflowErrorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StackOverflowErrorObject::StackOverflowErrorObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorObject(vtable, proto)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
AvmBox StackOverflowErrorObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}
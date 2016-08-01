#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3URLVariables.h"
namespace avmshell{
URLVariablesClass::URLVariablesClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())URLVariablesObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* URLVariablesClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) URLVariablesObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

URLVariablesObject* URLVariablesClass::CreateVars(Stringp s)
{
	Atom atoms[]={this->atom(),s->atom()};
	return (URLVariablesObject*)this->construct(1,atoms);
}

URLVariablesObject::URLVariablesObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}

//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
/*AvmBox URLVariablesObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox URLVariablesObject::AS3_decode(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox URLVariablesObject::AS3_toString(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}*/

}
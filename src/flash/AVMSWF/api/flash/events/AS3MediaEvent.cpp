#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MediaEvent.h"
namespace avmshell{
MediaEventClass::MediaEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MediaEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MediaEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MediaEventObject::MediaEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
MediaPromiseObject* MediaEventObject::AS3_data_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return NULL;
}

void MediaEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, MediaPromiseObject *pData)
{

}
EventObject* MediaEventObject::AS3_clone()
{
	return NULL;
}
Stringp MediaEventObject::AS3_toString()
{
	return NULL;
}

}
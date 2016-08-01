#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3HTMLUncaughtScriptExceptionEvent.h"
namespace avmshell{
HTMLUncaughtScriptExceptionEventClass::HTMLUncaughtScriptExceptionEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* HTMLUncaughtScriptExceptionEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) HTMLUncaughtScriptExceptionEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
HTMLUncaughtScriptExceptionEventObject::HTMLUncaughtScriptExceptionEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Atom HTMLUncaughtScriptExceptionEventObject::AS3_exceptionValue_get()
{
	return kAvmThunkUndefined;
}
void HTMLUncaughtScriptExceptionEventObject::AS3_exceptionValue_set(Atom  exceptionValue)
{

}
ArrayObject* HTMLUncaughtScriptExceptionEventObject::AS3_stackTrace_get()
{
	return NULL;
}
void HTMLUncaughtScriptExceptionEventObject::AS3_stackTrace_set(ArrayObject *pStackTrace)
{

}

void HTMLUncaughtScriptExceptionEventObject::AS3_constructor(Atom exceptionValue)
{

}
EventObject* HTMLUncaughtScriptExceptionEventObject::AS3_clone()
{
	return NULL;
}

}
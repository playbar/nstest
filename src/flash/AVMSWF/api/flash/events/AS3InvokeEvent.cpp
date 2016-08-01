#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3InvokeEvent.h"
namespace avmshell{
InvokeEventClass::InvokeEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* InvokeEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InvokeEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InvokeEventObject::InvokeEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* InvokeEventObject::AS3_arguments_get()
{
	return NULL;
}
FileObject* InvokeEventObject::AS3_currentDirectory_get()
{
	return NULL;
}
Stringp InvokeEventObject::AS3_reason_get()
{
	return NULL;
}

void InvokeEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, FileObject *pDir, ArrayObject *pArgv, Stringp pReason)
{

}
EventObject* InvokeEventObject::AS3_clone()
{
	return NULL;
}

}
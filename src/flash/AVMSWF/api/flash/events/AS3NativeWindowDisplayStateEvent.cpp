#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeWindowDisplayStateEvent.h"
namespace avmshell{
NativeWindowDisplayStateEventClass::NativeWindowDisplayStateEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeWindowDisplayStateEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeWindowDisplayStateEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeWindowDisplayStateEventObject::NativeWindowDisplayStateEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp NativeWindowDisplayStateEventObject::AS3_afterDisplayState_get()
{
	return NULL;
}
Stringp NativeWindowDisplayStateEventObject::AS3_beforeDisplayState_get()
{
	return NULL;
}

void NativeWindowDisplayStateEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
														  Stringp pBeforeDisplayState, Stringp pAfterDisplayState)
{

}
EventObject* NativeWindowDisplayStateEventObject::AS3_clone()
{
	return NULL;
}
Stringp NativeWindowDisplayStateEventObject::AS3_toString()
{
	return NULL;
}

}
#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLErrorEvent.h"
namespace avmshell{
SQLErrorEventClass::SQLErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//ErrorEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLErrorEventObject::SQLErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
SQLErrorObject* SQLErrorEventObject::AS3_error_get()
{
	return NULL;
}

void SQLErrorEventObject::AS3_constructor(Stringp type, bool bubbles, bool cancelable, SQLErrorObject *pError)
{

}
EventObject* SQLErrorEventObject::AS3_clone()
{
	return NULL;
}
Stringp SQLErrorEventObject::AS3_toString()
{
	return NULL;
}

}
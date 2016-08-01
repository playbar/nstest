#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLUpdateEvent.h"
namespace avmshell{
SQLUpdateEventClass::SQLUpdateEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLUpdateEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLUpdateEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLUpdateEventObject::SQLUpdateEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
double SQLUpdateEventObject::AS3_rowID_get()
{
	return 0;
}
Stringp SQLUpdateEventObject::AS3_table_get()
{
	return NULL;
}

void SQLUpdateEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pTable, double rowID)
{

}
EventObject* SQLUpdateEventObject::AS3_clone()
{
	return NULL;
}

}
#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLEvent.h"
namespace avmshell{
SQLEventClass::SQLEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLEventObject::SQLEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void SQLEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable)
{
	//Add your act code here...
}

EventObject* SQLEventObject::AS3_clone()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

}
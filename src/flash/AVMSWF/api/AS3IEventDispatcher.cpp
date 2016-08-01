#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "../Inc/AS3IEventDispatcher.h"
namespace avmshell{
IEventDispatcherClass::IEventDispatcherClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IEventDispatcherClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IEventDispatcherObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IEventDispatcherObject::IEventDispatcherObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
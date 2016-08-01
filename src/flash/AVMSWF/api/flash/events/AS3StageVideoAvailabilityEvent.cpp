#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageVideoAvailabilityEvent.h"
namespace avmshell{
StageVideoAvailabilityEventClass::StageVideoAvailabilityEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StageVideoAvailabilityEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageVideoAvailabilityEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageVideoAvailabilityEventObject::StageVideoAvailabilityEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp StageVideoAvailabilityEventObject::AS3_availability_get()
//{
//	//Add your act code here...
//	return NULL;//Modify this please!
//}
//
////AS3 contructor function..
//void StageVideoAvailabilityEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp availability)
//{
//	//Add your act code here...
//}

}
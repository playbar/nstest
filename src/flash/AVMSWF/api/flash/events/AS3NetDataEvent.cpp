#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetDataEvent.h"
namespace avmshell{
NetDataEventClass::NetDataEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetDataEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetDataEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetDataEventObject::NetDataEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Atom NetDataEventObject::AS3_info_get()
//{
//	return kAvmThunkUndefined;
//}
//double NetDataEventObject::AS3_timestamp_get()
//{
//	return 0;
//}
//
//void NetDataEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double timestamp, Atom info)
//{
//
//}
//EventObject* NetDataEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp NetDataEventObject::AS3_toString()
//{
//	return NULL;
//}

}
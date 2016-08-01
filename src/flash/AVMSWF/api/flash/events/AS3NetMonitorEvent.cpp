#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetMonitorEvent.h"
namespace avmshell{
NetMonitorEventClass::NetMonitorEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetMonitorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetMonitorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetMonitorEventObject::NetMonitorEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//NetStreamObject* NetMonitorEventObject::AS3_netStream_get()
//{
//	return NULL;
//}
//
//void NetMonitorEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, NetStreamObject *pNetStream)
//{
//
//}
//EventObject* NetMonitorEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp NetMonitorEventObject::AS3_toString()
//{
//	return NULL;
//}

}
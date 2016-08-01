#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetMonitor.h"
namespace avmshell{
NetMonitorClass::NetMonitorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetMonitorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetMonitorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetMonitorObject::NetMonitorObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* NetMonitorObject::AS3_listStreams()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

}
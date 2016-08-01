#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DNSResolverEvent.h"
namespace avmshell{
DNSResolverEventClass::DNSResolverEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DNSResolverEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DNSResolverEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DNSResolverEventObject::DNSResolverEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp DNSResolverEventObject::AS3_host_get()
{
	return NULL;
}
void DNSResolverEventObject::AS3_host_set(Stringp pHost)
{

}
ArrayObject* DNSResolverEventObject::AS3_resourceRecords_get()
{
	return NULL;
}
void DNSResolverEventObject::AS3_resourceRecords_set(ArrayObject *pResourceRecords)
{

}

void DNSResolverEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pHost, ArrayObject pResourceRecords)
{

}
EventObject* DNSResolverEventObject::AS3_clone()
{
	return NULL;
}
Stringp DNSResolverEventObject::AS3_toString()
{
	return NULL;
}

}
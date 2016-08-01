#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3InterfaceAddress.h"
namespace avmshell{
InterfaceAddressClass::InterfaceAddressClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* InterfaceAddressClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InterfaceAddressObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InterfaceAddressObject::InterfaceAddressObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp InterfaceAddressObject::AS3_address_get()
{
	return NULL;
}
void InterfaceAddressObject::AS3_address_set(Stringp pAddress)
{

}
Stringp InterfaceAddressObject::AS3_broadcast_get()
{
	return NULL;
}
void InterfaceAddressObject::AS3_broadcast_set(Stringp pBroadCast)
{

}
Stringp InterfaceAddressObject::AS3_ipVersion_get()
{
	return NULL;
}
void InterfaceAddressObject::AS3_ipVersion_set(Stringp pIpVersion)
{

}
int InterfaceAddressObject::AS3_prefixLength_get()
{
	return 0;
}
void InterfaceAddressObject::AS3_prefixLength_set(int prefixLength)
{

}

}
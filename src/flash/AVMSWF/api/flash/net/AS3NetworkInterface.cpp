#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetworkInterface.h"
namespace avmshell{
NetworkInterfaceClass::NetworkInterfaceClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetworkInterfaceClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetworkInterfaceObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetworkInterfaceObject::NetworkInterfaceObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool NetworkInterfaceObject::AS3_active_get()
{
	return false;
}
void NetworkInterfaceObject::AS3_active_set()
{

}
AvmBox NetworkInterfaceObject::AS3_addresses_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}
AvmBox NetworkInterfaceObject::AS3_addresses_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}
Stringp NetworkInterfaceObject::AS3_displayName_get()
{
	return NULL;
}
void NetworkInterfaceObject::AS3_displayName_set(Stringp pDisplayName)
{

}
Stringp NetworkInterfaceObject::AS3_hardwareAddress_get()
{
	return NULL;
}
void NetworkInterfaceObject::AS3_hardwareAddress_set(Stringp pHardwareAddress)
{

}
int NetworkInterfaceObject::AS3_mtu_get()
{
	return 0;
}
void NetworkInterfaceObject::AS3_mtu_set(int mtu)
{

}
Stringp NetworkInterfaceObject::AS3_name_get()
{
	return NULL;
}
void NetworkInterfaceObject::AS3_name_set(Stringp pName)
{

}
NetworkInterfaceObject* NetworkInterfaceObject::AS3_parent_get()
{
	return NULL;
}
void NetworkInterfaceObject::AS3_parent_set(NetworkInterfaceObject *pParent)
{

}
AvmBox NetworkInterfaceObject::AS3_subInterfaces_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}
AvmBox NetworkInterfaceObject::AS3_subInterfaces_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}

}
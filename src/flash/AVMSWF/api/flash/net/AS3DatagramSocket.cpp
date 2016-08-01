#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DatagramSocket.h"
namespace avmshell{
DatagramSocketClass::DatagramSocketClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DatagramSocketClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DatagramSocketObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DatagramSocketObject::DatagramSocketObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool DatagramSocketObject::AS3_isSupported_get()
{
	return false;
}
bool DatagramSocketObject::AS3_bound_get()
{
	return false;
}
bool DatagramSocketObject::AS3_connected_get()
{
	return false;
}
Stringp DatagramSocketObject::AS3_localAddress_get()
{
	return NULL;
}
int DatagramSocketObject::AS3_localPort_get()
{
	return 0;
}
Stringp DatagramSocketObject::AS3_remoteAddress_get()
{
	return NULL;
}
int DatagramSocketObject::AS3_remotePort_get()
{
	return 0;
}

void DatagramSocketObject::AS3_constructor()
{

}
void DatagramSocketObject::AS3_bind(int localPort, Stringp pLocalAddress)
{

}
void DatagramSocketObject::AS3_close()
{

}
void DatagramSocketObject::AS3_connect(Stringp pRemoteAddress, int remotePort)
{

}
void DatagramSocketObject::AS3_receive()
{

}
void DatagramSocketObject::AS3_send(ByteArrayObject *pBytes, uint32_t offset, uint32_t length, Stringp address, int port)
{

}

}
#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ServerSocketConnectEvent.h"
namespace avmshell{
ServerSocketConnectEventClass::ServerSocketConnectEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ServerSocketConnectEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ServerSocketConnectEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ServerSocketConnectEventObject::ServerSocketConnectEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
SocketObject* ServerSocketConnectEventObject::AS3_socket_get()
{
	return NULL;
}
void ServerSocketConnectEventObject::AS3_socket_set(SocketObject *pSocket)
{

}

void ServerSocketConnectEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, SocketObject *pSocket)
{

}
EventObject* ServerSocketConnectEventObject::AS3_clone()
{
	return NULL;
}
Stringp ServerSocketConnectEventObject::AS3_toString()
{
	return NULL;
}

}
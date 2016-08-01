#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ServerSocket.h"
namespace avmshell{
ServerSocketClass::ServerSocketClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ServerSocketClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ServerSocketObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ServerSocketObject::ServerSocketObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool ServerSocketObject::AS3_isSupported_get()
{
	return false;
}
bool ServerSocketObject::AS3_bound_get()
{
	return false;
}
bool ServerSocketObject::AS3_listening_get()
{
	return false;
}
Stringp ServerSocketObject::AS3_localAddress_get()
{
	return NULL;
}
int ServerSocketObject::AS3_localPort_get()
{
	return 0;
}

void ServerSocketObject::AS3_constructor()
{

}
void ServerSocketObject::AS3_bind(int localPort, Stringp localAddress)
{

}
void ServerSocketObject::AS3_close()
{

}
void ServerSocketObject::AS3_listen(int backlog)
{

}

}
#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SecureSocket.h"
namespace avmshell{
SecureSocketClass::SecureSocketClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SecureSocketClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SecureSocketObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SecureSocketObject::SecureSocketObject(VTable *vtable, ScriptObject* proto, int capacity): SocketObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool SecureSocketClass::AS3_isSupported_get()
{
	return false;
}
Stringp SecureSocketObject::AS3_serverCertificateStatus_get()
{
	return NULL;
}
void SecureSocketObject::AS3_constructor()
{

}
void SecureSocketObject::AS3_connect(Stringp pHost, int port)
{

}

X509CertificateObject* SecureSocketObject::AS3_serverCertificate_get() //new add
{
	LOGWHERE();
	return NULL;
}
void SecureSocketObject::AS3_addBinaryChainBuildingCertificate(ByteArrayObject* pCertificate, bool trusted)  //new add
{
	LOGWHERE();
	
}
}
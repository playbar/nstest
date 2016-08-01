#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMAuthenticateEvent.h"
namespace avmshell{
DRMAuthenticateEventClass::DRMAuthenticateEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMAuthenticateEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMAuthenticateEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMAuthenticateEventObject::DRMAuthenticateEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp DRMAuthenticateEventObject::AS3_authenticationType_get()
//{
//	return NULL;
//}
//Stringp DRMAuthenticateEventObject::AS3_header_get()
//{
//	return NULL;
//}
//NetStreamObject* DRMAuthenticateEventObject::AS3_netstream_get()
//{
//	return NULL;
//}
//Stringp DRMAuthenticateEventObject::AS3_passwordPrompt_get()
//{
//	return NULL;
//}
//Stringp DRMAuthenticateEventObject::AS3_urlPrompt_get()
//{
//	return NULL;
//}
//Stringp DRMAuthenticateEventObject::AS3_usernamePrompt_get()
//{
//	return NULL;
//}
//
//void DRMAuthenticateEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
//												 Stringp pHeader, Stringp pUserPrompt, Stringp pPassPrompt, Stringp pUrlPrompt, 
//												 String pAuthenticationType, NetStreamObject *pNetstream)
//{
//
//}
//EventObject* DRMAuthenticateEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp DRMAuthenticateEventObject::AS3_toString()
//{
//	return NULL;
//}

}
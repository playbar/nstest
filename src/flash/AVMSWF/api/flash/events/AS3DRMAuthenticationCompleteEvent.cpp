#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMAuthenticationCompleteEvent.h"
namespace avmshell{
DRMAuthenticationCompleteEventClass::DRMAuthenticationCompleteEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMAuthenticationCompleteEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMAuthenticationCompleteEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMAuthenticationCompleteEventObject::DRMAuthenticationCompleteEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp DRMAuthenticationCompleteEventObject::AS3_domain_get()
//{
//	return NULL;
//}
//void DRMAuthenticationCompleteEventObject::AS3_domain_set(Stringp pDomain)
//{
//
//}
//Stringp DRMAuthenticationCompleteEventObject::AS3_serverURL_get()
//{
//	return NULL;
//}
//void DRMAuthenticationCompleteEventObject::AS3_serverURL_set(Stringp pServerURL)
//{
//
//}
//ByteArrayObject* DRMAuthenticationCompleteEventObject::AS3_token_get()
//{
//	return NULL;
//}
//void DRMAuthenticationCompleteEventObject::AS3_token_set(ByteArrayObject *pToken)
//{
//
//}
//
//void DRMAuthenticationCompleteEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pInServerURL, 
//														   Stringp pInDomain, ByteArrayObject *pInToken)
//{
//
//}
//EventObject* DRMAuthenticationCompleteEventObject::AS3_clone()
//{
//	return NULL;
//}

}
#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMAuthenticationErrorEvent.h"
namespace avmshell{
DRMAuthenticationErrorEventClass::DRMAuthenticationErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//ErrorEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMAuthenticationErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMAuthenticationErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMAuthenticationErrorEventObject::DRMAuthenticationErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp DRMAuthenticationErrorEventObject::AS3_domain_get()
//{
//	return NULL;
//}
//void DRMAuthenticationErrorEventObject::AS3_domain_set(Stringp pDomain)
//{
//
//}
//Stringp DRMAuthenticationErrorEventObject::AS3_serverURL_get()
//{
//	return NULL;
//}
//void DRMAuthenticationErrorEventObject::AS3_serverURL_set(Stringp pServerURL)
//{
//
//}
//int DRMAuthenticationErrorEventObject::AS3_subErrorID_get()
//{
//	return 0;
//}
//void DRMAuthenticationErrorEventObject::AS3_subErrorID_set(int subErrorID)
//{
//
//}
//
//void DRMAuthenticationErrorEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, String pInDetail, 
//														int inErrorID, int inSubErrorID, Stringp pInServerURL, Stringp pInDomain)
//{
//
//}
//EventObject* DRMAuthenticationErrorEventObject::AS3_clone()
//{
//	return NULL;
//}

}
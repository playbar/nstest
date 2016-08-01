#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMErrorEvent.h"
namespace avmshell{
DRMErrorEventClass::DRMErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//ErrorEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMErrorEventObject::DRMErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//DRMContentDataObject* DRMErrorEventObject::AS3_contentData_get()
//{
//	return NULL;
//}
//void DRMErrorEventObject::AS3_contentData_set(DRMContentDataObject *pContentData)
//{
//
//}
//bool DRMErrorEventObject::AS3_drmUpdateNeeded_get()
//{
//	return false;
//}
//int DRMErrorEventObject::AS3_subErrorID_get()
//{
//	return 0;
//}
//bool DRMErrorEventObject::AS3_systemUpdateNeeded_get()
//{
//	return false;
//}
//
//void DRMErrorEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pInErrorDetail, 
//										  int inErrorCode, int insubErrorID, DRMContentDataObject *pInMetadata, 
//										  bool inSystemUpdateNeeded, bool inDrmUpdateNeeded)
//{
//
//}
//EventObject* DRMErrorEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp DRMErrorEventObject::AS3_toString()
//{
//	return NULL;
//}

}
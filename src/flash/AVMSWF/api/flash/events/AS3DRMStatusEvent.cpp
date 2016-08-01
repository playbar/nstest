#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMStatusEvent.h"
namespace avmshell{
DRMStatusEventClass::DRMStatusEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMStatusEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMStatusEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMStatusEventObject::DRMStatusEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//DRMContentDataObject* DRMStatusEventObject::AS3_contentData_get()
//{
//	return NULL;
//}
//void DRMStatusEventObject::AS3_contentData_set(DRMContentDataObject *pContentData)
//{
//
//}
//Stringp DRMStatusEventObject::AS3_detail_get()
//{
//	return NULL;
//}
//bool DRMStatusEventObject::AS3_isAnonymous_get()
//{
//	return false;
//}
//bool DRMStatusEventObject::AS3_isAvailableOffline_get()
//{
//	return false;
//}
//bool DRMStatusEventObject::AS3_isLocal_get()
//{
//	return false;
//}
//void DRMStatusEventObject::AS3_isLocal_set(bool isLocal)
//{
//
//}
//int DRMStatusEventObject::AS3_offlineLeasePeriod_get()
//{
//	return 0;
//}
//Atom DRMStatusEventObject::AS3_policies_get()
//{
//	return kAvmThunkUndefined;
//}
//DRMVoucherObject* DRMStatusEventObject::AS3_voucher_get()
//{
//	return NULL;
//}
//void DRMStatusEventObject::AS3_voucher_set(DRMVoucherObject *pVoucher)
//{
//
//}
//DateObject* DRMStatusEventObject::AS3_voucherEndDate_get()
//{
//	return NULL;
//}
//
//void DRMStatusEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
//										   DRMContentDataObject *pInMetadata, DRMVoucherObject *pInVoucher, bool inLocal)
//{
//
//}
//EventObject* DRMStatusEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp DRMStatusEventObject::AS3_toString()
//{
//	return NULL;
//}

}
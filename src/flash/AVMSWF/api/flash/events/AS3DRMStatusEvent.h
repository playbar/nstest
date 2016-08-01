#ifndef _AS3DRMStatusEvent_
#define _AS3DRMStatusEvent_

#include "AS3Event.h"
#include "AS3DRMContentData.h"
#include "AS3DRMVoucher.h"

namespace avmplus{namespace NativeID{

	class DRMVoucherObject;
	class DRMContentDataObject;

class DRMStatusEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DRM_STATUS : String = "drmStatus"

	Stringp DRM_STATUS;

private:};
class DRMStatusEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp m_detail;
	DRMVoucherObject *m_voucher;
	DRMContentDataObject *m_metadata;
	XBOOL m_isLocal;
private:};
}}
namespace avmshell{
	class DRMStatusEventClass : public ClassClosure//EventClass
	{
	public:
		DRMStatusEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMStatusEventClassSlots m_slots_DRMStatusEventClass;
};
class DRMStatusEventObject : public EventObject
{
	public:
		DRMStatusEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//DRMContentDataObject* AS3_contentData_get();
		//void AS3_contentData_set(DRMContentDataObject *pContentData);
		//Stringp AS3_detail_get();
		//bool AS3_isAnonymous_get();
		//bool AS3_isAvailableOffline_get();
		//bool AS3_isLocal_get();
		//void AS3_isLocal_set(bool isLocal);
		//int AS3_offlineLeasePeriod_get();
		//Atom AS3_policies_get();
		//DRMVoucherObject* AS3_voucher_get();
		//void AS3_voucher_set(DRMVoucherObject *pVoucher);
		//DateObject* AS3_voucherEndDate_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
		//						DRMContentDataObject *pInMetadata, DRMVoucherObject *pInVoucher, bool inLocal);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMStatusEventObjectSlots m_slots_DRMStatusEventObject;
};}
#endif
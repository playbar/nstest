#ifndef _AS3DRMVoucherStoreContext_
#define _AS3DRMVoucherStoreContext_

#include "AS3DRMManagerSession.h"

namespace avmplus{namespace NativeID{
class DRMVoucherStoreContextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DRMVoucherStoreContextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DRMVoucherStoreContextClass : public ClassClosure//DRMManagerSessionClass
	{
	public:
		DRMVoucherStoreContextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMVoucherStoreContextClassSlots m_slots_DRMVoucherStoreContextClass;
};
class DRMVoucherStoreContextObject : public DRMManagerSessionObject
{
	public:
		DRMVoucherStoreContextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		uint32_t AS3_getVoucherFromStoreInner(DRMContentDataObject *pData){return 0;}
		DRMVoucherObject* AS3_getVoucherInner(){return NULL;}

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMVoucherStoreContextObjectSlots m_slots_DRMVoucherStoreContextObject;
};}
#endif
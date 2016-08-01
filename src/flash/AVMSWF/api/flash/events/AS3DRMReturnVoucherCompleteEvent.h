#ifndef _AS3DRMReturnVoucherCompleteEvent_
#define _AS3DRMReturnVoucherCompleteEvent_

#include "AS3Event.h"

namespace avmplus
{
	namespace NativeID
	{
		class DRMReturnVoucherCompleteEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//RETURN_VOUCHER_COMPLETE:String = "returnVoucherComplete"

			Stringp RETURN_VOUCHER_COMPLETE;
		private:
		};
		class DRMReturnVoucherCompleteEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
			//Stringp m_licenseID;
			//int m_numberOfVouchersReturned;
			//Stringp m_policyID;
			//Stringp m_serverURL;
		private:
		};
	}
}
namespace avmshell{
	class DRMReturnVoucherCompleteEventClass : public ClassClosure//EventClass
	{
	public:
		DRMReturnVoucherCompleteEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMReturnVoucherCompleteEventClassSlots m_slots_DRMReturnVoucherCompleteEventClass;
	public:
		inline Stringp getSlotRETURN_VOUCHER_COMPLETE(){return m_slots_DRMReturnVoucherCompleteEventClass.RETURN_VOUCHER_COMPLETE;}
	};
	class DRMReturnVoucherCompleteEventObject : public EventObject
	{
	public:
		DRMReturnVoucherCompleteEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

		Stringp m_licenseID;
		int m_numberOfVouchersReturned;
		Stringp m_policyID;
		Stringp m_serverURL;

		Stringp AS3_licenseID_get();
		void AS3_licenseID_set(Stringp licenseID);
		int AS3_numberOfVouchersReturned_get();
		void AS3_numberOfVouchersReturned_set(int numOfVR);
	    
		Stringp AS3_policyID_get();
		void AS3_policyID_set(Stringp policyID);
	    
		Stringp AS3_serverURL_get();
		void AS3_serverURL_set(Stringp serverURL);

		virtual EventObject* Clone(EventObject*);
		DRMReturnVoucherCompleteEventObject* AS3_clone();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMReturnVoucherCompleteEventObjectSlots m_slots_DRMReturnVoucherCompleteEventObject;
	};
}
#endif
#ifndef _AS3DRMReturnVoucherErrorEvent_
#define _AS3DRMReturnVoucherErrorEvent_

#include "AS3ErrorEvent.h"

namespace avmplus
{
	namespace NativeID
	{
		class DRMReturnVoucherErrorEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//RETURN_VOUCHER_ERROR:String = "returnVoucherError"

			Stringp RETURN_VOUCHER_ERROR;
		private:
		};
		class DRMReturnVoucherErrorEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!

		private:
		};
	}
}
namespace avmshell{
	class DRMReturnVoucherErrorEventClass : public ClassClosure//EventClass
	{
	public:
		DRMReturnVoucherErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotRETURN_VOUCHER_ERROR(){return m_slots_DRMReturnVoucherErrorEventClass.RETURN_VOUCHER_ERROR;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMReturnVoucherErrorEventClassSlots m_slots_DRMReturnVoucherErrorEventClass;
	};
	class DRMReturnVoucherErrorEventObject : public ErrorEventObject
	{
	public:
		DRMReturnVoucherErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

		Stringp m_licenseID;
		Stringp m_policyID;
		Stringp m_serverURL;
		int  m_subErrorID;

		Stringp AS3_licenseID_get();
		void AS3_licenseID_set(Stringp licenseID);
	    
		Stringp AS3_policyID_get();
		void AS3_policyID_set(Stringp policyID);
	    
		Stringp AS3_serverURL_get();
		void AS3_serverURL_set(Stringp serverURL);
	    
		int AS3_subErrorID_get();
		void AS3_subErrorID_set(int subErrorID);

		virtual EventObject* Clone(EventObject*);
		DRMReturnVoucherErrorEventObject* AS3_clone();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMReturnVoucherErrorEventObjectSlots m_slots_DRMReturnVoucherErrorEventObject;
	};
}
#endif
#ifndef _AS3DRMDeviceGroup_
#define _AS3DRMDeviceGroup_

#include "AS3AuthenticationMethod.h"
namespace avmplus
{
	namespace NativeID
	{
		class DRMDeviceGroupClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
	

		private:
		};
		class DRMDeviceGroupObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class DRMDeviceGroupClass : public ClassClosure//EventClass
	{
	public:
		DRMDeviceGroupClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMDeviceGroupClassSlots m_slots_DRMDeviceGroupClass;
	};
	class DRMDeviceGroupObject : public ScriptObject
	{
	public:
		DRMDeviceGroupObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

		Stringp AS3_authenticationMethod_get();
		
		Stringp AS3_domain_get();
		
		Stringp AS3_name_get();
		void AS3_name_set(Stringp value);
		
		Stringp AS3_serverURL_get();
	public:
		Stringp m_strAuthenticationMethod;
		Stringp m_strDomain;
		Stringp m_strName;
		Stringp m_strServerURL;

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMDeviceGroupObjectSlots m_slots_DRMDeviceGroupObject;
	};
}
#endif
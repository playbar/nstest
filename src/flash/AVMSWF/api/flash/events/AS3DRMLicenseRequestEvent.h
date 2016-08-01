#ifndef _AS3DRMLicenseRequestEvent_
#define _AS3DRMLicenseRequestEvent_

#include "AS3Event.h"

namespace avmplus
{
	namespace NativeID
	{
		class DRMLicenseRequestEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//MEDIA_TYPE_DATA : String = "mediaTypeData"

			Stringp LICENSE_REQUEST;

		private:
		};
		class DRMLicenseRequestEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
			
			//LICENSE_REQUEST:String = "licenseRequest"

			//Stringp m_serverURL;
		private:
		};
	}
}
namespace avmshell{
	class DRMLicenseRequestEventClass : public ClassClosure//EventClass
	{
	public:
		DRMLicenseRequestEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		inline Stringp getSlotLICENSE_REQUEST(){return m_slots_DRMLicenseRequestEventClass.LICENSE_REQUEST;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMLicenseRequestEventClassSlots m_slots_DRMLicenseRequestEventClass;
	};
	class DRMLicenseRequestEventObject : public EventObject
	{
	public:
		DRMLicenseRequestEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp m_serverURL;
		Stringp AS3_serverURL_get();
		void AS3_serverURL_set(Stringp serverURL);
	public:
		virtual EventObject* Clone(EventObject *pObj);
		DRMLicenseRequestEventObject* AS3_clone();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMLicenseRequestEventObjectSlots m_slots_DRMLicenseRequestEventObject;
	};
}
#endif
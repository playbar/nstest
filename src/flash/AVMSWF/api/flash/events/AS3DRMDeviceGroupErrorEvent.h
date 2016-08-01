#ifndef _AS3DRMDeviceGroupErrorEvent_
#define _AS3DRMDeviceGroupErrorEvent_

#include "AS3Event.h"
#include "AS3ErrorEvent.h"
#include "AS3TextEvent.h"
#include "AS3DRMDeviceGroup.h"

namespace avmplus
{
	namespace NativeID
	{
		class DRMDeviceGroupErrorEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			//ADD_TO_DEVICE_GROUP_ERROR:String = "addToDeviceGroupError"
			//REMOVE_FROM_DEVICE_GROUP_ERROR:String = "removeFromDeviceGroupError"

			Stringp ADD_TO_DEVICE_GROUP_ERROR;
			Stringp REMOVE_FROM_DEVICE_GROUP_ERROR;

		private:
		};
		class DRMDeviceGroupErrorEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!

		private:
		};
	}
}
namespace avmshell{
	class DRMDeviceGroupErrorEventClass : public ClassClosure//EventClass
	{
	public:
		DRMDeviceGroupErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		inline Stringp getSlotADD_TO_DEVICE_GROUP_ERROR(){return m_slots_DRMDeviceGroupErrorEventClass.ADD_TO_DEVICE_GROUP_ERROR;}
		inline Stringp getSlotREMOVE_FROM_DEVICE_GROUP_ERROR(){return m_slots_DRMDeviceGroupErrorEventClass.REMOVE_FROM_DEVICE_GROUP_ERROR;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMDeviceGroupErrorEventClassSlots m_slots_DRMDeviceGroupErrorEventClass;
	};
	class DRMDeviceGroupErrorEventObject : public ErrorEventObject
	{
	public:
		DRMDeviceGroupErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(DRMDeviceGroupObject*) m_deviceGroup;
		bool                  m_drmUpdateNeeded;
		int                   m_subErrorID;
		bool                  m_systemUpdateNeeded;

		virtual EventObject* Clone(EventObject*);
		DRMDeviceGroupObject* AS3_deviceGroup_get();
		void AS3_deviceGroup_set(DRMDeviceGroupObject* deviceGroup);
		
		bool AS3_drmUpdateNeeded_get();
		
		int  AS3_subErrorID_get();
		void AS3_subErrorID_set(int subErrorID);
		
		bool AS3_systemUpdateNeeded_get();
	public:
		DRMDeviceGroupErrorEventObject* AS3_clone();
		Stringp  AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMDeviceGroupErrorEventObjectSlots m_slots_DRMDeviceGroupErrorEventObject;
	};
}
#endif
#ifndef _AS3DRMDeviceGroupEvent_
#define _AS3DRMDeviceGroupEvent_

#include "AS3Event.h"
#include "AS3DRMDeviceGroup.h"

namespace avmplus
{
	namespace NativeID
	{
		class DRMDeviceGroupEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//MEDIA_TYPE_DATA : String = "mediaTypeData"

			Stringp ADD_TO_DEVICE_GROUP_COMPLETE;
			Stringp REMOVE_FROM_DEVICE_GROUP_COMPLETE;

		private:
		};
		class DRMDeviceGroupEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
			//avmshell::DRMDeviceGroupObject* m_deviceGroup;
		private:
		};
	}
}
namespace avmshell{
	class DRMDeviceGroupEventClass : public ClassClosure//EventClass
	{
	public:
		DRMDeviceGroupEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotADD_TO_DEVICE_GROUP_COMPLETE(){return m_slots_DRMDeviceGroupEventClass.ADD_TO_DEVICE_GROUP_COMPLETE;}
		inline Stringp getSlotREMOVE_FROM_DEVICE_GROUP_COMPLETE(){return m_slots_DRMDeviceGroupEventClass.REMOVE_FROM_DEVICE_GROUP_COMPLETE;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMDeviceGroupEventClassSlots m_slots_DRMDeviceGroupEventClass;
	};
	class DRMDeviceGroupEventObject : public EventObject
	{
	public:
		DRMDeviceGroupEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(DRMDeviceGroupObject*) m_deviceGroup;
		DRMDeviceGroupObject* AS3_deviceGroup_get();
		void AS3_deviceGroup_set(DRMDeviceGroupObject* deviceGroup);
	public:
		virtual EventObject* Clone(EventObject*);
		DRMDeviceGroupEventObject* AS3_clone();
		Stringp  AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMDeviceGroupEventObjectSlots m_slots_DRMDeviceGroupEventObject;
	};
}
#endif
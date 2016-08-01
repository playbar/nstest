#ifndef _AS3GameInputEvent_
#define _AS3GameInputEvent_

#include "AS3Event.h"
#include "AS3GameInputDevice.h"

namespace avmplus
{
	namespace NativeID
	{
		class GameInputEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//DEVICE_ADDED:String = "deviceAdded"
			//DEVICE_REMOVED:String = "deviceRemoved"
			//DEVICE_UNUSABLE:String = "deviceUnusable"

			Stringp DEVICE_ADDED;
			Stringp DEVICE_REMOVED;
			Stringp DEVICE_UNUSABLE;

		private:
		};
		class GameInputEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class GameInputEventClass : public ClassClosure//EventClass
	{
	public:
		GameInputEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputEventClassSlots m_slots_GameInputEventClass;
	public:
		inline Stringp getSlotDEVICE_ADDED(){return m_slots_GameInputEventClass.DEVICE_ADDED;}
		inline Stringp getSlotDEVICE_REMOVED(){return m_slots_GameInputEventClass.DEVICE_REMOVED;}
		inline Stringp getSlotDEVICE_UNUSABLE(){return m_slots_GameInputEventClass.DEVICE_UNUSABLE;}
	};
	class GameInputEventObject : public EventObject
	{
	public:
		GameInputEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		DRCWB(GameInputDeviceObject*) m_pDevice;
		GameInputDeviceObject* AS3_device_get();

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputEventObjectSlots m_slots_GameInputEventObject;
	};
}
#endif
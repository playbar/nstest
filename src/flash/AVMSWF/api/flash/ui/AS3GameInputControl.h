#ifndef _AS3GameInputControl_
#define _AS3GameInputControl_

#include "AS3EventDispatcher.h"
#include "AS3GameInputDevice.h"

namespace avmplus
{
	namespace NativeID
	{
		class GameInputControlClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!


		private:
		};
		class GameInputControlObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class GameInputControlClass : public ClassClosure//EventClass
	{
	public:
		GameInputControlClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputControlClassSlots m_slots_GameInputControlClass;
	};
	class GameInputControlObject : public EventDispatcherObject
	{
	public:
		GameInputControlObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		DRCWB(GameInputDeviceObject*) m_pDevice;
		Stringp m_strID;
		double m_maxValue,m_minValue,m_value;

		GameInputDeviceObject* AS3_device_get();
		Stringp AS3_id_get();
		double AS3_maxValue_get();
		double AS3_minValue_get();
		double AS3_value_get();


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputControlObjectSlots m_slots_GameInputControlObject;
	};
}
#endif

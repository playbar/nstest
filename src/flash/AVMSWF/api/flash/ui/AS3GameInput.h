#ifndef _AS3GameInput_
#define _AS3GameInput_

#include "AS3EventDispatcher.h"
#include "AS3GameInputDevice.h"

namespace avmplus
{
	namespace NativeID
	{
		class GameInputClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!


		private:
		};
		class GameInputObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class GameInputClass : public ClassClosure//EventClass
	{
	public:
		GameInputClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		bool m_bSupported;
		int m_numDevices;

		bool AS3_isSupported_get();
		int AS3_numDevices_get();

		GameInputDeviceObject* AS3_getDeviceAt(int index);


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputClassSlots m_slots_GameInputClass;
	};
	class GameInputObject : public EventDispatcherObject
	{
	public:
		GameInputObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputObjectSlots m_slots_GameInputObject;
	};
}
#endif
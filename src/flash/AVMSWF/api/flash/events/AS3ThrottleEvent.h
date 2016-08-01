#ifndef _AS3ThrottleEvent_
#define _AS3ThrottleEvent_

#include "AS3Event.h"
#include "AS3ThrottleType.h"
#include "AS3Stage.h"

namespace avmplus
{
	namespace NativeID
	{
		class ThrottleEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//THROTTLE:String = "throttle"

			Stringp THROTTLE;
		private:
		};
		class ThrottleEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class ThrottleEventClass : public ClassClosure//EventClass
	{
	public:
		ThrottleEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		inline Stringp getSlotTHROTTLE(){return m_slots_ThrottleEventClass.THROTTLE;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ThrottleEventClassSlots m_slots_ThrottleEventClass;
	};
	class ThrottleEventObject : public EventObject
	{
	public:
		ThrottleEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* m_pData;
		Stringp m_strState;
		double m_nTargetFrameRate;
		virtual EventObject* Clone(EventObject *pObj);
		Stringp AS3_state_get();
		double AS3_targetFrameRate_get();

		ThrottleEventObject* AS3_clone();
		
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ThrottleEventObjectSlots m_slots_ThrottleEventObject;
	};
}
#endif
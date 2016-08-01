#ifndef _AS3GestureEvent_
#define _AS3GestureEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class GestureEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//GESTURE_TWO_FINGER_TAP : String = "gestureTwoFingerTap"

	Stringp GESTURE_TWO_FINGER_TAP;

private:};
class GestureEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GestureEventClass : public ClassClosure//EventClass
	{
	public:
		GestureEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GestureEventClassSlots m_slots_GestureEventClass;
};
class GestureEventObject : public EventObject
{
	public:
		GestureEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_altKey_get();
		void AS3_altKey_set(bool altKey);
		bool AS3_commandKey_get();
		void AS3_commandKey_set(bool commandKey);
		bool AS3_controlKey_get();
		void AS3_controlKey_set(bool controlKey);
		bool AS3_ctrlKey_get();
		void AS3_ctrlKey_set(bool ctrlKey);
		double AS3_localX_get();
		void AS3_localX_set(double localX);
		double AS3_localY_get();
		void AS3_localY_set(double localY);
		Stringp AS3_phase_get();
		void AS3_phase_set(Stringp pPhase);
		bool AS3_shiftKey_get();
		void AS3_shiftKey_set(bool shiftKey);
		double AS3_stageX_get();
		double AS3_stageY_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
								Stringp pPhase, double localX, double localY, bool ctrlKey, 
								bool altKey, bool shiftKey, bool commandKey, bool controlKey);
		EventObject* AS3_clone();
		Stringp AS3_toString();
		void AS3_updateAfterEvent();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GestureEventObjectSlots m_slots_GestureEventObject;
};}
#endif
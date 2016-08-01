#ifndef _AS3PressAndTapGestureEvent_
#define _AS3PressAndTapGestureEvent_

#include "AS3Event.h"
#include "AS3GestureEvent.h"

namespace avmplus{namespace NativeID{
class PressAndTapGestureEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//GESTURE_PRESS_AND_TAP : String = "gesturePressAndTap"

	Stringp GESTURE_PRESS_AND_TAP;

private:};
class PressAndTapGestureEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PressAndTapGestureEventClass : public ClassClosure//GestureEventClass
	{
	public:
		PressAndTapGestureEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PressAndTapGestureEventClassSlots m_slots_PressAndTapGestureEventClass;
};
class PressAndTapGestureEventObject : public GestureEventObject
{
	public:
		PressAndTapGestureEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		double AS3_tapLocalX_get();
		void AS3_tapLocalX_set(double tapLocalX);
		double AS3_tapLocalY_get();
		void AS3_tapLocalY_set(double tapLocalY);
		double AS3_tapStageX_get();
		double AS3_tapStageY_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pPhase, 
		//						double localX, double localY, double tapLocalX, double tapLocalY, 
		//						bool ctrlKey, bool altKey, bool shiftKey, bool commandKey, bool controlKey);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PressAndTapGestureEventObjectSlots m_slots_PressAndTapGestureEventObject;
};}
#endif
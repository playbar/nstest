#ifndef _AS3StageOrientationEvent_
#define _AS3StageOrientationEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class StageOrientationEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//ORIENTATION_CHANGE : String = "orientationChange"
    //ORIENTATION_CHANGING : String = "orientationChanging"

	Stringp ORIENTATION_CHANGE;
	Stringp ORIENTATION_CHANGING;

private:};
class StageOrientationEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StageOrientationEventClass : public ClassClosure//EventClass
	{
	public:
		StageOrientationEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageOrientationEventClassSlots m_slots_StageOrientationEventClass;
};
class StageOrientationEventObject : public EventObject
{
	public:
		StageOrientationEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Stringp AS3_afterOrientation_get();
		Stringp AS3_beforeOrientation_get();

		void AS3_constructor(String pType, bool bubbles, bool cancelable, 
								Stringp pBeforeOrientation, Stringp pAfterOrientation);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageOrientationEventObjectSlots m_slots_StageOrientationEventObject;
};}
#endif
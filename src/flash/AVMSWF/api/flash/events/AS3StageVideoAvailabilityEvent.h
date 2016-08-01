#ifndef _AS3StageVideoAvailabilityEvent_
#define _AS3StageVideoAvailabilityEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class StageVideoAvailabilityEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//STAGE_VIDEO_AVAILABILITY : String = "stageVideoAvailability"

	Stringp STAGE_VIDEO_AVAILABILITY;

private:};
class StageVideoAvailabilityEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp m_availability;
private:};
}}
namespace avmshell{
	class StageVideoAvailabilityEventClass : public ClassClosure//EventClass
	{
	public:
		StageVideoAvailabilityEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoAvailabilityEventClassSlots m_slots_StageVideoAvailabilityEventClass;
};
class StageVideoAvailabilityEventObject : public EventObject
{
	public:
		StageVideoAvailabilityEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Stringp AS3_availability_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp availability);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoAvailabilityEventObjectSlots m_slots_StageVideoAvailabilityEventObject;
};}
#endif
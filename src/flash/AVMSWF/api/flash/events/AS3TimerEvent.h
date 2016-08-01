#ifndef _AS3TimerEvent_
#define _AS3TimerEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class TimerEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp TIMER = "timer";
	//Stringp TIMER_COMPLETE = "timerComplete";

	Stringp TIMER;
	Stringp TIMER_COMPLETE;
private:};
class TimerEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TimerEventObject;

	class TimerEventClass : public ClassClosure//EventClass
	{
	public:
		TimerEventObject* CreateEventObject(int id);
		Stringp		 IDToStringType(int id);
	public:
		TimerEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TimerEventClassSlots m_slots_TimerEventClass;
	public:	
			Stringp getSlotTIMER(){return m_slots_TimerEventClass.TIMER;}
			Stringp getSlotTIMER_COMPLETE(){return m_slots_TimerEventClass.TIMER_COMPLETE;}
};
class TimerEventObject : public EventObject
{
	public:
		TimerEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable);
		TimerEventObject* AS3_clone();
		Stringp AS3_toString();
		void AS3_updateAfterEvent();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TimerEventObjectSlots m_slots_TimerEventObject;
};}
#endif
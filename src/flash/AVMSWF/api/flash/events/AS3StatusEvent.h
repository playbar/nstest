#ifndef _AS3StatusEvent_
#define _AS3StatusEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class StatusEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp STATUS  = "status";

	Stringp STATUS;
private:};
class StatusEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StatusEventObject;

	class StatusEventClass : public ClassClosure//EventClass
	{
	public:
		StatusEventObject* CreateEventObject(int id, Stringp pCode, Stringp pLevel);
		Stringp		 IDToStringType(int id);
	public:
		StatusEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StatusEventClassSlots m_slots_StatusEventClass;
	public:
		Stringp getSlotSTATUS(){return m_slots_StatusEventClass.STATUS;}
};
class StatusEventObject : public EventObject
{
	public:
		enum {LV_STATUS,LV_WARING,LV_ERROR};
		//DRCWB(Stringp) m_code;
		//DRCWB(Stringp) m_level;
		Stringp m_code;
		Stringp m_level;

	public:
		StatusEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		Stringp AS3_code_get();
		void AS3_code_set(Stringp code);
		Stringp AS3_level_get();
		void AS3_level_set(Stringp level);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp code, Stringp level);
		StatusEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StatusEventObjectSlots m_slots_StatusEventObject;
};}
#endif
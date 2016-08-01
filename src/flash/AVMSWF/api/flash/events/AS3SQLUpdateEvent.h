#ifndef _AS3SQLUpdateEvent_
#define _AS3SQLUpdateEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class SQLUpdateEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DELETE : Stringp = "delete"
//INSERT : Stringp = "insert"
//UPDATE : Stringp = "update"

	Stringp DELETE_C;
	Stringp INSERT;
	Stringp UPDATE;

private:};
class SQLUpdateEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLUpdateEventClass : public ClassClosure//EventClass
	{
	public:
		SQLUpdateEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLUpdateEventClassSlots m_slots_SQLUpdateEventClass;
};
class SQLUpdateEventObject : public EventObject
{
	public:
		SQLUpdateEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		double AS3_rowID_get();
		Stringp AS3_table_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pTable, double rowID);
		EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLUpdateEventObjectSlots m_slots_SQLUpdateEventObject;
};}
#endif
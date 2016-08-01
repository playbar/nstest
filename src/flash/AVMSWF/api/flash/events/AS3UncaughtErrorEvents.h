#ifndef _AS3UncaughtErrorEvents_
#define _AS3UncaughtErrorEvents_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class UncaughtErrorEventsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class UncaughtErrorEventsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class UncaughtErrorEventsObject;
	class UncaughtErrorEventsClass : public ClassClosure
	{
	public:
		UncaughtErrorEventsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		UncaughtErrorEventsObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::UncaughtErrorEventsClassSlots m_slots_UncaughtErrorEventsClass;
};
class UncaughtErrorEventsObject : public EventDispatcherObject
{
	public:
		UncaughtErrorEventsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::UncaughtErrorEventsObjectSlots m_slots_UncaughtErrorEventsObject;
};}
#endif
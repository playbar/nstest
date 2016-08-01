#ifndef _AS3UncaughtErrorEvent_
#define _AS3UncaughtErrorEvent_

#include "AS3Event.h"
#include "AS3ErrorEvent.h"

namespace avmplus{namespace NativeID{
class UncaughtErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//UNCAUGHT_ERROR : String = "uncaughtError"

	Stringp UNCAUGHT_ERROR;

private:};
class UncaughtErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	 ScriptObject *_error;
private:};
}}
namespace avmshell{
	class UncaughtErrorEventObject;
	class UncaughtErrorEventClass : public ClassClosure//ErrorEventClass
	{
	public:
		UncaughtErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		UncaughtErrorEventObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::UncaughtErrorEventClassSlots m_slots_UncaughtErrorEventClass;
};
class UncaughtErrorEventObject : public ErrorEventObject
{
	public:
		UncaughtErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Atom AS3_error_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Atom error_in);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::UncaughtErrorEventObjectSlots m_slots_UncaughtErrorEventObject;
};}
#endif
#ifndef _AS3HTMLUncaughtScriptExceptionEvent_
#define _AS3HTMLUncaughtScriptExceptionEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class HTMLUncaughtScriptExceptionEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//UNCAUGHT_SCRIPT_EXCEPTION : * = uncaughtScriptException

	Stringp UNCAUGHT_SCRIPT_EXCEPTION;

private:};
class HTMLUncaughtScriptExceptionEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTMLUncaughtScriptExceptionEventClass : public ClassClosure//EventClass
	{
	public:
		HTMLUncaughtScriptExceptionEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLUncaughtScriptExceptionEventClassSlots m_slots_HTMLUncaughtScriptExceptionEventClass;
};
class HTMLUncaughtScriptExceptionEventObject : public EventObject
{
	public:
		HTMLUncaughtScriptExceptionEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Atom AS3_exceptionValue_get();
		void AS3_exceptionValue_set(Atom  exceptionValue);
		ArrayObject* AS3_stackTrace_get();
		void AS3_stackTrace_set(ArrayObject *pStackTrace);

		void AS3_constructor(Atom exceptionValue);
		EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLUncaughtScriptExceptionEventObjectSlots m_slots_HTMLUncaughtScriptExceptionEventObject;
};}
#endif
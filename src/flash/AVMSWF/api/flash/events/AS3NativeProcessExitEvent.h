#ifndef _AS3NativeProcessExitEvent_
#define _AS3NativeProcessExitEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class NativeProcessExitEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//EXIT : String = "exit"

	Stringp EXIT;

private:};
class NativeProcessExitEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeProcessExitEventClass : public ClassClosure//EventClass
	{
	public:
		NativeProcessExitEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeProcessExitEventClassSlots m_slots_NativeProcessExitEventClass;
};
class NativeProcessExitEventObject : public EventObject
{
	public:
		NativeProcessExitEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		double AS3_exitCode_get();
		void AS3_exitCode_set(double pExitCode);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double exitCode);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeProcessExitEventObjectSlots m_slots_NativeProcessExitEventObject;
};}
#endif
#ifndef _AS3InvokeEvent_
#define _AS3InvokeEvent_

#include "AS3File.h"
#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class InvokeEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	Stringp INVOKE;

private:};
class InvokeEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class InvokeEventClass : public ClassClosure//EventClass
	{
	public:
		InvokeEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InvokeEventClassSlots m_slots_InvokeEventClass;
};
class InvokeEventObject : public EventObject
{
	public:
		InvokeEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		ArrayObject* AS3_arguments_get();
		FileObject* AS3_currentDirectory_get();
		Stringp AS3_reason_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, FileObject *pDir, ArrayObject *pArgv, Stringp pReason);
		EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InvokeEventObjectSlots m_slots_InvokeEventObject;
};}
#endif
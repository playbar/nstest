#ifndef _AS3SQLErrorEvent_
#define _AS3SQLErrorEvent_

#include "AS3Event.h"
#include "AS3ErrorEvent.h"
#include "AS3SQLError.h"

namespace avmplus{namespace NativeID{
class SQLErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//ERROR : String = "error"

	Stringp ERROR_C;

private:};
class SQLErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLErrorEventClass : public ClassClosure//ErrorEventClass
	{
	public:
		SQLErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLErrorEventClassSlots m_slots_SQLErrorEventClass;
};
class SQLErrorEventObject : public ErrorEventObject
{
	public:
		SQLErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		SQLErrorObject* AS3_error_get();

		void AS3_constructor(Stringp type, bool bubbles, bool cancelable, SQLErrorObject *pError);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLErrorEventObjectSlots m_slots_SQLErrorEventObject;
};}
#endif
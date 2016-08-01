#ifndef _AS3AsyncErrorEvent_
#define _AS3AsyncErrorEvent_

#include "../../core/ErrorClass.h"

#include "AS3ErrorEvent.h"

namespace avmplus{namespace NativeID{
class AsyncErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp ASYNC_ERROR = "asyncError";

	Stringp ASYNC_ERROR;
private:};
class AsyncErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class AsyncErrorEventObject;

	class AsyncErrorEventClass : public ClassClosure//ErrorEventClass
	{
	public:
		AsyncErrorEventObject* CreateEventObject(int id, Stringp pText, ErrorObject *pError);
		Stringp		 IDToStringType(int id);
	public:
		AsyncErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AsyncErrorEventClassSlots m_slots_AsyncErrorEventClass;
	public:
		Stringp getSlotASYNC_ERROR(){return m_slots_AsyncErrorEventClass.ASYNC_ERROR;}
};
class AsyncErrorEventObject : public ErrorEventObject
{
	public:
//		DRCWB(ErrorObject *) m_pError;
		ErrorObject *m_pError;
	public:
		AsyncErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		ErrorObject* AS3_error_get();
		void AS3_error_set(ErrorObject *pError);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp text, ErrorObject *pError);
		AsyncErrorEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AsyncErrorEventObjectSlots m_slots_AsyncErrorEventObject;
};}
#endif
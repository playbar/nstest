#ifndef _AS3NativeWindowDisplayStateEvent_
#define _AS3NativeWindowDisplayStateEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class NativeWindowDisplayStateEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DISPLAY_STATE_CHANGE : String = "displayStateChange"
	//DISPLAY_STATE_CHANGING : String = "displayStateChanging"

	Stringp DISPLAY_STATE_CHANGE;
	Stringp DISPLAY_STATE_CHANGING;

private:};
class NativeWindowDisplayStateEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowDisplayStateEventClass : public ClassClosure//EventClass
	{
	public:
		NativeWindowDisplayStateEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowDisplayStateEventClassSlots m_slots_NativeWindowDisplayStateEventClass;
};
class NativeWindowDisplayStateEventObject : public EventObject
{
	public:
		NativeWindowDisplayStateEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Stringp AS3_afterDisplayState_get();
		Stringp AS3_beforeDisplayState_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
								Stringp pBeforeDisplayState, Stringp pAfterDisplayState);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowDisplayStateEventObjectSlots m_slots_NativeWindowDisplayStateEventObject;
};}
#endif
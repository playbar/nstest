#ifndef _AS3SoftKeyboardEvent_
#define _AS3SoftKeyboardEvent_

#include "AS3Event.h"
#include "AS3InteractiveObject.h"

namespace avmplus{namespace NativeID{

	class InteractiveObjectObject;

class SoftKeyboardEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//SOFT_KEYBOARD_ACTIVATE : String = "softKeyboardActivate"
//SOFT_KEYBOARD_ACTIVATING : String = "softKeyboardActivating"
//SOFT_KEYBOARD_DEACTIVATE : String = "softKeyboardDeactivate"

	Stringp SOFT_KEYBOARD_ACTIVATE;
	Stringp SOFT_KEYBOARD_ACTIVATING;
	Stringp SOFT_KEYBOARD_DEACTIVATE;

private:};
class SoftKeyboardEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	InteractiveObjectObject *m_relatedObject;
	Stringp m_triggerType;
private:};
}}
namespace avmshell{
	class SoftKeyboardEventClass : public ClassClosure//EventClass
	{
	public:
		SoftKeyboardEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoftKeyboardEventClassSlots m_slots_SoftKeyboardEventClass;
};
class SoftKeyboardEventObject : public EventObject
{
	public:
		SoftKeyboardEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//InteractiveObjectObject* AS3_relatedObject_get();
		//void AS3_relatedObject_set(InteractiveObjectObject *pRelatedObject);
		//Stringp AS3_triggerType_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
		//						InteractiveObjectObject *pRelatedObjectVal, Stringp pTriggerTypeVal);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoftKeyboardEventObjectSlots m_slots_SoftKeyboardEventObject;
};}
#endif
#ifndef _AS3ScreenMouseEvent_
#define _AS3ScreenMouseEvent_

#include "AS3Event.h"
#include "AS3MouseEvent.h"

namespace avmplus{namespace NativeID{
class ScreenMouseEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CLICK : String = "click"
//MOUSE_DOWN : String = "mouseDown"
//MOUSE_UP : String = "mouseUp"
//RIGHT_CLICK : String = "rightClick"
//RIGHT_MOUSE_DOWN : String = "rightMouseDown"
//RIGHT_MOUSE_UP : String = "rightMouseUp"

	Stringp CLICK;
	Stringp MOUSE_DOWN;
	Stringp MOUSE_UP;
	Stringp RIGHT_CLICK;
	Stringp RIGHT_MOUSE_DOWN;
	Stringp RIGHT_MOUSE_UP;

private:};
class ScreenMouseEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ScreenMouseEventClass : public ClassClosure//MouseEventClass
	{
	public:
		ScreenMouseEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ScreenMouseEventClassSlots m_slots_ScreenMouseEventClass;
};
class ScreenMouseEventObject : public MouseEventObject
{
	public:
		ScreenMouseEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		double AS3_screenX_get();
		double AS3_screenY_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double screenX, double screenY, 
								bool ctrlKey, bool altKey, bool shiftKey, bool buttonDown, 
								bool commandKey, bool controlKey);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ScreenMouseEventObjectSlots m_slots_ScreenMouseEventObject;
};}
#endif
#ifndef _AS3NativeWindowBoundsEvent_
#define _AS3NativeWindowBoundsEvent_

#include "AS3Event.h"
#include "AS3Rectangle.h"

namespace avmplus{namespace NativeID{
class NativeWindowBoundsEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//MOVE : String = "move"
//MOVING : String = "moving"
//RESIZE : String = "resize"
//RESIZING : String = "resizing"

	Stringp MOVE;
	Stringp MOVING;
	Stringp RESIZE;
	Stringp RESIZING;

private:};
class NativeWindowBoundsEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowBoundsEventClass : public ClassClosure//EventClass
	{
	public:
		NativeWindowBoundsEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowBoundsEventClassSlots m_slots_NativeWindowBoundsEventClass;
};
class NativeWindowBoundsEventObject : public EventObject
{
	public:
		NativeWindowBoundsEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		RectangleObject* AS3_afterBounds_get();
		RectangleObject* AS3_beforeBounds_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
								RectangleObject *pBeforeBounds, RectangleObject *pAfterBounds);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowBoundsEventObjectSlots m_slots_NativeWindowBoundsEventObject;
};}
#endif
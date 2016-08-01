#ifndef _AS3TransformGestureEvent_
#define _AS3TransformGestureEvent_

#include "AS3Event.h"
#include "AS3GestureEvent.h"

namespace avmplus{namespace NativeID{
class TransformGestureEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//GESTURE_PAN : String = "gesturePan"
//GESTURE_ROTATE : String = "gestureRotate"
//GESTURE_SWIPE : String = "gestureSwipe"
//GESTURE_ZOOM : String = "gestureZoom"

	Stringp GESTURE_PAN;
	Stringp GESTURE_ROTATE;
	Stringp GESTURE_SWIPE;
	Stringp GESTURE_ZOOM;

private:};
class TransformGestureEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double m_scaleX;
	double m_scaleY;
	double m_rotation;
	double m_offsetX;
	double m_offsetY;
private:};
}}
namespace avmshell{
	class TransformGestureEventClass : public ClassClosure//GestureEventClass
	{
	public:
		TransformGestureEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TransformGestureEventClassSlots m_slots_TransformGestureEventClass;
};
class TransformGestureEventObject : public GestureEventObject
{
	public:
		TransformGestureEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_offsetX_get();
		//void AS3_offsetX_set(double offsetX);
		//double AS3_offsetY_get();
		//void AS3_offsetY_set(double offsetY);
		//double AS3_rotation_get();
		//void AS3_rotation_set(double rotation);
		//double AS3_scaleX_get();
		//void AS3_scaleX_set(double scaleX);
		//double AS3_scaleY_get();
		//void AS3_scaleY_set(double scaleY);

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
		//						Stringp phase, double localX, double localY, double scaleX, double scaleY, 
		//						double rotation, double offsetX, double offsetY, bool ctrlKey, 
		//						bool altKey, bool shiftKey, bool commandKey, bool controlKey);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TransformGestureEventObjectSlots m_slots_TransformGestureEventObject;
};}
#endif
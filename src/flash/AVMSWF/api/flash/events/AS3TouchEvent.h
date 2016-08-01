#ifndef _AS3TouchEvent_
#define _AS3TouchEvent_

#include "AS3Event.h"
#include "AS3InteractiveObject.h"

namespace avmplus{namespace NativeID{
class TouchEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//TOUCH_BEGIN : Stringp = "touchBegin"
//TOUCH_END : Stringp = "touchEnd"
//TOUCH_MOVE : Stringp = "touchMove"
//TOUCH_OUT : Stringp = "touchOut"
//TOUCH_OVER : Stringp = "touchOver"
//TOUCH_ROLL_OUT : Stringp = "touchRollOut"
//TOUCH_ROLL_OVER : Stringp = "touchRollOver"
//TOUCH_TAP : Stringp = "touchTap"

	Stringp TOUCH_BEGIN;
	Stringp TOUCH_END;
	Stringp TOUCH_MOVE;
	Stringp TOUCH_OUT;
	Stringp TOUCH_OVER;
	Stringp TOUCH_ROLL_OUT;
	Stringp TOUCH_ROLL_OVER;
	Stringp TOUCH_TAP;

	//Check adds
	Stringp	PROXIMITY_BEGIN;
	Stringp	PROXIMITY_END;
	Stringp	PROXIMITY_MOVE;
	Stringp	PROXIMITY_OUT;
	Stringp	PROXIMITY_OVER;
	Stringp	PROXIMITY_ROLL_OUT;
	Stringp	PROXIMITY_ROLL_OVER;
	//Check adds end

private:};
class TouchEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TouchEventClass : public ClassClosure//EventClass
	{
	public:
		TouchEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TouchEventClassSlots m_slots_TouchEventClass;
};
class TouchEventObject : public EventObject
{
	public:
		TouchEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_altKey_get();
		void AS3_altKey_set(bool altKey);
		bool AS3_commandKey_get();
		void AS3_commandKey_set(bool commandKey);
		bool AS3_controlKey_get();
		void AS3_controlKey_set(bool controlKey);
		bool AS3_ctrlKey_get();
		void AS3_ctrlKey_set(bool ctrlKey);
		bool AS3_isPrimaryTouchPoint_get();
		void AS3_isPrimaryTouchPoint_set(bool isPrimaryTouchPoint);
		bool AS3_isRelatedObjectInaccessible_get();
		void AS3_isRelatedObjectInaccessible_set(bool isRelatedObjectInaccessible);
		double AS3_localX_get();
		void AS3_localX_set(double localX);
		double AS3_localY_get();
		void AS3_localY_set(double localY);
		double AS3_pressure_get();
		void AS3_pressure_set(double pressure);
		InteractiveObjectObject* AS3_relatedObject_get();
		void AS3_relatedObject_set(InteractiveObjectObject *pRelatedObject);
		bool AS3_shiftKey_get();
		void AS3_shiftKey_set(bool shiftKey);
		double AS3_sizeX_get();
		void AS3_sizeX_set(double sizeX);
		double AS3_sizeY_get();
		void AS3_sizeY_set(double sizeY);
		double AS3_stageX_get();
		double AS3_stageY_get();
		int AS3_touchPointID_get();
		void AS3_touchPointID_set(int touchPointID);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
								int touchPointID, bool isPrimaryTouchPoint, double localX, 
								double localY, double sizeX, double sizeY, double pressure, 
								InteractiveObjectObject *pRelatedObject, bool ctrlKey, bool altKey, 
								bool shiftKey, bool commandKey, bool controlKey, double timstamp,
								Stringp touchIntent, bool isTouchPointCanceled);
		EventObject* AS3_clone();
		Stringp AS3_toString();
		void AS3_updateAfterEvent();

		//new adds 11
		bool AS3_isTouchPointCanceled_get();
		void AS3_isTouchPointCanceled_set(bool b);
		double AS3_timestamp_get();
		void AS3_timestamp_set(double d);
		Stringp AS3_touchIntent_get();
		void AS3_touchIntent_set(Stringp s);
		//new adds 11 end 

		//new adds 11
		uint32_t AS3_getSamples(ByteArrayObject* buffer, bool append = false);
		bool AS3_isToolButtonDown(int index);
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TouchEventObjectSlots m_slots_TouchEventObject;
};}
#endif
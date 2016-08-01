#ifndef _AS3MouseEvent_
#define _AS3MouseEvent_

#include "AS3InteractiveObject.h"

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class MouseEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp CLICK = "click";
	//Stringp DOUBLE_CLICK = "doubleClick";
	//Stringp MOUSE_DOWN = "mouseDown"; 
	//Stringp MOUSE_MOVE = "mouseMove";
	//Stringp MOUSE_OUT = "mouseOut";

	//Stringp MOUSE_OVER = "mouseOver"; 
	//Stringp MOUSE_UP = "mouseUp"; 
	//Stringp MOUSE_WHEEL = "mouseWheel"; 
	//Stringp ROLL_OUT = "rollOut"; 
	//Stringp ROLL_OVER = "rollOver"; 
	//CONTEXT_MENU : String = "contextMenu"
	//MIDDLE_CLICK : String = "middleClick"
	//MIDDLE_MOUSE_DOWN : String = "middleMouseDown"
	//MIDDLE_MOUSE_UP : String = "middleMouseUp"
	//RIGHT_CLICK : String = "rightClick"
	//RIGHT_MOUSE_DOWN : String = "rightMouseDown"
	//RIGHT_MOUSE_UP : String = "rightMouseUp"
	//RELEASE_OUTSIDE:String = "releaseOutside"
	Stringp CLICK;
	Stringp DOUBLE_CLICK_FORSPACE;
	Stringp MOUSE_DOWN; 
	Stringp MOUSE_MOVE;
	Stringp MOUSE_OUT;

	Stringp MOUSE_OVER; 
	Stringp MOUSE_UP; 
	Stringp MOUSE_WHEEL; 
	Stringp ROLL_OUT; 
	Stringp ROLL_OVER; 

	//new adds
	Stringp CONTEXT_MENU;
	Stringp MIDDLE_CLICK;
	Stringp MIDDLE_MOUSE_DOWN;
	Stringp MIDDLE_MOUSE_UP;
	Stringp RIGHT_CLICK;
	Stringp RIGHT_MOUSE_DOWN;
	Stringp RIGHT_MOUSE_UP;
	//new adds end
	
	//new adds 11
	Stringp RELEASE_OUTSIDE;
	//new adds 11 end

private:};
class MouseEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MouseEventObject;

	class MouseEventClass : public ClassClosure//EventClass
	{
	public:
		MouseEventObject* CreateEventObject(int id, void*data);
		Stringp		 IDToStringType(int id);
	public:
		MouseEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseEventClassSlots m_slots_MouseEventClass;
	public:
			Stringp getSlotCLICK(){return m_slots_MouseEventClass.CLICK;}
			Stringp getSlotDOUBLE_CLICK_FORSPACE(){return m_slots_MouseEventClass.DOUBLE_CLICK_FORSPACE;}
			Stringp getSlotMOUSE_DOWN(){return m_slots_MouseEventClass.MOUSE_DOWN;}
			Stringp getSlotMOUSE_MOVE(){return m_slots_MouseEventClass.MOUSE_MOVE;}
			Stringp getSlotMOUSE_OUT(){return m_slots_MouseEventClass.MOUSE_OUT;}
			Stringp getSlotMOUSE_OVER(){return m_slots_MouseEventClass.MOUSE_OVER;}
			Stringp getSlotMOUSE_UP(){return m_slots_MouseEventClass.MOUSE_UP;}
			Stringp getSlotMOUSE_WHEEL(){return m_slots_MouseEventClass.MOUSE_WHEEL;}
			Stringp getSlotROLL_OUT(){return m_slots_MouseEventClass.ROLL_OUT;}
			Stringp getSlotROLL_OVER(){return m_slots_MouseEventClass.ROLL_OVER;}

			//new adds
			inline Stringp getSlotCONTEXT_MENU(){return m_slots_MouseEventClass.CONTEXT_MENU;}
			inline Stringp getSlotMIDDLE_CLICK(){return m_slots_MouseEventClass.MIDDLE_CLICK;}
			inline Stringp getSlotMIDDLE_MOUSE_DOWN(){return m_slots_MouseEventClass.MIDDLE_MOUSE_DOWN;}
			inline Stringp getSlotMIDDLE_MOUSE_UP(){return m_slots_MouseEventClass.MIDDLE_MOUSE_UP;}
			inline Stringp getSlotRIGHT_CLICK(){return m_slots_MouseEventClass.RIGHT_CLICK;}
			inline Stringp getSlotRIGHT_MOUSE_DOWN(){return m_slots_MouseEventClass.RIGHT_MOUSE_DOWN;}
			inline Stringp getSlotRIGHT_MOUSE_UP(){return m_slots_MouseEventClass.RIGHT_MOUSE_UP;}
			//new adds end
			inline Stringp getSlotRELEASE_OUTSIDE(){return m_slots_MouseEventClass.RELEASE_OUTSIDE;}

			
};

class MouseEventObject : public EventObject
{
	public:
		bool						m_altKey;
		bool						m_buttonDown;
		bool						m_ctrlKey;
		//bool						m_bSetX,m_bSetY;
		int32_t						m_delta;
		double						m_localX;
		double						m_localY;
		//DRCWB(InteractiveObjectObject *) m_pRelatedObject;
		InteractiveObjectObject		*m_pRelatedObject;
		bool						m_shiftKey;
		double						m_stageX;
		double						m_stageY;

		//new adds
		bool m_isRelatedObjectInaccessible;
		//new adds end

	public:
		MouseEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject*pObj);

		bool AS3_altKey_get();
		void AS3_altKey_set(bool altKey);
		bool AS3_buttonDown_get();
		void AS3_buttonDown_set(bool buttonDown);
		bool AS3_ctrlKey_get();
		void AS3_ctrlKey_set(bool ctrlKey);
		int32_t AS3_delta_get();
		void AS3_delta_set(int32_t delta);
		double AS3_localX_get();
		void AS3_localX_set(double localX);
		double AS3_localY_get();
		void AS3_localY_set(double localY);
		InteractiveObjectObject* AS3_relatedObject_get();
		void AS3_relatedObject_set(InteractiveObjectObject *pRelatedObject);
		bool AS3_shiftKey_get();
		void AS3_shiftKey_set(bool shiftKey);
		double AS3_stageX_get();
		double AS3_stageY_get();
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable,
							 double localX, double localY,
							 InteractiveObjectObject *pRelatedObject, 
							 bool ctrlkey, bool altKey, bool shiftKey, bool buttonDown,
							 int32_t delta);
		MouseEventObject* AS3_clone();
		Stringp AS3_toString();
		void AS3_updateAfterEvent();

		//new adds
		int AS3_clickCount_get();
		bool AS3_commandKey_get();
		void AS3_commandKey_set(bool commandKey);
		bool AS3_controlKey_get();
		void AS3_controlKey_set(bool controlKey);
		bool AS3_isRelatedObjectInaccessible_get();
		void AS3_isRelatedObjectInaccessible_set(bool isRelatedObjectInaccessible);
		//new adds end

		//new adds 11
		double m_nMovementX;
		double m_nMovementY;

		double AS3_movementX_get();
		void AS3_movementX_set(double x);
		double AS3_movementY_get();
		void AS3_movementY_set(double y);
		//new adds 11 end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseEventObjectSlots m_slots_MouseEventObject;
};}
#endif
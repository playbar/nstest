#ifndef _AS3FocusEvent_
#define _AS3FocusEvent_

#include "AS3InteractiveObject.h"

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class FocusEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp FOCUS_IN = "focusIn";
	//Stringp FOCUS_OUT = "focusOut"; 
	//Stringp KEY_FOCUS_CHANGE = "keyFocusChange"; 
	//Stringp MOUSE_FOCUS_CHANGE = "mouseFocusChange"; 

	Stringp FOCUS_IN;
	Stringp FOCUS_OUT; 
	Stringp KEY_FOCUS_CHANGE; 
	Stringp MOUSE_FOCUS_CHANGE; 
private:};
class FocusEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FocusEventObject;

	class FocusEventClass : public ClassClosure//EventClass
	{
	public:
		FocusEventObject* CreateEventObject(int id, InteractiveObjectObject *pRelatedObject, bool shiftKey, uint32_t keyCode);
		Stringp		 IDToStringType(int id);
	public:
		FocusEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FocusEventClassSlots m_slots_FocusEventClass;
	public:
			Stringp getSlotFOCUS_IN(){return m_slots_FocusEventClass.FOCUS_IN;}
			Stringp getSlotFOCUS_OUT(){return m_slots_FocusEventClass.FOCUS_OUT;}
			Stringp	getSlotKEY_FOCUS_CHANGE(){return m_slots_FocusEventClass.KEY_FOCUS_CHANGE;}
			Stringp getSlotMOUSE_FOCUS_CHANGE(){return m_slots_FocusEventClass.MOUSE_FOCUS_CHANGE;}
};
class FocusEventObject : public EventObject
{
	public:
		uint32_t m_keyCode;
//		DRCWB(InteractiveObjectObject *) m_pRelatedObject;
		InteractiveObjectObject * m_pRelatedObject;
		bool m_shiftKey;

		//new adds
		bool m_isRelatedObjectInaccessible;
		//new adds end
	public:
		FocusEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		uint32_t AS3_keyCode_get();
		void AS3_keyCode_set(uint32_t keyCode);
		InteractiveObjectObject* AS3_relatedObject_get();
		void AS3_relatedObject_set(InteractiveObjectObject *pRelateObject);
		bool AS3_shiftKey_get();
		void AS3_shiftKey_set(bool shiftKey);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, InteractiveObjectObject *pRelatedObject, bool shiftKey, uint32_t keyCode);
		FocusEventObject* AS3_clone();
		Stringp AS3_toString();
		
		//new adds
		Stringp AS3_direction_get();
		void AS3_direction_set(Stringp pDirection);
		bool AS3_isRelatedObjectInaccessible_get();
		void AS3_isRelatedObjectInaccessible_set(bool isRelatedObjectInaccessible);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FocusEventObjectSlots m_slots_FocusEventObject;
};}
#endif
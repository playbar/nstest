#ifndef _AS3KeyboardEvent_
#define _AS3KeyboardEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class KeyboardEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp KEY_DOWN = "keyDown";
	//Stringp KEY_UP = "keyUp";

	Stringp KEY_DOWN;
	Stringp KEY_UP;
private:};
class KeyboardEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class KeyboardEventObject;

	class KeyboardEventClass : public ClassClosure//EventClass
	{
	public:
		KeyboardEventObject* CreateEventObject(int id, KEYEVENTDATA&data);
		Stringp		 IDToStringType(int id);
	public:
		KeyboardEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyboardEventClassSlots m_slots_KeyboardEventClass;
	public:
		Stringp getSlotKEY_DOWN(){return m_slots_KeyboardEventClass.KEY_DOWN;}
		Stringp getSlotKEY_UP(){return m_slots_KeyboardEventClass.KEY_UP;}
};
class KeyboardEventObject : public EventObject
{
	public:
		bool		m_altKey;		//for the future
		uint32_t	m_charCode;
		bool		m_ctrlKey;
		uint32_t	m_keyCode;
		uint32_t	m_keyLocation;
		bool		m_shiftKey;
	public:
		KeyboardEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		bool AS3_altKey_get();
		void AS3_altKey_set(bool shiftKey);
		uint32_t AS3_charCode_get();
		void AS3_charCode_set(uint32_t charCode);
		bool AS3_ctrlKey_get();
		void AS3_ctrlKey_set(bool ctrlKey);
		uint32_t AS3_keyCode_get();
		void AS3_keyCode_set(uint32_t keyCode);
		uint32_t AS3_keyLocation_get();
		void AS3_keyLocation_set(uint32_t keyLocation);
		bool AS3_shiftKey_get();
		void AS3_shiftKey_set(bool shiftKey);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable,
							 uint32_t charCode, uint32_t keyCode, uint32_t keyLocation,
							 bool ctrlKey, bool altKey, bool shiftKey);
		KeyboardEventObject* AS3_clone();
		Stringp AS3_toString();
		void AS3_updateAfterEvent();

		//new adds
		bool AS3_commandKey_get();
		void AS3_commandKey_set(bool commandKey);
		bool AS3_controlKey_get();
		void AS3_controlKey_set(bool controlKey);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyboardEventObjectSlots m_slots_KeyboardEventObject;
};}
#endif
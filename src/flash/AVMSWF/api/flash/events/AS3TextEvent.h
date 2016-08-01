#ifndef _AS3TextEvent_
#define _AS3TextEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class TextEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp LINK = "link";
	//Stringp TEXT_INPUT = "textInput";

	Stringp LINK;
	Stringp TEXT_INPUT;
private:};
class TextEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextEventObject;

	class TextEventClass : public ClassClosure//EventClass
	{
	public:
		TextEventObject* CreateEventObject(int id, Stringp pText);
		Stringp		 IDToStringType(int id);
	public:
		TextEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextEventClassSlots m_slots_TextEventClass;
	public:
			Stringp getSlotLINK(){return m_slots_TextEventClass.LINK;}
			Stringp getSlotTEXT_INPUT(){return m_slots_TextEventClass.TEXT_INPUT;}
};
class TextEventObject : public EventObject
{
	public:
		DRCWB(Stringp) m_pText;
	public:
		TextEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		Stringp AS3_text_get();
		void AS3_text_set(Stringp pText);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pText);
		TextEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextEventObjectSlots m_slots_TextEventObject;
};}
#endif
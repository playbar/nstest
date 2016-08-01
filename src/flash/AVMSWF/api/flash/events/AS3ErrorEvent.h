#ifndef _AS3ErrorEvent_
#define _AS3ErrorEvent_

#include "AS3TextEvent.h"

namespace avmplus{namespace NativeID{
class ErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp ERROR = "error"

	Stringp ERROR_FORSPACE;
private:};
class ErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ErrorEventObject;

	class ErrorEventClass : public ClassClosure//TextEventClass
	{
	public:
		ErrorEventObject* CreateEventObject(int id, Stringp pText);
		Stringp		 IDToStringType(int id);
	public:
		ErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ErrorEventClassSlots m_slots_ErrorEventClass;
	public:
		Stringp getSlotERROR_FORSPACE(){return m_slots_ErrorEventClass.ERROR_FORSPACE;}
};
class ErrorEventObject : public TextEventObject
{
	public:
		ErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pText);
		ErrorEventObject* AS3_clone();
		Stringp AS3_toString();

		//new adds
		int AS3_errorID_get();
		//new adds end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ErrorEventObjectSlots m_slots_ErrorEventObject;
};}
#endif
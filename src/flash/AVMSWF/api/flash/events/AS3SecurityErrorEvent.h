#ifndef _AS3SecurityErrorEvent_
#define _AS3SecurityErrorEvent_

#include "AS3ErrorEvent.h"

namespace avmplus{namespace NativeID{
class SecurityErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp SECURITY_ERROR = "securityError";

	Stringp SECURITY_ERROR;
private:};
class SecurityErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SecurityErrorEventObject;

	class SecurityErrorEventClass : public ClassClosure//ErrorEventClass
	{
	public:
		SecurityErrorEventObject* CreateEventObject(int id, Stringp pText);
		Stringp		 IDToStringType(int id);
	public:
		SecurityErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityErrorEventClassSlots m_slots_SecurityErrorEventClass;
	public:
		Stringp getSlotSECURITY_ERROR(){return m_slots_SecurityErrorEventClass.SECURITY_ERROR;}
};
class SecurityErrorEventObject : public ErrorEventObject
{
	public:
		SecurityErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual	EventObject* Clone(EventObject *pObj);

		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp text);
		SecurityErrorEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityErrorEventObjectSlots m_slots_SecurityErrorEventObject;
};}
#endif
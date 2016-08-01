#ifndef _AS3NetStatusEvent_
#define _AS3NetStatusEvent_

#include "../core/ObjectClass.h"

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class NetStatusEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp NET_STATUS = "netStatus"; 

	Stringp NET_STATUS;
private:};
class NetStatusEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetStatusEventObject;

	class NetStatusEventClass : public ClassClosure//EventClass
	{
	public:
		NetStatusEventObject* CreateEventObject(int id, ScriptObject *info);
		Stringp		 IDToStringType(int id);
	public:
		NetStatusEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStatusEventClassSlots m_slots_NetStatusEventClass;
	public:
		Stringp getSlotNET_STATUS(){return m_slots_NetStatusEventClass.NET_STATUS;}
};
class NetStatusEventObject : public EventObject
{
	public:
//		DRCWB(ScriptObject *) m_pInfo;
		ScriptObject *m_pInfo;
	public:
		NetStatusEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject*pObj);

		Atom AS3_info_get();
		void AS3_info_set(Atom v);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, ScriptObject *pInfo);
		NetStatusEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStatusEventObjectSlots m_slots_NetStatusEventObject;
};}
#endif
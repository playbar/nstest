#ifndef _AS3SyncEvent_
#define _AS3SyncEvent_

#include "../core/ArrayObject.h"

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class SyncEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp SYNC = "sync";

	Stringp SYNC;
private:};
class SyncEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SyncEventObject;

	class SyncEventClass : public ClassClosure//EventClass
	{
	public:
		SyncEventObject* CreateEventObject(int id, ArrayObject *pChangeList);
		Stringp		 IDToStringType(int id);
	public:
		SyncEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SyncEventClassSlots m_slots_SyncEventClass;
	public:
		Stringp getSlotSYNC(){return m_slots_SyncEventClass.SYNC;}

};
class SyncEventObject : public EventObject
{
	public:
//		DRCWB(ArrayObject *) m_pChangeList;
		ArrayObject *m_pChangeList;
	public:
		SyncEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		ArrayObject* AS3_changeList_get();
		void AS3_changeList_set(ArrayObject *pChangeList);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, ArrayObject *pChangeList);
		SyncEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SyncEventObjectSlots m_slots_SyncEventObject;
};}
#endif
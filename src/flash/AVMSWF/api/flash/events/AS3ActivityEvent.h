#ifndef _AS3ActivityEvent_
#define _AS3ActivityEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class ActivityEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp ACTIVITY = "activity";

	Stringp ACTIVITY;
private:};
class ActivityEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ActivityEventObject;

	class ActivityEventClass : public ClassClosure//EventClass
	{
	public:
//		EventObject* CreateEventObject(int id);
		ActivityEventObject* CreateEventObject(int id, bool activating);
		Stringp		 IDToStringType(int id);
	public:
		ActivityEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ActivityEventClassSlots m_slots_ActivityEventClass;
	public:
		Stringp getSlotACTIVITY(){return m_slots_ActivityEventClass.ACTIVITY;}
};
class ActivityEventObject : public EventObject
{
	public:
		ActivityEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject*pObj);
		//void* pData;//Set your data!!
		bool m_bActivity;
		bool AS3_activating_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_activating_set(bool);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_constructor(Stringp,bool,bool,bool);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		ActivityEventObject* AS3_clone();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_toString();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ActivityEventObjectSlots m_slots_ActivityEventObject;
	

};}
#endif
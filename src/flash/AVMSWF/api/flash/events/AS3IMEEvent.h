#ifndef _AS3IMEEvent_
#define _AS3IMEEvent_

#include "AS3TextEvent.h"

namespace avmplus{namespace NativeID{
class IMEEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp IME_COMPOSITION = "imeComposition";
//	IME_START_COMPOSITION : String = "imeStartComposition"

	Stringp IME_COMPOSITION;

	//new adds
	Stringp IME_START_COMPOSITION;
	//new adds end

private:};
class IMEEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IMEEventObject;

	class IMEEventClass : public ClassClosure//TextEventClass
	{
	public:
		IMEEventObject* CreateEventObject(int id, Stringp pText);
		Stringp		 IDToStringType(int id);
	public:
		IMEEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IMEEventClassSlots m_slots_IMEEventClass;
	public:
		inline Stringp getSlotIME_COMPOSITION(){return m_slots_IMEEventClass.IME_COMPOSITION;}

		//new adds
		inline Stringp getSlotIME_START_COMPOSITION(){return m_slots_IMEEventClass.IME_START_COMPOSITION;}
		//new adds end
};
class IMEEventObject : public TextEventObject
{
	public:
		IMEEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp text);
		IMEEventObject* AS3_clone();
		Stringp AS3_toString();
		
		//new adds
		AvmBox AS3_imeClient_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_imeClient_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IMEEventObjectSlots m_slots_IMEEventObject;
};}
#endif
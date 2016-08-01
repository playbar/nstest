#ifndef _AS3MediaEvent_
#define _AS3MediaEvent_

#include "AS3MediaPromise.h"

namespace avmplus{namespace NativeID{
class MediaEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	Stringp COMPLETE;
    Stringp SELECT;

private:};
class MediaEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MediaEventClass : public ClassClosure//EventClass
	{
	public:
		MediaEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MediaEventClassSlots m_slots_MediaEventClass;
};
class MediaEventObject : public EventObject
{
	public:
		MediaEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		MediaPromiseObject* AS3_data_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, MediaPromiseObject *pData);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MediaEventObjectSlots m_slots_MediaEventObject;
};}
#endif
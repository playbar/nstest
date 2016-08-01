#ifndef _AS3NetDataEvent_
#define _AS3NetDataEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class NetDataEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//MEDIA_TYPE_DATA : String = "mediaTypeData"

	Stringp MEDIA_TYPE_DATA;

private:};
class NetDataEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	ScriptObject *m_info;
	double m_timestamp;
private:};
}}
namespace avmshell{
	class NetDataEventClass : public ClassClosure//EventClass
	{
	public:
		NetDataEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetDataEventClassSlots m_slots_NetDataEventClass;
};
class NetDataEventObject : public EventObject
{
	public:
		NetDataEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Atom AS3_info_get();
		//double AS3_timestamp_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double timestamp, Atom info);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetDataEventObjectSlots m_slots_NetDataEventObject;
};}
#endif
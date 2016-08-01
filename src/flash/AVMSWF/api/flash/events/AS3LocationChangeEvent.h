#ifndef _AS3LocationChangeEvent_
#define _AS3LocationChangeEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class LocationChangeEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//LOCATION_CHANGE : String = "locationChange"
	//LOCATION_CHANGING : String = "locationChanging"

	Stringp LOCATION_CHANGE;
	Stringp LOCATION_CHANGING;

private:};
class LocationChangeEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LocationChangeEventClass : public ClassClosure//EventClass
	{
	public:
		LocationChangeEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LocationChangeEventClassSlots m_slots_LocationChangeEventClass;
};
class LocationChangeEventObject : public EventObject
{
	public:
		LocationChangeEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Stringp AS3_location_get();
		void AS3_location_set(Stringp pLocation);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp location);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LocationChangeEventObjectSlots m_slots_LocationChangeEventObject;
};}
#endif
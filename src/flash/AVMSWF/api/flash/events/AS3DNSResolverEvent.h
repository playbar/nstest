#ifndef _AS3DNSResolverEvent_
#define _AS3DNSResolverEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class DNSResolverEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//LOOKUP : String = "lookup"

	Stringp LOOKUP;

private:};
class DNSResolverEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DNSResolverEventClass : public ClassClosure//EventClass
	{
	public:
		DNSResolverEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DNSResolverEventClassSlots m_slots_DNSResolverEventClass;
};
class DNSResolverEventObject : public EventObject
{
	public:
		DNSResolverEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Stringp AS3_host_get();
		void AS3_host_set(Stringp pHost);
		ArrayObject* AS3_resourceRecords_get();
		void AS3_resourceRecords_set(ArrayObject *pResourceRecords);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pHost, ArrayObject pResourceRecords);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DNSResolverEventObjectSlots m_slots_DNSResolverEventObject;
};}
#endif
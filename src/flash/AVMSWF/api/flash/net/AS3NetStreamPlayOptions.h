#ifndef _AS3NetStreamPlayOptions_
#define _AS3NetStreamPlayOptions_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class NetStreamPlayOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetStreamPlayOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp	streamName;
	Stringp	oldStreamName;
	double	start;
	double	len;
	double	offset;
	Stringp	transition;
private:};
}}
namespace avmshell{
	class NetStreamPlayOptionsClass : public ClassClosure
	{
	public:
		NetStreamPlayOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamPlayOptionsClassSlots m_slots_NetStreamPlayOptionsClass;
};
class NetStreamPlayOptionsObject : public EventDispatcherObject
{
	public:
		NetStreamPlayOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_len_get();
		//void AS3_len_set(double len);
		//double AS3_offset_get();
		//void AS3_offset_set(double offset);
		//Stringp AS3_oldStreamName_get();
		//void AS3_oldStreamName_set(Stringp pOldStreamName);
		//double AS3_start_get();
		//void AS3_start_set(double start);
		//Stringp AS3_streamName_get();
		//void AS3_streamName_set(Stringp pStreamName);
		//Stringp AS3_transition_get();
		//void AS3_transition_set(Stringp pTransition);

		//void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamPlayOptionsObjectSlots m_slots_NetStreamPlayOptionsObject;
};}
#endif
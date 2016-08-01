#ifndef _AS3NewObjectSample_
#define _AS3NewObjectSample_

#include "AS3Sample.h"
namespace avmplus{namespace NativeID{
class NewObjectSampleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
private:};
class NewObjectSampleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	ClassClosure *type;
	double id;
private:};
}}
namespace avmshell{
	class NewObjectSampleClass : public ClassClosure//SampleClass
	{
	public:
		NewObjectSampleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NewObjectSampleClassSlots m_slots_NewObjectSampleClass;
};
class NewObjectSampleObject : public SampleObject
{
	public:
		NewObjectSampleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Atom AS3_object_get();
		double AS3_size_get();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NewObjectSampleObjectSlots m_slots_NewObjectSampleObject;
};}
#endif
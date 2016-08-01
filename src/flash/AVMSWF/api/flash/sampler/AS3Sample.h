#ifndef _AS3Sample_
#define _AS3Sample_
namespace avmplus{namespace NativeID{
class SampleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	ArrayObject *stack;
	double time;

private:};
class SampleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SampleClass : public ClassClosure
	{
	public:
		SampleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SampleClassSlots m_slots_SampleClass;
};
class SampleObject : public ScriptObject
{
	public:
		SampleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SampleObjectSlots m_slots_SampleObject;
};}
#endif
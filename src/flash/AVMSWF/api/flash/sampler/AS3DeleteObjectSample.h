#ifndef _AS3DeleteObjectSample_
#define _AS3DeleteObjectSample_
namespace avmplus{namespace NativeID{
class DeleteObjectSampleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	double id;
	double size;

private:};
class DeleteObjectSampleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DeleteObjectSampleClass : public ClassClosure
	{
	public:
		DeleteObjectSampleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DeleteObjectSampleClassSlots m_slots_DeleteObjectSampleClass;
};
class DeleteObjectSampleObject : public ScriptObject
{
	public:
		DeleteObjectSampleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DeleteObjectSampleObjectSlots m_slots_DeleteObjectSampleObject;
};}
#endif
#ifndef _AS3IDynamicPropertyWriter_
#define _AS3IDynamicPropertyWriter_
namespace avmplus{namespace NativeID{
class IDynamicPropertyWriterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IDynamicPropertyWriterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IDynamicPropertyWriterClass : public ClassClosure
	{
	public:
		IDynamicPropertyWriterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDynamicPropertyWriterClassSlots m_slots_IDynamicPropertyWriterClass;
};
class IDynamicPropertyWriterObject : public ScriptObject
{
	public:
		IDynamicPropertyWriterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDynamicPropertyWriterObjectSlots m_slots_IDynamicPropertyWriterObject;
};}
#endif
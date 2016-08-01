#ifndef _AS3IDynamicPropertyOutput_
#define _AS3IDynamicPropertyOutput_
namespace avmplus{namespace NativeID{
class IDynamicPropertyOutputClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IDynamicPropertyOutputObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IDynamicPropertyOutputClass : public ClassClosure
	{
	public:
		IDynamicPropertyOutputClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDynamicPropertyOutputClassSlots m_slots_IDynamicPropertyOutputClass;
};
class IDynamicPropertyOutputObject : public ScriptObject
{
	public:
		IDynamicPropertyOutputObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDynamicPropertyOutputObjectSlots m_slots_IDynamicPropertyOutputObject;
};}
#endif
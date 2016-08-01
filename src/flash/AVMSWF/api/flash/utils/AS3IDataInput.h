#ifndef _AS3IDataInput_
#define _AS3IDataInput_
namespace avmplus{namespace NativeID{
class IDataInputClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IDataInputObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IDataInputClass : public ClassClosure
	{
	public:
		IDataInputClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDataInputClassSlots m_slots_IDataInputClass;
};
class IDataInputObject : public ScriptObject
{
	public:
		IDataInputObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDataInputObjectSlots m_slots_IDataInputObject;
};}
#endif
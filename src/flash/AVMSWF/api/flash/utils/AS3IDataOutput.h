#ifndef _AS3IDataOutput_
#define _AS3IDataOutput_
namespace avmplus{namespace NativeID{
class IDataOutputClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IDataOutputObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IDataOutputClass : public ClassClosure
	{
	public:
		IDataOutputClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDataOutputClassSlots m_slots_IDataOutputClass;
};
class IDataOutputObject : public ScriptObject
{
	public:
		IDataOutputObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IDataOutputObjectSlots m_slots_IDataOutputObject;
};}
#endif
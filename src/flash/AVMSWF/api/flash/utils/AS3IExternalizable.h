#ifndef _AS3IExternalizable_
#define _AS3IExternalizable_
namespace avmplus{namespace NativeID{
class IExternalizableClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IExternalizableObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IExternalizableClass : public ClassClosure
	{
	public:
		IExternalizableClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IExternalizableClassSlots m_slots_IExternalizableClass;
};
class IExternalizableObject : public ScriptObject
{
	public:
		IExternalizableObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IExternalizableObjectSlots m_slots_IExternalizableObject;
};}
#endif
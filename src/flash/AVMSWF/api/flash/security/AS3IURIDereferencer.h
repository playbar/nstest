#ifndef _AS3IURIDereferencer_
#define _AS3IURIDereferencer_
namespace avmplus{namespace NativeID{
class IURIDereferencerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IURIDereferencerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IURIDereferencerClass : public ClassClosure
	{
	public:
		IURIDereferencerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IURIDereferencerClassSlots m_slots_IURIDereferencerClass;
};
class IURIDereferencerObject : public ScriptObject
{
	public:
		IURIDereferencerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IURIDereferencerObjectSlots m_slots_IURIDereferencerObject;
};}
#endif
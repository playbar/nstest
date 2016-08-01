#ifndef _AS3IFilePromise_
#define _AS3IFilePromise_
namespace avmplus{namespace NativeID{
class IFilePromiseClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IFilePromiseObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IFilePromiseClass : public ClassClosure
	{
	public:
		IFilePromiseClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IFilePromiseClassSlots m_slots_IFilePromiseClass;
};
class IFilePromiseObject : public ScriptObject
{
	public:
		IFilePromiseObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IFilePromiseObjectSlots m_slots_IFilePromiseObject;
};}
#endif
#ifndef _AS3IBitmapDrawable_
#define _AS3IBitmapDrawable_
namespace avmplus{namespace NativeID{
class IBitmapDrawableClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IBitmapDrawableObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IBitmapDrawableClass : public ClassClosure
	{
	public:
		IBitmapDrawableClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IBitmapDrawableClassSlots m_slots_IBitmapDrawableClass;
};
class IBitmapDrawableObject : public ScriptObject
{
	public:
		IBitmapDrawableObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IBitmapDrawableObjectSlots m_slots_IBitmapDrawableObject;
};}
#endif
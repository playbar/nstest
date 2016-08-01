#ifndef _AS3PrintMethod_
#define _AS3PrintMethod_
namespace avmplus{namespace NativeID{
class PrintMethodClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//AUTO : String = "auto"
//BITMAP : String = "bitmap"
//VECTOR : String = "vector"

	Stringp AUTO;
	Stringp BITMAP;
	Stringp VECTOR;

private:};
class PrintMethodObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PrintMethodClass : public ClassClosure
	{
	public:
		PrintMethodClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintMethodClassSlots m_slots_PrintMethodClass;
};
class PrintMethodObject : public ScriptObject
{
	public:
		PrintMethodObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintMethodObjectSlots m_slots_PrintMethodObject;
};}
#endif
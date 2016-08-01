#ifndef _AS3NativeWindowType_
#define _AS3NativeWindowType_
namespace avmplus{namespace NativeID{
class NativeWindowTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//LIGHTWEIGHT : String = "lightweight"
//NORMAL : String = "normal"
//UTILITY : String = "utility"

	Stringp LIGHTWEIGHT;
	Stringp NORMAL;
	Stringp UTILITY;

private:};
class NativeWindowTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowTypeClass : public ClassClosure
	{
	public:
		NativeWindowTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowTypeClassSlots m_slots_NativeWindowTypeClass;
};
class NativeWindowTypeObject : public ScriptObject
{
	public:
		NativeWindowTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowTypeObjectSlots m_slots_NativeWindowTypeObject;
};}
#endif
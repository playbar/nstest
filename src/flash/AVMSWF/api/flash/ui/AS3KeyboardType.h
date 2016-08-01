#ifndef _AS3KeyboardType_
#define _AS3KeyboardType_
namespace avmplus{namespace NativeID{
class KeyboardTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ALPHANUMERIC : Stringp = "alphanumeric"
//KEYPAD : Stringp = "keypad"
//NONE : Stringp = "none"

	Stringp ALPHANUMERIC;
	Stringp KEYPAD;
	Stringp NONE;

private:};
class KeyboardTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class KeyboardTypeClass : public ClassClosure
	{
	public:
		KeyboardTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyboardTypeClassSlots m_slots_KeyboardTypeClass;
};
class KeyboardTypeObject : public ScriptObject
{
	public:
		KeyboardTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyboardTypeObjectSlots m_slots_KeyboardTypeObject;
};}
#endif
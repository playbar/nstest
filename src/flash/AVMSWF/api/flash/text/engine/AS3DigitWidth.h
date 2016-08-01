#ifndef _AS3DigitWidth_
#define _AS3DigitWidth_
namespace avmplus{namespace NativeID{
class DigitWidthClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DEFAULT : Stringp = "default"
//PROPORTIONAL : Stringp = "proportional"
//TABULAR : Stringp = "tabular"

	Stringp DEFAULT;
	Stringp PROPORTIONAL;
	Stringp TABULAR;

private:};
class DigitWidthObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DigitWidthClass : public ClassClosure
	{
	public:
		DigitWidthClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DigitWidthClassSlots m_slots_DigitWidthClass;
};
class DigitWidthObject : public ScriptObject
{
	public:
		DigitWidthObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DigitWidthObjectSlots m_slots_DigitWidthObject;
};}
#endif
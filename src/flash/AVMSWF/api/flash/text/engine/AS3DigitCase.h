#ifndef _AS3DigitCase_
#define _AS3DigitCase_
namespace avmplus{namespace NativeID{
class DigitCaseClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DEFAULT : String = "default"
//LINING : String = "lining"
//OLD_STYLE : String = "oldStyle"

	Stringp DEFAULT;
	Stringp LINING;
	Stringp OLD_STYLE;

private:};
class DigitCaseObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DigitCaseClass : public ClassClosure
	{
	public:
		DigitCaseClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getDEFAULT(){return m_slots_DigitCaseClass.DEFAULT;}
		Stringp getLINING(){return m_slots_DigitCaseClass.LINING;}
		Stringp getOLD_STYLE(){return m_slots_DigitCaseClass.OLD_STYLE;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DigitCaseClassSlots m_slots_DigitCaseClass;
};
class DigitCaseObject : public ScriptObject
{
	public:
		DigitCaseObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DigitCaseObjectSlots m_slots_DigitCaseObject;
};}
#endif
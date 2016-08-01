#ifndef _AS3DateTimeNameStyle_
#define _AS3DateTimeNameStyle_
namespace avmplus{namespace NativeID{
class DateTimeNameStyleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//FULL : String = "full"
//LONG_ABBREVIATION : String = "longAbbreviation"
//SHORT_ABBREVIATION : String = "shortAbbreviation"

	Stringp FULL;
	Stringp LONG_ABBREVIATION;
	Stringp SHORT_ABBREVIATION;

private:};
class DateTimeNameStyleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DateTimeNameStyleClass : public ClassClosure
	{
	public:
		DateTimeNameStyleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeNameStyleClassSlots m_slots_DateTimeNameStyleClass;
};
class DateTimeNameStyleObject : public ScriptObject
{
	public:
		DateTimeNameStyleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeNameStyleObjectSlots m_slots_DateTimeNameStyleObject;
};}
#endif
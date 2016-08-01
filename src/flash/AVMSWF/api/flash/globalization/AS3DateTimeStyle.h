#ifndef _AS3DateTimeStyle_
#define _AS3DateTimeStyle_
namespace avmplus{namespace NativeID{
class DateTimeStyleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CUSTOM : String = "custom"
//LONG : String = "long"
//MEDIUM : String = "medium"
//NONE : String = "none"
//SHORT : String = "short"

	Stringp CUSTOM;
	Stringp LONG;
	Stringp MEDIUM;
	Stringp NONE;
	Stringp SHORT;

private:};
class DateTimeStyleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DateTimeStyleClass : public ClassClosure
	{
	public:
		DateTimeStyleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeStyleClassSlots m_slots_DateTimeStyleClass;
};
class DateTimeStyleObject : public ScriptObject
{
	public:
		DateTimeStyleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeStyleObjectSlots m_slots_DateTimeStyleObject;
};}
#endif
#ifndef _AS3SQLColumnNameStyle_
#define _AS3SQLColumnNameStyle_
namespace avmplus{namespace NativeID{
class SQLColumnNameStyleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DEFAULT : String = "default"
//LONG : String = "long"
//SHORT : String = "short"

	Stringp DEFAULT;
	Stringp LONG;
	Stringp SHORT;

private:};
class SQLColumnNameStyleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLColumnNameStyleClass : public ClassClosure
	{
	public:
		SQLColumnNameStyleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLColumnNameStyleClassSlots m_slots_SQLColumnNameStyleClass;
};
class SQLColumnNameStyleObject : public ScriptObject
{
	public:
		SQLColumnNameStyleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLColumnNameStyleObjectSlots m_slots_SQLColumnNameStyleObject;
};}
#endif
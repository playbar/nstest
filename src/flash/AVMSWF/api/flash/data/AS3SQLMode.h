#ifndef _AS3SQLMode_
#define _AS3SQLMode_
namespace avmplus{namespace NativeID{
class SQLModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CREATE : String = "create"
//READ : String = "read"
//UPDATE : String = "update"

	Stringp CREATE;
	Stringp READ;
	Stringp UPDATE;

private:};
class SQLModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLModeClass : public ClassClosure
	{
	public:
		SQLModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLModeClassSlots m_slots_SQLModeClass;
};
class SQLModeObject : public ScriptObject
{
	public:
		SQLModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLModeObjectSlots m_slots_SQLModeObject;
};}
#endif
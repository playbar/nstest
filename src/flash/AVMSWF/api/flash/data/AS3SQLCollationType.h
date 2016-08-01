#ifndef _AS3SQLCollationType_
#define _AS3SQLCollationType_
namespace avmplus{namespace NativeID{
class SQLCollationTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//BINARY : String = "binary"
//NO_CASE : String = "noCase"

	Stringp BINARY;
	Stringp NO_CASE;

private:};
class SQLCollationTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLCollationTypeClass : public ClassClosure
	{
	public:
		SQLCollationTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLCollationTypeClassSlots m_slots_SQLCollationTypeClass;
};
class SQLCollationTypeObject : public ScriptObject
{
	public:
		SQLCollationTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLCollationTypeObjectSlots m_slots_SQLCollationTypeObject;
};}
#endif
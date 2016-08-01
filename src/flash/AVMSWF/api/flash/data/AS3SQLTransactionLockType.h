#ifndef _AS3SQLTransactionLockType_
#define _AS3SQLTransactionLockType_
namespace avmplus{namespace NativeID{
class SQLTransactionLockTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DEFERRED : String = "deferred"
//EXCLUSIVE : String = "exclusive"
//IMMEDIATE : String = "immediate"

	Stringp DEFERRED;
	Stringp EXCLUSIVE;
	Stringp IMMEDIATE;

private:};
class SQLTransactionLockTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLTransactionLockTypeClass : public ClassClosure
	{
	public:
		SQLTransactionLockTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLTransactionLockTypeClassSlots m_slots_SQLTransactionLockTypeClass;
};
class SQLTransactionLockTypeObject : public ScriptObject
{
	public:
		SQLTransactionLockTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLTransactionLockTypeObjectSlots m_slots_SQLTransactionLockTypeObject;
};}
#endif
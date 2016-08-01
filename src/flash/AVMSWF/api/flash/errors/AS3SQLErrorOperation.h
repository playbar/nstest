#ifndef _AS3SQLErrorOperation_
#define _AS3SQLErrorOperation_
namespace avmplus{namespace NativeID{
class SQLErrorOperationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ANALYZE : Stringp = "analyze"
//ATTACH : Stringp = "attach"
//BEGIN : Stringp = "begin"
//CLOSE : Stringp = "close"
//COMMIT : Stringp = "commit"
//COMPACT : Stringp = "compact"
//DEANALYZE : Stringp = "deanalyze"
//DETACH : Stringp = "detach"
//EXECUTE : Stringp = "execute"
//OPEN : Stringp = "open"
//REENCRYPT : Stringp = "reencrypt"
//RELEASE_SAVEPOINT : Stringp = "releaseSavepoint"
//ROLLBACK : Stringp = "rollback"
//ROLLBACK_TO_SAVEPOINT : Stringp = "rollbackToSavepoint"
//SCHEMA : Stringp = "schema"
//SET_SAVEPOINT : Stringp = "setSavepoint"

	Stringp ANALYZE;
	Stringp ATTACH;
	Stringp BEGIN;
	Stringp CLOSE;
	Stringp COMMIT;
	Stringp COMPACT;
	Stringp DEANALYZE;
	Stringp DETACH;
	Stringp EXECUTE;
	Stringp OPEN;
	Stringp REENCRYPT;
	Stringp RELEASE_SAVEPOINT;
	Stringp ROLLBACK;
	Stringp ROLLBACK_TO_SAVEPOINT;
	Stringp SCHEMA;
	Stringp SET_SAVEPOINT;

private:};
class SQLErrorOperationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLErrorOperationClass : public ClassClosure
	{
	public:
		SQLErrorOperationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLErrorOperationClassSlots m_slots_SQLErrorOperationClass;
};
class SQLErrorOperationObject : public ScriptObject
{
	public:
		SQLErrorOperationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLErrorOperationObjectSlots m_slots_SQLErrorOperationObject;
};}
#endif
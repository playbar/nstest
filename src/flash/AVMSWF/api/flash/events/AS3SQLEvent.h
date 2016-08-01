#ifndef _AS3SQLEvent_
#define _AS3SQLEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class SQLEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ANALYZE : Stringp = "analyze"
//ATTACH : Stringp = "attach"
//BEGIN : Stringp = "begin"
//CANCEL : Stringp = "cancel"
//CLOSE : Stringp = "close"
//COMMIT : Stringp = "commit"
//COMPACT : Stringp = "compact"
//DEANALYZE : Stringp = "deanalyze"
//DETACH : Stringp = "detach"
//OPEN : Stringp = "open"
//REENCRYPT : Stringp = "reencrypt"
//RELEASE_SAVEPOINT : Stringp = "releaseSavepoint"
//RESULT : Stringp = "result"
//ROLLBACK : Stringp = "rollback"
//ROLLBACK_TO_SAVEPOINT : Stringp = "rollbackToSavepoint"
//SCHEMA : Stringp = "schema"
//SET_SAVEPOINT : Stringp = "setSavepoint"

	Stringp ANALYZE;
	Stringp ATTACH;
	Stringp BEGIN;
	Stringp CANCEL;
	Stringp CLOSE;
	Stringp COMMIT;
	Stringp COMPACT;
	Stringp DEANALYZE;
	Stringp DETACH;
	Stringp OPEN;
	Stringp REENCRYPT;
	Stringp RELEASE_SAVEPOINT;
	Stringp RESULT;
	Stringp ROLLBACK;
	Stringp ROLLBACK_TO_SAVEPOINT;
	Stringp SCHEMA;
	Stringp SET_SAVEPOINT;

private:};
class SQLEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLEventClass : public ClassClosure//EventClass
	{
	public:
		SQLEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLEventClassSlots m_slots_SQLEventClass;
};
class SQLEventObject : public EventObject
{
	public:
		SQLEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable);
		EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLEventObjectSlots m_slots_SQLEventObject;
};}
#endif
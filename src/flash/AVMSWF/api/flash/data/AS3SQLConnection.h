#ifndef _AS3SQLConnection_
#define _AS3SQLConnection_
namespace avmplus{namespace NativeID{
class SQLConnectionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLConnectionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLConnectionClass : public ClassClosure
	{
	public:
		SQLConnectionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		bool m_bSupported;
		bool AS3_isSupported_get();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLConnectionClassSlots m_slots_SQLConnectionClass;
};
class SQLConnectionObject : public ScriptObject
{
	public:
		SQLConnectionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_autoCompact_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cacheSize_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cacheSize_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_columnNameStyle_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_columnNameStyle_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_connected_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_inTransaction_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_lastInsertRowID_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_pageSize_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_totalChanges_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_addEventListener(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_analyze(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_attach(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_begin(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cancel(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_close(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_commit(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_compact(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_deanalyze(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_detach(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getSchemaResult(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_loadSchema(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_open(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_openAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_reencrypt(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_releaseSavepoint(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_removeEventListener(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_rollback(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_rollbackToSavepoint(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_setSavepoint(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLConnectionObjectSlots m_slots_SQLConnectionObject;
};}
#endif
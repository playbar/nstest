#ifndef _AS3SQLStatement_
#define _AS3SQLStatement_
namespace avmplus{namespace NativeID{
class SQLStatementClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLStatementObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLStatementClass : public ClassClosure
	{
	public:
		SQLStatementClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLStatementClassSlots m_slots_SQLStatementClass;
};
class SQLStatementObject : public ScriptObject
{
	public:
		SQLStatementObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_executing_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_itemClass_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_itemClass_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_parameters_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_sqlConnection_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_sqlConnection_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_text_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_text_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cancel(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_clearParameters(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_execute(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getResult(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_next(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLStatementObjectSlots m_slots_SQLStatementObject;
};}
#endif
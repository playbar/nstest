#ifndef _AS3SQLSchemaResult_
#define _AS3SQLSchemaResult_
namespace avmplus{namespace NativeID{
class SQLSchemaResultClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLSchemaResultObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLSchemaResultClass : public ClassClosure
	{
	public:
		SQLSchemaResultClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLSchemaResultClassSlots m_slots_SQLSchemaResultClass;
};
class SQLSchemaResultObject : public ScriptObject
{
	public:
		SQLSchemaResultObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_indices_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_tables_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_triggers_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_views_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLSchemaResultObjectSlots m_slots_SQLSchemaResultObject;
};}
#endif
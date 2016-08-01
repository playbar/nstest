#ifndef _AS3SQLSchema_
#define _AS3SQLSchema_
namespace avmplus{namespace NativeID{
class SQLSchemaClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLSchemaObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLSchemaClass : public ClassClosure
	{
	public:
		SQLSchemaClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLSchemaClassSlots m_slots_SQLSchemaClass;
};
class SQLSchemaObject : public ScriptObject
{
	public:
		SQLSchemaObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_database_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_name_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_sql_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLSchemaObjectSlots m_slots_SQLSchemaObject;
};}
#endif
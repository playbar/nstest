#ifndef _AS3SQLTableSchema_
#define _AS3SQLTableSchema_
namespace avmplus{namespace NativeID{
class SQLTableSchemaClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLTableSchemaObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLTableSchemaClass : public ClassClosure
	{
	public:
		SQLTableSchemaClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLTableSchemaClassSlots m_slots_SQLTableSchemaClass;
};
class SQLTableSchemaObject : public ScriptObject
{
	public:
		SQLTableSchemaObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_columns_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLTableSchemaObjectSlots m_slots_SQLTableSchemaObject;
};}
#endif
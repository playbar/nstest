#ifndef _AS3SQLIndexSchema_
#define _AS3SQLIndexSchema_
namespace avmplus{namespace NativeID{
class SQLIndexSchemaClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLIndexSchemaObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLIndexSchemaClass : public ClassClosure
	{
	public:
		SQLIndexSchemaClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLIndexSchemaClassSlots m_slots_SQLIndexSchemaClass;
};
class SQLIndexSchemaObject : public ScriptObject
{
	public:
		SQLIndexSchemaObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_table_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLIndexSchemaObjectSlots m_slots_SQLIndexSchemaObject;
};}
#endif
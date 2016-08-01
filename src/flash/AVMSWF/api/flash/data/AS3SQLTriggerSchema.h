#ifndef _AS3SQLTriggerSchema_
#define _AS3SQLTriggerSchema_
namespace avmplus{namespace NativeID{
class SQLTriggerSchemaClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLTriggerSchemaObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLTriggerSchemaClass : public ClassClosure
	{
	public:
		SQLTriggerSchemaClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLTriggerSchemaClassSlots m_slots_SQLTriggerSchemaClass;
};
class SQLTriggerSchemaObject : public ScriptObject
{
	public:
		SQLTriggerSchemaObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_table_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLTriggerSchemaObjectSlots m_slots_SQLTriggerSchemaObject;
};}
#endif
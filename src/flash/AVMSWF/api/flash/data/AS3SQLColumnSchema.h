#ifndef _AS3SQLColumnSchema_
#define _AS3SQLColumnSchema_
namespace avmplus{namespace NativeID{
class SQLColumnSchemaClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLColumnSchemaObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLColumnSchemaClass : public ClassClosure
	{
	public:
		SQLColumnSchemaClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLColumnSchemaClassSlots m_slots_SQLColumnSchemaClass;
};
class SQLColumnSchemaObject : public ScriptObject
{
	public:
		SQLColumnSchemaObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_allowNull_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_autoIncrement_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_dataType_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_defaultCollationType_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_name_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_primaryKey_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLColumnSchemaObjectSlots m_slots_SQLColumnSchemaObject;
};}
#endif
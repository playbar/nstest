#ifndef _AS3SQLViewSchema_
#define _AS3SQLViewSchema_
namespace avmplus{namespace NativeID{
class SQLViewSchemaClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLViewSchemaObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLViewSchemaClass : public ClassClosure
	{
	public:
		SQLViewSchemaClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLViewSchemaClassSlots m_slots_SQLViewSchemaClass;
};
class SQLViewSchemaObject : public ScriptObject
{
	public:
		SQLViewSchemaObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLViewSchemaObjectSlots m_slots_SQLViewSchemaObject;
};}
#endif
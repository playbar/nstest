#ifndef _AS3SQLResult_
#define _AS3SQLResult_
namespace avmplus{namespace NativeID{
class SQLResultClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLResultObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLResultClass : public ClassClosure
	{
	public:
		SQLResultClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLResultClassSlots m_slots_SQLResultClass;
};
class SQLResultObject : public ScriptObject
{
	public:
		SQLResultObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_complete_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_data_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_lastInsertRowID_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_rowsAffected_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLResultObjectSlots m_slots_SQLResultObject;
};}
#endif
#ifndef _AS3SRVRecord_
#define _AS3SRVRecord_
namespace avmplus{namespace NativeID{
class SRVRecordClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SRVRecordObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SRVRecordClass : public ClassClosure
	{
	public:
		SRVRecordClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SRVRecordClassSlots m_slots_SRVRecordClass;
};
class SRVRecordObject : public ScriptObject
{
	public:
		SRVRecordObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_port_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_port_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_priority_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_priority_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_target_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_target_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_weight_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_weight_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SRVRecordObjectSlots m_slots_SRVRecordObject;
};}
#endif
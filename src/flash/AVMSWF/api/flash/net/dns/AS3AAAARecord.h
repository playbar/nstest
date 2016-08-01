#ifndef _AS3AAAARecord_
#define _AS3AAAARecord_
namespace avmplus{namespace NativeID{
class AAAARecordClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class AAAARecordObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class AAAARecordClass : public ClassClosure
	{
	public:
		AAAARecordClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AAAARecordClassSlots m_slots_AAAARecordClass;
};
class AAAARecordObject : public ScriptObject
{
	public:
		AAAARecordObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_address_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_address_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AAAARecordObjectSlots m_slots_AAAARecordObject;
};}
#endif
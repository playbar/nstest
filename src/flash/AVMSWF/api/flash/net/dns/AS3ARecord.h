#ifndef _AS3ARecord_
#define _AS3ARecord_
namespace avmplus{namespace NativeID{
class ARecordClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ARecordObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ARecordClass : public ClassClosure
	{
	public:
		ARecordClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ARecordClassSlots m_slots_ARecordClass;
};
class ARecordObject : public ScriptObject
{
	public:
		ARecordObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_address_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_address_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ARecordObjectSlots m_slots_ARecordObject;
};}
#endif
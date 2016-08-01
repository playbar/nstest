#ifndef _AS3MXRecord_
#define _AS3MXRecord_
namespace avmplus{namespace NativeID{
class MXRecordClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MXRecordObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MXRecordClass : public ClassClosure
	{
	public:
		MXRecordClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MXRecordClassSlots m_slots_MXRecordClass;
};
class MXRecordObject : public ScriptObject
{
	public:
		MXRecordObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_exchange_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_exchange_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_preference_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_preference_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MXRecordObjectSlots m_slots_MXRecordObject;
};}
#endif
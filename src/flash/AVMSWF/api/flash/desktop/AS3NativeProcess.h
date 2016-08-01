#ifndef _AS3NativeProcess_
#define _AS3NativeProcess_
namespace avmplus{namespace NativeID{
class NativeProcessClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeProcessObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeProcessClass : public ClassClosure
	{
	public:
		NativeProcessClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeProcessClassSlots m_slots_NativeProcessClass;
};
class NativeProcessObject : public ScriptObject
{
	public:
		NativeProcessObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_isSupported_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_running_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_standardError_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_standardInput_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_standardOutput_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_closeInput(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_exit(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_start(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeProcessObjectSlots m_slots_NativeProcessObject;
};}
#endif
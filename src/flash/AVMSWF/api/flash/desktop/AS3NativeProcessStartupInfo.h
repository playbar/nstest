#ifndef _AS3NativeProcessStartupInfo_
#define _AS3NativeProcessStartupInfo_
namespace avmplus{namespace NativeID{
class NativeProcessStartupInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeProcessStartupInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeProcessStartupInfoClass : public ClassClosure
	{
	public:
		NativeProcessStartupInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeProcessStartupInfoClassSlots m_slots_NativeProcessStartupInfoClass;
};
class NativeProcessStartupInfoObject : public ScriptObject
{
	public:
		NativeProcessStartupInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_arguments_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_arguments_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_executable_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_executable_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_workingDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_workingDirectory_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeProcessStartupInfoObjectSlots m_slots_NativeProcessStartupInfoObject;
};}
#endif
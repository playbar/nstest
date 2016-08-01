#ifndef _AS3arguments_
#define _AS3arguments_
namespace avmplus{namespace NativeID{
class argumentsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class argumentsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class argumentsClass : public ClassClosure
	{
	public:
		argumentsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::argumentsClassSlots m_slots_argumentsClass;
};
class argumentsObject : public ScriptObject
{
	public:
		argumentsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_callee_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_callee_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_length_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_length_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::argumentsObjectSlots m_slots_argumentsObject;
};}
#endif
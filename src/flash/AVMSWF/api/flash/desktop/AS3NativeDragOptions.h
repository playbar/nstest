#ifndef _AS3NativeDragOptions_
#define _AS3NativeDragOptions_
namespace avmplus{namespace NativeID{
class NativeDragOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeDragOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeDragOptionsClass : public ClassClosure
	{
	public:
		NativeDragOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragOptionsClassSlots m_slots_NativeDragOptionsClass;
};
class NativeDragOptionsObject : public ScriptObject
{
	public:
		NativeDragOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_allowCopy_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_allowCopy_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_allowLink_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_allowLink_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_allowMove_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_allowMove_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_toString(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragOptionsObjectSlots m_slots_NativeDragOptionsObject;
};}
#endif
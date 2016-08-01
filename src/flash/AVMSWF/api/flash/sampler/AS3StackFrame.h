#ifndef _AS3StackFrame_
#define _AS3StackFrame_
namespace avmplus{namespace NativeID{
class StackFrameClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	Stringp file;
	uint32_t line;
	Stringp name;
	double scriptID;

private:};
class StackFrameObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StackFrameClass : public ClassClosure
	{
	public:
		StackFrameClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StackFrameClassSlots m_slots_StackFrameClass;
};
class StackFrameObject : public ScriptObject
{
	public:
		StackFrameObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//AvmBox AS3_toString(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StackFrameObjectSlots m_slots_StackFrameObject;
};}
#endif
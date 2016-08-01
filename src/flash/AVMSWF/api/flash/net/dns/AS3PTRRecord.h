#ifndef _AS3PTRRecord_
#define _AS3PTRRecord_
namespace avmplus{namespace NativeID{
class PTRRecordClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class PTRRecordObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PTRRecordClass : public ClassClosure
	{
	public:
		PTRRecordClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PTRRecordClassSlots m_slots_PTRRecordClass;
};
class PTRRecordObject : public ScriptObject
{
	public:
		PTRRecordObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_ptrdName_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_ptrdName_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PTRRecordObjectSlots m_slots_PTRRecordObject;
};}
#endif
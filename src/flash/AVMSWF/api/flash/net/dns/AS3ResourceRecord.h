#ifndef _AS3ResourceRecord_
#define _AS3ResourceRecord_
namespace avmplus{namespace NativeID{
class ResourceRecordClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ResourceRecordObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ResourceRecordClass : public ClassClosure
	{
	public:
		ResourceRecordClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ResourceRecordClassSlots m_slots_ResourceRecordClass;
};
class ResourceRecordObject : public ScriptObject
{
	public:
		ResourceRecordObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_name_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_name_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_ttl_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_ttl_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ResourceRecordObjectSlots m_slots_ResourceRecordObject;
};}
#endif
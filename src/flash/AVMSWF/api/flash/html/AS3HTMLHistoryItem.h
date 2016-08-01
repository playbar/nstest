#ifndef _AS3HTMLHistoryItem_
#define _AS3HTMLHistoryItem_
namespace avmplus{namespace NativeID{
class HTMLHistoryItemClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class HTMLHistoryItemObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTMLHistoryItemClass : public ClassClosure
	{
	public:
		HTMLHistoryItemClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLHistoryItemClassSlots m_slots_HTMLHistoryItemClass;
};
class HTMLHistoryItemObject : public ScriptObject
{
	public:
		HTMLHistoryItemObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_isPost_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_originalUrl_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_title_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_url_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLHistoryItemObjectSlots m_slots_HTMLHistoryItemObject;
};}
#endif
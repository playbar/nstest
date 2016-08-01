#ifndef _AS3PrintUIOptions_
#define _AS3PrintUIOptions_
namespace avmplus{namespace NativeID{
class PrintUIOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class PrintUIOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PrintUIOptionsClass : public ClassClosure
	{
	public:
		PrintUIOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintUIOptionsClassSlots m_slots_PrintUIOptionsClass;
};
class PrintUIOptionsObject : public ScriptObject
{
	public:
		PrintUIOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_disablePageRange_get();
		void AS3_disablePageRange_set(bool disablePageRange);
		uint32_t AS3_maxPage_get();
		void AS3_maxPage_set(uint32_t maxPage);
		uint32_t AS3_minPage_get();
		void AS3_minPage_set(uint32_t minPage);

		void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintUIOptionsObjectSlots m_slots_PrintUIOptionsObject;
};}
#endif
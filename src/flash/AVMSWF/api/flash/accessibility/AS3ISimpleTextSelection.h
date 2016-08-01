#ifndef _AS3ISimpleTextSelection_
#define _AS3ISimpleTextSelection_
namespace avmplus{namespace NativeID{
class ISimpleTextSelectionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ISimpleTextSelectionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ISimpleTextSelectionClass : public ClassClosure
	{
	public:
		ISimpleTextSelectionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ISimpleTextSelectionClassSlots m_slots_ISimpleTextSelectionClass;
};
class ISimpleTextSelectionObject : public ScriptObject
{
	public:
		ISimpleTextSelectionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ISimpleTextSelectionObjectSlots m_slots_ISimpleTextSelectionObject;
};}
#endif
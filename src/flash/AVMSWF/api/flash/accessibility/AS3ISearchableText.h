#ifndef _AS3ISearchableText_
#define _AS3ISearchableText_
namespace avmplus{namespace NativeID{
class ISearchableTextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ISearchableTextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ISearchableTextClass : public ClassClosure
	{
	public:
		ISearchableTextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ISearchableTextClassSlots m_slots_ISearchableTextClass;
};
class ISearchableTextObject : public ScriptObject
{
	public:
		ISearchableTextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ISearchableTextObjectSlots m_slots_ISearchableTextObject;
};}
#endif
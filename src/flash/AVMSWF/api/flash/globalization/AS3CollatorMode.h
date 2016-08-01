#ifndef _AS3CollatorMode_
#define _AS3CollatorMode_
namespace avmplus{namespace NativeID{
class CollatorModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//MATCHING : String = "matching"
//SORTING : String = "sorting"

	Stringp MATCHING;
	Stringp SORTING;

private:};
class CollatorModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CollatorModeClass : public ClassClosure
	{
	public:
		CollatorModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CollatorModeClassSlots m_slots_CollatorModeClass;
};
class CollatorModeObject : public ScriptObject
{
	public:
		CollatorModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CollatorModeObjectSlots m_slots_CollatorModeObject;
};}
#endif
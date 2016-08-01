#ifndef _AS3JustificationStyle_
#define _AS3JustificationStyle_
namespace avmplus{namespace NativeID{
class JustificationStyleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//PRIORITIZE_LEAST_ADJUSTMENT : String = "prioritizeLeastAdjustment"
//PUSH_IN_KINSOKU : String = "pushInKinsoku"
//PUSH_OUT_ONLY : String = "pushOutOnly"

	Stringp PRIORITIZE_LEAST_ADJUSTMENT;
	Stringp PUSH_IN_KINSOKU;
	Stringp PUSH_OUT_ONLY;

private:};
class JustificationStyleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class JustificationStyleClass : public ClassClosure
	{
	public:
		JustificationStyleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JustificationStyleClassSlots m_slots_JustificationStyleClass;
};
class JustificationStyleObject : public ScriptObject
{
	public:
		JustificationStyleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JustificationStyleObjectSlots m_slots_JustificationStyleObject;
};}
#endif
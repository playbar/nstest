#ifndef _AS3BreakOpportunity_
#define _AS3BreakOpportunity_
namespace avmplus{namespace NativeID{
class BreakOpportunityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ALL : String = "all"
//ANY : String = "any"
//AUTO : String = "auto"
//NONE : String = "none"

	Stringp ALL;
	Stringp ANY;
	Stringp AUTO;
	Stringp NONE;

private:};
class BreakOpportunityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BreakOpportunityClass : public ClassClosure
	{
	public:
		enum {BREAK_ALL,BREAK_ANY,BREAK_AUTO,BREAK_NONE};
		BreakOpportunityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		int GetBreakMode(Stringp strMode);
		Stringp GetBreakName(int nMode);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BreakOpportunityClassSlots m_slots_BreakOpportunityClass;
};
class BreakOpportunityObject : public ScriptObject
{
	public:
		BreakOpportunityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BreakOpportunityObjectSlots m_slots_BreakOpportunityObject;
};}
#endif
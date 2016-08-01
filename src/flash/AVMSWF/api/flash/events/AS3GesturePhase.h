#ifndef _AS3GesturePhase_
#define _AS3GesturePhase_
namespace avmplus{namespace NativeID{
class GesturePhaseClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ALL : String = "all"
//BEGIN : String = "begin"
//END : String = "end"
//UPDATE : String = "update"

	Stringp ALL;
	Stringp BEGIN;
	Stringp END;
	Stringp UPDATE;

private:};
class GesturePhaseObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GesturePhaseClass : public ClassClosure
	{
	public:
		GesturePhaseClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GesturePhaseClassSlots m_slots_GesturePhaseClass;
};
class GesturePhaseObject : public ScriptObject
{
	public:
		GesturePhaseObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GesturePhaseObjectSlots m_slots_GesturePhaseObject;
};}
#endif
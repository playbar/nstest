#ifndef _AS3CFFHinting_
#define _AS3CFFHinting_
namespace avmplus{namespace NativeID{
class CFFHintingClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//HORIZONTAL_STEM : String = "horizontalStem"
//NONE : String = "none"

	Stringp HORIZONTAL_STEM;
	Stringp NONE;

private:};
class CFFHintingObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CFFHintingClass : public ClassClosure
	{
	public:
		CFFHintingClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getHORIZONTAL_STEM(){return m_slots_CFFHintingClass.HORIZONTAL_STEM;}
		Stringp getNONE(){return m_slots_CFFHintingClass.NONE;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CFFHintingClassSlots m_slots_CFFHintingClass;
};
class CFFHintingObject : public ScriptObject
{
	public:
		CFFHintingObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CFFHintingObjectSlots m_slots_CFFHintingObject;
};}
#endif
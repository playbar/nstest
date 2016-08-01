#ifndef _AS3TypographicCase_
#define _AS3TypographicCase_
namespace avmplus{namespace NativeID{
class TypographicCaseClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CAPS : String = "caps"
//CAPS_AND_SMALL_CAPS : String = "capsAndSmallCaps"
//DEFAULT : String = "default"
//LOWERCASE : String = "lowercase"
//SMALL_CAPS : String = "smallCaps"
//TITLE : String = "title"
//UPPERCASE : String = "uppercase"

	Stringp CAPS;
	Stringp CAPS_AND_SMALL_CAPS;
	Stringp DEFAULT;
	Stringp LOWERCASE;
	Stringp SMALL_CAPS;
	Stringp TITLE;
	Stringp UPPERCASE;

private:};
class TypographicCaseObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TypographicCaseClass : public ClassClosure
	{
	public:
		TypographicCaseClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getCAPS(){return m_slots_TypographicCaseClass.CAPS;}
		Stringp getCAPS_AND_SMALL_CAPS(){return m_slots_TypographicCaseClass.CAPS_AND_SMALL_CAPS;}
		Stringp getDEFAULT(){return m_slots_TypographicCaseClass.DEFAULT;}
		Stringp getLOWERCASE(){return m_slots_TypographicCaseClass.LOWERCASE;}
		Stringp getSMALL_CAPS(){return m_slots_TypographicCaseClass.SMALL_CAPS;}
		Stringp getTITLE(){return m_slots_TypographicCaseClass.TITLE;}
		Stringp getUPPERCASE(){return m_slots_TypographicCaseClass.UPPERCASE;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TypographicCaseClassSlots m_slots_TypographicCaseClass;
};
class TypographicCaseObject : public ScriptObject
{
	public:
		TypographicCaseObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TypographicCaseObjectSlots m_slots_TypographicCaseObject;
};}
#endif
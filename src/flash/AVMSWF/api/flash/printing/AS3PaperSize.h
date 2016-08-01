#ifndef _AS3PaperSize_
#define _AS3PaperSize_
namespace avmplus{namespace NativeID{
class PaperSizeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//A4 : Stringp = "a4"
//A5 : Stringp = "a5"
//A6 : Stringp = "a6"
//CHOUKEI3GOU : Stringp = "choukei3gou"
//CHOUKEI4GOU : Stringp = "choukei4gou"
//ENV_10 : Stringp = "env_10"
//ENV_B5 : Stringp = "env_b5"
//ENV_C5 : Stringp = "env_c5"
//ENV_DL : Stringp = "env_dl"
//ENV_MONARCH : Stringp = "env_monarch"
//ENV_PERSONAL : Stringp = "env_personal"
//EXECUTIVE : Stringp = "executive"
//FOLIO : Stringp = "folio"
//JIS_B5 : Stringp = "jis_b5"
//LEGAL : Stringp = "legal"
//LETTER : Stringp = "letter"
//STATEMENT : Stringp = "statement"

	Stringp A4;
	Stringp A5;
	Stringp A6;
	Stringp CHOUKEI3GOU;
	Stringp CHOUKEI4GOU;
	Stringp ENV_10;
	Stringp ENV_B5;
	Stringp ENV_C5;
	Stringp ENV_DL;
	Stringp ENV_MONARCH;
	Stringp ENV_PERSONAL;
	Stringp EXECUTIVE;
	Stringp FOLIO;
	Stringp JIS_B5;
	Stringp LEGAL;
	Stringp LETTER;
	Stringp STATEMENT;

private:};
class PaperSizeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PaperSizeClass : public ClassClosure
	{
	public:
		PaperSizeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PaperSizeClassSlots m_slots_PaperSizeClass;
};
class PaperSizeObject : public ScriptObject
{
	public:
		PaperSizeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PaperSizeObjectSlots m_slots_PaperSizeObject;
};}
#endif
#ifndef _AS3LigatureLevel_
#define _AS3LigatureLevel_
namespace avmplus{namespace NativeID{
class LigatureLevelClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//COMMON : String = "common"
	//EXOTIC : String = "exotic"
	//MINIMUM : String = "minimum"
	//NONE : String = "none"
	//UNCOMMON : String = "uncommon"

	Stringp COMMON;
	Stringp EXOTIC;
	Stringp MINIMUM;
	Stringp NONE;
	Stringp UNCOMMON;

private:};
class LigatureLevelObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LigatureLevelClass : public ClassClosure
	{
	public:
		LigatureLevelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getCOMMON(){return m_slots_LigatureLevelClass.COMMON;}
		Stringp getEXOTIC(){return m_slots_LigatureLevelClass.EXOTIC;}
		Stringp getMINIMUM(){return m_slots_LigatureLevelClass.MINIMUM;}
		Stringp getNONE(){return m_slots_LigatureLevelClass.NONE;}
		Stringp getUNCOMMON(){return m_slots_LigatureLevelClass.UNCOMMON;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LigatureLevelClassSlots m_slots_LigatureLevelClass;
};
class LigatureLevelObject : public ScriptObject
{
	public:
		LigatureLevelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LigatureLevelObjectSlots m_slots_LigatureLevelObject;
};}
#endif
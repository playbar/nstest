#ifndef _AS3Kerning_
#define _AS3Kerning_
namespace avmplus{namespace NativeID{
class KerningClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//AUTO : String = "auto"
//OFF : String = "off"
//ON : String = "on"

	Stringp AUTO;
	Stringp OFF;
	Stringp ON;

private:};
class KerningObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class KerningClass : public ClassClosure
	{
	public:
		KerningClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getAUTO(){return m_slots_KerningClass.AUTO;}
		Stringp getOFF(){return m_slots_KerningClass.OFF;}
		Stringp getON(){return m_slots_KerningClass.ON;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KerningClassSlots m_slots_KerningClass;
};
class KerningObject : public ScriptObject
{
	public:
		KerningObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KerningObjectSlots m_slots_KerningObject;
};}
#endif
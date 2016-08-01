#ifndef _AS3FontType_
#define _AS3FontType_
namespace avmplus{namespace NativeID{
class FontTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp DEVICE = "device";
	//Stringp EMBEDDED = "embedded"; 

	//EMBEDDED_CFF : String = "embeddedCFF"

	Stringp DEVICE;
	Stringp EMBEDDED; 

	//new adds
	Stringp EMBEDDED_CFF;
	//new adds end

private:};
class FontTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	
private:};
}}
namespace avmshell{
	class FontTypeClass : public ClassClosure
	{
	public:
		FontTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontTypeClassSlots m_slots_FontTypeClass;
	public:
		inline Stringp getSlotDEVICE(){return m_slots_FontTypeClass.DEVICE;}
		inline Stringp getSlotEMBEDDED(){return m_slots_FontTypeClass.EMBEDDED;}

		//new adds
		inline Stringp getSlotEMBEDDED_CFF(){return m_slots_FontTypeClass.EMBEDDED_CFF;}
		//new adds end
};
class FontTypeObject : public ScriptObject
{
	public:
		FontTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontTypeObjectSlots m_slots_FontTypeObject;
};}
#endif
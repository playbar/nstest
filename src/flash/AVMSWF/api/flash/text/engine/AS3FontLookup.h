#ifndef _AS3FontLookup_
#define _AS3FontLookup_
namespace avmplus{namespace NativeID{
class FontLookupClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DEVICE : String = "device"
//EMBEDDED_CFF : String = "embeddedCFF"

	Stringp DEVICE;
	Stringp EMBEDDED_CFF;

private:};
class FontLookupObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FontLookupClass : public ClassClosure
	{
	public:
		FontLookupClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getDEVICE(){return m_slots_FontLookupClass.DEVICE;}
		Stringp getEMBEDDED_CFF(){return m_slots_FontLookupClass.EMBEDDED_CFF;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontLookupClassSlots m_slots_FontLookupClass;
};
class FontLookupObject : public ScriptObject
{
	public:
		FontLookupObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontLookupObjectSlots m_slots_FontLookupObject;
};}
#endif
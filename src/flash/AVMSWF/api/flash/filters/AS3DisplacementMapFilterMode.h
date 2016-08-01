#ifndef _AS3DisplacementMapFilterMode_
#define _AS3DisplacementMapFilterMode_
namespace avmplus{namespace NativeID{
class DisplacementMapFilterModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp CLAMP = "clamp";
	//Stringp COLOR = "color";
	//Stringp IGNORE = "ignore";
	//Stringp WRAP= "wrap";

	Stringp CLAMP;
	Stringp COLOR;
	Stringp IGNORE_FORSPACE;
	Stringp WRAP;
private:};
class DisplacementMapFilterModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	
private:};
}}
namespace avmshell{
	class DisplacementMapFilterModeClass : public ClassClosure
	{
	public:
		DisplacementMapFilterModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplacementMapFilterModeClassSlots m_slots_DisplacementMapFilterModeClass;
	public:
		inline Stringp getSlotCLAMP(){m_slots_DisplacementMapFilterModeClass.CLAMP;}
		inline Stringp getSlotCOLOR(){m_slots_DisplacementMapFilterModeClass.COLOR;}
		inline Stringp getSlotIGNORE(){m_slots_DisplacementMapFilterModeClass.IGNORE_FORSPACE;}
		inline Stringp getSlotWRAP(){m_slots_DisplacementMapFilterModeClass.WRAP;}
};
class DisplacementMapFilterModeObject : public ScriptObject
{
	public:
		DisplacementMapFilterModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplacementMapFilterModeObjectSlots m_slots_DisplacementMapFilterModeObject;
};}
#endif
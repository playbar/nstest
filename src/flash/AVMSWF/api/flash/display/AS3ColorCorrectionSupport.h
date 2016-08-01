#ifndef _AS3ColorCorrectionSupport_
#define _AS3ColorCorrectionSupport_
namespace avmplus{namespace NativeID{
class ColorCorrectionSupportClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DEFAULT_OFF : String = "defaultOff";
//DEFAULT_ON : String = "defaultOn";
//UNSUPPORTED : String = "unsupported";

	Stringp DEFAULT_OFF;
	Stringp DEFAULT_ON;
	Stringp UNSUPPORTED;

private:};
class ColorCorrectionSupportObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ColorCorrectionSupportClass : public ClassClosure
	{
	public:
		ColorCorrectionSupportClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		//new adds
		inline Stringp getSlotDEFAULT_OFF(){return m_slots_ColorCorrectionSupportClass.DEFAULT_OFF;}
		inline Stringp getSlotDEFAULT_ON(){return m_slots_ColorCorrectionSupportClass.DEFAULT_ON;}
		inline Stringp getSlotUNSUPPORTED(){return m_slots_ColorCorrectionSupportClass.UNSUPPORTED;}
		//new adds end

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorCorrectionSupportClassSlots m_slots_ColorCorrectionSupportClass;
};
class ColorCorrectionSupportObject : public ScriptObject
{
	public:
		ColorCorrectionSupportObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorCorrectionSupportObjectSlots m_slots_ColorCorrectionSupportObject;
};}
#endif
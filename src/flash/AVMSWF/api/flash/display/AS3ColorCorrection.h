#ifndef _AS3ColorCorrection_
#define _AS3ColorCorrection_
namespace avmplus{namespace NativeID{
class ColorCorrectionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

 //	  DEFAULT : String = "default"
 //   OFF : String = "off"
 //   ON : String = "on"

	Stringp DEFAULT;
	Stringp OFF;
	Stringp ON;

private:};
class ColorCorrectionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ColorCorrectionClass : public ClassClosure
	{
	public:
		ColorCorrectionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		//new adds
		inline Stringp getSlotDEFAULT(){return m_slots_ColorCorrectionClass.DEFAULT;}
		inline Stringp getSlotOFF(){return m_slots_ColorCorrectionClass.OFF;}
		inline Stringp getSlotON(){return m_slots_ColorCorrectionClass.ON;}
		//new adds end

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorCorrectionClassSlots m_slots_ColorCorrectionClass;
};
class ColorCorrectionObject : public ScriptObject
{
	public:
		ColorCorrectionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorCorrectionObjectSlots m_slots_ColorCorrectionObject;
};}
#endif
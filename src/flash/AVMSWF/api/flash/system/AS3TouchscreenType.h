#ifndef _AS3TouchscreenType_
#define _AS3TouchscreenType_
namespace avmplus{namespace NativeID{
class TouchscreenTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//FINGER : Stringp = "finger"
//NONE : Stringp = "none"
//STYLUS : Stringp = "stylus"

	Stringp FINGER;
	Stringp NONE;
	Stringp STYLUS;

private:};
class TouchscreenTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TouchscreenTypeClass : public ClassClosure
	{
	public:
		TouchscreenTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	//new adds
	public:
		Stringp GetSlotFINGER(){return m_slots_TouchscreenTypeClass.FINGER;}
		Stringp GetSlotNONE(){return m_slots_TouchscreenTypeClass.NONE;}
		Stringp GetSlotSTYLUS(){return m_slots_TouchscreenTypeClass.STYLUS;}
	//new adds end

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TouchscreenTypeClassSlots m_slots_TouchscreenTypeClass;
};
class TouchscreenTypeObject : public ScriptObject
{
	public:
		TouchscreenTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TouchscreenTypeObjectSlots m_slots_TouchscreenTypeObject;
};}
#endif
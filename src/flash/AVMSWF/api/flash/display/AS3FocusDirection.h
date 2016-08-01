#ifndef _AS3FocusDirection_
#define _AS3FocusDirection_
namespace avmplus{namespace NativeID{
class FocusDirectionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//BOTTOM : String = "bottom"
//NONE : String = "none"
//TOP : String = "top"

	Stringp BOTTOM;
	Stringp NONE;
	Stringp TOP;

private:};
class FocusDirectionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FocusDirectionClass : public ClassClosure
	{
	public:
		FocusDirectionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		//new adds
		inline Stringp getSlotBOTTOM(){return m_slots_FocusDirectionClass.BOTTOM;}
		inline Stringp getSlotNONE(){return m_slots_FocusDirectionClass.NONE;}
		inline Stringp getSlotTOP(){return m_slots_FocusDirectionClass.TOP;}
		//new adds end

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FocusDirectionClassSlots m_slots_FocusDirectionClass;
};
class FocusDirectionObject : public ScriptObject
{
	public:
		FocusDirectionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FocusDirectionObjectSlots m_slots_FocusDirectionObject;
};}
#endif
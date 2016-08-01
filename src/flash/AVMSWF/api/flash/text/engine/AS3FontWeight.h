#ifndef _AS3FontWeight_
#define _AS3FontWeight_
namespace avmplus{namespace NativeID{
class FontWeightClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//BOLD : String = "bold"
//NORMAL : String = "normal"

	Stringp BOLD;
	Stringp NORMAL;

private:};
class FontWeightObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FontWeightClass : public ClassClosure
	{
	public:
		FontWeightClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontWeightClassSlots m_slots_FontWeightClass;
	public:
		inline Stringp getSlotBOLD(){return m_slots_FontWeightClass.BOLD;}
		inline Stringp getSlotNORMAL(){return m_slots_FontWeightClass.NORMAL;}
};
class FontWeightObject : public ScriptObject
{
	public:
		FontWeightObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontWeightObjectSlots m_slots_FontWeightObject;
};}
#endif
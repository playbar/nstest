#ifndef _AS3CapsStyle_
#define _AS3CapsStyle_
namespace avmplus{namespace NativeID{
class CapsStyleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp NONE = "none";
	//Stringp ROUND = "round";
	//Stringp SQUARE = "square";

	Stringp NONE;
	Stringp ROUND;
	Stringp SQUARE;
private:};
class CapsStyleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

private:};
}}
namespace avmshell{
	class CapsStyleClass : public ClassClosure
	{
	public:
		CapsStyleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CapsStyleClassSlots m_slots_CapsStyleClass;
	public:
		inline Stringp getSlotNONE(){return m_slots_CapsStyleClass.NONE;}
		inline Stringp getSlotROUND(){return m_slots_CapsStyleClass.ROUND;}
		inline Stringp getSlotSQUARE(){return m_slots_CapsStyleClass.SQUARE;}
};
class CapsStyleObject : public ScriptObject
{
	public:
		CapsStyleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CapsStyleObjectSlots m_slots_CapsStyleObject;
};}
#endif
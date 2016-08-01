#ifndef _AS3SpreadMethod_
#define _AS3SpreadMethod_
namespace avmplus{namespace NativeID{
class SpreadMethodClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp PAD = "pad";
	//Stringp REFLECT = "reflect";
	//Stringp REPEAT = "repeat";

	Stringp PAD;
	Stringp REFLECT;
	Stringp REPEAT;
private:};
class SpreadMethodObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	
private:};
}}
namespace avmshell{
	class SpreadMethodClass : public ClassClosure
	{
	public:
		SpreadMethodClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SpreadMethodClassSlots m_slots_SpreadMethodClass;
	public:
		inline Stringp getSlotPAD(){return m_slots_SpreadMethodClass.PAD;}
		inline Stringp getSlotREFLECT(){return m_slots_SpreadMethodClass.REFLECT;}
		inline Stringp getSlotREPEAT(){return m_slots_SpreadMethodClass.REPEAT;}
};
class SpreadMethodObject : public ScriptObject
{
	public:
		SpreadMethodObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SpreadMethodObjectSlots m_slots_SpreadMethodObject;
};}
#endif
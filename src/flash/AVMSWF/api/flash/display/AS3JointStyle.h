#ifndef _AS3JointStyle_
#define _AS3JointStyle_
namespace avmplus{namespace NativeID{
class JointStyleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp BEVEL = "bevel";
	//Stringp MITER = "miter";
	//Stringp ROUND = "round";

	Stringp BEVEL;
	Stringp MITER;
	Stringp ROUND;
private:};
class JointStyleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

private:};
}}
namespace avmshell{
	class JointStyleClass : public ClassClosure
	{
	public:
		JointStyleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JointStyleClassSlots m_slots_JointStyleClass;
	public:
		inline Stringp getSlotBEVEL(){return m_slots_JointStyleClass.BEVEL;}
		inline Stringp getSlotMITER(){return m_slots_JointStyleClass.MITER;}
		inline Stringp getSlotROUND(){return m_slots_JointStyleClass.ROUND;}
};
class JointStyleObject : public ScriptObject
{
	public:
		JointStyleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JointStyleObjectSlots m_slots_JointStyleObject;
};}
#endif
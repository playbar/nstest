#ifndef _AS3SoftKeyboardTrigger_
#define _AS3SoftKeyboardTrigger_
namespace avmplus{namespace NativeID{
class SoftKeyboardTriggerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//CONTENT_TRIGGERED : String = "contentTriggered"
	//USER_TRIGGERED : String = "userTriggered"

	Stringp CONTENT_TRIGGERED;
	Stringp USER_TRIGGERED;

private:};
class SoftKeyboardTriggerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoftKeyboardTriggerClass : public ClassClosure
	{
	public:
		SoftKeyboardTriggerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoftKeyboardTriggerClassSlots m_slots_SoftKeyboardTriggerClass;
};
class SoftKeyboardTriggerObject : public ScriptObject
{
	public:
		SoftKeyboardTriggerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoftKeyboardTriggerObjectSlots m_slots_SoftKeyboardTriggerObject;
};}
#endif
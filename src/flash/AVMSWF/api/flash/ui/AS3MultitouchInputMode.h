#ifndef _AS3MultitouchInputMode_
#define _AS3MultitouchInputMode_
namespace avmplus{namespace NativeID{
class MultitouchInputModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//GESTURE : String = "gesture"
//NONE : String = "none"
//TOUCH_POINT : String = "touchPoint"

	Stringp GESTURE;
	Stringp NONE;
	Stringp TOUCH_POINT;

private:};
class MultitouchInputModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MultitouchInputModeClass : public ClassClosure
	{
	public:
		MultitouchInputModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MultitouchInputModeClassSlots m_slots_MultitouchInputModeClass;
};
class MultitouchInputModeObject : public ScriptObject
{
	public:
		MultitouchInputModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MultitouchInputModeObjectSlots m_slots_MultitouchInputModeObject;
};}
#endif
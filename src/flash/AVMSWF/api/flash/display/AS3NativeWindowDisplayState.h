#ifndef _AS3NativeWindowDisplayState_
#define _AS3NativeWindowDisplayState_
namespace avmplus{namespace NativeID{
class NativeWindowDisplayStateClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//MAXIMIZED : String = "maximized"
//MINIMIZED : String = "minimized"
//NORMAL : String = "normal"

	Stringp MAXIMIZED;
	Stringp MINIMIZED;
	Stringp NORMAL;

private:};
class NativeWindowDisplayStateObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowDisplayStateClass : public ClassClosure
	{
	public:
		NativeWindowDisplayStateClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowDisplayStateClassSlots m_slots_NativeWindowDisplayStateClass;
};
class NativeWindowDisplayStateObject : public ScriptObject
{
	public:
		NativeWindowDisplayStateObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowDisplayStateObjectSlots m_slots_NativeWindowDisplayStateObject;
};}
#endif
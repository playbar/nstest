#ifndef _AS3NativeWindowResize_
#define _AS3NativeWindowResize_
namespace avmplus{namespace NativeID{
class NativeWindowResizeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//BOTTOM : String = "B"
//BOTTOM_LEFT : String = "BL"
//BOTTOM_RIGHT : String = "BR"
//LEFT : String = "L"
//NONE : String
//RIGHT : String = "R"
//TOP : String = "T"
//TOP_LEFT : String = "TL"
//TOP_RIGHT : String = "TR"

	Stringp BOTTOM;
	Stringp BOTTOM_LEFT;
	Stringp BOTTOM_RIGHT;
	Stringp LEFT;
	Stringp NONE;
	Stringp RIGHT;
	Stringp TOP;
	Stringp TOP_LEFT;
	Stringp TOP_RIGHT;

private:};
class NativeWindowResizeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowResizeClass : public ClassClosure
	{
	public:
		NativeWindowResizeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowResizeClassSlots m_slots_NativeWindowResizeClass;
};
class NativeWindowResizeObject : public ScriptObject
{
	public:
		NativeWindowResizeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowResizeObjectSlots m_slots_NativeWindowResizeObject;
};}
#endif
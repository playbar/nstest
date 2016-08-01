#ifndef _AS3NativeWindowSystemChrome_
#define _AS3NativeWindowSystemChrome_
namespace avmplus{namespace NativeID{
class NativeWindowSystemChromeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ALTERNATE : String = "alternate"
//NONE : String = "none"
//STANDARD : String = "standard"

	Stringp ALTERNATE_C;
	Stringp NONE;
	Stringp STANDARD;

private:};
class NativeWindowSystemChromeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowSystemChromeClass : public ClassClosure
	{
	public:
		NativeWindowSystemChromeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowSystemChromeClassSlots m_slots_NativeWindowSystemChromeClass;
};
class NativeWindowSystemChromeObject : public ScriptObject
{
	public:
		NativeWindowSystemChromeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowSystemChromeObjectSlots m_slots_NativeWindowSystemChromeObject;
};}
#endif
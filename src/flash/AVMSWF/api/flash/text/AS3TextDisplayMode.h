#ifndef _AS3TextDisplayMode_
#define _AS3TextDisplayMode_
namespace avmplus{namespace NativeID{
class TextDisplayModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp CRT = "crt";
	//Stringp DEFAULT = "default"; 
	//Stringp LCD = "lcd";

	Stringp CRT;
	Stringp DEFAULT; 
	Stringp LCD;
private:};
class TextDisplayModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp CRT;
	Stringp DEFAULT; 
	Stringp LCD;
private:};
}}
namespace avmshell{
	class TextDisplayModeClass : public ClassClosure
	{
	public:
		TextDisplayModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextDisplayModeClassSlots m_slots_TextDisplayModeClass;
};
class TextDisplayModeObject : public ScriptObject
{
	public:
		TextDisplayModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextDisplayModeObjectSlots m_slots_TextDisplayModeObject;
};}
#endif
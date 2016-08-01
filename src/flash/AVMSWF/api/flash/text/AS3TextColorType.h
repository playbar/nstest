#ifndef _AS3TextColorType_
#define _AS3TextColorType_
namespace avmplus{namespace NativeID{
class TextColorTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp DARK_COLOR = "dark";
	//Stringp LIGHT_COLOR = "light"; 

	Stringp DARK_COLOR;
	Stringp LIGHT_COLOR; 
private:};
class TextColorTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp DARK_COLOR;
	Stringp LIGHT_COLOR; 
private:};
}}
namespace avmshell{
	class TextColorTypeClass : public ClassClosure
	{
	public:
		TextColorTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextColorTypeClassSlots m_slots_TextColorTypeClass;
};
class TextColorTypeObject : public ScriptObject
{
	public:
		TextColorTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextColorTypeObjectSlots m_slots_TextColorTypeObject;
};}
#endif
#ifndef _AS3TextFieldType_
#define _AS3TextFieldType_
namespace avmplus{namespace NativeID{
class TextFieldTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp DYNAMIC = "dynamic";
	//Stringp INPUT = "input";

	Stringp DYNAMIC;
	Stringp INPUT;
private:};
class TextFieldTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextFieldTypeClass : public ClassClosure
	{
	public:
		TextFieldTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFieldTypeClassSlots m_slots_TextFieldTypeClass;
	public:
		inline Stringp getSlotDYNAMIC(){return m_slots_TextFieldTypeClass.DYNAMIC;}
		inline Stringp getSlotINPUT(){return m_slots_TextFieldTypeClass.INPUT;}
};
class TextFieldTypeObject : public ScriptObject
{
	public:
		TextFieldTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFieldTypeObjectSlots m_slots_TextFieldTypeObject;
};}
#endif
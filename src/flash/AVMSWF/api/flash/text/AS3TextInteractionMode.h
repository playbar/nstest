#ifndef _AS3TextInteractionMode_
#define _AS3TextInteractionMode_
namespace avmplus{namespace NativeID{
class TextInteractionModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//NORMAL : String = "normal"
//SELECTION : String = "selection"

	Stringp NORMAL;
	Stringp SELECTION;

private:};
class TextInteractionModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextInteractionModeClass : public ClassClosure
	{
	public:
		TextInteractionModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextInteractionModeClassSlots m_slots_TextInteractionModeClass;
};
class TextInteractionModeObject : public ScriptObject
{
	public:
		TextInteractionModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextInteractionModeObjectSlots m_slots_TextInteractionModeObject;
};}
#endif
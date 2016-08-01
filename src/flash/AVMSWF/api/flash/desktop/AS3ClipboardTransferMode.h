#ifndef _AS3ClipboardTransferMode_
#define _AS3ClipboardTransferMode_
namespace avmplus{namespace NativeID{
class ClipboardTransferModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CLONE_ONLY : Stringp = "cloneOnly"
//CLONE_PREFERRED : Stringp = "clonePreferred"
//ORIGINAL_ONLY : Stringp = "originalOnly"
//ORIGINAL_PREFERRED : Stringp = "originalPreferred"

	Stringp CLONE_ONLY;
	Stringp CLONE_PREFERRED;
	Stringp ORIGINAL_ONLY;
	Stringp ORIGINAL_PREFERRED;

private:};
class ClipboardTransferModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ClipboardTransferModeClass : public ClassClosure
	{
	public:
		ClipboardTransferModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClipboardTransferModeClassSlots m_slots_ClipboardTransferModeClass;
};
class ClipboardTransferModeObject : public ScriptObject
{
	public:
		ClipboardTransferModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClipboardTransferModeObjectSlots m_slots_ClipboardTransferModeObject;
};}
#endif
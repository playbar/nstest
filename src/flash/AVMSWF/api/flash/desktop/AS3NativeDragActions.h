#ifndef _AS3NativeDragActions_
#define _AS3NativeDragActions_
namespace avmplus{namespace NativeID{
class NativeDragActionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//COPY : String = "copy"
//LINK : String = "link"
//MOVE : String = "move"
//NONE : String = "none"

	Stringp COPY;
	Stringp LINK;
	Stringp MOVE_C;
	Stringp NONE;

private:};
class NativeDragActionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeDragActionsClass : public ClassClosure
	{
	public:
		NativeDragActionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragActionsClassSlots m_slots_NativeDragActionsClass;
};
class NativeDragActionsObject : public ScriptObject
{
	public:
		NativeDragActionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragActionsObjectSlots m_slots_NativeDragActionsObject;
};}
#endif
#ifndef _AS3MouseCursor_
#define _AS3MouseCursor_
namespace avmplus{namespace NativeID{
class MouseCursorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ARROW : Stringp = "arrow"
//AUTO : Stringp = "auto"
//BUTTON : Stringp = "button"
//HAND : Stringp = "hand"
//IBEAM : Stringp = "ibeam"

	Stringp ARROW;
	Stringp AUTO;
	Stringp BUTTON;
	Stringp HAND;
	Stringp IBEAM;

private:};
class MouseCursorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MouseCursorClass : public ClassClosure
	{
	public:
		MouseCursorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getSlotARROW()
		{
			return m_slots_MouseCursorClass.ARROW;
		}
		Stringp getSlotAUTO()
		{
			return m_slots_MouseCursorClass.AUTO;
		}
		Stringp getSlotBUTTON()
		{
			return m_slots_MouseCursorClass.BUTTON;
		}
		Stringp getSlotHAND()
		{
			return m_slots_MouseCursorClass.HAND;
		}
		Stringp getSlotIBEAM()
		{
			return m_slots_MouseCursorClass.IBEAM;
		}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseCursorClassSlots m_slots_MouseCursorClass;
};
class MouseCursorObject : public ScriptObject
{
	public:
		MouseCursorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseCursorObjectSlots m_slots_MouseCursorObject;
};}
#endif
#ifndef _AS3TabAlignment_
#define _AS3TabAlignment_
namespace avmplus{namespace NativeID{
class TabAlignmentClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CENTER : String = "center"
//DECIMAL : String = "decimal"
//END : String = "end"
//START : String = "start"

	Stringp CENTER;
	Stringp DECIMAL;
	Stringp END;
	Stringp START;

private:};
class TabAlignmentObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TabAlignmentClass : public ClassClosure
	{
	public:
		TabAlignmentClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TabAlignmentClassSlots m_slots_TabAlignmentClass;
};
class TabAlignmentObject : public ScriptObject
{
	public:
		TabAlignmentObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TabAlignmentObjectSlots m_slots_TabAlignmentObject;
};}
#endif
#ifndef _AS3LineJustification_
#define _AS3LineJustification_
namespace avmplus{namespace NativeID{
class LineJustificationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//public static const ALL_BUT_LAST : String = "allButLast"
	//public static const ALL_INCLUDING_LAST : String = "allIncludingLast"
	//public static const UNJUSTIFIED : String = "unjustified"

	Stringp ALL_BUT_LAST;
	Stringp ALL_INCLUDING_LAST;
	Stringp UNJUSTIFIED;
	Stringp	ALL_BUT_MANDATORY_BREAK;


private:};
class LineJustificationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LineJustificationClass : public ClassClosure
	{
	public:
		LineJustificationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LineJustificationClassSlots m_slots_LineJustificationClass;
};
class LineJustificationObject : public ScriptObject
{
	public:
		LineJustificationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LineJustificationObjectSlots m_slots_LineJustificationObject;
};}
#endif
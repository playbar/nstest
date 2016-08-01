#ifndef _AS3TextRun_
#define _AS3TextRun_

namespace avmplus{namespace NativeID{
class TextRunClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextRunObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	int		beginIndex;
	int		endIndex;
	ScriptObject *textFormat;
private:};
}}
namespace avmshell{
	class TextRunClass : public ClassClosure
	{
	public:
		TextRunClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextRunClassSlots m_slots_TextRunClass;
};
class TextRunObject : public ScriptObject
{
	public:
		TextRunObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextRunObjectSlots m_slots_TextRunObject;
};}
#endif
#ifndef _AS3FontPosture_
#define _AS3FontPosture_
namespace avmplus{namespace NativeID{
class FontPostureClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ITALIC : String = "italic"
//NORMAL : String = "normal"

	Stringp ITALIC;
	Stringp NORMAL;

private:};
class FontPostureObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FontPostureClass : public ClassClosure
	{
	public:
		FontPostureClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getITALIC(){return m_slots_FontPostureClass.ITALIC;}
		Stringp getNORMAL(){return m_slots_FontPostureClass.NORMAL;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontPostureClassSlots m_slots_FontPostureClass;
};
class FontPostureObject : public ScriptObject
{
	public:
		FontPostureObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontPostureObjectSlots m_slots_FontPostureObject;
};}
#endif
#ifndef _AS3DateTimeNameContext_
#define _AS3DateTimeNameContext_
namespace avmplus{namespace NativeID{
class DateTimeNameContextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//FORMAT : String = "format"
//STANDALONE : String = "standalone"

	Stringp FORMAT;
	Stringp STANDALONE;

private:};
class DateTimeNameContextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DateTimeNameContextClass : public ClassClosure
	{
	public:
		DateTimeNameContextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeNameContextClassSlots m_slots_DateTimeNameContextClass;
};
class DateTimeNameContextObject : public ScriptObject
{
	public:
		DateTimeNameContextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeNameContextObjectSlots m_slots_DateTimeNameContextObject;
};}
#endif
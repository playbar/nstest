#ifndef _AS3StringTools_
#define _AS3StringTools_
namespace avmplus{namespace NativeID{
class StringToolsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StringToolsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StringToolsClass : public ClassClosure
	{
	public:
		StringToolsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ObjectVectorObject* AS3_getAvailableLocaleIDNames();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StringToolsClassSlots m_slots_StringToolsClass;
};
class StringToolsObject : public ScriptObject
{
	public:
		StringToolsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_actualLocaleIDName_get();
		Stringp AS3_lastOperationStatus_get();
		Stringp AS3_requestedLocaleIDName_get();

		void AS3_constructor(Stringp requestedLocaleIDName);
		Stringp AS3_toLowerCase(Stringp s);
		Stringp AS3_toUpperCase(Stringp s);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StringToolsObjectSlots m_slots_StringToolsObject;
};}
#endif
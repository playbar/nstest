#ifndef _AS3LocaleID_
#define _AS3LocaleID_
namespace avmplus{namespace NativeID{
class LocaleIDClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DEFAULT : String = "i-default"

	Stringp DEFAULT;

private:};
class LocaleIDObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LocaleIDClass : public ClassClosure
	{
	public:
		LocaleIDClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ObjectVectorObject* AS3_determinePreferredLocales(ObjectVectorObject *pWant, ObjectVectorObject *pHave, Stringp keyword);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LocaleIDClassSlots m_slots_LocaleIDClass;
};
class LocaleIDObject : public ScriptObject
{
	public:
		LocaleIDObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_lastOperationStatus_get();
		Stringp AS3_name_get();

		void AS3_constructor(Stringp name);
		ScriptObject* AS3_getKeysAndValues();
		Stringp AS3_getLanguage();
		Stringp AS3_getRegion();
		Stringp AS3_getScript();
		Stringp AS3_getVariant();
		bool AS3_isRightToLeft();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LocaleIDObjectSlots m_slots_LocaleIDObject;
};}
#endif
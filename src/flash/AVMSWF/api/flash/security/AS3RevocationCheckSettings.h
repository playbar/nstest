#ifndef _AS3RevocationCheckSettings_
#define _AS3RevocationCheckSettings_
namespace avmplus{namespace NativeID{
class RevocationCheckSettingsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ALWAYS_REQUIRED : String = "alwaysRequired"
//BEST_EFFORT : String = "bestEffort"
//NEVER : String = "never"
//REQUIRED_IF_AVAILABLE : String = "requiredIfInfoAvailable"

	Stringp ALWAYS_REQUIRED;
	Stringp BEST_EFFORT;
	Stringp NEVER;
	Stringp REQUIRED_IF_AVAILABLE;

private:};
class RevocationCheckSettingsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class RevocationCheckSettingsClass : public ClassClosure
	{
	public:
		RevocationCheckSettingsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RevocationCheckSettingsClassSlots m_slots_RevocationCheckSettingsClass;
};
class RevocationCheckSettingsObject : public ScriptObject
{
	public:
		RevocationCheckSettingsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RevocationCheckSettingsObjectSlots m_slots_RevocationCheckSettingsObject;
};}
#endif
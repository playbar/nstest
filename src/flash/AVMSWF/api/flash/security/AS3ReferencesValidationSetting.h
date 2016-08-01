#ifndef _AS3ReferencesValidationSetting_
#define _AS3ReferencesValidationSetting_
namespace avmplus{namespace NativeID{
class ReferencesValidationSettingClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//NEVER : String = "never"
//VALID_IDENTITY : String = "validIdentity"
//VALID_OR_UNKNOWN_IDENTITY : String = "validOrUnknownIdentity"

	Stringp NEVER;
	Stringp VALID_IDENTITY;
	Stringp VALID_OR_UNKNOWN_IDENTITY;

private:};
class ReferencesValidationSettingObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ReferencesValidationSettingClass : public ClassClosure
	{
	public:
		ReferencesValidationSettingClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ReferencesValidationSettingClassSlots m_slots_ReferencesValidationSettingClass;
};
class ReferencesValidationSettingObject : public ScriptObject
{
	public:
		ReferencesValidationSettingObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ReferencesValidationSettingObjectSlots m_slots_ReferencesValidationSettingObject;
};}
#endif
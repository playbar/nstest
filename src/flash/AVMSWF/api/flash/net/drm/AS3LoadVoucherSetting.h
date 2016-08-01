#ifndef _AS3LoadVoucherSetting_
#define _AS3LoadVoucherSetting_
namespace avmplus{namespace NativeID{
class LoadVoucherSettingClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ALLOW_SERVER : String = "allowServer"
//FORCE_REFRESH : String = "forceRefresh"
//LOCAL_ONLY : String = "localOnly"

	Stringp ALLOW_SERVER;
	Stringp FORCE_REFRESH;
	Stringp LOCAL_ONLY;

private:};
class LoadVoucherSettingObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LoadVoucherSettingClass : public ClassClosure
	{
	public:
		LoadVoucherSettingClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoadVoucherSettingClassSlots m_slots_LoadVoucherSettingClass;
};
class LoadVoucherSettingObject : public ScriptObject
{
	public:
		LoadVoucherSettingObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoadVoucherSettingObjectSlots m_slots_LoadVoucherSettingObject;
};}
#endif
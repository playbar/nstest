#ifndef _AS3SystemUpdaterType_
#define _AS3SystemUpdaterType_
namespace avmplus{namespace NativeID{
class SystemUpdaterTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DRM : String = "drm"
	//SYSTEM : String = "system"

	Stringp DRM;
	Stringp SYSTEM;

private:};
class SystemUpdaterTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SystemUpdaterTypeClass : public ClassClosure
	{
	public:
		SystemUpdaterTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemUpdaterTypeClassSlots m_slots_SystemUpdaterTypeClass;
};
class SystemUpdaterTypeObject : public ScriptObject
{
	public:
		SystemUpdaterTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemUpdaterTypeObjectSlots m_slots_SystemUpdaterTypeObject;
};}
#endif
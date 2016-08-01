#ifndef _AS3SystemIdleMode_
#define _AS3SystemIdleMode_
namespace avmplus{namespace NativeID{
class SystemIdleModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//KEEP_AWAKE : String = "keepAwake"
//NORMAL : String = "normal"

	Stringp KEEP_AWAKE;
	Stringp NORMAL;

private:};
class SystemIdleModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SystemIdleModeClass : public ClassClosure
	{
	public:
		SystemIdleModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemIdleModeClassSlots m_slots_SystemIdleModeClass;
};
class SystemIdleModeObject : public ScriptObject
{
	public:
		SystemIdleModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemIdleModeObjectSlots m_slots_SystemIdleModeObject;
};}
#endif
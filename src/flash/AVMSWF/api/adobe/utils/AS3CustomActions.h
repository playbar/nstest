#ifndef _AS3CustomActions_
#define _AS3CustomActions_
namespace avmplus{namespace NativeID{
class CustomActionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CustomActionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CustomActionsClass : public ClassClosure
	{
	public:
		CustomActionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ArrayObject* AS3_actionsList_get();
		Stringp AS3_getActions(Stringp name);
		void AS3_installActions(Stringp name, Stringp data);
		void AS3_uninstallActions(Stringp name);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CustomActionsClassSlots m_slots_CustomActionsClass;
};
class CustomActionsObject : public ScriptObject
{
	public:
		CustomActionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CustomActionsObjectSlots m_slots_CustomActionsObject;
};}
#endif
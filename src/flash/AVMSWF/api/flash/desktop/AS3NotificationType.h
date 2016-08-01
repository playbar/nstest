#ifndef _AS3NotificationType_
#define _AS3NotificationType_
namespace avmplus{namespace NativeID{
class NotificationTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CRITICAL : String = "critical"
//INFORMATIONAL : String = "informational"

	Stringp CRITICAL;
	Stringp INFORMATIONAL;

private:};
class NotificationTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NotificationTypeClass : public ClassClosure
	{
	public:
		NotificationTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NotificationTypeClassSlots m_slots_NotificationTypeClass;
};
class NotificationTypeObject : public ScriptObject
{
	public:
		NotificationTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NotificationTypeObjectSlots m_slots_NotificationTypeObject;
};}
#endif
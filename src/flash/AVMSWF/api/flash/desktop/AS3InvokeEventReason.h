#ifndef _AS3InvokeEventReason_
#define _AS3InvokeEventReason_
namespace avmplus{namespace NativeID{
class InvokeEventReasonClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//LOGIN : String = "login"
//STANDARD : String = "standard"
//NOTIFICATION:String = "notification"
//OPEN_URL:String = "openUrl"

	Stringp LOGIN;
	Stringp STANDARD;

	//new adds 11
	Stringp NOTIFICATION;
	Stringp OPEN_URL;
	//new adds 11 end

private:};
class InvokeEventReasonObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class InvokeEventReasonClass : public ClassClosure
	{
	public:
		InvokeEventReasonClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InvokeEventReasonClassSlots m_slots_InvokeEventReasonClass;
};
class InvokeEventReasonObject : public ScriptObject
{
	public:
		InvokeEventReasonObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InvokeEventReasonObjectSlots m_slots_InvokeEventReasonObject;
};}
#endif
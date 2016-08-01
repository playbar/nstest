#ifndef _AS3NetGroupSendResult_
#define _AS3NetGroupSendResult_
namespace avmplus{namespace NativeID{
class NetGroupSendResultClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ERROR : String = "error"
//NO_ROUTE : String = "no route"
//SENT : String = "sent"

	Stringp ERROR_C;
	Stringp NO_ROUTE;
	Stringp SENT;

private:};
class NetGroupSendResultObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetGroupSendResultClass : public ClassClosure
	{
	public:
		NetGroupSendResultClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupSendResultClassSlots m_slots_NetGroupSendResultClass;
};
class NetGroupSendResultObject : public ScriptObject
{
	public:
		NetGroupSendResultObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupSendResultObjectSlots m_slots_NetGroupSendResultObject;
};}
#endif
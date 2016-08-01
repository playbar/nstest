#ifndef _AS3NetGroupInfo_
#define _AS3NetGroupInfo_
namespace avmplus{namespace NativeID{
class NetGroupInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetGroupInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
double    m_postingSendDataBytesPerSecond;
double    m_postingSendControlBytesPerSecond;
double    m_postingReceiveDataBytesPerSecond;
double    m_postingReceiveControlBytesPerSecond;
double    m_routingSendBytesPerSecond;
double    m_routingReceiveBytesPerSecond;
double    m_objectReplicationSendBytesPerSecond;
double    m_objectReplicationReceiveBytesPerSecond;
private:};
}}
namespace avmshell{
	class NetGroupInfoClass : public ClassClosure
	{
	public:
		NetGroupInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupInfoClassSlots m_slots_NetGroupInfoClass;
};
class NetGroupInfoObject : public ScriptObject
{
	public:
		NetGroupInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_objectReplicationReceiveBytesPerSecond_get();
		//double AS3_objectReplicationSendBytesPerSecond_get();
		//double AS3_postingReceiveControlBytesPerSecond_get();
		//double AS3_postingReceiveDataBytesPerSecond_get();
		//double AS3_postingSendControlBytesPerSecond_get();
		//double AS3_postingSendDataBytesPerSecond_get();
		//double AS3_routingReceiveBytesPerSecond_get();
		//double AS3_routingSendBytesPerSecond_get();

		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupInfoObjectSlots m_slots_NetGroupInfoObject;
};}
#endif
#ifndef _AS3NetStreamMulticastInfo_
#define _AS3NetStreamMulticastInfo_
namespace avmplus{namespace NativeID{
class NetStreamMulticastInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetStreamMulticastInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double m_sendDataBytesPerSecond;
	double m_sendControlBytesPerSecond;
	double m_receiveDataBytesPerSecond;
	double m_receiveControlBytesPerSecond;
	double m_bytesPushedToPeers;
	double m_fragmentsPushedToPeers;
	double m_bytesRequestedByPeers;
	double m_fragmentsRequestedByPeers;
	double m_bytesPushedFromPeers;
	double m_fragmentsPushedFromPeers;
	double m_bytesRequestedFromPeers;
	double m_fragmentsRequestedFromPeers;
	double m_sendControlBytesPerSecondToServer;
	double m_receiveDataBytesPerSecondFromServer;
	double m_bytesReceivedFromServer;
	double m_fragmentsReceivedFromServer;
	double m_receiveDataBytesPerSecondFromIPMulticast;
	double m_bytesReceivedFromIPMulticast;
	double m_fragmentsReceivedFromIPMulticast;
private:};
}}
namespace avmshell{
	class NetStreamMulticastInfoClass : public ClassClosure
	{
	public:
		NetStreamMulticastInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamMulticastInfoClassSlots m_slots_NetStreamMulticastInfoClass;
};
class NetStreamMulticastInfoObject : public ScriptObject
{
	public:
		NetStreamMulticastInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_bytesPushedFromPeers_get();
		//double AS3_bytesPushedToPeers_get();
		//double AS3_bytesReceivedFromIPMulticast_get();
		//double AS3_bytesReceivedFromServer_get();
		//double AS3_bytesRequestedByPeers_get();
		//double AS3_bytesRequestedFromPeers_get();
		//double AS3_fragmentsPushedFromPeers_get();
		//double AS3_fragmentsPushedToPeers_get();
		//double AS3_fragmentsReceivedFromIPMulticast_get();
		//double AS3_fragmentsReceivedFromServer_get();
		//double AS3_fragmentsRequestedByPeers_get();
		//double AS3_fragmentsRequestedFromPeers_get();
		//double AS3_receiveControlBytesPerSecond_get();
		//double AS3_receiveDataBytesPerSecond_get();
		//double AS3_receiveDataBytesPerSecondFromIPMulticast_get();
		//double AS3_receiveDataBytesPerSecondFromServer_get();
		//double AS3_sendControlBytesPerSecond_get();
		//double AS3_sendControlBytesPerSecondToServer_get();
		//double AS3_sendDataBytesPerSecond_get();

		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamMulticastInfoObjectSlots m_slots_NetStreamMulticastInfoObject;
};}
#endif
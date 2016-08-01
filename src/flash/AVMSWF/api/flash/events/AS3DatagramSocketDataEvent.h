#ifndef _AS3DatagramSocketDataEvent_
#define _AS3DatagramSocketDataEvent_

#include "AS3Event.h"
#include "AS3ByteArray.h"

namespace avmplus{namespace NativeID{
class DatagramSocketDataEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DATA : String = "data"

	Stringp DATA;

private:};
class DatagramSocketDataEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DatagramSocketDataEventClass : public ClassClosure//EventClass
	{
	public:
		DatagramSocketDataEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DatagramSocketDataEventClassSlots m_slots_DatagramSocketDataEventClass;
};
class DatagramSocketDataEventObject : public EventObject
{
	public:
		DatagramSocketDataEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		ByteArrayObject* AS3_data_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_data_set(ByteArrayObject *pData);
		Stringp AS3_dstAddress_get();
		void AS3_dstAddress_set(Stringp pDstAddress);
		int AS3_dstPort_get();
		void AS3_dstPort_set(int dstPort);
		Stringp AS3_srcAddress_get();
		void AS3_srcAddress_set(Stringp pSrcAddress);
		int AS3_srcPort_get();
		void AS3_srcPort_set(int srcPort);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
								Stringp pSrcAddress, int srcPort, 
								String pDstAddress, int dstPort, ByteArrayObject *pAata);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DatagramSocketDataEventObjectSlots m_slots_DatagramSocketDataEventObject;
};}
#endif
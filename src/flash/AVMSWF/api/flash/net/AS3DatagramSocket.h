#ifndef _AS3DatagramSocket_
#define _AS3DatagramSocket_

#include "AS3EventDispatcher.h"

#include "AS3ByteArray.h"

namespace avmplus{namespace NativeID{
class DatagramSocketClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DatagramSocketObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DatagramSocketClass : public ClassClosure
	{
	public:
		DatagramSocketClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DatagramSocketClassSlots m_slots_DatagramSocketClass;
};
class DatagramSocketObject : public EventDispatcherObject
{
	public:
		DatagramSocketObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_isSupported_get();
		bool AS3_bound_get();
		bool AS3_connected_get();
		Stringp AS3_localAddress_get();
		int AS3_localPort_get();
		Stringp AS3_remoteAddress_get();
		int AS3_remotePort_get();

		void AS3_constructor();
		void AS3_bind(int localPort, Stringp pLocalAddress);
		void AS3_close();
		void AS3_connect(Stringp pRemoteAddress, int remotePort);
		void AS3_receive();
		void AS3_send(ByteArrayObject *pBytes, uint32_t offset, uint32_t length, Stringp address, int port);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DatagramSocketObjectSlots m_slots_DatagramSocketObject;
};}
#endif
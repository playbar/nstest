#ifndef _AS3ServerSocketConnectEvent_
#define _AS3ServerSocketConnectEvent_

#include "AS3Event.h"
#include "AS3Socket.h"

namespace avmplus{namespace NativeID{
class ServerSocketConnectEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//CONNECT : String = "connect"

	Stringp CONNECT;

private:};
class ServerSocketConnectEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ServerSocketConnectEventClass : public ClassClosure//EventClass
	{
	public:
		ServerSocketConnectEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ServerSocketConnectEventClassSlots m_slots_ServerSocketConnectEventClass;
};
class ServerSocketConnectEventObject : public EventObject
{
	public:
		ServerSocketConnectEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		SocketObject* AS3_socket_get();
		void AS3_socket_set(SocketObject *pSocket);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, SocketObject *pSocket);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ServerSocketConnectEventObjectSlots m_slots_ServerSocketConnectEventObject;
};}
#endif
#ifndef _AS3NetMonitorEvent_
#define _AS3NetMonitorEvent_

#include "AS3Event.h"
#include "AS3NetStream.h"

namespace avmplus{namespace NativeID{

class NetStreamObject;

class NetMonitorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//NET_STREAM_CREATE : String = "netStreamCreate"

	Stringp NET_STREAM_CREATE;

private:};
class NetMonitorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	NetStreamObject *m_netStream;
private:};
}}
namespace avmshell{
	class NetMonitorEventClass : public ClassClosure//EventClass
	{
	public:
		NetMonitorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetMonitorEventClassSlots m_slots_NetMonitorEventClass;
};
class NetMonitorEventObject : public EventObject
{
	public:
		NetMonitorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		////void* pData;//Set your data!!
		////NetStreamObject* AS3_netStream_get();

		////void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, NetStreamObject *pNetStream);
		////EventObject* AS3_clone();
		////Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetMonitorEventObjectSlots m_slots_NetMonitorEventObject;
};}
#endif
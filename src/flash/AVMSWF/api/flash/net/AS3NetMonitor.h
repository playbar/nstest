#ifndef _AS3NetMonitor_
#define _AS3NetMonitor_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class NetMonitorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetMonitorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetMonitorClass : public ClassClosure
	{
	public:
		NetMonitorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetMonitorClassSlots m_slots_NetMonitorClass;
};
class NetMonitorObject : public EventDispatcherObject
{
	public:
		NetMonitorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		ObjectVectorObject* AS3_listStreams();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetMonitorObjectSlots m_slots_NetMonitorObject;
};}
#endif
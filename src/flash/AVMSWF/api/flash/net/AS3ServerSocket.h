#ifndef _AS3ServerSocket_
#define _AS3ServerSocket_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class ServerSocketClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ServerSocketObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ServerSocketClass : public ClassClosure
	{
	public:
		ServerSocketClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ServerSocketClassSlots m_slots_ServerSocketClass;
};
class ServerSocketObject : public EventDispatcherObject
{
	public:
		ServerSocketObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_isSupported_get();
		bool AS3_bound_get();
		bool AS3_listening_get();
		Stringp AS3_localAddress_get();
		int AS3_localPort_get();

		void AS3_constructor();
		void AS3_bind(int localPort, Stringp localAddress);
		void AS3_close();
		void AS3_listen(int backlog);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ServerSocketObjectSlots m_slots_ServerSocketObject;
};}
#endif
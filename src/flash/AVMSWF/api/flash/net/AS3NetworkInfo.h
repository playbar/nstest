#ifndef _AS3NetworkInfo_
#define _AS3NetworkInfo_

#include "AS3EventDispatcher.h"

#include "AS3NetworkInfo.h"

namespace avmplus{namespace NativeID{
class NetworkInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetworkInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetworkInfoClass : public ClassClosure
	{
	public:
		NetworkInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetworkInfoClassSlots m_slots_NetworkInfoClass;
};
class NetworkInfoObject : public EventDispatcherObject
{
	public:
		NetworkInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_isSupported_get();
		NetworkInfoObject* AS3_networkInfo_get();
		AvmBox AS3_findInterfaces(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetworkInfoObjectSlots m_slots_NetworkInfoObject;
};}
#endif
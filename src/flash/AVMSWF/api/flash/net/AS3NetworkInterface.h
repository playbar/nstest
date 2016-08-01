#ifndef _AS3NetworkInterface_
#define _AS3NetworkInterface_
namespace avmplus{namespace NativeID{
class NetworkInterfaceClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetworkInterfaceObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetworkInterfaceClass : public ClassClosure
	{
	public:
		NetworkInterfaceClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetworkInterfaceClassSlots m_slots_NetworkInterfaceClass;
};
class NetworkInterfaceObject : public ScriptObject
{
	public:
		NetworkInterfaceObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_active_get();
		void AS3_active_set();
		AvmBox AS3_addresses_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_addresses_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_displayName_get();
		void AS3_displayName_set(Stringp pDisplayName);
		Stringp AS3_hardwareAddress_get();
		void AS3_hardwareAddress_set(Stringp pHardwareAddress);
		int AS3_mtu_get();
		void AS3_mtu_set(int mtu);
		Stringp AS3_name_get();
		void AS3_name_set(Stringp pName);
		NetworkInterfaceObject* AS3_parent_get();
		void AS3_parent_set(NetworkInterfaceObject *pParent);
		AvmBox AS3_subInterfaces_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_subInterfaces_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetworkInterfaceObjectSlots m_slots_NetworkInterfaceObject;
};}
#endif
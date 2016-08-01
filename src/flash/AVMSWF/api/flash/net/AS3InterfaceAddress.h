#ifndef _AS3InterfaceAddress_
#define _AS3InterfaceAddress_
namespace avmplus{namespace NativeID{
class InterfaceAddressClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class InterfaceAddressObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class InterfaceAddressClass : public ClassClosure
	{
	public:
		InterfaceAddressClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InterfaceAddressClassSlots m_slots_InterfaceAddressClass;
};
class InterfaceAddressObject : public ScriptObject
{
	public:
		InterfaceAddressObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Stringp AS3_address_get();
		void AS3_address_set(Stringp pAddress);
		Stringp AS3_broadcast_get();
		void AS3_broadcast_set(Stringp pBroadCast);
		Stringp AS3_ipVersion_get();
		void AS3_ipVersion_set(Stringp pIpVersion);
		int AS3_prefixLength_get();
		void AS3_prefixLength_set(int prefixLength);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InterfaceAddressObjectSlots m_slots_InterfaceAddressObject;
};}
#endif
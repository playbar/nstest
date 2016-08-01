#ifndef _AS3IPVersion_
#define _AS3IPVersion_
namespace avmplus{namespace NativeID{
class IPVersionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//IPV4 : String = "IPv4"
    //IPV6 : String = "IPv6"

	Stringp IPV4;
	Stringp IPV6;

private:};
class IPVersionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IPVersionClass : public ClassClosure
	{
	public:
		IPVersionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IPVersionClassSlots m_slots_IPVersionClass;
};
class IPVersionObject : public ScriptObject
{
	public:
		IPVersionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IPVersionObjectSlots m_slots_IPVersionObject;
};}
#endif
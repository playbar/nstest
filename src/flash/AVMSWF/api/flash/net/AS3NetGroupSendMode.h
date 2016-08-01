#ifndef _AS3NetGroupSendMode_
#define _AS3NetGroupSendMode_
namespace avmplus{namespace NativeID{
class NetGroupSendModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//NEXT_DECREASING : String = "nextDecreasing"
//NEXT_INCREASING : String = "nextIncreasing"

	Stringp NEXT_DECREASING;
	Stringp NEXT_INCREASING;

private:};
class NetGroupSendModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetGroupSendModeClass : public ClassClosure
	{
	public:
		NetGroupSendModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupSendModeClassSlots m_slots_NetGroupSendModeClass;
};
class NetGroupSendModeObject : public ScriptObject
{
	public:
		NetGroupSendModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupSendModeObjectSlots m_slots_NetGroupSendModeObject;
};}
#endif
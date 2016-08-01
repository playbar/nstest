#ifndef _AS3NetGroupReceiveMode_
#define _AS3NetGroupReceiveMode_
namespace avmplus{namespace NativeID{
class NetGroupReceiveModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//EXACT : String = "exact"
//NEAREST : String = "nearest"

	Stringp EXACT;
	Stringp NEAREST;

private:};
class NetGroupReceiveModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetGroupReceiveModeClass : public ClassClosure
	{
	public:
		NetGroupReceiveModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupReceiveModeClassSlots m_slots_NetGroupReceiveModeClass;
};
class NetGroupReceiveModeObject : public ScriptObject
{
	public:
		NetGroupReceiveModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupReceiveModeObjectSlots m_slots_NetGroupReceiveModeObject;
};}
#endif
#ifndef _AS3NetGroupReplicationStrategy_
#define _AS3NetGroupReplicationStrategy_
namespace avmplus{namespace NativeID{
class NetGroupReplicationStrategyClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//LOWEST_FIRST : String = "lowestFirst"
//RAREST_FIRST : String = "rarestFirst"

	Stringp LOWEST_FIRST;
	Stringp RAREST_FIRST;

private:};
class NetGroupReplicationStrategyObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetGroupReplicationStrategyClass : public ClassClosure
	{
	public:
		NetGroupReplicationStrategyClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupReplicationStrategyClassSlots m_slots_NetGroupReplicationStrategyClass;
};
class NetGroupReplicationStrategyObject : public ScriptObject
{
	public:
		NetGroupReplicationStrategyObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupReplicationStrategyObjectSlots m_slots_NetGroupReplicationStrategyObject;
};}
#endif
#ifndef _AS3NetStreamPlayTransitions_
#define _AS3NetStreamPlayTransitions_
namespace avmplus{namespace NativeID{
class NetStreamPlayTransitionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//APPEND : Stringp = "append"
//APPEND_AND_WAIT : Stringp = "appendAndWait"
//RESET : Stringp = "reset"
//RESUME : Stringp = "resume"
//STOP : Stringp = "stop"
//SWAP : Stringp = "swap"
//SWITCH : Stringp = "switch"

	Stringp APPEND;
	Stringp APPEND_AND_WAIT;
	Stringp RESET;
	Stringp RESUME;
	Stringp STOP;
	Stringp SWAP;
	Stringp SWITCH;

private:};
class NetStreamPlayTransitionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetStreamPlayTransitionsClass : public ClassClosure
	{
	public:
		NetStreamPlayTransitionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamPlayTransitionsClassSlots m_slots_NetStreamPlayTransitionsClass;
};
class NetStreamPlayTransitionsObject : public ScriptObject
{
	public:
		NetStreamPlayTransitionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamPlayTransitionsObjectSlots m_slots_NetStreamPlayTransitionsObject;
};}
#endif
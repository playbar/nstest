#ifndef _AS3SharedObjectFlushStatus_
#define _AS3SharedObjectFlushStatus_
namespace avmplus{namespace NativeID{
class SharedObjectFlushStatusClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp FLUSHED = "flushed";
	//Stringp PENDING = "pending";

	Stringp FLUSHED;
	Stringp PENDING;
private:};
class SharedObjectFlushStatusObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SharedObjectFlushStatusObject;

	class SharedObjectFlushStatusClass : public ClassClosure
	{
	public:
		SharedObjectFlushStatusObject* CreateSharedObjectFlushStatus();

	public:
		SharedObjectFlushStatusClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SharedObjectFlushStatusClassSlots m_slots_SharedObjectFlushStatusClass;

	public:
		Stringp IDToString(int id){return NULL;}

	public:
		inline Stringp getSlotFLUSHED(){return m_slots_SharedObjectFlushStatusClass.FLUSHED;}
		inline Stringp getSlotPENDING(){return m_slots_SharedObjectFlushStatusClass.PENDING;}
};
class SharedObjectFlushStatusObject : public ScriptObject
{
	public:
		SharedObjectFlushStatusObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
//		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SharedObjectFlushStatusObjectSlots m_slots_SharedObjectFlushStatusObject;
};}
#endif
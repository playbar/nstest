#ifndef _AS3EventPhase_
#define _AS3EventPhase_
namespace avmplus{namespace NativeID{
class EventPhaseClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//uint32_t AT_TARGET = 2;
	//uint32_t BUBBLING_PHASE = 3; 
	//uint32_t CAPTURING_PHASE = 1; 

	uint32_t AT_TARGET;
	uint32_t BUBBLING_PHASE; 
	uint32_t CAPTURING_PHASE;
private:};
class EventPhaseObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	uint32_t AT_TARGET;
	uint32_t BUBBLING_PHASE; 
	uint32_t CAPTURING_PHASE;
private:};
}}
namespace avmshell{
	class EventPhaseClass : public ClassClosure
	{
	public:
		EventPhaseClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EventPhaseClassSlots m_slots_EventPhaseClass;
};
class EventPhaseObject : public ScriptObject
{
	public:
		EventPhaseObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EventPhaseObjectSlots m_slots_EventPhaseObject;
};}
#endif
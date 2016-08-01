#ifndef _AS3ActionScriptVersion_
#define _AS3ActionScriptVersion_
namespace avmplus{namespace NativeID{
class ActionScriptVersionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//uint32_t ACTIONSCRIPT2 = 2;
	//uint32_t ACTIONSCRIPT3 = 3;

	uint32_t ACTIONSCRIPT2;
	uint32_t ACTIONSCRIPT3;
private:};
class ActionScriptVersionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ActionScriptVersionClass : public ClassClosure
	{
	public:
		ActionScriptVersionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ActionScriptVersionClassSlots m_slots_ActionScriptVersionClass;
	public:
		inline uint32_t getSlotACTIONSCRIPT2(){return m_slots_ActionScriptVersionClass.ACTIONSCRIPT2;}
		inline uint32_t getSlotACTIONSCRIPT3(){return m_slots_ActionScriptVersionClass.ACTIONSCRIPT3;}
};
class ActionScriptVersionObject : public ScriptObject
{
	public:
		ActionScriptVersionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ActionScriptVersionObjectSlots m_slots_ActionScriptVersionObject;
};}
#endif
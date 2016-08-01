#ifndef _AS3NativeDragManager_
#define _AS3NativeDragManager_
namespace avmplus{namespace NativeID{
class NativeDragManagerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeDragManagerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeDragManagerClass : public ClassClosure
	{
	public:
		NativeDragManagerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragManagerClassSlots m_slots_NativeDragManagerClass;
};
class NativeDragManagerObject : public ScriptObject
{
	public:
		NativeDragManagerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_dragInitiator_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_dropAction_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_dropAction_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isDragging_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isSupported_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragManagerObjectSlots m_slots_NativeDragManagerObject;
};}
#endif
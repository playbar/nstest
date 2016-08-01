#ifndef _AS3IEventDispatcher_
#define _AS3IEventDispatcher_
namespace avmplus{namespace NativeID{
class IEventDispatcherClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IEventDispatcherObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IEventDispatcherClass : public ClassClosure
	{
	public:
		IEventDispatcherClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IEventDispatcherClassSlots m_slots_IEventDispatcherClass;
};
class IEventDispatcherObject : public ScriptObject
{
	public:
		IEventDispatcherObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IEventDispatcherObjectSlots m_slots_IEventDispatcherObject;
};}
#endif
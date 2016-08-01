#ifndef _AS3IIMEClient_
#define _AS3IIMEClient_
namespace avmplus{namespace NativeID{
class IIMEClientClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IIMEClientObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IIMEClientClass : public ClassClosure
	{
	public:
		IIMEClientClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IIMEClientClassSlots m_slots_IIMEClientClass;
};
class IIMEClientObject : public ScriptObject
{
	public:
		IIMEClientObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IIMEClientObjectSlots m_slots_IIMEClientObject;
};}
#endif
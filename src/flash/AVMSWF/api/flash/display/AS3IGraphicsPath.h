#ifndef _AS3IGraphicsPath_
#define _AS3IGraphicsPath_
namespace avmplus{namespace NativeID{
class IGraphicsPathClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IGraphicsPathObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IGraphicsPathClass : public ClassClosure
	{
	public:
		IGraphicsPathClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsPathClassSlots m_slots_IGraphicsPathClass;
};
class IGraphicsPathObject : public ScriptObject
{
	public:
		IGraphicsPathObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsPathObjectSlots m_slots_IGraphicsPathObject;
};}
#endif
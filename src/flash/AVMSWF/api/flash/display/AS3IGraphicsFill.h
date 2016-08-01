#ifndef _AS3IGraphicsFill_
#define _AS3IGraphicsFill_
namespace avmplus{namespace NativeID{
class IGraphicsFillClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IGraphicsFillObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IGraphicsFillClass : public ClassClosure
	{
	public:
		IGraphicsFillClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsFillClassSlots m_slots_IGraphicsFillClass;
};
class IGraphicsFillObject : public ScriptObject
{
	public:
		IGraphicsFillObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsFillObjectSlots m_slots_IGraphicsFillObject;
};}
#endif
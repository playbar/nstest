#ifndef _AS3GraphicsEndFill_
#define _AS3GraphicsEndFill_
namespace avmplus{namespace NativeID{
class GraphicsEndFillClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsEndFillObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GraphicsEndFillClass : public ClassClosure
	{
	public:
		GraphicsEndFillClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsEndFillClassSlots m_slots_GraphicsEndFillClass;
};
class GraphicsEndFillObject : public ScriptObject
{
	public:
		GraphicsEndFillObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
//		void* pData;//Set your data!!
//		void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsEndFillObjectSlots m_slots_GraphicsEndFillObject;
};}
#endif
#ifndef _AS3GraphicsSolidFill_
#define _AS3GraphicsSolidFill_
namespace avmplus{namespace NativeID{
class GraphicsSolidFillClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsSolidFillObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	uint32_t color;
	double alpha;
private:};
}}
namespace avmshell{
	class GraphicsSolidFillClass : public ClassClosure
	{
	public:
		GraphicsSolidFillClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsSolidFillClassSlots m_slots_GraphicsSolidFillClass;
};
class GraphicsSolidFillObject : public ScriptObject
{
	public:
		GraphicsSolidFillObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_alpha_get();
		//void AS3_alpha_set(double alpha);
		//uint32_t AS3_color_get();
		//void AS3_color_set(uint32_t color);
		//void AS3_constructor(uint32_t color, double alpha);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsSolidFillObjectSlots m_slots_GraphicsSolidFillObject;
};}
#endif
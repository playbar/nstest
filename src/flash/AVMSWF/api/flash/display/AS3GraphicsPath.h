#ifndef _AS3GraphicsPath_
#define _AS3GraphicsPath_
namespace avmplus{namespace NativeID{
class GraphicsPathClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsPathObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	IntVectorObject *commands;
	DoubleVectorObject *data;
	Stringp _winding;
private:};
}}
namespace avmshell{
	class GraphicsPathClass : public ClassClosure
	{
	public:
		GraphicsPathClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsPathClassSlots m_slots_GraphicsPathClass;
};
class GraphicsPathObject : public ScriptObject
{
	public:
		GraphicsPathObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//AvmBox AS3_commands_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_commands_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_data_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_data_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//Stringp AS3_winding_get();
		//void AS3_winding_set(Stringp pWinding);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_curveTo(double controlX, double controlY, double anchorX, double anchorY);
		//void AS3_lineTo(double x, double y);
		//void AS3_moveTo(double x, double y);
		//void AS3_wideLineTo(double x, double y);
		//void AS3_wideMoveTo(double x, double y);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsPathObjectSlots m_slots_GraphicsPathObject;
};}
#endif
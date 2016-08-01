#ifndef _AS3GraphicsStroke_
#define _AS3GraphicsStroke_
namespace avmplus{namespace NativeID{
class GraphicsStrokeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsStrokeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double thickness;
	XBOOL pixelHinting;
	Stringp _caps;
	Stringp _joints;
	double miterLimit;
	Stringp _scaleMode;
//	IGraphicsFillObject *fill;
	int32_t fill;//place for IGraphicsFillObject *fill
private:};
}}
namespace avmshell{
	class GraphicsStrokeClass : public ClassClosure
	{
	public:
		GraphicsStrokeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsStrokeClassSlots m_slots_GraphicsStrokeClass;
};
class GraphicsStrokeObject : public ScriptObject
{
	public:
		GraphicsStrokeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Stringp AS3_caps_get();
		//void AS3_caps_set(Stringp pCaps);
		//AvmBox AS3_fill_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_fill_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//Stringp AS3_joints_get();
		//void AS3_joints_set(Stringp pJoints);
		//double AS3_miterLimit_get();
		//void AS3_miterLimit_set(double miterLimit);
		//bool AS3_pixelHinting_get();
		//void AS3_pixelHinting_set(bool pixelHinting);
		//Stringp AS3_scaleMode_get();
		//void AS3_scaleMode_set(Stringp pScaleMode);
		//double AS3_thickness_get();
		//void AS3_thickness_set(double thickness);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsStrokeObjectSlots m_slots_GraphicsStrokeObject;
};}
#endif
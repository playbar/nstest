#ifndef _AS3FontMetrics_
#define _AS3FontMetrics_
namespace avmplus{namespace NativeID{

//class RectangleObject;

class FontMetricsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FontMetricsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	void *emBox;
	double strikethroughOffset;
	double strikethroughThickness;
	double underlineOffset;
	double underlineThickness;
	double subscriptOffset;
	double subscriptScale;
	double superscriptOffset;
	double superscriptScale;
	double lineGap;
private:};
}}
namespace avmshell{
	class FontMetricsObject;
	class FontMetricsClass : public ClassClosure
	{
	public:
		FontMetricsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		FontMetricsObject* CreateObject(int nFontSize);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontMetricsClassSlots m_slots_FontMetricsClass;
};
class FontMetricsObject : public ScriptObject
{
	public:
		FontMetricsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void SetFontSize(int nFontSize);		
		//void* pData;//Set your data!!
		//AvmBox AS3_emBox_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_emBox_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_strikethroughOffset_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_strikethroughOffset_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_strikethroughThickness_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_strikethroughThickness_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_subscriptOffset_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_subscriptOffset_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_subscriptScale_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_subscriptScale_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_superscriptOffset_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_superscriptOffset_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_superscriptScale_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_superscriptScale_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_underlineOffset_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_underlineOffset_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_underlineThickness_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_underlineThickness_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontMetricsObjectSlots m_slots_FontMetricsObject;
};}
#endif
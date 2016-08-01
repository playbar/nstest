#ifndef _AS3CompositionAttributeRange_
#define _AS3CompositionAttributeRange_
namespace avmplus{namespace NativeID{
class CompositionAttributeRangeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CompositionAttributeRangeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	int32_t relativeStart;
	int32_t relativeEnd;
	XBOOL	selected;
	XBOOL	converted;
private:};
}}
namespace avmshell{
	class CompositionAttributeRangeClass : public ClassClosure
	{
	public:
		CompositionAttributeRangeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CompositionAttributeRangeClassSlots m_slots_CompositionAttributeRangeClass;
};
class CompositionAttributeRangeObject : public ScriptObject
{
	public:
		CompositionAttributeRangeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//AvmBox AS3_converted_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_converted_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_relativeEnd_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_relativeEnd_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_relativeStart_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_relativeStart_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_selected_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_selected_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CompositionAttributeRangeObjectSlots m_slots_CompositionAttributeRangeObject;
};}
#endif
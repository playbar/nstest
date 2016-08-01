#ifndef _AS3ShaderParameter_
#define _AS3ShaderParameter_
namespace avmplus{namespace NativeID{
class ShaderParameterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ShaderParameterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderParameterClass : public ClassClosure
	{
	public:
		ShaderParameterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderParameterClassSlots m_slots_ShaderParameterClass;
};
class ShaderParameterObject : public ScriptObject
{
	public:
		ShaderParameterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		int AS3_index_get();
		Stringp AS3_type_get();
		ArrayObject* AS3_value_get();
		void AS3_value_set(ArrayObject *pValue);

//		void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderParameterObjectSlots m_slots_ShaderParameterObject;
};}
#endif
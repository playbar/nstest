#ifndef _AS3ShaderInput_
#define _AS3ShaderInput_
namespace avmplus{namespace NativeID{
class ShaderInputClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ShaderInputObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderInputClass : public ClassClosure
	{
	public:
		ShaderInputClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderInputClassSlots m_slots_ShaderInputClass;
};
class ShaderInputObject : public ScriptObject
{
	public:
		ShaderInputObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		int AS3_channels_get();
		int AS3_height_get();
		void AS3_height_set(int height);
		int AS3_index_get();
		Atom AS3_input_get();
		void AS3_input_set(Atom input);
		int AS3_width_get();
		void AS3_width_set(int width);

		void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderInputObjectSlots m_slots_ShaderInputObject;
};}
#endif
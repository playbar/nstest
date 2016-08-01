#ifndef _AS3EastAsianJustifier_
#define _AS3EastAsianJustifier_
namespace avmplus{namespace NativeID{
class EastAsianJustifierClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class EastAsianJustifierObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class EastAsianJustifierObject;
	class EastAsianJustifierClass : public ClassClosure
	{
	public:
		EastAsianJustifierClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		EastAsianJustifierObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EastAsianJustifierClassSlots m_slots_EastAsianJustifierClass;
};
class EastAsianJustifierObject : public ScriptObject
{
	public:
		EastAsianJustifierObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_justificationStyle_get();
		void AS3_justificationStyle_set(Stringp justificationStyle);

		bool AS3_composeTrailingIdeographicSpaces_get(){return false;}
		void AS3_composeTrailingIdeographicSpaces_set(bool value){}
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EastAsianJustifierObjectSlots m_slots_EastAsianJustifierObject;
};}
#endif
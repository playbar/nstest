#ifndef _AS3SpaceJustifier_
#define _AS3SpaceJustifier_
namespace avmplus{namespace NativeID{
class SpaceJustifierClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SpaceJustifierObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SpaceJustifierObject;
	class SpaceJustifierClass : public ClassClosure
	{
	public:
		SpaceJustifierClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		SpaceJustifierObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SpaceJustifierClassSlots m_slots_SpaceJustifierClass;
};
class SpaceJustifierObject : public ScriptObject
{
	public:
		SpaceJustifierObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		bool AS3_letterSpacing_get();
		void AS3_letterSpacing_set(bool letterSpacing);
		double AS3_maximumSpacing_get();
		void AS3_maximumSpacing_set(double maximumSpacing);
		double AS3_minimumSpacing_get();
		void AS3_minimumSpacing_set(double minimumSpacing);
		double AS3_optimumSpacing_get();
		void AS3_optimumSpacing_set(double optimumSpacing);
		void AS3_cloneSpacing(SpaceJustifierObject*);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SpaceJustifierObjectSlots m_slots_SpaceJustifierObject;
};}
#endif
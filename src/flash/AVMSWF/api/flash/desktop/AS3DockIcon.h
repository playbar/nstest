#ifndef _AS3DockIcon_
#define _AS3DockIcon_
namespace avmplus{namespace NativeID{
class DockIconClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DockIconObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DockIconClass : public ClassClosure
	{
	public:
		DockIconClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DockIconClassSlots m_slots_DockIconClass;
};
class DockIconObject : public ScriptObject
{
	public:
		DockIconObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_bitmaps_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bitmaps_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menu_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menu_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bounce(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DockIconObjectSlots m_slots_DockIconObject;
};}
#endif
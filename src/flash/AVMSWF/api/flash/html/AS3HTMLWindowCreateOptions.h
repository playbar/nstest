#ifndef _AS3HTMLWindowCreateOptions_
#define _AS3HTMLWindowCreateOptions_
namespace avmplus{namespace NativeID{
class HTMLWindowCreateOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class HTMLWindowCreateOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTMLWindowCreateOptionsClass : public ClassClosure
	{
	public:
		HTMLWindowCreateOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLWindowCreateOptionsClassSlots m_slots_HTMLWindowCreateOptionsClass;
};
class HTMLWindowCreateOptionsObject : public ScriptObject
{
	public:
		HTMLWindowCreateOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_fullscreen_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_fullscreen_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_locationBarVisible_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_locationBarVisible_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menuBarVisible_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menuBarVisible_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_resizable_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_resizable_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_scrollBarsVisible_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_scrollBarsVisible_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_statusBarVisible_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_statusBarVisible_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_toolBarVisible_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_toolBarVisible_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_x_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_x_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_y_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_y_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLWindowCreateOptionsObjectSlots m_slots_HTMLWindowCreateOptionsObject;
};}
#endif
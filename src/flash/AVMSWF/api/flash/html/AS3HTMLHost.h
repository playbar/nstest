#ifndef _AS3HTMLHost_
#define _AS3HTMLHost_
namespace avmplus{namespace NativeID{
class HTMLHostClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class HTMLHostObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTMLHostClass : public ClassClosure
	{
	public:
		HTMLHostClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLHostClassSlots m_slots_HTMLHostClass;
};
class HTMLHostObject : public ScriptObject
{
	public:
		HTMLHostObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_htmlLoader_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_windowRect_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_windowRect_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_createWindow(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_updateLocation(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_updateStatus(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_updateTitle(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_windowBlur(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_windowClose(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_windowFocus(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLHostObjectSlots m_slots_HTMLHostObject;
};}
#endif
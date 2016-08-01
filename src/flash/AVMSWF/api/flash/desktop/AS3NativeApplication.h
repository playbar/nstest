#ifndef _AS3NativeApplication_
#define _AS3NativeApplication_
namespace avmplus{namespace NativeID{
class NativeApplicationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeApplicationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeApplicationClass : public ClassClosure
	{
	public:
		NativeApplicationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeApplicationClassSlots m_slots_NativeApplicationClass;
};
class NativeApplicationObject : public ScriptObject
{
	public:
		NativeApplicationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_nativeApplication_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_supportsDefaultApplication_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_supportsDockIcon_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_supportsMenu_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_supportsStartAtLogin_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_supportsSystemTrayIcon_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_activeWindow_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_applicationDescriptor_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_applicationID_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_autoExit_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_autoExit_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_icon_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_idleThreshold_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_idleThreshold_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menu_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menu_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_openedWindows_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_publisherID_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_runtimePatchLevel_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_runtimeVersion_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_startAtLogin_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_startAtLogin_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_systemIdleMode_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_systemIdleMode_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_timeSinceLastUserInput_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_activate(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_addEventListener(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_clear(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_copy(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cut(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_dispatchEvent(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_exit(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getDefaultApplication(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isSetAsDefaultApplication(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_paste(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_removeAsDefaultApplication(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_removeEventListener(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_selectAll(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_setAsDefaultApplication(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds 11
		bool m_bExecuteInBackground;
		bool AS3_executeInBackground_get();
		void AS3_executeInBackground_set(bool bIBK);
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeApplicationObjectSlots m_slots_NativeApplicationObject;
};}
#endif
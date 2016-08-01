#ifndef _AS3HTMLLoader_
#define _AS3HTMLLoader_
namespace avmplus{namespace NativeID{
class HTMLLoaderClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class HTMLLoaderObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTMLLoaderClass : public ClassClosure
	{
	public:
		HTMLLoaderClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLLoaderClassSlots m_slots_HTMLLoaderClass;
};
class HTMLLoaderObject : public ScriptObject
{
	public:
		HTMLLoaderObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_isSupported_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_pdfCapability_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_swfCapability_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_createRootWindow(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_authenticate_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_authenticate_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cacheResponse_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cacheResponse_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_contentHeight_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_contentWidth_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_hasFocusableContent_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_historyLength_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_historyPosition_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_historyPosition_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_htmlHost_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_htmlHost_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_idleTimeout_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_idleTimeout_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_loaded_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_location_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_manageCookies_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_manageCookies_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_navigateInSystemBrowser_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_navigateInSystemBrowser_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_paintsDefaultBackground_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_paintsDefaultBackground_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_placeLoadStringContentInApplicationSandbox_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_placeLoadStringContentInApplicationSandbox_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_runtimeApplicationDomain_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_runtimeApplicationDomain_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_scrollH_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_scrollH_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_scrollV_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_scrollV_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_textEncodingFallback_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_textEncodingFallback_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_textEncodingOverride_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_textEncodingOverride_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_useCache_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_useCache_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_userAgent_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_userAgent_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_window_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cancelLoad(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getHistoryAt(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_historyBack(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_historyForward(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_historyGo(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_load(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_loadString(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_reload(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLLoaderObjectSlots m_slots_HTMLLoaderObject;
};}
#endif
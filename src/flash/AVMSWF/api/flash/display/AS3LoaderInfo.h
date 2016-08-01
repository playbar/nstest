#ifndef _AS3LoaderInfo_
#define _AS3LoaderInfo_

#include "AS3EventDispatcher.h"
#include "AS3ByteArray.h"
#include "AS3UncaughtErrorEvents.h"

namespace avmplus{namespace NativeID{
class LoaderInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class LoaderInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LoaderInfoObject;
	class DisplayObjectObject;
	class LoaderObject;
	class EventDispatcherObject;
	class ApplicationDomainObject;
	class ShellCodeContext;
	class ByteArrayObject;
	class LoaderInfoClass : public ClassClosure
	{
	public:
		LoaderInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		avmshell::LoaderInfoObject* CreateLoaderInfo(void*,ShellCodeContext*);

	//new adds
	LoaderInfoObject* AS3_getLoaderInfoByDefinition(Atom object);
	//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoaderInfoClassSlots m_slots_LoaderInfoClass;
};
class LoaderInfoObject : public EventDispatcherObject
{
	public:
		LoaderInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);		
		virtual void Release();
		void ClearEventInfo();
	public:
		//DRCWB(Stringp) m_strLoaderURL;
		//DRCWB(Stringp) m_strURL;
		DRCWB(ScriptObject*) m_params;
		DRCWB(LoaderInfoObject*) m_loadBy;
		DRCWB(ByteArrayObject*) m_bytes;
		DRCWB(UncaughtErrorEventsObject*) m_pErrorEvent;
		//DRCWB(ShellCodeContext*) m_pCodeContext;
		//DRCWB(ApplicationDomainObject*)		m_pAppDomain;
		//DRCWB(SecurityDomainObject*)		m_pSecurityDomain;
		LoaderObject*  m_pLoader;
		void Reset(bool bClear);
		//LoaderObject*  m_pLoader;
		virtual EventDispatcherObject* GetParentObject(bool bSeek);
		class ScriptPlayer*m_pInfoPlayer;
		void PushPlayer(ScriptPlayer*player);
		//void* pData;//Set your data!!
		uint32 AS3_actionScriptVersion_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		ApplicationDomainObject* AS3_applicationDomain_get();
		uint32 AS3_bytesLoaded_get();
		uint32 AS3_bytesTotal_get();
		ByteArrayObject* AS3_bytes_get();
		bool AS3_childAllowsParent_get();
		DisplayObjectObject* AS3_content_get();
		Stringp AS3_contentType_get();
		double AS3_frameRate_get();
		int AS3_height_get();
		LoaderObject* AS3_loader_get();
		Stringp AS3_loaderURL_get();
		AvmBox AS3_parameters_get();
		bool AS3_parentAllowsChild_get();
		bool AS3_sameDomain_get();
		EventDispatcherObject* AS3_sharedEvents_get();
		uint32 AS3_swfVersion_get();
		Stringp AS3_url_get();
		int AS3_width_get();

		//new adds
//		ByteArrayObject* AS3_bytes_get();
		Atom AS3_childSandboxBridge_get();
		void AS3_childSandboxBridge_set(Atom childSandboxBridge);
		bool AS3_isURLInaccessible_get();
		Atom AS3_parentSandboxBridge_get();
		void AS3_parentSandboxBridge_set(Atom parentSandboxBridge);
		UncaughtErrorEventsObject* AS3_uncaughtErrorEvents_get();
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoaderInfoObjectSlots m_slots_LoaderInfoObject;
};}
#endif
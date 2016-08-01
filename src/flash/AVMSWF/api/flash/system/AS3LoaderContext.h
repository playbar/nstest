#ifndef _AS3LoaderContext_
#define _AS3LoaderContext_

#include "AS3DisplayObjectContainer.h"

namespace avmshell
{
	class ApplicationDomainObject;
	class SecurityDomainObject;
};

namespace avmplus{namespace NativeID{
class LoaderContextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class LoaderContextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	XBOOL checkPolicyFile;
	avmshell::ApplicationDomainObject* applicationDomain;//指定用于 Loader.load() 或 Loader.loadBytes() 方法的应用程序域。
	
	avmshell::SecurityDomainObject* securityDomain;
private:};
}}
namespace avmshell{
	class ApplicationDomainObject;
//	class SecurityDomain;	-->del by dsx
	class LoaderContextClass : public ClassClosure
	{
	public:
		LoaderContextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoaderContextClassSlots m_slots_LoaderContextClass;
};
class LoaderContextObject : public ScriptObject
{
	public:
		LoaderContextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		inline ApplicationDomainObject* GetApplicationDomain()
		{ return m_slots_LoaderContextObject.applicationDomain; }
		inline bool GetCheckPolicyFile()
		{ return m_slots_LoaderContextObject.checkPolicyFile!=0; }
		inline SecurityDomainObject* GetSecurityDomain()
		{	return m_slots_LoaderContextObject.securityDomain;	}
		void Init(ApplicationDomainObject*a,bool b,SecurityDomainObject*s)
		{
			m_slots_LoaderContextObject.applicationDomain=a;
			m_slots_LoaderContextObject.checkPolicyFile=b;
			m_slots_LoaderContextObject.securityDomain=s;
		}
		void SetApplication(ApplicationDomainObject*pApp)
		{
			WBRC(gc(),&m_slots_LoaderContextObject,&m_slots_LoaderContextObject.applicationDomain,pApp);
		}
		//new adds
		bool AS3_allowCodeImport_get();
		void AS3_allowCodeImport_set(bool allowCodeImport);
		bool AS3_allowLoadBytesCodeExecution_get();
		void AS3_allowLoadBytesCodeExecution_set(bool allowLoadBytesCodeExecution);
		Stringp AS3_imageDecodingPolicy_get();
		void AS3_imageDecodingPolicy_set(Stringp pImageDecodingPolicy);
		Atom AS3_parameters_get();
		void AS3_parameters_set(Atom parameters);
		DisplayObjectContainerObject* AS3_requestedContentParent_get();
		void AS3_requestedContentParent_set(DisplayObjectContainerObject *pRequestedContentParent);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoaderContextObjectSlots m_slots_LoaderContextObject;
};}
#endif
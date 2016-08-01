#ifndef _AS3URLRequest_
#define _AS3URLRequest_

#include "AS3URLRequestHeader.h"
#include "AS3URLRequestMethod.h"
#include "AS3URLRequestDefaults.h"

#include "RCHashTable.h"

namespace avmplus{namespace NativeID{
class URLRequestClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class URLRequestObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class URLRequestObject;
	class URLRequestClass : public ClassClosure
	{
	public:
		URLRequestClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		//DWB(EventHashTable*) m_aliasTable;
		//static ClassClosure* getClassByAlias(AvmMethodEnv,Stringp);
		static void navigateToURL(AvmMethodEnv,URLRequestObject*,Stringp);
		static void registerClassAlias(AvmMethodEnv,Stringp,ClassClosure*);
		static void sendToURL(AvmMethodEnv,URLRequestObject*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestClassSlots m_slots_URLRequestClass;
};
class URLRequestObject : public ScriptObject
{
	public:
		URLRequestObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		XU8 m_nMethod;
		DRCWB(Atom)			 m_pData;
		DRCWB(Stringp)		 m_strURL;
		DRCWB(Stringp)		 m_strContext;
		DRCWB(Stringp)		 m_strDigest;
		DRCWB(ArrayObject*)  m_headers;

		//new adds
		//DRCWB(Stringp) m_userAgent;
		//double m_idleTimeout;

		//bool m_authenticate;
		//bool m_cacheResponse;
		//bool m_followRedirects;
		//bool m_manageCookies;
		//bool m_useCache;
		//new adds end

		Stringp AS3_contentType_get()
		{	return m_strContext;}
		void AS3_contentType_set(Stringp strContext)
		{
			m_strContext=strContext;
		}
		Atom AS3_data_get()
		{
			return m_pData;//m_pData?m_pData->atom():nullObjectAtom;
		}
		void AS3_data_set(Atom a)
		{
			m_pData=a;//core()->atomToScriptObject(a);
		}
		Stringp AS3_method_get();		
		void AS3_method_set(Stringp);
		ArrayObject* AS3_requestHeaders_get();
		void AS3_requestHeaders_set(ArrayObject*pHeaders)
		{
			m_headers=pHeaders;
		}
		Stringp AS3_url_get(){return m_strURL;}
		Stringp AS3_digest_get()
		{
			if(m_strDigest) return m_strDigest;
			return m_strURL;
		}
		void AS3_digest_set(Stringp s)
		{
			m_strDigest=s;
		}
		void AS3_url_set(Stringp url)
		{
			m_strURL=url;
		};
		Stringp GetPostData(XU8Array&datas,bool bFileRef);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds
		bool AS3_authenticate_get()
		{
			return m_pURLRequestDefaultsClass->AS3_authenticate_get();
		}
		void AS3_authenticate_set(bool authenticate)
		{
			m_pURLRequestDefaultsClass->AS3_authenticate_set(authenticate);
		}
		bool AS3_cacheResponse_get()
		{
			return m_pURLRequestDefaultsClass->AS3_cacheResponse_get();
		}
		void AS3_cacheResponse_set(bool cacheResponse)
		{
			m_pURLRequestDefaultsClass->AS3_cacheResponse_set(cacheResponse);
		}
		//AvmBox AS3_digest_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_digest_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_followRedirects_get()
		{
			return m_pURLRequestDefaultsClass->AS3_followRedirects_get();
		}
		void AS3_followRedirects_set(bool followRedirects)
		{
			m_pURLRequestDefaultsClass->AS3_followRedirects_set(followRedirects);
		}
		double AS3_idleTimeout_get()
		{
			return m_pURLRequestDefaultsClass->AS3_idleTimeout_get();
		}
		void AS3_idleTimeout_set(double idleTimeout)
		{
			m_pURLRequestDefaultsClass->AS3_idleTimeout_set(idleTimeout);
		}
		bool AS3_manageCookies_get()
		{
			return m_pURLRequestDefaultsClass->AS3_manageCookies_get();
		}
		void AS3_manageCookies_set(bool manageCookies)
		{
			m_pURLRequestDefaultsClass->AS3_manageCookies_set(manageCookies);
		}
		bool AS3_useCache_get()
		{
			return m_pURLRequestDefaultsClass->AS3_useCache_get();
		}
		void AS3_useCache_set(bool useCache)
		{
			m_pURLRequestDefaultsClass->AS3_useCache_set(useCache);
		}
		Stringp AS3_userAgent_get()
		{
			return m_pURLRequestDefaultsClass->AS3_userAgent_get();
		}
		void AS3_userAgent_set(Stringp userAgent)
		{
			m_pURLRequestDefaultsClass->AS3_userAgent_set(userAgent);
		}
		//new adds end

		//new adds 11
 		void AS3_useRedirectedURL(URLRequestObject* pSourceRequest, bool wholeURL = false, void* pattern = NULL, Stringp sReplace = NULL);
 		
 		//new adds 11 end
	private:
		URLRequestDefaultsClass * const m_pURLRequestDefaultsClass;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestObjectSlots m_slots_URLRequestObject;
};}
#endif
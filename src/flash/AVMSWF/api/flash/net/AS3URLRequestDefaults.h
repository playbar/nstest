#ifndef _AS3URLRequestDefaults_
#define _AS3URLRequestDefaults_
namespace avmplus{namespace NativeID{
class URLRequestDefaultsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class URLRequestDefaultsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class URLRequestDefaultsClass : public ClassClosure
	{
	public:
		URLRequestDefaultsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		bool AS3_authenticate_get();
		void AS3_authenticate_set(bool  authenticate);
		bool AS3_cacheResponse_get();
		void AS3_cacheResponse_set(bool cacheResponse);
		bool AS3_followRedirects_get();
		void AS3_followRedirects_set(bool followRedirects);
		double AS3_idleTimeout_get();
		void AS3_idleTimeout_set(double idleTimeout);
		bool AS3_manageCookies_get();
		void AS3_manageCookies_set(bool manageCookies);
		bool AS3_useCache_get();
		void AS3_useCache_set(bool useCache);
		Stringp AS3_userAgent_get();
		void AS3_userAgent_set(Stringp pUserAgent);

		Atom AS3_setLoginCredentialsForHost(Stringp pHostname, Stringp pUser, Stringp pPassword);

	private:
		DRCWB(Stringp) m_userAgent;
		double m_idleTimeout;

		bool m_authenticate;
		bool m_cacheResponse;
		bool m_followRedirects;
		bool m_manageCookies;
		bool m_useCache;

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestDefaultsClassSlots m_slots_URLRequestDefaultsClass;
};
class URLRequestDefaultsObject : public ScriptObject
{
	public:
		URLRequestDefaultsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestDefaultsObjectSlots m_slots_URLRequestDefaultsObject;
};}
#endif
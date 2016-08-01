#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3URLRequestDefaults.h"
namespace avmshell{
URLRequestDefaultsClass::URLRequestDefaultsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* URLRequestDefaultsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) URLRequestDefaultsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
URLRequestDefaultsObject::URLRequestDefaultsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool URLRequestDefaultsClass::AS3_authenticate_get()
{
	return m_authenticate;
}
void URLRequestDefaultsClass::AS3_authenticate_set(bool  authenticate)
{
	m_authenticate =authenticate;
}
bool URLRequestDefaultsClass::AS3_cacheResponse_get()
{
	return m_cacheResponse;
}
void URLRequestDefaultsClass::AS3_cacheResponse_set(bool cacheResponse)
{
	m_cacheResponse = cacheResponse;
}
bool URLRequestDefaultsClass::AS3_followRedirects_get()
{
	return m_followRedirects;
}
void URLRequestDefaultsClass::AS3_followRedirects_set(bool followRedirects)
{
	m_followRedirects = followRedirects;
}
double URLRequestDefaultsClass::AS3_idleTimeout_get()
{
	return m_idleTimeout;
}
void URLRequestDefaultsClass::AS3_idleTimeout_set(double idleTimeout)
{
	m_idleTimeout = idleTimeout;
}
bool URLRequestDefaultsClass::AS3_manageCookies_get()
{
	return m_manageCookies;
}
void URLRequestDefaultsClass::AS3_manageCookies_set(bool manageCookies)
{
	m_manageCookies = manageCookies;
}
bool URLRequestDefaultsClass::AS3_useCache_get()
{
	return m_useCache;
}
void URLRequestDefaultsClass::AS3_useCache_set(bool useCache)
{
	m_useCache = useCache;
}
Stringp URLRequestDefaultsClass::AS3_userAgent_get()
{
	return m_userAgent;
}
void URLRequestDefaultsClass::AS3_userAgent_set(Stringp pUserAgent)
{
	m_userAgent = pUserAgent;
}

Atom URLRequestDefaultsClass::AS3_setLoginCredentialsForHost(Stringp pHostname, Stringp pUser, Stringp pPassword)
{
	return kAvmThunkUndefined;
}

}